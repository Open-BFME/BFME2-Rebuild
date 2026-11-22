"""Produce H2 facts from a protected wide plan and an audited compiler job."""

import argparse
from collections import defaultdict
import json
import os
from pathlib import Path
import re

import header_class_source
from common import PIN, write
from header_decl_check import evaluate_declaration, index_ledger
from header_probe_plan import compile_source, plan_existing_header
from header_worker_proof import collect, plan
from measure import COMPILER, request
from pool_compare import projection
from pool_worker import MAX_PLAN
from wide import admission, comparison_inputs

ISSUER = re.compile(r"[1-9][0-9]{0,19}\Z")


def inventory(git, base, candidate, parser):
    """Inventory the exact old and composed header blobs. Unsupported shapes hold."""
    findings, probes, decisions, fields = [], {"base": {}, "head": {}}, [], []
    for item in git.changes(base, candidate):
        path = item.path
        if not path.startswith("include/"):
            continue
        before = git.run("show", f"{base}:{path}", check=False)
        after = git.run("show", f"{candidate}:{path}", check=False)
        if before.returncode or after.returncode:
            findings.append({"header": path, "status": "held",
                             "reason": "A new or deleted header needs inventory from the header producer."})
            continue
        changes = plan_existing_header(path, before.stdout.decode("utf-8"),
                                       after.stdout.decode("utf-8"), parser)
        if changes["status"] != "planned":
            issues = changes.get("issues", [])
            issue_reason = (issues[0].get("reason")
                            if issues and isinstance(issues[0], dict) and issues[0].get("reason")
                            else None)
            findings.append({"header": path, "status": "held",
                             "reason": changes.get("reason") or issue_reason or "An unsupported declaration requires evidence.",
                             "issues": issues})
            continue
        for decl in changes["after"]:
            if decl["kind"] == "instance_data":
                fields.append({"header": path, "declaration": decl["qualified"], "line": decl["line"]})
                continue
            old = decl.get("before")
            locations = {}
            for side, target in (("base", old), ("head", decl)):
                if target is None:
                    continue
                keys = tuple((path, compile_source(path, value).encode("ascii")) for value in (None, target))
                for key in keys:
                    probes[side][key] = (key[0], key[1], ())
                locations[side] = keys
            decisions.append((path, decl, locations))
    return findings, probes, decisions, fields


def compile_probes(git, revision, expected_tree, inputs, roots, run):
    if not inputs:
        return {}
    sources, hashes = projection(git, revision)
    results = {}
    keys = list(inputs)
    for start in range(0, len(keys), MAX_PLAN):
        members = keys[start:start + MAX_PLAN]
        payload, rows, contracts, prepared = plan(
            [inputs[k] for k in members], expected_tree, sources, hashes,
            profile=PIN["profile_sha256"], compiler=COMPILER)
        evidence = collect(run("compile", payload), payload, rows, contracts, prepared, roots)
        for key, row in zip(members, rows, strict=True):
            results[key] = evidence[row["tu"]]
    return results


def fact(observation):
    if observation["status"] != "observed":
        return None
    return {name: observation["receipt"][name] for name in (
        "object_sha256", "compiler_request_sha256", "compiler_contract")}


def produce(git, binding, candidate, parser, roots, run, *, budget):
    """Only the protected CLI chooses Git revisions, profile, inputs and launcher."""
    base = binding["base"]
    base_tree = git.value("rev-parse", base + "^{tree}")
    if git.value("rev-parse", candidate + "^{tree}") != binding["tree"]:
        raise ValueError("The H2 candidate does not have the admitted tree.")
    findings, probes, declarations, fields = inventory(git, base, candidate, parser)
    probe_count = sum(len(side) for side in probes.values())
    if type(budget) is not int or budget <= 0:
        raise ValueError("H2 probe budget must be a positive protected integer.")
    if probe_count > budget:
        findings.append({"status": "held", "reason": "The protected H2 probe budget needs a wider assignment."})
    elif declarations:
        metadata = [json.loads(git.run("show", f"{base}:config/{name}.json").stdout)
                    for name in ("map", "symbols")]
        ledger = index_ledger(*metadata)
        observed = {
            "base": compile_probes(git, base, base_tree, probes["base"], roots, run),
            "head": compile_probes(git, candidate, binding["tree"], probes["head"], roots, run),
        }
        for path, decl, locations in declarations:
            by_side = {side: [observed[side][key] for key in keys]
                       for side, keys in locations.items()}
            row = {"header": path, "line": decl["line"], "declaration": decl["qualified"]}
            if any(item["status"] != "observed" for items in by_side.values() for item in items):
                findings.append(row | {"status": "held", "reason": "An isolated H2 declaration probe did not compile."})
                continue

            def pair(side):
                return [dict(item, pin=item["pins"]) for item in by_side[side]]

            outcome = evaluate_declaration(path, decl["qualified"], ledger, pair("head"),
                                           base=pair("base") if "base" in by_side else None)
            findings.append(row | outcome | {"receipts": {
                side: [fact(item) for item in items] for side, items in by_side.items()}})
    statuses = {row["status"] for row in findings}
    status = ("refuse" if "refuse" in statuses else
              "held" if statuses & {"held", "ledger_request"} else "pass")
    return {
        "version": 1, "component": "H2", "status": status,
        "repo": binding["repo"], "number": binding["number"], "base": base,
        "head": binding["head"], "tree": binding["tree"], "base_tree": base_tree,
        "policy": binding["policy"], "checker": binding["checker"],
        "image": PIN["image_sha256"], "profile": PIN["profile_sha256"], "compiler": COMPILER,
        "probe_count": probe_count, "findings": findings, "layout_only": fields,
    }


def main():
    cli = argparse.ArgumentParser(description=__doc__)
    cli.add_argument("wide_admission", type=Path)
    cli.add_argument("output", type=Path)
    cli.add_argument("--probe-budget", required=True, type=int)
    args = cli.parse_args()
    run, attempt = [os.environ.get(x, "") for x in ("GITHUB_RUN_ID", "GITHUB_RUN_ATTEMPT")]
    if not ISSUER.fullmatch(run) or not ISSUER.fullmatch(attempt):
        raise ValueError("Invalid protected workflow issuer.")
    plan_doc, plan_hash = admission(args.wide_admission, kinds=("wide", "compile"))
    with comparison_inputs(plan_doc) as (_, git, tip, _judge, roots):
        result = produce(git, plan_doc, tip, header_class_source, roots, request, budget=args.probe_budget)
    args.output.mkdir(mode=0o700)
    write(args.output / "header-h2.json", result | {"plan": plan_hash, "issuer": {"run": run, "attempt": attempt}})


if __name__ == "__main__":
    main()
