"""Paths for a local judge invocation."""
import json
import os
from pathlib import Path

TOOL_ROOT = Path(__file__).resolve().parent
config_path = TOOL_ROOT / "local-paths.json"
config = json.loads(config_path.read_text(encoding="utf-8")) if config_path.is_file() else {}

def configured_path(name, default=None):
    raw = os.environ.get("BFME_JUDGE_" + name, config.get(name, default))
    return None if raw is None else Path(raw).expanduser().resolve()

STATE = configured_path("STATE", Path.home() / ".cache" / "bfme2-judge")
RUNNER = configured_path("RUNNER", Path.home() / "open-bfme-2-toolchain")
REPO = configured_path("REPO", Path.cwd() / ".git")
RETAIL = configured_path("RETAIL", STATE / "inputs" / "game.dat")
WINE_PREFIX = configured_path("WINEPREFIX", STATE / "wine")
DEMANGLER = configured_path("DEMANGLER", TOOL_ROOT / "bin" / "demangle")
FARM = configured_path("FARM")
RUNNER_MODULES = TOOL_ROOT / "runner-tools"
