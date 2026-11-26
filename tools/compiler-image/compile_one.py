#!/usr/bin/env python3
"""Compile one translation unit through the installed sealed compiler entry.

Matches the flag selection, STLport/SDK/allocator opt-in rules, and environment
used by tools/judge/bin/judge.py and ci/managed-nt2/pool_compare.py.
"""
import argparse
import hashlib
import json
import os
import re
import subprocess
import sys
import tempfile
from pathlib import Path

SEALED = Path("/opt/boat-compiler")
ENTRY = Path("/usr/local/libexec/bfme2-compiler/run")
MAX_REPLY = 96 * 1024**2

sys.path.insert(0, str(SEALED / "guard"))
from pool_protocol import (  # noqa: E402
    MAX_FILE,
    MAX_FILES,
    MAX_OBJECT,
    MAX_SOURCE,
    VERSION,
    check_object,
    decode,
    digest,
    encode,
    pack_sources,
    source_path,
)

BASE = [
    "-nologo", "-c", "-O2", "-GR-", "-EHsc-", "-O1",
    "-arch:SSE", "-G7", "-DNDEBUG", "-MD", "-EHsc", "-Gy",
]
ALLOWED = re.compile(
    r"^[-/](O[12bdgistxy][0-9-]*|Ob[0-2]|Oy-?|G[3-7BRSdrsyFfhz]-?|GS-?|Gy-?|"
    r"EH[sca-]+|arch:SSE2?|Zc:[a-zA-Z]+-?|"
    r"D[A-Za-z_][A-Za-z0-9_]*(=[A-Za-z0-9_]*)?|U[A-Za-z_]+|Zp[1248]?|"
    r"J|vmg|vm[bsmv]|GX-?|GF|Gm-?|MD|MT|ML)$"
)
ALLOC_TOKEN = "-Ireference/shims/bfmealloc_alloconly"
STL_DEFAULT_LINES = (
    "\n// stlport46\n"
    "// cl: /D_STLP_USE_STATIC_LIB /D_STLP_USE_MALLOC /D_BFME_RETAIL_TREE_INSERT_LAYOUT\n"
)
STL_DEFAULT_EXEMPT = ("src/zlib-1.1.4/", "src/lua-4.0.1/")


def json_bytes(value):
    return json.dumps(value, sort_keys=True, ensure_ascii=True, separators=(",", ":")).encode("ascii")


def marker(text, name):
    return re.search(r"^// " + re.escape(name) + r"[ \t]*$", text, re.M) is not None


# Libraries built without /MD (tools/judge/bin/judge.py STATIC_CRT_LIBRARIES).
STATIC_CRT_LIBRARIES = ("src/Code/Libraries/Source/Apt/", "src/fesl/", "src/Code/Libraries/Source/core/")
STATIC_CRT_LINE = "\n// cl-runtime: static\n"


def effective_source(tu, text):
    if tu.startswith(STATIC_CRT_LIBRARIES):
        text += STATIC_CRT_LINE
    if not tu.startswith("src/") or not tu.lower().endswith(".cpp") or tu.startswith(STL_DEFAULT_EXEMPT):
        return text
    if any(marker(text, name) for name in ("stlport46", "stlport", "msvcstl")):
        return text
    return text + STL_DEFAULT_LINES


def compiler_base(text):
    if re.search(r"^// cl-profile: upstream[ \t]*$", text, re.M):
        return ["-nologo", "-c", "-Gy"]
    base = list(BASE)
    if re.search(r"^// cl-arch: x87[ \t]*$", text, re.M):
        base = [flag for flag in base if flag != "-arch:SSE"]
    if re.search(r"^// cl-runtime: static[ \t]*$", text, re.M):
        base = ["-MT" if flag == "-MD" else flag for flag in base]
    return base


