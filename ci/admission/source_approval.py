"""Require an exact protected source-rules receipt before dispatching a compile."""

import io
import json
import re
import subprocess
import tempfile
from pathlib import Path
import zipfile

from common import REPO, REPO_ID

BOT_ID = 41898282
# tools/gates/workflow.py publishes one aggregate status that names any failing G1-G8 rule. Older runs also wrote
# one status per rule; their aggregate status carried the same decision, so only the aggregate is read.
CONTEXTS = ("BFME II / source rules",)
RUN_URL = re.compile(r"https://github\.com/" + re.escape(REPO) + r"/actions/runs/([1-9][0-9]{0,19})")
RUN_ID = re.compile(r"[1-9][0-9]{0,19}")
WORKFLOWS = {".github/workflows/rules.yml", ".github/workflows/managed-pr.yml"}
TOOLS = Path(__file__).resolve().parents[2] / "tools/gates"


class Unavailable(ValueError):
    pass


class Refused(Unavailable):
    pass


MAX_CANDIDATES = 4


def candidate_runs(api, head, base):
    """Protected runs that published a passing source-rules status for this head and main, newest first.

    Only the Actions bot's statuses for this main count. The newest such status decides a refusal: a failure there
    refuses. Otherwise every run that published a passing status is a candidate, so a later run that re-published it
    and then died (a compiler run whose machine failed) does not hide an earlier run's receipt. The caller
    authenticates each candidate run and its receipt."""
    newest, passing = {}, {}
    suffix = f"(main {base[:12]})"
    for page in range(1, 5):
        entries = api.call(f"commits/{head}/statuses?per_page=100&page={page}")
        for item in entries:
            match = RUN_URL.fullmatch(item.get("target_url") or "")
            if (item.get("context") not in CONTEXTS or (item.get("creator") or {}).get("id") != BOT_ID
                    or not (item.get("description") or "").endswith(suffix) or match is None):
                continue
            if item["context"] not in newest or item["id"] > newest[item["context"]]["id"]:
                newest[item["context"]] = item
            if item.get("state") == "success":
                seen = passing.setdefault(match[1], {})
                seen[item["context"]] = max(seen.get(item["context"], 0), item["id"])
        if len(entries) < 100:
            break
    for context, item in newest.items():
        if item.get("state") == "failure":
            raise Refused(f"Current source verdict {context} refused")
    complete = [run for run, seen in passing.items() if set(seen) == set(CONTEXTS)]
    if not complete:
        raise Unavailable("The current protected source rules have not all passed")
    return sorted(complete, key=lambda run: max(passing[run].values()), reverse=True)[:MAX_CANDIDATES]


def statuses(api, head, base):
    """The newest protected run that published a passing source-rules status for this head and main."""
    return candidate_runs(api, head, base)[0]


def download(artifact, token):
    """Read a single small JSON artifact from the fixed GitHub repository."""
    if not RUN_ID.fullmatch(str(artifact)) or not isinstance(token, str):
        raise Unavailable("The source receipt download is unavailable")
    limit = 16_384
    with tempfile.TemporaryFile() as response:
        done = subprocess.run(
            ["/usr/bin/prlimit", f"--fsize={limit}:{limit}", "--",
             "gh", "api", f"repos/{REPO}/actions/artifacts/{artifact}/zip"],
            stdout=response, stderr=subprocess.DEVNULL, stdin=subprocess.DEVNULL,
            env={"PATH": "/usr/bin:/bin", "GH_TOKEN": token, "LANG": "C"}, timeout=25, check=False
        )
        if done.returncode:
            raise Unavailable("GitHub did not return the bounded source receipt")
        response.seek(0)
        raw = response.read(limit + 1)
    if len(raw) > limit:
        raise Unavailable("The source receipt exceeds its size limit")
    with zipfile.ZipFile(io.BytesIO(raw)) as archive:
        entries = archive.infolist()
        if len(entries) != 1 or entries[0].filename != "receipt.json" or entries[0].file_size > 4096:
            raise Unavailable("The source receipt archive has unexpected contents")
        with archive.open(entries[0]) as stream:
            record = stream.read(4097)
        if len(record) > 4096:
            raise Unavailable("The source receipt exceeds its JSON size limit")
    return json.loads(record)


def require_source(api, number, head, base, tree, policy_digest, load_receipt):
    """Bind the all-rule outcome to exact PR, tree, current policy, run and attempt."""
    value = receipt_for(api, number, head, base, policy_digest, load_receipt)
    if value["tree"] != tree:
        raise Unavailable("The protected source receipt has a different tree, policy or attempt")
    return value


def receipt_for(api, number, head, base, policy_digest, load_receipt):
    """The exact all-rule receipt for this PR, head, main and current policy; its tree is the protected merge.

    Tries the candidate runs newest first and returns the first that authenticates completely."""
    failure = Unavailable("The current protected source rules have not all passed")
    for run_id in candidate_runs(api, head, base):
        try:
            return receipt_of_run(api, run_id, number, head, base, policy_digest, load_receipt)
        except Unavailable as exc:
            failure = exc
    raise failure


def receipt_of_run(api, run_id, number, head, base, policy_digest, load_receipt):
    run = api.call(f"actions/runs/{run_id}")
    event_head = {"pull_request_target": head, "repository_dispatch": base}.get(run.get("event"))
    attempt = str(run.get("run_attempt", ""))
    if (run.get("path") not in WORKFLOWS or event_head is None or run.get("head_sha") != event_head
            or str(run.get("id")) != run_id or (run.get("repository") or {}).get("id") != REPO_ID
            or run.get("status") != "completed" or run.get("conclusion") != "success"
            or not RUN_ID.fullmatch(attempt)):
        raise Unavailable("The source status does not name a successful protected workflow attempt")
    listed = api.call(f"actions/runs/{run_id}/artifacts?per_page=100")
    name = f"bfme-source-receipt-{attempt}"
    artifacts = [row for row in listed["artifacts"] if row.get("name") == name]
    if len(artifacts) != 1 or artifacts[0].get("expired") is not False:
        raise Unavailable("The source attempt's exact receipt is missing or expired")
    row = artifacts[0]
    if type(row.get("size_in_bytes")) is not int or not 0 < row["size_in_bytes"] <= 16_384:
        raise Unavailable("The source receipt exceeds its size limit")
    if not RUN_ID.fullmatch(str(row.get("id", ""))):
        raise Unavailable("The source receipt does not have a valid artifact id")
    value = load_receipt(row["id"])
    tree = value.get("tree") if isinstance(value, dict) else None
    expected = {"repo": REPO_ID, "number": number, "head": head, "base": base, "tree": tree,
                "tools_commit": base, "policy_sha256": policy_digest,
                "run": run_id, "attempt": attempt, "state": "pass"}
    if value != expected or not (isinstance(tree, str) and re.fullmatch(r"[0-9a-f]{40}", tree)):
        raise Unavailable("The protected source receipt has a different tree, policy or attempt")
    return value
