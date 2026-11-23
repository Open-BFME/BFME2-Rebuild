"""Bind and publish the protected GitHub source-check job."""

import base64
from dataclasses import dataclass
from http.client import HTTPException
import json
import os
from pathlib import Path
import re
import subprocess
import sys
from urllib.parse import quote
import tempfile
from urllib.error import HTTPError
import urllib.request

OID = re.compile(r"[0-9a-f]{40}")
DIGEST = re.compile(r"[0-9a-f]{64}")
REPOSITORY = "Open-BFME/BFME2-Rebuild"
REPOSITORY_ID = 1464323350
MAIN_REF = "refs/heads/main"
QUEUE_REF = re.compile(r"refs/heads/gh-readonly-queue/main/[A-Za-z0-9][A-Za-z0-9._-]{0,200}")
LABELS = (
    "Paths", "Generated annotations", "Forbidden constructs", "Stand-in types",
    "Comments", "Change scope", "One definition", "Address names",
)
RULES = dict(zip((f"G{i}" for i in range(1, 9)), LABELS, strict=True))
# Both jobs load these siblings from the selected protected checkout.
sys.path.insert(0, str(Path(__file__).resolve().parent))
from source_report import public_text, public_report

API = "https://api.github.com"
REASONS = {
    "unexpected-event": "This check needs an open pull request to main or a current merge queue candidate.",
    "tools-unavailable": "The protected source check could not be loaded from this main revision.",
    "current-state-unavailable": "GitHub could not verify the current pull request and protected main. Start a new check.",
    "pull-request-moved": "The author head changed, the pull request closed, or its target changed. Use a check for the current pull request.",
    "main-moved": "Main advanced while this check was running. Start a new check on current main.",
    "queue-moved": "The merge queue replaced this candidate; GitHub will check the replacement.",
    "queue-history": "The queued change is missing its recorded main commit; ask a maintainer to requeue it.",
    "fork-unavailable": "The source check could not read this fork without credentials while the repository is private.",
    "queue-private": "This source check for merge groups is enabled when the repository is public.",
    "missing-ref": "GitHub could not find the pull request commit or main branch. Check the branch and start a new run.",
    "fetch-unavailable": "GitHub's Git transfer did not complete. Start a new check.",
    "merge-history": "This check could not find one shared base commit for your branch and main. Rebase on main and push the updated branch.",
    "merge-conflict": "The author head conflicts with protected main. Resolve the conflict and push the updated branch.",
    "merge-unavailable": "Git could not compose the exact author head with protected main. Start a new check.",
    "merge-ref-mismatch": "GitHub and this check produced different merges for the same commits. Ask a maintainer to verify the pull request.",
    "check-unavailable": "The protected source check did not produce a verified result. Start a new check.",
    "status-unavailable": "GitHub did not acknowledge every source status. Start a new check.",
}


# bfme-remeasure: the compiler check's own intake. bfme-source-refresh: admission asks for the source rules of a
# fork commit on a main that moved since its last event (.github/workflows/rules.yml; no compiler machine).
DISPATCH_ACTIONS = ("bfme-remeasure", "bfme-source-refresh", "bfme-source-recheck")
# Aggregate status texts for what only the author can fix on this main; ci/admission/admit.py reads them.
CONFLICT_OUTCOME = "Conflicts with main: merge or rebase main into your branch and push"
HISTORY_OUTCOME = "No shared history with main: rebase your branch on main and push"
AUTHOR_OUTCOMES = {"merge-conflict": CONFLICT_OUTCOME, "merge-history": HISTORY_OUTCOME}


class CheckUnavailable(Exception):
    """Carry only fixed reason identifiers across the trust boundary."""

    def __init__(self, reason):
        super().__init__(reason)
        self.reason = reason


def is_oid(value):
    return isinstance(value, str) and OID.fullmatch(value) is not None


