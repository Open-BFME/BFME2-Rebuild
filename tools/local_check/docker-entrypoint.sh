#!/bin/sh
# Entry point of the local pre-check image: initialize the persistent Wine prefix
# (Z: maps to /, as judge.py requires), check the mounts, then run the judge.
set -eu

if [ ! -f "$VC71_ROOT/Vc7/bin/cl.exe" ]; then
    echo "Mount your verified Visual C++ 7.1 root read-only at $VC71_ROOT" >&2
    echo "(python tools/local_check/bootstrap.py fetches and verifies it; precheck.py mounts it)." >&2
    exit 2
fi
if [ ! -d "$BFME_JUDGE_RUNNER/tools" ]; then
    echo "Mount the Open-BFME-2 toolchain support checkout read-only at $BFME_JUDGE_RUNNER (tools/README.md)." >&2
    exit 2
fi
if [ ! -f /work/tools/judge/bin/judge.py ] || [ ! -d "$BFME_JUDGE_REPO/objects" ]; then
    echo "Mount the checkout at /work and its Git directory (git rev-parse --git-common-dir) at $BFME_JUDGE_REPO." >&2
    exit 2
fi

if [ ! -e "$BFME_JUDGE_WINEPREFIX/dosdevices/z:" ]; then
    mkdir -p "$BFME_JUDGE_WINEPREFIX"
    WINEPREFIX="$BFME_JUDGE_WINEPREFIX" wineboot -i >/dev/null 2>&1
    WINEPREFIX="$BFME_JUDGE_WINEPREFIX" wineserver -w
fi

exec python3 /work/tools/judge/bin/judge.py "$@"
