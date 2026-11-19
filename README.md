# Battle for Middle-earth II engine rebuild

This project reconstructs the engine as maintainable source organized around the
reference build's translation units and shared headers. A translation unit
is one C or C++ source file together with the headers it includes. The
compiler produces one object file for each unit. The target is the game's
version 1.06 `game.dat`. The source should read like an ordinary engineering
project and compile to the reference code.

## Status

The maintainers recorded these progress counts at
**2026-11-19 03:05 UTC**, revision `768dd266f4`.

| Measure | At that snapshot |
| --- | ---: |
| Owned functions credited MATCH | 14,733 / 41,567 |
| Credited functions with proven references | 11,297 |
| Whole objects VERIFIED | 139 / 2,161 |
| Headers outside Draft / inside Draft | 1,449 / 2,734 |
| Owned function names classified as real / placeholder | 23,483 / 18,084 |

About a third of the assigned functions had match credit at that snapshot.
See [the checker verdicts](docs/ledger-and-checks.md#read-the-verdict) for how
bytes, references, names and object verification are measured.

In the project's 2026-11-18 substitution test, the executable used
**318 rebuilt translation units** alongside the remaining engine code
from the reference build. Recorded simulation checksums agreed
**200 of 200 on Udun** in the extended run, **40 of 40 on Fords of Isen II**,
and **20 of 20 after save and reload** against the reference reload.
Linking and running an executable made entirely from the rebuild remain
open at the progress snapshot.

The earlier [Open-BFME-2](https://github.com/Open-BFME/Open-BFME-2) project
reports **100%** code byte matching and measures largely one-function source
files in its published phase-two baseline. Its
[2026-11-09 boot receipt](https://github.com/Open-BFME/Open-BFME-2/blob/master/docs/linking_obstacles/fx-module-class-eager-instances-2026-11-06.md)
records reaching the menu. This repository starts from shared definitions and
groups recovered functions into evidenced translation units. Its match count
uses the owned-function ledger shown above. The older project's percentage
measures executable code bytes.

## Help rebuild the source

Start with the [worklist](worklist/README.md) and
[contribution guide](CONTRIBUTING.md). Good first contributions are a small
group of related functions, a provisional header under `include/Draft/`,
or evidence for a name or file assignment. The [pull-request
guide](ci/README.md) explains eligibility and review. A plain clone is
enough to choose an item, inspect the public Zero Hour source, and request
evidence. A local checker run is optional. [AGENTS.md](AGENTS.md) is the
starting point for coding agents.

## Licence and affiliation

This is a modified, derived work. It is not the original program, is not an
Electronic Arts product, and has no affiliation or association with Electronic
Arts Inc. or its affiliates or their employees.

“Command & Conquer” and “Battle for Middle-earth” are named in this repository
only to say which games the code belongs to. This repository is not endorsed by
and does not speak for any owner of those names.

See [LICENSE](LICENSE) and [NOTICE](NOTICE) for the GPL-3 terms and
third-party notices. The Electronic Arts GPL release and its additional
terms cover the text derived from that release. GameSpy SDK and EA PPMalloc
text retain their own notices and are outside this repository's GPL-3 licence
statement.
