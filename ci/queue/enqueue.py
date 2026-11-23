"""Auto-enqueue ready pull requests into GitHub Native Merge Queue."""
import json
import os
from pathlib import Path
import re
import sys
import urllib.error
import urllib.request

OWNER = "Open-BFME"
NAME = "BFME2-Rebuild"
REPO = f"{OWNER}/{NAME}"
CANARY_PRS = frozenset({3, 4, 21, 27, 29})
DEFAULT_REFUSED_PREFIXES = ("canary/", "ci-corpus/", "corpus/")
HOLD_LABEL = "landing-hold"
MAX_FILES = 60
MAX_ENQUEUE = 16
MAX_BACKFILL = 18
MAX_ACTIVE_BOAT = 18
MIN_RATELIMIT_BACKFILL = 500
MIN_RATELIMIT_ENQUEUE = 100
LIMIT = 4 * 1024**2
HEX40 = re.compile(r"[0-9a-f]{40}\Z")

OPEN_PRS_QUERY = """
query($owner: String!, $name: String!, $cursor: String) {
  repository(owner: $owner, name: $name) {
    defaultBranchRef {
      target { oid }
    }
    pullRequests(states: OPEN, baseRefName: "main", first: 50, after: $cursor,
                 orderBy: {field: UPDATED_AT, direction: ASC}) {
      pageInfo { hasNextPage endCursor }
      nodes {
        id
        number
        isDraft
        headRefOid
        headRefName
        mergeable
        isInMergeQueue
        author {
          login
          ... on User { databaseId }
        }
        labels(first: 20) {
          nodes { name }
        }
        files(first: 65) {
          totalCount
          nodes { path }
        }
        commits(last: 1) {
          nodes {
            commit {
              status {
                contexts {
                  context
                  state
                  description
                }
              }
            }
          }
        }
        latestOpinionatedReviews(first: 20) {
          nodes {
            state
            commit { oid }
            author {
              ... on User { databaseId }
            }
          }
        }
      }
    }
  }
}
"""

ENQUEUE_MUTATION = """
mutation($id: ID!, $oid: GitObjectID!) {
  enqueuePullRequest(input: {pullRequestId: $id, expectedHeadOid: $oid}) {
    mergeQueueEntry {
      id
      position
    }
  }
}
"""


class ApiError(Exception):
    pass


class NoRedirect(urllib.request.HTTPRedirectHandler):
    def redirect_request(self, req, fp, code, msg, headers, newurl):
        return None


class GitHubApi:
    def __init__(self, token):
        if not token or not re.fullmatch(r"[A-Za-z0-9_.-]{15,2048}", token):
            raise ApiError("Missing or malformed BFME_QUEUE_TOKEN.")
        self._token = token
        self._opener = urllib.request.build_opener(urllib.request.ProxyHandler({}), NoRedirect())
        self.ratelimit_remaining = None
        self.ratelimit_reset = None

    def _record_ratelimit(self, headers):
        if not headers:
            return
        rem = headers.get("X-RateLimit-Remaining")
        if rem is not None:
            try:
                self.ratelimit_remaining = int(rem)
            except (TypeError, ValueError):
                pass
        rst = headers.get("X-RateLimit-Reset")
        if rst is not None:
            try:
                self.ratelimit_reset = int(rst)
            except (TypeError, ValueError):
                pass

    def _send(self, method, url, body=None):
        data = json.dumps(body, sort_keys=True).encode("utf-8") if body is not None else None
        req = urllib.request.Request(url, data=data, method=method, headers={
            "Authorization": "Bearer " + self._token,
            "Accept": "application/vnd.github+json",
            "X-GitHub-Api-Version": "2022-11-28",
            "Content-Type": "application/json",
        })
        try:
            with self._opener.open(req, timeout=30) as resp:
                code, raw = resp.status, resp.read(LIMIT + 1)
                self._record_ratelimit(resp.headers)
        except urllib.error.HTTPError as exc:
            code, raw = exc.code, exc.read(LIMIT + 1)
            self._record_ratelimit(exc.headers)
        except (urllib.error.URLError, OSError) as exc:
            raise ApiError(f"Connection failure on {method} {url}: {type(exc).__name__}") from None
        if len(raw) > LIMIT:
            raise ApiError("GitHub API response exceeded size limit.")
        payload = json.loads(raw) if raw.strip() else None
        return code, payload

    def graphql(self, query, variables):
        code, payload = self._send("POST", "https://api.github.com/graphql",
                                   {"query": query, "variables": variables})
        if code != 200 or not isinstance(payload, dict):
            raise ApiError(f"GraphQL HTTP {code}")
        if payload.get("errors"):
            msg = (payload["errors"][0] or {}).get("message", "unknown error")
            raise ApiError(f"GraphQL error: {msg}")
        return payload.get("data") or {}

    def get(self, path):
        code, payload = self._send("GET", f"https://api.github.com/repos/{REPO}/{path}")
        if code != 200:
            raise ApiError(f"GET {path} returned HTTP {code}")
        return payload

    def post(self, path, body):
        code, payload = self._send("POST", f"https://api.github.com/repos/{REPO}/{path}", body)
        if code not in (200, 201, 204):
            raise ApiError(f"POST {path} returned HTTP {code}")
        return payload


