"""Tests for ledger compact serialization, paid tracking, and pruning (ci/admission/ledger.py).

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
CLOCK = lambda: datetime.datetime(2026, 11, 20, 19, 40, tzinfo=datetime.timezone.utc)  # noqa: E731
MAIN = "5458a191e8f7d75b0dfaf1f7de8b177c58314127"
HEAD = "1" * 40
TREE = "a1" * 20
BUDGET = {"daily_total": 100, "daily_per_author": 50}


class FakeLedgerApi:
    def __init__(self):
        self._token, self.live, self.writes = "x" * 20, True, []
        self.store = {}
        self.branch = True

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
                return 200, {"content": base64.b64encode(text.encode()).decode(), "sha": sha}
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


class PaidTracking(unittest.TestCase):
    def test_prune_records_paid_for_superseded_base(self):
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
        self.assertEqual(data["reservations"], [])
        paid = data["days"][DAY].get("paid", [])
        self.assertEqual(paid, [[1, HEAD, "narrow", 100]])

    def test_unclaimed_today_reservation_on_old_base_kept(self):
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
        paid = data["days"][DAY].get("paid", [])
        self.assertEqual(paid, [[1, HEAD, "narrow", 100]])

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

    def test_paid_dedup(self):
        data = {
            "version": 2,
            "days": {DAY: {"total": 1, "authors": {"100": 1}, "wide_total": 0, "wide_authors": {},
                           "paid": [[1, HEAD, "narrow", 100]]}},
            "reservations": [
                {"day": DAY, "number": 1, "author": 100, "head": HEAD, "base": "a" * 40,
                 "tree": TREE, "kind": "narrow", "admitted_by": {"run": "7", "attempt": "1"},
                 "claimed_by": {"run": "800", "attempt": "1"}},
            ],
        }
        ledger._prune(data, YESTERDAY, "b" * 40)
        paid = data["days"][DAY].get("paid", [])
        self.assertEqual(len(paid), 1)

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
    def test_2000_reservation_stress_stays_under_1mb(self):
        old_base = "a" * 40
        new_base = "b" * 40
        reservations = []
        for i in range(2000):
            reservations.append({
                "day": DAY, "number": i + 1, "author": 1000 + (i % 50),
                "head": f"{i:040x}", "base": old_base,
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
        size_before = len(json.dumps(data, sort_keys=True, separators=(",", ":")))
        ledger._prune(data, YESTERDAY, new_base)
        size_after = len(json.dumps(data, sort_keys=True, separators=(",", ":")))
        self.assertEqual(len(data["reservations"]), 0)
        self.assertEqual(len(data["days"][DAY].get("paid", [])), 2000)
        self.assertLess(size_after, 500 * 1024)
        self.assertLess(size_after, size_before)


if __name__ == "__main__":
    unittest.main()
