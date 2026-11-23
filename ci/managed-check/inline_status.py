"""Post the third exact-head status from the compact advisory receipt."""
import os
from pathlib import Path
import sys

from common import HEX40, HEX64, REPO_ID, policy, read
from github_status import GitHub, Superseded, event

_STATUS_EVENT = event
CONTEXT = "BFME II / Inline virtual"
VERDICTS = {
    "pass": ("success", "Inline virtual behaviour passed"),
    "fail": ("failure", "Inline virtual behaviour changed; see the advisory report"),
    "pending": ("pending", "Inline virtual behaviour needs more evidence; see the report"),
}


def verified_state(record, binding, admission):
    expected = {"version": 1, "repo": REPO_ID, "number": binding["number"],
                "run": os.environ["GITHUB_RUN_ID"], "attempt": os.environ["GITHUB_RUN_ATTEMPT"]}
    keys = set(expected) | {"binding", "policy_sha256", "verdict", "state", "evidence_sha256", "plan"}
    if set(record) != keys or any(record.get(key) != value for key, value in expected.items()):
        raise ValueError("Inline virtual evidence came from a different run or pull request.")
    measured = record["binding"]
    if (set(measured) != {"base", "head", "tree"}
            or any(measured[key] != binding[key] for key in ("base", "head"))
            or not HEX40.fullmatch(measured["tree"])
            or not HEX64.fullmatch(record["plan"])
            or admission.get("kind") not in {"compile", "wide"}
            or any(admission.get(key) != value for key, value in (measured | {
                "repo": REPO_ID, "version": 1, "number": binding["number"], "plan": record["plan"]}).items())
            or record["policy_sha256"] != policy()
            or record["verdict"] != record["state"]
            or record["state"] not in VERDICTS
            or not HEX64.fullmatch(record["evidence_sha256"])):
        raise ValueError("Inline virtual evidence has the wrong base, head, tree or check policy.")
    return VERDICTS[record["state"]]


def publish(path, admission):
    api = GitHub()
    decision = read(admission, 4096) if admission.is_file() else None
    allow_disjoint = (path.is_file() and isinstance(decision, dict)
                      and decision.get("kind") == "compile"
                      and decision.get("automatic_kind") != "wide")
    binding = event(api, allow_moved_main=allow_disjoint) if event is _STATUS_EVENT else event(api)
    check = api.current_or_disjoint if (allow_disjoint and hasattr(api, "current_or_disjoint")) else api.current
    check(binding)
    if path.is_file() and decision is not None:
        state, text = verified_state(read(path, 4096), binding, decision)
    else:
        state, text = "pending", "Inline virtual evidence is unavailable; see the Actions run"
    check(binding)
    api.status(binding, state, f"{text} (main {binding['base'][:12]})", context=CONTEXT)


if __name__ == "__main__":
    try:
        publish(Path(sys.argv[1]), Path(sys.argv[2]))
    except Superseded:
        print("The pull request or main has moved.")
        raise SystemExit(2) from None
