"""Land pull requests on main one at a time.

A pull request lands only when its required checks passed on its exact head
AND were measured against the main commit that is current when it merges.
.github/workflows/landing.yml runs this file in one job for the whole
repository, so nothing else moves main while it works.

  lander.py select DIR   choose one pull request, refresh a stale measurement,
                         record the exact runs whose receipts must be downloaded
  lander.py land DIR     verify the receipts, re-read main, merge with the
                         expected head, verify the new main commit, report

Receipt and run checks are the protected ones in ci/managed-check/landing_read.py,
imported, not copied.
"""
import inspect
import json
import os
from pathlib import Path
import re
import sys
from concurrent.futures import ThreadPoolExecutor
import threading
import datetime
import time

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
sys.path.insert(1, str(HERE.parent / "managed-check"))

import landing_read as lr  # noqa: E402
from common import CONTEXT as BOAT, HEX40, REPO, REPO_ID, read  # noqa: E402
from inline_status import CONTEXT as INLINE  # noqa: E402
from github_status import (  # noqa: E402  (the compiler check's own admission & disjoint-TU rules)
    BUILD_NEUTRAL_PREFIXES,
    BUILD_NEUTRAL_WORKFLOWS,
    DISJOINT_TU_PREFIXES,
    DISJOINT_TU_SUFFIXES,
    MAX_ANCESTOR_AHEAD,
    admitted,
    is_build_neutral_path as _is_build_neutral_path,
    is_disjoint_tu_path as _is_disjoint_tu_path,
)
from api import Api, ApiError, ReadOnly, printable  # noqa: E402

RULES = lr.RULES

# --- Policy -----------------------------------------------------------------
# "report-only": Inline virtual must not be failure on current main; pending or
# missing is allowed. "required": it must be success like the other two.
INLINE_VIRTUAL_POLICY = "report-only"
REQUIRED = (BOAT, RULES)
REFUSED_BRANCH_PREFIXES = ("canary/", "ci-corpus/", "corpus/")
MERGE_METHOD = "squash"            # one commit per pull request, credited to its author
LANDING_CONTEXT = "BFME II / Landing"
HOLD_LABEL = "landing-hold"
REMEASURE_EVENT = "bfme-remeasure"  # both check workflows measure the PR on current main
LANDED_EVENT = "bfme-landed"        # state and progress workflows run after a landing
NEXT_EVENT = "bfme-land-next"       # this workflow takes the next pull request
ADMIT_REQUEST_EVENT = "bfme-admit-request"  # admission.yml reserves and starts a measurement of a fork
SOURCE_RECHECK_EVENT = "bfme-source-recheck"  # hosted G1-G8 source rules only (0 Boat VMs)
MEASURE_WAIT_SECONDS = 1800         # wide changes measured 744 to 978 s
LAND_SETTLE_SECONDS = 180           # bounded wait for just-completed runs on stage == land
MAX_PARALLEL_REMEASURES = 5         # fan out stale-ready PRs without holding the global lock
POLL_SECONDS = 20
MAX_OPEN_PULLS = 1000
HEADER_PRIORITY_WAIT_SECONDS = 720
TRAILER = "Landed by the BFME II landing workflow"
# Reconciliation reads only the body this workflow writes (after the title, which a pull request author chooses).
MAIN_MARKER_RE = re.compile(r"\(main ([0-9a-f]{12})\)")
MEASURED_LINE = re.compile(r"Measured on main ([0-9a-f]{40}), head ([0-9a-f]{40}), tree ([0-9a-f]{40})\.")
NUMBER_LINE = re.compile(r"Pull request #([1-9][0-9]{0,8})\.")
TRAILER_LINE = re.compile(re.escape(TRAILER) + r", run [0-9]{1,20}\.")


class Held(Exception):
    """A named reason this run does not merge. final=True means the author or a maintainer must act."""

    def __init__(self, code, expected, found, final=False):
        super().__init__(code)
        self.code, self.expected, self.found, self.final = code, expected, found, final

    def line(self):
        return f"{self.code}: Expected {self.expected}. Found {self.found}."


class Failed(Held):
    """Something went wrong after main moved, or the inputs are unusable. The job fails."""


def load_config(path=HERE / "landing.json"):
    data = json.loads(Path(path).read_text(encoding="utf-8"))
    approvers = data.get("header_approvers")
    if not isinstance(approvers, list) or any(type(item) is not int or item <= 0 for item in approvers):
        raise Failed("config", "header_approvers as a list of numeric GitHub account ids", "something else")
    exceptions = set()
    for item in data.get("rehearsal_exceptions", []):
        if type(item.get("number")) is not int or not HEX40.fullmatch(item.get("head", "")):
            raise Failed("config", "rehearsal_exceptions entries with a number and a 40-character head", "something else")
        exceptions.add((item["number"], item["head"]))
    if data.get("remeasure") not in {"repository_dispatch", "off"}:
        raise Failed("config", 'remeasure "repository_dispatch" or "off"', repr(data.get("remeasure")))
    return {"approvers": set(approvers), "exceptions": exceptions, "remeasure": data["remeasure"]}


def marker(main):
    return f"(main {main[:12]})"


def current_main(api):
    sha = (api.call("git/ref/heads/main").get("object") or {}).get("sha", "")
    if not HEX40.fullmatch(sha):
        raise Failed("main-unreadable", "the main branch commit", "missing or malformed metadata")
    return sha


def read_statuses_all(api, head):
    """Read all commit statuses once, returning (latest_any, latest_bot) maps by context."""
    latest, latest_bot = {}, {}
    for page in range(1, 11):
        entries = api.call(f"commits/{head}/statuses?per_page=100&page={page}")
        if not isinstance(entries, list):
            raise Failed("status-list", "GitHub's status list", "a different response")
        for item in entries:
            if isinstance(item, dict) and isinstance(item.get("context"), str) and isinstance(item.get("id"), int):
                context = item["context"]
                if context not in latest or item["id"] > latest[context]["id"]:
                    latest[context] = item
                if (item.get("creator") or {}).get("id") == lr.BOT_ID:
                    if context not in latest_bot or item["id"] > latest_bot[context]["id"]:
                        latest_bot[context] = item
        if len(entries) < 100:
            break
    return latest, latest_bot


def read_statuses(api, head):
    latest, _ = read_statuses_all(api, head)
    return latest

def classify(status, main):
    """fresh-* = measured on this main; stale-* = measured on another main."""
    if not status:
        return "missing"
    state = status.get("state")
    bound = marker(main) in (status.get("description") or "")
    if (status.get("creator") or {}).get("id") != lr.BOT_ID:
        return "untrusted"
    if state == "pending":
        return "pending"
    if state not in {"success", "failure", "error"}:
        return "missing"
    kind = "success" if state == "success" else ("error" if state == "error" else "failure")
    return ("fresh-" if bound else "stale-") + kind


ADMISSION_CONTEXT = "BFME II / Admission"
ADMITTED_BY_RULE = "Admitted by rule:"
HEADER_RULES_CONTEXT = "BFME II / Header rules"   # H1-H8, written by the header route on the exact head
LEDGER_REQUEST_LABEL = "ledger-request"
PROTECTED_PATHS = ("ci/", "tools/", "config/", "expected/", "baseline/")


def latest_bot_status(api, head, context):
    entries = api.call(f"commits/{head}/statuses?per_page=100&page=1")
    found = [item for item in entries if isinstance(item, dict) and item.get("context") == context
             and (item.get("creator") or {}).get("id") == lr.BOT_ID]
    return max(found, key=lambda item: item["id"]) if found else None


def admission_status(api, head):
    """The latest BFME II / Admission status on this exact commit written by GitHub Actions, or None."""
    entries = api.call(f"commits/{head}/statuses?per_page=100&page=1")
    found = [item for item in entries if isinstance(item, dict) and item.get("context") == ADMISSION_CONTEXT
             and (item.get("creator") or {}).get("id") == lr.BOT_ID]
    return max(found, key=lambda item: item["id"]) if found else None


MAX_ANCESTOR_AGE_SECONDS = 20 * 3600


def _status_fresh_enough(status_obj, max_age_seconds, now=None):
    if not isinstance(status_obj, dict):
        return False
    ts = status_obj.get("created_at") or ""
    if not ts:
        return True
    try:
        created = datetime.datetime.fromisoformat(ts.replace("Z", "+00:00"))
    except ValueError:
        return False
    ref_now = now if now is not None else datetime.datetime.now(datetime.timezone.utc)
    return (ref_now - created).total_seconds() <= max_age_seconds


def changed_files_detail(api, number):
    names = []
    all_simple_tu = True
    for page in range(1, 31):
        entries = api.call(f"pulls/{number}/files?per_page=100&page={page}")
        if not isinstance(entries, list):
            raise Failed("file-list", "the pull request's changed file list", "a different response")
        for item in entries:
            fname = item.get("filename", "")
            names.append(fname)
            if item.get("previous_filename"):
                names.append(item["previous_filename"])
                all_simple_tu = False
            elif item.get("status") not in {"modified", "added"} or not _is_disjoint_tu_path(fname):
                all_simple_tu = False
        if len(entries) < 100:
            return names, bool(names) and all_simple_tu
    raise Held("too-many-files", "at most 3000 changed files", "more", final=True)


def changed_files(api, number):
    names, _ = changed_files_detail(api, number)
    return names


def extract_main_prefix(status_obj):
    if not status_obj or (status_obj.get("creator") or {}).get("id") != lr.BOT_ID:
        return None
    m = MAIN_MARKER_RE.search(status_obj.get("description") or "")
    return m.group(1) if m else None


