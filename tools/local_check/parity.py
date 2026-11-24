#!/usr/bin/env python3
"""Parity: the local judge against the hosted verdicts the state branch records.

usage:
  parity.py [--units 20] [--seed 0] [--state-ref origin/state] [--backend auto|host|wsl|docker]
            [--config FILE] [--json FILE] [--plan-only]

The state branch (ci/state/STATE-BRANCH.md) holds, for one main commit, the hosted check's verdict of
every function: [verdict, percent, proven], plus each file's verified and error flags. This samples
--units files spread over the source libraries, runs tools/judge/bin/judge.py locally on that same
commit with that commit's own judge code, and compares every function, verdict for verdict, through
ci/managed-check/compare.py's index (the reduction the pull-request check uses).

The judge reads its ledger from `main`; a scratch Git directory whose `main` is the measured commit
(objects shared through an alternates file, nothing copied) keeps your own branches untouched.
Exit status: 0 every sampled verdict agrees, 1 a difference, 2 setup problem.
"""
import argparse
import io
import json
import random
import subprocess
import sys
import tarfile
import time
from pathlib import Path

HERE = Path(__file__).resolve().parent
REPO_ROOT = HERE.parents[1]
sys.path.insert(0, str(HERE))
import backends  # noqa: E402
import precheck  # noqa: E402

STATE_PREFIX = "files/"


def git(repo, *args, binary=False):
    done = subprocess.run(["git", "-C", str(repo), *args], capture_output=True)
    if done.returncode:
        raise precheck.SetupError(f"git {' '.join(args)}: {done.stderr.decode(errors='replace').strip()}")
    return done.stdout if binary else done.stdout.decode("utf-8").strip()


def read_state(repo, ref):
    try:
        index = json.loads(git(repo, "show", f"{ref}:index.json"))
    except precheck.SetupError as error:
        raise precheck.SetupError(f"{error}\n  fetch the hosted verdicts first: git fetch origin state") from None
    return index


def library(tu):
    """The library a unit belongs to: src/<lib>, or src/Code/<part>/<area> for the game code."""
    parts = tu.split("/")
    if len(parts) > 3 and parts[1] == "Code":
        return "/".join(parts[:4] if parts[2] in ("Libraries", "GameEngine", "GameEngineDevice") else parts[:3])
    return "/".join(parts[:2])


def sample(units, count, seed=0):
    """COUNT units dealt round-robin over the libraries, each library's order shuffled by SEED."""
    groups = {}
    for tu in sorted(units):
        groups.setdefault(library(tu), []).append(tu)
    rng = random.Random(seed)
    queues = []
    for name in sorted(groups):
        members = groups[name][:]
        rng.shuffle(members)
        queues.append(members)
    rng.shuffle(queues)
    picked = []
    while len(picked) < count and any(queues):
        for queue in queues:
            if queue and len(picked) < count:
                picked.append(queue.pop())
    return sorted(picked)


def state_record(repo, ref, tu):
    return json.loads(git(repo, "show", f"{ref}:{STATE_PREFIX}{tu}.json"))


def normalize_rva(text):
    return f"0x{int(text, 16):08X}"


def local_record(item, compare):
    """The hosted record shape from one judge result, through compare.index."""
    unit = compare.index(precheck.results_table({"results": [item]}))[item["tu"]]
    proven = {normalize_rva(rva) for rva in unit["proven"]}
    functions = {normalize_rva(rva): [status, round(float(pct), 6), normalize_rva(rva) in proven]
                 for rva, (status, pct) in unit["rows"].items()}
    return {"functions": functions, "verified": unit["verified"], "error": unit["error"]}


def differences(tu, hosted, local):
    lines = []
    for flag in ("verified", "error"):
        if bool(hosted.get(flag)) != bool(local[flag]):
            lines.append(f"{tu}: {flag} hosted {hosted.get(flag)} local {local[flag]}")
    wanted = {normalize_rva(k): v for k, v in hosted.get("functions", {}).items()}
    for rva in sorted(set(wanted) | set(local["functions"])):
        h, l = wanted.get(rva), local["functions"].get(rva)
        if h is None or l is None:
            lines.append(f"{tu} {rva}: hosted {h} local {l}")
        elif h[0] != l[0] or abs(float(h[1]) - float(l[1])) > 1e-6 or bool(h[2]) != bool(l[2]):
            lines.append(f"{tu} {rva}: hosted {h} local {l}")
    return lines


def agreement(hosted, local_items, compare):
    """{tu: [difference lines]} and per-function counts."""
    result, total, agreed = {}, 0, 0
    for tu, record in hosted.items():
        item = local_items.get(tu)
        if item is None:
            result[tu] = [f"{tu}: no local result"]
            total += len(record.get("functions", {}))
            continue
        mine = local_record(item, compare)
        lines = differences(tu, record, mine)
        result[tu] = lines
        keys = {normalize_rva(k) for k in record.get("functions", {})} | set(mine["functions"])
        bad = {line.split()[1].rstrip(":") for line in lines if line.startswith(tu + " 0x")}
        total += len(keys)
        agreed += len(keys - bad)
    return result, total, agreed


