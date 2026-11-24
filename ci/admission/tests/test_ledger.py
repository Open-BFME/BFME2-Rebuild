"""Tests for ledger compact serialization, safe pruning, blob fallback, and race safety (ci/admission/ledger.py).

Run from the repository root:  python3 -m unittest discover -s ci/admission/tests -v
"""
import base64
import datetime
import json
from pathlib import Path
import sys
import unittest

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "ci/admission"))
sys.path.insert(1, str(ROOT / "ci/managed-check"))
import ledger  # noqa: E402

DAY = "2026-11-20"
YESTERDAY = "2026-11-19"
OLD_DAY = "2026-11-18"
CLOCK = lambda: datetime.datetime(2026, 11, 20, 19, 40, tzinfo=datetime.timezone.utc)  # noqa: E731
MAIN = "5458a191e8f7d75b0dfaf1f7de8b177c58314127"
HEAD = "1" * 40
TREE = "a1" * 20
BUDGET = {"daily_total": 100, "daily_per_author": 50}


class FakeLedgerApi:
    def __init__(self):
        self._token, self.live, self.writes = "x" * 20, True, []
        self.store = {}
        self.blobs = {}
        self.trees = {}
        self.branch = True
        self.contents_mode = "normal"

    def write(self, method, path, body):
        self.writes.append((method, path, body))
        return self._send(method, path, body)

    def _send(self, method, path, body):
        if method == "GET":
            if path == f"git/ref/heads/{ledger.LEDGER_BRANCH}":
                return (200, {"object": {"sha": "c" * 40}}) if self.branch else (404, {})
            if path.startswith("contents/"):
                name = path.split("?")[0][len("contents/"):]
                if name not in self.store:
                    return 404, {}
                text, sha = self.store[name]
                if self.contents_mode == "too_large_403":
                    return 403, {"message": "too_large"}
                if self.contents_mode == "empty_content":
                    return 200, {"sha": sha, "content": "", "encoding": "none"}
                return 200, {"content": base64.b64encode(text.encode()).decode(),
                             "encoding": "base64", "sha": sha}
            if path.startswith("git/blobs/"):
                blob_sha = path[len("git/blobs/"):]
                if blob_sha in self.blobs:
                    return 200, {"content": base64.b64encode(self.blobs[blob_sha]).decode(),
                                 "encoding": "base64"}
                return 404, {}
            if path.startswith("git/trees/"):
                ref = path[len("git/trees/"):]
                if ref in self.trees:
                    return 200, {"tree": self.trees[ref]}
                return 404, {}
            return 404, {}
        if path == "git/trees":
            return 201, {"sha": "a" * 40}
        if path == "git/commits":
            return 201, {"sha": "b" * 40}
        if path == "git/refs":
            self.branch = True
            return 201, {}
        if path.startswith("contents/"):
            name = path[len("contents/"):]
            prior = self.store.get(name)
            if (prior and body.get("sha") != prior[1]) or (not prior and body.get("sha")):
                return 409, {"message": "sha mismatch"}
            self.store[name] = (base64.b64decode(body["content"]).decode(), f"{len(self.writes):040d}")
            return 201, {}
        return 404, {}


def seed(api, data):
    api.store["ledger.json"] = (json.dumps(data), "0" * 40)


def seed_blob(api, data):
    text = json.dumps(data)
    sha = "b1" * 20
    api.store["ledger.json"] = (text, sha)
    api.blobs[sha] = text.encode()
    api.trees[ledger.LEDGER_BRANCH] = [
        {"path": "ledger.json", "type": "blob", "sha": sha},
        {"path": "README.md", "type": "blob", "sha": "0" * 40},
    ]


class CompactSerialization(unittest.TestCase):
    def test_write_uses_compact_json(self):
        api = FakeLedgerApi()
        ledger.reserve(api, BUDGET, 100, 1, HEAD, MAIN, "7", "1", CLOCK, TREE, "narrow")
        raw = api.store["ledger.json"][0]
        self.assertNotIn("  ", raw)
        self.assertNotIn("\n ", raw)
        self.assertTrue(raw.endswith("\n"))
        parsed = json.loads(raw)
        self.assertEqual(parsed["version"], 2)

    def test_compact_round_trips_through_read(self):
        api = FakeLedgerApi()
        ledger.reserve(api, BUDGET, 100, 1, HEAD, MAIN, "7", "1", CLOCK, TREE, "narrow")
        item = ledger.find(api, 1, HEAD, MAIN, CLOCK)
        self.assertIsNotNone(item)
        self.assertEqual(item["number"], 1)


