# Local pre-check image: the Boat compiler environment without the compiler.
#
# Ubuntu 24.04 with the exact Wine packages of the hosted bfme2-compiler-v1 image
# (docs/compiler-image.md, tools/compiler-image/fresh-install.sh), Python 3, Git and
# the LLVM demangler built from tools/judge/demangle.cpp. It contains no Microsoft
# file: the verified Visual C++ 7.1 root is mounted read-only at run time.
#
# Build (the context is tools/, nothing else is sent):
#   docker build -f tools/local_check/Dockerfile -t bfme2-local-check tools
#
# Run (tools/local_check/precheck.py --backend docker does this for you):
#   docker run --rm --network none \
#     --mount type=bind,source="$PWD",target=/work,readonly \
#     --mount type=bind,source="$(git rev-parse --path-format=absolute --git-common-dir)",target=/git,readonly \
#     --mount type=bind,source=/path/to/vc71,target=/opt/toolchain/vs/vc71,readonly \
#     --mount type=bind,source=/path/to/vcruntime/msvcp71.dll,target=/opt/toolchain/msvcp71.dll,readonly \
#     --mount type=bind,source=/path/to/vcruntime/msvcr71.dll,target=/opt/toolchain/msvcr71.dll,readonly \
#     --mount type=bind,source=/path/to/open-bfme-2,target=/opt/runner,readonly \
#     --mount type=volume,source=bfme2-judge-state,target=/state \
#     bfme2-local-check --json <revision> src/<unit>.cpp
FROM ubuntu:24.04

ARG WINE_VERSION=9.0~repack-4build3
ARG LLVM_VERSION=18

ENV DEBIAN_FRONTEND=noninteractive \
    LANG=C \
    LC_ALL=C \
    PYTHONDONTWRITEBYTECODE=1

RUN dpkg --add-architecture i386 \
 && apt-get update \
 && apt-get install -y --no-install-recommends \
      ca-certificates git python3 binutils g++ "llvm-${LLVM_VERSION}-dev" \
      "wine=${WINE_VERSION}" \
      "wine64=${WINE_VERSION}" \
      "wine32:i386=${WINE_VERSION}" \
      "wine32-preloader:i386=${WINE_VERSION}" \
      "wine64-preloader=${WINE_VERSION}" \
 && apt-mark hold wine wine64 wine32:i386 wine32-preloader:i386 wine64-preloader \
 && rm -rf /var/lib/apt/lists/* \
 && git config --system --add safe.directory '*'

COPY judge/demangle.cpp /usr/local/src/bfme2/demangle.cpp
RUN c++ -std=c++17 -O2 /usr/local/src/bfme2/demangle.cpp \
      $(llvm-config-${LLVM_VERSION} --ldflags --libs --system-libs) \
      -o /usr/local/bin/bfme2-demangle

COPY local_check/docker-entrypoint.sh /usr/local/bin/bfme2-local-check
RUN chmod 0755 /usr/local/bin/bfme2-local-check

# Wine 9.0 aborts at start-up when TMPDIR is set (seen under the WSL2 kernel); judge.py sets TMPDIR for
# its own scratch files and cl.exe uses the prefix's %TEMP%, so this wrapper changes no compiled byte.
RUN printf '%s\n' '#!/bin/sh' 'unset TMPDIR' 'exec /usr/bin/wine "$@"' > /usr/local/bin/wine \
 && chmod 0755 /usr/local/bin/wine

ENV VC71_ROOT=/opt/toolchain/vs/vc71 \
    BFME_JUDGE_RUNNER=/opt/runner \
    BFME_JUDGE_REPO=/git \
    BFME_JUDGE_RETAIL=/work/baseline/game.dat \
    BFME_JUDGE_STATE=/state \
    BFME_JUDGE_WINEPREFIX=/state/wine \
    BFME_JUDGE_DEMANGLER=/usr/local/bin/bfme2-demangle \
    WINEDEBUG=-all \
    WINEDLLOVERRIDES=mscoree,mshtml=

WORKDIR /work
ENTRYPOINT ["/usr/local/bin/bfme2-local-check"]
