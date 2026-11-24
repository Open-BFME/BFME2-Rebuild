#!/usr/bin/env python3
"""Live local view of a pre-check run: http://localhost:8080 (standard library only, no CDN).

usage:
  viewer.py [--dir RUN_DIR] [--port 8080]      view a run (default: the latest under the user cache)

precheck.py --view starts the same server while it runs. The page polls status.json, which the run
rewrites after every step and every unit: current step, current unit and the verdict of each
function (PASS / MISMATCH / COMPILE ERROR). Selecting a function shows a side-by-side disassembly
of the retail bytes (baseline/game.dat) and the compiled object, relocated operands zeroed on both
sides as the judge compares them. The server listens on 127.0.0.1 only.
"""
import argparse
import difflib
import hashlib
import json
import os
import re
import shutil
import subprocess
import sys
import tempfile
import threading
import time
from http.server import BaseHTTPRequestHandler, ThreadingHTTPServer
from pathlib import Path
from urllib.parse import parse_qs, urlparse

HERE = Path(__file__).resolve().parent
PAGE = HERE / "viewer.html"
STATUS = "status.json"


# ---------------------------------------------------------------------------
# Status file


class Status:
    """The run's status.json, rewritten atomically so the page never reads half a document."""

    def __init__(self, directory, **fields):
        self.directory = Path(directory)
        self.directory.mkdir(parents=True, exist_ok=True)
        self.lock = threading.Lock()
        self.data = {"state": "running", "step": "", "unit": "", "started": time.time(), "steps": [],
                     "units": {}, "messages": [], **fields}
        self.write()

    def write(self):
        self.data["updated"] = time.time()
        path = self.directory / STATUS
        temporary = path.with_name(STATUS + ".tmp")
        temporary.write_text(json.dumps(self.data, indent=1, default=list), encoding="utf-8")
        os.replace(temporary, path)

    def step(self, name, state="running"):
        with self.lock:
            for item in self.data["steps"]:
                if item["state"] == "running":
                    item["state"] = "done"
            self.data["steps"].append({"name": name, "state": state, "t": time.time()})
            self.data["step"] = name
            self.write()

    def unit(self, tu, **fields):
        with self.lock:
            entry = self.data["units"].setdefault(tu, {"state": "pending", "functions": []})
            entry.update(fields)
            if fields.get("state") == "running":
                self.data["unit"] = tu
            self.write()

    def message(self, text):
        with self.lock:
            self.data["messages"].append(text)
            self.write()

    def finish(self, state, summary=""):
        with self.lock:
            for item in self.data["steps"]:
                if item["state"] == "running":
                    item["state"] = "done" if state == "done" else state
            self.data.update(state=state, summary=summary, unit="")
            self.write()


# ---------------------------------------------------------------------------
# Disassembly and side-by-side rows


def objdump():
    return shutil.which("objdump")


def disassemble(blob, tool=None):
    """objdump -D -b binary -mi386 -Mintel, the listing judge.py's --diff uses; None without objdump."""
    tool = tool or objdump()
    if not tool or not blob:
        return None
    handle, name = tempfile.mkstemp(suffix=".bin")
    try:
        with os.fdopen(handle, "wb") as out:
            out.write(blob)
        text = subprocess.run([tool, "-D", "-b", "binary", "-mi386", "-Mintel", name], capture_output=True,
                              text=True).stdout
    finally:
        os.unlink(name)
    lines = []
    for line in text.splitlines():
        match = re.match(r"\s*([0-9a-f]+):\s+((?:[0-9a-f]{2} )+)\s*(.*)", line)
        if match:
            lines.append(f"{int(match[1], 16):5x}: {match[2].strip():<24} {match[3]}")
    return lines


def hex_rows(blob, width=8):
    return [f"{i:5x}: " + " ".join(f"{b:02x}" for b in blob[i:i + width]) for i in range(0, len(blob), width)]


def _code(line):
    """The comparable part of a listing line: the instruction text (offset and bytes dropped)."""
    parts = line.split(None, 1)
    return parts[1] if len(parts) > 1 else line