def load_config(path=None):
    config_path = Path(path) if path else Path(__file__).resolve().parents[1] / "landing/landing.json"
    data = json.loads(config_path.read_text(encoding="utf-8"))
    approvers = {int(uid) for uid in data.get("header_approvers", []) if type(uid) is int and uid > 0}
    refused = tuple(data.get("refused_branches", DEFAULT_REFUSED_PREFIXES))
    return approvers, refused


def allowed_file(path):
    if not isinstance(path, str) or not path or path.startswith("/") or ".." in path.split("/"):
        return False
    if path.casefold().startswith("src/code/tools/"):
        return False
    return path.startswith("src/") or path.startswith("include/")


def status_map(pr):
    commits = ((pr.get("commits") or {}).get("nodes") or [])
    if not commits:
        return {}
    status = ((commits[-1] or {}).get("commit") or {}).get("status") or {}
    contexts = status.get("contexts") or []
    return {c["context"]: c.get("state") for c in contexts if isinstance(c, dict) and c.get("context")}


def header_approved(pr, approvers):
    head_oid = pr.get("headRefOid")
    author_id = (pr.get("author") or {}).get("databaseId")
    reviews = ((pr.get("latestOpinionatedReviews") or {}).get("nodes") or [])
    for rev in reviews:
        if not isinstance(rev, dict) or rev.get("state") != "APPROVED":
            continue
        if (rev.get("commit") or {}).get("oid") != head_oid:
            continue
        reviewer_id = (rev.get("author") or {}).get("databaseId")
        if reviewer_id in approvers and reviewer_id != author_id:
            return True
    return False


def base_eligible(pr, refused_prefixes):
    number = pr.get("number")
    if pr.get("isDraft") or pr.get("isInMergeQueue") or pr.get("mergeable") == "CONFLICTING":
        return None
    if number in CANARY_PRS:
        return None
    head_ref = pr.get("headRefName") or ""
    if any(head_ref.startswith(prefix) for prefix in refused_prefixes):
        return None
    labels = {node.get("name") for node in ((pr.get("labels") or {}).get("nodes") or []) if isinstance(node, dict)}
    if HOLD_LABEL in labels:
        return None
    files_obj = pr.get("files") or {}
    nodes = files_obj.get("nodes") or []
    total_count = files_obj.get("totalCount", len(nodes))
    paths = [n.get("path", "") for n in nodes if isinstance(n, dict)]
    if not paths or total_count > MAX_FILES or len(paths) > MAX_FILES:
        return None
    if not all(allowed_file(p) for p in paths):
        return None
    return paths


def ready_to_enqueue(pr, paths, statuses, approvers):
    if statuses.get("BFME II / source rules") != "SUCCESS":
        return False
    if statuses.get("BFME II / Boat") != "SUCCESS":
        return False
    if statuses.get("BFME II / Inline virtual") == "FAILURE":
        return False
    if any(p.startswith("include/") for p in paths):
        if statuses.get("BFME II / Header rules") != "SUCCESS" and not header_approved(pr, approvers):
            return False
    return True


def needs_backfill(statuses):
    boat = statuses.get("BFME II / Boat")
    if boat not in (None, "PENDING", "ERROR"):
        return False
    if statuses.get("BFME II / source rules") == "FAILURE":
        return False
    return True


