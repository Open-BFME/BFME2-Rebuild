"""Plan and verify exact-main state workflow inputs without publication credentials."""
import os
from pathlib import Path
import re
import sys

from state_snapshot import MAX_PLAN_BYTES, MAX_SHARD_BYTES, assemble, authoritative_units, canonical, digest, load, make_plan, validate_plan
from state_compact import compact

sys.path.append(str(Path(__file__).resolve().parent.parent / "managed-check"))
from common import HEX40, PIN, REPO, REPO_ID, read
from gitrepo import Git
from state_evidence import LIMIT, cached_pairs, header
from state_delta import inherited, merge


class OverBudget(ValueError):
    """The plan needs more runners than the approved cap."""

    def __init__(self, count, budget):
        super().__init__(f"This full refresh requires {count} runners, above the {budget}-runner cap.")
        self.count, self.budget = count, budget


def make_checked_plan(root, main, budget, *, measured=None):
    if not re.fullmatch(r"[1-9][0-9]{0,2}", budget) or int(budget) > 128:
        raise ValueError("Set BFME_STATE_BUDGET_JOBS to the approved runner cap, between 1 and 128.")
    plan = make_plan(root, main, PIN["checker"], per_batch=64)
    if measured is not None:
        plan["batches"] = [measured[i:i+64] for i in range(0, len(measured), 64)]
        validate_plan(plan)
    count = len(plan["batches"])
    if count > int(budget):
        raise OverBudget(count, budget)
    return plan


def plan_budget(env):
    """An explicit seed may use its own, higher cap (BFME_STATE_SEED_BUDGET_JOBS); every other run uses the
    standing cap."""
    if env.get("BFME_STATE_SEED") == "true" and env.get("STATE_SEED_BUDGET"):
        return env["STATE_SEED_BUDGET"]
    return env.get("STATE_BUDGET", "")


def skip_over_budget(error, env, output):
    """A routine refresh that needs more runners than the cap cannot finish here: it measures nothing (active=no)
    and succeeds, instead of failing on every push until a maintainer seeds the state. An explicit seed must
    finish its baseline, so it still fails."""
    if env.get("BFME_STATE_SEED") == "true":
        return False
    print(f"::notice title=Protected main state not measured::{error} "
          "Run 'Measure protected main state' by hand with seed enabled to refresh the protected baseline.")
    with Path(output).open("a") as file:
        file.write("active=no\n")
    return True


def select_shards(plan, directory, plan_attempt, current_attempt):
    attempt_format = r"[1-9][0-9]{0,19}"
    if not all(re.fullmatch(attempt_format, value) for value in (plan_attempt, current_attempt)):
        raise ValueError("Expected exact state plan and runner attempts. Found invalid attempt metadata.")
    first, last = int(plan_attempt), int(current_attempt)
    if first > last:
        raise ValueError("Expected the current or earlier state plan. Found a future attempt.")
    pattern = re.compile(rf"bfme-state-{first}-shard-(0|[1-9][0-9]{{0,2}})-try-({attempt_format})")
    instances = {}
    for path in directory.iterdir():
        match = pattern.fullmatch(path.name)
        if not match or path.is_symlink() or not path.is_dir():
            raise ValueError("Expected immutable state shard directories. Found a foreign artifact.")
        batch, attempt = map(int, match.groups())
        if not (0 <= batch < len(plan["batches"]) and first <= attempt <= last):
            raise ValueError("Expected a planned shard and valid attempt. Found a foreign artifact.")
        if batch not in instances or instances[batch][0] < attempt:
            instances[batch] = (attempt, path)
    if set(instances) != set(range(len(plan["batches"]))):
        raise ValueError("Expected every exact state shard. Found missing artifacts. Keep the prior state.")
    reused = sum(attempt < last for attempt, _ in instances.values())
    print(f"Joining {len(instances)} exact-plan state shards. Reused {reused} earlier-attempt artifacts.")
    return [instances[index][1] for index in range(len(plan["batches"]))]


def select_fresh_shards(plan, directory, attempt):
    """The weekly independent full recompute only uses its own attempt."""
    if not re.fullmatch(r"[1-9][0-9]{0,19}", attempt):
        raise ValueError("Expected a valid Actions attempt. Found invalid metadata.")
    wanted = [directory / f"bfme-state-{attempt}-shard-{index}"
              for index in range(len(plan["batches"]))]
    if (set(wanted) != set(directory.iterdir())
            or any(path.is_symlink() or not path.is_dir() for path in wanted)):
        raise ValueError("Expected every fresh parity shard. Found missing or foreign artifacts.")
    return wanted


def assembled(root, plan, shard_dirs, *, required=None):
    shards = []
    for index, root_dir in enumerate(shard_dirs):
        path = root_dir / f"state-shard-{index}.json"
        doc = load(path, MAX_SHARD_BYTES)
        if doc.get("batch") != index:
            raise ValueError("A state shard's batch does not match its artifact.")
        shards.append(doc)
    return assemble(plan, shards, root, required=required)


