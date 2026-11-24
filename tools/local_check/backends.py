"""How the local pre-check runs the judge and the compiler on each host.

Judge backends (full Boat-equivalent comparison through tools/judge/bin/judge.py):
  host    Linux (or WSL itself) with Wine on PATH, the documented tools/README.md setup.
  wsl     Windows: the same judge inside WSL, reading this checkout through /mnt/<drive>.
  docker  Any host: tools/local_check/Dockerfile (Ubuntu 24.04 + the Boat Wine pin), with the
          verified VC7.1 root, the support checkout and the Git directory mounted read-only.

Compiler backends (compile-only smoke check, no byte comparison):
  native  Windows: cl.exe runs directly, no Wine.
  wine    Linux: wine cl.exe, the Boat command line.

The judge itself is POSIX-only (fcntl locks, /proc/meminfo, fork workers), so native
Windows runs compile only. Both compiler backends build their flags with the
judge's own rule functions, read from tools/judge/bin/judge.py (load_judge_rules),
so no compile rule is restated here.
"""
import ast
import contextlib
import os
import re
import shutil
import subprocess
import sys
import types
from pathlib import Path, PureWindowsPath

REPO_ROOT = Path(__file__).resolve().parents[2]
JUDGE = REPO_ROOT / "tools" / "judge" / "bin" / "judge.py"
JUDGE_RELATIVE = "tools/judge/bin/judge.py"
DEFAULT_IMAGE = "bfme2-local-check"
STATE_VOLUME = "bfme2-judge-state"

# The judge definitions the compile-only check executes, unchanged, from judge.py.
RULE_NAMES = ("BASE", "ALLOWED", "STL_DEFAULT_LINES", "STL_DEFAULT_EXEMPT",
              "compiler_base", "effective_source", "compile_settings")
sys.path[:0] = [str(REPO_ROOT / "tools" / "judge"), str(REPO_ROOT / "tools" / "judge" / "runner-tools")]
import compiler_support  # noqa: E402  (pure helpers shared with judge.py)
from cl_flags import OPTION_ENV  # noqa: E402  (variables cl.exe reads flags from; never inherited)


# ---------------------------------------------------------------------------
# Path conversion


def wsl_path(path):
    """C:\\a\\b -> /mnt/c/a/b; \\\\wsl$\\Distro\\x and \\\\wsl.localhost\\Distro\\x -> /x; POSIX unchanged."""
    text = str(path)
    if text.startswith("/"):
        return text
    windows = PureWindowsPath(text)
    if windows.drive.startswith("\\\\"):
        host = windows.drive.lstrip("\\").split("\\")[0].lower()
        if host in ("wsl$", "wsl.localhost"):
            return "/" + "/".join(windows.parts[1:])
        raise ValueError(f"{text}: network paths are not reachable from WSL")
    if not windows.drive:
        raise ValueError(f"{text}: expected an absolute Windows path")
    return "/mnt/" + windows.drive[0].lower() + "/" + "/".join(windows.parts[1:])


def wine_z_path(path):
    """The Z: spelling judge.py uses (its private prefix maps Z: to /)."""
    return "Z:" + str(Path(path)).replace("/", "\\")


# ---------------------------------------------------------------------------
# Judge backends


def judge_environment(config, *, repo_git_dir, retail, state=None, convert=str):
    """BFME_JUDGE_* settings for judge.py (tools/README.md), with paths spelled for the backend."""
    roots = config["roots"]
    env = {"BFME_JUDGE_REPO": convert(repo_git_dir), "BFME_JUDGE_RETAIL": convert(retail),
           "VC71_ROOT": convert(roots["vc71"]), "LANG": "C", "LC_ALL": "C"}
    if roots.get("runner"):
        env["BFME_JUDGE_RUNNER"] = convert(roots["runner"])
    if config.get("demangler"):
        env["BFME_JUDGE_DEMANGLER"] = config["demangler"]
    if state is not None:
        env["BFME_JUDGE_STATE"] = convert(state)
        env["BFME_JUDGE_WINEPREFIX"] = convert(Path(state) / "wine")
    return env


def judge_arguments(revision, tus):
    return ["--json", revision, *tus]


def host_judge_command(revision, tus, *, python=None, repo_root=REPO_ROOT):
    return [python or sys.executable, str(Path(repo_root) / JUDGE_RELATIVE), *judge_arguments(revision, tus)]


# Initialize the judge's Wine prefix (Z: -> /) on first use, then run judge.py.
_WSL_SCRIPT = ('p="${BFME_JUDGE_WINEPREFIX:-$HOME/.cache/bfme2-judge/wine}"; '
               '[ -e "$p/dosdevices/z:" ] || { mkdir -p "$p" && WINEPREFIX="$p" WINEDEBUG=-all wineboot -i '
               '>/dev/null 2>&1 && WINEPREFIX="$p" wineserver -w; }; exec python3 "$@"')


