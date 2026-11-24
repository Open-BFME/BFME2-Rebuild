"""Shared identities and safe interchange for the protected managed check."""
import hashlib
import json
import os
import re
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parent
ENGINE = ROOT.parent / "managed-nt2"
sys.path.insert(1, str(ENGINE))
REPO = "Open-BFME/BFME2-Rebuild"
REPO_ID = 1464323350
CONTEXT = "BFME II / Boat"

DEFAULT_RUNNER_LABEL = "boat-small/from=bfme2-compiler-v1"
APPROVED_RUNNERS = {
    "boat-small/from=bfme2-compiler-v1": {
        "image_sha256": "154f0ee3f79f129b5d2e09adaef61dd2fab208040bff6b64e7fb0e182181dcf6",
        "profile_sha256": "e1f176db430a157b71cfb2f1adffbe5d0464aecb25f7e8796a83e015232ba0e3",
    },
    "bfme2-srv1-compiler-v1": {
        "image_sha256": "f7f41fb0ed9dae26567d42ade2dfef53e33bedf60d803cab42e49d282f831df3",
        "profile_sha256": "863fb71483c8e3d18b1788dca694a3787018aee4b0b2b25c18cb940f14222f86",
    },
}


def active_runner(env=None):
    label = (env or os.environ).get("BFME_PR_RUNNER", "").strip() or DEFAULT_RUNNER_LABEL
    if label not in APPROVED_RUNNERS:
        raise ValueError(f"Unknown runner label: {label!r}")
    return label


RUNNER_LABEL = active_runner()
PIN = {
    "image_sha256": APPROVED_RUNNERS[RUNNER_LABEL]["image_sha256"],
    "profile_sha256": APPROVED_RUNNERS[RUNNER_LABEL]["profile_sha256"],
    "checker": "5159c4ec34d3a6cee2599b557edd70134f85093c5cecbe464935c03846924d00",
    "baseline_sha256": "f008b587570bad693981dc7218588c81d192a1e064b0f7f861539c51156a7640",
}
APPROVED_IMAGE_PROFILES = frozenset(
    (v["image_sha256"], v["profile_sha256"]) for v in APPROVED_RUNNERS.values()
)
HEX40 = re.compile(r"[0-9a-f]{40}\Z")
HEX64 = re.compile(r"[0-9a-f]{64}\Z")


def encoded(data):
    return (json.dumps(data, sort_keys=True, separators=(",", ":"), ensure_ascii=True) + "\n").encode("ascii")


def digest(data):
    return hashlib.sha256(data).hexdigest()


def read(path, limit=16 * 1024**2):
    with Path(path).open("rb") as stream:
        raw = stream.read(limit + 1)
    if len(raw) > limit:
        raise ValueError("A managed check input exceeded its limit.")
    return json.loads(raw)


def write(path, item):
    Path(path).write_bytes(encoded(item))


def policy():
    return digest((ROOT / "bundle.sha256").read_bytes())


def validate_binding(item):
    if (not isinstance(item, dict) or item.get("version") != 1 or item.get("repo") != REPO_ID
            or type(item.get("number")) is not int or not 0 < item["number"] < 10**9
            or any(not HEX40.fullmatch(item.get(key, "")) for key in ("base", "head", "tree"))
            or any(item.get(key) != value for key, value in PIN.items())
            or item.get("policy") != policy()):
        raise ValueError("A managed receipt has an unexpected source or tool identity.")
    return item