def side_by_side(left, right, *, key=_code):
    """Rows of {left, right, kind} with kind same, change, left (retail only) or right (compiled only)."""
    rows = []
    matcher = difflib.SequenceMatcher(None, [key(x) for x in left], [key(x) for x in right], autojunk=False)
    for tag, i1, i2, j1, j2 in matcher.get_opcodes():
        if tag == "equal":
            rows += [{"left": left[i], "right": right[j], "kind": "same"} for i, j in zip(range(i1, i2), range(j1, j2))]
            continue
        a, b = left[i1:i2], right[j1:j2]
        for k in range(max(len(a), len(b))):
            l, r = (a[k] if k < len(a) else ""), (b[k] if k < len(b) else "")
            rows.append({"left": l, "right": r, "kind": "change" if l and r else ("left" if l else "right")})
    return rows


def unified_to_rows(text):
    """Side-by-side rows from judge.py --diff output (a unified diff of retail vs compiled)."""
    rows, minus, plus = [], [], []

    def flush():
        for k in range(max(len(minus), len(plus))):
            l, r = (minus[k] if k < len(minus) else ""), (plus[k] if k < len(plus) else "")
            rows.append({"left": l, "right": r, "kind": "change" if l and r else ("left" if l else "right")})
        minus.clear()
        plus.clear()

    header = []
    for line in text.splitlines():
        if line.startswith(("---", "+++")):
            header.append(line)
            continue
        if line.startswith("@@"):
            flush()
            rows.append({"left": line, "right": "", "kind": "gap"})
        elif line.startswith("-"):
            minus.append(line[1:])
        elif line.startswith("+"):
            plus.append(line[1:])
        elif line.startswith(" "):
            flush()
            rows.append({"left": line[1:], "right": line[1:], "kind": "same"})
    flush()
    return header, rows


def object_diff(retail, rva, size, base_obj, symbol, *, tool=None):
    """Side-by-side of retail bytes (RVA, SIZE in RETAIL) and compiled SYMBOL of BASE_OBJ, relocations zeroed."""
    sys.path.insert(0, str(HERE))
    import objdiff_project
    if not size:
        return {"state": "error", "note": "the ledger gives this row no size"}
    retail = objdiff_project.PE(retail).read(int(str(rva), 16), int(size))
    compiled_bodies = objdiff_project.function_bodies(Path(base_obj).read_bytes()) \
        if base_obj and Path(base_obj).is_file() else {}
    if not symbol or symbol not in compiled_bodies:
        return {"state": "ready", "source": "retail only",
                "rows": [{"left": line, "right": "", "kind": "left"}
                         for line in (disassemble(retail, tool) or hex_rows(retail))],
                "note": "no compiled function was paired with this row (MISSING or not emitted)"}
    body, relocs = compiled_bodies[symbol]
    if len(body) > len(retail) and all(b == 0xCC for b in body[len(retail):]):
        body = body[:len(retail)]
    relocs = [r for r in relocs if r < len(body)]
    left = objdiff_project.masked(retail, [r for r in relocs if r < len(retail)])
    right = objdiff_project.masked(body, relocs)
    listing_left, listing_right = disassemble(left, tool), disassemble(right, tool)
    source = "objdump"
    if listing_left is None or listing_right is None:
        listing_left, listing_right, source = hex_rows(left), hex_rows(right), "bytes (no objdump on PATH)"
    return {"state": "ready", "source": source, "rows": side_by_side(listing_left, listing_right),
            "equal": left == right, "sizes": [len(retail), len(body)]}


# ---------------------------------------------------------------------------
# Server