def list_open_prs(api):
    prs = []
    main_sha = None
    cursor = None
    for _ in range(30):
        data = api.graphql(OPEN_PRS_QUERY, {"owner": OWNER, "name": NAME, "cursor": cursor})
        repo = data.get("repository") or {}
        if main_sha is None:
            main_sha = ((repo.get("defaultBranchRef") or {}).get("target") or {}).get("oid")
        conn = repo.get("pullRequests") or {}
        prs.extend(conn.get("nodes") or [])
        page = conn.get("pageInfo") or {}
        if not page.get("hasNextPage") or not page.get("endCursor"):
            break
        cursor = page["endCursor"]
    return main_sha, prs


def active_boat_runs(api):
    total = 0
    for status in ("in_progress", "queued"):
        data = api.get(f"actions/workflows/managed-pr.yml/runs?status={status}&per_page=1")
        total += int((data or {}).get("total_count", 0))
    return total


def run(api=None, *, config_path=None, env=None):
    env = os.environ if env is None else env
    api = GitHubApi(env.get("BFME_QUEUE_TOKEN", "")) if api is None else api
    approvers, refused_prefixes = load_config(config_path)
    main_sha, prs = list_open_prs(api)

    enqueue_candidates = []
    backfill_candidates = []
    for pr in prs:
        paths = base_eligible(pr, refused_prefixes)
        if paths is None:
            continue
        statuses = status_map(pr)
        if ready_to_enqueue(pr, paths, statuses, approvers):
            enqueue_candidates.append(pr)
        elif needs_backfill(statuses):
            backfill_candidates.append(pr)

    enqueued = []
    enqueue_errors = []
    for pr in enqueue_candidates[:MAX_ENQUEUE]:
        if api.ratelimit_remaining is not None and api.ratelimit_remaining <= MIN_RATELIMIT_ENQUEUE:
            break
        num = pr["number"]
        try:
            api.graphql(ENQUEUE_MUTATION, {"id": pr["id"], "oid": pr["headRefOid"]})
            enqueued.append(num)
            print(f"Enqueued PR #{num} ({pr['headRefOid'][:12]})")
        except ApiError as exc:
            enqueue_errors.append((num, str(exc)))
            print(f"Skipped enqueue for PR #{num}: {exc}", file=sys.stderr)

    backfilled = []
    if (backfill_candidates
            and main_sha and HEX40.fullmatch(main_sha)
            and (api.ratelimit_remaining is None or api.ratelimit_remaining > MIN_RATELIMIT_BACKFILL)):
        try:
            active = active_boat_runs(api)
        except ApiError as exc:
            print(f"Skipping backfill; could not inspect active Boat runs: {exc}", file=sys.stderr)
            active = MAX_ACTIVE_BOAT
        slots = min(MAX_BACKFILL, max(0, MAX_ACTIVE_BOAT - active))
        if slots > 0:
            for pr in backfill_candidates[:slots]:
                if api.ratelimit_remaining is not None and api.ratelimit_remaining <= MIN_RATELIMIT_BACKFILL:
                    break
                num = pr["number"]
                oid = pr.get("headRefOid", "")
                if not HEX40.fullmatch(oid):
                    continue
                try:
                    api.post("dispatches", {
                        "event_type": "bfme-source-refresh",
                        "client_payload": {"number": num, "head": oid, "base": main_sha},
                    })
                    backfilled.append(num)
                    print(f"Dispatched bfme-source-refresh for PR #{num} ({oid[:12]})")
                except ApiError as exc:
                    print(f"Skipped backfill for PR #{num}: {exc}", file=sys.stderr)

    summary_path = env.get("GITHUB_STEP_SUMMARY")
    if summary_path:
        lines = [
            "### BFME II Merge Queue Enqueue",
            "",
            f"- **Open PRs scanned:** {len(prs)}",
            f"- **Enqueued:** {len(enqueued)} ({', '.join(f'#{n}' for n in enqueued) or 'none'})",
            f"- **Backfill dispatched:** {len(backfilled)} ({', '.join(f'#{n}' for n in backfilled) or 'none'})",
        ]
        if enqueue_errors:
            lines.append(f"- **Enqueue rejections:** {len(enqueue_errors)}")
        with Path(summary_path).open("a", encoding="utf-8", newline="\n") as stream:
            stream.write("\n".join(lines) + "\n")

    return {"enqueued": enqueued, "backfilled": backfilled, "errors": enqueue_errors}


def main():
    run()


if __name__ == "__main__":
    main()
