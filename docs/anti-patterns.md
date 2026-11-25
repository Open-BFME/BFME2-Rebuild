# Matching anti-patterns

EA built the original from ordinary source with one compiler setting per
library and the stock Visual C++ 7.1, Platform SDK, DirectX and STLport
headers. Each construct below can make a function match for the wrong reason.
The match then says nothing about the source, and the workaround spreads to
every file that copies it.

If a function only matches with one of them, the cause is elsewhere: a ledger
name or owner, a shared header, a library setting or the checker. Record the
case in [hard_rows.md](hard_rows.md), or file a ledger request with the
evidence, and stop.

| # | Do not | Instead |
|---|---|---|
| 1 | Change compiler settings in a source file: a `// cl:` line, or `#define`/`#undef` of `_CRTIMP`, `_STLP_*`, the project's `_BFME_*`/`_STLP_BFME_*` switches, `_DLL`, `_MT` or a runtime header guard such as `_INC_STDLIB`. | Leave settings to the library profile, so every unit in a library sees the same flags and the same STLport. A library-wide STLport setting belongs in the shared header that sets it, as Zero Hour's `PreRTS.h` sets `_STLP_USE_NEWALLOC`. |
| 2 | Declare a runtime or Windows function yourself: `__declspec(dllimport)`, `_CRTIMP`, `WINBASEAPI`, an `__imp_` name, a `namespace XCRT { extern "C" ... }` wrapper, `#define _strdup ...`. | Include `<string.h>`, `<stdlib.h>`, `<math.h>` or `<windows.h>` and call the function by its own name. |
| 3 | Write a stand-in SDK header (`windows.h`, `winsock.h`, a D3DX declaration). | Use the Platform SDK or DirectX SDK header. |
| 4 | Use `extern "C"` on game code to reach a ledger spelling. | Use C++ linkage and ask for the ledger name to be corrected. `extern "C"` is for a definition in a `.c` file, for wrapping a C header, and for a runtime hook the game replaces, such as `_purecall`. |
| 5 | Give a local body the name of a runtime or SDK function. | Name it from its evidence. A local `free` linked next to msvcr71's `free` fails with LNK2005, so it cannot be that function. |
| 6 | Declare a game function or global inside a `.cpp` file. | Declare it once, in its header, and include that header. |
| 7 | Include the same header twice in one file. | Include it once. Zero Hour has some repeated includes; they change nothing, and adding one never helps a match. |
| 8 | Steer code generation: `volatile` on a local, a reference that aliases another variable, `__declspec(nothrow)` or `noinline`, `__forceinline`, `register`, `__assume`, a dummy statement. | Write what the original most plausibly said. If Zero Hour has the construct at that place, cite it. |
| 9 | Explicitly instantiate a template (`template void X<T>::f();`) only to place a copy in an object. | Prefer the use that instantiated it in the original. An instantiation can stand in while that use is unwritten; say so next to it and remove it when the use lands. A template the original exports is evidenced by the export table. Zero Hour's game code has none. |
| 10 | Rewrite a vendored library unit (STLport, zlib, Lua, GameSpy). | Compile upstream's file unchanged with the library's own flags. |

The source rules (G3) refuse rows 1, 2, 4, 5 and 7 and `__declspec(nothrow)` when
a pull request adds them. Row 4 allows `extern "C"` around `#include` lines, the
`#ifdef __cplusplus` form of a header that C code includes too, and a name the
ledger places on a C object, a prebuilt library or an import. G2 already refuses changes to `// cl:` lines. The
other rows are review rules; see [RULES.md](../RULES.md).

## What the compiler already does

Visual C++ 7.1 decides which calls go through the import table. Under `/MD`
its headers declare most runtime functions `dllimport`, so a call is
`call [import]`. They declare `memcpy`, `memset`, `memcmp`, `strcpy`,
`strcat`, `strcmp`, `strlen` and the math intrinsics (`fabs`, `sin`, `cos`,
`sqrt`, `atan2` and others) without it. When the compiler does not inline
one of those, the call goes through a `jmp [import]` thunk even under `/MD`.
A file never needs to choose the call shape by hand.

msvcr71 exports some functions under two names at one address, and the
original's import table uses one spelling for each:

| Source spelling | Import name in the original |
|---|---|
| `strcpy` | `_mbscpy` |
| `strcat` | `_mbscat` |
| `_strdup` | `_mbsdup` |
| `_stricmp` | `_strcmpi` |
| `labs` | `abs` |
| `getc` | `fgetc` |
| `putc` | `fputc` |

A thunk call labelled `_mbscpy` is a plain `strcpy` in the source. The checker
treats the two spellings as one function (`import_aliases.py`).

A library whose `_CRTIMP` functions (`sprintf`, `atoi` and the like) are only
ever reached through thunks was built without `/MD`. In the original that is
true of Apt, FESL and DirtySock. Whether they used `/MT` or `/ML` is not yet
shown. Everything else uses `/MD`.

## Per-file settings

EA set compiler flags per project. Zero Hour's `.dsp` files carry almost no
per-file code-generation settings: one `/GX` on `w3d_dep.cpp` in WW3D2, the
rest being warning and precompiled-header settings. Of GameEngine's 666 units
on 2026-11-24, 49 ask for `/EHs`, 5 turn exceptions off, 3 ask for `/O2`, 3 for
`/Ob2`, 3 for `/Oy-`, 2 for `/Oi` and 2 for `/G6`. Treat each as a hypothesis
to test against the binary, not as a setting EA used.

## Workarounds already in the tree

Counted on 2026-11-24, outside the vendored libraries. The source rules stop
these from growing; removing them is tracked in issue #687.

| Construct | Occurrences | Files |
|---|---|---|
| `/D_CRTIMP=` in a `// cl:` line | 249 | 247 |
| STLport switches in a `// cl:` line | 257 | 255 |
| `#define`/`#undef` of a library setting | 77 | 22 |
| Hand import declaration (`dllimport`, `_CRTIMP`, `WINBASEAPI`) | 214 | 68 |
| Hand declaration of a runtime or Windows function | 286 | 95 |
| `__imp_` name in code | 17 | 5 |
| Macro rename of a runtime function | 13 | 6 |
| Local body with a runtime name (`free`, `strdup`) | 2 | 2 |
| Header included more than once (every copy counted) | 516 | 107 |
| Explicit template instantiation | 113 | 22 |
| `__declspec(nothrow)` | 7 | 6 |

Removing one of them can change what a function compiles to. Remove them with
the check, file by file, and only where every function in the file still
matches.
