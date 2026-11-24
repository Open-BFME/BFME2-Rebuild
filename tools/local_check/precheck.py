#!/usr/bin/env python3
"""One-command local pull-request pre-flight: source gate, affected units, judge, regression diff.

usage:
  precheck.py [--bootstrap] [--base origin/main] [--head HEAD] [--backend auto|host|wsl|docker|native|wine]
              [--compile-only] [--skip-gate] [--image NAME] [--build-image] [--config FILE] [--json FILE]
              [--view [--port 8080]]
  precheck.py [--bootstrap] --unit src/...cpp [--unit ...] [--worktree] [--objdiff DIR] [--view]

--bootstrap fetches and verifies the pinned toolchain first (tools/local_check/bootstrap.py).
--unit compiles and judges the named units on HEAD (or, with --worktree, on the uncommitted working
tree) without a base comparison, writes an objdiff project for each, and can be repeated without limit.
--view serves a live page (tools/local_check/viewer.py) at http://localhost:8080.

Steps, mirroring the hosted BFME II / Boat check:
  1. tools/gates/gate.py on merge-base(base, head)..head; any G1-G8 / H1-H10 failure stops here.
  2. The affected translation units: the changed sources plus every includer, computed with
     ci/managed-check/intake.py's own include-closure and refusal rules.
  3. tools/judge/bin/judge.py on the base and the head for those units (backend host, wsl or docker),
     compared with ci/managed-check/compare.py's own regression reduction.
     --compile-only instead compiles each unit with cl.exe (native on Windows, wine on Linux) and
     reports only compile errors; it cannot prove a MATCH.

Exit status: 0 no regression, 1 gate failure, refusal or regression, 2 setup problem.
A local run supplies no GitHub status and no landing approval; the hosted check stays authoritative.
"""
import argparse
import io
import json
import os
import shutil
import subprocess
import sys
import tarfile
import tempfile
import time
from dataclasses import dataclass, field
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO_ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
import backends  # noqa: E402
import verify_toolchain  # noqa: E402

CI_PATHS = [REPO_ROOT / "ci" / "managed-check", REPO_ROOT / "ci" / "managed-nt2", REPO_ROOT / "tools" / "gates"]
NARROW_UNITS = 8
LEDGER_FILES = ("map.json", "header_manifest.json", "symbols.json")
HOST_ENV = ("PATH", "HOME", "USER", "LOGNAME", "TMPDIR", "DISPLAY", "XDG_RUNTIME_DIR", "JUDGE_WORKERS", "JUDGE_CACHE")


class SetupError(RuntimeError):
    """The local environment cannot run the check (exit status 2)."""


def ci_modules():
    """Import the hosted check's intake and comparison modules from this checkout."""
    for path in reversed(CI_PATHS):
        if str(path) not in sys.path:
            sys.path.insert(0, str(path))
    import compare  # noqa: E402
    import gitrepo  # noqa: E402
    import intake  # noqa: E402
    import pool_compare  # noqa: E402
    import pool_protocol  # noqa: E402
    return intake, compare, gitrepo, pool_compare, pool_protocol


def git(repo, *args, check=True):
    done = subprocess.run(["git", "-C", str(repo), *args], capture_output=True, text=True)
    if check and done.returncode:
        raise SetupError(f"git {' '.join(args)}: {done.stderr.strip()}")
    return done.stdout.strip()


def git_common_dir(repo):
    path = Path(git(repo, "rev-parse", "--git-common-dir"))
    return path if path.is_absolute() else (Path(repo) / path).resolve()


# ---------------------------------------------------------------------------
# 1. Source gate


def run_gate(repo, base, head, *, run=subprocess.run):
    """Run tools/gates/gate.py. Returns (status, markdown report)."""
    with tempfile.TemporaryDirectory(prefix="bfme2-gate-") as scratch:
        result_path, report_path = Path(scratch) / "gate.json", Path(scratch) / "gate.md"
        run([sys.executable, str(REPO_ROOT / "tools" / "gates" / "gate.py"), str(repo), base, head,
             "--json", str(result_path), "--report", str(report_path)], capture_output=True, text=True)
        try:
            status = json.loads(result_path.read_text(encoding="utf-8"))["status"]
            report = report_path.read_text(encoding="utf-8")
        except (OSError, ValueError, KeyError):
            return "error", "The gate wrote no result."
    return status, report


# ---------------------------------------------------------------------------
# 2. Affected units (intake.py rules)