def read_json(path, limit):
    with Path(path).open("rb") as stream:
        data = stream.read(limit + 1)
    if len(data) > limit:
        raise ValueError("Input exceeds its size limit")
    return json.loads(data)


@dataclass(frozen=True)
class Binding:
    repo: str
    repo_id: int
    number: int | None
    base: str
    head: str
    tools_commit: str
    queue_ref: str | None = None

    @property
    def identity(self):
        return {"base": self.base, "head": self.head, "tools_commit": self.tools_commit}

    def check_pr(self, pr):
        try:
            valid = (pr["number"] == self.number and pr["state"] == "open"
                     and pr["base"]["ref"] == "main"
                     and pr["base"]["repo"]["id"] == self.repo_id
                     and pr["head"]["sha"] == self.head)
        except (KeyError, TypeError):
            valid = False
        if not valid:
            raise CheckUnavailable("pull-request-moved")


def event(env):
    try:
        repo = env["GITHUB_REPOSITORY"]
        repo_id = int(env["GITHUB_REPOSITORY_ID"])
        sha = env["GITHUB_SHA"]
        name = env["GITHUB_EVENT_NAME"]
        ref = env["GITHUB_REF"]
        if (repo.casefold() != REPOSITORY.casefold() or repo_id != REPOSITORY_ID or not is_oid(sha)
                or env["GITHUB_SERVER_URL"] != "https://github.com"
                or env["GITHUB_API_URL"] != API):
            raise ValueError
        if name == "merge_group":
            group_event = read_json(env["GITHUB_EVENT_PATH"], 2 * 1024**2)
            group = group_event["merge_group"]
            base = group["base_sha"]
            if (group_event["action"] != "checks_requested"
                    or group_event["repository"]["id"] != repo_id
                    or group["base_ref"] != MAIN_REF or not is_oid(base) or base == sha
                    or group["head_sha"] != sha or group["head_ref"] != ref
                    or not QUEUE_REF.fullmatch(ref) or ".." in ref or ref.endswith((".", ".lock"))):
                raise ValueError
            return Binding(repo, repo_id, None, base, sha, base, ref)
        if ref != MAIN_REF:
            raise ValueError
        if name == "pull_request_target":
            pr = read_json(env["GITHUB_EVENT_PATH"], 2 * 1024**2)["pull_request"]
            binding = Binding(repo, repo_id, pr["number"], sha, pr["head"]["sha"], sha)
            binding.check_pr(pr)
        elif name == "repository_dispatch":
            event_data = read_json(env["GITHUB_EVENT_PATH"], 2 * 1024**2)
            payload = event_data["client_payload"]
            if (event_data["action"] not in DISPATCH_ACTIONS or not isinstance(payload, dict)
                    or set(payload) != {"number", "head", "base"}
                    or not is_oid(payload["base"]) or not is_oid(payload["head"])):
                raise ValueError
            if payload["base"] != sha:
                raise CheckUnavailable("main-moved")
            binding = Binding(repo, repo_id, payload["number"], sha, payload["head"], sha)
        else:
            raise ValueError
        if (type(binding.number) is not int or not 0 < binding.number < 10**9
                or not is_oid(binding.base) or not is_oid(binding.head)):
            raise ValueError
        return binding
    except CheckUnavailable as exc:
        if exc.reason == "main-moved":
            raise
        raise CheckUnavailable("unexpected-event") from None
    except (KeyError, TypeError, ValueError, OSError):
        raise CheckUnavailable("unexpected-event") from None


class NoRedirect(urllib.request.HTTPRedirectHandler):
    def redirect_request(self, req, fp, code, msg, headers, newurl):
        return None


