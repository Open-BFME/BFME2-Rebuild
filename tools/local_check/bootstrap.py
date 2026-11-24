#!/usr/bin/env python3
"""Fetch, verify and configure the pinned local toolchain with one command.

usage:
  bootstrap.py [--cache-dir DIR] [--backend auto|host|wsl|docker|native|wine] [--config FILE]
               [--no-demangler] [--refresh] [--json]

What it does, idempotently (a second run only re-verifies):
  1. Fetches the inputs of the hosted compiler image from the organization's public repositories at
     pinned commits, with shallow, blob-filtered, sparse Git fetches (no credentials, no full clone):
       Open-BFME-1  inputs/toolchains/vs2003   Visual C++ .NET 2003 (cl.exe 13.10.3077), its runtime
                                               DLLs, CRT headers and libraries and the Platform SDK
       Open-BFME-2  tools/, vendor/stlport*, vendor/dxsdk-aug2005, reference/shims
                                               the judge's support tree (tools/README.md revision)
     Everything lands in the per-user cache (never inside this checkout).
  2. Materializes the approved STLport 4.6 overlay from the support tree's patch layer.
  3. Verifies every pinned digest with verify_toolchain.py (binaries, overlay and allocator files,
     subtree listings, DirectX headers) against the hosted image manifest.
  4. Builds the LLVM demangler from tools/judge/demangle.cpp for the host or WSL backend
     (the Docker image builds its own).
  5. Writes the toolchain.json that precheck.py reads.

Exit status: 0 ready, 1 verification failed, 2 setup problem (network, Git, disk).
"""
import argparse
import json
import os
import shutil
import subprocess
import sys
from dataclasses import dataclass
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO_ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
import backends  # noqa: E402
import verify_toolchain  # noqa: E402

CACHE_ENV = "BFME_TOOLCHAIN_CACHE"
VS2003 = "inputs/toolchains/vs2003"
VC71_RELATIVE = VS2003 + "/Program Files/Microsoft Visual Studio .NET 2003"
SUPPORT_REVISION = "39cdd2ca5c409c01b9c08d5dae7b3835c59a8b25"   # tools/README.md "Use revision"


@dataclass(frozen=True)
class Source:
    name: str
    url: str
    commit: str
    patterns: tuple      # non-cone sparse-checkout patterns, anchored at the repository root


# Only what the hosted image holds: Vc7/bin and Common7/IDE carry exactly the pinned binaries, and
# PlatformSDK only its Include/ tree, so each subtree listing digest can match the manifest.
COMPILER = Source(
    "open-bfme-1", "https://github.com/Open-BFME/Open-BFME-1.git", "7a5e7bca5ae0fec82286704f8bacb34ec1b26870",
    tuple(f"/{VC71_RELATIVE}/{name}" for name in (
        "Vc7/bin/cl.exe", "Vc7/bin/c1.dll", "Vc7/bin/c1xx.dll", "Vc7/bin/c2.dll",
        "Common7/IDE/mspdb71.dll", "Common7/IDE/msvcr71.dll",
        "Vc7/include/", "Vc7/lib/", "Vc7/PlatformSDK/Include/")) +
    (f"/{VS2003}/msvcp71.dll", f"/{VS2003}/msvcr71.dll"))

SUPPORT = Source(
    "open-bfme-2", "https://github.com/Open-BFME/Open-BFME-2.git", SUPPORT_REVISION,
    ("/.gitmodules", "/tools/", "/vendor/stlport/", "/vendor/stlport-4.6/", "/vendor/stlport-4.6-bfme/",
     "/vendor/dxsdk-aug2005/", "/reference/shims/"))

SOURCES = (COMPILER, SUPPORT)
STAMP = ".bfme2-bootstrap.json"

# Git settings that keep the fetched bytes exact and the fetch non-interactive on every host.
GIT_CONFIG = ("-c", "core.autocrlf=false", "-c", "core.eol=lf", "-c", "core.longpaths=true",
              "-c", "core.symlinks=false", "-c", "credential.helper=", "-c", "advice.detachedHead=false")
