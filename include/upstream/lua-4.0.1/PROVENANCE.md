# Lua 4.0.1 as retail's `game.dat` links it

| | |
|---|---|
| Release | Lua 4.0.1 (TeCGraf, PUC-Rio), `LUA_VERSION "Lua 4.0.1"` in `include/lua.h` |
| Source URL | https://www.lua.org/ftp/lua-4.0.1.tar.gz |
| Tarball SHA-256 | `df746e149cf6939e90009d2e540eee918d585b4d1bc6d68b19316a050d484d2a` |
| Tarball MD5 | `a31d963dbdf727f9b34eee1e0d29132c` |
| Tarball size | 158426 bytes |
| Downloaded | 2026-11-17 (agent v01); hashes computed on the downloaded file and equal to the checker's `libraries/releases.json` |
| Licence | the permissive grant at the end of `include/lua.h` and in `COPYRIGHT`, reproduced by the files here unchanged |

## What is here

Every file under this directory is byte-identical to the file of the same path in the tarball
(`lua-4.0.1/include/*.h`, `lua-4.0.1/src/*.h`, `COPYRIGHT`, `README`). The `.c` files are under
`src/lua-4.0.1/Lua/`; each starts with a prefix (`// cl-profile: upstream`, `// cl: ...`,
`// UPSTREAM: lua-4.0.1; pristine payload SHA256 ...`, then the `// FUNCTION:` lines the
checker needs) and then the upstream file, byte-identical. The SHA-256 in the prefix is of the part
after the prefix and is checked by the landing gate against the release's file hashes.

The headers are not included with an absolute path: compile with
`-Iinclude/upstream/lua-4.0.1/include -Iinclude/upstream/lua-4.0.1/src`. The public API
(`lua.h`, `lauxlib.h`, `lualib.h`, `luadebug.h`) is in `include/`; the interpreter's internals
(`lstate.h`, `lobject.h`, `lopcodes.h`, ...) are in `src/`. The headers carry no `extern "C"`
block: a C++ file must wrap its `#include "lua.h"` in one, or the calls mangle instead of binding
retail's `_lua_*` cdecl symbols.

## Not imported

`src/lua/lua.c`, `src/luac/*`, `src/ltests.c`, `etc/*`, `test/*`, `doc/*`, `config`, `Makefile`s and
the tarball's other support files: retail links none of them.

## Retail's Lua is not stock 4.0.1

The files here are stock. Retail links a fork (Lua 4.0.1 plus a boolean type, sized tables and a few other changes), so
the sources that reproduce retail are altered versions of these. They are kept apart, marked as altered as the licence
requires, in `include/upstream/lua-4.0.1-bfme/` (headers) and `src/lua-4.0.1/Lua/` (sources), with one provenance row per
altered file and one evidence row per hunk in `include/upstream/lua-4.0.1-bfme/PROVENANCE.md`. Nothing here was changed
for that; the release originals of the altered `.c` files were added below, byte-identical, so the difference can be diffed.

## Compile profile that reproduces retail

`cl -nologo -c -Gy -O2 -GR- -EHsc- /MD` plus, ahead of these paths, the altered headers
(`-Iinclude/upstream/lua-4.0.1-bfme/src -Iinclude/upstream/lua-4.0.1-bfme/include -Iinclude/upstream/lua-4.0.1/include -Iinclude/upstream/lua-4.0.1/src`;
`// cl-profile: upstream` plus the `// cl:` line). `/MD` matters: retail linked the DLL C runtime, so
libc calls are `__imp__` indirections. No `LUA_NUM_TYPE`, `LUA_DEBUG` or other define is needed.

## File hashes

