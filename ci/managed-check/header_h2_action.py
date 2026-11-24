"""Run a bounded admitted H2 probe or authenticate that same attempt's receipt."""

import argparse
import os
from pathlib import Path
import sys
from tempfile import TemporaryDirectory
from urllib.error import URLError
from zipfile import BadZipFile

from common import APPROVED_IMAGE_PROFILES, HEX40, HEX64, PIN, REPO_ID, policy, read, write
from fork_admission import read_source_receipt, source_receipt_matches
from gitrepo import Git
from measure import COMPILER
from recovery_artifacts import Actions, ArtifactPending, epoch
from wide_recovery import active_run
from compact_matrix import run_id

MAX_H2_BYTES = 128 * 1024


def source_policy():
    source_dir = Path(__file__).resolve().parents[2] / "tools/gates"
    sys.path.insert(0, str(source_dir))
    from gate import policy_digest
    return policy_digest(source_dir / "config.json")


def probe(admitted, output):
    """The Boat command has no GitHub token and only one protected plan."""
    from wide import admission
    import header_producer

    plan, _ = admission(admitted, kinds=("wide", "compile"))
    receipt = read_source_receipt(os.environ.pop("BFME_H2_SOURCE", ""))
    if (not any(name.startswith("include/") for name in plan["changed_paths"])
            or not source_receipt_matches(receipt, plan, plan)
            or receipt["policy_sha256"] != source_policy()):
        raise ValueError("The protected source result does not approve this header tree.")
    sys.argv = [sys.argv[0], str(admitted), str(output), "--probe-budget", "8"]
    header_producer.main()


def receipt_binding(decision, run):
    return ({"version": 1, "repo": REPO_ID, "component": "H2", "policy": policy(),
             "checker": PIN["checker"], "image": PIN["image_sha256"],
             "profile": PIN["profile_sha256"], "compiler": COMPILER,
             "issuer": {"run": run["id"], "attempt": run["attempt"]}}
            | {name: decision[name] for name in ("number", "base", "head", "tree", "plan")})


def verified_receipt(record, decision, run, approved_images=None):
    expected = receipt_binding(decision, run)
    flexible = {"image", "profile"} if approved_images is not None else frozenset()
    if (set(record) != set(expected) | {"base_tree", "probe_count", "status", "findings", "layout_only"}
            or any(record.get(key) != value for key, value in expected.items() if key not in flexible)
            or not isinstance(record["base_tree"], str) or not HEX40.fullmatch(record["base_tree"])
            or record["status"] not in {"pass", "held", "refuse"}
            or type(record["probe_count"]) is not int or record["probe_count"] < 0
            or (record["probe_count"] > 8 and record["status"] == "pass")
            or not isinstance(record["findings"], list) or not isinstance(record["layout_only"], list)):
        raise ValueError("The H2 receipt does not bind the current protected attempt.")
    if approved_images is not None:
        if (record.get("image"), record.get("profile")) not in approved_images:
            raise ValueError("The H2 receipt does not bind the current protected attempt.")
    return record


def current(api, decision):
    pr = api.get(f"pulls/{decision['number']}")
    return (pr["number"] == decision["number"] and pr["state"] == "open" and pr.get("draft") is not True
            and pr["head"]["sha"] == decision["head"] and pr["base"]["ref"] == "main"
            and pr["base"]["repo"]["id"] == REPO_ID
            and api.get("git/ref/heads/main")["object"]["sha"] == decision["base"])


def own_decision(decision):
    if (decision["kind"] not in {"wide", "compile"} or decision["repo"] != REPO_ID
            or type(decision["number"]) is not int or not 0 < decision["number"] < 10**9
            or decision["base"] != os.environ["GITHUB_SHA"]
            or any(not HEX40.fullmatch(decision[key]) for key in ("head", "base", "tree"))
            or not HEX64.fullmatch(decision["plan"])):
        raise ValueError("Missing protected H2 admission identity.")
    return decision


def same_artifact(row, api, run, name, ident, checksum, started, limit):
    issuer = row["workflow_run"]
    if (row["id"] != ident or type(ident) is not int or ident <= 0
            or not HEX64.fullmatch(checksum) or row["digest"] != "sha256:" + checksum
            or row["name"] != name or row["expired"] is not False
            or type(row["size_in_bytes"]) is not int or not 0 < row["size_in_bytes"] <= limit
            or not isinstance(issuer, dict) or str(issuer["id"]) != run["id"]
            or issuer["repository_id"] != REPO_ID or issuer["head_sha"] != api.run_head
            or epoch(row["created_at"]) < started):
        raise ValueError("Unexpected H2 input artifact provenance.")
    return ident, checksum


def read_result(api, decision, run, artifact, approved_images=None):
    with TemporaryDirectory(prefix="bfme-header-evidence-") as scratch:
        api.extract(artifact, {"header-h2.json"}, MAX_H2_BYTES, scratch)
        result = verified_receipt(read(Path(scratch) / "header-h2.json", MAX_H2_BYTES), decision, run,
                                  approved_images=approved_images)
    base_tree = Git(Path.cwd() / ".git").value("rev-parse", decision["base"] + "^{tree}")
    if result["base_tree"] != base_tree or not current(api, decision):
        raise ValueError("The H2 baseline or current binding changed during readback.")
    return result


