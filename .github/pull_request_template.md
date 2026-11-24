<!-- One file, one Draft header or one function per pull request. Keep the Claim lines first. -->

Claim: src/path/File.cpp
<!-- or: Claim: include/Draft/X.h   or one line per function: Claim: 0x00ABCDEF -->

Confidence: N/10 — <one-sentence reason>
<!-- 10 matches, every name and type evidenced; 7 matches, one name or type inferred;
     5 matches, names or types guessed; 3 partial match; 1 does not match.
     Every comment you write on this pull request or a review carries the same line. -->

## What this changes

<!-- The functions or the header, in a sentence or two. -->

## Evidence

- Zero Hour counterpart (path:line), and where `baseline/game.dat` differs from it:
- Where each new or changed name and type comes from (string, export, own vtable slot, Zero Hour):
- Virtuals added or changed, with their vtable slot:
- Not confirmed:

## Checklist (RULES.md, docs/anti-patterns.md)

- [ ] `gate.py` passes on this head; local `precheck.py` result:
- [ ] Real types, one definition, real templates; no anti-pattern
- [ ] Only the claimed item changed; `// FUNCTION:` and `// cl:` lines untouched
- [ ] `Removed-Declaration:` lines for every declaration removed or renamed
