# Altered Lua 4.0.1: the fork retail `game.dat` links

Retail `game.dat` links a fork of Lua 4.0.1 that has a boolean type and a few other changes. The pristine
release is in `include/upstream/lua-4.0.1/` (see its `PROVENANCE.md`) and stays untouched; the release originals of
the altered `.c` files sit there too, byte-identical, so the difference can be diffed (nothing compiles them). This
directory holds the headers the fork needs, and `src/lua-4.0.1/Lua/` holds the altered sources at the map's paths.
Lua 4.0.1's licence permits modified versions if the copyright notice stays and the changes are marked: every
altered file has a `BFME-ALTERED:` notice, every changed hunk has a `BFME-RETAIL 0x<address>` comment naming the
retail address that proves it, and Lua's copyright notice (in `lua.h`, referred to by every file) is unchanged. Names that
later Lua releases also use (`bvalue`, `b`, `TK_TRUE`, `TK_FALSE`) are borrowed for readability and say so; `OP_PUSHBOOL`
follows `OP_PUSHINT`; the `Hash` field has a neutral name; `codepushbool` and `bfmeGo1039E` are the ledger's names. None of
the names is recovered from the binary.

Method: compile the unmodified upstream sources against the pristine headers, find the rows that differ from retail,
and change only what the retail bytes require. All addresses are RVAs in retail 1.06 `game.dat` (VA = RVA + 0x400000).

A quoted `#include "x.h"` looks in the including file's own directory first, so the headers that are not altered sit
here too, **byte-identical to upstream** (listed at the end); otherwise an altered header's neighbour would pull in the
pristine copy of the altered one. Compile with
`-Iinclude/upstream/lua-4.0.1-bfme/src -Iinclude/upstream/lua-4.0.1-bfme/include -Iinclude/upstream/lua-4.0.1/include -Iinclude/upstream/lua-4.0.1/src`.
A `.c` file under `src/lua-4.0.1/Lua/` that has no row below is the pristine upstream file (its prefix carries the
payload hash), compiled against these headers.

## Altered files

