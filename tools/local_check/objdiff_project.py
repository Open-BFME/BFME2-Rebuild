#!/usr/bin/env python3
"""objdiff project for one translation unit: retail target object, compiled base object, objdiff.json.

usage:
  objdiff_project.py project --unit src/...cpp [--dir DIR] [--revision REV] [--config FILE]
  objdiff_project.py build --unit src/...cpp [--config FILE] OUTPUT.obj      (objdiff's custom_make)

The target object holds the retail bytes of every .text row of expected/<unit>.json, read from
baseline/game.dat, one COMDAT section per function under the ledger's symbol name, so objdiff pairs
each retail function with the compiled function of the same name. The base object is the unit
compiled from the working tree with the judge's own cl.exe command (native cl.exe on Windows,
wine cl.exe elsewhere); objdiff rebuilds it through `build` and reloads on every change.

The project directory lives in the per-user cache, never in the checkout. objdiff compares bytes as
they are: relocated operands differ between retail (absolute addresses) and the object (zero plus a
relocation), which the judge masks; read judge verdicts in precheck.py's output or live viewer.
"""
import argparse
import json
import re
import shutil
import struct
import subprocess
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO_ROOT = HERE.parents[1]
sys.path.insert(0, str(REPO_ROOT / "tools" / "judge"))
from binary_model import PE  # noqa: E402  (the judge's bounded, pure-Python PE reader)

SCHEMA_URL = "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json"
MIN_VERSION = "2.0.0"

IMAGE_FILE_MACHINE_I386 = 0x14C
TEXT_FLAGS = 0x60000020 | 0x00100000 | 0x1000   # CODE | EXECUTE | READ, ALIGN_1BYTES, LNK_COMDAT
STORAGE_EXTERNAL, STORAGE_STATIC = 2, 3
FUNCTION_TYPE = 0x20
COMDAT_ANY = 2
IMAGE_SCN_CNT_CODE = 0x20


def slug(tu):
    return re.sub(r"[^A-Za-z0-9_.-]+", "_", tu[4:] if tu.startswith("src/") else tu)


# ---------------------------------------------------------------------------
# COFF writing and reading


def coff_object(functions):
    """An i386 COFF object with one COMDAT .text section per (name, bytes) function."""
    count = len(functions)
    header_size = 20 + 40 * count
    raw, pointers, offset = [], [], header_size
    for _name, body in functions:
        pointers.append(offset)
        raw.append(bytes(body))
        offset += len(body)
    symbol_table = offset
    strings = bytearray()
    symbols = bytearray()

    def name_field(name):
        encoded = name.encode("utf-8")
        if len(encoded) <= 8:
            return encoded.ljust(8, b"\0")
        position = 4 + len(strings)
        strings.extend(encoded + b"\0")
        return struct.pack("<II", 0, position)

    for index, (name, body) in enumerate(functions, start=1):
        symbols += struct.pack("<8sIhHBB", b".text\0\0\0", 0, index, 0, STORAGE_STATIC, 1)
        symbols += struct.pack("<IHHIhB3x", len(body), 0, 0, 0, 0, COMDAT_ANY)
        symbols += struct.pack("<8sIhHBB", name_field(name), 0, index, FUNCTION_TYPE, STORAGE_EXTERNAL, 0)
    sections = b"".join(struct.pack("<8sIIIIIIHHI", b".text\0\0\0", 0, 0, len(body), pointer, 0, 0, 0, 0,
                                    TEXT_FLAGS) for body, pointer in zip(raw, pointers))
    header = struct.pack("<HHIIIHH", IMAGE_FILE_MACHINE_I386, count, 0, symbol_table, 3 * count, 0, 0)
    return header + sections + b"".join(raw) + bytes(symbols) + struct.pack("<I", 4 + len(strings)) + bytes(strings)


