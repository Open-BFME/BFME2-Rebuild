# Approval policy

Deterministic: a pull request lands or it does not, decided by machine checks. No model is part of the
decision.

## Lands with no person

All of these hold for the pull request's current head, checked against current `main`:

1. Every required check passed on current `main`: **BFME II / source rules** (G1–G8),
   **BFME II / Boat** (Visual C++ 7.1 match comparison; zero regressions), and **BFME II / Landing**
   (**BFME II / Inline virtual** pending can land; a fresh failure blocks).
2. Every changed path is under `src/` or `include/`; nothing under `config/`, `expected/`,
   `baseline/`, `reference/`, `tools/`, `ci/`, `.github/`, `docs/`, or a root file changes.
3. For source-only pull requests (`src/` only): eligible scope (1–8 files, at most 8 affected
   translation units) admitted within the narrow daily budget.
4. For header pull requests (`include/`, with or without `src/`): eligible scope (up to 60 files)
   admitted within the wide daily budget (H10), AND **BFME II / Header rules** (H1–H10) passed on
   current `main` (covering compiled symbol-to-ledger matches, complete declaration comparisons,
   layout-neutral edits, H5 typed-access member witnesses, and H5 VC7.1 virtual-override proofs).

## Needs a person

- Any header pull request whose **BFME II / Header rules** check is **held** (for example: an
  unproved layout/size/base change, more than 4 changed classes in one header, an override form
  outside H5's automated subset, or an H9 `Held: ledger request` item). A listed maintainer in
  `ci/landing/landing.json` approves that exact head commit to land it once Boat is green.
- Any change touching protected paths (`config/`, `expected/`, `tools/`, `ci/`, `.github/`, etc.).
- Anything a required check could not run on or that exhausted automatic retries.