def collect(admission, output, ident, digest):
    """Verify the exact artifact advertised by the already-paid Boat collector."""
    digest = digest.removeprefix("sha256:")
    decision = own_decision(read(admission, 4096))
    run = active_run()
    if not ident.isascii() or not ident.isdecimal() or not 0 < len(ident) <= 20 or int(ident) <= 0:
        raise ValueError("Missing protected H2 artifact identity.")
    api = Actions(decision["head"], base=decision["base"])
    started = api.anchor()
    if not current(api, decision):
        raise ValueError("The pull request or protected main moved before H2 readback.")
    row = api.get(f"actions/artifacts/{int(ident)}")
    artifact = same_artifact(row, api, run, "bfme-header-h2-" + run["attempt"],
                             int(ident), digest, started, MAX_H2_BYTES)
    result = read_result(api, decision, run, artifact)
    output.mkdir(mode=0o700)
    write(output / "header-h2.json", result)
    return result


def published_issuer(api, binding):
    # The status is a locator. Authenticate the selected workflow, attempt,
    # admission artifact, H2 artifact and paper trail against protected Git.
    from landing_read import BOT_ID, CONTEXT, URL

    statuses = api.get(f"commits/{binding['head']}/statuses?per_page=100")
    chosen = max((item for item in statuses if item.get("context") == CONTEXT),
                 key=lambda item: item["id"], default={})
    target = URL.fullmatch(chosen.get("target_url") or "")
    if (not target or (chosen.get("creator") or {}).get("id") != BOT_ID
            or chosen.get("state") != "success"
            or not (chosen.get("description") or "").endswith(f"(main {binding['base'][:12]})")):
        raise ValueError("No current protected Boat status identifies H2 evidence.")
    row = api.get(f"actions/runs/{target[1]}")
    expected = {"pull_request_target": binding["head"], "repository_dispatch": binding["base"]}.get(row.get("event"))
    if (expected is None or row.get("head_sha") != expected
            or row.get("path") != ".github/workflows/managed-pr.yml"
            or (row.get("repository") or {}).get("id") != REPO_ID
            or str(row.get("id")) != target[1]
            or row.get("status") not in {"in_progress", "completed"}):
        raise ValueError("The H2 status did not identify the protected comparison workflow.")
    # The verified artifact and terminal Boat verdict may already exist while
    # this run's hosted notification is finishing.
    return run_id(target[1], str(row.get("run_attempt", "")))


def named_artifact(api, run, name, started, limit):
    rows = api.get(f"actions/runs/{run['id']}/artifacts?name={name}&per_page=5")["artifacts"]
    exact = [row for row in rows if row["name"] == name]
    if len(exact) != 1:
        raise ValueError("The protected H2 attempt has no unique artifact.")
    row = exact[0]
    return same_artifact(row, api, run, name, row["id"], row["digest"].removeprefix("sha256:"),
                         started, limit)


def read_published(binding):
    """Read already-published H2 from the exact protected base, head and composed tree."""
    if (type(binding["number"]) is not int or not 0 < binding["number"] < 10**9
            or binding["base"] != os.environ["GITHUB_SHA"]
            or any(not HEX40.fullmatch(binding[key]) for key in ("head", "base", "tree"))):
        raise ValueError("Use an exact current protected pull request binding.")
    api = Actions(binding["head"], base=binding["base"])
    if not current(api, binding):
        raise ValueError("The pull request or protected main moved before H2 readback.")
    run = published_issuer(api, binding)
    api.run = run
    started = api.anchor()
    artifact = named_artifact(api, run, "bfme-admission-status-" + run["attempt"], started, 4096)
    with TemporaryDirectory(prefix="bfme-header-admission-") as scratch:
        api.extract(artifact, {"decision.json"}, 4096, scratch)
        decision = own_decision(read(Path(scratch) / "decision.json", 4096))
    if any(decision[key] != binding[key] for key in ("number", "base", "head", "tree")):
        raise ValueError("The H2 producer admitted a different pull request.")
    artifact = named_artifact(api, run, "bfme-header-h2-" + run["attempt"], started, MAX_H2_BYTES)
    result = read_result(api, decision, run, artifact, approved_images=APPROVED_IMAGE_PROFILES)
    if published_issuer(api, binding) != run:
        raise ValueError("A newer protected attempt superseded the H2 evidence.")
    return result


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    tasks = parser.add_subparsers(required=True, dest="operation")
    command = tasks.add_parser("probe")
    command.add_argument("admission", type=Path)
    command.add_argument("output", type=Path)
    command = tasks.add_parser("collect")
    command.add_argument("admission", type=Path)
    command.add_argument("output", type=Path)
    command.add_argument("artifact")
    command.add_argument("digest")
    args = parser.parse_args()
    try:
        if args.operation == "probe":
            probe(args.admission, args.output)
        else:
            result = collect(args.admission, args.output, args.artifact, args.digest)
            print("Authenticated H2 declaration receipt: " + result["status"])
            with Path(os.environ["GITHUB_OUTPUT"]).open("a") as out:
                out.write("verified=yes\n")
    except (ArtifactPending, BadZipFile, KeyError, OSError, RuntimeError, TypeError, URLError, ValueError):
        raise SystemExit("Protected H2 evidence is unavailable for this exact attempt.") from None


if __name__ == "__main__":
    main()