def read_coff(data):
    """(sections, symbols) of an i386 COFF object; enough to find function bodies and relocations."""
    machine, count, _stamp, table, symbol_count, optional, _flags = struct.unpack_from("<HHIIIHH", data, 0)
    if machine != IMAGE_FILE_MACHINE_I386:
        raise ValueError("not an i386 COFF object")
    strings = data[table + 18 * symbol_count:]
    sections = []
    for i in range(count):
        name, _vs, _va, size, pointer, relocs, _lines, nrelocs, _nlines, flags = struct.unpack_from(
            "<8sIIIIIIHHI", data, 20 + optional + 40 * i)
        sections.append({"name": name.rstrip(b"\0").decode("ascii", "replace"), "size": size, "pointer": pointer,
                         "relocs": [struct.unpack_from("<IIH", data, relocs + 10 * k) for k in range(nrelocs)],
                         "flags": flags})
    symbols, index = [], 0
    while index < symbol_count:
        raw_name, value, section, kind, storage, aux = struct.unpack_from("<8sIhHBB", data, table + 18 * index)
        if raw_name[:4] == b"\0\0\0\0":
            start = struct.unpack_from("<I", raw_name, 4)[0]
            name = strings[start:strings.index(b"\0", start)].decode("utf-8", "replace")
        else:
            name = raw_name.rstrip(b"\0").decode("utf-8", "replace")
        symbols.append({"index": index, "name": name, "value": value, "section": section, "type": kind,
                        "storage": storage})
        index += 1 + aux
    return sections, symbols


def function_bodies(data):
    """{symbol: (bytes, [relocation offsets])} for every code symbol of the object."""
    sections, symbols = read_coff(data)
    bodies = {}
    for symbol in symbols:
        if symbol["section"] <= 0 or not symbol["type"] & FUNCTION_TYPE or symbol["name"].startswith("."):
            continue
        section = sections[symbol["section"] - 1]
        if not section["flags"] & IMAGE_SCN_CNT_CODE:
            continue
        later = [s["value"] for s in symbols if s["section"] == symbol["section"] and s["value"] > symbol["value"]
                 and s["type"] & FUNCTION_TYPE]
        end = min(later) if later else section["size"]
        start = section["pointer"] + symbol["value"]
        body = data[start:section["pointer"] + end]
        relocs = [offset - symbol["value"] for offset, _index, kind in section["relocs"]
                  if kind and symbol["value"] <= offset < end]
        bodies[symbol["name"]] = (body, relocs)
    return bodies


def masked(body, relocs):
    """Relocated operands zeroed, as judge.py compares them."""
    out = bytearray(body)
    for offset in relocs:
        out[offset:offset + 4] = bytes(len(out[offset:offset + 4]))
    return bytes(out)


# ---------------------------------------------------------------------------
# Retail side


def read_json(repo, relative, revision=None):
    if revision:
        done = subprocess.run(["git", "-C", str(repo), "show", f"{revision}:{relative}"], capture_output=True)
        if done.returncode:
            return None
        return json.loads(done.stdout.decode("utf-8"))
    path = Path(repo) / relative
    return json.loads(path.read_text(encoding="utf-8")) if path.is_file() else None


def expected_rows(repo, tu, revision=None):
    """The unit's .text rows, [{rva, size, name, role}]: its objects' rows in the ledger (config/map.json,
    the owned functions) plus expected/<unit>.json (the emitted ones), from REVISION or the working tree."""
    found = {}
    ledger = read_json(repo, "config/map.json", revision) or {}
    objects = {item["object_id"] for item in ledger.get("objects", []) if item.get("tu") == tu}
    record = read_json(repo, "expected/" + tu[4:] + ".json", revision) or {}
    for row in [r for r in ledger.get("rows", []) if r.get("object") in objects] + record.get("rows", []):
        if row.get("section", ".text") == ".text" and row.get("size") and row.get("name"):
            found.setdefault(int(row["rva"], 16), {"rva": int(row["rva"], 16), "size": int(row["size"]),
                                                   "name": row["name"], "role": row.get("role", "")})
    return [found[rva] for rva in sorted(found)]


def retail_functions(retail, rows):
    image = PE(retail) if not isinstance(retail, PE) else retail
    seen, functions = set(), []
    for row in rows:
        if row["name"] in seen:
            continue
        seen.add(row["name"])
        functions.append((row["name"], image.read(row["rva"], row["size"])))
    return functions


# ---------------------------------------------------------------------------
# Project


def project_config(tu, *, target, base, repo, builder):
    """objdiff.json (https://github.com/encounter/objdiff config.schema.json, objdiff 2.x and 3.x)."""
    return {
        "$schema": SCHEMA_URL,
        "min_version": MIN_VERSION,
        "custom_make": builder[0],
        "custom_args": builder[1:],
        "build_target": False,
        "build_base": True,
        # precheck.py rewrites target/ every round; objdiff then rebuilds base/ (never watched: no loop).
        "watch_patterns": ["target/*.obj"],
        "units": [{
            "name": tu,
            "target_path": target,
            "base_path": base,
            "metadata": {"source_path": str(Path(repo) / tu), "complete": False},
        }],
    }