@dataclass
class Plan:
    base: str
    head: str
    changed: list = field(default_factory=list)
    affected: list = field(default_factory=list)
    refusals: list = field(default_factory=list)   # (rule, location)

    @property
    def route(self):
        return "compile" if len(self.affected) <= NARROW_UNITS else "wide"


def plan_changes(repo, base, head, *, modules=None):
    """The source delta and its affected units, refused where Boat's intake would refuse."""
    intake, _compare, gitrepo, pool_compare, pool_protocol = modules or ci_modules()
    repository = gitrepo.Git(git_common_dir(repo))
    plan = Plan(base, head)
    status = [part for part in git(repo, "diff", "--no-renames", "--name-status", "-z", base, head).split("\0") if part]
    for kind, path in zip(status[0::2], status[1::2]):
        if not gitrepo.safe_path(path):
            continue
        plan.changed.append(path)
        if intake.tu(path):
            if not exists(repo, base, "expected/" + path[4:] + ".json"):
                plan.refusals.append(("new_tu", path))
            if kind == "D":
                plan.refusals.append(("deletion", path))
        if kind != "D":
            data = repository.run("cat-file", "blob", f"{head}:{path}").stdout
            issue = pool_protocol.compiler_source_problem(data)
            if issue:
                plan.refusals.append((issue[0], f"{path}:{issue[1]}"))
    plan.changed.sort()
    if not plan.changed:
        return plan
    old_files, _ = pool_compare.projection(repository, base)
    new_files, _ = pool_compare.projection(repository, head)
    cache = {}
    affected = intake.affected(old_files, plan.changed, include_cache=cache)
    affected |= intake.affected(new_files, plan.changed, include_cache=cache)
    plan.affected = sorted(affected)
    if not affected:
        plan.refusals.append(("no-includers", None))
    elif len(affected) > intake.MAX_AFFECTED:
        plan.refusals.append(("wide-cap", f"{len(affected)} units"))
    for path in plan.affected:
        if not (exists(repo, base, "expected/" + path[4:] + ".json") and exists(repo, base, path)
                and exists(repo, head, path)) and ("new_tu", path) not in plan.refusals:
            plan.refusals.append(("new_tu", path))
    return plan


def exists(repo, revision, path):
    return subprocess.run(["git", "-C", str(repo), "cat-file", "-e", f"{revision}:{path}"],
                          capture_output=True).returncode == 0


def check_local_main(repo, base):
    """judge.py compares each revision's ledger with the local `main`; it must be the upstream one."""
    stale = []
    for name in LEDGER_FILES:
        mine = git(repo, "rev-parse", "--verify", "--quiet", f"main:config/{name}", check=False)
        theirs = git(repo, "rev-parse", "--verify", "--quiet", f"{base}:config/{name}", check=False)
        if mine != theirs:
            stale.append(name)
    if stale:
        raise SetupError("local main has a different ledger (" + ", ".join(stale) + ") than the base; "
                         "run: git fetch origin && git branch -f main origin/main")


# ---------------------------------------------------------------------------
# 3a. Judge on base and head


def parse_judge_output(stdout):
    for line in reversed(stdout.splitlines()):
        line = line.strip()
        if line.startswith("{"):
            data = json.loads(line)
            if "results" in data:
                return data
    raise SetupError("judge.py printed no JSON result")