def prepare(repo, main, workspace, backend):
    """Judge code of MAIN in WORKSPACE/work, and a Git directory whose main is MAIN. Returns (work, git, mounts)."""
    workspace = Path(workspace)
    work, gitdir = workspace / "work", workspace / "git"
    if not (work / "tools" / "judge" / "bin" / "judge.py").is_file():
        work.mkdir(parents=True, exist_ok=True)
        data = git(repo, "archive", "--format=tar", main, "--", "tools", "baseline", binary=True)
        with tarfile.open(fileobj=io.BytesIO(data)) as archive:
            archive.extractall(work, filter="data")
    objects = precheck.git_common_dir(repo) / "objects"
    if not (gitdir / "HEAD").is_file():
        subprocess.run(["git", "init", "-q", "--bare", str(gitdir)], check=True)
    mounts, alternate = (), objects.as_posix()
    if backend == "docker":
        mounts, alternate = ((str(objects), "/objects"),), "/objects"
    elif backend == "wsl":
        alternate = backends.wsl_path(objects)
    (gitdir / "objects" / "info").mkdir(parents=True, exist_ok=True)
    # Bytes, not text: a Windows text write would end the lines with CR, which Git reads as part of the path.
    (gitdir / "objects" / "info" / "alternates").write_bytes(alternate.encode("utf-8") + b"\n")
    (gitdir / "refs" / "heads").mkdir(parents=True, exist_ok=True)
    (gitdir / "refs" / "heads" / "main").write_bytes(main.encode("ascii") + b"\n")
    return work, gitdir, mounts


def build_parser():
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--units", type=int, default=20)
    parser.add_argument("--seed", type=int, default=0)
    parser.add_argument("--state-ref", default="origin/state")
    parser.add_argument("--backend", default="auto", choices=("auto", "host", "wsl", "docker"))
    parser.add_argument("--image", default=backends.DEFAULT_IMAGE)
    parser.add_argument("--config", type=Path)
    parser.add_argument("--json", type=Path)
    parser.add_argument("--plan-only", action="store_true", help="print the sample and stop")
    return parser


def main(argv=None, *, repo=REPO_ROOT, judge_factory=precheck.JudgeRunner, toolchain=precheck.load_toolchain,
         run=subprocess.run, workspace=None):
    args = build_parser().parse_args(argv)
    try:
        index = read_state(repo, args.state_ref)
        main_commit = index["main"]
        units = sample(list(index["files"]), args.units, args.seed)
        print(f"State: main {main_commit}, hosted checker {index.get('checker', '?')[:16]}..., "
              f"{len(index['files'])} files; sampled {len(units)} across "
              f"{len({library(tu) for tu in units})} libraries:")
        for tu in units:
            print("  " + tu)
        if args.plan_only:
            return 0
        backend = args.backend if args.backend != "auto" else backends.default_backend(compile_only=False)
        if backend in ("native", "wine"):
            raise precheck.SetupError("parity needs a judge backend (host, wsl or docker)")
        config = toolchain(args.config)
        if workspace is None:
            import bootstrap
            workspace = bootstrap.cache_root() / "parity" / main_commit[:12]
        work, gitdir, mounts = prepare(repo, main_commit, workspace, backend)
        if backend == "docker":
            precheck.ensure_image(args.image, run=run)
        judge = judge_factory(backend, config, work, image=args.image, git_dir=gitdir, mounts=mounts)
        hosted = {tu: state_record(repo, args.state_ref, tu) for tu in units}
        print(f"Judging {len(units)} units locally on {main_commit[:12]} with backend {backend} ...")
        # One call: the judge prepares the revision once and compiles the units in parallel
        # (JUDGE_WORKERS, default 12); a call per unit pays that set-up and runs one compiler at a time.
        started = time.time()
        items = precheck.results_table(judge(main_commit, units))
        print(f"  judged {len(items)} units in {time.time() - started:.0f} s")
    except precheck.SetupError as error:
        print(f"parity: {error}", file=sys.stderr)
        return 2
    _intake, compare, *_ = precheck.ci_modules()
    result, total, agreed = agreement(hosted, items, compare)
    same = sorted(tu for tu, lines in result.items() if not lines)
    print(f"Agreement: {len(same)}/{len(units)} units identical; {agreed}/{total} functions agree "
          f"({100.0 * agreed / total if total else 100:.1f}%).")
    for tu, lines in sorted(result.items()):
        for line in lines[:10]:
            print("  DIFF " + line)
    if args.json:
        args.json.write_text(json.dumps({"main": main_commit, "hosted_checker": index.get("checker"),
                                         "backend": backend, "units": units, "identical_units": same,
                                         "functions": total, "functions_agree": agreed,
                                         "differences": result}, indent=2) + "\n", encoding="utf-8")
    return 0 if len(same) == len(units) else 1


if __name__ == "__main__":
    sys.exit(main())