GIT_ENV = {"GIT_TERMINAL_PROMPT": "0", "GIT_ASKPASS": "", "GCM_INTERACTIVE": "never", "GIT_LFS_SKIP_SMUDGE": "1"}

DEMANGLER_BUILD = ('set -e; out="$1"; mkdir -p "$(dirname "$out")"; '
                   'cfg=$(command -v llvm-config-18 || command -v llvm-config || true); '
                   '[ -n "$cfg" ] || { echo "no llvm-config-18 (install llvm-18-dev)" >&2; exit 3; }; '
                   'c++ -std=c++17 -O2 "$2" $($cfg --ldflags --libs --system-libs) -o "$out"; '
                   'printf "_ZN1a1bEv\\n?f@@YAXXZ\\n" | "$out" >/dev/null; echo "$out"')
INSTALL_HINT = {"host": "sudo apt-get install g++ llvm-18-dev   (macOS: brew install llvm@18, then put its bin on PATH)",
                "wsl": "wsl -e sudo apt-get install g++ llvm-18-dev"}


class BootstrapError(RuntimeError):
    """The toolchain could not be fetched or prepared (exit status 2)."""


def cache_root(environ=None, os_name=None, platform=None, home=None):
    """Per-user cache directory, outside any checkout."""
    environ = os.environ if environ is None else environ
    os_name = os.name if os_name is None else os_name
    platform = sys.platform if platform is None else platform
    home = Path.home() if home is None else Path(home)
    if environ.get(CACHE_ENV):
        return Path(environ[CACHE_ENV])
    if os_name == "nt":
        base = Path(environ["LOCALAPPDATA"]) if environ.get("LOCALAPPDATA") else home / "AppData" / "Local"
        return base / "bfme2-rebuild" / "cache"
    if platform == "darwin":
        return home / "Library" / "Caches" / "bfme2-rebuild"
    base = Path(environ["XDG_CACHE_HOME"]) if environ.get("XDG_CACHE_HOME") else home / ".cache"
    return base / "bfme2-rebuild"


def git_command(destination, *args):
    return ["git", *GIT_CONFIG, "-C", str(destination), *args]


def fetch_commands(source, destination):
    """The Git commands that fetch SOURCE at its pinned commit into DESTINATION (shallow, sparse)."""
    return [
        ["git", "init", "-q", str(destination)],
        git_command(destination, "config", "core.autocrlf", "false"),
        git_command(destination, "config", "core.longpaths", "true"),
        git_command(destination, "remote", "remove", "origin"),
        git_command(destination, "remote", "add", "origin", source.url),
        git_command(destination, "sparse-checkout", "set", "--no-cone", "--stdin"),
        git_command(destination, "fetch", "--quiet", "--depth", "1", "--filter=blob:none", "--no-tags", "origin",
                    source.commit),
        git_command(destination, "checkout", "--quiet", "--force", "--detach", source.commit),
    ]


def stamp(destination):
    try:
        return json.loads((Path(destination) / STAMP).read_text(encoding="utf-8"))
    except (OSError, ValueError):
        return {}


def fetch(source, destination, *, run=subprocess.run, refresh=False, log=print):
    """Fetch SOURCE once; later calls return at once while the stamp and HEAD agree."""
    destination = Path(destination)
    if not refresh and stamp(destination).get("commit") == source.commit and stamp(destination).get(
            "patterns") == list(source.patterns):
        head = run(git_command(destination, "rev-parse", "HEAD"), capture_output=True, text=True)
        if head.returncode == 0 and head.stdout.strip() == source.commit:
            return False
    if shutil.which("git") is None and run is subprocess.run:
        raise BootstrapError("git is not on PATH")
    destination.mkdir(parents=True, exist_ok=True)
    log(f"Fetching {source.url} at {source.commit[:12]} (sparse, shallow) into {destination} ...")
    env = {**os.environ, **GIT_ENV}
    for command in fetch_commands(source, destination):
        stdin = "\n".join(source.patterns) + "\n" if "--stdin" in command else None
        done = run(command, input=stdin, capture_output=True, text=True, env=env)
        if done.returncode and command[-2:] != ["remove", "origin"]:
            detail = (done.stderr or done.stdout or "").strip().splitlines()[-5:]
            raise BootstrapError(f"{' '.join(command[len(GIT_CONFIG) + 1:])} failed:\n  " + "\n  ".join(detail))
    (destination / STAMP).write_text(json.dumps({"url": source.url, "commit": source.commit,
                                                 "patterns": list(source.patterns)}, indent=2) + "\n",
                                     encoding="utf-8")
    return True