class JudgeRunner:
    """Runs judge.py through one backend and returns its parsed --json document."""

    def __init__(self, backend, config, repo, *, image=backends.DEFAULT_IMAGE, run=subprocess.run, git_dir=None,
                 mounts=(), engine=None):
        self.backend, self.config, self.repo, self.image, self.run = backend, config, Path(repo), image, run
        self.git_dir = Path(git_dir) if git_dir else git_common_dir(repo)
        self.retail = self.repo / "baseline" / "game.dat"
        self.mounts = tuple(mounts)
        self.engine = engine or backends.container_engine()

    def command(self, revision, tus, extra=()):
        if self.backend == "host":
            return backends.host_judge_command(revision, tus, repo_root=self.repo, extra=extra), self._host_env()
        if self.backend == "wsl":
            env = backends.judge_environment(self.config, repo_git_dir=self.git_dir, retail=self.retail,
                                             convert=backends.wsl_path)
            return backends.wsl_judge_command(revision, tus, env, repo_root=self.repo, extra=extra), None
        if self.backend == "docker":
            return backends.docker_judge_command(revision, tus, self.config, repo_root=self.repo,
                                                 git_common_dir=self.git_dir, image=self.image, extra=extra,
                                                 docker=self.engine, mounts=self.mounts), None
        raise SetupError(f"backend {self.backend!r} cannot run the judge")

    def diff(self, revision, tu, rva):
        """judge.py --diff: the judge's own unified disassembly diff of one function (retail vs compiled)."""
        argv, env = self.command(revision, [tu], extra=("--diff", rva))
        done = self.run(argv, env=env, capture_output=True, text=True, cwd=str(self.repo))
        if done.returncode:
            raise SetupError("judge.py --diff failed: " + (done.stderr or done.stdout or "").strip()[-400:])
        return done.stdout

    def _host_env(self):
        """A minimal environment, so no remote-dispatch setting of the caller reaches the judge."""
        state = Path(os.environ.get("BFME_JUDGE_STATE", Path.home() / ".cache" / "bfme2-judge"))
        env = {key: os.environ[key] for key in HOST_ENV if key in os.environ}
        env.update(backends.judge_environment(self.config, repo_git_dir=self.git_dir, retail=self.retail,
                                              state=state))
        prefix = Path(env["BFME_JUDGE_WINEPREFIX"])
        if not (prefix / "dosdevices" / "z:").exists():
            prefix.mkdir(parents=True, exist_ok=True)
            self.run(["wineboot", "-i"], env={**env, "WINEPREFIX": str(prefix), "WINEDEBUG": "-all"},
                     capture_output=True)
            self.run(["wineserver", "-w"], env={**env, "WINEPREFIX": str(prefix)}, capture_output=True)
        return env

    def __call__(self, revision, tus):
        argv, env = self.command(revision, tus)
        done = self.run(argv, env=env, capture_output=True, text=True, cwd=str(self.repo))
        if done.returncode:
            tail = "\n".join((done.stderr or done.stdout or "").strip().splitlines()[-15:])
            raise SetupError(f"judge.py exited {done.returncode} on {revision[:12]}:\n{tail}")
        return parse_judge_output(done.stdout)


def results_table(document):
    table = {}
    for item in document["results"]:
        item.setdefault("expected", [])
        item.setdefault("verification_blockers", [])
        item.setdefault("gates", [])
        item.setdefault("rows", [])
        table[item["tu"]] = item
    return table


def label(before, after, error):
    if error or after == "COMPILE_ERROR":
        return "COMPILE ERROR"
    if after == "MATCH":
        return "PASS" if before == "MATCH" else "PASS (gained)"
    return "MISMATCH (lost MATCH)" if before == "MATCH" else "MISMATCH"


def function_rows(old, new, changed):
    """Per-function lines: every row of a changed unit, and changed rows of includer-only units."""
    lines = []
    for tu in sorted(new):
        before = {row["rva"]: row for row in old.get(tu, {}).get("rows", [])}
        for row in new[tu]["rows"]:
            prior = before.get(row["rva"], {}).get("status", "MISSING")
            if tu not in changed and prior == row["status"]:
                continue
            name = row.get("name") or row.get("symbol") or ""
            lines.append({"tu": tu, "rva": row["rva"], "name": name, "before": prior, "after": row["status"],
                          "label": label(prior, row["status"], new[tu].get("error"))})
        for rva in sorted(set(before) - {row["rva"] for row in new[tu]["rows"]}):
            lines.append({"tu": tu, "rva": rva, "name": before[rva].get("name", ""), "before": before[rva]["status"],
                          "after": "absent", "label": "MISMATCH (row removed)"})
    return lines


def judge_verdict(old_doc, new_doc, changed, *, modules=None):
    _intake, compare, *_ = modules or ci_modules()
    old, new = results_table(old_doc), results_table(new_doc)
    missing = sorted(set(old) ^ set(new))
    if missing:
        raise SetupError("judge results cover different units on base and head: " + ", ".join(missing))
    counts, rules, hints = compare.reduction(compare.index(old), compare.index(new))
    return {"counts": counts, "rules": sorted(rules), "hints": list(dict.fromkeys(hints))[:8],
            "functions": function_rows(old, new, set(changed)),
            "errors": {tu: item["error"] for tu, item in new.items() if item.get("error")}}


# ---------------------------------------------------------------------------
# 3b. Compile only


