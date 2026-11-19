# STLport 4.6 library sources as retail's `game.dat` links them

| | |
|---|---|
| Release | STLport 4.6 (`_STLPORT_VERSION 0x460`, October 2003) |
| Source URL | https://sourceforge.net/projects/stlport/files/STLport%20archive/STLport%204/STLport-4.6.tar.gz |
| Tarball SHA-256 | `744dd31ac34246bdba4ef50f79b4d936c842a3e4f13915a86cc06f7c7cf84f60` (gzip header dated 2003-10-31) |
| Licence | the STLport licence (HP, SGI, Moscow Center for SPARC Technology, Boris Fomitchev), in `README` here and in each file's own header; the notices are kept |

## What is here

The fifteen headers of upstream's `src/` directory (`stlport_prefix.h`, `c_locale.h`, `locale_impl.h`,
`fstream_impl.h`, `num_put.h`, ...), which the library sources include by name and which the
`stlport/` tree the checker overlays (`// stlport46`) does not carry. Their text is the release's own; three
of them (`msvc_warning_disablers.h`, `uint64.h`, `vc_warning_disable.h`) ship with CRLF line ends and are
normalised to LF by the repository. Compile the library units with `-Iinclude/upstream/stlport-4.6`.

`c_locale_win32/c_locale_win32.c` is upstream's file of that name, unmodified, which retail's `src/c_locale.c`
object `#include`s (the unit under `src/stlport/` carries upstream's `c_locale.c` as its payload and rows 84 of
them). `shims/windows.h` declares the kernel32 file, mapping and national-language calls, constants and structures
that `fstream.cpp` and `c_locale_win32.c` use, with the documented calling convention and layout; no Platform SDK
header is used or reproduced (retail's import table names the `lstrcmpiA` call's kernel32 export `lstrcmpi`, which
the shim reproduces).

The library sources themselves are under `src/stlport-4.6/STLport-4.6/src/`, at the object names the map gives
(`time_facets.cpp`, `num_get.cpp`, `num_get_float.cpp`, `num_put_float.cpp`, `strstream.cpp`, ...); each is
upstream's file of that name with the `// FUNCTION:` rows placed above the definitions they annotate.

## Which units are pristine and which are altered

Pristine payloads (`// UPSTREAM:` line with the payload's SHA-256, then upstream's file byte for byte): `num_get.cpp`,
`num_put_float.cpp`, `strstream.cpp`, `time_facets.cpp`, `c_locale.c` (under the object `src/stlport/.../c_locale_win32.c`).
Units with a marked alteration (the file says so at the place): `num_get_float.cpp` (a dead `#if 0 // def __ICL` block that holds
a compiler-optimisation pragma is removed, because the checker bans the construct), `fstream.cpp` (the body of `struct ioinfo`
sits in `shims/ioinfo.h` because struct bodies are kept out of `.cpp` files), `ios.cpp` (`ios_base::~ios_base` is not defined:
retail frees through the game's own `free` 0x00030830, which needs the game-allocator overlay). `iostream.cpp` and
`locale_impl.cpp` are upstream's text unchanged, in the project's flags (`/GR- /EHsc- /D_STLP_NO_EXCEPTIONS /D_STLP_USE_MALLOC`).
Once `stlport-4.6` is registered as a pristine release the two structural alterations go away (the pristine path accepts
upstream's text as it is).

## Why 4.6 and not 4.6.1 or 4.6.2

The three releases differ in `src/iostream.cpp`, `src/locale_impl.cpp` and `src/locale_impl.h` only (and in
headers that do not change VC 7.1 code). Retail's bodies of the functions that differ are 4.6's (target facts,
read from `game.dat` 1.06):

- `ios_base::Init::Init` 0x00016AA0 tests `_S_count == 0` before calling `_S_initialize`, and
  `ios_base::Init::~Init` 0x00015E70 tests `> 0`; 4.6.1 and later call unconditionally.
- `locale::~locale` 0x00007150 is 7 bytes, `_M_impl->decr()` with no null check; 4.6.1 added the check.
- `locale::_S_initialize` 0x0000BAC0 returns on `count > 0` and increments the counter last.

Measured by the earlier decomp's `tools/stlport_trial.py library` at the pinned VC 7.1 toolchain (a donor-side
measurement of this release's text, not a fact about retail): 4.6 reproduced 298 ledger rows against 291 for
4.6.1 and for 4.6.2. In this tree the units below reproduce retail's bytes from upstream's text with no edit:
`time_facets.cpp`, `num_get.cpp`, `num_get_float.cpp`, `num_put_float.cpp`, `strstream.cpp`,
`c_locale.c` (with its win32 file).

## Compile profile that reproduces retail

Upstream's `src/vc_common.mak` release-static flags: `cl -nologo -c -O2 -GR -EHsc -MD -DNDEBUG -DWIN32
-D_WINDOWS -D_MBCS -D_STLP_USE_STATIC_LIB -D_STLP_NO_FORCE_INSTANTIATE`, plus `-G6` (retail's `inc` against the
default's `add reg,1`) and no SSE (`// cl-arch: x87`): the library was built for plain x86 in 2003-2004. A unit
whose rows call the library's out-of-line `__copy_trivial` adds `-D_STLP_BFME_COPY_TRIVIAL_EXTERN` (the
BFME patch layer's P1: retail calls the library's own out-of-line copy).

## Header hashes (equal to the tarball's `src/` files)

The release's own `SHA256SUMS` record (531 payload files, kept with the owner's vendored copy); the three CRLF
headers above differ from it only in line ends.

