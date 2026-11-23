"""Repository-scoped GitHub status API and protected event binding."""
import os
import re
import urllib.error
import urllib.request

from common import CONTEXT, HEX40, REPO, REPO_ID, encoded, read


class Superseded(Exception):
    pass


class NoRedirect(urllib.request.HTTPRedirectHandler):
    def redirect_request(self, req, fp, code, msg, headers, newurl):
        return None


DISJOINT_TU_PREFIXES = ("src/Code/", "src/unk/", "src/nbench/")
DISJOINT_TU_FORBIDDEN_PREFIXES = ("src/code/tools/",)
DISJOINT_TU_SUFFIXES = (".c", ".cpp")
MAX_ANCESTOR_AHEAD = 32
BUILD_NEUTRAL_PREFIXES = (
    "docs/", "worklist/", "ci/nightly/", "ci/progress/", "ci/alarms/",
    "ci/public-text/", "ci/managed-check/tests/", "ci/landing/tests/", "ci/admission/tests/",
)
BUILD_NEUTRAL_WORKFLOWS = frozenset({
    ".github/workflows/nightly.yml", ".github/workflows/nightly-trial.yml",
    ".github/workflows/progress.yml", ".github/workflows/ci-tests.yml",
})


def is_build_neutral_path(name):
    return (isinstance(name, str) and bool(name)
            and ".." not in name.split("/")
            and (name.startswith(BUILD_NEUTRAL_PREFIXES)
                 or name in BUILD_NEUTRAL_WORKFLOWS
                 or ("/" not in name and name.endswith(".md"))))


def is_disjoint_tu_path(name):
    if not isinstance(name, str) or not name or name.startswith("/") or ".." in name.split("/"):
        return False
    lower = name.lower()
    if lower.startswith(DISJOINT_TU_FORBIDDEN_PREFIXES):
        return False
    return name.startswith(DISJOINT_TU_PREFIXES) and lower.endswith(DISJOINT_TU_SUFFIXES)


def event(api=None, *, allow_moved_main=False):
    if (os.environ.get("GITHUB_REPOSITORY") != REPO
            or os.environ.get("GITHUB_REPOSITORY_ID") != str(REPO_ID)
            or os.environ.get("GITHUB_REF") != "refs/heads/main"
            or os.environ.get("GITHUB_EVENT_NAME") not in {"pull_request_target", "repository_dispatch"}):
        raise ValueError("This workflow requires the approved protected repository and main.")
    payload = read(os.environ["GITHUB_EVENT_PATH"], 2 * 1024**2)
    base = os.environ["GITHUB_SHA"]
    if not HEX40.fullmatch(base):
        raise ValueError("Expected a full protected main revision. Found an invalid workflow SHA.")
    if os.environ["GITHUB_EVENT_NAME"] == "repository_dispatch":
        request = payload.get("client_payload")
        if (payload.get("action") != "bfme-remeasure" or not isinstance(request, dict)
                or set(request) != {"number", "head", "base"}
                or type(request["number"]) is not int or not 0 < request["number"] < 10**9
                or not isinstance(request["head"], str) or not HEX40.fullmatch(request["head"])
                or request["base"] != base or api is None):
            raise ValueError("Expected a bfme-remeasure request for this protected main and a full PR head. Found an invalid or stale dispatch.")
        number, head = request["number"], request["head"]
        pr = api.call(f"pulls/{number}")
        if ((pr.get("head") or {}).get("sha") != head
                or pr.get("state") != "open"
                or (pr.get("base") or {}).get("ref") != "main"):
            raise Superseded()
        if not allow_moved_main and api.call("git/ref/heads/main")["object"]["sha"] != base:
            raise Superseded()
    else:
        pr = payload["pull_request"]
        head, number = pr["head"]["sha"], pr["number"]
    validate_pr(pr, number, head)
    return {"number": number, "base": base, "head": head, "login": "bfme-boat", "uid": 1,
            "event_association": pr.get("author_association")}


def validate_pr(pr, number, head):
    if (pr["state"] != "open" or type(number) is not int or not 0 < number < 10**9
            or pr["number"] != number or pr["base"]["repo"]["id"] != REPO_ID
            or pr["base"]["ref"] != "main" or not HEX40.fullmatch(head)):
        raise ValueError("The pull request metadata is outside the managed check scope.")