def export_sources(repo, revision, destination):
    data = subprocess.run(["git", "-C", str(repo), "archive", "--format=tar", revision, "--", "src", "include"],
                          capture_output=True, check=True).stdout
    with tarfile.open(fileobj=io.BytesIO(data)) as archive:
        archive.extractall(destination, filter="data")


def compile_verdict(backend, repo, plan, *, compile_unit=backends.compile_unit, export=export_sources):
    outcome = {}
    with tempfile.TemporaryDirectory(prefix="bfme2-precheck-") as scratch:
        for side, revision in (("base", plan.base), ("head", plan.head)):
            job = Path(scratch) / side
            export(repo, revision, job)
            for tu in plan.affected:
                outcome.setdefault(tu, {})[side] = compile_unit(backend, job, tu)
    regressions = sorted(tu for tu, sides in outcome.items() if sides["base"][0] and not sides["head"][0])
    return {"units": {tu: {"base": "OK" if s["base"][0] else "COMPILE ERROR",
                           "head": "OK" if s["head"][0] else "COMPILE ERROR",
                           "diagnostics": s["head"][1]} for tu, s in sorted(outcome.items())},
            "rules": ["compile"] if regressions else [], "regressions": regressions}


# ---------------------------------------------------------------------------
# Orchestration


def load_toolchain(config_file):
    """The saved configuration, with cl.exe and its DLLs re-verified against the manifest."""
    path = Path(config_file) if config_file else verify_toolchain.config_path()
    if not path.is_file():
        raise SetupError(f"no toolchain configuration at {path}. Add --bootstrap (fetches and verifies the "
                         "pinned toolchain), or run: python tools/local_check/bootstrap.py")
    config = verify_toolchain.load_config(path)
    report = verify_toolchain.verify(verify_toolchain.roots_from_config(config), verify_toolchain.load_manifest(),
                                     binaries_only=True)
    if not report.ok:
        raise SetupError("the configured compiler no longer matches docs/compiler-image.md:\n  " +
                         "\n  ".join(report.errors))
    return config


def compiler_backend(kind, config):
    roots = verify_toolchain.roots_from_config(config).resolved()
    runner = roots.runner
    overlay = roots.overlay
    stlport45 = runner / "vendor" / "stlport" if runner else None
    allocator = roots.allocator or (runner / "reference" / "shims" / "bfmealloc_alloconly" if runner else None)
    prefix = Path(os.environ.get("BFME_JUDGE_WINEPREFIX", Path.home() / ".cache" / "bfme2-judge" / "wine"))
    return backends.CompilerBackend(kind, roots, overlay=overlay, stlport45=stlport45, allocator=allocator,
                                    wineprefix=prefix if kind == "wine" else None)


def ensure_image(image, *, run=subprocess.run, build=False, engine=None):
    """Build the image when it is missing, older than its inputs (label digest) or --build-image is given."""
    engine = engine or backends.container_engine()
    try:
        done = run([engine, "image", "inspect", "--format",
                    "{{ index .Config.Labels \"" + backends.IMAGE_LABEL + "\" }}", image],
                   capture_output=True, text=True)
    except OSError:
        raise SetupError(f"{engine} is not installed; install Docker or Podman, or use --backend wsl/host") from None
    current = done.returncode == 0 and (done.stdout or "").strip() == backends.image_digest()
    if current and not build:
        return
    print(f"Building {image} with {engine} from tools/local_check/Dockerfile (no Microsoft file enters it)...")
    if run(backends.docker_build_command(image=image, docker=engine)).returncode:
        raise SetupError(f"{engine} build failed")


def ensure_toolchain(args, *, toolchain, bootstrapper=None, log=print):
    """--bootstrap: fetch and verify the pinned toolchain first (quick when the cache is current)."""
    if args.bootstrap:
        import bootstrap
        result = (bootstrapper or bootstrap.run_bootstrap)(backend=args.backend, config_file=args.config, log=log)
        if not result.ok:
            raise SetupError("toolchain verification failed:\n  " + "\n  ".join(result.report.errors))
        for note in result.notes:
            log("Bootstrap: " + note)
    return toolchain(args.config)


# ---------------------------------------------------------------------------
# Single-unit rounds (--unit): HEAD or the working tree, no base comparison


