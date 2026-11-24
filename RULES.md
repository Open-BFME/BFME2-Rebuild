# Source rules

The checks enforce these rules. A refusal names the rule, the file, the line and what to change.
G1–G8 run on every pull request (**BFME II / source rules**, and locally with `tools/gates/gate.py`).
H1–H10 run when `include/` changes (**BFME II / Header rules**). Review rules the checks cannot see are in
[docs/anti-patterns.md](docs/anti-patterns.md).

## Source rules (G1–G8)

| Rule | Do this | Bad | Good |
| --- | --- | --- | --- |
| **G1 Paths** | Edit existing `.c`/`.cpp` under `src/` (never `src/Code/Tools/`) and headers under `include/`; add headers only outside `include/Draft/`, delete headers only inside it; no new source files, no moving a body between files; at most 60 paths, 1 MiB per file, 8 MiB in all. | Add `src/Game/NewThing.cpp` for a recovered function. | Write the body under its `// FUNCTION:` line in the file that owns it. |
| **G2 Annotations** | Never change, move or reorder `// FUNCTION:`, `// DATA:`, `// cl:`, `// stlport*` or `// psdk` lines; only a `// dxsdk` or `// platformsdk` line may be added at the top of a non-vendored `.c`/`.cpp`. | Edit `// cl: /O1` to `// cl: /O2`. | Leave the line; file a ledger request if the setting looks wrong. |
| **G3 Constructs** | No inline assembly, emitted bytes, symbol aliases or pragmas other than `once`, `warning` (and `pack` in headers). | `__asm nop` or `#pragma optimize("", off)`. | Ordinary C or C++. |
| **G3 Workarounds** | Outside vendored libraries, no hand-written runtime, SDK or STLport workaround: `dllimport`/`_CRTIMP`/`WINBASEAPI` declarations, `__imp_` names, C-linkage or calling-convention declarations of runtime/Windows functions, bodies with their names, `#define`/`#undef` of `_CRTIMP`, `_DLL`, `_MT`, runtime header guards, `_BFME_*` or (in sources) `_STLP_*`, a header included twice, `__declspec(nothrow)`. | `extern "C" __declspec(dllimport) int __cdecl _strcmpi(const char *, const char *);` | `#include <string.h>` and call `_strcmpi`. |
| **G4 Types** | Do not raise the count of `void *`, `reinterpret_cast`, C-style pointer casts, `this` arithmetic or casts of call results in the files you change. | `void *getFile();` for a known `File`. | `File *getFile();` |
| **G4b Type swaps** (advisory) | Keep an evidenced pointer or class type when editing a declaration. | `File *m_file;` → `unsigned int m_file;` | Keep `File *m_file;`. |
| **G5 Comments** | New comments carry evidence or intent only; no dates, author or tool tags, local paths, attempt scores, or a bare label repeating the row's address. | `// 0x005DA563 size 75, tried 4 variants, 2026-11-20` | `// The resend walk is kept: the reference follows the next link.` |
| **G6 Scope** | One logical item; substantive edits in at most three source files (a Draft promotion may also touch every includer). | Four unrelated units in one pull request. | One pull request per unit. |
| **G7 One definition** | Define classes, structs and unions only in shared headers, once; never redefine a type or global that exists anywhere in the tree. | A second `struct FileInfo` in a `.cpp`. | `#include` the existing header. |
| **G8 Names** | No new address-shaped names (`sub_…`, `rva…`, `g_Va…`, `Rva…`) unless the ledger accepts that exact spelling. | `void sub_00401234();` | An evidenced name, or the ledger's accepted placeholder. |

## Header rules (H1–H10)

| Rule | Do this | Bad | Good |
| --- | --- | --- | --- |
| **H1 Wide parity** | Every unit that includes a changed header must still match as before. | A new inline body that changes code in 40 includers. | Declare it; define it out of line in its owning file. |
| **H2 Ledger names** | Each added or changed declaration compiles to the exact symbol the ledger holds. | `int getCount();` where the ledger has `getCount() const`. | `int getCount() const;` |
| **H3 Qualified identity** | Nothing defined twice under one qualified name. | `Foo::Bar` in two headers. | One header, included by both users. |
| **H4 Removals** | Each removed or renamed declaration gets a `Removed-Declaration: Class::member` line in the last commit message, and nothing still uses it. | Rename `m_unk28` to `m_duration` with no trailer. | Add `Removed-Declaration: Foo::m_unk28`. |
| **H5 Layout proof** | A changed size, offset, base or virtual order must be used by a newly matched function in the same pull request. | Insert a member nothing reads. | Add the member with the function that reads it. |
| **H6 Inline bodies** | An inline body in a header stays trivial unless its behaviour is proven. | A 20-line inline helper with no owning row. | Declare it; define it under its `// FUNCTION:` line. |
| **H7 Draft hygiene** | A Draft replaced by a real header is deleted, and every includer moved, in the same pull request. | Add `include/GameLogic/Foo.h` and keep `include/Draft/Foo.h`. | Move all includers, delete the Draft. |
| **H8 Inlining and macros** | No forced inlining or macro tricks the original does not have. | `__forceinline` on a new accessor. | A plain declaration, or the Zero Hour inline it corresponds to. |
| **H9 Ledger request** | A name or size the ledger lacks holds the pull request for a maintainer; it never fails. | Inventing a spelling to get past H2. | Say what the ledger needs in the description. |
| **H10 Budget** | Header pull requests have a separate daily budget (`Held: daily limit` waits by itself). | Opening many small header PRs at once. | Batch related declarations for one class. |

## When a setting or the ledger looks wrong

Leave `// cl:` lines and SDK markers as they are. Put a **Ledger change request** section in the pull request
(or open an issue titled `Ledger: …`) with the RVA, current and requested name, owner or setting, and the
evidence (slot, caller, string, extent).