def resolve_disjoint_ancestor(api, prefix12, main, pr_files, compare_cache=None, cache_lock=None):
    """Return full 40-hex ancestor SHA iff main..ancestor only modified disjoint src/Code/*.c(pp) files."""
    if not prefix12 or not re.fullmatch(r"[0-9a-f]{12}", prefix12) or not pr_files:
        return None
    if not all(_is_disjoint_tu_path(name) for name in pr_files):
        return None
    cache_key = (prefix12, main)
    cmp = None
    hit = False
    if compare_cache is not None:
        if cache_lock is not None:
            with cache_lock:
                if cache_key in compare_cache:
                    cmp, hit = compare_cache[cache_key], True
        elif cache_key in compare_cache:
            cmp, hit = compare_cache[cache_key], True
    if not hit:
        try:
            cmp = api.call(f"compare/{prefix12}...{main}")
        except ApiError:
            cmp = None
        if compare_cache is not None:
            if cache_lock is not None:
                with cache_lock:
                    compare_cache[cache_key] = cmp
            else:
                compare_cache[cache_key] = cmp
    if not isinstance(cmp, dict):
        return None
    if cmp.get("status") != "ahead" or cmp.get("behind_by") != 0:
        return None
    ahead_by = cmp.get("ahead_by")
    if type(ahead_by) is not int or not 1 <= ahead_by <= MAX_ANCESTOR_AHEAD:
        return None
    base_sha = (cmp.get("base_commit") or {}).get("sha", "")
    merge_base_sha = (cmp.get("merge_base_commit") or {}).get("sha", "")
    if not HEX40.fullmatch(base_sha) or not base_sha.startswith(prefix12) or merge_base_sha != base_sha:
        return None
    intervening = cmp.get("files")
    if not isinstance(intervening, list) or not (0 < len(intervening) < 300):
        return None
    intervening_lower = set()
    for item in intervening:
        if not isinstance(item, dict):
            return None
        fname = item.get("filename", "")
        prev = item.get("previous_filename")
        if _is_build_neutral_path(fname) and (not prev or _is_build_neutral_path(prev)):
            continue
        if (item.get("status") != "modified"
                or prev
                or not _is_disjoint_tu_path(fname)):
            return None
        intervening_lower.add(fname.lower())
    pr_lower = {name.lower() for name in pr_files}
    if intervening_lower & pr_lower:
        return None
    return base_sha


def blob_sha_in_tree(api, tree_sha, rel_path, tree_cache=None):
    """Walk git/trees/{sha} path segments to find the exact blob SHA of rel_path in tree_sha."""
    if not HEX40.fullmatch(tree_sha) or not rel_path or rel_path.startswith("/") or ".." in rel_path.split("/"):
        return None
    parts = rel_path.split("/")
    cur = tree_sha
    for idx, seg in enumerate(parts):
        if tree_cache is not None and cur in tree_cache:
            entries = tree_cache[cur]
        else:
            doc = api.call(f"git/trees/{cur}")
            if not isinstance(doc, dict) or doc.get("truncated") is True or not isinstance(doc.get("tree"), list):
                return None
            entries = {item.get("path"): item for item in doc["tree"] if isinstance(item, dict)}
            if tree_cache is not None:
                tree_cache[cur] = entries
        entry = entries.get(seg)
        if not entry:
            return None
        want_type = "blob" if idx == len(parts) - 1 else "tree"
        if entry.get("type") != want_type or not HEX40.fullmatch(entry.get("sha", "")):
            return None
        cur = entry["sha"]
    return cur


OPEN_PULLS_QUERY = """query($owner: String!, $name: String!, $cursor: String) {
  repository(owner: $owner, name: $name) {
    pullRequests(states: OPEN, baseRefName: "main", orderBy: {field: CREATED_AT, direction: ASC}, first: 50, after: $cursor) {
      pageInfo { hasNextPage endCursor }
      nodes {
        number
        id
        title
        state
        isDraft
        authorAssociation
        mergeable
        baseRefName
        headRefName
        headRefOid
        author { login ... on User { databaseId } ... on Bot { databaseId } }
        headRepository { databaseId }
        baseRepository { databaseId }
        labels(first: 50) { nodes { name } }
        files(first: 100) { totalCount nodes { path changeType } }
        reviews(first: 30) {
          totalCount
          nodes {
            state
            commit { oid }
            author { ... on User { databaseId } ... on Bot { databaseId } }
          }
        }
        commits(last: 1) {
          nodes {
            commit {
              oid
              status {
                contexts {
                  context
                  state
                  description
                  targetUrl
                  createdAt
                  creator { ... on User { databaseId } ... on Bot { databaseId } }
                }
              }
            }
          }
        }
      }
    }
  }
}"""


def _gql_to_prefetched(node):
    if not isinstance(node, dict) or not isinstance(node.get("number"), int):
        return None
    mergeable_map = {"MERGEABLE": True, "CONFLICTING": False, "UNKNOWN": None}
    head_repo_id = (node.get("headRepository") or {}).get("databaseId")
    base_repo_id = (node.get("baseRepository") or {}).get("databaseId")
    pr = {
        "number": node["number"],
        "node_id": node.get("id"),
        "title": node.get("title") or "",
        "state": (node.get("state") or "").lower(),
        "draft": node.get("isDraft"),
        "author_association": node.get("authorAssociation"),
        "mergeable": mergeable_map.get(node.get("mergeable")),
        "user": {"login": (node.get("author") or {}).get("login"), "id": (node.get("author") or {}).get("databaseId")},
        "base": {"ref": node.get("baseRefName") or "", "repo": {"id": base_repo_id} if base_repo_id else {}},
        "head": {
            "sha": node.get("headRefOid") or "",
            "ref": node.get("headRefName") or "",
            "repo": {"id": head_repo_id} if head_repo_id else None,
        },
        "labels": [{"name": item["name"]} for item in ((node.get("labels") or {}).get("nodes") or ())
                   if isinstance(item, dict) and item.get("name")],
    }
    statuses, bot_statuses = {}, {}
    commit_nodes = (node.get("commits") or {}).get("nodes") or ()
    commit_obj = (commit_nodes[0].get("commit") if commit_nodes else None) or {}
    commit_oid = commit_obj.get("oid")
    status_obj = (commit_obj.get("status") or {}) if (not commit_oid or commit_oid == node.get("headRefOid")) else {}
    for idx, ctx in enumerate(status_obj.get("contexts") or ()):
        if not isinstance(ctx, dict) or not isinstance(ctx.get("context"), str):
            continue
        creator_id = (ctx.get("creator") or {}).get("databaseId")
        item = {
            "id": idx + 1,
            "context": ctx["context"],
            "state": (ctx.get("state") or "").lower(),
            "description": ctx.get("description") or "",
            "target_url": ctx.get("targetUrl") or "",
            "created_at": ctx.get("createdAt") or "",
            "creator": {"id": creator_id} if creator_id is not None else {},
        }
        statuses[ctx["context"]] = item
        if creator_id == lr.BOT_ID:
            bot_statuses[ctx["context"]] = item
    files_node = node.get("files")
    files, all_simple_tu = None, False
    if isinstance(files_node, dict):
        total = files_node.get("totalCount")
        fnodes = files_node.get("nodes") or ()
        if isinstance(total, int) and total <= 100 and len(fnodes) == total:
            if not any(isinstance(f, dict) and f.get("changeType") == "RENAMED" for f in fnodes):
                files = [f.get("path", "") for f in fnodes if isinstance(f, dict)]
                all_simple_tu = bool(files) and all(
                    isinstance(f, dict)
                    and f.get("changeType") in {"MODIFIED", "ADDED"}
                    and _is_disjoint_tu_path(f.get("path", ""))
                    for f in fnodes
                )
    reviews_node = node.get("reviews")
    reviews = None
    if isinstance(reviews_node, dict):
        rtotal = reviews_node.get("totalCount")
        rnodes = reviews_node.get("nodes") or ()
        if isinstance(rtotal, int) and 0 <= rtotal <= 30 and len(rnodes) == rtotal and all(isinstance(r, dict) for r in rnodes):
            reviews = [
                {
                    "id": idx + 1,
                    "state": (r.get("state") or "").upper(),
                    "commit_id": (r.get("commit") or {}).get("oid") or "",
                    "user": {"id": (r.get("author") or {}).get("databaseId")},
                }
                for idx, r in enumerate(rnodes)
            ]
    return {
        "number": node["number"],
        "pr": pr,
        "statuses": statuses,
        "bot_statuses": bot_statuses,
        "files": files,
        "all_simple_tu": all_simple_tu,
        "reviews": reviews,
    }


def _maintainer_review_from_records(records, pr, head, allow):
    """Evaluate listed-maintainer approval on exact head from prefetched review records (0 REST calls)."""
    if not allow:
        return False
    author_id = (pr.get("user") or {}).get("id")
    latest = {}
    for idx, item in enumerate(records):
        actor = (item.get("user") or {}).get("id")
        if type(actor) is not int or actor not in allow or actor == author_id:
            continue
        if item.get("state") in {"APPROVED", "CHANGES_REQUESTED", "DISMISSED"}:
            rid = item.get("id", idx)
            if actor not in latest or rid > latest[actor][0]:
                latest[actor] = (rid, item["state"] == "APPROVED" and item.get("commit_id") == head)
    return any(approved for _, approved in latest.values())


def fetch_open_pulls_batch(api):
    """Fetch up to MAX_OPEN_PULLS open PRs + statuses + files via GraphQL (50/call). Returns None on fallback."""
    if not hasattr(api, "query"):
        return None
    owner, name = REPO.split("/", 1)
    results = []
    cursor = None
    pages = (MAX_OPEN_PULLS + 49) // 50
    try:
        for _ in range(pages):
            data = api.query(OPEN_PULLS_QUERY, {"owner": owner, "name": name, "cursor": cursor})
            conn = ((data or {}).get("repository") or {}).get("pullRequests")
            if not isinstance(conn, dict):
                return None
            for node in conn.get("nodes") or ():
                parsed = _gql_to_prefetched(node)
                if parsed is not None:
                    results.append(parsed)
                if len(results) >= MAX_OPEN_PULLS:
                    return results
            page_info = conn.get("pageInfo") or {}
            if not page_info.get("hasNextPage") or not page_info.get("endCursor"):
                break
            cursor = page_info["endCursor"]
        return results
    except ApiError:
        return None