def snapshot_worktree(repo, paths=("src", "include")):
    """A commit object of HEAD plus the uncommitted changes under PATHS; no ref, index or file changes."""
    head = git(repo, "rev-parse", "HEAD")
    paths = [path for path in paths if (Path(repo) / path).exists()]
    with tempfile.TemporaryDirectory(prefix="bfme2-index-") as scratch:
        env = {**os.environ, "GIT_INDEX_FILE": str(Path(scratch) / "index")}
        for argv in (["read-tree", head], ["add", "-A", "--", *paths]):
            done = subprocess.run(["git", "-C", str(repo), *argv], env=env, capture_output=True, text=True)
            if done.returncode:
                raise SetupError(f"git {' '.join(argv)}: {done.stderr.strip()}")
        tree = subprocess.run(["git", "-C", str(repo), "write-tree"], env=env, capture_output=True, text=True,
                              check=True).stdout.strip()
    if tree == git(repo, "rev-parse", "HEAD^{tree}"):
        return head, False
    message = "precheck: working-tree snapshot (local only, never pushed)"
    commit = subprocess.run(["git", "-C", str(repo), "commit-tree", tree, "-p", head, "-m", message],
                            capture_output=True, text=True, env={**os.environ, **SNAPSHOT_IDENTITY})
    if commit.returncode:
        raise SetupError("git commit-tree: " + commit.stderr.strip())
    return commit.stdout.strip(), True


SNAPSHOT_IDENTITY = {"GIT_AUTHOR_NAME": "precheck", "GIT_AUTHOR_EMAIL": "precheck@localhost",
                     "GIT_COMMITTER_NAME": "precheck", "GIT_COMMITTER_EMAIL": "precheck@localhost"}


def unit_rows(document, repo, revision):
    """Per-function lines of a --unit judge run: every row, labelled as the hosted report labels a new MATCH."""
    lines = []
    for tu, item in sorted(results_table(document).items()):
        names = ledger_names(repo, revision, tu)
        for row in item["rows"]:
            status = row["status"]
            name = row.get("symbol") or names.get(int(row["rva"], 16)) or row.get("old") or ""
            lines.append({"tu": tu, "rva": row["rva"], "name": name, "symbol": row.get("symbol"),
                          "size": row.get("size") or row.get("code_size"), "after": status, "pct": row.get("pct"),
                          "proof": row.get("proof"), "label": unit_label(status, item.get("error"), row)})
    return lines


def unit_label(status, error, row):
    if error or status == "COMPILE_ERROR":
        return "COMPILE ERROR"
    if status == "MATCH" and row.get("credited", True):
        return "PASS" if row.get("proof", "proven") == "proven" else "PASS (unproven)"
    return "MISMATCH" + (f" {row['pct']:.0f}%" if isinstance(row.get("pct"), (int, float)) and status == "PARTIAL"
                         else f" ({status})" if status != "MATCH" else " (not credited)")


def ledger_names(repo, revision, tu):
    try:
        text = git(repo, "show", f"{revision}:expected/{tu[4:]}.json")
        return {int(row["rva"], 16): row.get("name", "") for row in json.loads(text).get("rows", [])}
    except (SetupError, ValueError, KeyError):
        return {}


ROUNDS_NOTE = ("Local rounds are unlimited: edit, re-run, repeat. The hosted BFME II / Boat check allows "
               "3 rounds per pull request, so spend them only once this passes.")


def print_report(plan, verdict, compile_only):
    print(f"Units: {len(plan.affected)} affected by {len(plan.changed)} changed source file(s); "
          f"Boat route: {plan.route}")
    if compile_only:
        for tu, unit in verdict["units"].items():
            print(f"  {unit['head']:<13} {tu} (base {unit['base']})")
            if unit["head"] != "OK":
                for line in unit["diagnostics"][:10]:
                    print("      " + line)
        print("Compile-only: byte matches are not compared; run a judge backend before relying on a MATCH.")
    else:
        for row in verdict["functions"]:
            print(f"  {row['label']:<22} {row['tu']} {row['rva']} {row['name']} ({row['before']} -> {row['after']})")
        for tu, error in verdict["errors"].items():
            print(f"  COMPILE ERROR {tu}:\n      " + "\n      ".join(str(error).splitlines()[:10]))
        counts = verdict["counts"]
        print("Gained %(gained)d; lost credit %(lost_credit)d, proof %(lost_proof)d, verified %(lost_verified)d; "
              "new compile errors %(new_compiler_errors)d; new source gates %(new_source_gates)d" % counts)
        for hint in verdict["hints"]:
            print("  - " + hint)
    if verdict["rules"]:
        print("REGRESSION: " + ", ".join(verdict["rules"]))
    else:
        print("PASS: no regression against the base.")