class GitHub:
    def __init__(self, binding, token):
        if not token or not re.fullmatch(r"[A-Za-z0-9_.-]{15,2048}", token):
            raise CheckUnavailable("current-state-unavailable")
        self.binding = binding
        self.token = token
        self.opener = urllib.request.build_opener(urllib.request.ProxyHandler({}), NoRedirect())

    def call(self, path, body=None, *, missing=None):
        suffix = f"/{path}" if path else ""
        request = urllib.request.Request(
            f"{API}/repos/{self.binding.repo}{suffix}",
            data=json.dumps(body).encode("ascii") if body is not None else None,
            headers={"Authorization": f"Bearer {self.token}",
                     "Accept": "application/vnd.github+json", "Content-Type": "application/json"},
        )
        reason = "status-unavailable" if body is not None else "current-state-unavailable"
        try:
            with self.opener.open(request, timeout=25) as response:
                if response.status != (201 if body is not None else 200):
                    raise CheckUnavailable(reason)
                raw = response.read(2 * 1024**2 + 1)
            if len(raw) > 2 * 1024**2:
                raise CheckUnavailable(reason)
            return json.loads(raw)
        except HTTPError as exc:
            raise CheckUnavailable(missing if exc.code == 404 and missing else reason) from None
        except (HTTPException, OSError, ValueError):
            raise CheckUnavailable(reason) from None

    def current(self):
        binding = self.binding
        pr = None
        if binding.number is not None:
            pr = self.call(f"pulls/{binding.number}")
            binding.check_pr(pr)
        try:
            if self.call("git/ref/heads/main")["object"]["sha"] != binding.tools_commit:
                raise CheckUnavailable("main-moved")
            if binding.queue_ref:
                path = "git/ref/" + quote(binding.queue_ref.removeprefix("refs/"), safe="/")
                if self.call(path, missing="queue-moved")["object"]["sha"] != binding.head:
                    raise CheckUnavailable("queue-moved")
        except (KeyError, TypeError):
            raise CheckUnavailable("current-state-unavailable") from None
        return pr

    def transport(self, pr):
        metadata = self.call("")
        if (not isinstance(metadata, dict) or metadata.get("id") != self.binding.repo_id
                or type(metadata.get("private")) is not bool):
            raise CheckUnavailable("current-state-unavailable")
        if not metadata["private"]:
            return "public"
        if self.binding.queue_ref:
            raise CheckUnavailable("queue-private")
        # In a private repository, allow credentialed Git only for changes
        # GitHub confirms live in this same repository.
        head_repo = pr.get("head", {}).get("repo")
        if not isinstance(head_repo, dict) or head_repo.get("id") != self.binding.repo_id:
            raise CheckUnavailable("fork-unavailable")
        return "private"

    def status(self, name, state, run, outcome=None):
        if not re.fullmatch(r"[1-9][0-9]{0,20}", run):
            raise CheckUnavailable("status-unavailable")
        context = f"BFME II / {name}"
        binding = self.binding
        label = RULES.get(name, "Source rules")
        outcome = outcome or {"success": f"{label} passed", "failure": f"Fix needed for {label.lower()}",
                              "error": f"{label} could not finish"}[state]
        result = self.call(
            f"statuses/{binding.head}",
            {"state": state, "context": context, "description": f"{outcome} (main {binding.base[:12]})",
             "target_url": f"https://github.com/{binding.repo}/actions/runs/{run}"},
        )
        if not isinstance(result, dict) or result.get("state") != state or result.get("context") != context:
            raise CheckUnavailable("status-unavailable")


def isolated_env(home):
    return {"HOME": str(home), "PATH": "/usr/bin:/bin", "LC_ALL": "C",
            "GIT_CONFIG_NOSYSTEM": "1", "GIT_CONFIG_GLOBAL": "/dev/null",
            "GIT_ATTR_NOSYSTEM": "1", "GIT_NO_REPLACE_OBJECTS": "1", "GIT_TERMINAL_PROMPT": "0"}


