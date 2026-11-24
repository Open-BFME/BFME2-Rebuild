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

`tools/local_check/` repeats the hosted pull-request check on your machine
before you spend a Boat run. It brings no compiler: you supply your own
licensed copy of Visual Studio .NET 2003, kept outside the checkout, and it
must match the image byte for byte.

1. **Verify the toolchain once.** Every digest comes from
   [docs/compiler-image.md](../docs/compiler-image.md). `cl.exe`, `c1.dll`,
   `c1xx.dll`, `c2.dll`, `mspdb71.dll`, `msvcr71.dll` and `msvcp71.dll`
   must match. Header trees you name (`--psdk-root`, `--dxsdk-root`,
   `--stlport-root`, `--overlay-root`, or a whole `--tools-root` laid out like
   `build.sh --inputs`) are checked too. Add `--strict` to fail on any header
   difference. The verified paths are saved to
   `%LOCALAPPDATA%\bfme2-rebuild\toolchain.json` or
   `~/.config/bfme2-rebuild/toolchain.json`, never to the checkout.

   ```sh
   python tools/local_check/verify_toolchain.py \
     --vc71-root "/path/to/Microsoft Visual Studio .NET 2003" \
     --runner-root /path/to/open-bfme-2 --demangler "$HOME/.cache/bfme2-judge/demangle"
   ```

2. **Pre-check a committed branch.** This runs `tools/gates/gate.py` first and
   stops on any source-rule failure. It then finds the changed and affected
   units with `ci/managed-check/intake.py`, including its refusals such as
   `new_tu`, `deletion`, `no-includers` and `wide-cap`. The judge runs on the
   merge base and on `HEAD`, and `ci/managed-check/compare.py` reduces the two
   results. Each function prints as `PASS`, `MISMATCH` or `COMPILE ERROR`.
   Exit status `0` means no regression, `1` means a gate failure, refusal or
   regression, and `2` means a setup problem.

   ```sh
   git fetch origin && git branch -f main origin/main   # the judge reads the ledger on local main
   python tools/local_check/precheck.py --base origin/main
   ```

| Host | Backend | What runs |
| --- | --- | --- |
| Linux with Wine | `host` (default when `wine` is on `PATH`) | `judge.py` with the setup above |
| Windows | `wsl` (default when WSL is installed) | the same judge inside WSL, with Wine installed there |
| Linux, macOS, Windows | `docker` | [Dockerfile](local_check/Dockerfile): Ubuntu 24.04, Wine `9.0~repack-4build3`, Python 3 and the demangler, no network |
| Windows | `native` (`--compile-only`) | `cl.exe` directly, no Wine; compile errors only |
| Linux | `wine` (`--compile-only`) | `wine cl.exe`; compile errors only |

The Docker image holds no Microsoft file. `precheck.py --backend docker` builds
it from `tools/` on first use and mounts the checkout, its Git directory, the
verified VC7.1 root and the support checkout read-only. Judge state stays in
the `bfme2-judge-state` volume.

`--compile-only` builds each `cl.exe` command from the rule functions in
`tools/judge/bin/judge.py`: base flags, `// cl:` allowlist, default STLport
4.6 and SDK markers. It finds compile errors in seconds but cannot prove a
`MATCH`. Units that use `// stlport46` need `--overlay-root`. The judge
itself stays POSIX-only because it relies on `fcntl` locks, `/proc/meminfo`
and forked workers, so a Windows judge runs in WSL or Docker.

A local pass is a forecast, not a status. The hosted check still decides
admission and landing.

Run the offline tests with `python -m unittest discover -s tools/local_check -v`.

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
