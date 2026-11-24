#!/usr/bin/env python3
"""Verify a bring-your-own Visual C++ 7.1 toolchain against the Boat compiler image.

Every digest comes from docs/compiler-image.md, the published manifest of the
hosted `bfme2-compiler-v1` image. This tool never downloads or copies a
compiler: point it at a copy you are entitled to use, kept outside the
checkout. On success it records the verified paths in a per-user
configuration file outside Git, which tools/local_check/precheck.py reads.

usage:
  verify_toolchain.py --vc71-root DIR [--runner-root DIR] [--demangler FILE]
                      [--vcruntime-root DIR] [--psdk-root DIR] [--dxsdk-root DIR]
                      [--stlport-root DIR] [--overlay-root DIR] [--allocator-root DIR]
                      [--tools-root DIR] [--strict] [--no-save] [--config FILE] [--json]

Exit status: 0 verified, 1 verification failed, 2 usage error.
"""
import argparse
import hashlib
import json
import os
import re
import subprocess
import sys
import time
from dataclasses import dataclass, field
from pathlib import Path

REPO_ROOT = Path(__file__).resolve().parents[2]
MANIFEST_DOC = REPO_ROOT / "docs" / "compiler-image.md"
DXSDK_MANIFEST = REPO_ROOT / "ci" / "managed-check" / "dxsdk-includes.manifest"
TOOLS_README = REPO_ROOT / "tools" / "README.md"

SECTION_BINARIES = "Required proprietary binary SHA-256 digests"
SECTION_SUBTREES = "Subtree manifest SHA-256 digests"
SECTION_FILES = "Approved STLport 4.6 overlay and allocator file digests"

CONFIG_VERSION = 1
CONFIG_ENV = "BFME_LOCAL_CHECK_CONFIG"

_HEX = r"[0-9a-f]{64}"
_FILE_ROW = re.compile(r"^\|\s*`(tools/[^`]+)`\s*\|\s*`(" + _HEX + r")`\s*\|\s*$")
_SUBTREE_ROW = re.compile(r"^\|\s*`(tools/[^`]+)`\s*\|\s*([\d,]+)\s*\|\s*`(" + _HEX + r")`\s*\|\s*$")
_RUNNER_REVISION = re.compile(r"Use revision `([0-9a-f]{40})`")


class ManifestError(ValueError):
    """docs/compiler-image.md no longer has the tables this tool reads."""


@dataclass
class Manifest:
    binaries: dict          # manifest path -> sha256
    subtrees: dict          # manifest path -> (file count, listing sha256)
    files: dict             # manifest path -> sha256 (overlay and allocator)
    digest: str = ""        # sha256 of the manifest document itself


def _section(text, title):
    """Lines of the Markdown section headed `### <title>`, up to the next heading."""
    lines = text.splitlines()
    for index, line in enumerate(lines):
        if line.strip().lstrip("#").strip() == title and line.lstrip().startswith("#"):
            body = []
            for following in lines[index + 1:]:
                if following.lstrip().startswith("#"):
                    break
                body.append(following)
            return body
    raise ManifestError(f"docs/compiler-image.md has no section titled {title!r}")


def parse_manifest(text):
    """Read the three digest tables of docs/compiler-image.md."""
    binaries = {}
    for line in _section(text, SECTION_BINARIES):
        match = _FILE_ROW.match(line.strip())
        if match:
            binaries[match[1]] = match[2]
    subtrees = {}
    for line in _section(text, SECTION_SUBTREES):
        match = _SUBTREE_ROW.match(line.strip())
        if match:
            subtrees[match[1]] = (int(match[2].replace(",", "")), match[3])
    files = {}
    for line in _section(text, SECTION_FILES):
        match = _FILE_ROW.match(line.strip())
        if match:
            files[match[1]] = match[2]
    if "tools/vc/Vc7/bin/cl.exe" not in binaries:
        raise ManifestError("the binary digest table does not pin tools/vc/Vc7/bin/cl.exe")
    return Manifest(binaries, subtrees, files, hashlib.sha256(text.encode("utf-8")).hexdigest())


def load_manifest(path=MANIFEST_DOC):
    return parse_manifest(Path(path).read_text(encoding="utf-8"))


def parse_dxsdk_manifest(text):
    """`<sha256>  <size>  Include/<name>` lines -> {Include/<name>: sha256}."""
    entries = {}
    for line in text.splitlines():
        parts = line.split()
        if len(parts) == 3 and re.fullmatch(_HEX, parts[0]):
            entries[parts[2]] = parts[0]
    return entries


def sha256_file(path):
    digest = hashlib.sha256()
    with open(path, "rb") as handle:
        for chunk in iter(lambda: handle.read(1024 * 1024), b""):
            digest.update(chunk)
    return digest.hexdigest()