def build_plan(tu, raw_bytes):
    text = effective_source(tu, raw_bytes.decode("utf-8-sig", "replace"))
    flags = []
    for line in text.splitlines():
        if line.startswith("// cl:"):
            for tok in line[6:].split():
                tok = tok.replace("/", "-", 1) if tok.startswith("/") else tok
                if tok == ALLOC_TOKEN:
                    flags.append(tok)
                    continue
                if re.fullmatch(r"-W[0-4]", tok):
                    flags.append(tok)
                    continue
                if (tok.startswith("-I") and tok[2:].startswith(("include/", "src/"))
                        and ".." not in Path(tok[2:]).parts and "\\" not in tok[2:]):
                    flags.append(tok)
                    continue
                if not ALLOWED.match(tok):
                    raise ValueError(f"flag refused: {tok}")
                flags.append(tok)

    allocation = marker(text, "bfmealloc_alloconly") or ALLOC_TOKEN in flags
    flags = [flag for flag in flags if flag != ALLOC_TOKEN]
    use_stl46 = marker(text, "stlport46")
    use_stl45 = (not use_stl46) and marker(text, "stlport")
    if allocation and not (use_stl46 or use_stl45):
        raise ValueError("bfmealloc_alloconly requires // stlport or // stlport46")

    if use_stl46:
        if allocation:
            flags.append("-I@overlay/bfmealloc_alloconly")
        flags.extend(["-I@overlay/base", "-I@stlport46/stlport"])
    elif use_stl45:
        if allocation:
            flags.append("-I@allocator")
        flags.append("-I@stlport45")

    if allocation:
        if "-U_STLP_USE_STATIC_LIB" in flags:
            raise ValueError("bfmealloc_alloconly requires _STLP_USE_STATIC_LIB")
        if not any(flag.startswith("-D_STLP_USE_STATIC_LIB") for flag in flags):
            flags.append("-D_STLP_USE_STATIC_LIB")

    sdk_flags = []
    dx = marker(text, "dxsdk")
    platform = dx or marker(text, "psdk") or marker(text, "platformsdk")
    if dx:
        sdk_flags.append("-I@dx")
    if platform:
        sdk_flags.append("-I@vc/Vc7/PlatformSDK/Include")

    base = compiler_base(text)
    full_flags = base + ["-showIncludes", *sdk_flags, *flags, "-Iinclude"]
    env = {
        "INCLUDE": ["vc/Vc7/include"],
        "LANG": "C",
        "LC_ALL": "C",
        "LIB": ["vc/Vc7/lib"],
        "WINEDEBUG": "-all",
        "WINEPATH": ["vc/Vc7/bin", "vc/Common7/IDE", "vcruntime"],
    }
    contract = digest(json_bytes({"flags": full_flags, "env": env, "tu": tu}))
    return {
        "tu": tu,
        "source": digest(raw_bytes),
        "flags": full_flags,
        "env": env,
        "contract": contract,
    }


def collect_sources(repo, tu):
    """Collect the repository's src/ and include/ projection for the compiler."""
    candidates = []
    git_dir = repo / ".git"
    if git_dir.exists():
        listed = subprocess.run(
            ["git", "-C", str(repo), "ls-files", "-z", "--", "include", "src"],
            capture_output=True, check=True,
        ).stdout
        candidates = [p for p in listed.decode("utf-8", "strict").split("\0") if p]
    else:
        for top in ("include", "src"):
            base = repo / top
            if base.is_dir():
                for path in sorted(base.rglob("*")):
                    if path.is_file() and not path.is_symlink():
                        candidates.append(path.relative_to(repo).as_posix())

    selected = [p for p in sorted(set(candidates)) if source_path(p) and (repo / p).is_file()]
    if tu not in selected:
        selected.append(tu)

    encoded = {}
    total = 0
    for rel in selected:
        data = (repo / rel).read_bytes()
        if len(data) > MAX_FILE:
            raise ValueError(f"Source file exceeds compiler size limit ({MAX_FILE} bytes): {rel}")
        total += len(data)
        if total > MAX_SOURCE or len(encoded) >= MAX_FILES:
            raise ValueError(
                f"Source projection exceeds compiler limits (max {MAX_FILES} files, {MAX_SOURCE} bytes)."
            )
        encoded[rel] = encode(data)
    return encoded


def invoke_entry(operation, payload=b""):
    cmd = ["/usr/bin/prlimit", f"--fsize={MAX_REPLY}:{MAX_REPLY}", "--"]
    if os.geteuid() != 0:
        cmd += ["/usr/bin/sudo", "-n", "--"]
    cmd += [str(ENTRY), operation]
    with tempfile.TemporaryFile() as out, tempfile.TemporaryFile() as err:
        proc = subprocess.run(
            cmd,
            input=payload,
            stdout=out,
            stderr=err,
            close_fds=True,
            timeout=480,
            env={"PATH": "/usr/bin:/bin", "LANG": "C", "LC_ALL": "C"},
            check=False,
        )
        out.seek(0)
        err.seek(0)
        raw = out.read(MAX_REPLY + 1)
        stderr_text = err.read(4096).decode("utf-8", "replace").strip()
        if proc.returncode or not 0 < len(raw) <= MAX_REPLY:
            raise RuntimeError(f"Compiler entry failed (exit {proc.returncode}): {stderr_text}")
    return json.loads(raw)


