# Where the numbers come from

Every figure in the README and in [status.md](status.md), how it was measured, where the record lives, and what a clone can reproduce.
"Working revision" means the maintainers' development revision the snapshot was exported from; it is not a
commit in this repository, whose `src/`, `include/`, `config/` and `expected/` are that revision's tree.

| Figure | How it is measured | Record | Reproducible from a clone? |
| --- | --- | --- | --- |
| Functions credited MATCH, with proven references | The checker compiles every source file with the reference compiler and compares each owned function with `baseline/game.dat`; "proven" adds the reference check ([verdicts](../ledger-and-checks.md#read-the-verdict)) | The maintainers' checker state at the working revision named in the README | Per file, yes, with the local toolchain ([tools/README.md](../../tools/README.md)); for a pull request, the automatic check reports the change |
| Whole objects VERIFIED | The same run: the whole object file of a source file equals the original's | Same | Same |
| Headers outside `include/Draft/`, Drafts in use, unused Drafts | Counted from this tree: a Draft is "in use" when a file includes it | This tree | Yes: `python3 worklist/generate.py` writes the Draft lists |
| Source files with code, named and address-named, and scaffolds | Counted from this tree: a `.c`/`.cpp` file "has code" if anything remains after removing comments and preprocessor lines | This tree | Yes, from `src/` |
| Names real / placeholder | The ledger `config/map.json`: owned rows whose name is a placeholder spelling ([placeholder names](../anti-patterns.md#facts-that-remove-the-need-for-a-workaround)) | This tree | Yes, from `config/map.json` |
| Link: unresolved symbols and duplicates | A standalone link of all rebuilt objects of the working revision | Maintainers' link record | No: needs the compiled objects and the linker |
| Substitution test (323 files, checksums equal, perturbed control) | A game image with the rebuilt files substituted for the original's, playing scripted skirmishes and save/load, compared frame by frame with the original's checksums | Maintainers' game runs (2026-11-19); the checksum records are private | No: needs the game data and the run harness |
| Known weaknesses (80 files graded, ten findings) | A read-only review on 2026-11-19 at one working revision, sampled files graded on six axes | Not committed | No; the ten findings point at files you can read |
| `void *` counts | Two different measures: the README's 852 counts declarations in ordinary headers that take or return `void *` (lexical); `tools/gates/baseline.json` counts every `void *` occurrence the source rules track as a ratchet | README: the review; ratchet: this tree | The ratchet, yes (`tools/gates`) |

The automatic checks on pull requests are the reproducible measure that matters most: each report names the
exact head and `main` it compared and lists what changed, function by function.
