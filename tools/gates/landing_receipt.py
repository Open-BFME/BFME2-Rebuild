"""Retain only the validated, bounded source-rule fields used by landing."""
import json
import os
from pathlib import Path
import re
import sys

# The runner uses isolated Python. Add only this protected checkout directory.
sys.path.insert(0, str(Path(__file__).resolve().parent))
from workflow import CheckUnavailable, QUEUE_REF, REASONS, event

OID = re.compile(r"[0-9a-f]{40}")
HASH = re.compile(r"[0-9a-f]{64}")
ID = re.compile(r"[1-9][0-9]{0,19}")


def keep(document, base, repository_id, number, run, attempt, *, queue_ref=None):
    if queue_ref is not None:
        if number is not None:
            raise ValueError("Merge-group receipt must not carry a PR number.")
        if not all(ID.fullmatch(value) for value in (repository_id, run, attempt)):
            raise ValueError("Use the exact repository, run and attempt.")
        if not isinstance(queue_ref, str) or not QUEUE_REF.fullmatch(queue_ref):
            raise ValueError("Use the exact merge-queue ref.")
    elif not all(ID.fullmatch(value) for value in (repository_id, number, run, attempt)):
        raise ValueError("Use the exact repository, PR, run and attempt.")
    if (document.get("base") != base or document.get("tools_commit") != base
            or document.get("state") != "pass" or document.get("overall") != "success"
            or not HASH.fullmatch(document.get("policy_sha256", ""))
            or not all(OID.fullmatch(document.get(key, "")) for key in ("base", "head", "tree"))):
        raise ValueError("Only the exact successful source-rules decision is retained.")
    keys = ("base", "head", "tree", "tools_commit", "policy_sha256", "state")
    common = {key: document[key] for key in keys} | {
        "repo": int(repository_id), "run": run, "attempt": attempt}
    if queue_ref is not None:
        return common | {"kind": "merge_group", "queue_ref": queue_ref}
    return common | {"number": int(number)}


def main(path):
    os.umask(0o077)
    env = os.environ
    event_name = env.get("GITHUB_EVENT_NAME")
    if event_name not in {"pull_request_target", "repository_dispatch", "merge_group"}:
        raise ValueError("found a historical or unsupported event")
    binding = event(env)
    raw = env["SOURCE_RULE_RECEIPT"]
    if not 0 < len(raw) <= 4096:
        raise ValueError("The trusted source rules receipt exceeded its size allowance.")
    decision = json.loads(raw)
    if decision.get("head") != binding.head or decision.get("fixture_case"):
        raise ValueError("found a result for another author head or for a historical fixture")
    if event_name == "merge_group":
        record = keep(decision, binding.base, str(binding.repo_id),
                      None, env["GITHUB_RUN_ID"], env["GITHUB_RUN_ATTEMPT"],
                      queue_ref=binding.queue_ref)
    else:
        record = keep(decision, binding.base, str(binding.repo_id),
                      str(binding.number), env["GITHUB_RUN_ID"], env["GITHUB_RUN_ATTEMPT"])
    path.mkdir(mode=0o700)
    (path / "receipt.json").write_text(json.dumps(record, sort_keys=True, separators=(",", ":"))+"\n")


if __name__ == "__main__":
    try:
        main(Path(sys.argv[1]))
    except CheckUnavailable as exc:
        print(f"Expected a current source result for landing. Found: {REASONS[exc.reason]}", file=sys.stderr)
        sys.exit(1)
    except (AttributeError, IndexError, KeyError, OSError, TypeError, ValueError):
        print("Expected an exact successful source result for landing. Found a missing or inconsistent result.", file=sys.stderr)
        sys.exit(1)