def build_parser():
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--base", default="origin/main", help="upstream base revision (default origin/main)")
    parser.add_argument("--head", default="HEAD", help="committed revision to check (default HEAD)")
    parser.add_argument("--backend", default="auto", choices=("auto", "host", "wsl", "docker", "native", "wine"))
    parser.add_argument("--compile-only", action="store_true", help="compile errors only, no judge")
    parser.add_argument("--skip-gate", action="store_true", help="skip tools/gates/gate.py (not recommended)")
    parser.add_argument("--image", default=backends.DEFAULT_IMAGE, help="docker image name")
    parser.add_argument("--build-image", action="store_true", help="rebuild the docker image first")
    parser.add_argument("--config", type=Path, help="toolchain configuration (default: per-user location)")
    parser.add_argument("--json", type=Path, help="also write the result as JSON")
    parser.add_argument("--bootstrap", action="store_true",
                        help="fetch and verify the pinned toolchain first (public repositories, cached, no credentials)")
    parser.add_argument("--unit", action="append", metavar="SRC",
                        help="compile and judge this unit on HEAD only, no base comparison (repeatable)")
    parser.add_argument("--worktree", action="store_true",
                        help="with --unit: include uncommitted changes under src/ and include/")
    parser.add_argument("--view", action="store_true",
                        help="serve the live viewer while running, and afterwards until Ctrl-C")
    parser.add_argument("--port", type=int, default=8080, help="viewer port (default 8080)")
    parser.add_argument("--objdiff", type=Path, metavar="DIR",
                        help="with --unit: objdiff project directory (default: per-user cache)")
    parser.add_argument("--no-objdiff", action="store_true", help="with --unit: skip the objdiff project")
    return parser


def resolve_backend(args):
    if args.backend != "auto":
        return args.backend
    return backends.default_backend(compile_only=args.compile_only)


def run_directory():
    import bootstrap
    stamp = time.strftime("%Y%m%d-%H%M%S")
    return bootstrap.cache_root() / "runs" / f"{stamp}-{os.getpid()}"


class NoStatus:
    """Stands in for viewer.Status when no viewer runs."""
    directory = None

    def __getattr__(self, _name):
        return lambda *args, **kwargs: None


def start_viewer(args, status_factory=None, **fields):
    if not args.view:
        return NoStatus(), None
    import viewer
    status = (status_factory or viewer.Status)(run_directory(), **fields)
    return status, viewer


def judge_units(judge, revision, units, status, *, label):
    """One judge call for all units: the judge prepares the revision once and compiles in parallel."""
    status.step(f"judge {label} ({len(units)} unit(s))")
    for tu in units:
        status.unit(tu, state="running")
    return judge(revision, units)


def object_paths(directory, tu):
    import objdiff_project
    name = objdiff_project.slug(tu) + ".obj"
    return Path(directory) / "target" / name, Path(directory) / "base" / name


def prepare_objects(args, repo, revision, tu, config, *, compiler_factory, export=export_sources):
    """Retail target and compiled base objects plus objdiff.json, when a local cl.exe can run."""
    import objdiff_project
    directory = args.objdiff or objdiff_project.default_directory(tu)
    target, base = object_paths(directory, tu)
    kind = "native" if os.name == "nt" else ("wine" if shutil.which("wine") else None)
    compiled = None
    base.unlink(missing_ok=True)   # never diff against an earlier round's object
    if kind:
        try:
            with tempfile.TemporaryDirectory(prefix="bfme2-objdiff-") as scratch:
                export(repo, revision, Path(scratch))
                ok, _lines = backends.compile_unit(compiler_factory(kind, config), Path(scratch), tu)
                built = Path(scratch) / "obj" / (tu.replace("/", "_") + ".obj")
                if ok and built.is_file():
                    base.parent.mkdir(parents=True, exist_ok=True)
                    shutil.copyfile(built, base)
                    compiled = base
        except (OSError, ValueError, subprocess.CalledProcessError):
            compiled = None
    project = objdiff_project.write_project(directory, tu, repo=repo, revision=revision,
                                            compiled=compiled, config_file=args.config)
    return project, target, (base if compiled else None)