Headers and text files (equal to the tarball's):

| file | SHA-256 |
|---|---|
| `include/lauxlib.h` | `67bb2e7a372e428571975e8ff6d087f022da76e531e3bccdb4336c3934a8c60c` |
| `include/lua.h` | `ecf14d8b6eb040d337ac52d4b8b39d84e3e0a95afd414206825fd07ad14220e7` |
| `include/luadebug.h` | `c039bd07a93654ffe9ef0f59de46393e773a19b413efa4b7d6a225db2e8cf72a` |
| `include/lualib.h` | `4dde9c616be6ce7de5e29ca108583cef963cf73020c0a3ab487ef7f2387b92d8` |
| `src/lapi.h` | `bec7a4282df1dd51c772b6edfbb0e29e5e01632398ae31d0878789143dff28c3` |
| `src/lcode.h` | `71169a0566560a2ec51917ed899ebca3393f2488eabe5293b6e1afd78802bc5e` |
| `src/ldebug.h` | `531d63d4f3729e80e123707c2a97ba08ca735ba3569751846f67d1619d3fe301` |
| `src/ldo.h` | `562ab0da32295bd71e49fd91c06d762c6bb152e2399801197e014eb49697bcec` |
| `src/lfunc.h` | `80c05efca4c94750f2e8e815669b59ed75b84b19933d24285509dd58f2ac0d06` |
| `src/lgc.h` | `70059721e5fee18bbc289cabace35dcbc8c72fa2147dad11cf0198a6ea9fbeaa` |
| `src/llex.h` | `73aee22a5c454465ac4e2970e4618ddbca9a47370173513f13022e94e5d38eeb` |
| `src/llimits.h` | `22cf123c846abe6180a53cb14ee52cc92d10f594da1c91dbb0f12960d1f75c68` |
| `src/lmem.h` | `11612383c7d6b26d7d38564445a5f5ed80e6f2344ebc7cc7cacce7e76766e8ba` |
| `src/lobject.h` | `e051401d75cf5793a55eba62c768beec8a401bb0c671ca02bd4d412dd765eb0c` |
| `src/lopcodes.h` | `75a5ed87b6caa25542cb90ff8f8000e551e94501fec75dbb716d3e3fac0aeb79` |
| `src/lparser.h` | `76e1c4e4b44bb1e14984cba9440238f997fc7334ff53d64ff700684cfb1c7017` |
| `src/lstate.h` | `5282ee333b426c7442af3a73108526e5924fcea111407a2bb2ec58c41f7dd47e` |
| `src/lstring.h` | `4feb6e39dc41d847173929885e1a895d380253b468ab2b093b05768555b1c1c8` |
| `src/ltable.h` | `5f7143074f521d808b1a7b7ea8113ea92fbe9a6685e25eb2226a54c9d97cc978` |
| `src/ltm.h` | `ef399ca5af756aec4cc701b52cac5afb1f0d265917c54a9817b2ee20df89f9d1` |
| `src/lundump.h` | `5ab39d5cc7d768835cea8d1dc075be7a729e2de6bf6c2bd0b054928c9283e89f` |
| `src/lvm.h` | `6aaeffc3d2e5e52ee2146a95278ec5a15cad147bde4f486291f648fac5896e21` |
| `src/lzio.h` | `be259449bd1fb99b1cb9b5dbfcc5a0f6afbf27e22836e76429f4aa8cd4c29e07` |
| `COPYRIGHT` | `c1cfb9b1dfefcd570ae07258045f0929ea4500bc9c215d69bcfabf9a665564da` |
| `README` | `c84cf263c04916eb387ab1cc02fc97a02c7113b1e9a320f03ba87fbb622cbea4` |

Source payloads (the bytes after each `.c` file's prefix; equal to the tarball's file of that name):

| file | SHA-256 |
|---|---|
| `src/lapi.c` | `7de4e815bf408b8916a71f785aa3aaac047e16edac047cd263eeeeef3ae9d841` |
| `src/lcode.c` | `64d35eb1c7a5b02b2c0c8aeb9f15050e7a7056b37d15f7a8826fce2d96bb9131` |
| `src/ldebug.c` | `98094b921dbae78c1ecd72943570fe96ae31a633e13f4005e5d81647a1427f82` |
| `src/ldo.c` | `b58e271a20fcd38fa41557fa08fd7dcd96d56251fe7828a7d8380a8330ed357d` |
| `src/lfunc.c` | `93270ee0b4b1aeb0950ccd9edb88e6be9e73ea029814625f04ad5f67c51aa709` |
| `src/lgc.c` | `cc54c5fe52fac83ef63c97a0b472de410b1f66d659e93c29408af30d979b5114` |
| `src/lib/lauxlib.c` | `ecfcf916a21a7f7ea4f576afbe008f36ee6c230bcfc3b4dc17d906ef57d501b2` |
| `src/lib/lbaselib.c` | `beae773b781aacea0eb36a9460a277288ef8e097f4160aa355daebd45994a3c4` |
| `src/lib/ldblib.c` | `41345c982f8a2f440e256170663b0adacf779ddabf5662e257667b0ed4c2f381` |
| `src/lib/liolib.c` | `fe1cef02d273c2ea3bc193dc5669d4bf65356ba0be107b927e141b6dcebc2519` |
| `src/lib/lmathlib.c` | `8da5398f26e7108cd69042dd4d14960f4289f19e1d815de3049fd4bd7f92c16a` |
| `src/lib/lstrlib.c` | `8ea27e8338014b02f6601e1911162f21937ba6936d231dbc126be55b0dd06765` |
| `src/llex.c` | `b947617e3adeb104ea90c28354d7ef80abfe6f577efa1972d56675fd80890077` |
| `src/lmem.c` | `d8499b502680054b382425840e1bd6058a888ecb251006d54e52bf4492ea5431` |
| `src/lobject.c` | `d408bfadb5c485c7b4c2d8d614166e370bb2fad41fc48ae061e423092559f445` |
| `src/lparser.c` | `085d9451d30802a00e15ec58e1884461dc1efb561b52df04be075365d4673e03` |
| `src/lstate.c` | `c62cbd5854360fdad909ff1bdf5910c3458bc78d12ab279cc0bc5076c5ecdf98` |
| `src/lstring.c` | `f6d3c03f0083b0e642d93e6a82e11c0607bf414f85d9d3c33843ce5e11067fbb` |
| `src/ltable.c` | `58154fb54cae5dc69c2c055c0466245911f52c830a5b2932b01a994b96d4401d` |
| `src/ltm.c` | `697845eb37e6ba0c9e85f496562116696d4b73707889f869847fc80438509aba` |
| `src/lundump.c` | `7f37de96c114de2bd37de63340487926b2004b9667295f1dd2b865b78505c157` |
| `src/lvm.c` | `e1e717c5bdf2e18b7bc15e15196b33ff4e383b8b3f4cd467b47825077042dc19` |
| `src/lzio.c` | `015c1f2b6f46a536f3b911e4ae1156e1f0fa21367a63d053675fed1551a8bf16` |

## Release originals of altered sources

The retail game compiles altered versions of the files below (see `include/upstream/lua-4.0.1-bfme/PROVENANCE.md`). The
release originals are kept here, byte-identical to the tarball's file, so the alteration can be diffed; nothing compiles
them. Their hashes are in the "Source payloads" table above and repeated here.

| file | SHA-256 |
|---|---|
| `src/lapi.c` | `7de4e815bf408b8916a71f785aa3aaac047e16edac047cd263eeeeef3ae9d841` |
| `src/lib/lbaselib.c` | `beae773b781aacea0eb36a9460a277288ef8e097f4160aa355daebd45994a3c4` |
| `src/lcode.c` | `64d35eb1c7a5b02b2c0c8aeb9f15050e7a7056b37d15f7a8826fce2d96bb9131` |
| `src/lib/ldblib.c` | `41345c982f8a2f440e256170663b0adacf779ddabf5662e257667b0ed4c2f381` |
| `src/lib/liolib.c` | `fe1cef02d273c2ea3bc193dc5669d4bf65356ba0be107b927e141b6dcebc2519` |
| `src/llex.c` | `b947617e3adeb104ea90c28354d7ef80abfe6f577efa1972d56675fd80890077` |
| `src/lobject.c` | `d408bfadb5c485c7b4c2d8d614166e370bb2fad41fc48ae061e423092559f445` |
| `src/lparser.c` | `085d9451d30802a00e15ec58e1884461dc1efb561b52df04be075365d4673e03` |
| `src/lstate.c` | `c62cbd5854360fdad909ff1bdf5910c3458bc78d12ab279cc0bc5076c5ecdf98` |
| `src/ltable.c` | `58154fb54cae5dc69c2c055c0466245911f52c830a5b2932b01a994b96d4401d` |
| `src/lvm.c` | `e1e717c5bdf2e18b7bc15e15196b33ff4e383b8b3f4cd467b47825077042dc19` |