@dataclass
class Layout:
    cache: Path

    @property
    def compiler_checkout(self):
        return self.cache / COMPILER.name

    @property
    def runner(self):
        return self.cache / SUPPORT.name

    @property
    def vc71(self):
        return self.compiler_checkout / Path(*VC71_RELATIVE.split("/"))

    @property
    def vcruntime(self):
        return self.compiler_checkout / Path(*VS2003.split("/"))

    @property
    def allocator(self):
        return self.runner / "reference" / "shims" / "bfmealloc_alloconly"

    @property
    def demangler(self):
        return self.cache / "bin" / ("demangle.exe" if os.name == "nt" else "demangle")


def materialize_overlay(runner, *, run=subprocess.run):
    """Apply the support tree's STLport 4.6 patch layer with the judge's own code; returns the overlay root."""
    paths = [REPO_ROOT / "tools" / "judge" / "runner-tools", REPO_ROOT / "tools" / "judge", Path(runner) / "tools"]
    code = ("import sys; sys.path[:0] = sys.argv[1].split('\\n'); "
            "import stlport_overlay; print(stlport_overlay.materialize())")
    env = {**os.environ, "BFME_JUDGE_RUNNER": str(runner), "PYTHONDONTWRITEBYTECODE": "1"}
    done = run([sys.executable, "-I", "-c", code, "\n".join(map(str, paths))], capture_output=True, text=True,
               env=env)
    if done.returncode:
        raise BootstrapError("STLport 4.6 overlay did not materialize:\n  " +
                             "\n  ".join((done.stderr or "").strip().splitlines()[-5:]))
    return Path(done.stdout.strip().splitlines()[-1])


def build_demangler(backend, layout, *, run=subprocess.run):
    """Build tools/judge/demangle.cpp where the judge runs. Returns (path as the judge sees it, note)."""
    source = REPO_ROOT / "tools" / "judge" / "demangle.cpp"
    if backend in ("docker", "native", "wine"):
        return None, ("the Docker image builds its own demangler" if backend == "docker"
                      else "compile-only backends need no demangler")
    if backend == "wsl":
        argv = ["wsl.exe", "-e", "sh", "-c", DEMANGLER_BUILD.replace('out="$1"', 'out="$HOME/.cache/bfme2-rebuild/'
                                                                     'bin/demangle"'),
                "sh", "unused", backends.wsl_path(source)]
    else:
        argv = ["sh", "-c", DEMANGLER_BUILD, "sh", str(layout.cache / "bin" / "demangle"), str(source)]
    try:
        done = run(argv, capture_output=True, text=True)
    except OSError as error:
        return None, f"cannot build the demangler: {error}"
    if done.returncode:
        reason = (done.stderr or done.stdout or "").strip().splitlines()[-1:] or ["failed"]
        return None, f"demangler not built ({reason[0]}); install: {INSTALL_HINT.get(backend, INSTALL_HINT['host'])}"
    return done.stdout.strip().splitlines()[-1], "built"


@dataclass
class Result:
    ok: bool
    config: Path
    report: object
    demangler: str
    notes: list