def listing_digest(local_dir, manifest_prefix):
    """SHA-256 of the `sha256sum` listing the manifest uses.

    tools/compiler-image/fresh-install.sh lists `find | LC_ALL=C sort -z | xargs -0 sha256sum`:
    one `<sha256>  <manifest_prefix>/<relative path>\\n` line per file, ordered by the path's
    bytes (not by the whole line), so a local directory hashes exactly as its counterpart under
    /opt/boat-compiler does. Returns (file count, digest)."""
    local_dir = Path(local_dir)
    names = sorted((f"{manifest_prefix}/{path.relative_to(local_dir).as_posix()}".encode("utf-8"), path)
                   for path in local_dir.rglob("*") if path.is_file())
    lines = [sha256_file(path).encode("ascii") + b"  " + name + b"\n" for name, path in names]
    return len(lines), hashlib.sha256(b"".join(lines)).hexdigest()


@dataclass
class Roots:
    vc71: Path
    vcruntime: Path = None
    psdk: Path = None
    dxsdk: Path = None
    stlport46: Path = None
    overlay: Path = None
    allocator: Path = None
    runner: Path = None

    @classmethod
    def from_tools_root(cls, tools_root, **overrides):
        """The --inputs layout of tools/compiler-image/build.sh (docs/compiler-image.md)."""
        tools_root = Path(tools_root)
        values = dict(vc71=tools_root / "vc", vcruntime=tools_root / "vcruntime", dxsdk=tools_root / "dx",
                      stlport46=tools_root / "stlport-4.6", overlay=tools_root / "overlay",
                      allocator=tools_root / "allocator")
        values.update({key: value for key, value in overrides.items() if value is not None})
        return cls(**values)

    def resolved(self):
        """Fill defaults: the Platform SDK inside VC7.1, SDK trees inside the support checkout."""
        values = {name: (Path(value).expanduser().resolve() if value is not None else None)
                  for name, value in self.__dict__.items()}
        if values["psdk"] is None:
            values["psdk"] = values["vc71"] / "Vc7" / "PlatformSDK"
        runner = values["runner"]
        if runner is not None:
            if values["stlport46"] is None and (runner / "vendor" / "stlport-4.6").is_dir():
                values["stlport46"] = runner / "vendor" / "stlport-4.6"
            if values["dxsdk"] is None and (runner / "vendor" / "dxsdk-aug2005").is_dir():
                values["dxsdk"] = runner / "vendor" / "dxsdk-aug2005"
        if values["vcruntime"] is None:
            values["vcruntime"] = find_vcruntime(values["vc71"])
        return Roots(**values)

    def as_json(self):
        return {name: (str(value) if value is not None else None) for name, value in self.__dict__.items()}


def find_vcruntime(vc71):
    """The directory holding msvcp71.dll: the image's vcruntime/ sibling or the usual install places."""
    candidates = [vc71.parent / "vcruntime", vc71, vc71 / "Vc7" / "bin", vc71 / "Common7" / "IDE",
                  vc71.parent, vc71.parents[1] if len(vc71.parents) > 1 else vc71.parent]
    for candidate in candidates:
        if (candidate / "msvcp71.dll").is_file():
            return candidate
    return vc71.parent / "vcruntime"


def local_path(roots, manifest_path):
    """Map a `tools/...` manifest path to the user's copy, or None when that root was not given."""
    rest = manifest_path[len("tools/"):]
    table = (("vc/Vc7/PlatformSDK", roots.psdk), ("vc", roots.vc71), ("vcruntime", roots.vcruntime),
             ("dx", roots.dxsdk), ("stlport-4.6", roots.stlport46), ("overlay", roots.overlay),
             ("allocator", roots.allocator))
    for prefix, root in table:
        if rest == prefix or rest.startswith(prefix + "/"):
            if root is None:
                return None
            tail = rest[len(prefix):].lstrip("/")
            return Path(root, *tail.split("/")) if tail else Path(root)
    return None


@dataclass
class Report:
    roots: Roots
    errors: list = field(default_factory=list)
    warnings: list = field(default_factory=list)
    checked: dict = field(default_factory=dict)      # manifest path -> status
    cl_sha256: str = ""

    @property
    def ok(self):
        return not self.errors


def _check_file(report, roots, manifest_path, expected, required):
    path = local_path(roots, manifest_path)
    if path is None:
        report.checked[manifest_path] = "skipped"
        return
    if not path.is_file():
        report.checked[manifest_path] = "missing"
        (report.errors if required else report.warnings).append(f"missing {manifest_path}: expected at {path}")
        return
    actual = sha256_file(path)
    if actual != expected:
        report.checked[manifest_path] = "mismatch"
        report.errors.append(f"{manifest_path} at {path}: SHA-256 {actual}, Boat pins {expected}")
        return
    report.checked[manifest_path] = "ok"
    if manifest_path == "tools/vc/Vc7/bin/cl.exe":
        report.cl_sha256 = actual


