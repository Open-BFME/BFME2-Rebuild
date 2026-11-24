"""Check the protected binding of an automatic compiler reservation."""

import json
import os
from pathlib import Path
import re
import sys

from common import HEX40, HEX64, REPO, REPO_ID, read
from github_status import admitted

OWNER_ID = 1760627
OWNER_LOGIN = "Ancalgonn"
TRUSTED_KIND = "trusted-narrow"
TRUSTED_WIDE_KIND = "trusted-wide"
BOT_ID = 41898282
BOT_LOGIN = "github-actions[bot]"
WORKFLOW = ".github/workflows/admission.yml"
RUN_ID = re.compile(r"[1-9][0-9]{0,19}\Z")
TRUSTED_ROLES = {"OWNER", "MEMBER", "COLLABORATOR"}


def enabled(env):
    return env.get("BFME_AUTO_ADMISSION") == "yes" and env.get("BFME_PR_COMPACT") == "yes"


def wide_enabled(env):
    return enabled(env) and env.get("BFME_AUTO_HEADERS") == "yes"


def budget_configured(env, kind="narrow"):
    prefix = "BFME_ADMISSION_DAILY_" if kind == "narrow" else "BFME_ADMISSION_WIDE_DAILY_"
    limits = [env.get(prefix + key, "") for key in ("TOTAL", "PER_AUTHOR")]
    return (all(re.fullmatch(r"[1-9][0-9]{0,4}", value) for value in limits)
            and 0 < int(limits[1]) <= int(limits[0]) <= 5040)


def reservation_kind(pr):
    repository = ((pr.get("head") or {}).get("repo") or {}).get("id")
    if type(repository) is not int or repository <= 0:
        return None
    if repository == REPO_ID or pr.get("author_association") in TRUSTED_ROLES:
        return TRUSTED_KIND
    return "narrow"


def outside(pr):
    return reservation_kind(pr) == "narrow"


def direct_starter(env):
    actor = env.get("GITHUB_ACTOR_ID")
    return ((actor == str(OWNER_ID) or
             (actor == str(BOT_ID) and env.get("GITHUB_EVENT_NAME") == "repository_dispatch"))
            and env.get("BFME_TRIGGERING_ACTOR") in {OWNER_LOGIN, BOT_LOGIN})


def bot_dispatch(env):
    return (env.get("GITHUB_EVENT_NAME") == "repository_dispatch"
            and env.get("GITHUB_ACTOR_ID") == str(BOT_ID)
            and env.get("BFME_TRIGGERING_ACTOR") == BOT_LOGIN
            and env.get("GITHUB_RUN_ATTEMPT") == "1"
            and RUN_ID.fullmatch(env.get("GITHUB_RUN_ID", "")) is not None)


def manual_label(pr, env):
    if env.get("GITHUB_EVENT_NAME") != "pull_request_target":
        return False
    data = read(env["GITHUB_EVENT_PATH"], 2 * 1024**2)
    return (data.get("action") == "labeled" and (data.get("label") or {}).get("name") == "boat-ok"
            and any(label.get("name") == "boat-ok" for label in pr.get("labels") or ()))


def authority(pr, binding, env):
    starter = direct_starter(env)
    kind = reservation_kind(pr)
    if kind is None:
        return {"approved": False}
    if not enabled(env):
        if kind == TRUSTED_KIND:
            return {"approved": starter and admitted(pr, binding["event_association"])}
        # The owner can approve the exact fork head with a fresh label event.
        # Neither a foreign synchronize event nor a bot dispatch inherits it.
        return {"approved": starter and manual_label(pr, env)}
    if starter and env.get("GITHUB_EVENT_NAME") == "pull_request_target":
        if kind == TRUSTED_KIND and admitted(pr, binding["event_association"]) or manual_label(pr, env):
            return {"approved": True}
    author = (pr.get("user") or {}).get("id")
    decision = {"approved": False, "automatic_pending": True}
    if wide_enabled(env):
        decision["automatic_wide_allowed"] = True
    if pr.get("draft") is False and type(author) is int and author > 0 and bot_dispatch(env):
        decision["automatic_author"] = author
    return decision


def narrow(plan):
    return (0 < len(plan["request_paths"]) <= 8 and bool(plan["changed_paths"])
            and all(path.startswith("src/") for path in plan["changed_paths"]))


def wide_eligible(plan):
    changed = plan.get("changed_paths") or ()
    return (0 < len(changed) <= 60
            and any(path.startswith("include/") for path in changed)
            and all(path.startswith(("src/", "include/")) for path in changed))


def verified_issuer(api, reservation, binding, author, tree, kind):
    if (not isinstance(reservation, dict) or reservation.get("author") != author
            or reservation.get("tree") != tree or reservation.get("kind") != kind
            or any(reservation.get(key) != binding[key] for key in ("number", "base", "head"))):
        return False
    issuer = reservation.get("admitted_by") or {}
    run, attempt = issuer.get("run"), issuer.get("attempt")
    if not isinstance(run, str) or not RUN_ID.fullmatch(run) or not isinstance(attempt, str) or not RUN_ID.fullmatch(attempt):
        return False
    origin = api.call(f"actions/runs/{run}")
    if (str(origin.get("id")) != run or str(origin.get("run_attempt")) != attempt
            or (origin.get("repository") or {}).get("id") != REPO_ID or origin.get("path") != WORKFLOW):
        return False
    if (origin.get("status"), origin.get("conclusion")) not in {("in_progress", None), ("completed", "success")}:
        return False
    if origin.get("event") == "pull_request_target":
        return origin.get("head_sha") == binding["head"]
    return (origin.get("event") in {"schedule", "workflow_dispatch", "repository_dispatch"}
            and origin.get("head_branch") == "main" and origin.get("head_sha") == binding["base"])