class Diffs:
    """Computes each requested diff once, in the background, and caches it in the run directory."""

    def __init__(self, directory, provider=None):
        self.directory = Path(directory) / "diffs"
        self.directory.mkdir(parents=True, exist_ok=True)
        self.provider = provider
        self.lock = threading.Lock()
        self.running = set()
        self.latest = {}

    def path(self, tu, rva):
        return self.directory / (hashlib.sha256(f"{tu}\0{rva}".encode()).hexdigest()[:24] + ".json")

    def get(self, tu, rva):
        path = self.path(tu, rva)
        if path.is_file():
            try:
                return json.loads(path.read_text(encoding="utf-8"))
            except ValueError:
                pass
        provider = self.provider or self.from_status(tu)
        if provider is None:
            return {"state": "error", "note": "no diff source for this run (no objects and no judge backend)"}
        with self.lock:
            if (tu, rva) in self.latest:
                return self.latest.pop((tu, rva))
            if (tu, rva) not in self.running:
                self.running.add((tu, rva))
                threading.Thread(target=self._compute, args=(provider, tu, rva, path), daemon=True).start()
        return {"state": "pending"}

    def _compute(self, provider, tu, rva, path):
        try:
            result = provider(tu, rva)
        except Exception as error:  # noqa: BLE001 - shown on the page, never fatal to the run
            result = {"state": "error", "note": f"{type(error).__name__}: {error}", "provisional": True}
        with self.lock:
            if result.pop("provisional", False):
                self.latest[(tu, rva)] = result      # served once, recomputed on the next request
            else:
                path.write_text(json.dumps(result), encoding="utf-8")
            self.running.discard((tu, rva))

    def from_status(self, tu):
        """A finished run viewed later: diff from the objects its status recorded."""
        try:
            data = json.loads((self.directory.parent / STATUS).read_text(encoding="utf-8"))
        except (OSError, ValueError):
            return None
        unit = data.get("units", {}).get(tu, {})
        if not data.get("retail") or not unit.get("base_obj"):
            return None
        return lambda tu, rva: row_diff(data["retail"], unit, rva)


def row_diff(retail, unit, rva, *, tool=None):
    """object_diff for the status row RVA of UNIT (its size and compiled symbol come from the judge)."""
    row = next((f for f in unit.get("functions", []) if f.get("rva") == rva), {})
    return object_diff(retail, rva, row.get("size"), unit.get("base_obj"), row.get("symbol"), tool=tool)


def handler_for(directory, diffs):
    directory = Path(directory)

    class Handler(BaseHTTPRequestHandler):
        def log_message(self, *args):
            pass

        def send(self, code, body, kind):
            self.send_response(code)
            self.send_header("Content-Type", kind)
            self.send_header("Cache-Control", "no-store")
            self.send_header("Content-Length", str(len(body)))
            self.end_headers()
            self.wfile.write(body)

        def do_GET(self):
            url = urlparse(self.path)
            if url.path in ("/", "/index.html"):
                return self.send(200, PAGE.read_bytes(), "text/html; charset=utf-8")
            if url.path == "/status.json":
                try:
                    return self.send(200, (directory / STATUS).read_bytes(), "application/json")
                except OSError:
                    return self.send(200, b'{"state":"waiting","units":{},"steps":[]}', "application/json")
            if url.path == "/diff":
                query = parse_qs(url.query)
                tu, rva = query.get("tu", [""])[0], query.get("rva", [""])[0]
                body = json.dumps(diffs.get(tu, rva)).encode()
                return self.send(200, body, "application/json")
            return self.send(404, b"not found", "text/plain")

    return Handler


def serve(directory, *, port=8080, provider=None, host="127.0.0.1"):
    """Start the viewer in a daemon thread; returns (server, url). Falls back to a free port if PORT is taken."""
    diffs = Diffs(directory, provider)
    for candidate in (port, 0):
        try:
            server = ThreadingHTTPServer((host, candidate), handler_for(directory, diffs))
            break
        except OSError:
            continue
    server.daemon_threads = True
    threading.Thread(target=server.serve_forever, daemon=True).start()
    return server, f"http://localhost:{server.server_address[1]}"


def runs_root():
    sys.path.insert(0, str(HERE))
    import bootstrap
    return bootstrap.cache_root() / "runs"


def latest_run():
    runs = sorted((p for p in runs_root().glob("*") if (p / STATUS).is_file()), key=lambda p: p.stat().st_mtime)
    return runs[-1] if runs else None


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.split("\n\n")[0])
    parser.add_argument("--dir", type=Path, help="run directory (default: the latest run)")
    parser.add_argument("--port", type=int, default=8080)
    args = parser.parse_args(argv)
    directory = args.dir or latest_run()
    if directory is None:
        print("viewer: no run yet; start one with precheck.py --view", file=sys.stderr)
        return 2
    server, url = serve(directory, port=args.port)
    print(f"Viewing {directory} at {url} (Ctrl-C to stop)")
    try:
        while True:
            time.sleep(3600)
    except KeyboardInterrupt:
        server.shutdown()
    return 0


if __name__ == "__main__":
    sys.exit(main())