def main():
    parser = argparse.ArgumentParser(description="Compile one TU via /usr/local/libexec/bfme2-compiler/run.")
    parser.add_argument("source", help="Source path under src/ (e.g. src/Code/.../UnicodeString.cpp)")
    parser.add_argument("--repo", help="Repository checkout root (default: git top-level or cwd)")
    parser.add_argument("--out", help="Optional path to write the compiled COFF .obj file")
    parser.add_argument("--json", action="store_true", dest="as_json", help="Print full JSON result")
    parser.add_argument("--show-includes", action="store_true", help="Print recorded include list")
    args = parser.parse_args()

    if args.repo:
        repo = Path(args.repo).resolve(strict=True)
    else:
        top = subprocess.run(["git", "rev-parse", "--show-toplevel"], capture_output=True, text=True)
        repo = Path(top.stdout.strip()).resolve(strict=True) if top.returncode == 0 else Path.cwd().resolve()

    src_arg = Path(args.source)
    src_abs = src_arg.resolve(strict=True) if src_arg.is_absolute() else (repo / src_arg).resolve(strict=True)
    if not src_abs.is_relative_to(repo):
        raise SystemExit(f"Source file is outside repo root {repo}: {src_abs}")
    tu = src_abs.relative_to(repo).as_posix()
    if not source_path(tu) or not tu.startswith("src/") or Path(tu).suffix.lower() not in {".c", ".cpp"}:
        raise SystemExit(f"Expected a .c or .cpp path under src/: {tu}")

    profile_bytes = (SEALED / "worker.json").read_bytes()
    profile_sha = digest(profile_bytes)
    raw_tu = src_abs.read_bytes()
    plan_entry = build_plan(tu, raw_tu)
    sources = collect_sources(repo, tu)
    wire = json_bytes({
        "version": VERSION,
        "profile": profile_sha,
        "source": pack_sources(sources),
        "plan": [plan_entry],
    })

    reply = invoke_entry("compile", wire)
    if reply.get("request") != digest(wire) or reply.get("profile") != profile_sha:
        raise SystemExit("Compiler response digest or profile mismatch.")
    result = reply["results"][0]
    if result["status"] != "compiled":
        if args.as_json:
            print(json.dumps(result, indent=2, sort_keys=True))
        else:
            print(f"COMPILE ERROR: {tu}", file=sys.stderr)
            print(result.get("diagnostics", ""), file=sys.stderr)
        return 1

    obj = decode(result["artifact"], MAX_OBJECT)
    obj_sha = check_object(obj)
    if obj_sha != result["sha256"]:
        raise SystemExit("COFF object hash mismatch.")
    normalized_sha = hashlib.sha256(obj[:4] + b"\x00\x00\x00\x00" + obj[8:]).hexdigest()

    if args.out:
        out_path = Path(args.out)
        out_path.parent.mkdir(parents=True, exist_ok=True)
        out_path.write_bytes(obj)

    summary = {
        "tu": tu,
        "source_sha256": result["source"],
        "object_sha256": obj_sha,
        "normalized_object_sha256": normalized_sha,
        "object_bytes": len(obj),
        "includes_count": len(result["includes"]),
        "flags": plan_entry["flags"],
        "env": plan_entry["env"],
        "profile_sha256": profile_sha,
    }
    if args.as_json or args.show_includes:
        summary["includes"] = result["includes"]

    if args.as_json:
        print(json.dumps(summary, indent=2, sort_keys=True))
    else:
        print(f"tu:                       {summary['tu']}")
        print(f"object_sha256:            {summary['object_sha256']}")
        print(f"normalized_object_sha256: {summary['normalized_object_sha256']}")
        print(f"object_bytes:             {summary['object_bytes']}")
        print(f"includes_count:           {summary['includes_count']}")
        print(f"profile_sha256:           {summary['profile_sha256']}")
        if args.show_includes:
            for kind, rel, h in result["includes"]:
                print(f"  {kind:10s} {h}  {rel}")
    return 0


if __name__ == "__main__":
    raise SystemExit(main())
