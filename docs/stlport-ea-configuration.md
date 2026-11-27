# STLport 4.6 as Electronic Arts built it

`vendor/stlport-4.6/` holds the STLport 4.6 release unmodified, plus the changes the
retail program shows Electronic Arts made. This page lists each way the current
toolchain differs from the release, the evidence in `game.dat` 1.06, and what
`vendor/stlport-4.6/` does about it. Addresses are virtual addresses (`VA = RVA + 0x400000`).
The ledger (`config/map.json`) records RVAs.

## The changes EA made (applied in `vendor/stlport-4.6/stlport/`)

### Containers allocate from the game heap

`allocator<T>::allocate(n)` calls `0x4307F0(n * sizeof(T), 0)`. That function is
`MemoryPool::_Allocate(size, 3, heap)` through the pointer at `0xDE0404`
(`allocateBlock`, `GameMemory.cpp`). `allocator<T>::deallocate(p, n)` calls
`0x430830(p)`, which is `_Free(p, 3)` through `0xDE03FC` (`freeBlock`).

- Retail `allocator<unsigned char>::allocate` at `0x4073E0` is `n ? 0x4307F0(n, 0) : 0`, with no
  out-of-memory retry. 332 of the 446 calls to `0x4307F0` are in STLport code, 130 of them in
  `_M_create_node`.
- No 4.6 configuration produces this. `_STLP_USE_MALLOC` gives `__malloc_alloc<0>::allocate`,
  which calls the C runtime's `malloc` and loops on `_S_oom_malloc`. Retail has that body
  as well, at `0x425090` (calling msvcr71's `malloc` through the import table, with the retry at
  `0x424200`), because `dll_main.cpp` instantiates it explicitly, but no container calls it.
  `_STLP_USE_NEWALLOC` gives `operator new` and `operator delete`
  (`0x42FDA0`/`0x42FD60`, AllocType 1, not 3). The release's default, `__node_alloc`, is in the program too
  (`0x423D80`, `0x427A90`, explicit instantiations of the library), and no container calls it either.
- The library itself was built this way. `collate.cpp`'s `allocator<unsigned short>::allocate`
  (`0x417710`) calls `0x4307F0(2n, 0)`. Compiled from the release's `collate.cpp`, it matches
  retail only with this change. So the choice is library-wide configuration, not something game
  units set one by one.
- Zero Hour shows the switch EA used. `GameEngine/Include/Precompiled/PreRTS.h` and
  `Common/STLTypedefs.h` define `_STLP_USE_NEWALLOC` and, commented out next to it,
  `_STLP_USE_CUSTOM_NEWALLOC STLSpecialAlloc`. `STLSpecialAlloc` is a class with static
  `allocate(size_t)` and `deallocate(void *, size_t)` (`GameMemory.h`). STLport 4.6 has no
  `_STLP_USE_CUSTOM_NEWALLOC`, so EA's copy of `stl/_alloc.h` carried it.

`stl/_alloc.h` gains that branch (`__sgi_alloc` is the named class), and
`stl_user_config.h` defines `STLSpecialAlloc` with inline members that call
`allocateBlock(n, 0)` and `freeBlock(p)` and selects it.

### The library is linked statically

`game.dat` contains the library objects (`dll_main.cpp` through `strstream.cpp`, rows from
`0x406F80`) and imports no STLport DLL. With `/MD` and no `_STLP_USE_STATIC_LIB`, 4.6 declares its
classes `__declspec(dllimport)`. `stl_user_config.h` defines `_STLP_USE_STATIC_LIB`, as upstream's
static-library makefile does.

### STLport's try blocks are compiled out

All 123 retail `_M_create_node` bodies have no exception frame. That includes trees and lists of
`AsciiString` (`0x42C552`, in `INI.cpp`), whose copy constructor can throw, so 4.6's
`_STLP_TRY { _Construct(...) } _STLP_UNWIND(deallocate)` would give them one under `/EHsc`.
Game code itself keeps its frames: its own destructors register unwind states, and
`vector::_M_fill_insert` bodies with a local copy of the value keep theirs. `stl_user_config.h`
defines `_STLP_NO_EXCEPTIONS`, which 121 game units set today on their own. The measurement below
is what decided it: with it, no sampled function stops matching; without it, four do.

## Patch-layer and per-file changes that are not EA's