class Git:
    def __init__(self, repo):
        self.repo = Path(repo)
        self.environment = isolated_env(self.repo.parent)
        for key in ("SSL_CERT_FILE", "SSL_CERT_DIR"):
            if key in os.environ:
                self.environment[key] = os.environ[key]

    def run(self, *args, extra_env=None, quota=False, timeout=120):
        limit = ["prlimit", "--fsize=268435456:268435456", "--as=1610612736:1610612736", "--"] if quota else []
        return subprocess.run(
            [*limit, "git", "-c", "core.hooksPath=/dev/null", "-c", "protocol.ext.allow=never",
             "-c", "credential.helper=", "-c", "http.followRedirects=false", "-c", "fetch.fsckObjects=true",
             "-c", "fetch.unpackLimit=1", "-c", "gc.auto=0", "--git-dir", str(self.repo), *args],
            env=self.environment | (extra_env or {}), capture_output=True, timeout=timeout,
        )

    def value(self, *args, optional=False):
        result = self.run(*args)
        if result.returncode and optional:
            return None
        if result.returncode:
            raise CheckUnavailable("merge-unavailable")
        return result.stdout.decode("ascii").strip()

    def rev(self, ref, kind="commit", optional=False):
        result = self.value("rev-parse", "--verify", f"{ref}^{{{kind}}}", optional=optional)
        if result is not None and not is_oid(result):
            raise CheckUnavailable("merge-unavailable")
        return result


def receive(git, binding, remote, token=None):
    if git.run("init", "-q", "--bare", "--template=", timeout=15).returncode:
        raise CheckUnavailable("fetch-unavailable")
    auth = {}
    if token:
        basic = base64.b64encode(f"x-access-token:{token}".encode("ascii")).decode("ascii")
        auth = {"GIT_CONFIG_COUNT": "1", "GIT_CONFIG_KEY_0": "http.https://github.com/.extraheader",
                "GIT_CONFIG_VALUE_0": f"AUTHORIZATION: basic {basic}"}
    number = binding.number
    base_ref = "+refs/heads/main:refs/source-rules/main"
    candidate_ref = binding.queue_ref or f"refs/pull/{number}/head"
    head_ref = f"+{candidate_ref}:refs/source-rules/head"
    options = ("fetch", "-q", "--depth=128", "--no-tags", "--no-recurse-submodules", "--no-write-fetch-head")
    run_options = {"extra_env": auth, "quota": True}
    received = git.run(*options, "--", remote, base_ref, head_ref, **run_options)
    if received.returncode:
        missing = b"remote ref" in received.stderr.lower()
        if missing:
            raise CheckUnavailable("queue-moved" if binding.queue_ref else "missing-ref")
        raise CheckUnavailable("fetch-unavailable")
    verify_received(git, binding)
    if binding.queue_ref:
        return
    # This ref is optional. Its absence must never cancel an otherwise usable fetch.
    try:
        git.run(*options, "--", remote, f"+refs/pull/{number}/merge:refs/source-rules/merge",
                timeout=30, **run_options)
    except subprocess.TimeoutExpired:
        pass


def verify_received(git, binding):
    refs = {"refs/source-rules/main": (binding.tools_commit, "main-moved"),
            "refs/source-rules/head": (binding.head, "queue-moved" if binding.queue_ref else "pull-request-moved")}
    for ref, (expected, reason) in refs.items():
        actual = git.rev(ref, optional=True)
        if actual is None:
            raise CheckUnavailable("missing-ref")
        if actual != expected:
            raise CheckUnavailable(reason)


