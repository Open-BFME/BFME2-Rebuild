# The checks on a pull request

Source rules run automatically on pull requests to `main`. An admitted comparison also publishes Boat and Inline
virtual results. You do not need to install anything on your machine.

| Status | What it checks | Where it runs |
| --- | --- | --- |
| **BFME II / source rules** | The text rules G1 to G8 in [RULES.md](../RULES.md): paths and sizes, annotations, constructs, stand-in types, comments, scope, definitions and names. One status names any rule that needs a fix (`Fix needed in G1, G5`); its Details page has the full G1 to G8 table. | GitHub-hosted runner, within about a minute |
| **BFME II / Boat** | Compiles the changed files with the original 2003 compiler and compares every function with the original program, byte for byte, with its references. | A Boat compiler job for a narrow change or a matrix of Boat jobs for a wider change. Intake and reporting run on GitHub-hosted runners. |
| **BFME II / Inline virtual** | How the change affects inline virtual functions. | Reports on GitHub-hosted runners. Pending does not hold a landing; a fresh failure blocks landing, and a stale failure triggers remeasurement on current `main`. |

A fourth status, **BFME II / Landing**, says whether and when the pull request lands
([ci/landing/README.md](landing/README.md)).

## How a check runs

1. **Intake (GitHub-hosted).** The change is read from the pull request's exact latest commit with no credential
   and checked against the source rules. A change that cannot be compiled safely is refused here, before any
   compiler machine starts:
   - an edit to a workflow, the checker, the ledger, configuration or the baseline (`.github/`, `ci/`,
     `tools/`, `config/`, `expected/`, `baseline/`);
   - a new or deleted source file (recovered functions go into the existing file that owns them);
   - a file over the size limits (1 MiB per file, 8 MiB of changed source, at most 60 changed paths);
   - an `#include` that is not a literal path relative to the project, such as an absolute path.

   The refusal names the rule and how to fix it.
2. **Compile and compare (Boat).** A narrow comparison runs on one machine. A wider comparison uses several
   machines. The isolated compiler receives source files and headers. The protected check compares the results
   with the original. The maintainer's account and the repository's workflow bot can start the compiler jobs.
3. **Report (GitHub-hosted).** The verdict is published as the status on the pull request's exact head, with the
   readable report in the run's summary (the status's **Details** link).

Timing varies with runner availability and the files affected. A shared header change under `include/` also
checks the affected source files.

## Pull requests from a fork

- The source rules run at once.
- **Ready source-only pull requests** (1-8 eligible `src/` paths affecting at most 8 translation units, passing
  current source rules, within the daily admission limits) are admitted automatically on every push without a
  `boat-ok` label.
- **Draft pull requests** run source rules, while automatic Boat admission waits until the pull request is marked
  **Ready for review**.
- **Ready header (`include/`) pull requests** (up to 60 files under `src/` and `include/`, passing current
  source rules) are admitted automatically under the wide daily budget (currently 10/author and 40 total/day; narrow source-only budget is 60/author and 300 total/day).
- **Other maintainer-scoped pull requests** wait for a maintainer to admit each commit with the `boat-ok` label
  (`Held: awaiting-admission` / `Held: awaiting-readmission`).

An event started by the maintainer on a pull request from a branch of this repository enters compiler intake
automatically.

## When it lands

A pull request lands by itself, as one squash commit credited to you, when the source rules and the Boat check
passed on its latest commit against the current `main`, Inline virtual is not failing (a fresh failure blocks
landing and a stale failure triggers remeasurement; pending can land), it is ready for review (not a draft), it
merges cleanly and it changes nothing under `.github/`. Source rules (including repository-wide G7 one-definition checks) must be current on `main` before landing; for `src/Code/*.c(pp)`-only pull requests, a Boat pass on a recent disjoint `src/Code/`-only ancestor carries forward after a fast hosted source-rules recheck. Header pull requests are prioritized ahead of narrow landings while measuring. Changes under
`include/` land automatically once **BFME II / Header rules** pass on current `main`, or once a listed
maintainer in `ci/landing/landing.json` approves that exact commit when header rules are held (not failed). A draft can receive
feedback before you choose **Ready for review**.

## Read the result

- **Pending:** queued, waiting for admission, or running.
- **Success:** the change meets the landing checks on the `main` commit named in the report.
- **Failure:** the report names the cause: a refused path or rule, a merge conflict, a compiler diagnostic, a
  function that no longer matches, a lost reference proof or a whole object that no longer verifies. Fix it and
  push to the same branch; the checks run again.

Report rows show the file, the function where known, its address and size, and the verdict before and after. A
near miss shows the match percentage, both sizes and the first differing offset, and for a function the pull
request edited, a few compiled instructions near that offset. Compiler diagnostics give the source location and
the diagnostic code. The report never contains the original program's bytes or disassembly; read those from
`baseline/game.dat` yourself.

If your change intentionally removes or renames a declaration the check protects, put one
`Removed-Declaration: Class::member` line per declaration in the latest commit message and explain it in the
description. The check names the declaration when it asks for this.

## Say what you are working on

Start the pull request description with `Claim:` lines, as the template shows: one file, one Draft header or one
function address per line. Claims are a courtesy so two people do not write the same function; they reserve
nothing, and nothing enforces them. Open a draft early to show your claim.