class GitHub:
    def __init__(self):
        self.token = os.environ.pop("BFME_STATUS_TOKEN", None) or os.environ.pop("BFME_READ_TOKEN", None)
        if not self.token or not re.fullmatch(r"[A-Za-zA-Z0-9_.-]{15,2048}", self.token):
            raise ValueError("This protected step requires its repository token.")
        self.api = urllib.request.build_opener(urllib.request.ProxyHandler({}), NoRedirect())

    def call(self, path, value=None):
        import json
        request = urllib.request.Request("https://api.github.com/repos/" + REPO + "/" + path,
                                         data=encoded(value) if value is not None else None,
                                         headers={"Authorization": "Bearer " + self.token,
                                                  "Accept": "application/vnd.github+json",
                                                  "Content-Type": "application/json"})
        try:
            with self.api.open(request, timeout=25) as response:
                raw = response.read(2 * 1024**2 + 1)
        except urllib.error.HTTPError as exc:
            raise ValueError(f"The GitHub API returned HTTP {exc.code}.") from None
        if len(raw) > 2 * 1024**2:
            raise ValueError("The GitHub API response exceeded the configured limit.")
        return json.loads(raw)

    def current(self, binding):
        number, head, base = (binding[key] for key in ("number", "head", "base"))
        pr = self.call(f"pulls/{number}")
        if pr["state"] != "open" or pr["base"]["ref"] != "main":
            raise Superseded()
        validate_pr(pr, number, head)
        if (pr["head"]["sha"] != head or self.call("git/ref/heads/main")["object"]["sha"] != base):
            raise Superseded()
        return pr

    def disjoint_ancestor_ok(self, binding, actual_main):
        """Return True iff binding['base'] is a disjoint ancestor of actual_main for a narrow disjoint-TU PR."""
        number, base = binding["number"], binding["base"]
        if not HEX40.fullmatch(base) or not HEX40.fullmatch(actual_main) or base == actual_main:
            return False
        try:
            entries = self.call(f"pulls/{number}/files?per_page=100&page=1")
        except ValueError:
            return False
        if not isinstance(entries, list) or not (0 < len(entries) <= 8):
            return False
        pr_files = []
        for item in entries:
            if not isinstance(item, dict):
                return False
            fname = item.get("filename", "")
            if item.get("previous_filename"):
                return False
            if item.get("status") not in {"modified", "added"} or not is_disjoint_tu_path(fname):
                return False
            pr_files.append(fname)
        try:
            cmp = self.call(f"compare/{base}...{actual_main}")
        except ValueError:
            return False
        if not isinstance(cmp, dict):
            return False
        if cmp.get("status") != "ahead" or cmp.get("behind_by") != 0:
            return False
        ahead_by = cmp.get("ahead_by")
        if type(ahead_by) is not int or not 1 <= ahead_by <= MAX_ANCESTOR_AHEAD:
            return False
        base_sha = (cmp.get("base_commit") or {}).get("sha", "")
        merge_base_sha = (cmp.get("merge_base_commit") or {}).get("sha", "")
        if base_sha != base or merge_base_sha != base:
            return False
        intervening = cmp.get("files")
        if not isinstance(intervening, list) or not (0 < len(intervening) < 300):
            return False
        intervening_lower = set()
        for item in intervening:
            if not isinstance(item, dict):
                return False
            fname = item.get("filename", "")
            prev = item.get("previous_filename")
            if is_build_neutral_path(fname) and (not prev or is_build_neutral_path(prev)):
                continue
            if item.get("status") != "modified" or prev or not is_disjoint_tu_path(fname):
                return False
            intervening_lower.add(fname.lower())
        pr_lower = {name.lower() for name in pr_files}
        if intervening_lower & pr_lower:
            return False
        return True

    def current_or_disjoint(self, binding):
        number, head, base = (binding[key] for key in ("number", "head", "base"))
        pr = self.call(f"pulls/{number}")
        if pr["state"] != "open" or pr["base"]["ref"] != "main":
            raise Superseded()
        validate_pr(pr, number, head)
        if pr["head"]["sha"] != head:
            raise Superseded()
        actual_main = self.call("git/ref/heads/main")["object"]["sha"]
        if actual_main != base and not self.disjoint_ancestor_ok(binding, actual_main):
            raise Superseded()
        return pr

    def status(self, binding, state, text, *, context=CONTEXT):
        run = os.environ["GITHUB_RUN_ID"]
        if not re.fullmatch(r"[1-9][0-9]{0,20}", run):
            raise ValueError("The Actions run ID is invalid.")
        result = self.call("statuses/" + binding["head"], {"state": state, "context": context,
                           "description": text, "target_url": f"https://github.com/{REPO}/actions/runs/{run}"})
        if result.get("state") != state or result.get("context") != context:
            raise ValueError("GitHub did not acknowledge the expected commit status.")


def admitted(pr, event_association=None):
    head_repo = pr.get("head", {}).get("repo") or {}
    roles = {"OWNER", "MEMBER", "COLLABORATOR"}
    return (head_repo.get("id") == REPO_ID
            or pr.get("author_association") in roles
            or event_association in roles
            or any(label.get("name") == "boat-ok" for label in pr.get("labels", ())))