| altered file | upstream file | upstream SHA-256 | file SHA-256 (for a .c: the payload after the prefix and FUNCTION lines) | retail addresses |
|---|---|---|---|---|
| `include/upstream/lua-4.0.1-bfme/src/lapi.h` | `src/lapi.h` | `bec7a4282df1dd51c772b6edfbb0e29e5e01632398ae31d0878789143dff28c3` | `bec7a4282df1dd51c772b6edfbb0e29e5e01632398ae31d0878789143dff28c3` |  |
| `include/upstream/lua-4.0.1-bfme/src/lcode.h` | `src/lcode.h` | `71169a0566560a2ec51917ed899ebca3393f2488eabe5293b6e1afd78802bc5e` | `e850fc4d5289ef027339273075cca025ae7f91e07529a36c5429a3fdaf1ad3a4` | 0x00752970 |
| `include/upstream/lua-4.0.1-bfme/src/ldebug.h` | `src/ldebug.h` | `531d63d4f3729e80e123707c2a97ba08ca735ba3569751846f67d1619d3fe301` | `531d63d4f3729e80e123707c2a97ba08ca735ba3569751846f67d1619d3fe301` |  |
| `include/upstream/lua-4.0.1-bfme/src/ldo.h` | `src/ldo.h` | `562ab0da32295bd71e49fd91c06d762c6bb152e2399801197e014eb49697bcec` | `562ab0da32295bd71e49fd91c06d762c6bb152e2399801197e014eb49697bcec` |  |
| `include/upstream/lua-4.0.1-bfme/src/lfunc.h` | `src/lfunc.h` | `80c05efca4c94750f2e8e815669b59ed75b84b19933d24285509dd58f2ac0d06` | `80c05efca4c94750f2e8e815669b59ed75b84b19933d24285509dd58f2ac0d06` |  |
| `include/upstream/lua-4.0.1-bfme/src/lgc.h` | `src/lgc.h` | `70059721e5fee18bbc289cabace35dcbc8c72fa2147dad11cf0198a6ea9fbeaa` | `70059721e5fee18bbc289cabace35dcbc8c72fa2147dad11cf0198a6ea9fbeaa` |  |
| `include/upstream/lua-4.0.1-bfme/src/llex.h` | `src/llex.h` | `73aee22a5c454465ac4e2970e4618ddbca9a47370173513f13022e94e5d38eeb` | `056292109debf3efc41ea4d7ed0d015a9ceff54cc75ecf45174d1a5cad5d3380` | 0x008F3168 0x00756F10 |
| `include/upstream/lua-4.0.1-bfme/src/llimits.h` | `src/llimits.h` | `22cf123c846abe6180a53cb14ee52cc92d10f594da1c91dbb0f12960d1f75c68` | `22cf123c846abe6180a53cb14ee52cc92d10f594da1c91dbb0f12960d1f75c68` |  |
| `include/upstream/lua-4.0.1-bfme/src/lmem.h` | `src/lmem.h` | `11612383c7d6b26d7d38564445a5f5ed80e6f2344ebc7cc7cacce7e76766e8ba` | `11612383c7d6b26d7d38564445a5f5ed80e6f2344ebc7cc7cacce7e76766e8ba` |  |
| `include/upstream/lua-4.0.1-bfme/src/lobject.h` | `src/lobject.h` | `e051401d75cf5793a55eba62c768beec8a401bb0c671ca02bd4d412dd765eb0c` | `0c43a9b15fb4c7dc897601681e0f2dd9b3af75485245efe47b5016bd99ea1391` | 0x0074D924 0x0074FB56 0x0074EF1F 0x00750770 |
| `include/upstream/lua-4.0.1-bfme/src/lopcodes.h` | `src/lopcodes.h` | `75a5ed87b6caa25542cb90ff8f8000e551e94501fec75dbb716d3e3fac0aeb79` | `a3ebbb3b5eed0f7a49f7ed7a0aa41db3713fdc39bc215cdb72dfad5d3b5eb329` | 0x0074EF10 |
| `include/upstream/lua-4.0.1-bfme/src/lparser.h` | `src/lparser.h` | `76e1c4e4b44bb1e14984cba9440238f997fc7334ff53d64ff700684cfb1c7017` | `76e1c4e4b44bb1e14984cba9440238f997fc7334ff53d64ff700684cfb1c7017` |  |
| `include/upstream/lua-4.0.1-bfme/src/lstate.h` | `src/lstate.h` | `5282ee333b426c7442af3a73108526e5924fcea111407a2bb2ec58c41f7dd47e` | `5282ee333b426c7442af3a73108526e5924fcea111407a2bb2ec58c41f7dd47e` |  |
| `include/upstream/lua-4.0.1-bfme/src/lstring.h` | `src/lstring.h` | `4feb6e39dc41d847173929885e1a895d380253b468ab2b093b05768555b1c1c8` | `4feb6e39dc41d847173929885e1a895d380253b468ab2b093b05768555b1c1c8` |  |
| `include/upstream/lua-4.0.1-bfme/src/ltable.h` | `src/ltable.h` | `5f7143074f521d808b1a7b7ea8113ea92fbe9a6685e25eb2226a54c9d97cc978` | `e37f2f9929992f1473ade45325a6a79f93942039d0dd1f0cd990beef5363e626` | 0x00750770 |
| `include/upstream/lua-4.0.1-bfme/src/ltm.h` | `src/ltm.h` | `ef399ca5af756aec4cc701b52cac5afb1f0d265917c54a9817b2ee20df89f9d1` | `ef399ca5af756aec4cc701b52cac5afb1f0d265917c54a9817b2ee20df89f9d1` |  |
| `include/upstream/lua-4.0.1-bfme/src/lundump.h` | `src/lundump.h` | `5ab39d5cc7d768835cea8d1dc075be7a729e2de6bf6c2bd0b054928c9283e89f` | `5ab39d5cc7d768835cea8d1dc075be7a729e2de6bf6c2bd0b054928c9283e89f` |  |
| `include/upstream/lua-4.0.1-bfme/src/lvm.h` | `src/lvm.h` | `6aaeffc3d2e5e52ee2146a95278ec5a15cad147bde4f486291f648fac5896e21` | `6aaeffc3d2e5e52ee2146a95278ec5a15cad147bde4f486291f648fac5896e21` |  |
| `include/upstream/lua-4.0.1-bfme/src/lzio.h` | `src/lzio.h` | `be259449bd1fb99b1cb9b5dbfcc5a0f6afbf27e22836e76429f4aa8cd4c29e07` | `be259449bd1fb99b1cb9b5dbfcc5a0f6afbf27e22836e76429f4aa8cd4c29e07` |  |
| `include/upstream/lua-4.0.1-bfme/include/lauxlib.h` | `include/lauxlib.h` | `67bb2e7a372e428571975e8ff6d087f022da76e531e3bccdb4336c3934a8c60c` | `67bb2e7a372e428571975e8ff6d087f022da76e531e3bccdb4336c3934a8c60c` |  |
| `include/upstream/lua-4.0.1-bfme/include/lua.h` | `include/lua.h` | `ecf14d8b6eb040d337ac52d4b8b39d84e3e0a95afd414206825fd07ad14220e7` | `349aa8e185cde8abd62136f90109e6350c5b94e56ec6c218dea07524f441b0e2` | 0x007478E0 |
| `include/upstream/lua-4.0.1-bfme/include/luadebug.h` | `include/luadebug.h` | `c039bd07a93654ffe9ef0f59de46393e773a19b413efa4b7d6a225db2e8cf72a` | `c039bd07a93654ffe9ef0f59de46393e773a19b413efa4b7d6a225db2e8cf72a` |  |
| `include/upstream/lua-4.0.1-bfme/include/lualib.h` | `include/lualib.h` | `4dde9c616be6ce7de5e29ca108583cef963cf73020c0a3ab487ef7f2387b92d8` | `4dde9c616be6ce7de5e29ca108583cef963cf73020c0a3ab487ef7f2387b92d8` |  |
| `src/lua-4.0.1/Lua/lapi.c` | `src/lapi.c` | `7de4e815bf408b8916a71f785aa3aaac047e16edac047cd263eeeeef3ae9d841` | `faf35f83895e67e052375f1a81072645d4e9f9ccca23231a174c9f4925189643` | 0x00747540 0x007478E0 0x007478E9 |
| `src/lua-4.0.1/Lua/lbaselib.c` | `src/lib/lbaselib.c` | `beae773b781aacea0eb36a9460a277288ef8e097f4160aa355daebd45994a3c4` | `106455bd884bfbd49ee05defa24b992fed5c870630dbda92a759a8144eda0e0d` | 0x00333B36 0x0074B764 0x0074B788 0x0074BFC3 |
| `src/lua-4.0.1/Lua/lcode.c` | `src/lcode.c` | `64d35eb1c7a5b02b2c0c8aeb9f15050e7a7056b37d15f7a8826fce2d96bb9131` | `c7f0449e54a18fc1cd8aed8606ab59ea3c30cda0bad029609feadd767be7da85` | 0x00752970 0x0075340C 0x008F2BD2 |
| `src/lua-4.0.1/Lua/ldblib.c` | `src/lib/ldblib.c` | `41345c982f8a2f440e256170663b0adacf779ddabf5662e257667b0ed4c2f381` | `b1cde834116788683d21468f6e93ea70f58b92d09a15d7ef60c075abbb2386f0` | 0x00747ECC |
| `src/lua-4.0.1/Lua/liolib.c` | `src/lib/liolib.c` | `fe1cef02d273c2ea3bc193dc5669d4bf65356ba0be107b927e141b6dcebc2519` | `ef6a876c3d21ba3480b90dd8e7609240ae8868a87a55d79b3be78a999966e340` | 0x0074A0ED 0x00BC9CB8 |
| `src/lua-4.0.1/Lua/llex.c` | `src/llex.c` | `b947617e3adeb104ea90c28354d7ef80abfe6f577efa1972d56675fd80890077` | `4af44e04e795977fd34942741f21b187402c42aca476d4ef39c0d37580c905cf` | 0x008F3168 |
| `src/lua-4.0.1/Lua/lobject.c` | `src/lobject.c` | `d408bfadb5c485c7b4c2d8d614166e370bb2fad41fc48ae061e423092559f445` | `6f340c8d2a57673e13eefccaf2545470b9096097c13cf91bb05d54e375ade74d` | 0x0074E0F2 |
| `src/lua-4.0.1/Lua/lparser.c` | `src/lparser.c` | `085d9451d30802a00e15ec58e1884461dc1efb561b52df04be075365d4673e03` | `bf2d6b1a83b40c8b3242a257b6b5921109a3d4802e2421e186b66c778c5dc43b` | 0x007556B5 0x007556C9 |
| `src/lua-4.0.1/Lua/lstate.c` | `src/lstate.c` | `c62cbd5854360fdad909ff1bdf5910c3458bc78d12ab279cc0bc5076c5ecdf98` | `7a4bd24a25df51b3aeb45b6cdfab7fe8b7355629bd8d1b5ca313123258249a24` | 0x0074DF0A 0x0074DF30 |
| `src/lua-4.0.1/Lua/ltable.c` | `src/ltable.c` | `58154fb54cae5dc69c2c055c0466245911f52c830a5b2932b01a994b96d4401d` | `3cfacb7d070168585f4843f7cc0cfad5d36bdb2155fd0f39a573e03125923dd9` | 0x00750770 0x0075079A |
| `src/lua-4.0.1/Lua/lvm.c` | `src/lvm.c` | `e1e717c5bdf2e18b7bc15e15196b33ff4e383b8b3f4cd467b47825077042dc19` | `d1c753e3082b16c3e48ae9b043f84acaa0591616ace6c0cb80af8257ed8aaf9a` | 0x0074E368 0x0074EA90 0x0074ECD5 0x0074EDC0 0x0074EF10 0x0074F14F 0x0074F509 0x0074F637 0x0074F65F 0x0074F67F 0x0074F6A3 0x0074F6C7 |