class SafePruning(unittest.TestCase):
    def test_today_reservation_on_old_base_kept(self):
        old_base = "a" * 40
        new_base = "b" * 40
        data = {
            "version": 2,
            "days": {DAY: {"total": 1, "authors": {"100": 1}, "wide_total": 0, "wide_authors": {}}},
            "reservations": [
                {"day": DAY, "number": 1, "author": 100, "head": HEAD, "base": old_base,
                 "tree": TREE, "kind": "narrow", "admitted_by": {"run": "7", "attempt": "1"},
                 "claimed_by": {"run": "800", "attempt": "1"}},
            ],
        }
        ledger._prune(data, YESTERDAY, new_base)
        self.assertEqual(len(data["reservations"]), 1)

    def test_today_unclaimed_reservation_on_old_base_kept(self):
        old_base = "a" * 40
        new_base = "b" * 40
        data = {
            "version": 2,
            "days": {DAY: {"total": 1, "authors": {"100": 1}, "wide_total": 0, "wide_authors": {}}},
            "reservations": [
                {"day": DAY, "number": 1, "author": 100, "head": HEAD, "base": old_base,
                 "tree": TREE, "kind": "narrow", "admitted_by": {"run": "7", "attempt": "1"},
                 "claimed_by": None},
            ],
        }
        ledger._prune(data, YESTERDAY, new_base)
        self.assertEqual(len(data["reservations"]), 1)

    def test_old_day_wrong_base_dropped(self):
        old_base = "a" * 40
        new_base = "b" * 40
        data = {
            "version": 2,
            "days": {OLD_DAY: {"total": 1, "authors": {"100": 1}, "wide_total": 0, "wide_authors": {}}},
            "reservations": [
                {"day": OLD_DAY, "number": 1, "author": 100, "head": HEAD, "base": old_base,
                 "tree": TREE, "kind": "narrow", "admitted_by": {"run": "7", "attempt": "1"},
                 "claimed_by": {"run": "800", "attempt": "1"}},
            ],
        }
        ledger._prune(data, YESTERDAY, new_base)
        self.assertEqual(len(data["reservations"]), 0)
        self.assertNotIn(OLD_DAY, data["days"])

    def test_old_day_same_base_kept(self):
        base = "a" * 40
        data = {
            "version": 2,
            "days": {OLD_DAY: {"total": 1, "authors": {"100": 1}, "wide_total": 0, "wide_authors": {}}},
            "reservations": [
                {"day": OLD_DAY, "number": 1, "author": 100, "head": HEAD, "base": base,
                 "tree": TREE, "kind": "narrow", "admitted_by": {"run": "7", "attempt": "1"},
                 "claimed_by": None},
            ],
        }
        ledger._prune(data, YESTERDAY, base)
        self.assertEqual(len(data["reservations"]), 1)

    def test_stale_m1_cannot_erase_m2_claimed_reservation(self):
        """Critic #4 scenario: M1 reads ledger, M2 reserves+claims, M1 writes with pruning.

        With the old _prune that dropped base!=caller_base, M1's write would erase M2's
        reservation. The safe prune keeps all reservations with day >= yesterday.
        """
        base_m1 = "a" * 40
        base_m2 = "b" * 40
        head_m2 = "2" * 40
        data = {
            "version": 2,
            "days": {DAY: {"total": 2, "authors": {"100": 1, "200": 1}, "wide_total": 0, "wide_authors": {}}},
            "reservations": [
                {"day": DAY, "number": 1, "author": 100, "head": HEAD, "base": base_m1,
                 "tree": TREE, "kind": "narrow", "admitted_by": {"run": "7", "attempt": "1"},
                 "claimed_by": None},
                {"day": DAY, "number": 2, "author": 200, "head": head_m2, "base": base_m2,
                 "tree": "b1" * 20, "kind": "narrow", "admitted_by": {"run": "8", "attempt": "1"},
                 "claimed_by": {"run": "900", "attempt": "1"}},
            ],
        }
        ledger._prune(data, YESTERDAY, base_m1)
        self.assertEqual(len(data["reservations"]), 2)
        m2_item = next(r for r in data["reservations"] if r["number"] == 2)
        self.assertEqual(m2_item["claimed_by"], {"run": "900", "attempt": "1"})


