# Source rules

Use Python 3.10 or newer and Git. Run this command from the trusted base
checkout or another trusted installation of the source checks.

    python3 tools/gates/gate.py /path/to/repo BASE HEAD --json rules-result.json

Add `--report rules-report.md` to write the report to a file. The Git input can
be a bare or working repository. BASE and HEAD can name commits or trees.
The check reads tracked Git objects, including the base ledger when needed.
Working-tree files and Git diff attributes have no effect. It runs without
a compiler, a network connection, or runtime evidence. Exit 0 means pass,
1 means fail, and 2 means the check could not complete. A dependent rule
reports a blocked failure if G1 cannot inspect an eligible source blob.

| Rule | Check and remedy |
| --- | --- |
| G1 Paths | Modify existing C/C++ units and eligible headers within the intake limits, add headers only outside Draft and delete headers only under Draft, and restore out-of-scope paths or exact function moves to their original locations. Use LF or CRLF line endings. |
| G2 Generated annotations | Keep generated row and upstream marker lines verbatim and in order, restore any marker moved away from its existing function body, and preserve compiler directives. |
| G3 Forbidden constructs | Use ordinary C/C++ and the declared symbols, with no inline assembly, naked functions, emitted bytes, symbol aliases, or pragmas outside the approved forms, including compiler controls formed by token pasting. Outside the vendored libraries (their `shims/` included) it also refuses runtime, SDK and STLport workarounds: import declarations and `__imp_` names, also inside macro bodies; declarations and bodies of the functions in `runtime_function_names` and `platform_function_names` that carry C linkage, a calling convention or an import; defines of library settings and, in source files, of `_STLP_*` and runtime names; a header included twice on one branch path (every copy is reported, so an added copy fails wherever it goes); `__declspec(nothrow)`; and C linkage (`extern "C"`) outside C code, except around `#include` lines, in the `#ifdef __cplusplus` form of a header, on the names above, and on a C name the base ledger places on a `.c` object, a prebuilt library or an import. |
| G4 Stand-in types | Keep each count of explicit stand-ins at or below the base total over the changed files, and declare the evidenced types so callers and members can use them directly. |
| G5 Comments | Keep new comments focused on evidence and intent, and move attribution, dates, private paths, work branches, and attempt measurements to the pull request description. |
| G6 One item | Keep substantive source edits within the configured file cap apart from narrow Draft promotion include and type-use edits, and split unrelated work or update every literal includer of the removed Draft. |
| G7 One definition | Define new classes, structs and unions in shared headers and keep one explicit definition of each named type or externally linked object. |
| G8 Address names | Introduce an address-shaped identifier only if the base ledger recognizes that exact spelling, and use an evidenced name or request the accepted spelling through the ledger process. |

## How to read a refusal

The report lists G1 through G8 in order. A finding gives the path, a
one-based physical line number, the rule and a one-line action. An annotation
removal points to its line in the base. A path-level failure uses line 1.
`config.json` holds the limits and allow-lists. Changes to this directory,
including the policy, code and informational baseline, are maintainer changes.

- **G1** follows the compiler CI path limits, modes and suffixes. A move
  expressed as delete plus add is two path changes. Recognizable exact
  cross-unit body moves are detected by the external function's lexical
  definition. The changed-source byte limits count head blobs, including
  their actual line terminators. Lexical checks accept LF, CRLF and mixtures
  of the two, and use normalized terminators for comparison.
- **G2** covers `FUNCTION` and reserved row markers `DATA`, `GLOBAL` and
  `VTABLE`, plus `UPSTREAM` provenance. Draft status and snapshot banners
  describe the lifecycle of a Draft, so the generated-row check allows
  their retirement. The sequence preserves spelling, indentation, multiplicity, and order.
  Compiler option and profile comments, STL and SDK selections, and the allocator marker
  are read as the compiler drivers read them, including directives on raw physical lines.
  The check measures movement past an existing recognizable function declaration.
- **G3** lexes complete C/C++ blobs, including block comments, escaped
  literals, raw strings, and line continuations. In macro definitions the check examines
  pastes that could produce reserved compiler controls, including argument-driven pastes.
  A maintainer-approved
  exception is a token fingerprint and a cap specific to a tracked file.
  The general pragma allow-list permits `once` and `warning` everywhere and
  `pack` in headers.
  It can preserve an identical occurrence already in the base.