def evaluate(api, number, main, config, *, prefetched=None, compare_cache=None, cache_lock=None):
    """Return a decision dict: stage land | remeasure | hold, with every named reason found."""
    pr = prefetched["pr"] if prefetched is not None else api.call(f"pulls/{number}")
    head = (pr.get("head") or {}).get("sha", "")
    decision = {"number": number, "head": head, "title": printable(pr.get("title", ""), 120),
                "author": (pr.get("user") or {}).get("login"), "node_id": pr.get("node_id"),
                "stage": "hold", "reasons": [], "checks": {}, "measured_base": main,
                "ancestor_landing": False, "header_change": False, "pr_files": []}
    reasons = decision["reasons"]

    def hold(code, expected, found, final=False):
        reasons.append(Held(code, expected, found, final))

    if pr.get("state") != "open" or pr.get("number") != number or not HEX40.fullmatch(head):
        hold("not-open", "an open pull request with an exact head", "a closed or malformed pull request", True)
        return decision
    base = pr.get("base") or {}
    if base.get("ref") != "main" or (base.get("repo") or {}).get("id") != REPO_ID:
        hold("not-main", "a pull request into this repository's main", f"base {printable(base.get('ref'), 60)}", True)
        return decision
    head_repo = (pr.get("head") or {}).get("repo") or {}
    decision["fork"] = head_repo.get("id") != REPO_ID
    # Trusted: a branch of this repository, or an author who is an owner, member or collaborator.
    decision["trusted"] = not decision["fork"] or pr.get("author_association") in {"OWNER", "MEMBER", "COLLABORATOR"}
    head_ref = (pr.get("head") or {}).get("ref", "")
    # Rehearsal and corpus branches are ours. A fork's branch name is chosen by its author and decides nothing.
    if (not decision["fork"] and head_ref.startswith(REFUSED_BRANCH_PREFIXES)
            and (number, head) not in config["exceptions"]):
        hold("refused-branch", "an ordinary contributor branch",
             f"branch {printable(head_ref, 80)}, a rehearsal or corpus branch that never lands", True)
    if pr.get("draft") is not False:
        hold("draft", "a pull request marked ready for review",
             "a draft; choose Ready for review when you want it to land")
    decision["has_ledger_label"] = any(label.get("name") == LEDGER_REQUEST_LABEL for label in pr.get("labels") or ())
    if any(label.get("name") == HOLD_LABEL for label in pr.get("labels") or ()):
        hold("hold-label", f"no {HOLD_LABEL} label", f"the {HOLD_LABEL} label, set by a maintainer")
    if prefetched is not None:
        statuses, bot_statuses = prefetched["statuses"], prefetched["bot_statuses"]
        # If a non-bot actor overwrote a bot-owned context in GraphQL's latest-only view, re-read full REST history.
        bot_sensitive = (ADMISSION_CONTEXT, HEADER_RULES_CONTEXT, BOAT, RULES, INLINE, LANDING_CONTEXT)
        if any(ctx in statuses and ctx not in bot_statuses for ctx in bot_sensitive):
            statuses, bot_statuses = read_statuses_all(api, head)
    else:
        statuses, bot_statuses = read_statuses_all(api, head)
    adm_status = bot_statuses.get(ADMISSION_CONTEXT) or {}
    if not head_repo:
        hold("head-repository-missing", "the pull request's source repository",
             "a deleted repository; open a new pull request from an existing fork", True)
    elif admitted(pr):
        decision["rule_admitted"] = False
    elif (adm_status.get("state") == "success"
          and (adm_status.get("description") or "").startswith(ADMITTED_BY_RULE)):
        # Admitted by rule: each compiler measurement of it is reserved by .github/workflows/admission.yml.
        decision["rule_admitted"] = True
    elif (adm_status.get("description") or "").startswith("Held: daily limit"):
        hold("daily-limit", "room in today's limit for automatic admissions",
             "the limit is reached; retried automatically, you do not need to do anything")
    else:
        hold("awaiting-admission", "an admission of this commit to the compiler check",
             "none yet. Changes to source files only are admitted automatically within a daily limit; other "
             "changes wait for a maintainer (label boat-ok); you do not need to do anything")
    if prefetched is not None and prefetched.get("files") is not None:
        files, all_simple_tu = prefetched["files"], prefetched["all_simple_tu"]
    elif prefetched is not None and any(r.code in {"refused-branch", "draft", "hold-label"} for r in reasons):
        files, all_simple_tu = [], False
    else:
        files, all_simple_tu = changed_files_detail(api, number)
    decision["pr_files"] = list(files)
    if any(name.startswith(".github/") for name in files):
        hold("workflow-change", "no change under .github/", "a workflow change; a maintainer merges those by hand", True)
    protected = sorted({name.split("/", 1)[0] + "/" for name in files if name.startswith(PROTECTED_PATHS)})
    if protected:
        # The checks refuse these paths too; the landing workflow never relies on that alone.
        hold("protected-path", "changes only outside " + ", ".join(PROTECTED_PATHS),
             f"a change under {', '.join(protected)}; a maintainer merges those by hand", True)
    header_change = any(name.startswith("include/") for name in files)
    decision["header_change"] = header_change
    decision["boat_status"] = bot_statuses.get(BOAT)
    boat_kind = classify(decision["boat_status"], main)
    pf_reviews = prefetched.get("reviews") if prefetched is not None else None
    can_check_reviews_rest = not reasons and boat_kind in {"fresh-success", "stale-success"}
    if header_change and config.get("auto_headers"):
        rules = bot_statuses.get(HEADER_RULES_CONTEXT) or {}
        text = rules.get("description") or ""
        if rules.get("state") == "success" and marker(main) in text:
            pass                                    # H1-H8 passed on this head and this main
        elif rules.get("state") in ("failure", "error"):
            hold("header-rules", f"{HEADER_RULES_CONTEXT} success", f"{printable(text, 100)}", True)
        else:
            approved_by_maintainer = False
            if config["approvers"]:
                if pf_reviews is not None:
                    approved_by_maintainer = _maintainer_review_from_records(pf_reviews, pr, head, config["approvers"])
                elif can_check_reviews_rest:
                    try:
                        approved_by_maintainer = lr.maintainer_review(
                            api, pr, {"number": number, "head": head}, config["approvers"]
                        )
                    except lr.Waiting:
                        approved_by_maintainer = False
            if approved_by_maintainer:
                pass                                # listed maintainer approved this exact head
            elif rules.get("state") == "success":
                decision["header_stale"] = True     # measured on another main: re-measured like the checks
            elif rules.get("state") == "pending" and text.startswith("Held: ledger request"):
                decision["ledger_request"] = True
                hold("ledger-request", "a decision on the ledger change this pull request asks for",
                     f"{printable(text, 90)}; it waits and never fails, you do not need to do anything")
            elif rules.get("state") == "pending" and text.startswith("Held: rule not built"):
                hold("header-rule-not-built", "every header rule to be in place", printable(text, 90))
            elif rules.get("state") == "pending" and text.startswith("Held:"):
                hold("header-rules", f"{HEADER_RULES_CONTEXT} success or listed header approval",
                     f"{printable(text, 90)}; a listed maintainer approval can clear this hold")
            elif boat_kind in {"fresh-success", "stale-success"}:
                hold("header-rules", f"{HEADER_RULES_CONTEXT} on head {head[:12]}", "no result yet")
    elif header_change:
        if not config["approvers"]:
            hold("header-approvers-empty", "a listed header approver in ci/landing/landing.json",
                 "an empty list, so no header change can land automatically", True)
        elif pf_reviews is not None:
            if not _maintainer_review_from_records(pf_reviews, pr, head, config["approvers"]):
                hold("header-approval", f"an approval of head {head[:12]} by a listed header approver",
                     "none yet. A maintainer listed in ci/landing/landing.json reviews every change under "
                     "include/; a new push needs a new approval")
        elif can_check_reviews_rest:
            try:
                approved = lr.maintainer_review(api, pr, {"number": number, "head": head}, config["approvers"])
            except lr.Waiting as exc:
                hold("header-approval", "a readable review history", str(exc))
            else:
                if not approved:
                    hold("header-approval", f"an approval of head {head[:12]} by a listed header approver",
                         "none yet. A maintainer listed in ci/landing/landing.json reviews every change under "
                         "include/; a new push needs a new approval")
    # GitHub does not recompute "mergeable" when main moves. A compiler check that passed on this exact main composed
    # this head onto it (unique ancestor, three-way merge), so an older "false" does not hold it; the merge itself is
    # still GitHub's and refuses a real conflict.
    if pr.get("mergeable") is False and classify(bot_statuses.get(BOAT), main) != "fresh-success":
        hold("merge-conflict", "a pull request that merges cleanly into main",
             "a conflict; merge or rebase main into the branch", True)
    decision["landing_status"] = bot_statuses.get(LANDING_CONTEXT)
    prior_landing = decision["landing_status"] or {}
    if (prior_landing.get("state") == "failure"
            and (prior_landing.get("description") or "").startswith("Held: merge-forbidden.")):
        hold("merge-forbidden", "an updated head after GitHub refused the automated merge (HTTP 403)",
             "same commit that got HTTP 403; update or rebase your branch onto main and push", True)
    stale = False
    required = REQUIRED + ((INLINE,) if INLINE_VIRTUAL_POLICY == "required" else ())
    for context in (BOAT, RULES, INLINE):
        kind = classify(bot_statuses.get(context), main)
        decision["checks"][context] = kind
        if context == INLINE and context not in required:
            if kind == "fresh-failure":
                hold("inline-virtual-failure", f"{INLINE} not failure on main {main[:12]}", "failure", True)
            elif kind == "stale-failure":
                stale = True
            continue
        if kind == "fresh-success":
            continue
        if kind == "stale-success":
            stale = True
        elif kind == "fresh-error":
            hold("infrastructure-error", f"{context} to complete on main {main[:12]}",
                 "an infrastructure error; retried automatically, you do not need to push")
        elif kind == "stale-error":
            hold("infrastructure-error", f"{context} to complete",
                 "an infrastructure error on an older main; retried automatically, you do not need to push")
        elif kind == "fresh-failure":
            hold("check-failure", f"{context} success on main {main[:12]}",
                 "failure; open that check's Details for the reason, then push a fix", True)
        elif kind == "stale-failure":
            hold("check-failure", f"{context} success",
                 "failure on an older main; open that check's Details, then push a fix to start a new check", True)
        elif kind == "pending":
            hold("measuring", f"{context} finished", "pending")
        elif (context == BOAT and kind == "missing" and decision["fork"]
              and pr.get("author_association") not in {"OWNER", "MEMBER", "COLLABORATOR"}):
            if config.get("auto_admission") or decision.get("rule_admitted"):
                hold("awaiting-measurement", f"{BOAT} on head {head[:12]}",
                     "no compiler check for this commit yet; automatic admission starts it, you do not need to do anything")
            else:
                # Without automatic admission, a maintainer admits each new contributor commit via boat-ok.
                hold("awaiting-readmission", f"a maintainer to admit commit {head[:12]} (remove and add the boat-ok label)",
                     "no compiler check for this commit yet. A maintainer reads each new commit from a contributor "
                     "before the compiler runs it; you do not need to do anything")
        else:
            hold("no-status", f"{context} from GitHub Actions on head {head[:12]}", kind)
    only_missing_rules = (
        bool(config.get("auto_admission"))
        and len(reasons) == 1
        and reasons[0].code == "no-status"
        and decision["checks"].get(RULES) == "missing"
        and decision["checks"].get(BOAT) == "stale-success"
    )
    if not reasons or only_missing_rules:
        if (stale and not decision.get("header_stale") and not header_change and all_simple_tu
                and pr.get("mergeable") is not False
                and decision["checks"].get(BOAT) == "stale-success"
                and decision["checks"].get(INLINE) != "stale-failure"):
            boat_st = bot_statuses.get(BOAT)
            boat_prefix = extract_main_prefix(boat_st) if _status_fresh_enough(boat_st, MAX_ANCESTOR_AGE_SECONDS) else None
            if boat_prefix:
                ancestor = resolve_disjoint_ancestor(api, boat_prefix, main, files,
                                                     compare_cache=compare_cache, cache_lock=cache_lock)
                if ancestor:
                    if not reasons and decision["checks"].get(RULES) == "fresh-success":
                        # Repo-wide G1-G8 (including G7 one-definition) passed on current main, and Boat
                        # compiled this exact TU against a disjoint ancestor with identical headers/configs.
                        decision["stage"] = "land"
                        decision["measured_base"] = ancestor
                        decision["ancestor_landing"] = True
                        inline_st = bot_statuses.get(INLINE)
                        inline_ok = (inline_st
                                     and inline_st.get("state") == "success"
                                     and marker(ancestor) in (inline_st.get("description") or ""))
                        decision["inline"] = "success" if inline_ok else "pending"
                        return decision
                    if decision["checks"].get(RULES) in {"stale-success", "missing"}:
                        # Refresh only hosted source rules on current main (0 Boat VMs) to prove G7.
                        reasons.clear()
                        decision["source_recheck_only"] = True
                        decision["measured_base"] = ancestor
        if not reasons:
            decision["stage"] = "remeasure" if stale or decision.get("header_stale") else "land"
            decision["inline"] = bot_statuses.get(INLINE, {}).get("state") if decision["checks"][INLINE] == "fresh-success" else "pending"
    return decision


