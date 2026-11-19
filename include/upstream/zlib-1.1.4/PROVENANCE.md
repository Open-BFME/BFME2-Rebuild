# zlib 1.1.4 as retail's `game.dat` links it

| | |
|---|---|
| Release | zlib 1.1.4 (Jean-loup Gailly, Mark Adler), `ZLIB_VERSION "1.1.4"` in `zlib.h` |
| Source URL | https://www.zlib.net/fossils/zlib-1.1.4.tar.gz |
| Tarball SHA-256 | `9e3e973174f9910fd51539ef9ce94c86a3943d4f897fab8e9adf4b19e6a8291e` |
| Tarball MD5 | `abc405d0bdd3ee22782d7aa20e440f08` |
| Tarball size | 181144 bytes |
| Downloaded | 2026-11-17 (agent v01); hashes computed on the downloaded file and equal to the checker's `libraries/releases.json` |
| Licence | the zlib licence in `zlib.h`'s header comment (also summarised in `README`), unchanged |

## What is here

Every file under this directory is byte-identical to the file of the same name in the tarball: the
eleven headers retail's sources include and `README`. The `.c` files are under
`src/zlib-1.1.4/Compression/ZLib/` (the directory EA kept it in); each starts with the checker's prefix
(`// cl-profile: upstream`, `// cl: ...`, `// UPSTREAM: zlib-1.1.4; pristine payload SHA256 ...`, the
`// FUNCTION:` lines) and then the upstream file, byte-identical.

Compile with `-Iinclude/upstream/zlib-1.1.4`. These are C files that use K&R-style definitions
(`uLong ZEXPORT adler32(adler, buf, len) uLong adler; ...`); `infblock.c` defines
`struct inflate_codes_state` itself, which the gates normally refuse in a `.c` file and accept here
only because the payload hash is an official release file's.

## Not imported

`crc32.c`, `gzio.c`, `example.c`, `minigzip.c`, `maketree.c`, `contrib/`, `nt/`, `msdos/`, `amiga/`,
`os2/`, `zlib.3`, `zlib.html`, `FAQ`, `ChangeLog`, `algorithm.txt`, the `Makefile`s: retail does not
link them (no row sits at a `crc32` or `gz*` function).

## Compile profile that reproduces retail

`cl -nologo -c -Gy -O2 -GR- -EHsc- /MD -Iinclude/upstream/zlib-1.1.4`. No `ASMV`, `DYNAMIC_CRC_TABLE`,
`DEBUG` or other define is needed. `/MD` matters: retail linked the DLL C runtime, so `calloc`/`free`/
`memcpy` are `__imp__` indirect calls. All twelve files compile to retail's bytes with no source change.

## File hashes

Headers and text files (equal to the tarball's):

| file | SHA-256 |
|---|---|
| `README` | `be76f03a0c3db1b1b8879359449bd82286555d8546ad3911976ed660078a2f73` |
| `deflate.h` | `094cdbd9e851140334099f46a2047b9ee722dcc3f2cf3c0b32e4b56dd5cab823` |
| `infblock.h` | `8e2b864ee159b0e8bc6ca7a9ac71369b3dc06588ea0d6c2b9e8b2d2248ef8004` |
| `infcodes.h` | `665310317ceba2af7032fd50d16eaf191f0ae0c805af712f9f9f655807355c89` |
| `inffast.h` | `d446d816a4416c849c7e706c583f2f6500ebaa7840d60f4ce2d81a05363e5a35` |
| `inffixed.h` | `2a28f6ec7a9042b31ee2f214aff62a8ae9ab079a584e278a8e7e0f156b329d9e` |
| `inftrees.h` | `52e1dac5dbd3fa922273adea9dae6c6b8c74acc58e1254f6af0bd47691ec36af` |
| `infutil.h` | `e235b2fae8e43eb084c8cccd765a1ea8ebceb9cbfe2f6c3852c1cc4caed340ee` |
| `trees.h` | `81579041310716219bbbf7c98849d91f5aeabdd7eadb1f8e9d7e2aa57724b58b` |
| `zconf.h` | `7f991f3b845b18fe68520af54f800c2e2756c26be419b42b46e59b57c492e56a` |
| `zlib.h` | `7388aeb8cc550dcbc158fd43dc90ee900f5fc6a3d055ceaf4c83aa441e6d387f` |
| `zutil.h` | `07a2a41547262b9894db1ad591e724e61f1aa44075bb00538a621a09ef4633ff` |

Source payloads (the bytes after each `.c` file's prefix; equal to the tarball's file of that name):

| file | SHA-256 |
|---|---|
| `adler32.c` | `7a921ef4c444aa3f49c312b39499e17bf12b25856ca5a904ea768037016ab337` |
| `compress.c` | `33dc3e2ae63b27e605a741eff245a9f01358a77593bae6c46502ccbff2f968a3` |
| `deflate.c` | `24de2ffce0907d7692fc6eb5abcc200e9781a8eca24d8db9289f709fcdf5e3be` |
| `infblock.c` | `282797dee3077d40cbe492428c2eab75b04b496b854b180b3e85140f8ad58394` |
| `infcodes.c` | `524e1f47ad64638cd2fd6dd163a8290b313f4d65968e20bd4e05860e1c4d0a4b` |
| `inffast.c` | `1043e4300e9a81e7897ebac507f4cbd75bdcdabd0a77410538404d5076f151f9` |
| `inflate.c` | `ae643f81db42e3664ac56050146dc90212aba4f450aadcb8ed54908b494887d4` |
| `inftrees.c` | `43ac236f9a64e933ec06e5bde627e9bb2bda553f6db4b605ecff6862347cbf84` |
| `infutil.c` | `a098f985bdaaddfb9aee665f2dceb1cd6bb07ce997c19908f96b9d13984eeb29` |
| `trees.c` | `41ea406e6be0b4f9d92c1877e89093868f5f25a723688880cd05cda1b5107f1f` |
| `uncompr.c` | `31d19ae0f3284924d97d770525c0f2b9e5d4384f0d3ef90aa56a618a1fe7b565` |
| `zutil.c` | `9ef7ed0e467652c1116864dd9b20a2a0602cd68d92312c55f4d6899ffca8ed18` |