## Hunks and their evidence

| file | hunk | evidence |
|---|---|---|
| src/lobject.h | LUA_TMARK 6 -> 7 | luaD_call 0x0074D8D0 stores 7 into the callee mark (`mov DWORD PTR [edi],0x7` at 0x0074D924) |
| src/lobject.h | NUM_TAGS 6 -> 7 | luaT_init 0x0074FB40 passes 7 to the vector grow (0x0074FB56) and stores last_tag 6 (`mov DWORD PTR [esi+0x4c],0x6`, 0x0074FB74) |
| src/lobject.h | union Value gains `int b` (the boolean payload) | luaV_execute 0x0074EDE0, the boolean case stores tag 6 at +0 and a 32-bit payload at +8 (`mov DWORD PTR [esi+0x8],ebx`, 0x0074EF1F) |
| src/lobject.h | macro `bvalue` | the accessor for the field above; same store, 0x0074EF1F |
| src/lobject.h | struct Hash gains an `int` at +0xC between `size` and `firstfree` | luaH_new 0x00750770 stores its third argument at +0xC (0x0075079A); setnodevector 0x007506E0 stores firstfree at +0x10 (0x00750762); collecttable 0x00750EB0 reads next at +0x14 and mark at +0x18 (0x00750EC0, 0x00750EC5, 0x00750ECD) |
| src/lopcodes.h | opcode OP_PUSHBOOL inserted after OP_POP (value 6; every later opcode moves up by one) | luaV_execute 0x0074EDE0: jump-table entry 6 (0x0074F964) goes to 0x0074EF10, which computes `(i>>6) - 0x1FFFFFF` and stores it as a tag-6 value; entry 7 (0x0074EEEE) is the stock PUSHINT |
| src/llex.h | TK_TRUE and TK_FALSE appended to the reserved words | reserved-word table at 0x008F3168 has 20 entries ending "while", "true", "false", "" (the NAME slot); luaX_init 0x00756F10 counts 0x14 of them (0x00756F39) |
| src/llex.h | NUM_RESERVED counts to TK_FALSE (20) | luaX_init 0x00756F10 `cmp ebx,0x14` at 0x00756F39 |
| src/ltable.h | luaH_new takes a third int argument | luaH_new 0x00750770 takes three arguments (`mov ecx,DWORD PTR [esp+0x20]` at 0x00750780, stored at 0x0075079A); its callers push three: 0x0074DF0A (f_luaopen), 0x0074ECD5 (luaV_pack), 0x0074F14F (OP_CREATETABLE) |
| src/lcode.h | declaration of `codepushbool` | codepushbool 0x00752970, called from simpleexp 0x007556BF (true) and 0x007556D3 (false) with `push 1|0; push fs` |
| include/lua.h | lua_newtable takes a size argument | lua_newtable 0x007478E0 loads its second argument (`mov eax,DWORD PTR [esp+0x8]`, 0x007478E0) and passes it to luaH_new; callers push two arguments: 0x0074DF30 (f_luaopen), 0x00747ECC (getinfo) |
| src/lua-4.0.1/Lua/ltable.c | luaH_new takes `sizehint` | luaH_new 0x00750770 (three arguments, see ltable.h) |
| src/lua-4.0.1/Lua/ltable.c | luaH_new stores `sizehint` between linking into roottable and setting mark | luaH_new 0x00750770: `mov [esi+0x14],eax; mov [edi+0x28],esi; mov [esi+0xc],ecx; mov [esi+0x18],esi` (0x00750794..0x0075079D) |
| src/lua-4.0.1/Lua/lapi.c | lua_newtable takes `size` and passes it as luaH_new's third argument | lua_newtable 0x007478E0: `push eax(size); push 0; push esi; call luaH_new` (0x007478E9..0x007478ED) |
| src/lua-4.0.1/Lua/lstate.c | f_luaopen calls luaH_new with a third argument 0 | f_luaopen 0x0074DEF0: `push 0; push 0xa; push esi; call 0x00750770` (0x0074DF0A) |
| src/lua-4.0.1/Lua/lstate.c | f_luaopen calls lua_newtable with size 0 | f_luaopen 0x0074DEF0: `push 0; push esi; call 0x007478E0` (0x0074DF30) |
| src/lua-4.0.1/Lua/lvm.c | luaV_pack calls luaH_new with a third argument 0 | luaV_pack 0x0074ECD0: `push 0; push 0; push esi; call 0x00750770` (0x0074ECD5) |
| src/lua-4.0.1/Lua/lvm.c | OP_CREATETABLE calls luaH_new with a third argument 0 | luaV_execute 0x0074EDE0, the OP_CREATETABLE case: `push 0; shr ebx,6; push ebx; push edi; call 0x00750770` (0x0074F14F) |
| src/lua-4.0.1/Lua/ldblib.c | getinfo calls lua_newtable with size 0 | getinfo 0x00747E00: `push 0; push esi; call 0x007478E0; add esp,8` (0x00747ECC) |
| src/lua-4.0.1/Lua/lobject.c | luaO_equalObj compares two booleans by payload | luaO_equalObj 0x0074E0C0: the tag switch has a seventh case (`cmp edx,0x6; ja` at 0x0074E0D1) whose target 0x0074E0F2 compares the +8 words of both objects |
| src/lua-4.0.1/Lua/lvm.c | luaV_tostring converts booleans to "true"/"false" | luaV_tostring 0x0074E360: `cmp eax,0x6` at 0x0074E36A, then luaS_new of the two strings at 0x007BC408 ("true") and 0x007BC410 ("false") and tag 3 stored (0x0074E38B, 0x0074E3A1) |
| src/lua-4.0.1/Lua/lvm.c | luaV_lessthan orders two booleans by payload | luaV_lessthan 0x0074EA40: after the number (0x0074EA47) and string (0x0074EA70) branches, `cmp eax,0x6 ... cmp DWORD PTR [esi],0x6 ... setl al` (0x0074EA90..0x0074EAA5) |
| src/lua-4.0.1/Lua/lvm.c | function `istrue` (macro `ISTRUE`): the three-way truth test | standalone copy 0x0074EDC0 (`cmp eax,1 -> 0; cmp eax,6 -> [ecx+8]; else 1`), same test inlined at 0x0074F513, 0x0074F637, 0x0074F65F, 0x0074F67F, 0x0074F6A3 |
| src/lua-4.0.1/Lua/lvm.c | luaV_execute gains the OP_PUSHBOOL case | luaV_execute 0x0074EDE0: jump-table entry 6 -> 0x0074EF10 (`shr ebx,6; sub ebx,0x1FFFFFF; mov [esi],6; mov [esi+8],ebx`, 0x0074EF10..0x0074EF1F) |
| src/lua-4.0.1/Lua/lvm.c | OP_NOT stores tag 6, then stores !truth as a number | luaV_execute 0x0074EDE0, OP_NOT (jump-table entry 32): `mov [esi-0x10],6` (0x0074F509), the truth test on the same slot (0x0074F510..0x0074F526), `sete cl; fild; fstp QWORD PTR [esi-8]` (0x0074F52B..0x0074F53A) |
| src/lua-4.0.1/Lua/lvm.c | OP_JMPT, OP_JMPF, OP_JMPONT, OP_JMPONF test truth (nil false, boolean by payload) instead of "not nil" | luaV_execute 0x0074EDE0, jump-table entries 39..42: 0x0074F637 (JMPT), 0x0074F65F (JMPF), 0x0074F67F (JMPONT), 0x0074F6A3 (JMPONF) each compare the tag with 1 and 6 and test the +8 word of a tag-6 value |
| src/lua-4.0.1/Lua/lvm.c | OP_PUSHNILJMP pushes boolean false instead of nil | luaV_execute 0x0074EDE0, jump-table entry 44: `mov [esi+8],0; mov [esi],6; add esi,0x10` (0x0074F6C7..0x0074F6D8) |
| src/lua-4.0.1/Lua/lcode.c | opcode property table gains the OP_PUSHBOOL entry {iS, 1, 0} | retail luaK_opproperties at 0x008F2BC0: entry 6 (0x008F2BD2) is {mode 2 (iS), push 1, pop 0}, entry 7 the PUSHINT {2,1,0}; the other 49 entries equal stock |
| src/lua-4.0.1/Lua/lcode.c | new function `codepushbool`: emits OP_PUSHBOOL for 1 or 0 | codepushbool 0x00752970: stack level +1 with the maxstacksize update (0x00752981..0x007529B4), `((b != 0) + 0x1FFFFFF) << 6 | 6` (0x007529B8..0x007529C1), codelineinfo 0x007521D0 (0x007529C4), growvector and store (0x007529D1..0x007529F7), pc+1 (0x00752A01) |
| src/lua-4.0.1/Lua/lparser.c | simpleexp handles TK_TRUE and TK_FALSE with `codepushbool` | simpleexp 0x00755650: after the string case, `next; push 1; push fs; call 0x00752970` (0x007556B5..0x007556BF) and the same with 0 (0x007556C9..0x007556D3) |
| src/lua-4.0.1/Lua/llex.c | the reserved-word table gains "true" and "false" after "while" | retail table 0x008F3168 entries 18 and 19 point at 0x007BC408 ("true") and 0x007BC410 ("false"); entry 20 is the empty NAME slot |
| src/lua-4.0.1/Lua/lcode.c | luaK_tostack materialises a true test with OP_PUSHBOOL instead of OP_PUSHINT 1 | luaK_tostack 0x00753160 stores the instruction word 0x80000006 (OP_PUSHBOOL, argument 1) at 0x0075340C; stock stores the PUSHINT word |
| src/lua-4.0.1/Lua/lapi.c | new function `bfmeGo1039E`: pushes a boolean | bfmeGo1039E 0x00747540 (45 bytes): `mov [eax],6` (0x0074754B), `mov [ecx+0x8],edx` (0x00747553), stack-limit test and luaD_checkstack (0x00747558..0x00747565), `add [esi],0x10`; it sits between lua_pushnil 0x00747510 and lua_pushnumber 0x00747570 |
| src/lua-4.0.1/Lua/lbaselib.c | declaration of the game's log function, which `print` calls | luaB_print 0x0074B700 calls 0x00333B36 (`call 0x333b36`, 0x0074B769, 0x0074B772, 0x0074B78D) where stock calls fputs |
| src/lua-4.0.1/Lua/lbaselib.c | print writes the separator tab and the string through bfmeLogMsg574 | luaB_print 0x0074B700: `push 0xbbb854 ("	"); call 0x333b36` (0x0074B764, 0x0074B769) and `push ebx; call 0x333b36` (0x0074B771, 0x0074B772) |
| src/lua-4.0.1/Lua/lbaselib.c | print ends the line through bfmeLogMsg574 | luaB_print 0x0074B700: `push 0xbbe498 ("
"); call 0x333b36` (0x0074B788, 0x0074B78D) |
| src/lua-4.0.1/Lua/lbaselib.c | tostring: booleans share the number case | luaB_tostring 0x0074BFA0: `cmp eax,0x6; ja` (0x0074BFB3) and jump-table entry 6 at 0x0074C0B8 pointing at 0x0074BFC3, the same target as entry 2 (NUMBER) |
| src/lua-4.0.1/Lua/liolib.c | pushresult reports a fixed "generic I/O error" and code -1 instead of strerror(errno) and errno | pushresult 0x0074A0D0 (the ledger calls this row io_remove and 0x0074AF10 pushresult; the bytes say the reverse): `push 0xcf1fc4` = "generic I/O error" (0x0074A0ED), `fld QWORD PTR ds:0xbc9cb8` = -1.0 (0x0074A0F8) |

