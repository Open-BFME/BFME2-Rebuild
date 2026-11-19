# Instructions for contributors and coding agents

## Project and goal

This is a structural rebuild of the Battle for Middle-earth II engine. The goal
is maintainable source, organized like an ordinary engineering project, that
compiles to the code in the reference build. Prefer sound types, names, ownership
and shared implementations over increasing the match count.

## Layout

- `src/` holds compilable translation units, the individual source files passed
  to the compiler. `src/unk/` holds units whose original paths are uncertain.
- `include/` holds shared headers. `include/Draft/` holds provisional layouts.
- `config/map.json` is the ledger of reference addresses, names and assignments
  to translation units. A function's relative virtual address (RVA) is its
  offset from the reference executable's load base. `expected/` contains
  generated records for emitted code and neighboring ranges.
  [Find a row and its owner](docs/ledger-and-checks.md#find-a-row-and-its-owner).
- `worklist/` lists missing functions and Draft headers.
- `reference/` contains the published Generals and Zero Hour source for
  comparison. The rebuild does not compile that directory.
- `docs/`, `tools/` and `ci/` describe evidence, checks and pull requests.

## Pick work

Read [CONTRIBUTING.md](CONTRIBUTING.md) and [the worklist](worklist/README.md).
Choose a small, related batch. Check current `main` for an existing body in
both source and shared headers. An empty annotation can describe a body
defined inline elsewhere. Check open issues and pull requests. Use the
[worklist claim procedure](worklist/README.md#claim-an-item) for function
addresses and coordinate shared headers by path.

Open the Zero Hour counterpart at the worklist's path and line, then
check the target's current shared header and callers. A plain clone includes
the public source and recorded evidence. Ask for missing target evidence
before guessing at a layout or function name. If the ledger has the wrong
identity or owner, [request a ledger change](docs/ledger-and-checks.md#request-a-ledger-change).

## Rules

- Keep one definition of each class, struct, global and template. Use the
  established header and actual field and parameter types.
- Name known fields for what they do. Use evidence for names, offsets, bases
  and virtual order. Retain honest placeholders where identity is unknown.
- Move all Draft includers and retire the Draft in the same header promotion.
  Check every affected unit and exercise the recovered layout.
- For a header-inline virtual, confirm its behavior, including its return
  value, against the reference class's own virtual table slot. An inline body
  can have no ledger row and receive no direct byte comparison.
- Use the existing library or engine template. Keep related code in its
  evidenced unit and request a placement review before changing ownership.
- Preserve generated annotations and provenance notices. Give a brief reason
  for nonobvious code. Record unresolved evidence in an issue or the pull
  request description. Search the [hard cases](docs/hard_rows.md) before retrying.

[Source quality and examples](docs/source-quality.md) lists the full rules.
[Recipes](docs/recipes.md) covers proven approaches.

## Read the check

The **BFME II / Boat** check accepts regular UTF-8 `.c` and `.cpp` files
under `src/` and `.h`, `.hh`, `.hpp` and `.inl` under `include/`. Follow
the [pull-request guide](ci/README.md) for the first-time `boat-ok` label
and Pending, Pass and Fail. Public admissions open after the dedicated
compiler-only pool is verified. A local check is optional.
Read the reported pull request head and main revision. The check preserves
existing match credit, reference proof and verified object results in
affected units. A sound header change can pass with zero additional
matches. Diagnostics can show the first differing function offset and a
short excerpt from your compiled function. Reference disassembly remains
private.

- **MATCH** credits equal comparison bytes under the checker's comparison
  rules. Read the reference proof result too.
- **Proven** means the checker also resolved the row's checked references.
- **VERIFIED** checks the assigned object rows, emitted extras, compile
  contract and order. Scope is the assigned object and its evidence.
- **PARTIAL** is a byte mismatch. **MISNAMED** is an identity mismatch.
  **WRONG_REFERENCE** means equal compared bytes reach a wrong target.

See [verdicts and limits](docs/ledger-and-checks.md#read-the-verdict). Runtime
behavior and original file boundaries require their own evidence.

## Never do this

- Do not commit a retail executable, retail asset, raw disassembly dump,
  private symbol source, private host details or credentials.
- Do not borrow a name from another class that shares a folded address.
- Do not hand-edit generated annotations, the ledger, sidecars or approvals to
  get a passing check. Use the maintainer's ledger-request process.
- Do not use naked functions, emitted bytes, raw `this` offsets, fake
  templates, duplicate type views, source-specific layout macros, compiler
  shaping pragmas or unsupported aliases. Keep documented existing exceptions
  within their approved scope.
- Do not bypass a checker finding, credit an unverified attempt or run the
  project's landing process yourself.