def write_project(directory, tu, *, repo=REPO_ROOT, revision=None, compiled=None, config_file=None,
                  python=sys.executable):
    """Write target/, base/ and objdiff.json under DIRECTORY. Returns the objdiff.json path."""
    directory = Path(directory)
    name = slug(tu)
    target, base = f"target/{name}.obj", f"base/{name}.obj"
    rows = expected_rows(repo, tu, revision)
    (directory / "target").mkdir(parents=True, exist_ok=True)
    (directory / "base").mkdir(parents=True, exist_ok=True)
    (directory / target).write_bytes(coff_object(retail_functions(Path(repo) / "baseline" / "game.dat", rows)))
    if compiled is not None and Path(compiled).is_file() and Path(compiled).resolve() != (directory / base).resolve():
        shutil.copyfile(compiled, directory / base)
    builder = [python, str(HERE / "objdiff_project.py"), "build", "--unit", tu, "--repo", str(repo)]
    if config_file:
        builder += ["--config", str(config_file)]
    config = project_config(tu, target=target, base=base, repo=repo, builder=builder)
    path = directory / "objdiff.json"
    path.write_text(json.dumps(config, indent=2) + "\n", encoding="utf-8")
    return path


def compile_working_tree(tu, output, *, repo=REPO_ROOT, config_file=None, run=subprocess.run):
    """Compile TU from the working tree with the judge's cl.exe command; the object goes to OUTPUT."""
    sys.path.insert(0, str(HERE))
    import precheck
    config = precheck.load_toolchain(config_file)
    backend = precheck.compiler_backend("native" if sys.platform == "win32" else "wine", config)
    output = Path(output).resolve()
    output.parent.mkdir(parents=True, exist_ok=True)
    text = (Path(repo) / tu).read_text(encoding="utf-8-sig", errors="replace")
    argv, env = backend.command(tu, text, output)
    done = run(argv, cwd=str(repo), env=env, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
               stderr=subprocess.STDOUT, timeout=420)
    out = done.stdout.decode("utf-8", "replace") if isinstance(done.stdout, bytes) else (done.stdout or "")
    lines = [line for line in out.splitlines() if not line.startswith("Note: including file:")]
    return done.returncode == 0 and output.is_file(), lines


def default_directory(tu):
    import bootstrap
    return bootstrap.cache_root() / "objdiff" / slug(tu)


def main(argv=None):
    sys.path.insert(0, str(HERE))
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    sub = parser.add_subparsers(dest="command", required=True)
    project = sub.add_parser("project", help="write the objdiff project for a unit")
    project.add_argument("--unit", required=True)
    project.add_argument("--dir", type=Path)
    project.add_argument("--revision", help="read expected/ from this revision (default: working tree)")
    project.add_argument("--repo", type=Path, default=REPO_ROOT)
    project.add_argument("--config", type=Path)
    project.add_argument("--no-compile", action="store_true")
    build = sub.add_parser("build", help="compile the unit's base object (objdiff custom_make)")
    build.add_argument("--unit", required=True)
    build.add_argument("--repo", type=Path, default=REPO_ROOT)
    build.add_argument("--config", type=Path)
    build.add_argument("output", type=Path)
    args = parser.parse_args(argv)
    if args.command == "build":
        ok, lines = compile_working_tree(args.unit, args.output, repo=args.repo, config_file=args.config)
        print("\n".join(lines))
        return 0 if ok else 1
    directory = args.dir or default_directory(args.unit)
    compiled = None
    if not args.no_compile:
        compiled = directory / "base" / (slug(args.unit) + ".obj")
        ok, lines = compile_working_tree(args.unit, compiled, repo=args.repo, config_file=args.config)
        if not ok:
            print("\n".join(lines[-15:]), file=sys.stderr)
    path = write_project(directory, args.unit, repo=args.repo, revision=args.revision, config_file=args.config)
    print(f"objdiff project: {path}  (open this directory in objdiff)")
    return 0


if __name__ == "__main__":
    sys.exit(main())