def compose(git, binding):
    """Use the comparison runner's unique ancestor and three-way tree merge."""
    base, head = binding.base, binding.head
    if binding.queue_ref:
        ancestry = git.run("merge-base", "--is-ancestor", base, head)
        if ancestry.returncode:
            raise CheckUnavailable("queue-history")
        # GitHub already composed this candidate. Use its tree unchanged.
        return git.rev(head, "tree"), "queue-candidate"
    ancestors = git.run("merge-base", "--all", base, head)
    if ancestors.returncode not in {0, 1}:
        raise CheckUnavailable("merge-unavailable")
    parents = ancestors.stdout.decode("ascii").splitlines()
    if len(parents) != 1 or not is_oid(parents[0]):
        raise CheckUnavailable("merge-history")
    if parents[0] == base:
        tree = git.rev(head, "tree")
    else:
        merged = git.run("merge-tree", "--write-tree", "--no-messages", base, head)
        if merged.returncode == 1:
            raise CheckUnavailable("merge-conflict")
        first = merged.stdout.decode("ascii").splitlines()
        if merged.returncode or len(first) != 1 or not is_oid(first[0]):
            raise CheckUnavailable("merge-unavailable")
        tree = git.rev(first[0], "tree")
    reported = git.rev("refs/source-rules/merge", optional=True)
    origin = "composed-missing-merge-ref"
    if reported:
        origin = "composed-stale-merge-ref"
        commit = git.run("cat-file", "commit", reported)
        if not commit.returncode:
            header = commit.stdout.split(b"\n\n", 1)[0].splitlines()
            recorded = [line[7:] for line in header if line.startswith(b"parent ")]
            if recorded == [base.encode("ascii"), head.encode("ascii")]:
                if git.rev(reported, "tree") != tree:
                    raise CheckUnavailable("merge-ref-mismatch")
                origin = "verified-merge-ref"
    return tree, origin


def output(env, name, value):
    with Path(env["GITHUB_OUTPUT"]).open("a", encoding="utf-8") as stream:
        print(f"{name}={value}", file=stream)


def summary(env, report):
    with Path(env["GITHUB_STEP_SUMMARY"]).open("a", encoding="utf-8") as stream:
        stream.write(report)


def explanation(reason):
    return f"\n**Binding [{reason}]** {REASONS[reason]}\n"


def error_table(reason):
    rows = [f"| {rule} {label} | Error |" for rule, label in RULES.items()]
    rows.insert(4, "| G4b Type swaps | Unavailable (advisory) |")
    return "| Rule | Result |\n| --- | --- |\n" + "\n".join(rows) + "\n" + explanation(reason)


def pending(binding, reason):
    return (binding.identity if binding else {}) | {
        "tree": None, "policy_sha256": None, "state": "pending", "reason": reason,
        "overall": "error", "states": dict.fromkeys(RULES, "error"),
    }


def verified_tools(binding):
    checkout = Path(__file__).resolve().parents[2] / ".git"
    if Git(checkout).rev("HEAD", optional=True) != binding.tools_commit:
        raise CheckUnavailable("tools-unavailable")


def authorize(env):
    """Check only GitHub metadata and select the least-privileged transfer."""
    try:
        binding = event(env)
        verified_tools(binding)
        api = GitHub(binding, env.pop("GH_TOKEN", None))
        output(env, "transport", api.transport(api.current()))
    except CheckUnavailable as exc:
        output(env, "reason", exc.reason)
    except (AttributeError, KeyError, OSError, TypeError, ValueError, subprocess.SubprocessError):
        output(env, "reason", "current-state-unavailable")


def acquire(env):
    try:
        binding = event(env)
        verified_tools(binding)
        transport, token = env.get("SOURCE_TRANSPORT"), env.pop("GH_TOKEN", None)
        if transport == "public":
            if token:
                raise CheckUnavailable("fetch-unavailable")
        elif transport != "private" or binding.queue_ref or not token or not re.fullmatch(r"[A-Za-z0-9_.-]{15,2048}", token):
            raise CheckUnavailable("fetch-unavailable")
        scratch = Path(tempfile.mkdtemp(prefix="bfme-rules.", dir=env["RUNNER_TEMP"]))
        receive(Git(scratch / "objects.git"), binding, f"https://github.com/{binding.repo}.git", token)
        output(env, "scratch", scratch)
    except CheckUnavailable as exc:
        output(env, "reason", exc.reason)
    except (OSError, KeyError, ValueError, subprocess.SubprocessError):
        output(env, "reason", "fetch-unavailable")


