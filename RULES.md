# Source rules

These checks run on pull requests to main. A refusal gives a rule number,
a file and line, and a way to fix the change.

| Rule | What to do | Bad example | Fix |
| --- | --- | --- | --- |
| **G1 Paths** | Edit existing C/C++ source files and eligible headers, add permanent headers or remove Draft headers, and stay within 60 paths, 1 MiB per file and 8 MiB of changed source. | Add a new source file for a recovered function. | Restore the original source arrangement and edit the existing owning file. |
| **G2 Annotations** | Preserve the spelling, order, and associated definitions of generated annotations. | Move a `// FUNCTION:` record above another body. | Put the original record back above its own definition. |
| **G3 Constructs** | Use typed C or C++ and declared symbols within the approved pragma and assembly exceptions. | Introduce `__asm nop`. | Express the operation in ordinary C or C++ and use the declared symbol. |
| **G3 Workarounds** | Get runtime and SDK functions from their headers, and leave runtime and STLport settings to the library profile. Refused when added outside the vendored libraries: `__declspec(dllimport)`, `_CRTIMP` or `WINBASEAPI` on a declaration, an `__imp_` name, a declaration with C linkage, a calling convention or an import of a runtime or Windows function, a body with such a name, a `#define` or `#undef` of `_CRTIMP`, `_DLL`, `_MT`, a runtime header guard or the project's `_BFME_*` switches anywhere, of `_STLP_*` or a runtime function name in a source file, a header included twice on one branch path, `__declspec(nothrow)`, and `extern "C"` on anything but a C header, a runtime or SDK function, or a name the ledger places on a C object. | Add `namespace ArmorCRT { extern "C" __declspec(dllimport) int __cdecl _strcmpi(const char *, const char *); }`. | Include `<string.h>` and call `_strcmpi`. If the call still differs, the file's setting or the ledger is wrong: file a ledger request with the evidence. See [docs/anti-patterns.md](docs/anti-patterns.md). |
| **G4 Types** | Keep each measured stand-in count at or below its previous total across the changed files. | Add a `void *` return for a known `File` pointer. | Declare `File *` and let callers use the typed pointer directly. |
| **G4b Type swaps - advisory** | Keep the evidenced type when editing a declaration, especially a pointer or class field that could be replaced by an integer of the same size. | Change `File *m_file;` to `unsigned int m_file;`. | Keep `File *m_file;` unless the code and layout evidence establish a different type. |
| **G5 Comments** | Use new comments to explain evidence or intent and put private work notes in the pull request description. | Leave a local path or an experiment score in a source comment. | Cite the useful evidence in the comment and move the work note to the description. |
| **G6 Scope** | Keep substantive edits to at most three source files, with a narrow exception for updating includers and type uses during a Draft promotion. | Recover four unrelated units in one pull request. | Split the independent changes into separate pull requests. |
| **G7 Definitions** | Put new class, struct and union definitions in shared headers and keep one definition per named type or externally linked object. | Add a second `struct FileInfo` to a private header. | Include the existing shared definition. |
| **G8 Names** | Introduce address-shaped names only when the base ledger accepts that exact spelling. | Add `sub_00401234` with no accepted name. | Use an evidence-backed name or ask a maintainer to record the accepted spelling. |

Keep compiler directives such as `// cl:` and SDK markers unchanged.
If a different setting is needed, file a ledger request with the file, the requested setting, and the evidence for it.

G4b appears in the summary for review. It has no status and does not
affect the source rules result.

The source review uses evidence to decide naming, readability, type
identity, and original module ownership. See
[Source quality](docs/source-quality.md) for what to cite.