def decision_matches(decision, binding):
    author = decision.get("automatic_author")
    base_keys = {"version", "repo", "number", "base", "head", "tree", "kind", "plan", "automatic_author"}
    keys = set(decision)
    if keys == base_keys:
        kind_ok = decision["kind"] == "compile"
    elif keys == base_keys | {"automatic_kind"}:
        kind_ok = decision["automatic_kind"] == "wide" and decision["kind"] in {"compile", "wide"}
    else:
        return False
    return (kind_ok and decision["version"] == 1 and decision["repo"] == REPO_ID
            and type(author) is int and author > 0 and type(decision["number"]) is int
            and all(decision[key] == binding[key] for key in ("number", "base", "head"))
            and isinstance(decision["tree"], str) and HEX40.fullmatch(decision["tree"]) is not None
            and isinstance(decision["plan"], str) and HEX64.fullmatch(decision["plan"]) is not None)


def source_receipt_matches(receipt, binding, decision):
    expected = {"base": binding["base"], "head": binding["head"], "tools_commit": binding["base"],
                "tree": decision["tree"], "state": "pass", "overall": "success",
                "states": dict.fromkeys((f"G{index}" for index in range(1, 9)), "success")}
    projections = {"verified-merge-ref", "composed-missing-merge-ref", "composed-stale-merge-ref"}
    fields = expected.keys() | {"projection", "policy_sha256"}
    return (isinstance(receipt, dict) and receipt.keys() == fields
            and all(receipt[key] == value for key, value in expected.items())
            and receipt["projection"] in projections
            and isinstance(receipt["policy_sha256"], str)
            and HEX64.fullmatch(receipt["policy_sha256"]) is not None)


def read_source_receipt(text):
    if len(text) > 4096:
        raise ValueError("The source rules receipt exceeds its limit.")
    return json.loads(text)


def claim_compiler(api, book, ledger, binding, decision, env):
    """Run on a hosted worker with only source-free data and the ledger token."""
    if not (enabled(env) and bot_dispatch(env) and decision_matches(decision, binding)):
        return False
    author = decision["automatic_author"]
    pr = api.current(binding)
    pr_kind = reservation_kind(pr)
    if decision.get("automatic_kind") == "wide":
        if not wide_enabled(env):
            return False
        if pr_kind == "narrow":
            if not budget_configured(env, "wide"):
                return False
            kind = "wide"
        elif pr_kind == TRUSTED_KIND:
            kind = TRUSTED_WIDE_KIND
        else:
            return False
    else:
        if pr_kind is None or (pr_kind == "narrow" and not budget_configured(env, "narrow")):
            return False
        kind = pr_kind
    if pr.get("draft") is not False or (pr.get("user") or {}).get("id") != author:
        return False
    args = (binding["number"], binding["head"], binding["base"])
    reservation = ledger.find(book, *args)
    tree = decision["tree"]
    if not verified_issuer(api, reservation, binding, author, tree, kind):
        return False
    claim = ledger.claim(book, *args, env["GITHUB_RUN_ID"], env["GITHUB_RUN_ATTEMPT"],
                         tree=tree, kind=kind, admitted_by=reservation["admitted_by"])
    if claim not in {"claimed", "already-claimed-by-this-run"}:
        return False
    latest = api.current(binding)
    return (reservation_kind(latest) == pr_kind and latest.get("draft") is False
            and (latest.get("user") or {}).get("id") == author)


def main(path):
    from github_status import GitHub, Superseded, event

    root = Path(__file__).resolve().parent.parent
    sys.path.insert(0, str(root / "admission"))
    sys.path.insert(0, str(root / "landing"))
    import ledger
    from api import Api, ApiError

    env = os.environ
    try:
        api = GitHub()
        book = Api(REPO, api.token, live=True)
        binding = event(api)
        decision = read(path, 4096)
        receipt = read_source_receipt(env.get("BFME_AUTOMATIC_SOURCE", ""))
        allowed = (decision_matches(decision, binding)
                   and source_receipt_matches(receipt, binding, decision)
                   and claim_compiler(api, book, ledger, binding, decision, env))
    except (Superseded, ApiError, ledger.LedgerError, OSError, ValueError, TypeError, KeyError, AttributeError):
        allowed = False
    if allowed:
        with Path(env["GITHUB_OUTPUT"]).open("a") as stream:
            stream.write("allowed=yes\n")
        print("Reserved the one compiler start for this exact protected run.")
    else:
        print("No valid automatic admission reservation. The compiler has not been requested.")


if __name__ == "__main__":
    main(sys.argv[1])