def wsl_judge_command(revision, tus, env, *, repo_root=REPO_ROOT, wsl="wsl.exe"):
    assignments = [f"{key}={value}" for key, value in sorted(env.items())]
    return [wsl, "-e", "env", *assignments, "sh", "-c", _WSL_SCRIPT, "sh",
            wsl_path(Path(repo_root) / JUDGE_RELATIVE), *judge_arguments(revision, tus)]


def docker_judge_command(revision, tus, config, *, repo_root, git_common_dir, image=DEFAULT_IMAGE,
                         docker="docker", state_volume=STATE_VOLUME):
    """Read-only mounts of the checkout, its Git objects, VC7.1 and the support tree; no network."""
    roots = config["roots"]

    def bind(source, target):
        return ["--mount", f"type=bind,source={source},target={target},readonly"]

    command = [docker, "run", "--rm", "--network", "none",
               *bind(repo_root, "/work"), *bind(git_common_dir, "/git"), *bind(roots["vc71"], "/opt/vc71"),
               "--mount", f"type=volume,source={state_volume},target=/state"]
    if roots.get("runner"):
        command += bind(roots["runner"], "/opt/runner")
    command += [image, *judge_arguments(revision, tus)]
    return command


def docker_build_command(*, image=DEFAULT_IMAGE, docker="docker", repo_root=REPO_ROOT):
    """The build context is tools/: the image needs only judge/demangle.cpp and the entry script."""
    tools = Path(repo_root) / "tools"
    return [docker, "build", "-f", str(tools / "local_check" / "Dockerfile"), "-t", image, str(tools)]


def default_backend(*, compile_only, os_name=None, which=shutil.which):
    os_name = os.name if os_name is None else os_name
    if os_name == "nt":
        if compile_only:
            return "native"
        if which("wsl.exe") or which("wsl"):
            return "wsl"
        return "docker"
    if compile_only:
        return "wine"
    return "host" if which("wine") else "docker"


# ---------------------------------------------------------------------------
# Compile-only: the judge's own flag rules, executed outside the judge


def load_judge_rules(namespace, judge_path=JUDGE):
    """Execute RULE_NAMES from judge.py's top level in NAMESPACE, which supplies their dependencies.

    judge.py cannot be imported on Windows (fcntl, fork), so only the rule definitions run,
    exactly as written there. A renamed or removed rule fails loudly instead of drifting."""
    source = Path(judge_path).read_text(encoding="utf-8")
    tree = ast.parse(source, str(judge_path))
    picked, found = [], set()
    for node in tree.body:
        if isinstance(node, ast.FunctionDef) and node.name in RULE_NAMES:
            picked.append(node)
            found.add(node.name)
        elif isinstance(node, ast.Assign):
            names = {target.id for target in node.targets if isinstance(target, ast.Name)}
            if names & set(RULE_NAMES):
                picked.append(node)
                found |= names & set(RULE_NAMES)
    missing = sorted(set(RULE_NAMES) - found)
    if missing:
        raise RuntimeError(f"{judge_path} no longer defines {', '.join(missing)}; update tools/local_check")
    exec(compile(ast.Module(body=picked, type_ignores=[]), str(judge_path), "exec"), namespace)
    return namespace


class RunnerView:
    """Stands in for judge.py's RUNNER: maps the support-tree paths compile_settings names."""

    def __init__(self, runner=None, *, stlport45=None, allocator=None):
        self.runner = Path(runner) if runner else None
        self.mapped = {"vendor/stlport": stlport45, "reference/shims/bfmealloc_alloconly": allocator}

    def __truediv__(self, name):
        mapped = self.mapped.get(str(name).replace("\\", "/"))
        if mapped is not None:
            return Path(mapped)
        if self.runner is None:
            raise ValueError(f"this unit needs {name} from the support checkout; pass --runner-root")
        return self.runner / name