def run_units(args, repo, backend, *, judge_factory, compiler_factory, toolchain, run, bootstrapper,
              status_factory=None, objects=prepare_objects):
    """--unit: compile and judge units on HEAD (or a working-tree snapshot) only. Unlimited local rounds."""
    units = sorted(dict.fromkeys(path.replace("\\", "/") for path in args.unit))
    for tu in units:
        if not tu.startswith("src/") or Path(tu).suffix.lower() not in (".c", ".cpp"):
            raise SetupError(f"--unit {tu}: give a translation unit under src/ (.c or .cpp)")
    if args.worktree:
        head, dirty = snapshot_worktree(repo)
        source = "working tree" + ("" if dirty else " (clean, same as HEAD)")
    else:
        head, source = git(repo, "rev-parse", "--verify", args.head + "^{commit}"), args.head
    for tu in units:
        if not exists(repo, head, tu):
            raise SetupError(f"{tu} does not exist at {source}")
    compile_only = args.compile_only or backend in ("native", "wine")
    retail = str(Path(repo) / "baseline" / "game.dat")
    status, viewer = start_viewer(args, status_factory, mode="unit", head=head, source=source, backend=backend,
                                  retail=retail)
    for tu in units:
        status.unit(tu, state="pending")
    status.step("toolchain")
    config = ensure_toolchain(args, toolchain=toolchain, bootstrapper=bootstrapper)
    judge = None
    if not compile_only:
        check_local_main(repo, head)
        if backend == "docker":
            status.step("container image")
            ensure_image(args.image, run=run, build=args.build_image)
        judge = judge_factory(backend, config, repo, image=args.image)
    server = url = None
    objects_of = {}

    def provider(tu, rva):
        """Local objects and objdump first (instant), else the judge's own --diff through the backend."""
        import viewer as view
        unit = {**status.data["units"].get(tu, {}), "base_obj": objects_of.get(tu)}
        if unit["base_obj"] and view.objdump():
            return view.row_diff(retail, unit, rva)
        if judge is not None:
            text = judge.diff(head, tu, rva)
            header, rows = view.unified_to_rows(text)
            note = " ".join(header) or text.strip()[-300:]
            if header and not rows:
                note += " · no differences after masking"
            # Before the unit's objects exist this is a stand-in: recompute once they do.
            return {"state": "ready", "source": "judge.py --diff (" + backend + ")", "rows": rows, "note": note,
                    "provisional": unit.get("state") != "done"}
        return view.row_diff(retail, unit, rva)

    if viewer is not None:
        server, url = viewer.serve(status.directory, port=args.port, provider=provider)
        print(f"Live view: {url}")
    lines, errors = [], {}
    for tu in units:
        status.step(f"{'compile' if compile_only else 'judge'} {tu}")
        status.unit(tu, state="running")
        if compile_only:
            with tempfile.TemporaryDirectory(prefix="bfme2-unit-") as scratch:
                export_sources(repo, head, Path(scratch))
                ok, diagnostics = backends.compile_unit(compiler_factory(backend, config), Path(scratch), tu)
            rows = [{"tu": tu, "rva": "", "name": "", "after": "OK" if ok else "COMPILE_ERROR",
                     "label": "COMPILED" if ok else "COMPILE ERROR"}]
            if not ok:
                errors[tu] = "\n".join(diagnostics)
        else:
            document = judge(head, [tu])
            rows = unit_rows(document, repo, head)
            error = results_table(document).get(tu, {}).get("error")
            if error:
                errors[tu] = str(error)
        fields = {"state": "done", "functions": rows, "error": errors.get(tu, "")}
        if not args.no_objdiff and exists(repo, head, "expected/" + tu[4:] + ".json"):
            status.step(f"objdiff project {tu}")
            project, target, base = objects(args, repo, head, tu, config, compiler_factory=compiler_factory)
            objects_of[tu] = str(base) if base else None
            fields.update(objdiff=str(project), target_obj=str(target), base_obj=str(base or ""))
            print(f"objdiff project: {project}")
        status.unit(tu, **fields)
        lines += rows
    print_units(lines, errors, compile_only, source)
    passed = sum(row["label"].startswith("PASS") for row in lines)
    status.finish("done", f"{passed}/{len(lines)} PASS" if not compile_only else "compiled")
    if args.json:
        args.json.write_text(json.dumps({"head": head, "source": source, "backend": backend, "units": units,
                                         "functions": lines, "errors": errors}, indent=2) + "\n", encoding="utf-8")
    hold(server, url)
    return 1 if errors else 0


