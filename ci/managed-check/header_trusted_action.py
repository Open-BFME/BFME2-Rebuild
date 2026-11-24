"""Produce and authenticate bounded H4/H6 trusted header observations."""

import argparse
import os
from pathlib import Path
import re
import sys
from tempfile import TemporaryDirectory
from urllib.error import URLError
from zipfile import BadZipFile

from common import APPROVED_IMAGE_PROFILES, HEX40, HEX64, PIN, REPO, REPO_ID, policy, read, write
from fork_admission import read_source_receipt, source_receipt_matches
from gitrepo import Git
import header_h2_action as h2
from measure import COMPILER
from intake import reconstruct
from recovery_artifacts import Actions, ArtifactPending
from wide import admission

MAX_HT_BYTES = 512 * 1024
ISSUER = re.compile(r"[1-9][0-9]{0,19}\Z")


def _tools_dir():
    return Path(__file__).resolve().parents[2] / "tools" / "gates"


def receipt_binding(decision, run):
    return ({"version": 1, "repo": REPO_ID, "component": "HT", "policy": policy(),
             "source_policy": h2.source_policy(),
             "checker": PIN["checker"], "image": PIN["image_sha256"],
             "profile": PIN["profile_sha256"], "compiler": COMPILER,
             "issuer": {"run": run["id"], "attempt": run["attempt"]}}
            | {name: decision[name] for name in ("number", "base", "head", "tree", "plan")})


def verified_receipt(record, decision, run, approved_images=None):
    expected = receipt_binding(decision, run)
    flexible = {"image", "profile"} if approved_images is not None else frozenset()
    if (not isinstance(record, dict)
            or set(record) != set(expected) | {"base_tree", "observations"}
            or any(record.get(key) != value for key, value in expected.items() if key not in flexible)
            or not isinstance(record.get("base_tree"), str) or not HEX40.fullmatch(record["base_tree"])
            or not isinstance(record.get("observations"), dict)):
        raise ValueError("The trusted header receipt does not bind the current protected attempt.")
    if approved_images is not None:
        if (record.get("image"), record.get("profile")) not in approved_images:
            raise ValueError("The trusted header receipt does not bind the current protected attempt.")
    return record


def produce(admitted, output):
    """Run on Boat with no token: reconstruct the bound candidate tree and produce H4/H6 inventories."""
    run_id_str = os.environ.get("GITHUB_RUN_ID", "")
    attempt_str = os.environ.get("GITHUB_RUN_ATTEMPT", "")
    if not ISSUER.fullmatch(run_id_str) or not ISSUER.fullmatch(attempt_str):
        raise ValueError("Invalid protected workflow issuer.")
    plan, plan_hash = admission(admitted, kinds=("wide", "compile"))
    receipt = read_source_receipt(os.environ.pop("BFME_H2_SOURCE", ""))
    src_policy = h2.source_policy()
    if (not any(name.startswith("include/") for name in plan["changed_paths"])
            or not source_receipt_matches(receipt, plan, plan)
            or receipt["policy_sha256"] != src_policy):
        raise ValueError("The protected source result does not approve this header tree.")

    checkout = Git(Path.cwd() / ".git")
    if checkout.rev("HEAD") != plan["base"]:
        raise ValueError("The trusted header producer has an unexpected base checkout.")
    base_tree = checkout.value("rev-parse", plan["base"] + "^{tree}")

    tools = _tools_dir()
    if str(tools) not in sys.path:
        sys.path.insert(0, str(tools))
    from gate import policy_at
    from git_objects import Git as GateGit
    from header_inventory import Binding
    import header_trusted

    with TemporaryDirectory(prefix="bfme-header-trusted-") as scratch:
        git = Git(Path(scratch) / "source.git", initialize=True)
        alternates = git.path / "objects/info/alternates"
        alternates.write_text(str((checkout.path / "objects").resolve(strict=True)) + "\n")
        git.run("update-ref", "refs/heads/main", plan["base"])
        tip = reconstruct(git, plan)
        if git.value("rev-parse", tip + "^{tree}") != plan["tree"]:
            raise ValueError("The reconstructed candidate tree differs from the admitted tree.")
        binding = Binding(
            REPO, plan["base"], plan["head"], plan["tree"],
            src_policy, PIN["checker"], int(run_id_str), int(attempt_str),
        )
        with GateGit(git.path) as repo:
            observations = header_trusted.produce(repo, policy_at(tools / "config.json"), binding)

    run = {"id": run_id_str, "attempt": attempt_str}
    decision = {key: plan[key] for key in ("number", "base", "head", "tree")} | {"plan": plan_hash}
    document = receipt_binding(decision, run) | {"base_tree": base_tree, "observations": observations}
    output.mkdir(mode=0o700, exist_ok=True)
    write(output / "header-trusted.json", document)
    return document


def read_published(binding):
    """Read already-published H4/H6 trusted receipt for the exact protected base, head and composed tree."""
    if (type(binding["number"]) is not int or not 0 < binding["number"] < 10**9
            or binding["base"] != os.environ["GITHUB_SHA"]
            or any(not HEX40.fullmatch(binding[key]) for key in ("head", "base", "tree"))):
        raise ValueError("Use an exact current protected pull request binding.")
    api = Actions(binding["head"], base=binding["base"])
    if not h2.current(api, binding):
        raise ValueError("The pull request or protected main moved before trusted header readback.")
    run = h2.published_issuer(api, binding)
    api.run = run
    started = api.anchor()
    decision_artifact = h2.named_artifact(api, run, "bfme-admission-status-" + run["attempt"], started, 4096)
    with TemporaryDirectory(prefix="bfme-header-trusted-adm-") as scratch:
        api.extract(decision_artifact, {"decision.json"}, 4096, scratch)
        decision = h2.own_decision(read(Path(scratch) / "decision.json", 4096))
    if any(decision[key] != binding[key] for key in ("number", "base", "head", "tree")):
        raise ValueError("The trusted header producer admitted a different pull request.")
    ht_artifact = h2.named_artifact(api, run, "bfme-header-trusted-" + run["attempt"], started, MAX_HT_BYTES)
    with TemporaryDirectory(prefix="bfme-header-trusted-read-") as scratch:
        api.extract(ht_artifact, {"header-trusted.json"}, MAX_HT_BYTES, scratch)
        result = verified_receipt(read(Path(scratch) / "header-trusted.json", MAX_HT_BYTES), decision, run,
                                  approved_images=APPROVED_IMAGE_PROFILES)
    base_tree = Git(Path.cwd() / ".git").value("rev-parse", decision["base"] + "^{tree}")
    if result["base_tree"] != base_tree or not h2.current(api, decision):
        raise ValueError("The trusted header baseline or current binding changed during readback.")
    if h2.published_issuer(api, binding) != run:
        raise ValueError("A newer protected attempt superseded the trusted header evidence.")
    return {"issuer": run, "observations": result["observations"]}


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    tasks = parser.add_subparsers(required=True, dest="operation")
    command = tasks.add_parser("produce")
    command.add_argument("admission", type=Path)
    command.add_argument("output", type=Path)
    args = parser.parse_args()
    try:
        produce(args.admission, args.output)
    except (ArtifactPending, BadZipFile, KeyError, OSError, RuntimeError, TypeError, URLError, ValueError):
        raise SystemExit("Protected trusted header observations could not be produced for this attempt.") from None


if __name__ == "__main__":
    main()