def run_bootstrap(*, cache=None, backend="auto", config_file=None, demangler=True, refresh=False,
                  run=subprocess.run, log=print, fetcher=fetch, overlay=materialize_overlay,
                  demangler_builder=build_demangler):
    cache = Path(cache) if cache else cache_root()
    if verify_toolchain._inside(cache, REPO_ROOT):
        raise BootstrapError(f"{cache} is inside this checkout; choose a cache outside Git")
    layout = Layout(cache.expanduser().resolve())
    for source, destination in ((COMPILER, layout.compiler_checkout), (SUPPORT, layout.runner)):
        fetcher(source, destination, run=run, refresh=refresh, log=log)
    notes = []
    overlay_root = overlay(layout.runner, run=run)
    roots = verify_toolchain.Roots(vc71=layout.vc71, vcruntime=layout.vcruntime, runner=layout.runner,
                                   overlay=overlay_root, allocator=layout.allocator)
    manifest = verify_toolchain.load_manifest()
    dx = verify_toolchain.parse_dxsdk_manifest(verify_toolchain.DXSDK_MANIFEST.read_text(encoding="utf-8")) \
        if verify_toolchain.DXSDK_MANIFEST.is_file() else {}
    log("Verifying every pinned digest against the hosted image manifest ...")
    report = verify_toolchain.verify(roots, manifest, strict=True, dxsdk_entries=dx)
    verify_toolchain.check_runner(report, report.roots.runner)
    if backend == "auto":
        backend = backends.default_backend(compile_only=False)
    built = None
    if demangler and report.ok:
        built, note = demangler_builder(backend, layout, run=run)
        notes.append(f"demangler ({backend}): {note}")
    target = Path(config_file) if config_file else verify_toolchain.config_path()
    if report.ok:
        verify_toolchain.save_config(report, target, demangler=built, manifest=manifest, extra={
            "bootstrap": {"cache": str(layout.cache), "backend": backend,
                          "sources": {s.name: {"url": s.url, "commit": s.commit} for s in SOURCES}}})
    return Result(report.ok, target, report, built, notes)


def summarize(result, log=print):
    counts = {}
    for status in result.report.checked.values():
        counts[status] = counts.get(status, 0) + 1
    log("Checked: " + ", ".join(f"{count} {status}" for status, count in sorted(counts.items())))
    for warning in result.report.warnings:
        log("WARNING: " + warning)
    for error in result.report.errors:
        log("ERROR: " + error)
    for note in result.notes:
        log("NOTE: " + note)
    if result.ok:
        log(f"READY: cl.exe {result.report.cl_sha256[:16]}... is the Boat compiler; wrote {result.config}")


def build_parser():
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--cache-dir", type=Path, help=f"where the toolchain goes (default: per-user cache, "
                                                       f"or ${CACHE_ENV})")
    parser.add_argument("--backend", default="auto", choices=("auto", "host", "wsl", "docker", "native", "wine"),
                        help="the backend the demangler is built for")
    parser.add_argument("--config", type=Path, help="toolchain configuration to write (default: per-user location)")
    parser.add_argument("--no-demangler", action="store_true", help="do not build the demangler")
    parser.add_argument("--refresh", action="store_true", help="fetch again even when the cache is current")
    parser.add_argument("--json", action="store_true", help="print the result as JSON")
    return parser


def main(argv=None):
    args = build_parser().parse_args(argv)
    try:
        result = run_bootstrap(cache=args.cache_dir, backend=args.backend, config_file=args.config,
                               demangler=not args.no_demangler, refresh=args.refresh,
                               log=(lambda *_: None) if args.json else print)
    except (BootstrapError, OSError, verify_toolchain.ManifestError) as error:
        print(f"bootstrap: {error}", file=sys.stderr)
        return 2
    if args.json:
        print(json.dumps({"ok": result.ok, "config": str(result.config), "errors": result.report.errors,
                          "warnings": result.report.warnings, "checked": result.report.checked,
                          "demangler": result.demangler, "notes": result.notes}, indent=2))
    else:
        summarize(result)
    return 0 if result.ok else 1


if __name__ == "__main__":
    sys.exit(main())