def verify(roots, manifest, *, strict=False, dxsdk_entries=None, binaries_only=False):
    """Check every pinned binary (required) and every supplied header tree against the manifest."""
    roots = roots.resolved()
    report = Report(roots)
    if not (roots.vc71 / "Vc7" / "bin").is_dir():
        report.errors.append(f"{roots.vc71} is not a Visual Studio .NET 2003 root (no Vc7/bin directory)")
    for manifest_path, expected in sorted(manifest.binaries.items()):
        _check_file(report, roots, manifest_path, expected, required=True)
    if binaries_only:
        return report
    for manifest_path, expected in sorted(manifest.files.items()):
        _check_file(report, roots, manifest_path, expected, required=True)
    for manifest_path, (count, expected) in sorted(manifest.subtrees.items()):
        directory = local_path(roots, manifest_path)
        if directory is None or not directory.is_dir():
            report.checked[manifest_path] = "skipped"
            continue
        local_count, actual = listing_digest(directory, manifest_path)
        if actual == expected:
            report.checked[manifest_path] = "ok"
            continue
        report.checked[manifest_path] = "differs"
        message = (f"{manifest_path} at {directory}: {local_count} files, listing SHA-256 {actual}; "
                   f"Boat has {count} files, {expected}")
        (report.errors if strict else report.warnings).append(message)
    if dxsdk_entries and roots.dxsdk is not None and (roots.dxsdk / "Include").is_dir():
        for name, expected in sorted(dxsdk_entries.items()):
            _check_file(report, roots, "tools/dx/" + name, expected, required=True)
    for name in ("vc71", "runner"):
        root = getattr(roots, name)
        if root is not None and _inside(root, REPO_ROOT):
            report.errors.append(f"{root} is inside this checkout; keep compilers and SDKs outside Git")
    return report


def _inside(path, parent):
    try:
        Path(path).resolve().relative_to(Path(parent).resolve())
        return True
    except ValueError:
        return False


def runner_revision(readme=TOOLS_README):
    match = _RUNNER_REVISION.search(Path(readme).read_text(encoding="utf-8"))
    return match[1] if match else None


def check_runner(report, runner, readme=TOOLS_README):
    """Warn when the support checkout is not at the revision tools/README.md measured."""
    if runner is None:
        return
    if not (runner / "tools").is_dir():
        report.errors.append(f"--runner-root {runner} has no tools/ directory (Open-BFME-2 support tree)")
        return
    wanted = runner_revision(readme)
    try:
        actual = subprocess.run(["git", "-C", str(runner), "rev-parse", "HEAD"], capture_output=True,
                                text=True, check=True).stdout.strip()
    except (OSError, subprocess.CalledProcessError):
        actual = None
    if wanted and actual and actual != wanted:
        report.warnings.append(f"support checkout {runner} is at {actual}; tools/README.md measured {wanted}")


def config_path(environ=None, os_name=None, home=None):
    """Per-user configuration outside any checkout."""
    environ = os.environ if environ is None else environ
    os_name = os.name if os_name is None else os_name
    home = Path.home() if home is None else Path(home)
    if environ.get(CONFIG_ENV):
        return Path(environ[CONFIG_ENV])
    if os_name == "nt":
        base = Path(environ["LOCALAPPDATA"]) if environ.get("LOCALAPPDATA") else home / "AppData" / "Local"
    else:
        base = Path(environ["XDG_CONFIG_HOME"]) if environ.get("XDG_CONFIG_HOME") else home / ".config"
    return base / "bfme2-rebuild" / "toolchain.json"


def save_config(report, path, *, demangler=None, manifest=None, extra=None):
    path = Path(path)
    if _inside(path, REPO_ROOT):
        raise ValueError(f"refusing to write machine configuration inside the checkout: {path}")
    data = {"version": CONFIG_VERSION, "roots": report.roots.as_json(), "cl_sha256": report.cl_sha256,
            "manifest_sha256": manifest.digest if manifest else "", "demangler": demangler,
            "checked": report.checked, "warnings": report.warnings,
            "verified_at": time.strftime("%Y-%m-%dT%H:%M:%SZ", time.gmtime()), **(extra or {})}
    path.parent.mkdir(parents=True, exist_ok=True)
    temporary = path.with_name(path.name + ".tmp")
    temporary.write_text(json.dumps(data, indent=2, sort_keys=True) + "\n", encoding="utf-8")
    os.replace(temporary, path)
    return data