def append_advisory(report, repo, base, tree, scratch):
    """Collect an independent advisory. An unavailable result leaves all statuses intact."""
    outcome, details = "Unavailable (advisory)", []
    try:
        result = subprocess.run(
            [sys.executable, "-E", "-S", "-B", str(Path(__file__).with_name("shadow_types.py")),
             str(repo.repo), base, tree],
            env=isolated_env(scratch), cwd=scratch, timeout=12, capture_output=True, check=True,
        )
        findings = json.loads(result.stdout)
        if not isinstance(findings, list):
            raise ValueError
        for finding in findings[:20]:
            path = public_text(finding["path"][:220])
            name = public_text(finding["name"][:96])
            before = public_text(finding["old_type"][:140])
            after = public_text(finding["new_type"][:140])
            line = finding["line"]
            if type(line) is not int or line <= 0:
                raise ValueError
            details.append(
                f"- <code>{path}:{line}</code> **G4b advisory** "
                f"<code>{name}</code> changed from <code>{before}</code> to <code>{after}</code>. "
                "Check the declaration against the known type and layout."
            )
        if len(findings) > len(details):
            details.append(f"- **G4b advisory** {len(findings) - len(details)} more type changes need review.")
        outcome = f"Review {len(findings)} (advisory)" if findings else "No findings (advisory)"
    except (OSError, KeyError, TypeError, ValueError, subprocess.SubprocessError):
        details = []
    lines = report.splitlines()
    for index, line in enumerate(lines):
        if line.startswith("| G4 "):
            lines.insert(index + 1, f"| G4b Type swaps | {outcome} |")
            break
    if details:
        lines.extend(("", *details))
    return "\n".join(lines) + "\n"


def evaluate_bound(binding, scratch):
    repo = Git(scratch / "objects.git")
    verify_received(repo, binding)
    tree, projection = compose(repo, binding)
    result_file = scratch / "result.json"
    proc = subprocess.run(
        [sys.executable, "-E", "-S", "-B", str(Path(__file__).with_name("gate.py")),
         str(repo.repo), binding.base, tree, "--json", str(result_file), "--report", os.devnull],
        env=isolated_env(scratch), cwd=scratch, capture_output=True, timeout=480,
    )
    expected = {0: "pass", 1: "fail"}.get(proc.returncode)
    if expected is None:
        raise CheckUnavailable("check-unavailable")
    result = read_json(result_file, 32 * 1024**2)
    items = result["gates"]
    statuses = {item["id"]: item["status"] for item in items}
    status_map = {"pass": "success", "fail": "failure", "blocked": "failure"}
    if (len(items) != len(RULES) or set(statuses) != set(RULES)
            or not set(statuses.values()) <= set(status_map) or result["status"] != expected
            or (expected == "pass") != all(state == "pass" for state in statuses.values())
            or result["base_tree"] != repo.rev(binding.base, "tree") or result["head_tree"] != tree
            or not DIGEST.fullmatch(result["policy_sha256"])):
        raise CheckUnavailable("check-unavailable")

    if binding.queue_ref and statuses["G1"] == "pass" and statuses["G6"] != "pass":
        statuses["G6"] = "pass"
        items = [
            {**item, "status": "pass", "findings": []} if item["id"] == "G6" else item
            for item in items
        ]
        expected = "pass" if all(state == "pass" for state in statuses.values()) else "fail"

    report = public_report(items, RULES)
    report = append_advisory(report, repo, binding.base, tree, scratch)
    return binding.identity | {
        "tree": tree, "projection": projection, "policy_sha256": result["policy_sha256"], "state": expected,
        "overall": status_map[expected], "states": {rule: status_map[statuses[rule]] for rule in RULES},
    }, report