def assemble_evidence(root, plan, shard_dirs):
    main = plan["main"]
    tree = Git(root / ".git").value("rev-parse", main + "^{tree}")
    identity = header(main, tree)
    evidence = {}
    for index, paths in enumerate(plan["batches"]):
        row = load(shard_dirs[index] / f"base-shard-{index}.json", MAX_SHARD_BYTES)
        if (row.pop("plan", None) != digest(plan) or row.pop("batch", None) != index
                or set(row["evidence"]) != set(paths)):
            raise ValueError("Exact base evidence has incomplete or foreign translation units.")
        commands = [{"tu": tu, **{key: row["evidence"][tu]["compiler"][key] for key in ("source", "contract")}}
                    for tu in paths]
        evidence.update(cached_pairs(row, identity, commands))
    return identity | {"evidence": evidence}


def main(mode, *args):
    root = Path.cwd()
    main = os.environ["GITHUB_SHA"]
    if (os.environ.get("GITHUB_REPOSITORY") != REPO or os.environ.get("GITHUB_REPOSITORY_ID") != str(REPO_ID)
            or os.environ.get("GITHUB_REF") != "refs/heads/main" or not HEX40.fullmatch(main)):
        raise ValueError("Use the configured protected repository and main revision.")
    if mode == "plan":
        if os.environ.get("GITHUB_EVENT_NAME") == "repository_dispatch":
            event = read(os.environ["GITHUB_EVENT_PATH"], 128 * 1024)
            record = event.get("client_payload")
            if (event.get("action") != "bfme-landed" or not isinstance(record, dict)
                    or set(record) != {"number", "head", "parent", "commit", "tree"}
                    or type(record["number"]) is not int or not 0 < record["number"] < 10**9
                    or not all(isinstance(record[key], str) and HEX40.fullmatch(record[key])
                               for key in ("head","parent","commit","tree"))
                    or record["commit"] != main):
                raise ValueError("Expected the exact protected landing receipt. Found an invalid or stale dispatch.")
            checkout = Git(root / ".git")
            if (checkout.rev(main + "^") != record["parent"]
                    or checkout.value("rev-parse", main + "^{tree}") != record["tree"]):
                raise ValueError("Expected the landed parent and tree from the repository. Found different Git objects.")
        units = authoritative_units(root, main)
        carried = inherited(root, main, Path(args[2])) if len(args) == 3 else {}
        measured = sorted(set(units) - set(carried))
        if not measured:
            measured.append(units[0])
            del carried[units[0]]
        try:
            result = make_checked_plan(root, main, plan_budget(os.environ), measured=measured)
        except OverBudget as error:
            if skip_over_budget(error, os.environ, os.environ["GITHUB_OUTPUT"]):
                return
            raise
        print(f"Measuring {len(measured)} units. Reusing {len(carried)} unaffected units from the exact predecessor.")
        if len(args) == 3:
            tree = Git(root / ".git").value("rev-parse", main + "^{tree}")
            stored = canonical(header(main, tree) | {"plan": digest(result), "evidence": carried})
            if len(stored) > LIMIT:
                raise ValueError("Inherited state exceeds its artifact allowance.")
            with Path(args[1]).open("xb") as file:
                file.write(stored)
        batches = ",".join(str(index) for index in range(len(result["batches"])))
        with Path(os.environ["GITHUB_OUTPUT"]).open("a") as file:
            file.write(f"batches=[{batches}]\nactive=yes\n")
        out = Path(args[0])
    elif mode == "assemble":
        plan, shards, out, *cache = map(Path, args)
        plan = load(plan, MAX_PLAN_BYTES)
        if plan["main"] != main or plan["checker"] != PIN["checker"]:
            raise ValueError("The state plan names a different revision or protected checker.")
        validate_plan(plan)
        attempt = os.environ["GITHUB_RUN_ATTEMPT"]
        plan_attempt = os.environ.get("BFME_STATE_PLAN_ATTEMPT")
        if plan_attempt is None:
            shard_dirs = select_fresh_shards(plan, shards, attempt)
        else:
            shard_dirs = select_shards(plan, shards, plan_attempt, attempt)
        required = authoritative_units(root, main)
        tree = Git(root / ".git").value("rev-parse", main + "^{tree}")
        identity = header(main, tree)
        carry = {}
        if len(cache) == 2:
            saved = load(cache[1], LIMIT)
            if saved.pop("plan", None) != digest(plan):
                raise ValueError("Inherited evidence belongs to a different refresh plan.")
            carry = saved["evidence"]
            commands = [{"tu": tu, **{key: entry["compiler"][key] for key in ("source", "contract")}}
                        for tu, entry in carry.items()]
            cached_pairs(saved, identity, commands)
        dirty = [tu for batch in plan["batches"] for tu in batch]
        result = assembled(root, plan, shard_dirs, required=dirty)
        carried_numeric = compact({"binding": {}, "results": [entry.get("wide_native", entry["native"]) | {"tu": tu}
                                                                for tu, entry in carry.items()]},
                                  carry, prehashed=True)
        result["tus"] = merge(required, result["tus"], carried_numeric)
        if cache:
            assembled_cache = assemble_evidence(root, plan, shard_dirs)
            assembled_cache["evidence"] = merge(required, assembled_cache["evidence"], carry)
            content = canonical(assembled_cache)
            if len(content) > LIMIT:
                raise ValueError("Exact parent evidence exceeds its publication budget.")
            with cache[0].open("xb") as file:
                file.write(content)
    else:
        raise ValueError("Unknown protected state workflow operation.")
    with out.open("xb") as file:
        file.write(canonical(result))


if __name__ == "__main__":
    os.umask(0o077)
    main(*sys.argv[1:])