def load_config(path):
    data = json.loads(Path(path).read_text(encoding="utf-8"))
    if data.get("version") != CONFIG_VERSION or not data.get("roots", {}).get("vc71"):
        raise ValueError(f"{path} is not a toolchain configuration written by verify_toolchain.py")
    return data


def roots_from_config(data):
    return Roots(**{name: (Path(value) if value else None) for name, value in data["roots"].items()})


GUIDANCE = """
The local pre-check needs the retail Visual C++ .NET 2003 compiler (cl.exe 13.10.3077)
that the hosted Boat image uses. Point --vc71-root at the "Microsoft Visual Studio .NET 2003"
directory of a copy you are licensed to use (it contains Vc7/bin/cl.exe and Common7/IDE).
Do not download compilers from mirrors, and never commit them: keep them outside this checkout.
Every file must match docs/compiler-image.md byte for byte; a service pack or another build of
cl.exe compiles different bytes and makes the comparison meaningless.
""".strip()


def build_parser():
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--vc71-root", type=Path, help="Microsoft Visual Studio .NET 2003 directory")
    parser.add_argument("--tools-root", type=Path,
                        help="a directory laid out like tools/compiler-image/build.sh --inputs")
    parser.add_argument("--vcruntime-root", type=Path, help="directory with msvcp71.dll and msvcr71.dll")
    parser.add_argument("--psdk-root", type=Path, help="Platform SDK (default: <vc71-root>/Vc7/PlatformSDK)")
    parser.add_argument("--dxsdk-root", type=Path, help="DirectX SDK (August 2005) directory with Include/")
    parser.add_argument("--stlport-root", type=Path, help="pristine STLport 4.6 tree (stlport/ and src/)")
    parser.add_argument("--overlay-root", type=Path, help="approved STLport 4.6 overlay directory")
    parser.add_argument("--allocator-root", type=Path, help="approved allocator shim directory")
    parser.add_argument("--runner-root", type=Path, help="Open-BFME-2 toolchain support checkout")
    parser.add_argument("--demangler", type=Path, help="LLVM demangler built from tools/judge/demangle.cpp")
    parser.add_argument("--strict", action="store_true", help="fail on any header tree that differs")
    parser.add_argument("--no-save", action="store_true", help="verify only; do not write the configuration")
    parser.add_argument("--config", type=Path, help="configuration file (default: per-user location)")
    parser.add_argument("--json", action="store_true", help="print the report as JSON")
    return parser


def main(argv=None):
    args = build_parser().parse_args(argv)
    if args.vc71_root is None and args.tools_root is None:
        print("verify_toolchain.py: pass --vc71-root (or --tools-root).\n\n" + GUIDANCE, file=sys.stderr)
        return 2
    overrides = dict(vc71=args.vc71_root, vcruntime=args.vcruntime_root, psdk=args.psdk_root,
                     dxsdk=args.dxsdk_root, stlport46=args.stlport_root, overlay=args.overlay_root,
                     allocator=args.allocator_root, runner=args.runner_root)
    if args.tools_root is not None:
        roots = Roots.from_tools_root(args.tools_root, **overrides)
    else:
        roots = Roots(**overrides)
    try:
        manifest = load_manifest()
    except (OSError, ManifestError) as error:
        print(f"verify_toolchain.py: cannot read the Boat manifest: {error}", file=sys.stderr)
        return 1
    dxsdk_entries = parse_dxsdk_manifest(DXSDK_MANIFEST.read_text(encoding="utf-8")) if DXSDK_MANIFEST.is_file() else {}
    report = verify(roots, manifest, strict=args.strict, dxsdk_entries=dxsdk_entries)
    check_runner(report, report.roots.runner)
    if args.demangler is not None and not args.demangler.is_file():
        report.warnings.append(f"--demangler {args.demangler} does not exist yet (see tools/README.md)")
    target = args.config or config_path()
    saved = None
    if report.ok and not args.no_save:
        saved = save_config(report, target, demangler=str(args.demangler) if args.demangler else None,
                            manifest=manifest)
    if args.json:
        print(json.dumps({"ok": report.ok, "errors": report.errors, "warnings": report.warnings,
                          "checked": report.checked, "config": str(target) if saved else None}, indent=2))
    else:
        counts = {}
        for status in report.checked.values():
            counts[status] = counts.get(status, 0) + 1
        print("Checked against docs/compiler-image.md: " +
              ", ".join(f"{count} {status}" for status, count in sorted(counts.items())))
        for warning in report.warnings:
            print("WARNING: " + warning)
        for error in report.errors:
            print("ERROR: " + error)
        if report.ok:
            print(f"OK: cl.exe {report.cl_sha256} is the Boat compiler." +
                  (f" Saved {target}" if saved else ""))
        else:
            print("\n" + GUIDANCE)
    return 0 if report.ok else 1


if __name__ == "__main__":
    sys.exit(main())