def open_pulls(api):
    numbers = []
    for page in range(1, MAX_OPEN_PULLS // 100 + 1):
        entries = api.call(f"pulls?state=open&base=main&sort=created&direction=asc&per_page=100&page={page}")
        numbers += [item["number"] for item in entries]
        if len(entries) < 100:
            break
    return numbers


def main_failed_landing(api, main):
    status = read_statuses(api, main).get(LANDING_CONTEXT)
    return status and status.get("state") == "failure" and (status.get("creator") or {}).get("id") == lr.BOT_ID


CLEARED = "Cleared to land on main"


def report_hold(api, decision, main=None):
    """Live only: keep one current BFME II / Landing status on the pull request head.

    That status is also a required check of main's ruleset. "Cleared to land" exists only while this job merges
    that exact head, so a clearance left behind by an interrupted run is withdrawn here.
    """
    if not api.live:
        return
    prior = decision.get("landing_status") or {}
    if not decision["reasons"]:
        if prior.get("state") == "success" and (prior.get("description") or "").startswith(CLEARED):
            status(api, decision["head"], "pending", "Eligible; waiting for the landing job")
        return
    first = decision["reasons"][0]
    if (first.code == "measuring" and main
            and prior.get("state") == "pending"
            and prior.get("description") == f"Measuring on main {main[:12]} before landing"):
        return
    state = "failure" if first.final else "pending"
    text = printable(f"Held: {first.code}. Expected {first.expected}.", 140)
    if prior.get("state") == state and prior.get("description") == text:
        return
    status(api, decision["head"], state, text)


def label_ledger_request(api, decision):
    """H9: a pull request waiting for a ledger decision carries the ledger-request label (live only, once)."""
    if not api.live or not decision.get("ledger_request") or decision.get("has_ledger_label"):
        return
    code, _ = api.write("POST", f"issues/{decision['number']}/labels", {"labels": [LEDGER_REQUEST_LABEL]})
    if code not in (200, 201):
        print(f"label: Expected HTTP 200 adding {LEDGER_REQUEST_LABEL} to #{decision['number']}. Found HTTP {code}.",
              file=sys.stderr)


def status(api, sha, state, text):
    run = getattr(api, "run_id", "") or ""
    body = {"state": state, "context": LANDING_CONTEXT, "description": printable(text, 140)}
    if run.isdigit():
        body["target_url"] = f"https://github.com/{REPO}/actions/runs/{run}"
    code, _ = api.write("POST", f"statuses/{sha}", body)
    if code != 201:
        raise Failed("status-write", f"HTTP 201 for the {LANDING_CONTEXT} status", f"HTTP {code}")


def comment(api, number, text):
    code, _ = api.write("POST", f"issues/{number}/comments", {"body": text})
    if code != 201:
        print(f"comment: Expected HTTP 201 for a comment on #{number}. Found HTTP {code}.", file=sys.stderr)


def dispatch(api, event, payload):
    code, _ = api.write("POST", "dispatches", {"event_type": event, "client_payload": payload})
    if code != 204:
        raise Failed("dispatch", f"HTTP 204 for repository_dispatch {event}", f"HTTP {code}")


def optional_inline_supported():
    """ci/managed-check/landing_read.py accepts inline_required=False from its remeasure release on."""
    return "inline_required" in inspect.signature(lr.start_runs).parameters


def inline_options(inline):
    if inline == "success":
        return {}
    if INLINE_VIRTUAL_POLICY == "required":
        raise Held("inline-virtual", f"{INLINE} success", inline)
    if not optional_inline_supported():
        raise Held("inline-virtual-pending", "a receipt reader that accepts a report-only Inline virtual result",
                   "the installed ci/managed-check/landing_read.py, which requires Inline virtual success")
    return {"inline_required": False}


def _verify_single_run(api, context, status_obj, binding):
    if not status_obj or status_obj.get("state") != "success":
        raise Held("run-binding", f"{context} success on head {binding['head'][:12]}",
                   str((status_obj or {}).get("state") or "missing"))
    if (status_obj.get("creator") or {}).get("id") != lr.BOT_ID:
        raise Held("run-binding", f"{context} from GitHub Actions", "a different writer")
    target = lr.URL.fullmatch(status_obj.get("target_url") or "")
    if not target or marker(binding["base"]) not in (status_obj.get("description") or ""):
        raise Held("run-binding", f"{context} linked to main {binding['base'][:12]}", "stale or missing link")
    run_id = target[1]
    run = api.call("actions/runs/" + run_id)
    event = run.get("event")
    run_sha = {"pull_request_target": binding["head"], "repository_dispatch": binding["base"]}.get(event)
    expected_paths = (lr.FLOW[context], lr.FLOW[BOAT]) if context == RULES else (lr.FLOW[context],)
    if (str(run.get("id")) != run_id or run.get("path") not in expected_paths
            or run_sha is None or run.get("head_sha") != run_sha
            or (run.get("repository") or {}).get("id") != REPO_ID
            or run.get("status") != "completed" or run.get("conclusion") != "success"
            or not lr.NUMBER.fullmatch(str(run.get("run_attempt", "")))):
        raise Held("run-binding", f"a completed successful protected {context} run", "incompatible run metadata")
    linked = (run.get("pull_requests") or []) if event == "pull_request_target" else []
    if linked and not any(item.get("number") == binding["number"]
                          and (item.get("head") or {}).get("sha") == binding["head"]
                          and (item.get("base") or {}).get("sha") == binding["base"]
                          and (item.get("base") or {}).get("ref") == "main" for item in linked):
        raise Held("run-binding", f"{context} bound to #{binding['number']} and main {binding['base'][:12]}",
                   "another associated pair")
    return [run_id, str(run["run_attempt"])]


def runs_for(api, binding, inline, *, target_main=None):
    """Exact producing runs, read by the protected landing_read.start_runs (or split-base for disjoint ancestor)."""
    options = inline_options(inline)
    inline_mode = "success" if not options else "pending"
    if target_main is None or target_main == binding["base"]:
        try:
            runs = lr.start_runs(api, binding, **options)
        except lr.Waiting as exc:
            raise Held("run-binding", "protected runs bound to this head and main", str(exc)) from None
        return {"match": list(runs[BOAT]), "rules": list(runs[RULES]), "inline": inline_mode}
    _, bot_statuses = read_statuses_all(api, binding["head"])
    match_run = _verify_single_run(api, BOAT, bot_statuses.get(BOAT), binding)
    if inline_mode == "success":
        inline_st = bot_statuses.get(INLINE) or {}
        inline_target = lr.URL.fullmatch(inline_st.get("target_url") or "")
        if (inline_st.get("state") != "success" or not inline_target
                or inline_target[1] != match_run[0]
                or marker(binding["base"]) not in (inline_st.get("description") or "")):
            raise Held("run-binding", "inline virtual from the same Boat run", "mismatched inline run")
    rules_binding = {"number": binding["number"], "base": target_main, "head": binding["head"]}
    rules_run = _verify_single_run(api, RULES, bot_statuses.get(RULES), rules_binding)
    return {"match": match_run, "rules": rules_run, "inline": inline_mode}


def output(name, value):
    path = os.environ.get("GITHUB_OUTPUT")
    if path:
        with Path(path).open("a") as stream:
            stream.write(f"{name}={value}\n")


def headline(text):
    """One line for someone reading the Actions page: an annotation on the run plus the top of the summary."""
    text = printable(text, 200).replace("::", ": ")
    print(f"::notice title=BFME II landing::{text}")
    path = os.environ.get("GITHUB_STEP_SUMMARY")
    if path:
        with Path(path).open("a") as stream:
            stream.write(f"### {text}\n\n")


def summary(lines):
    text = "\n".join(lines) + "\n"
    print(text)
    path = os.environ.get("GITHUB_STEP_SUMMARY")
    if path:
        with Path(path).open("a") as stream:
            stream.write(text)


def table(decisions, main, live):
    lines = [f"**BFME II landing** ({'live' if live else 'dry run'}) on main `{main[:12]}`", "",
             "| PR | Head | Boat | Source rules | Inline virtual | Decision |", "|---|---|---|---|---|---|"]
    for item in decisions:
        if item["reasons"]:
            first = item["reasons"][0]
            more = f" (+{len(item['reasons']) - 1} more)" if len(item["reasons"]) > 1 else ""
            verdict = f"Held: {first.line()}{more}"
        else:
            verdict = {"land": "Eligible: measured on current main", "remeasure": "Eligible after a fresh measurement on current main"}[item["stage"]]
        checks = item["checks"]
        lines.append(f"| #{item['number']} | `{item['head'][:12]}` | {checks.get(BOAT, '-')} | {checks.get(RULES, '-')} "
                     f"| {checks.get(INLINE, '-')} | {verdict} |")
    return lines


def wait_for_measurement(api, config, binding, clock, sleep, check_first=False, max_wait=MEASURE_WAIT_SECONDS,
                         *, target_main=None):
    """Poll until verified results exist AND their producing runs have completed and bind.

    A check posts its status a few seconds before its workflow run is marked completed, so a fresh status alone
    is not enough: the protected run check in landing_read requires a completed run.
    """
    expected_main = target_main or binding["base"]
    deadline = clock() + (max_wait if api.live else 0)
    first = True
    while True:
        if not (first and check_first):
            sleep(POLL_SECONDS)
        first = False
        main = current_main(api)
        if main != expected_main:
            raise Held("main-moved", f"main {expected_main[:12]} while measuring", f"main {main[:12]}")
        decision = evaluate(api, binding["number"], main, config)
        if decision["head"] != binding["head"]:
            raise Held("head-changed", f"head {binding['head'][:12]}", f"head {decision['head'][:12]}")
        if decision["stage"] == "land" and decision["measured_base"] == binding["base"]:
            try:
                return decision, runs_for(api, binding, decision["inline"], target_main=expected_main)
            except Held as exc:
                if exc.code != "run-binding":
                    raise
                print(f"Fresh results seen; waiting for their runs to complete ({exc.found[:120]}).")
                if clock() >= deadline:
                    raise
                continue
        hard = [item for item in decision["reasons"] if item.code != "measuring"]
        if hard:
            raise hard[0]
        if clock() >= deadline:
            raise Held("measurement-timeout", f"fresh results {marker(main)} within {MEASURE_WAIT_SECONDS} s",
                       "checks still pending or not started; are the check workflows listening for "
                       f"repository_dispatch {REMEASURE_EVENT}?")


def timed_out(decision, main):
    """A re-measurement that already timed out on this main is not retried until main or the head changes."""
    text = (decision.get("landing_status") or {}).get("description") or ""
    return text.startswith("Held: measurement-timeout") and marker(main) in text


def measuring_state(decision, main, wall_clock=None):
    """Return 'active', 'expired', or None for an in-flight non-blocking remeasurement on this main."""
    prior = decision.get("landing_status") or {}
    expected = f"Measuring on main {main[:12]} before landing"
    if prior.get("state") != "pending" or prior.get("description") != expected:
        return None
    created = prior.get("created_at") or ""
    if not created:
        return "active"
    try:
        started = datetime.datetime.strptime(created, "%Y-%m-%dT%H:%M:%SZ").replace(tzinfo=datetime.timezone.utc)
    except ValueError:
        return "active"
    now = wall_clock() if wall_clock else datetime.datetime.now(datetime.timezone.utc)
    if now.tzinfo is None:
        now = now.replace(tzinfo=datetime.timezone.utc)
    return "expired" if (now - started).total_seconds() >= MEASURE_WAIT_SECONDS else "active"


def _status_age_seconds(status_obj, wall_clock=None):
    created = (status_obj or {}).get("created_at") or ""
    if not created:
        return 0.0
    try:
        started = datetime.datetime.strptime(created, "%Y-%m-%dT%H:%M:%SZ").replace(tzinfo=datetime.timezone.utc)
    except ValueError:
        return 0.0
    now = wall_clock() if wall_clock else datetime.datetime.now(datetime.timezone.utc)
    if now.tzinfo is None:
        now = now.replace(tzinfo=datetime.timezone.utc)
    return max(0.0, (now - started).total_seconds())


def active_header_measuring(decisions, main, wall_clock=None):
    """Return eligible header PRs currently measuring on main (bounded to HEADER_PRIORITY_WAIT_SECONDS)."""
    active = []
    for item in decisions:
        if not item.get("header_change"):
            continue
        reasons = item.get("reasons") or []
        if any(r.final or r.code not in {"measuring", "header-rules"} for r in reasons):
            continue
        if any(r.code == "header-rules" and r.found != "no result yet" for r in reasons):
            continue
        boat_st = item.get("boat_status") or {}
        boat_pending = (boat_st.get("state") == "pending"
                        and (boat_st.get("creator") or {}).get("id") == lr.BOT_ID
                        and marker(main) in (boat_st.get("description") or "")
                        and _status_age_seconds(boat_st, wall_clock) < HEADER_PRIORITY_WAIT_SECONDS)
        landing_active = (measuring_state(item, main, wall_clock) == "active"
                          and _status_age_seconds(item.get("landing_status"), wall_clock) < HEADER_PRIORITY_WAIT_SECONDS)
        if boat_pending or landing_active:
            active.append(item)
    return active


ENQUEUE = ("mutation($pr: ID!, $head: GitObjectID!) { enqueuePullRequest(input: "
           "{pullRequestId: $pr, expectedHeadOid: $head}) { mergeQueueEntry { position } } }")
QUEUED = "Cleared for the merge queue at head"
GROUP_REF = re.compile(r"refs/heads/gh-readonly-queue/main/pr-([1-9][0-9]{0,8})-([0-9a-f]{40})\Z")


def group(api, env):
    """merge_group: vouch for a queue commit only when this workflow queued that exact pull request head.

    main's ruleset requires BFME II / Landing on the queue commit as well, so a pull request queued any other way
    (a writer's "merge when ready") is drained from the queue with a named reason. Nothing from the queue commit is
    run: the workflow checks these tools out from the group's base, which is main.
    """
    event = read(env["GITHUB_EVENT_PATH"], 2 * 1024**2).get("merge_group") or {}
    sha, ref = event.get("head_sha") or "", event.get("head_ref") or ""
    if not HEX40.fullmatch(sha):
        raise Failed("merge-group", "a merge group with a 40-character head commit", "missing or malformed metadata")
    found = GROUP_REF.fullmatch(ref)
    if not found:
        held = Held("merge-group-ref", "a queue ref refs/heads/gh-readonly-queue/main/pr-<number>-<head>",
                    printable(ref, 100) or "no ref")
    else:
        number, head = int(found[1]), found[2]
        pr = api.call(f"pulls/{number}")
        landing = read_statuses(api, head).get(LANDING_CONTEXT) or {}
        held = None
        if pr.get("state") != "open" or (pr.get("head") or {}).get("sha") != head:
            held = Held("head-changed", f"#{number} open at head {head[:12]}", "a different head or a closed pull request")
        elif (landing.get("state") != "success" or landing.get("description") != f"{QUEUED} {head[:12]}"
              or (landing.get("creator") or {}).get("id") != lr.BOT_ID):
            held = Held("not-queued-by-landing", f"#{number} queued by the landing workflow at head {head[:12]}",
                        "a pull request added to the queue another way")
    if held:
        status(api, sha, "failure", f"Held: {held.code}. Expected {held.expected}.")
        headline(f"Merge group {sha[:12]} refused: {held.code}.")
        summary([f"Refused merge group {sha[:12]}. {held.line()}"])
        return 0
    status(api, sha, "success", f"Cleared in the merge queue: #{number} head {head[:12]}")
    headline(f"Merge group {sha[:12]} cleared for #{number} at head {head[:12]}.")
    return 0


def merge_queue_active(api):
    """True when an active ruleset requires the merge queue on main.

    The private Free repository answers 403 (rules unavailable); then the job merges directly. That is safe in both
    directions: once a queue is required, GitHub refuses a direct merge (HTTP 405) and the job only holds.
    """
    try:
        rules = api.call("rules/branches/main")
    except ApiError:
        return False
    return isinstance(rules, list) and any(isinstance(rule, dict) and rule.get("type") == "merge_queue"
                                           for rule in rules)


def enqueue(api, decisions, main, lines):
    """Merge-queue mode: add every eligible pull request to GitHub's queue with its expected head.

    The queue measures main plus the pull request again (merge_group) and merges exactly what it measured, so this
    job neither re-measures nor merges. The hold rules above (admission, header approval, refusals) still decide
    what may enter the queue.
    """
    ready = [item for item in decisions if item["stage"] in {"land", "remeasure"}]
    done, failed = [], []
    for item in ready:
        prior = (item.get("landing_status") or {}).get("description") or ""
        if prior == f"{QUEUED} {item['head'][:12]}":
            continue
        if not api.live:
            done.append(f"#{item['number']} (dry run: would queue)")
            continue
        # The ruleset requires this status on the pull request and on its queue commit (see group()).
        status(api, item["head"], "success", f"{QUEUED} {item['head'][:12]}")
        try:
            api.mutate(ENQUEUE, {"pr": item["node_id"], "head": item["head"]})
        except ApiError as exc:
            held = Held("enqueue-refused", "GitHub to accept the pull request into the merge queue", str(exc)[:160])
            report_hold(api, {"head": item["head"], "reasons": [held], "landing_status": None})
            failed.append(f"#{item['number']}")
            continue
        done.append(f"#{item['number']}")
    headline(f"Merge queue mode on main {main[:12]}: queued {', '.join(done) or 'nothing new'}"
             + (f"; refused {', '.join(failed)}" if failed else "") + ".")
    summary(lines)
    output("action", "none")
    return 0


def select(api, env, config, directory, clock=time.monotonic, sleep=time.sleep):
    main = current_main(api)
    if env.get("GITHUB_SHA") != main:
        headline(f"Superseded: main moved to {main[:12]} after this run started; a newer run takes over.")
        output("action", "none")
        return 0
    reconcile(api, main)
    if main_failed_landing(api, main):
        raise Failed("main-unverified", f"main {main[:12]} without a failed {LANDING_CONTEXT} status",
                     "a failed post-landing verification; a maintainer must repair main first")
    only = env.get("BFME_LANDING_PR", "").strip()
    if only and not lr.NUMBER.fullmatch(only):
        raise Failed("input", "a pull request number or nothing", printable(only, 40))
    compare_cache, cache_lock = {}, threading.Lock()
    prefetched_list = None if only else fetch_open_pulls_batch(api)
    if prefetched_list is not None:
        if len(prefetched_list) > 1 and getattr(api, "thread_safe_reads", False):
            with ThreadPoolExecutor(max_workers=min(6, len(prefetched_list))) as pool:
                decisions = list(pool.map(
                    lambda item: evaluate(api, item["number"], main, config,
                                          prefetched=item, compare_cache=compare_cache, cache_lock=cache_lock),
                    prefetched_list,
                ))
        else:
            decisions = [
                evaluate(api, item["number"], main, config,
                         prefetched=item, compare_cache=compare_cache, cache_lock=cache_lock)
                for item in prefetched_list
            ]
    else:
        numbers = [int(only)] if only else open_pulls(api)
        if len(numbers) > 1 and getattr(api, "thread_safe_reads", False):
            with ThreadPoolExecutor(max_workers=min(6, len(numbers))) as pool:
                decisions = list(pool.map(
                    lambda n: evaluate(api, n, main, config, compare_cache=compare_cache, cache_lock=cache_lock),
                    numbers,
                ))
        else:
            decisions = [
                evaluate(api, number, main, config, compare_cache=compare_cache, cache_lock=cache_lock)
                for number in numbers
            ]
    for item in decisions:
        report_hold(api, item, main=main)
        label_ledger_request(api, item)
    lines = table(decisions, main, api.live)
    if merge_queue_active(api):
        return enqueue(api, decisions, main, lines)
    ready_land = sorted(
        (item for item in decisions if item["stage"] == "land"),
        key=lambda d: (0 if d.get("header_change") else 1, 0 if not d.get("ancestor_landing") else 1, d["number"]),
    )
    pick = ready_land[0] if ready_land else None
    if pick is not None and not pick.get("header_change") and not only:
        header_active = active_header_measuring(decisions, main)
        if header_active:
            names = ", ".join(f"#{item['number']}" for item in header_active)
            headline(f"Pausing narrow landings on main {main[:12]} while header PR(s) {names} finish measuring.")
            summary(lines + ["", f"Header priority: holding narrow landing #{pick['number']} while {names} measure on main {main[:12]}."])
            output("action", "none")
            return 0
        # Prevent a continuous stream of narrow PRs from starving a stale header PR waiting to re-measure,
        # but only yield the ready narrow PR when a stale header can actually be dispatched under the cap.
        stale_headers = [
            item for item in decisions
            if item.get("header_change") and item["stage"] == "remeasure" and not timed_out(item, main)
            and measuring_state(item, main) is None
            and (config.get("auto_admission") or item.get("trusted"))
        ]
        current_active = sum(
            1 for item in decisions
            if measuring_state(item, main) == "active"
            or (
                item["reasons"] and any(r.code == "measuring" for r in item["reasons"])
                and (item.get("boat_status") or {}).get("state") == "pending"
                and marker(main) in ((item.get("boat_status") or {}).get("description") or "")
                and measuring_state(item, main) != "expired"
            )
        )
        if stale_headers and config["remeasure"] != "off" and current_active < MAX_PARALLEL_REMEASURES:
            pick = None
    if pick is None:
        stale_items = [item for item in decisions if item["stage"] == "remeasure" and not timed_out(item, main)]
        if not stale_items:
            held = [f"#{item['number']} {item['reasons'][0].code}" for item in decisions if item["reasons"]]
            headline(f"Nothing to land on main {main[:12]}: {len(decisions)} open pull request(s)"
                     + (f"; held {', '.join(held[:6])}" if held else "") + ".")
            summary(lines + ["", "Nothing to land."])
            output("action", "none")
            return 0
        if config["remeasure"] == "off":
            first = stale_items[0]
            headline(f"#{first['number']} waits: measured on an older main and automatic re-measurement is off.")
            summary(lines + ["", f"#{first['number']} needs a fresh measurement on main {main[:12]}; "
                             "automatic re-measurement is off. Push a commit or reopen the pull request."])
            output("action", "none")
            return 0
        if not api.live:
            first = stale_items[0]
            headline(f"Dry run: would re-measure #{first['number']} on main {main[:12]}, then land it.")
            summary(lines + ["", f"Would send repository_dispatch {REMEASURE_EVENT} for #{first['number']} "
                             f"head {first['head'][:12]} on main {main[:12]} and land once fresh results arrive."])
            output("action", "none")
            return 0
        dispatched, active, expired, held_notes, pending_dispatch = [], [], [], [], []
        for item in stale_items:
            mstate = measuring_state(item, main)
            if mstate == "active":
                active.append(f"#{item['number']}")
            elif mstate == "expired":
                exc = Held("measurement-timeout", f"fresh results {marker(main)} within {MEASURE_WAIT_SECONDS} s",
                           "checks still pending or not started; are the check workflows listening for "
                           f"repository_dispatch {REMEASURE_EVENT}?")
                report_hold(api, {"head": item["head"], "reasons": [exc], "landing_status": None})
                expired.append(f"#{item['number']}")
            else:
                pending_dispatch.append(item)
        in_flight_measuring = sum(
            1 for item in decisions
            if item["reasons"] and any(r.code == "measuring" for r in item["reasons"])
            and measuring_state(item, main) != "expired"
            and (
                measuring_state(item, main) == "active"
                or (
                    (item.get("boat_status") or {}).get("state") == "pending"
                    and marker(main) in ((item.get("boat_status") or {}).get("description") or "")
                )
            )
        )
        pending_dispatch.sort(key=lambda d: (0 if d.get("header_change") else (1 if d.get("source_recheck_only") else 2), d["number"]))
        for item in pending_dispatch:
            if len(active) + in_flight_measuring + len(dispatched) >= MAX_PARALLEL_REMEASURES:
                break
            item_binding = {"number": item["number"], "base": main, "head": item["head"]}
            if item.get("source_recheck_only") and config.get("auto_admission"):
                dispatch(api, SOURCE_RECHECK_EVENT, item_binding)
            elif config.get("auto_admission"):
                dispatch(api, ADMIT_REQUEST_EVENT, {"number": item["number"]})
            elif not item.get("trusted"):
                held = Held("awaiting-readmission", f"a maintainer to admit commit {item['head'][:12]} on main "
                            f"{main[:12]} (remove and add the boat-ok label)",
                            "a measurement on an older main only; you do not need to do anything")
                report_hold(api, {"head": item["head"], "reasons": [held], "landing_status": None})
                held_notes.append(f"#{item['number']} held. {held.line()}")
                continue
            else:
                dispatch(api, REMEASURE_EVENT, item_binding)
            status(api, item["head"], "pending", f"Measuring on main {main[:12]} before landing")
            print(f"Requested a fresh measurement of #{item['number']} on main {main[:12]}.")
            dispatched.append(f"#{item['number']}")
        notes = []
        if dispatched:
            notes.append(f"Requested fresh measurements on main {main[:12]} for {', '.join(dispatched)}; "
                         "landing resumes automatically when results arrive.")
        if active:
            notes.append(f"Already measuring on main {main[:12]}: {', '.join(active)}.")
        if expired:
            notes.append(f"Held (measurement-timeout) on main {main[:12]}: {', '.join(expired)}.")
        notes.extend(held_notes)
        if dispatched:
            headline(f"Requested non-blocking re-measurement on main {main[:12]} for {', '.join(dispatched)}.")
        elif expired:
            headline(f"Held {', '.join(expired)}: measurement-timeout.")
        elif held_notes:
            headline(f"Held: awaiting-readmission ({len(held_notes)} pull request(s)).")
        else:
            waiting_desc = ", ".join(active) if active else f"{in_flight_measuring} measuring pull request(s)"
            headline(f"Waiting for in-flight measurements on main {main[:12]}: {waiting_desc}.")
        summary(lines + [""] + (notes or ["Nothing to land."]))
        output("action", "none")
        return 0
    measured_base = pick["measured_base"]
    binding = {"number": pick["number"], "base": measured_base, "head": pick["head"]}
    try:
        pick, runs = wait_for_measurement(api, config, binding, clock, sleep,
                                          check_first=True, max_wait=LAND_SETTLE_SECONDS, target_main=main)
    except Held as exc:
        report_hold(api, {"head": binding["head"], "reasons": [exc], "landing_status": None})
        headline(f"Held #{binding['number']}: {exc.code}. Expected {exc.expected}.")
        summary(lines + ["", f"#{binding['number']} held while waiting for its results. {exc.line()}"])
        output("action", "none")
        return 0
    Path(directory).mkdir(mode=0o700, parents=True)
    record = binding | {"runs": runs}
    if measured_base != main:
        record["target_main"] = main
    Path(directory, "binding.json").write_text(json.dumps(record, sort_keys=True) + "\n")
    for key, value in (("action", "land"), ("number", binding["number"]), ("match_run", runs["match"][0]),
                       ("match_attempt", runs["match"][1]), ("rules_run", runs["rules"][0]),
                       ("rules_attempt", runs["rules"][1]), ("inline", runs["inline"])):
        output(key, value)
    if measured_base != main:
        headline(f"#{binding['number']} passed on disjoint ancestor {measured_base[:12]} for main {main[:12]}; checking receipts and merging next.")
        summary(lines + ["", f"Candidate #{binding['number']} head {binding['head'][:12]} measured on disjoint ancestor "
                         f"{measured_base[:12]}, landing onto main {main[:12]}; collecting its receipts."])
    else:
        headline(f"#{binding['number']} is measured on current main {main[:12]}; checking its receipts and merging next.")
        summary(lines + ["", f"Candidate #{binding['number']} head {binding['head'][:12]} on main {main[:12]}; "
                         "collecting its receipts."])
    return 0


def land(api, env, config, directory, sleep=time.sleep):
    """Merge the selected pull request. Any hold replaces its Landing status, so "Measuring" never stays behind."""
    directory = Path(directory)
    prior = read(directory / "binding.json", 4096)
    try:
        return land_bound(api, env, config, directory, prior, sleep)
    except Failed:
        raise
    except Held as exc:
        report_hold(api, {"head": prior["head"], "reasons": [exc], "landing_status": None})
        raise
    except (ApiError, OSError, ValueError, KeyError, TypeError):
        if api.live:
            try:
                report_hold(api, {"head": prior["head"], "landing_status": None, "reasons": [Held(
                    "interrupted", "a confirmed merge result", "an interrupted landing run; the next run checks main")]})
            except (Held, ApiError, OSError):
                pass
        raise


def land_bound(api, env, config, directory, prior, sleep):
    binding = {key: prior[key] for key in ("number", "base", "head")}
    target_main = prior.get("target_main") or binding["base"]
    is_ancestor = target_main != binding["base"]
    if env.get("GITHUB_SHA") != target_main:
        raise Failed("binding", f"a candidate bound to this run's main {env.get('GITHUB_SHA', '')[:12]}",
                     f"main {target_main[:12]}")
    decision = evaluate(api, binding["number"], target_main, config)
    if decision["head"] != binding["head"]:
        raise Held("head-changed", f"head {binding['head'][:12]}", f"head {decision['head'][:12]}")
    if decision["stage"] != "land" or decision["measured_base"] != binding["base"]:
        raise decision["reasons"][0] if decision["reasons"] else Held(
            "stale", f"results measured on main {binding['base'][:12]}", "results for another main")
    runs = runs_for(api, binding, decision["inline"], target_main=target_main)
    if runs != prior["runs"]:
        raise Held("attempt-changed", "the same producing runs as when the receipts were downloaded", "different runs")
    match = read(directory / "match/verdict.json", 256 * 1024)
    rules = read(directory / "rules/receipt.json", 4096)
    from gate import policy_digest
    rules_policy = policy_digest(lr.TOOLS / "config.json")
    options = inline_options(runs["inline"])
    inline = read(directory / "inline/inline-status.json", 4096) if not options else None
    if is_ancestor:
        # Verify Boat match (+ optional inline) on measured ancestor, and source rules (incl. repo-wide G7) on target_main.
        base, head, number = binding["base"], binding["head"], binding["number"]
        if (match.get("repo") != REPO_ID or match.get("version") != 1 or match.get("number") != number
                or match.get("base") != base or match.get("head") != head or match.get("kind") != "ok"
                or not HEX40.fullmatch(match.get("tree", "")) or not lr.HEX64.fullmatch(match.get("plan", ""))):
            raise Held("receipt", "a match receipt bound to the disjoint ancestor and PR head", "mismatched match receipt")
        measured_tree = match["tree"]
        positions = {"base": base, "head": head, "tree": measured_tree}
        if not options and (not isinstance(inline, dict) or inline.get("binding") != positions
                or inline.get("plan") != match["plan"] or inline.get("number") != number
                or inline.get("repo") != REPO_ID or inline.get("run") != runs["match"][0]
                or inline.get("attempt") != runs["match"][1] or inline.get("policy_sha256") != lr.policy()
                or inline.get("state") != "pass" or inline.get("verdict") != "pass"
                or not lr.HEX64.fullmatch(inline.get("evidence_sha256", ""))):
            raise Held("receipt", "an inline receipt bound to the disjoint ancestor tree", "mismatched inline receipt")
        if (rules.get("base") != target_main or rules.get("head") != head
                or not HEX40.fullmatch(rules.get("tree", "")) or rules.get("number") != number
                or rules.get("tools_commit") != target_main or rules.get("repo") != REPO_ID
                or rules.get("run") != runs["rules"][0] or rules.get("attempt") != runs["rules"][1]
                or rules.get("state") != "pass" or rules.get("policy_sha256") != rules_policy):
            raise Held("receipt", "source rules passed on current main", "mismatched or stale source rules receipt")
        tree = rules["tree"]
        # Cryptographically prove every PR TU has the exact same blob SHA in target_tree as in measured_tree.
        m_cache, t_cache = {}, {}
        for fname in decision["pr_files"]:
            m_blob = blob_sha_in_tree(api, measured_tree, fname, tree_cache=m_cache)
            t_blob = blob_sha_in_tree(api, tree, fname, tree_cache=t_cache)
            if not m_blob or m_blob != t_blob:
                raise Held("receipt", f"identical blob for {fname} on ancestor and current main",
                           f"ancestor {str(m_blob)[:12]} vs current {str(t_blob)[:12]}")
    else:
        try:
            positions = lr.compare_receipts(match, inline, rules, binding, runs["match"][1], runs["match"][0],
                                            runs["rules"][1], runs["rules"][0], rules_policy, **options)
        except lr.Waiting as exc:
            raise Held("receipt", "receipts for the same main, head and tree", str(exc)) from None
        tree = positions["tree"]
        measured_tree = tree

    if not api.live:
        main = current_main(api)
        if main != target_main:
            raise Held("main-moved", f"main {target_main[:12]}, the commit bound for landing", f"main {main[:12]}")
        headline(f"Dry run: would merge #{binding['number']} onto main {main[:12]}.")
        if is_ancestor:
            summary([f"Would merge #{binding['number']} ({MERGE_METHOD}) with expected head {binding['head'][:12]} "
                     f"onto main {main[:12]} (Boat on disjoint ancestor {binding['base'][:12]}, "
                     f"source rules on {main[:12]}), expecting parent {main[:12]} and tree {tree[:12]}."])
        else:
            summary([f"Would merge #{binding['number']} ({MERGE_METHOD}) with expected head {binding['head'][:12]} "
                     f"onto main {main[:12]}, expecting one parent {main[:12]} and tree {tree[:12]}."])
        return 0
    # The ruleset requires this status from GitHub Actions, so only this job can make the head mergeable.
    status(api, binding["head"], "success", f"{CLEARED} {target_main[:12]}")
    # Immediately before the merge: main must still be the bound target commit.
    main = current_main(api)
    if main != target_main:
        raise Held("main-moved", f"main {target_main[:12]}, the commit bound for landing", f"main {main[:12]}")
    extra_line = (f"Boat measured on disjoint ancestor {binding['base']}, measured tree {measured_tree}.\n"
                  if is_ancestor else "")
    commit_message = (f"Measured on main {target_main}, head {binding['head']}, tree {tree}.\n"
                      f"{extra_line}"
                      f"Pull request #{binding['number']}.\n"
                      f"{TRAILER}, run {env.get('GITHUB_RUN_ID', '0') or '0'}.")
    body = {"merge_method": MERGE_METHOD, "sha": binding["head"],
            "commit_title": printable(f"{decision['title']} (#{binding['number']})", 250),
            "commit_message": commit_message}
    code, payload = api.write("PUT", f"pulls/{binding['number']}/merge", body)
    errors = (payload or {}).get("errors")
    detail = f"{(payload or {}).get('message', '')} ({errors})" if errors else (payload or {}).get("message", "")
    message = printable(detail, 240)
    if code == 409:
        raise Held("head-changed", f"head {binding['head'][:12]} at merge",
                   f"a newer head; GitHub refused the merge ({message}). The new head is measured from scratch")
    if code == 403:
        hdrs = getattr(api, "last_headers", None) or {}
        diag = ", ".join(f"{k}={printable(v, 100)}" for k, v in sorted(hdrs.items()) if v)
        if diag:
            print(f"merge 403 diagnostics for #{binding['number']}: {diag}", file=sys.stderr)
        comment(api, binding["number"], f"Held: GitHub refused the automated merge (HTTP 403: {message}). "
                "This can happen when a fork branch lags a `.github/workflows/` update on `main`; "
                "update or rebase your branch onto current `main` and push.")
        if env.get("BFME_LANDING_CHAIN") == "1":
            try:
                dispatch(api, NEXT_EVENT, {"after": target_main})
            except Held as exc:
                print(exc.line(), file=sys.stderr)
        raise Held("merge-forbidden", "GitHub to allow the landing workflow to merge this pull request",
                   f"HTTP 403: {message}; update or rebase onto main and push", True)
    if code == 405:
        comment(api, binding["number"], f"Held: GitHub would not merge this pull request ({message}). "
                "If it conflicts with main, merge or rebase main into your branch and push.")
        raise Held("merge-refused", "GitHub to merge the pull request", f"HTTP 405: {message}", True)
    if code != 200 or not (payload or {}).get("merged") or not HEX40.fullmatch((payload or {}).get("sha", "")):
        raise Failed("merge", "HTTP 200 and a merged commit", f"HTTP {code}: {message}")
    commit = payload["sha"]
    target_binding = {"number": binding["number"], "base": target_main, "head": binding["head"]}
    verify(api, target_binding, tree, commit, sleep)
    # Dispatch bfme-landed BEFORE posting BFME II / Landing success on commit so crash recovery (reconcile)
    # guarantees the protected-state workflow receives the handoff even if this job stops mid-finalization.
    notified = []
    try:
        dispatch(api, LANDED_EVENT, landed_payload(binding["number"], binding["head"], target_main, commit, tree))
        notified.append(LANDED_EVENT)
    except Held as exc:
        print(exc.line(), file=sys.stderr)
        status(api, binding["head"], "success", f"Landed as {commit[:12]}")
        raise
    status(api, commit, "success",
           f"Verified landing of #{binding['number']}: parent {target_main[:12]}, tree {tree[:12]}")
    status(api, binding["head"], "success", f"Landed as {commit[:12]}")
    if is_ancestor:
        comment(api, binding["number"], f"Landed as {commit} on main (parent {target_main[:12]}, tree {tree[:12]}). "
                f"Boat measured on disjoint ancestor {binding['base'][:12]}, source rules passed on {target_main[:12]}; "
                f"the new main commit has exactly that parent and tree.")
    else:
        comment(api, binding["number"], f"Landed as {commit} on main. Measured on main {target_main[:12]}, "
                f"candidate tree {tree[:12]}; the new main commit has exactly that parent and tree.")
    if env.get("BFME_LANDING_CHAIN") == "1":
        try:
            dispatch(api, NEXT_EVENT, {"after": commit})
            notified.append(NEXT_EVENT)
        except Held as exc:
            print(exc.line(), file=sys.stderr)
    headline(f"Landed #{binding['number']} as {commit[:12]} on main (parent {target_main[:12]}, tree verified).")
    summary([f"Landed #{binding['number']} as {commit[:12]} (parent {target_main[:12]}, tree {tree[:12]}). "
             f"Sent: {', '.join(notified) or 'nothing'}."])
    return 0


def landed_payload(number, head, parent, commit, tree):
    """The exact bfme-landed hand-off the state workflow accepts: these five keys and no others."""
    return {"number": number, "head": head, "parent": parent, "commit": commit, "tree": tree}


def reconcile(api, main):
    """Finish a landing whose merge outcome was never confirmed (lost response, timeout, stopped job).

    A commit made by this workflow names, in its message, the main, head and tree it was measured on. If main's
    newest commit is one of those and has no BFME II / Landing status yet, verify it now, before anything else.
    """
    record = api.call(f"git/commits/{main}")
    # Only the body is ours; the title is the pull request author's and is never parsed.
    lines = (record.get("message") or "").partition("\n\n")[2].splitlines()
    if not any(TRAILER_LINE.fullmatch(line) for line in lines) or LANDING_CONTEXT in read_statuses(api, main):
        return
    measured = next(filter(None, map(MEASURED_LINE.fullmatch, lines)), None)
    number = next(filter(None, map(NUMBER_LINE.fullmatch, lines)), None)
    if not measured or not number:
        raise Failed("unreconciled-landing", f"main {main[:12]} to name its measured main, head, tree and pull request",
                     "a landing commit without them; a maintainer must check main")
    if not api.live:
        raise Held("unreconciled-landing", f"a {LANDING_CONTEXT} status on main {main[:12]}",
                   "a landing commit not yet verified; the next live run verifies it first")
    parent, head, tree = measured.groups()
    parents = [item.get("sha") for item in record.get("parents") or ()]
    found_tree = (record.get("tree") or {}).get("sha")
    binding = {"number": int(number[1]), "head": head, "base": parent}
    if parents != [parent]:
        fail_after(api, binding, main, f"exactly one parent {parent[:12]}",
                   "parents " + (", ".join(p[:12] for p in parents) or "none"))
    if found_tree != tree:
        fail_after(api, binding, main, f"tree {tree[:12]}", f"tree {str(found_tree)[:12]}")
    dispatch(api, LANDED_EVENT, landed_payload(binding["number"], head, parent, main, tree))
    status(api, main, "success", f"Verified landing of #{binding['number']}: parent {parent[:12]}, tree {tree[:12]}")
    print(f"Reconciled the earlier landing of #{binding['number']} as {main[:12]}.")


def verify(api, binding, tree, commit, sleep):
    for attempt in range(5):
        main = current_main(api)
        if main == commit:
            break
        sleep(2)
    else:
        fail_after(api, binding, commit, f"main at the merged commit {commit[:12]}", f"main {main[:12]}")
    record = api.call(f"git/commits/{commit}")
    parents = [item.get("sha") for item in record.get("parents") or ()]
    found_tree = (record.get("tree") or {}).get("sha")
    if parents != [binding["base"]]:
        fail_after(api, binding, commit, f"exactly one parent {binding['base'][:12]}",
                   "parents " + (", ".join(p[:12] for p in parents) or "none"))
    if found_tree != tree:
        fail_after(api, binding, commit, f"tree {tree[:12]}", f"tree {str(found_tree)[:12]}")


def fail_after(api, binding, commit, expected, found):
    try:
        status(api, commit, "failure", f"Landing verification failed: expected {expected}")
        comment(api, binding["number"], f"Merged as {commit}, but main failed verification: expected {expected}, "
                f"found {found}. Automatic landing is stopped until a maintainer repairs main.")
    finally:
        raise Failed("post-merge", expected, found)


def run(argv, env, api=None, config=None, **kwargs):
    mode, directory = argv
    if env.get("GITHUB_REPOSITORY") != REPO or env.get("GITHUB_REPOSITORY_ID") != str(REPO_ID):
        raise Failed("repository", "this repository", "another repository")
    if mode == "group":
        # GitHub starts this for the merge queue; it only ever writes one status, so it always runs live.
        if (env.get("GITHUB_EVENT_NAME") != "merge_group"
                or not (env.get("GITHUB_REF") or "").startswith("refs/heads/gh-readonly-queue/main/")):
            raise Failed("event", "a merge_group event for main's queue", "another event or ref")
        if api is None:
            api = Api(REPO, env.pop("BFME_LANDING_TOKEN", None), True)
        api.run_id = env.get("GITHUB_RUN_ID", "")
        return group(api, env)
    if env.get("GITHUB_REF") != "refs/heads/main":
        raise Failed("repository", "this repository's main branch", "another ref")
    live = env.get("BFME_LANDING_MODE") == "live"
    if api is None:
        api = Api(REPO, env.pop("BFME_LANDING_TOKEN", None), live)
    api.run_id = env.get("GITHUB_RUN_ID", "")
    api.run_attempt = env.get("GITHUB_RUN_ATTEMPT", "")
    config = dict(config or load_config())
    config.setdefault("auto_headers", env.get("BFME_AUTO_HEADERS") == "yes")
    config.setdefault("auto_admission", env.get("BFME_AUTO_ADMISSION") == "yes")
    if mode == "select":
        return select(api, env, config, directory, **kwargs)
    if mode == "land":
        return land(api, env, config, directory, **kwargs)
    raise Failed("mode", "select, land or group", printable(mode, 20))


def main():
    try:
        return run(sys.argv[1:], os.environ)
    except Failed as exc:
        print(f"Landing stopped. {exc.line()}", file=sys.stderr)
        headline(f"Stopped: {exc.code}. Expected {exc.expected}.")
        summary([f"Stopped. {exc.line()}"])
        return 1
    except Held as exc:
        print(f"Landing held. {exc.line()}", file=sys.stderr)
        headline(f"Held: {exc.code}. Expected {exc.expected}.")
        summary([f"Held. {exc.line()}"])
        return 0
    except (lr.Waiting, ApiError, ReadOnly) as exc:
        print(f"Landing stopped. {printable(exc, 300)}", file=sys.stderr)
        headline(f"Stopped: {printable(exc, 160)}")
        summary([f"Stopped. {printable(exc, 300)}"])
        return 1
    except (OSError, ValueError, KeyError, TypeError) as exc:
        detail = f"Expected readable landing inputs. Found a {type(exc).__name__} while reading them."
        print(f"Landing stopped. {detail}", file=sys.stderr)
        headline(f"Stopped: {detail}")
        summary([f"Stopped. {detail}"])
        return 1


if __name__ == "__main__":
    sys.exit(main())