| file | SHA-256 |
|---|---|
| `README` | `75dc082b3fa743c9802586ba87ae64d096bedcd31e6f92d5652be87da85b5385` |
| `aligned_buffer.h` | `4ee23f39c5d595b6c77a4dec55936d7a904c26e1ac452505eeb2c3b324341f65` |
| `c_locale.h` | `ea67cdd040bd9d18b030e0da9dae8ca567c2b7b50fd854d081b562e2ac5ec25e` |
| `complex_impl.h` | `4c2123cd1c72d5ad47602ae1fe21d284c83a7cd260a4f5273b468b7cb95de928` |
| `fstream_impl.h` | `bb5af46b9c86efcb6e91e51a7aedd013f57c7fd7503acd456d329406b75d1ce5` |
| `locale_impl.h` | `37dbda19f503b44153d1c623ad7f1eb698938d9597abb9136c85d56848ff6132` |
| `locale_nonclassic.h` | `da9722387391e8e3096e6fb02d2be9cbe88d42ffef80bdea500532810b5ecb98` |
| `message_facets.h` | `689c6b3962f3b58451a63ab8c4624d0175a0cabb32139bc852df35ffb929f54d` |
| `msvc_warning_disablers.h` | `7f6b6d8fe61a34896bf584c007faf3fe70b568e47c2e07662966aec3b2e346a9` |
| `mwerks_debug_prefix.h` | `b9f4de58c6a7ce0468805a3d51eae0aba3f05c46ebde7eb68207ed49d4bdecbf` |
| `mwerks_prefix.h` | `dd41185551bff5dd899baf3cf62e6255b4dccef57a1bcf2cd33f5c63903265b2` |
| `mwerks_x86_prefix.h` | `a2b9e131423b624ab3a6b6c44978c296c29d6e29788954626fa41d65f7bf6fa8` |
| `num_put.h` | `f8cb7a6e880c78a8db41404788dc8c7c062bb3506d0301a34c158f5ff7f443d1` |
| `stlport_prefix.h` | `114aee5496e73b8cdd1481b7f5450cc6450beb28ab557c4a88a8dd4077891d7b` |
| `uint64.h` | `e586ce6d7dcd8d28d886b584cacc9fb8af4bd183dd775ceffee9a42644a24869` |
| `vc_warning_disable.h` | `c41220ecd377a385a0c87be35e707c9dd7a010e3bc57c022198610d22b8c69df` |
| `c_locale_win32/c_locale_win32.c` | `b528fc3da4c2d092ad2e53bc70c734ff01543253e6dc8a06c993f56d2c7b36ff` |
