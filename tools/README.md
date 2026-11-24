# Checker

The **BFME II / Boat** check runs by itself on every pull request (a pull
request from a fork after the maintainer admits it). The compiler runs on
isolated runners and a separate step judges the object files they return. You can inspect the report
without installing a compiler, Wine or the game. See the
[pull-request instructions](../ci/README.md) for eligibility and reporting.

## What the two checks do

The **judge** works on a Git revision and one or more translation units. It reads
the committed source and headers, the reference address ledger in
`config/map.json`, symbol and Draft metadata, and the unit's records in
`expected/`. It compiles with the reference toolchain, reads the resulting
COFF object and compares owned functions and expected emitted code with
their reference ranges. The comparison accounts for relocation operands
and checks their target identities separately. It also checks annotations,
emitted extras, order and compile receipts where object verification
requires them. [Read the verdicts](../docs/ledger-and-checks.md#read-the-verdict).

The **pull-request precheck** measures the change on the recorded base. It
checks source rules, generated inputs, changed units and the closure of
affected includers, and compares the new verdicts with the baseline for
regressions. Its result is a report, whereas the maintainers' landing check
remeasures before updating the repository. The landing and farm programs
are operated by maintainers.

## Optional local check inputs

A local judge compares against `baseline/game.dat`, the version 1.06
program whose image hash `config/symbols.json` records. It needs no other
game file. It does need the x86 Microsoft Visual C++ 7.1 compiler from Visual Studio
.NET 2003, a compatible Wine setup, Python, the corresponding STLport and
its overlays, and any DirectX SDK or shim headers required by the unit.
Never include a compiler, another binary or raw checker artifacts in a
contribution.

The ordinary compilation profile uses `-nologo -c -O2 -GR- -EHsc- -O1
-arch:SSE -G7 -DNDEBUG -MD -EHsc -Gy`. The checker adds compiler include
capture, the project's include root and allowed per-unit flags. Source
markers select the STL, SDK and any evidenced profile exceptions, including
upstream C and x87 profiles. Consult the committed unit and these rules
before changing flags. A whole translation unit shares the profile.

## What you can reproduce locally

With matching inputs and a committed checkout, the packaged judge can
compile a selected translation unit, decode its object file and print
the owned rows, expected emitted rows, reference proofs and assigned-object
verdicts. An object file is the compiler's output for one translation unit.
You can run the judge separately on a base and a proposed revision to
inspect existing rows. The published comparison below records an actual
five-file agreement with the installed checker.

The hosted check finds the affected files from a pull request, validates
the submitted paths and source, and compares those files with the recorded
base. The service also verifies admission, reviewer and commit identities
before landing. Those behaviors need the hosted precheck and landing
service. A local judge invocation supplies no GitHub status or landing
approval. A standalone link, executable run and comparison of recorded
simulation behavior each need a separate test.

The public pull-request check is the standard for review. You can choose
work, read the current evidence and interpret a hosted report with a plain
checkout. Keep local binary comparisons, disassembly and object files in
your own reference environment.

## Run the packaged judge locally

The [judge source](judge/bin/judge.py) works with a committed revision and
a clean local `main` containing the approved ledger. This Linux setup uses
Python, Git, Wine with Win32 support, and Visual Studio .NET 2003. The
compiler used for the 2026-11-18 comparison reports file version
`13.10.3077` and product version `7.10.3077` in its executable
version resource. Use a matching copy of the toolchain.

The packaged code includes the object reader, relocation and emission
rules, compiler selection, result cache, symbol indexing, assembly
policy and three compiler-support modules. The remaining Python helpers,
the approved STLport headers and overlays, and the SDK and build shims
come from a separate installation of the
[Open-BFME-2 toolchain support tree](https://github.com/Open-BFME/Open-BFME-2/tree/39cdd2ca5c409c01b9c08d5dae7b3835c59a8b25).
Use revision `39cdd2ca5c409c01b9c08d5dae7b3835c59a8b25` for the configuration measured below. The required
parts are `tools/`, `vendor/stlport*`, `vendor/dxsdk-aug2005/` where used,
and the corresponding `reference/shims/` and compiler include trees. A
sparse checkout of the support tree can keep its runtime baselines out of
your checkout. Set the path to the reference program explicitly; it is `baseline/game.dat` in the checkout.

Build the checked-in LLVM demangler source and point the checker to the
result. The comparison used LLVM 18 from the demangler's shared-library
dependency on 2026-11-18. Set the paths below from the root of a checkout of
this project. Use an initialized Wine prefix with its Z drive mapped to
the filesystem root, or initialize one at the configured path before judging.

```sh
export BFME_JUDGE_RUNNER=/path/to/the/support-checkout
export BFME_JUDGE_REPO="$(git rev-parse --absolute-git-dir)"
export BFME_JUDGE_RETAIL="$PWD/baseline/game.dat"
export BFME_JUDGE_STATE="$HOME/.cache/bfme2-judge"
export BFME_JUDGE_WINEPREFIX="$BFME_JUDGE_STATE/wine"
export VC71_ROOT="/path/to/Microsoft Visual Studio .NET 2003"

mkdir -p "$BFME_JUDGE_STATE"
c++ -std=c++17 -O2 tools/judge/demangle.cpp \
  $(llvm-config-18 --ldflags --libs --system-libs) \
  -o "$BFME_JUDGE_STATE/demangle"
export BFME_JUDGE_DEMANGLER="$BFME_JUDGE_STATE/demangle"
PILOT_REMOTE=0 python3 tools/judge/bin/judge.py --json HEAD \
  src/Code/GameEngine/Source/Common/System/UnicodeString.cpp
```

The launcher writes logs and scratch state under `BFME_JUDGE_STATE`. Read
`summary`, `results[].rows`, `results[].expected` and
`results[].verification_blockers`. An exit status of zero means the
comparison completed. Individual rows can still be missing or partial.
Run only source you trust with your own local checker. Keep `--diff`
output, compiled objects and raw logs local.

## Fast local pre-check (`tools/local_check/`)

Repeat the hosted **BFME II / Boat** check on your machine, as often as you like.

1. **Install** Python 3, Git, and one judge host: Wine on Linux, WSL with Wine, or Docker/Podman (any OS).
2. **Fetch and verify the toolchain once:** `python tools/local_check/bootstrap.py`. It sparse-fetches
   the pinned compiler (Open-BFME-1) and support tree (Open-BFME-2) into your user cache without
   credentials, checks every digest of the hosted image manifest and writes `toolchain.json`.
3. **Sync the ledger:** `git fetch origin && git branch -f main origin/main` (the judge reads local `main`).
4. **Check a branch like Boat:** `python tools/local_check/precheck.py --bootstrap --base origin/main`.
   It runs `tools/gates/gate.py`, finds affected units, judges base and `HEAD` and reports each
   function as `PASS`, `MISMATCH` or `COMPILE ERROR`. Exit `0` means no regression.
5. **Iterate on one unit:** `precheck.py --unit src/<path>.cpp [--worktree]` judges `HEAD`, or with
   `--worktree` your uncommitted edits, without a base. It also writes an objdiff project
   (`objdiff.json` plus retail and compiled objects) and prints its path.
6. **Watch it live:** add `--view` and open <http://localhost:8080> to see the step, the unit, each
   verdict and a side-by-side retail/compiled disassembly. `viewer.py` reopens the last run.
7. **Check parity** with hosted verdicts: `git fetch origin state && python tools/local_check/parity.py`.

`--backend host|wsl|docker|native|wine` overrides the automatic choice. `native` and `wine` only compile.
Set `CONTAINER_CONNECTION` for a non-default Podman machine. A local pass is a forecast: the hosted
check still decides admission. Tests: `python -m unittest discover -s tools/local_check`.

## Copy comparison

At **2026-11-18T23:41:22Z**, the installed checker `513841a485f384d051de6cf28d2087d001104c71de9b2576be5710891ec5553e` and this copy
`7a5cd352145f8ce2b0301ce699c757861f836b56c7656d80b189559ba977e807` independently compiled the same five files at
`f9922c7bcf283ea72ae574374c3c59ffaa49f42c` using the same reference inputs. The measured owned rows,
expected emitted rows, per-row credit and proof, and object verdicts agreed.

| File | Owned rows | Expected emitted rows | MATCH | Proven | VERIFIED |
| --- | ---: | ---: | ---: | ---: | --- |
| GameFont.cpp | 48 | 92 | 17 | 9 | no |
| SidesList.cpp | 123 | 382 | 34 | 16 | no |
| AptLanLobby.cpp | 63 | 90 | 31 | 8 | no |
| UnicodeString.cpp | 47 | 50 | 19 | 19 | no |
| trees.c | 20 | 0 | 20 | 20 | yes |

The checker identity changes when the path configuration code changes.
This receipt covers the listed files at the measured revision. The hosted
check still supplies the pull-request and landing verdicts.