## Unaltered headers kept beside the altered ones

Their rows above give equal upstream and file hashes and an empty address column: they are byte-identical to upstream.

| file | upstream SHA-256 |
|---|---|
| `src/lapi.h` | `bec7a4282df1dd51c772b6edfbb0e29e5e01632398ae31d0878789143dff28c3` |
| `src/ldebug.h` | `531d63d4f3729e80e123707c2a97ba08ca735ba3569751846f67d1619d3fe301` |
| `src/ldo.h` | `562ab0da32295bd71e49fd91c06d762c6bb152e2399801197e014eb49697bcec` |
| `src/lfunc.h` | `80c05efca4c94750f2e8e815669b59ed75b84b19933d24285509dd58f2ac0d06` |
| `src/lgc.h` | `70059721e5fee18bbc289cabace35dcbc8c72fa2147dad11cf0198a6ea9fbeaa` |
| `src/llimits.h` | `22cf123c846abe6180a53cb14ee52cc92d10f594da1c91dbb0f12960d1f75c68` |
| `src/lmem.h` | `11612383c7d6b26d7d38564445a5f5ed80e6f2344ebc7cc7cacce7e76766e8ba` |
| `src/lparser.h` | `76e1c4e4b44bb1e14984cba9440238f997fc7334ff53d64ff700684cfb1c7017` |
| `src/lstate.h` | `5282ee333b426c7442af3a73108526e5924fcea111407a2bb2ec58c41f7dd47e` |
| `src/lstring.h` | `4feb6e39dc41d847173929885e1a895d380253b468ab2b093b05768555b1c1c8` |
| `src/ltm.h` | `ef399ca5af756aec4cc701b52cac5afb1f0d265917c54a9817b2ee20df89f9d1` |
| `src/lundump.h` | `5ab39d5cc7d768835cea8d1dc075be7a729e2de6bf6c2bd0b054928c9283e89f` |
| `src/lvm.h` | `6aaeffc3d2e5e52ee2146a95278ec5a15cad147bde4f486291f648fac5896e21` |
| `src/lzio.h` | `be259449bd1fb99b1cb9b5dbfcc5a0f6afbf27e22836e76429f4aa8cd4c29e07` |
| `include/lauxlib.h` | `67bb2e7a372e428571975e8ff6d087f022da76e531e3bccdb4336c3934a8c60c` |
| `include/luadebug.h` | `c039bd07a93654ffe9ef0f59de46393e773a19b413efa4b7d6a225db2e8cf72a` |
| `include/lualib.h` | `4dde9c616be6ce7de5e29ca108583cef963cf73020c0a3ab487ef7f2387b92d8` |
