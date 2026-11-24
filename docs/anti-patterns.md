# Anti-patterns

The checks match bytes; they cannot tell whether the source is honest. EA built the original from ordinary
source, one compiler setting per library, and the stock Visual C++ 7.1, Platform SDK, DirectX and STLport
headers. Each row below either makes a function match for the wrong reason or makes the source worse even when
it matches. Reviewers refuse them. Enforced rules are in [RULES.md](../RULES.md).

If a function only matches with one of these, the cause is elsewhere (a ledger name or owner, a shared header,
a library setting). Close the pull request with a dead-end comment (AGENTS.md step 15) and stop.

## Forcing a match

| # | Do not | Bad | Good |
| --- | --- | --- | --- |
| 1 | Change compiler settings from a source file. | `#define _CRTIMP` or `#undef _STLP_USE_NEWALLOC` at the top of a `.cpp`. | Leave settings to the library profile; a library-wide STLport switch lives in the shared header that sets it. |
| 2 | Declare a runtime or Windows function yourself. | `namespace XCRT { extern "C" __declspec(dllimport) char *_strdup(const char *); }` | `#include <string.h>` and call `_strdup`. |
| 3 | Write a stand-in SDK header. | A local `winsock.h` with three declarations. | The Platform SDK or DirectX SDK header. |
| 4 | Use `extern "C"` on game code to reach a ledger spelling. | `extern "C" void InitGame();` | C++ linkage; ask for the ledger name to be corrected. |
| 5 | Give a local body a runtime or SDK name. | `void free(void *p) { ... }` in game code. | A name from its evidence; a local `free` cannot be msvcr71's `free`. |
| 6 | Declare a game function or global inside a `.cpp`. | `extern int TheFrameCount;` in `Foo.cpp`. | Declare it once in its header and include that. |
| 7 | Include a header twice. | `#include "Common/AsciiString.h"` on two lines. | Include it once. |
| 8 | Steer code generation. | `volatile int i;`, `register`, `__assume`, `__forceinline`, a dead local kept for stack layout. | What the original most plausibly said; cite Zero Hour if it has the construct there. |
| 9 | Instantiate a template only to place a copy. | `template void Vector<int>::grow();` | The use that instantiated it; if that use is unwritten, say so beside the instantiation. |
| 10 | Rewrite a vendored library unit (STLport, zlib, Lua, GameSpy). | Hand-edit `src/stlport-4.6/...`. | Compile upstream's file unchanged. |
| 11 | Edit row size or ledger metadata to hide a mismatch. | Shrink a row's size to drop a jump table. | Bring the extent evidence to a ledger request. |

Workarounds already in the tree are not precedent; do not copy them. Remove one only where every function in
the file still matches.

## Source quality

| # | Do not | Bad | Good |
| --- | --- | --- | --- |
| 12 | Let `void *` or an integer stand in for a type the code shows. | `int getLANSlot(int i);` returning a pointer callers cast back. | `LANGameSlot *getLANSlot(int i);` |
| 13 | Invent a name or borrow one from another class. | Calling slot 12 `update` because another class's slot 12 is `update`. | A name from a string, an export, this class's own vtable or corresponding Zero Hour code; otherwise keep the placeholder. |
| 14 | Rename a field without a witnessing access. | `m_unk0x28` → `m_health` on a guess. | `m_duration` because the parser and the getter both use it as a duration; cite them. |
| 15 | Hand-write a container or library copy. | A struct with `begin`/`end`/`cap` pointers next to code that uses `std::vector`. | The real STLport or engine template (`std::vector`, `MAKE_DLINK`). |
| 16 | Define a type twice, under any name. | A flattened `RenderObjClass` Draft beside the real one. | One definition; a Draft promotion moves every includer and deletes the Draft. |
| 17 | Give an inline virtual a default body without slot evidence. | `virtual bool Create() { return false; }` when the slot reaches a fold returning true. | Read the class's own slot in `baseline/game.dat` and write what it does. |
| 18 | Write comments that are work notes. | `// matches retail now`, `// tried /O2`, `// size 0x4B`. | `// Index by input octet; the table has 256 entries.` |
| 19 | Change lines outside the item. | Reformatting a neighbouring function. | Only the claimed functions or header. |
| 20 | Follow Zero Hour where the bytes differ. | Copying Zero Hour's zero-length guard into `Coord2D::Normalize`. | Follow `baseline/game.dat`; note the difference in the pull request. |
| 21 | Keep dead code. | A string declared in a branch that never runs. | Remove it; if the match then fails, it is a dead end (AGENTS.md step 15). |
| 22 | Use a different type for one global in two places. | `int g_x;` in one file, `unsigned g_x;` declared in another. | One declaration in the header, matching the definition. |

## When it does not match

| # | Do not | Bad | Good |
| --- | --- | --- | --- |
| 23 | Shuffle statements when the vtable pointers differ. | Reordering stores in a constructor whose vtable store differs. | Compare each slot with the reference; fix the missing override or its signature (`const`, parameter types). |
| 24 | Write a destructor the original left implicit. | `~FooModuleData() {}` that adds an early vtable store. | Leave it implicit when the implicit one accounts for the reference. |
| 25 | Call a same-unit helper through a public declaration. | A helper declared in a header whose reference callers pass arguments in registers. | `static` file-local linkage, with its callers in the same unit. |
| 26 | Rewrite floating-point maths into an equivalent form. | `x / len` where the reference multiplies by `1.0f / len`. | The reference's intermediate types and expression order. |

## Facts that remove the need for a workaround

Under `/MD` the headers already declare most runtime functions `dllimport`. `memcpy`, `memset`, `memcmp`,
`strcpy`, `strcat`, `strcmp`, `strlen` and the math intrinsics are not; when not inlined they go through a
`jmp [import]` thunk. Never choose the call shape by hand.

msvcr71 exports some functions under two names; the checker treats them as one. Write the left spelling:
`strcpy`=`_mbscpy`, `strcat`=`_mbscat`, `_strdup`=`_mbsdup`, `_stricmp`=`_strcmpi`, `labs`=`abs`,
`getc`=`fgetc`, `putc`=`fputc`.

Placeholder spellings say what is unknown: `Rva00261513Filter` (class), `rva0028C15E()` (function),
`vfunc_0x58`/`slot27` (vtable entry), `m_unk0x28` (member at offset), `g_Va00E0333C` (global at VA),
`src/unk/O<rva>.cpp` (file with unknown name), `include/Draft/` (provisional layout). Replacing one with an
evidenced name is a contribution of its own.

Finished files to imitate: `src/Code/GameEngine/Source/GameNetwork/FrameDataManager.cpp`,
`src/Code/GameEngine/Source/GameNetwork/FrameData.cpp`, `src/Code/Libraries/Source/WWVegas/WWLib/hash.cpp`,
`src/Code/Libraries/Source/WWVegas/WWMath/cullsys.cpp`, `src/Code/GameEngine/Source/GameClient/DisplayString.cpp`.
