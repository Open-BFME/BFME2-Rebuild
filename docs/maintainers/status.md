# Status in detail

The live figures are on the
[`progress` branch](https://github.com/Open-BFME/BFME2-Rebuild/tree/progress), measured by a workflow
after every change to `main`. This page holds the maintainers' fuller counts from one snapshot. The two
use different measures: the progress card's *finished functions* must be matched, proven, in a properly
named file and declared in a real header, so it is lower than the count of functions *credited MATCH*
below.

## Counts

The maintainers recorded these progress counts at
**2026-11-21**, revision `2e6bf1e322` of their
working repository (the tree of this snapshot's `src/`, `include/`,
`config/` and `expected/` is that revision or a later one).

Terms ([full definitions](../ledger-and-checks.md#read-the-verdict)): an
*owned function* is one the ledger assigns to a source file here; *credited
MATCH* means it compiles to the original bytes; *proven* means its references
(calls, data) were checked too; a *VERIFIED* object is a whole source file
whose object code equals the original's; a *placeholder* name is a stand-in
for an unknown real name ([placeholder names](../anti-patterns.md#facts-that-remove-the-need-for-a-workaround)).

| Measure | At that snapshot |
| --- | ---: |
| Owned functions credited MATCH | 16,591 / 41,565 |
| Credited functions with proven references | 13,476 |
| Whole objects VERIFIED | 212 / 2,151 |
| Headers outside `include/Draft/` | 1,745 |
| Owned function names classified as real / placeholder | 23,569 / 17,996 |

40% of the owned functions had match credit at that snapshot.

Source files with code: **857 named** and **491 still named by
address** (under `src/unk/`, waiting for evidence of their real name and
place). Another 1,903 files hold only generated
annotations and no code yet (1,674 address-named, 229
named); they are scaffolds, counted apart.

In this snapshot's tree, **12 Draft headers are in use** (included by
some file; these are the ones to retire). Another 2,057 Drafts are
unused generated layout guesses: 2,047 are kept as leads for classes not
written yet and 10 are dead copies of headers that now exist outside
`include/Draft/`. See the [worklist](../../worklist/README.md).

See [the checker verdicts](../ledger-and-checks.md#read-the-verdict) for how
bytes, references, names and object verification are measured.

The game does not yet build from this source alone. A link of the rebuilt
objects by themselves, at working revision `04194f7449`, still lacked 3,130
symbols; 1,052 of them belong to functions nobody has written yet.

In the project's substitution test of 2026-11-20, an executable used
**366 rebuilt translation units** (6,147 functions) alongside the remaining
engine code from the reference build. Its recorded simulation checksums
agreed with the reference build's on **40 of 40** compared frames to frame
1000 on Udun, **40 of 40** to frame 1000 on Fords of Isen II and **200 of
200** to frame 5000 on Udun, and its save-and-reload run passed. A
deliberately perturbed control run diverged at frame 500, so the comparison
does detect a difference.

## The earlier project

The earlier [Open-BFME-2](https://github.com/Open-BFME/Open-BFME-2) project
reports **100%** code byte matching and measures largely one-function source
files in its published phase-two baseline. Its
[2026-11-09 boot receipt](https://github.com/Open-BFME/Open-BFME-2/blob/master/docs/linking_obstacles/fx-module-class-eager-instances-2026-11-06.md)
records reaching the menu. This repository starts from shared definitions and
groups recovered functions into evidenced translation units: the earlier
project proved per-function byte matching, this one organises the result into
real source files. Its match count
uses the owned-function ledger shown above. The older project's percentage
measures executable code bytes.