class BlobFallback(unittest.TestCase):
    def test_read_with_empty_content_uses_blob(self):
        api = FakeLedgerApi()
        data = {"version": 2, "days": {}, "reservations": [
            {"day": DAY, "number": 1, "author": 100, "head": HEAD, "base": MAIN,
             "tree": TREE, "kind": "narrow", "admitted_by": {"run": "7", "attempt": "1"},
             "claimed_by": None}
        ]}
        seed_blob(api, data)
        api.contents_mode = "empty_content"
        result, sha = ledger._read(api)
        self.assertEqual(result["version"], 2)
        self.assertEqual(len(result["reservations"]), 1)
        self.assertIsNotNone(sha)

    def test_read_with_403_too_large_uses_tree_then_blob(self):
        api = FakeLedgerApi()
        data = {"version": 2, "days": {}, "reservations": [
            {"day": DAY, "number": 1, "author": 100, "head": HEAD, "base": MAIN,
             "tree": TREE, "kind": "narrow", "admitted_by": {"run": "7", "attempt": "1"},
             "claimed_by": None}
        ]}
        seed_blob(api, data)
        api.contents_mode = "too_large_403"
        result, sha = ledger._read(api)
        self.assertEqual(result["version"], 2)
        self.assertEqual(len(result["reservations"]), 1)
        self.assertIsNotNone(sha)

    def test_large_reservation_round_trip_via_blob(self):
        api = FakeLedgerApi()
        reservations = []
        for i in range(5000):
            reservations.append({
                "day": DAY, "number": i + 1, "author": 1000 + (i % 50),
                "head": f"{i:040x}", "base": MAIN,
                "tree": f"{i + 1:040x}", "kind": "narrow",
                "admitted_by": {"run": str(i + 100), "attempt": "1"},
                "claimed_by": None,
            })
        data = {
            "version": 2,
            "days": {DAY: {"total": 5000, "authors": {str(1000 + i): 100 for i in range(50)},
                           "wide_total": 0, "wide_authors": {}}},
            "reservations": reservations,
        }
        raw = json.dumps(data, sort_keys=True, separators=(",", ":"))
        self.assertGreater(len(raw), 1_000_000)
        seed_blob(api, data)
        api.contents_mode = "empty_content"
        result, sha = ledger._read(api)
        self.assertEqual(len(result["reservations"]), 5000)
        self.assertEqual(result["reservations"][0]["number"], 1)
        self.assertEqual(result["reservations"][4999]["number"], 5000)


class BackwardCompat(unittest.TestCase):
    def test_reserve_checks_paid_for_already_paid_today(self):
        new_base = "b" * 40
        api = FakeLedgerApi()
        data = {
            "version": 2,
            "days": {DAY: {"total": 1, "authors": {"100": 1}, "wide_total": 0, "wide_authors": {},
                           "paid": [[1, HEAD, "narrow", 100]]}},
            "reservations": [],
        }
        seed(api, data)
        result = ledger.reserve(api, BUDGET, 100, 1, HEAD, new_base, "7", "1", CLOCK, TREE, "narrow")
        self.assertEqual(result, "reserved")
        raw = json.loads(api.store["ledger.json"][0])
        self.assertEqual(raw["days"][DAY]["total"], 1)

    def test_backward_compat_no_paid_key(self):
        api = FakeLedgerApi()
        data = {
            "version": 2,
            "days": {DAY: {"total": 0, "authors": {}, "wide_total": 0, "wide_authors": {}}},
            "reservations": [],
        }
        seed(api, data)
        result = ledger.reserve(api, BUDGET, 100, 1, HEAD, MAIN, "7", "1", CLOCK, TREE, "narrow")
        self.assertEqual(result, "reserved")


class StressTest(unittest.TestCase):
    def test_2000_reservation_compact_size(self):
        reservations = []
        for i in range(2000):
            reservations.append({
                "day": DAY, "number": i + 1, "author": 1000 + (i % 50),
                "head": f"{i:040x}", "base": MAIN,
                "tree": f"{i + 1:040x}", "kind": "narrow",
                "admitted_by": {"run": str(i + 100), "attempt": "1"},
                "claimed_by": {"run": str(i + 200), "attempt": "1"},
            })
        data = {
            "version": 2,
            "days": {DAY: {"total": 2000, "authors": {str(1000 + i): 40 for i in range(50)},
                           "wide_total": 0, "wide_authors": {}}},
            "reservations": reservations,
        }
        size = len(json.dumps(data, sort_keys=True, separators=(",", ":")))
        self.assertLess(size, 1_000_000)


if __name__ == "__main__":
    unittest.main()
