"""Bound, source-free parent-side receipts from the exact protected main."""

import math
from pathlib import Path

from common import HEX64, PIN, digest, policy, read
from emitted_data import compare_public_data, known_reference_names, public_data
from managed_matrix import SIDES, check_native
from reporting import GATES
from virtual_atoms import collect

SCHEMA = 2
LIMIT = 128 * 1024**2
ROW_KEYS = {"rva", "status", "credited", "proof", "pct"}
PARENT = SIDES["base"]


def gate_fingerprint(value):
    reason = next((phrase for phrase in GATES if phrase in value), "native")
    category = next((label for label in ("annotations", "banned construct", "assembly", " asm ")
                     if label in value), "")
    if category == " asm ":
        category = "assembly"
    # Keep the reduction's category and its existing owner-facing remedy.
    label = category.strip() + " " + reason if category and category not in reason else reason
    return label + " " + digest(value.encode())


def hashed_name(name):
    return digest(name.encode("ascii"))


def object_path(job, tu):
    return job / "obj" / (tu.replace("/", "_") + ".obj")


def compiler_record(command, side, outcome, job):
    row = {key: command[key] for key in ("tu", "source", "contract")} | {"side": side}
    output = object_path(job, command["tu"])
    if not output.is_file():
        if not outcome.get("error"):
            raise ValueError("The compiler omitted its exact outcome.")
        return row | {"status": "error"}
    witness = read(output.with_suffix(".obj.compile.json"), 4 * 1024**2)
    if (witness["source_sha256"] != command["source"]
            or witness["compiler_contract"] != command["contract"]
            or not HEX64.fullmatch(witness["object_sha256"])):
        raise ValueError("The object witness differs from the protected compiler input.")
    dependencies = sorted(
        [str(Path(path).relative_to(job)) if Path(path).is_relative_to(job)
         else "sealed/" + Path(path).name, checksum]
        for path, checksum in witness["headers"].items())
    return row | {"status": "compiled", "object_sha256": witness["object_sha256"],
                  "dependencies": dependencies}


def make_entry(judge, command, outcome, job):
    tu = command["tu"]
    compiler = compiler_record(command, PARENT, outcome, job)
    inline = collect({"shard": 0, "plan_sha256": "", "results": [compiler]},
                     find_object=lambda _side, name: object_path(job, name))["records"][0]
    names = (public_data(judge.coff(object_path(job, tu))) if compiler["status"] == "compiled" else ())
    return {"native": native_record(outcome), "compiler": compiler, "inline": inline,
            "data": sorted({hashed_name(name) for name in names})}


def native_record(outcome):
    record = {
        "rows": [{key: value for key, value in row.items() if key in ROW_KEYS} for row in outcome["rows"]],
        "gates": sorted({gate_fingerprint(value) for value in outcome["gates"]}),
        "object_verified": bool(outcome["object_verified"]),
    }
    # Persist the COFF and relocation witness; a MATCH alone does not prove selection 2.
    providers = {key["rva"] for key in outcome.get("select_any_providers", ())}
    matched = sorted({int(row["rva"], 16) for row in outcome["rows"] + outcome.get("expected", [])
                      if row["status"] == "MATCH" and int(row["rva"], 16) in providers})
    if record["object_verified"] and matched:
        record["select_any_matches"] = matched
    if outcome.get("error"):
        record["error"] = "Compilation failed in an affected translation unit."
    return record


def validate_native(native, compiler):
    if (set(native) - {"rows", "gates", "object_verified", "error", "select_any_matches"}
            or bool(native.get("error")) != (compiler["status"] == "error")):
        raise ValueError("Cached comparison contains private or unsupported fields.")
    check_native(native)
    matches = native.get("select_any_matches", [])
    if (not isinstance(matches, list) or any(type(rva) is not int or not 0 < rva < 2**32 for rva in matches)
            or matches != sorted(set(matches)) or (matches and not native["object_verified"])):
        raise ValueError("Invalid select-any provider matches.")
    if any(set(item) - ROW_KEYS or ("pct" in item and
           (type(item["pct"]) not in {float, int} or not math.isfinite(item["pct"])
            or not 0 <= item["pct"] <= 100)) for item in native["rows"]):
        raise ValueError("Cached comparison contains an unsupported row.")