def evaluate(env):
    binding = None
    try:
        binding = event(env)
        reason = env.get("FETCH_REASON")
        if reason in REASONS:
            raise CheckUnavailable(reason)
        if not env.get("SOURCE_OBJECTS"):
            raise CheckUnavailable("fetch-unavailable")
        receipt, report = evaluate_bound(binding, Path(env["SOURCE_OBJECTS"]))
    except CheckUnavailable as exc:
        receipt, report = pending(binding, exc.reason), error_table(exc.reason)
    except (KeyError, TypeError, ValueError, OSError, subprocess.SubprocessError):
        receipt, report = pending(binding, "check-unavailable"), error_table("check-unavailable")
    summary(env, report)
    output(env, "status", json.dumps(receipt, sort_keys=True, separators=(",", ":")))


def evaluation(receipt, binding):
    if not isinstance(receipt, dict) or any(receipt.get(key) != value for key, value in binding.identity.items()):
        raise ValueError
    states = receipt["states"]
    overall = receipt["overall"]
    projections = {"queue-candidate"} if binding.queue_ref else {
        "verified-merge-ref", "composed-missing-merge-ref", "composed-stale-merge-ref",
    }
    verdict_keys = binding.identity.keys() | {"tree", "projection", "policy_sha256", "state", "overall", "states"}
    valid_verdict = (
        receipt.keys() == verdict_keys and overall in {"success", "failure"} and set(states) == set(RULES)
        and set(states.values()) <= {"success", "failure"}
        and (overall == "success") == all(state == "success" for state in states.values())
        and receipt["state"] == {"success": "pass", "failure": "fail"}[overall]
        and is_oid(receipt["tree"]) and DIGEST.fullmatch(receipt["policy_sha256"])
        and receipt["projection"] in projections
    )
    valid_error = receipt == pending(binding, receipt.get("reason")) and receipt.get("reason") in REASONS
    if not (valid_verdict or valid_error):
        raise ValueError
    return receipt


def publish(env):
    binding = None
    try:
        supplied = json.loads(env.get("RULE_STATUS", ""))
    except ValueError:
        supplied = None
    prior_reason = supplied.get("reason") if isinstance(supplied, dict) else None
    try:
        binding = event(env)
        verified_tools(binding)
        try:
            receipt = evaluation(supplied, binding)
        except (AttributeError, KeyError, TypeError, ValueError):
            raise CheckUnavailable("check-unavailable") from None
        api = GitHub(binding, env.pop("GH_TOKEN", None))
        api.current()
        publication_failed = False
        for rule in RULES:
            try:
                api.status(rule, receipt["states"][rule], env["GITHUB_RUN_ID"])
            except CheckUnavailable:
                publication_failed = True
        # Repeat the comparison job's freshness check before the aggregate.
        api.current()
        named = None if publication_failed else AUTHOR_OUTCOMES.get(receipt.get("reason"))
        api.status("source rules", "error" if publication_failed else receipt["overall"], env["GITHUB_RUN_ID"], named)
        if publication_failed:
            raise CheckUnavailable("status-unavailable")
    except CheckUnavailable as exc:
        if exc.reason != prior_reason:
            summary(env, explanation(exc.reason))
        receipt = pending(binding, exc.reason)
    except (KeyError, TypeError, ValueError, OSError):
        reason = "status-unavailable"
        summary(env, explanation(reason))
        receipt = pending(binding, reason)
    output(env, "receipt", json.dumps(receipt, sort_keys=True, separators=(",", ":")))
    if receipt["state"] == "pending":
        reason = receipt.get("reason", "status-unavailable")
        print(f"Expected a current source decision. Found: {REASONS[reason]}", file=sys.stderr)
    elif receipt["state"] == "fail":
        names = ", ".join(rule for rule, state in receipt["states"].items() if state == "failure")
        print(f"Expected all source rules to pass. Found a refusal in {names}.", file=sys.stderr)
    return 0 if receipt["state"] == "pass" else 1


def main():
    os.umask(0o077)
    actions = {"authorize": authorize, "fetch": acquire, "evaluate": evaluate, "publish": publish}
    return actions[sys.argv[1]](os.environ)


if __name__ == "__main__":
    sys.exit(main())