- **G4** counts `void *` declaration and signature forms,
  `reinterpret_cast`, C-style pointer casts, arithmetic on explicit
  `this` or simple byte-pointer aliases, and casts of direct call results other than explicit primitive floating conversions.
  An explicit void-pointer slot in an annotated function head must agree with the complete
  protected compiler name and address. For a direct header member prototype, its full
  name, access, calling convention and signature must resolve to one protected owned
  address. The header allowance covers supported built-in encodings in ordinary headers
  with no branching beyond a conventional include guard. Data members, function bodies,
  and other unproved slots remain in the count. The counts are five independent ratchets.
  A Draft deletion contributes
  its old count and zero new occurrences. Whole-tree totals in
  `baseline.json` are informational. For a PR verdict the check measures
  both trees afresh over the changed paths.
- **G4b** screens direct, single-line declarations with the same name and
  lexical scope in a changed region. A pointer or recognized class type
  becoming a built-in integer can flag for review. G4b appears in the pull
  request summary as an advisory and leaves the source rules status unchanged.
- **G5** reads only added comment text. It permits hexadecimal addresses
  in evidence, and refuses standalone row-address labels that repeat the
  ledger. Vendored paths are listed in the configuration.
- **G6** compares lexical tokens and directive boundaries. Ordinary comment
  and formatting edits do not consume the source-file cap. For an eligible
  Draft removal and permanent header addition, an includer can repoint the
  relevant literal include and substitute the identified type spelling.
  The overall path limit still applies.
- **G7** extends the existing landing check's declaration-head, scope and
  before/after ownership logic. It prohibits new class, struct and union
  definitions at any source scope, including local or anonymous types.
  It distinguishes namespace identities,
  declarations and file-local objects, including the individual linkage of comma
  declarations. It checks the full eligible tree,
  including vendored paths. It reports explicit same-file duplicate
  globals and new same-name type or external object definitions with a
  surviving other owner.
- **G8** indexes `name` and `accepted_names` from the base ledger at most once
  per ledger blob. Exact symbol keys with candidates in the protected symbol ledger
  can supply an accepted spelling. Recognized compiler decoration is removed from a name
  before checking the exact source component. Reusing a spelling already
  present in the base source tree does not introduce a new spelling.

## Scope of the decision

These are lexical source rules. Whether a name is true, whether code
reads as source, and whether a type is the right one stay with a sampled
audit. The audit also judges evidence for fields and method owners,
same-layout copies under different names, template and macro reuse,
undocumented integer stand-ins, and original translation-unit ownership.
The check works on explicit syntax. It cannot resolve arbitrary
preprocessor expansions, indirect pointer data flow, template
instantiation, or semantic equivalence of two implementations. For example, in a function returning ObjectID, introducing
`unsigned int objectID = m_ownerID;` and returning
`static_cast<ObjectID>(objectID)` can erase a meaningful type or can carry
a raw value intentionally. In this example the original field declaration
is already `unsigned int m_ownerID;`. Returning ObjectID still makes the
correct role of the local an evidence question. Layout and callers
decide what the local should be. The build and inline-virtual checks have
their own verdicts.

Run the fixtures from this directory.

    python3 -B -m unittest -v test_gate.py

The replay helper runs the same checks on each selected first-parent
landing and can reproduce the informational baseline from a pinned tip.

    python3 -B replay.py replay /path/to/development.git TIP result.json --count 500
    python3 -B replay.py inventory /path/to/development.git TIP inventory.json


## Ledger runtime names

`ledger_runtime_names.py` checks the ledger rather than a change. Retail
imports the C runtime from `msvcr71.dll`, so a name the program imports
cannot also name a body linked into `game.dat`. The check refuses a row of
`config/map.json` whose name or accepted alias is a `msvcr71.dll` import or
another export of the same import entry (`strcpy` and `_mbscpy`, `_strdup`
and `_mbsdup`, `_stricmp` and `_strcmpi`, `abs` and `labs`, `getc` and
`fgetc`, `putc` and `fputc`), also as a C symbol (`_free` for `free`). The
exceptions are the linker's one import thunk per entry, bodies from a
`msvcrt.lib` object, and the reviewed entries in its `ALLOWED` table. The
import names come from `baseline/game.dat` and
`ci/managed-nt2/engine/gen/import_aliases.py`.

    python3 -B ledger_runtime_names.py ../.. --json runtime-names.json

Exit 0 means no refused row, 1 lists the refused rows, and 2 means an input
could not be read. `test_ledger_runtime_names.py` also runs it on the
committed ledger.