| Current change | Where it is enabled | What retail shows | Verdict |
|---|---|---|---|
| `_tree.c`: `insert_unique` begin path calls `_M_insert(__x, __y, __v)` | always (base patch) | `_Rb_tree::insert_unique` `0x473053` calls `_M_insert(y, y, v, 0)`, the release's text | remove: the release is retail |
| `_BFME_RETAIL_TREE_INSERT_LAYOUT` (restores the release's branch, reordered) | 216 `// cl:` lines and the judges' default lines | as above | remove with the patch |
| `__copy_trivial`/`__copy_trivial_backward` declared out of line, `throw()` | `_STLP_BFME_COPY_TRIVIAL_EXTERN` (119 files, `DockUpdate.cpp` `#define`) | `0x4179B0` is the release's inline `__copy_trivial`, kept out of line by the compiler in `numpunct.cpp` (its `wstring` statics). Compiling the release's `numpunct.cpp` with upstream's static-library flags gives the same 41 bytes. The copy sits among that object's rows (`0x417950`-`0x417B5F`). The linker kept the first select-any copy. `0x620840` is the same for `__copy_trivial_backward` in `assetmanager_impl.cpp` | remove: an ordinary COMDAT, no EA change. The per-file copies are discarded duplicates (#683) |
| `_Rb_tree_node_base::_S_minimum/_S_maximum` forced inline, `#pragma optimize` | `_STLP_BFME_TREE_NODE_BASE_O2`, `_STLP_BFME_RB_MINMAX_IMPORT` | `0x4237F0`/`0x423810` are `dll_main.cpp`'s select-any copies (from `_Rb_global<bool>`) | remove (no unit sets either) |
| `__lower_bound<size_t>` declared, not defined | `_STLP_BFME_LOWER_BOUND_EXTERN` | `0x968D2F` is one kept COMDAT | remove (no unit sets it) |
| `_List_iterator_base::operator!=` out of line | `_STLP_BFME_LIST_NE_EXTERN` | `0xACFE40` is one kept COMDAT | remove (no unit sets it) |
| `list::resize(size_type, const _Tp&)` | always (base patch) | no `list::resize` row in the ledger | remove: no evidence |
| `_List_node` 32-byte tail | `BFME_PARTICLE_LIST_NODE_TAIL` | none | remove (no unit sets it) |
| `allocator<T>` through a declared `allocator<char>::allocate(n, 0)` and `::free` | `bfmealloc_alloconly` shim (88 markers, 41 `-I` flags) | the game heap, see above | replaced by `_STLP_USE_CUSTOM_NEWALLOC`, now for every unit and the library |
| forced-inline copy and fill helpers, declared `__copy_trivial` | `bfmealloc` shim | as for `__copy_trivial` | remove (no unit uses the shim) |
| `hashtable::_M_new_node` forced inline, try block dropped | `bfmehashtable` shim, `BFME_HASHTABLE_NEW_NODE_NOFORCEINLINE` | the try block is gone if `_STLP_NO_EXCEPTIONS` is set | remove (no unit uses the shim) |
| `_List_iterator_base` members under `#pragma optimize`, `list<int>::_M_create_node` `noinline` | `bfmelist` shim | none beyond the kept COMDATs | remove (no unit uses the shim) |
| `_STLP_USE_MALLOC` | 176 `// cl:` lines, `DockUpdate.cpp`, the judges' default lines | containers never reach `__malloc_alloc` (above) | remove: replaced by the game-heap allocator |
| `_STLP_NO_FORCE_INSTANTIATE` | 13 game `// cl:` lines | an upstream makefile flag for building the library | remove from game units; library units keep upstream's flags |
| `c_locale_win32.c`: private `__FixGrouping` removed, `_BFME_LOCALE_SPLIT_PROVIDERS` | the locale library unit | library-unit placement (#686) | not header configuration; left to the library units |

## Measured on game units

A local VC 7.1 judge (`tools/local_check`, the Boat image) compiled 49 game units, covering each of
the 27 combinations of per-file STLport switches on main (01df801e) plus unmarked units, which get
the judges' default lines. Each unit was compiled two ways: as on main, and with every per-file
STLport switch removed (`// cl:` tokens, `// bfmealloc_alloconly`, `DockUpdate.cpp`'s `#define`s)
against `vendor/stlport-4.6/stlport` alone, with no patch layer and no shim. 865 ledger functions:

| Configuration | PASS | lost vs main | gained |
|---|---:|---:|---:|
| main (patch layer, per-file switches) | 408 | | |
| release + game heap + static library | 399 | 14 | 5 |
| release + game heap + static library + `_STLP_NO_EXCEPTIONS` (this tree) | 404 | 10 | 6 |
| the same, with `O00035A50.cpp` calling `allocateBlock` | 416 | 0 | 8 |

- The ten functions lost in the third row are all in `src/unk/O00035A50.cpp`, which stops
  compiling. It calls the shim's static `_STL::allocator<char>::allocate(n, (const void *)'str')`.
  Retail passes `'str'` as the heap argument of `0x4307F0`, so the call is `allocateBlock(n, 'str')`.
  With that seven-line change the unit goes from 10/12 to 12/12.
- Gains: `TunnelTracker.cpp` 9 to 14 of 20, `TransitionDamageFX.cpp` 13 to 14 of 23,
  `O00035A50.cpp` 10 to 12 of 12.
- Without `_STLP_NO_EXCEPTIONS`, `ProductionQueueHordeContain.cpp` loses 2 and `htree.cpp` loses 2.

The sample is not the tree. Units move to this configuration only after a whole-tree run shows
that nothing that matches stops matching. Two things the sample does not cover: the library units
under `src/stlport-4.6/`, which keep upstream's makefile flags, and the select-any duplicates
discussed in #683. A unit compiled without `_STLP_BFME_COPY_TRIVIAL_EXTERN` emits its own
`__copy_trivial`, which the linker discards in favour of `numpunct.cpp`'s.
