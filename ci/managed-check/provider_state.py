"""Bind unchanged select-any providers to the protected plan before partitioning its TUs."""
import os
from pathlib import Path
import sys

from common import read

STATE = Path(__file__).resolve().parents[1] / "managed-state"
sys.path.append(str(STATE))


def eligible(snapshot, identity, affected, changed):
    if (set(snapshot) != set(identity) | {"evidence"}
            or any(snapshot[key] != value for key, value in identity.items())):
        return {}
    excluded, changed = set(affected), set(changed)
    providers = {}
    for tu, entry in snapshot["evidence"].items():
        native = entry.get("wide_native", entry["native"])
        compiler = entry["compiler"]
        if (tu in excluded or tu in changed or native.get("object_verified") is not True
                or native.get("error") or compiler["tu"] != tu or compiler["status"] != "compiled"
                or any(path in changed for path, _ in compiler["dependencies"])):
            continue
        nonmatches = {int(row["rva"], 16) for row in native["rows"] if row["status"] != "MATCH"}
        rows = [rva for rva in native.get("select_any_matches", []) if rva not in nonmatches]
        if rows:
            providers[tu] = rows
    return providers


def for_plan(git, base, affected, changed):
    path = os.environ.get("BFME_PROVIDER_STATE")
    if not path or not Path(path).is_file():
        return {}
    from state_evidence import LIMIT, header
    tree = git.value("rev-parse", base + "^{tree}")
    try:
        rows = eligible(read(path, LIMIT), header(base, tree), affected, changed)
    except (OSError, KeyError, ValueError, TypeError, AttributeError):
        rows = {}
    return {"select_any": rows} if rows else {}


def witnesses(providers, judge):
    ledger = judge.map_index(judge.MAP)
    result = []
    for tu, addresses in providers.items():
        units = ledger["objects"].get(tu, ())
        if len(units) != 1:
            continue
        unit = units[0]
        owner = unit["object_id"]
        order = str(unit.get("link_index", ""))
        if unit.get("library") or unit.get("contradictions") or not order.isascii() or not order.isdecimal():
            continue
        for address in addresses:
            row = ledger["rows"].get(address)
            if row and row["object"] == owner and not row.get("membership_hypothesis"):
                key = {"owner": owner, "object": owner, "rva": address,
                       "symbol": row["name"], "link_index": int(order)}
                result.append((tu, key))
    return result