def print_units(lines, errors, compile_only, source):
    print(f"Units on {source}:")
    for row in lines:
        print(f"  {row['label']:<24} {row['tu']} {row['rva']} {row['name']}")
    for tu, error in errors.items():
        print(f"  COMPILE ERROR {tu}:\n      " + "\n      ".join(error.splitlines()[:10]))
    if not compile_only:
        passed = sum(row["label"].startswith("PASS") for row in lines)
        print(f"{passed}/{len(lines)} functions PASS.")
    else:
        print("Compile-only: byte matches are not compared; run a judge backend before relying on a MATCH.")
    print(ROUNDS_NOTE)


def hold(server, url):
    """Keep the viewer up after the run until Ctrl-C."""
    if server is None:
        return
    print(f"Run finished; the live view stays at {url} until Ctrl-C.")
    try:
        while True:
            time.sleep(3600)
    except KeyboardInterrupt:
        server.shutdown()


def main(argv=None, *, repo=REPO_ROOT, gate=run_gate, planner=plan_changes, judge_factory=JudgeRunner,
         compiler_factory=compiler_backend, toolchain=load_toolchain, run=subprocess.run, bootstrapper=None,
         status_factory=None, objects=prepare_objects):
    args = build_parser().parse_args(argv)
    server = url = None
    try:
        backend = resolve_backend(args)
        if args.unit:
            return run_units(args, repo, backend, judge_factory=judge_factory, compiler_factory=compiler_factory,
                             toolchain=toolchain, run=run, bootstrapper=bootstrapper, status_factory=status_factory,
                             objects=objects)
        head = git(repo, "rev-parse", "--verify", args.head + "^{commit}")
        base = git(repo, "merge-base", args.base, head)
        status, viewer = start_viewer(args, status_factory, mode="branch", head=head, base=base, backend=backend)
        if viewer is not None:
            server, url = viewer.serve(status.directory, port=args.port)
            print(f"Live view: {url}")
        if not args.skip_gate:
            status.step("source gate")
            gate_status, report = gate(repo, base, head)
            if gate_status != "pass":
                print(report)
                print(f"GATE {gate_status.upper()}: fix the source rules before spending a Boat run.")
                status.finish("failed", "source gate " + gate_status)
                hold(server, url)
                return 1
            print("Gate: pass")
        status.step("affected units")
        plan = planner(repo, base, head)
        if plan.refusals:
            for rule, location in plan.refusals:
                print(f"REFUSED by Boat intake: {rule}" + (f" ({location})" if location else ""))
            status.finish("failed", "refused by intake")
            hold(server, url)
            return 1
        if not plan.affected:
            print("No source change to measure.")
            status.finish("done", "no source change")
            return 0
        for tu in plan.affected:
            status.unit(tu, state="pending")
        compile_only = args.compile_only or backend in ("native", "wine")
        status.step("toolchain")
        config = ensure_toolchain(args, toolchain=toolchain, bootstrapper=bootstrapper)
        if compile_only:
            status.step("compile")
            verdict = compile_verdict(compiler_factory(backend, config), repo, plan)
        else:
            check_local_main(repo, base)
            if backend == "docker":
                status.step("container image")
                ensure_image(args.image, run=run, build=args.build_image)
            judge = judge_factory(backend, config, repo, image=args.image)
            old = judge_units(judge, base, plan.affected, status, label="base")
            new = judge_units(judge, head, plan.affected, status, label="head")
            verdict = judge_verdict(old, new, plan.changed)
            for tu in plan.affected:
                status.unit(tu, state="done", functions=[row for row in verdict["functions"] if row["tu"] == tu],
                            error=verdict["errors"].get(tu, ""))
    except SetupError as error:
        print(f"precheck: {error}", file=sys.stderr)
        return 2
    print_report(plan, verdict, compile_only)
    print(ROUNDS_NOTE)
    status.finish("done", "REGRESSION: " + ", ".join(verdict["rules"]) if verdict["rules"] else "no regression")
    if args.json:
        args.json.write_text(json.dumps({"base": base, "head": head, "backend": backend,
                                         "changed": plan.changed, "affected": plan.affected,
                                         **verdict}, indent=2, default=list) + "\n", encoding="utf-8")
    hold(server, url)
    return 1 if verdict["rules"] else 0


if __name__ == "__main__":
    sys.exit(main())
