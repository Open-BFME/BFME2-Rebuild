"""Report newly defined public COFF data without exporting raw object contents."""
import re

MAX_NAMES = 8
MAX_COUNT = 250_000
PUBLIC_NAME = re.compile(r"[A-Za-z0-9_?@$][A-Za-z0-9_?@$.:]{0,255}\Z")


def public_data(coff):
    names = set()
    for symbol in coff.symbols:
        if symbol.get("storage") != 2 or symbol.get("type", 0) & 0x20:
            continue
        if not PUBLIC_NAME.fullmatch(symbol.get("name", "")):
            continue
        section = symbol["section"]
        common = section == 0 and symbol.get("value", 0) > 0
        defined = 0 < section <= len(coff.sections)
        if common or (defined and not coff.sections[section - 1]["characteristics"] & 0x20000020):
            names.add(symbol["name"])
    return names


def compare_public_data(before, after, known):
    extra = sorted(after - before - known)
    return {"count": len(extra), "names": extra[:MAX_NAMES]} if extra else None


def known_reference_names(judge, checkout):
    symbols = judge.config_json(checkout / "config/symbols.json")
    return (set(judge.MAP_NAME_INDEX)
            | set(symbols["symbols"])
            | set(symbols.get("vtables", ())))


def record_pairs(judge, checkout, objects, outcomes):
    pairs = [(path, before, after) for path, before, after in objects
             if before is not None and after is not None and before.is_file() and after.is_file()]
    if not pairs:
        return
    known = known_reference_names(judge, checkout)
    for path, before, after in pairs:
        if extra := compare_public_data(public_data(judge.coff(before)), public_data(judge.coff(after)), known):
            outcomes[path]["emitted_data"] = extra


def valid_witness(item):
    if not isinstance(item, dict) or set(item) != {"count", "names"}:
        return False
    names = item["names"]
    count = item["count"]
    return (type(count) is int and 0 < count <= MAX_COUNT and isinstance(names, list)
            and len(names) == min(count, MAX_NAMES)
            and all(isinstance(name, str) and PUBLIC_NAME.fullmatch(name) for name in names)
            and names == sorted(set(names)))