def header(sha, tree):
    return {"schema": SCHEMA, "sha": sha, "tree": tree, "pins": PIN, "policy": policy(),
            "engine": digest((Path(__file__).resolve().parent.parent /
                              "managed-nt2/bundle.sha256").read_bytes())}


def cached_pairs(snapshot, header_fields, commands):
    if (not isinstance(snapshot, dict) or set(snapshot) != set(header_fields) | {"evidence"}
            or any(snapshot.get(field) != value for field, value in header_fields.items())):
        raise ValueError("State belongs to a different protected main or checker.")
    evidence = snapshot["evidence"]
    if not isinstance(evidence, dict):
        raise ValueError("State has no exact-parent evidence.")
    trusted = {}
    for command in commands:
        tu = command["tu"]
        row = evidence[tu]
        required = {"native", "compiler", "inline", "data"}
        if set(row) not in (required, required | {"wide_native"}):
            raise ValueError("Cached evidence has unexpected fields.")
        compiler, inline, native, names = (row[key] for key in ("compiler", "inline", "native", "data"))
        if (any(compiler.get(key) != command[key] for key in ("tu", "source", "contract"))
                or compiler["side"] != PARENT or compiler["status"] not in {"compiled", "error"}
                or inline["tu"] != tu or inline["side"] != PARENT
                or bool(native.get("error")) != (compiler["status"] == "error")):
            raise ValueError("Cached comparison has a different protected compiler input.")
        validate_native(native, compiler)
        if "wide_native" in row:
            validate_native(row["wide_native"], compiler)
        expected = {"tu", "source", "contract", "side", "status"}
        if compiler["status"] == "compiled":
            expected |= {"object_sha256", "dependencies"}
            dependencies = compiler["dependencies"]
            if (not HEX64.fullmatch(compiler["object_sha256"]) or not isinstance(dependencies, list)
                    or len(dependencies) > 4096 or any(
                        not isinstance(pair, list) or len(pair) != 2 or not isinstance(pair[0], str)
                        or not 0 < len(pair[0]) < 512 or not HEX64.fullmatch(pair[1]) for pair in dependencies)):
                raise ValueError("Cached compiler witness is invalid.")
        if set(compiler) != expected:
            raise ValueError("Cached compiler witness has unexpected fields.")
        if set(inline) not in ({"side", "tu", "tables", "bodies"},
                               {"side", "tu", "tables", "bodies", "body_witnesses"},
                               {"side", "tu", "unavailable"}):
            raise ValueError("Cached virtual effect record is invalid.")
        if (not isinstance(names, list) or len(names) > 250_000
                or not all(isinstance(name, str) and HEX64.fullmatch(name) for name in names)
                or names != sorted(set(names))):
            raise ValueError("Cached comparison contains invalid data symbol identities.")
        trusted[tu] = row
    return trusted


def read_parent(path, git, revision, commands):
    if not path:
        return {}
    try:
        tree = git.value("rev-parse", revision + "^{tree}")
        return cached_pairs(read(path, LIMIT), header(revision, tree), commands)
    except (OSError, ValueError, TypeError, KeyError, AttributeError):
        print("Exact main state unavailable. Compiling the protected parent.")
        return {}


def record_cached_data(judge, checkout, entries, job, outcomes):
    known = None
    for tu, old in entries.items():
        new = object_path(job, tu)
        if old["compiler"]["status"] != "compiled" or not new.is_file():
            continue
        if known is None:
            known = known_reference_names(judge, checkout)
        old_names = set(old["data"])
        after = {name for name in public_data(judge.coff(new)) if hashed_name(name) not in old_names}
        if extra := compare_public_data(set(), after, known):
            outcomes[tu]["emitted_data"] = extra