class CompilerBackend:
    """Builds the judge's cl.exe command and environment for one host (native or wine)."""

    def __init__(self, kind, roots, *, overlay=None, stlport45=None, allocator=None, wine="wine",
                 wineprefix=None, judge_path=JUDGE):
        if kind not in ("native", "wine"):
            raise ValueError(f"unknown compiler backend {kind!r}")
        self.kind = kind
        self.roots = roots
        self.vc = Path(roots.vc71)
        self.overlay = Path(overlay) if overlay else None
        self.wine = wine
        self.wineprefix = wineprefix
        self.rules = load_judge_rules({
            "re": re, "Path": Path, "compiler_support": compiler_support, "VC": self.vc,
            "build": types.SimpleNamespace(compiler_environment=self.environment, wine_path=self.path,
                                           dxsdk_root=self._dxsdk_root),
            "wine_path": self.path, "PILOT": None, "stlport46_overlay": self._overlay,
            "RUNNER": RunnerView(roots.runner, stlport45=stlport45, allocator=allocator)}, judge_path)

    @property
    def cl(self):
        return self.vc / "Vc7" / "bin" / "cl.exe"

    def path(self, path):
        return str(Path(path)) if self.kind == "native" else wine_z_path(Path(path).resolve())

    def _dxsdk_root(self):
        if self.roots.dxsdk is None:
            raise ValueError("a // dxsdk unit needs --dxsdk-root (DirectX SDK, August 2005)")
        return Path(self.roots.dxsdk)

    def _overlay(self):
        if self.overlay is None:
            raise ValueError("a // stlport46 unit needs the approved overlay; pass --overlay-root "
                             "(the overlay/ directory of a compiler-image inputs tree)")
        return self.overlay

    def environment(self, root=None, source=None, base=None):
        """INCLUDE, LIB and the DLL search path of docs/compiler-image.md, for this host."""
        root = Path(root or self.vc)
        env = {key: value for key, value in (os.environ if base is None else base).items()
               if key.upper() not in OPTION_ENV}
        dlls = [root / "Vc7" / "bin", root / "Common7" / "IDE"]
        if self.roots.vcruntime:
            dlls.append(Path(self.roots.vcruntime))
        env["INCLUDE"] = self.path(root / "Vc7" / "include")
        env["LIB"] = self.path(root / "Vc7" / "lib")
        env["LANG"] = env["LC_ALL"] = "C"
        if self.kind == "native":
            env["PATH"] = os.pathsep.join([*map(str, dlls), env.get("PATH", "")])
        else:
            env["WINEPATH"] = ";".join(self.path(d) for d in dlls)
            env["WINEDEBUG"] = "-all"
            env["WINEDLLOVERRIDES"] = "mscoree,mshtml="
            if self.wineprefix:
                env["WINEPREFIX"] = str(self.wineprefix)
        return env

    @contextlib.contextmanager
    def _stlport_module(self):
        """compile_settings imports stlport_overlay for TREE; give it the verified STLport 4.6 tree."""
        module = types.ModuleType("stlport_overlay")
        module.TREE = Path(self.roots.stlport46) if self.roots.stlport46 else Path("stlport-4.6-not-configured")
        module.ROOT = None
        previous = sys.modules.get("stlport_overlay")
        sys.modules["stlport_overlay"] = module
        try:
            yield
        finally:
            if previous is None:
                sys.modules.pop("stlport_overlay", None)
            else:
                sys.modules["stlport_overlay"] = previous

    def command(self, tu, text, obj):
        """(argv, env) exactly as judge.compile_tu builds them, prefixed by wine on Linux.

        Raises ValueError for a refused `// cl:` flag, as the judge does."""
        rules = self.rules
        source = rules["effective_source"](tu, text)
        with self._stlport_module():
            flags, env, _roots = rules["compile_settings"](source)
        base = rules["compiler_base"](source)
        argv = [str(self.cl)] + base + ["-showIncludes"] + flags + ["-Iinclude", "-Fo" + self.path(obj),
                                                                 tu.replace("/", "\\")]
        if self.kind == "wine":
            argv = [self.wine] + argv
        return argv, env


def compile_unit(backend, job, tu, *, timeout=420, run=subprocess.run):
    """Compile TU inside an exported tree JOB. Returns (ok, diagnostic lines)."""
    job = Path(job)
    text = (job / tu).read_text(encoding="utf-8-sig", errors="replace")
    obj = job / "obj" / (tu.replace("/", "_") + ".obj")
    obj.parent.mkdir(parents=True, exist_ok=True)
    obj.unlink(missing_ok=True)
    try:
        argv, env = backend.command(tu, text, obj)
    except ValueError as error:
        return False, [str(error)]
    try:
        done = run(argv, cwd=job, env=env, stdin=subprocess.DEVNULL, stdout=subprocess.PIPE,
                   stderr=subprocess.STDOUT, timeout=timeout)
        output = (done.stdout or b"").decode("utf-8", "replace") if isinstance(done.stdout, bytes) else (done.stdout or "")
        code = done.returncode
    except subprocess.TimeoutExpired:
        return False, [f"cl.exe did not finish within {timeout} s"]
    except OSError as error:
        return False, [f"cannot start {argv[0]}: {error}"]
    if code == 0 and obj.exists():
        return True, []
    lines = [line for line in output.splitlines() if "error" in line.lower() or "warning C4" in line][:25]
    return False, lines or output.strip().splitlines()[-25:]
