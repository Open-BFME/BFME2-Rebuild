"""Offline tests for automatic admission (ci/admission/admit.py).

The pull request shapes are the real fork shapes captured from a public repository
(ci/landing/tests/fixtures/fork-shapes.json, associations NONE and CONTRIBUTOR).
Run from the repository root:  python3 -m unittest discover -s ci/admission/tests -v
"""
import base64
import copy
import datetime
import io
import json
import re
from contextlib import redirect_stdout
from pathlib import Path
import sys
import tempfile
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "ci/admission"))
import admit  # noqa: E402
from api import Api, ReadOnly  # noqa: E402

SHAPES = json.loads((ROOT / "ci/landing/tests/fixtures/fork-shapes.json").read_text())
MAIN = "5458a191e8f7d75b0dfaf1f7de8b177c58314127"
HEAD = "1" * 40
TREE = "a1" * 20
PREVIEW = "f" * 40
CONFIG = {"max_paths": 60, "source_prefix": "src/"}
BUDGET = {"daily_total": 3, "daily_per_author": 2}
DAY = "2026-11-20"
CLOCK = lambda: datetime.datetime(2026, 11, 20, 19, 40, tzinfo=datetime.timezone.utc)  # noqa: E731


def fork_pr(association="NONE", head=HEAD, number=21, author=424242, draft=False, labels=()):
    shape = SHAPES[association]
    return {"number": number, "state": "open", "draft": draft, "user": {"id": author, "login": "a-stranger"},
            "mergeable": True, "merge_commit_sha": PREVIEW,
            "author_association": shape["author_association"], "labels": [{"name": n} for n in labels],
            "head": {"sha": head, "ref": "anything", "repo": dict(shape["head"]["repo"])},
            "base": {"ref": "main", "sha": MAIN, "repo": {"id": admit.REPO_ID}}}


class LedgerApi(Api):
    """A stateful stand-in for the GitHub API: pull requests, statuses, the ledger branch, dispatches."""

    def __init__(self, live, prs, files, ledger_failure=None):
        self._token, self.live, self.writes, self.run_id, self.run_attempt = "x" * 20, live, [], "7", "1"
        self.prs = {pr["number"]: pr for pr in prs}
        self.files = files
        self.statuses = {}
        self.store = {}
        self.branch = False
        self.ledger_failure = ledger_failure
        self.next_id = 1
        self.main = MAIN
        self.preview_base = MAIN          # the main GitHub's merge preview was computed on

    def _send(self, method, path, body):
        if method == "GET":
            if path.startswith("pulls?"):
                return 200, [copy.deepcopy(pr) for pr in self.prs.values()]
            if path.startswith("pulls/") and "/files" in path:
                return 200, [{"filename": name} if isinstance(name, str) else name
                             for name in self.files[int(path.split("/")[1])]]
            if path.startswith("pulls/"):
                return 200, copy.deepcopy(self.prs[int(path.split("/")[1])])
            if path.startswith("commits/"):
                if getattr(self, "status_failure", None):
                    return self.status_failure, {"message": "boom"}
                rows = list(reversed(self.statuses.get(path.split("/")[1], [])))
                found = re.search(r"[?&]page=([0-9]+)", path)
                page = int(found.group(1)) if found else 1
                return 200, rows[(page - 1) * 100:page * 100]
            if path.startswith("actions/runs/"):
                run = getattr(self, "runs", {}).get(path[len("actions/runs/"):])
                return (200, copy.deepcopy(run)) if run else (404, {})
            if path == "git/ref/heads/main":
                return 200, {"object": {"sha": self.main}}
            if path.startswith("git/commits/"):
                heads = [pr["head"]["sha"] for pr in self.prs.values() if pr.get("merge_commit_sha") == path[12:]]
                return 200, {"sha": path[12:], "tree": {"sha": TREE},
                             "parents": [{"sha": self.preview_base}, {"sha": heads[0] if heads else "0" * 40}]}
            if path == f"git/ref/heads/{admit.ledger.LEDGER_BRANCH}":
                return (200, {"object": {"sha": "c" * 40}}) if self.branch else (404, {})
            if path.startswith("contents/"):
                if self.ledger_failure:
                    return self.ledger_failure, {"message": "boom"}
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
        if path.startswith("statuses/"):
            sha = path.split("/")[1]
            self.statuses.setdefault(sha, []).append(dict(body, id=self.next_id, creator={"id": admit.BOT_ID}))
            self.next_id += 1
            return 201, {}
        if path == "dispatches":
            return 204, None
        return 404, {}


def env(live=True, event="pull_request_target", path="", sha=MAIN, total="3", per_author="2"):
    return {"GITHUB_REPOSITORY": admit.REPO, "GITHUB_REPOSITORY_ID": str(admit.REPO_ID),
            "GITHUB_REF": "refs/heads/main", "GITHUB_EVENT_NAME": event, "GITHUB_EVENT_PATH": path,
            "BFME_AUTO_ADMISSION": "yes" if live else "", "GITHUB_RUN_ID": "7", "GITHUB_RUN_ATTEMPT": "1",
            "GITHUB_SHA": sha, "BFME_ADMISSION_DAILY_TOTAL": total, "BFME_ADMISSION_DAILY_PER_AUTHOR": per_author}


def handle(api, number=21, base=MAIN, budget=BUDGET, **kwargs):
    with redirect_stdout(io.StringIO()):
        return handle_with_proof(api, number, CONFIG, base, budget, CLOCK, sleep=lambda seconds: None, **kwargs)


def latest(api, head=HEAD):
    return api.statuses[head][-1]


def dispatches(api):
    return [w[2] for w in api.writes if w[1] == "dispatches"]


# These unit tests exercise the controller around the exact-receipt seam.
# The integrated proof tests below exercise its real source receipt.
def handle_with_proof(*args, **kwargs):
    return admit.handle(*args, **kwargs, source_proof=lambda *_: None)


def run_with_proof(*args, **kwargs):
    with patch.object(admit, "check_source", lambda *_: None):
        return admit.run(*args, **kwargs)


class Decide(unittest.TestCase):
    def verdict(self, files, **kwargs):
        return admit.decide(fork_pr(**kwargs), files, CONFIG)

    def test_source_only_fork_is_admitted(self):
        for association in SHAPES:
            self.assertEqual(self.verdict(["src/Code/A.cpp"], association=association)[0], "admit")

    def test_members_and_same_repository_take_budget_free_bot_route(self):
        pr = fork_pr()
        pr["author_association"] = "MEMBER"
        self.assertEqual(admit.decide(pr, ["src/a.cpp"], CONFIG)[0], "admit-trusted")
        pr = fork_pr()
        pr["head"]["repo"] = {"id": admit.REPO_ID}
        self.assertEqual(admit.decide(pr, ["src/a.cpp"], CONFIG)[0], "admit-trusted")

    def test_workflow_change_never_reaches_the_compiler(self):
        verdict, state, text = self.verdict(["src/a.cpp", ".github/workflows/managed-pr.yml"], labels=("boat-ok",))
        self.assertEqual((verdict, state), ("never", "failure"))
        self.assertIn(".github/", text)

    def test_maintainer_paths_wait_as_before(self):
        for path in ("include/GameLogic/Object.h", "ci/x.py", "tools/gates/y.py", "config/z.json",
                     "expected/a.json", "baseline/game.dat", "README.md"):
            verdict, state, text = self.verdict(["src/a.cpp", path])
            self.assertEqual((verdict, state), ("wait", "pending"), path)
            self.assertTrue(text.startswith("Waiting for a maintainer"))

    def test_a_rename_out_of_a_maintainer_path_waits(self):
        verdict, _, _ = admit.decide(fork_pr(), ["src/a.h", "include/a.h"], CONFIG)
        self.assertEqual(verdict, "wait")

    def test_too_many_files_wait(self):
        self.assertEqual(self.verdict([f"src/f{i}.cpp" for i in range(61)])[0], "wait")

    def test_draft_and_deleted_fork(self):
        self.assertEqual(self.verdict(["src/a.cpp"], draft=True)[0], "draft")
        pr = fork_pr()
        pr["head"]["repo"] = None
        self.assertEqual(admit.decide(pr, ["src/a.cpp"], CONFIG)[0], "never")


class Handle(unittest.TestCase):
    def test_admission_writes_status_ledger_and_the_existing_dispatch(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/Code/A.cpp"]})
        line = handle(api)
        self.assertIn("admitted by rule", line)
        self.assertEqual((latest(api)["state"], latest(api)["description"]), ("success", admit.admitted_text(MAIN)))
        self.assertEqual(dispatches(api), [{"event_type": "bfme-remeasure",
                                            "client_payload": {"number": 21, "head": HEAD, "base": MAIN}}])
        ledger = json.loads(api.store["ledger.json"][0])
        counts = ledger["days"][DAY]
        self.assertEqual((counts["total"], counts["authors"]), (1, {"424242": 1}))
        self.assertEqual(ledger["reservations"], [{"day": DAY, "number": 21, "author": 424242, "head": HEAD, "base": MAIN,
                                                   "tree": TREE, "kind": "narrow",
                                                   "admitted_by": {"run": "7", "attempt": "1"},
                                                   "claimed_by": None}])
        # The reservation is written before the status, and the status before the dispatch.
        order = [w[1].split("/")[0] for w in api.writes if w[1].split("/")[0] in ("contents", "statuses", "dispatches")]
        self.assertEqual(order, ["contents", "statuses", "dispatches"])
        self.assertTrue(api.branch)

    def test_same_commit_is_counted_once_and_measured_once(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        handle(api)
        # Rapid duplicate event before claim does NOT double-dispatch.
        self.assertIn("already dispatched", handle(api))
        self.assertEqual(len(dispatches(api)), 1)
        self.assertEqual(json.loads(api.store["ledger.json"][0])["days"][DAY]["total"], 1)
        # Hourly refresh re-dispatches a stranded unclaimed reservation (still counting nothing).
        self.assertIn("admitted by rule", handle(api, refresh=True))
        self.assertEqual(len(dispatches(api)), 2)
        self.assertEqual(json.loads(api.store["ledger.json"][0])["days"][DAY]["total"], 1)
        # Claimed: measured, nothing more is dispatched.
        self.assertEqual(admit.ledger.claim(api, 21, HEAD, MAIN, "900", "1", CLOCK, tree=TREE), "claimed")
        self.assertIn("reservation was already used", handle(api))
        self.assertEqual(len(dispatches(api)), 2)

    def test_outage_does_not_overwrite_already_measured_pr_status(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        handle(api)
        self.assertEqual(admit.ledger.claim(api, 21, HEAD, MAIN, "900", "1", CLOCK, tree=TREE), "claimed")
        now_s = CLOCK().strftime("%Y-%m-%dT%H:%M:%SZ")
        orig_call = api.call
        def outage_call(path):
            if path.startswith("actions/workflows/managed-pr.yml/runs"):
                return {"workflow_runs": [
                    {"id": 100 + i, "event": "repository_dispatch", "status": "completed", "conclusion": "failure", "updated_at": now_s}
                    for i in range(4)
                ]}
            if path.startswith("actions/runs/") and "/jobs" in path:
                return {"jobs": [{"name": "Isolated Boat compilershard 1", "conclusion": "failure"}]}
            return orig_call(path)
        api.call = outage_call
        line = handle(api)
        self.assertIn("reservation was already used", line)
        self.assertEqual(latest(api)["state"], "success")
        self.assertEqual(latest(api)["description"], admit.admitted_text(MAIN))

    def test_outage_breaker_counts_timed_out_boat_runs(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        now_s = CLOCK().strftime("%Y-%m-%dT%H:%M:%SZ")
        orig_call = api.call
        def timeout_call(path):
            if path.startswith("actions/workflows/managed-pr.yml/runs"):
                return {"workflow_runs": [
                    {"id": 200 + i, "event": "repository_dispatch", "status": "completed", "conclusion": "timed_out", "updated_at": now_s}
                    for i in range(4)
                ]}
            if path.startswith("actions/runs/") and "/jobs" in path:
                return {"jobs": [{"name": "Isolated Boat compilershard 1", "conclusion": "timed_out"}]}
            return orig_call(path)
        api.call = timeout_call
        line = handle(api)
        self.assertIn("paused, compiler runners unavailable", line)
        self.assertEqual(dispatches(api), [])

    def test_each_new_push_is_judged_again(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        handle(api)
        api.prs[21]["head"]["sha"] = "2" * 40
        self.assertIn("admitted by rule", handle(api))
        self.assertEqual(len(dispatches(api)), 2)
        self.assertEqual(json.loads(api.store["ledger.json"][0])["days"][DAY]["authors"], {"424242": 2})

    def test_per_author_limit_holds_with_a_plain_status(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        for n in range(3):
            api.prs[21]["head"]["sha"] = str(n + 1) * 40
            handle(api)
        self.assertEqual(latest(api, "3" * 40)["description"], "Held: daily limit, retried automatically")
        self.assertEqual(len(dispatches(api)), 2)

    def test_global_limit_holds(self):
        prs = [fork_pr(number=30 + n, head=str(n + 4) * 40, author=1000 + n) for n in range(4)]
        for n, pr in enumerate(prs):
            pr["merge_commit_sha"] = format(0xabc0 + n, "040x")
        api = LedgerApi(True, prs, {pr["number"]: ["src/a.cpp"] for pr in prs})
        for pr in prs:
            handle(api, pr["number"])
        self.assertEqual(len(dispatches(api)), 3)
        self.assertEqual(latest(api, "7" * 40)["description"], admit.DAILY_LIMIT)

    def test_unreadable_ledger_fails_closed(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]}, ledger_failure=500)
        self.assertIn("held", handle(api))
        self.assertEqual(latest(api)["description"], admit.DAILY_LIMIT)
        self.assertEqual(dispatches(api), [])

    def test_wait_status_is_written_once(self):
        api = LedgerApi(True, [fork_pr()], {21: ["include/a.h"]})
        handle(api)
        handle(api)
        self.assertEqual(len(api.statuses[HEAD]), 1)
        self.assertEqual(dispatches(api), [])

    def test_dry_run_writes_nothing(self):
        api = LedgerApi(False, [fork_pr()], {21: ["src/a.cpp"]})
        with self.assertRaises(ReadOnly):
            api.write("POST", "dispatches", {})
        handle(api)
        self.assertEqual(api.writes, [])

    def test_retry_admits_a_held_commit_after_the_day_turns(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        api.statuses[HEAD] = [{"id": 1, "context": admit.CONTEXT, "state": "pending",
                               "description": admit.DAILY_LIMIT, "creator": {"id": admit.BOT_ID}}]
        with redirect_stdout(io.StringIO()):
            run_with_proof(["retry"], env(event="schedule"), api=api, config=CONFIG, clock=CLOCK)
        self.assertEqual(latest(api)["description"], admit.admitted_text(MAIN))
        self.assertEqual(len(dispatches(api)), 1)


class Variables(unittest.TestCase):
    """Limits come from repository variables; missing or invalid limits admit nothing."""

    def test_limits_are_validated_like_the_compiler_check(self):
        self.assertEqual(admit.limits(env(total="150", per_author="30")), {"daily_total": 150, "daily_per_author": 30})
        for total, per_author in (("", ""), ("150", ""), ("30", "150"), ("0", "0"), ("6000", "1"), ("1.5", "1")):
            self.assertIsNone(admit.limits(env(total=total, per_author=per_author)), (total, per_author))

    def test_unset_limits_hold_without_reserving(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        self.assertIn("not configured", handle(api, budget=None))
        self.assertEqual(latest(api)["description"], admit.DAILY_LIMIT)
        self.assertEqual((dispatches(api), api.store), ([], {}))

    def test_switched_off_is_a_dry_run(self):
        api = LedgerApi(False, [fork_pr()], {21: ["src/a.cpp"]})
        with redirect_stdout(io.StringIO()) as out:
            run_with_proof(["retry"], env(live=False, event="schedule"), api=api, config=CONFIG, clock=CLOCK)
        self.assertEqual(api.writes, [])
        self.assertIn("Admission (dry run): #21: would check source receipts and reserve narrow commit", out.getvalue())

    def test_request_from_the_landing_workflow_reserves_on_the_runs_own_main(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        with tempfile.TemporaryDirectory() as tmp:
            path = Path(tmp, "event.json")
            path.write_text(json.dumps({"action": "bfme-admit-request", "client_payload": {"number": 21}}))
            with redirect_stdout(io.StringIO()):
                run_with_proof(["request"], env(event="repository_dispatch", path=str(path)), api=api, config=CONFIG,
                          clock=CLOCK)
        self.assertEqual(dispatches(api)[0]["client_payload"], {"number": 21, "head": HEAD, "base": MAIN})

    def test_a_run_started_on_an_older_main_decides_nothing(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        with redirect_stdout(io.StringIO()) as out:
            run_with_proof(["retry"], env(event="schedule", sha="9" * 40), api=api, config=CONFIG, clock=CLOCK)
        self.assertEqual(api.writes, [])
        self.assertIn("Superseded", out.getvalue())

    def test_the_reservation_not_the_status_decides(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        handle(api)
        admit.ledger.claim(api, 21, HEAD, MAIN, "900", "1", CLOCK, tree=TREE)
        self.assertIn("reservation was already used on main", handle(api))
        self.assertEqual(len(dispatches(api)), 1)
        newer = "e" * 40
        api.preview_base = api.main = newer
        self.assertIn("admitted by rule", handle(api, base=newer))
        self.assertEqual(dispatches(api)[-1]["client_payload"]["base"], newer)
        self.assertEqual(latest(api)["description"], admit.admitted_text(newer))

    def test_a_preview_computed_on_another_main_reserves_nothing(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        api.preview_base = "9" * 40
        self.assertIn("not ready", handle(api))
        self.assertEqual(latest(api)["description"], admit.PREVIEW_WAIT)
        self.assertEqual((dispatches(api), api.store), ([], {}))

    def test_a_conflict_reserves_nothing(self):
        pr = fork_pr()
        pr["mergeable"] = False
        api = LedgerApi(True, [pr], {21: ["src/a.cpp"]})
        self.assertIn("conflicts with main", handle(api))
        self.assertEqual(latest(api)["description"], admit.CONFLICT)
        self.assertEqual((dispatches(api), api.store), ([], {}))

    def test_claim_with_another_tree_is_refused_without_writing(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        handle(api)
        writes = len(api.writes)
        self.assertEqual(admit.ledger.claim(api, 21, HEAD, MAIN, "900", "1", CLOCK, tree="b2" * 20), "tree-mismatch")
        self.assertEqual(len(api.writes), writes)
        self.assertEqual(admit.ledger.claim(api, 21, HEAD, MAIN, "900", "1", CLOCK, tree=TREE), "claimed")


class Claim(unittest.TestCase):
    """The compiler check uses a reservation once; reruns and duplicate deliveries allocate nothing."""

    def reserved(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        handle(api)
        return api

    def test_first_claim_wins_and_reruns_or_duplicates_get_nothing(self):
        api = self.reserved()
        L = admit.ledger
        self.assertEqual(L.claim(api, 21, HEAD, MAIN, "900", "1", CLOCK), "claimed")
        self.assertEqual(L.claim(api, 21, HEAD, MAIN, "900", "1", CLOCK), "already-claimed-by-this-run")
        self.assertEqual(L.claim(api, 21, HEAD, MAIN, "900", "2", CLOCK), "claimed-by-other")
        self.assertEqual(L.claim(api, 21, HEAD, MAIN, "901", "1", CLOCK), "claimed-by-other")
        self.assertEqual(L.find(api, 21, HEAD, MAIN, CLOCK)["claimed_by"], {"run": "900", "attempt": "1"})

    def test_wrong_pull_request_head_or_base_finds_nothing(self):
        api = self.reserved()
        L = admit.ledger
        for number, head, base in ((22, HEAD, MAIN), (21, "9" * 40, MAIN), (21, HEAD, "8" * 40)):
            self.assertEqual(L.claim(api, number, head, base, "900", "1", CLOCK), "missing")

    def test_a_reservation_from_yesterday_can_still_be_claimed(self):
        api = self.reserved()
        tomorrow = lambda: CLOCK() + datetime.timedelta(days=1)  # noqa: E731
        self.assertEqual(admit.ledger.claim(api, 21, HEAD, MAIN, "900", "1", tomorrow), "claimed")

    def test_same_measurement_is_reserved_once(self):
        api = self.reserved()
        self.assertEqual(admit.ledger.reserve(api, BUDGET, 424242, 21, HEAD, MAIN, "8", "1", CLOCK, tree=TREE), "already")
        self.assertEqual(json.loads(api.store["ledger.json"][0])["days"][DAY]["total"], 1)


class SingleSnapshot(unittest.TestCase):
    """One ledger snapshot holds counters and keys; a stranded reservation is re-issued, not lost."""

    def seeded(self, day, base, claimed):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        api.branch = True
        record = {"version": 2, "days": {day: {"total": 1, "authors": {"424242": 1}, "wide_total": 0,
                                                "wide_authors": {}}},
                  "reservations": [{"day": day, "number": 21, "author": 424242, "head": HEAD, "base": base,
                                    "tree": TREE, "kind": "narrow", "admitted_by": {"run": "5", "attempt": "1"},
                                    "claimed_by": {"run": "800", "attempt": "1"} if claimed else None}]}
        api.store["ledger.json"] = (json.dumps(record), "0" * 40)
        return api

    def test_a_tuple_claimed_days_ago_on_the_same_main_is_not_measured_again(self):
        api = self.seeded("2026-11-17", MAIN, claimed=True)
        self.assertIn("reservation was already used", handle(api))
        self.assertEqual(dispatches(api), [])
        self.assertEqual(latest(api)["description"], admit.admitted_text(MAIN))

    def test_a_stranded_reservation_is_re_issued_to_this_run_and_dispatched(self):
        api = self.seeded(DAY, MAIN, claimed=False)
        self.assertIn("admitted by rule", handle(api))
        self.assertEqual(len(dispatches(api)), 1)
        item = json.loads(api.store["ledger.json"][0])["reservations"][0]
        self.assertEqual((item["admitted_by"], item["claimed_by"]), ({"run": "7", "attempt": "1"}, None))
        self.assertEqual(json.loads(api.store["ledger.json"][0])["days"][DAY]["total"], 1)

    def test_old_bases_are_pruned_on_the_next_write(self):
        api = self.seeded("2026-11-17", "9" * 40, claimed=True)
        handle(api)
        ledger = json.loads(api.store["ledger.json"][0])
        self.assertEqual([item["base"] for item in ledger["reservations"]], [MAIN])
        self.assertEqual(sorted(ledger["days"]), [DAY])


WIDE = {"daily_total": 2, "daily_per_author": 1}


class Headers(unittest.TestCase):
    """H10: with BFME_AUTO_HEADERS=yes, header changes are admitted to the wide check under their own budget."""

    def test_routing_with_and_without_the_switch(self):
        pr = fork_pr()
        self.assertEqual(admit.decide(pr, ["src/a.cpp", "include/a.h"], CONFIG)[0], "wait")
        self.assertEqual(admit.decide(pr, ["src/a.cpp", "include/a.h"], CONFIG, True)[0], "admit-wide")
        self.assertEqual(admit.decide(pr, ["include/a.h"], CONFIG, True)[0], "admit-wide")
        self.assertEqual(admit.decide(pr, ["src/a.cpp"], CONFIG, True)[0], "admit")
        for other in ("config/map.json", "tools/x.py", "ci/y.py", "expected/z.json", "baseline/game.dat"):
            self.assertEqual(admit.decide(pr, ["include/a.h", other], CONFIG, True)[0], "wait", other)
        self.assertEqual(admit.decide(pr, ["include/a.h", ".github/workflows/x.yml"], CONFIG, True)[0], "never")

    def test_wide_admission_has_its_own_budget_and_kind(self):
        api = LedgerApi(True, [fork_pr()], {21: ["include/a.h", "src/a.cpp"]})
        with redirect_stdout(io.StringIO()):
            line = handle_with_proof(api, 21, CONFIG, MAIN, BUDGET, CLOCK, sleep=lambda s: None, auto_headers=True,
                                wide_budget=WIDE)
        self.assertIn("admitted by rule", line)
        self.assertEqual(latest(api)["description"], admit.admitted_text(MAIN, "wide"))
        ledger = json.loads(api.store["ledger.json"][0])
        counts = ledger["days"][DAY]
        self.assertEqual((counts["total"], counts["wide_total"], counts["wide_authors"]), (0, 1, {"424242": 1}))
        self.assertEqual(ledger["reservations"][0]["kind"], "wide")

    def test_wide_limit_and_unset_wide_budget_hold(self):
        api = LedgerApi(True, [fork_pr()], {21: ["include/a.h"]})
        for n in range(2):
            api.prs[21]["head"]["sha"] = str(n + 1) * 40
            with redirect_stdout(io.StringIO()):
                handle_with_proof(api, 21, CONFIG, MAIN, BUDGET, CLOCK, sleep=lambda s: None, auto_headers=True,
                             wide_budget=WIDE)
        self.assertEqual(latest(api, "2" * 40)["description"], admit.DAILY_LIMIT)
        api = LedgerApi(True, [fork_pr()], {21: ["include/a.h"]})
        with redirect_stdout(io.StringIO()):
            handle_with_proof(api, 21, CONFIG, MAIN, BUDGET, CLOCK, sleep=lambda s: None, auto_headers=True,
                         wide_budget=None)
        self.assertEqual((latest(api)["description"], dispatches(api)), (admit.DAILY_LIMIT, []))

    def test_wide_limits_come_from_their_own_variables(self):
        environment = env() | {"BFME_ADMISSION_WIDE_DAILY_TOTAL": "20", "BFME_ADMISSION_WIDE_DAILY_PER_AUTHOR": "3"}
        self.assertEqual(admit.limits(environment, "wide"), {"daily_total": 20, "daily_per_author": 3})
        self.assertIsNone(admit.limits(env(), "wide"))


class TrustedAlreadyMeasured(unittest.TestCase):
    """A trusted head with a final compiler verdict on this main is not compared again; a pending one still is."""

    def member(self, state, description):
        pr = fork_pr()
        pr["author_association"] = "MEMBER"
        api = LedgerApi(True, [pr], {21: ["src/a.cpp"]})
        api.statuses[HEAD] = [{"id": 1, "context": "BFME II / Boat", "state": state, "description": description,
                               "creator": {"id": admit.BOT_ID}}]
        return api

    def test_final_verdict_on_this_main_skips(self):
        api = self.member("success", f"BFME II comparison passed (main {MAIN[:12]})")
        self.assertIn("already has a compiler verdict", handle(api))
        self.assertEqual((dispatches(api), api.store), ([], {}))

    def test_verdict_on_another_main_or_pending_does_not_skip(self):
        for state, description in (("success", "BFME II comparison passed (main 000000000000)"),
                                   ("pending", "Waiting for protected intake and Boat comparison"),
                                   ("error", f"Boat comparison unavailable. Re-run all jobs. (main {MAIN[:12]})"),
                                   ("success", f"Something else (main {MAIN[:12]})")):
            api = self.member(state, description)
            self.assertNotIn("already has a compiler verdict", handle(api), (state, description))


class SourceWaitFreshness(unittest.TestCase):
    """An exhausted source wait never writes a held status for a commit that is no longer current."""

    def run_wait(self, change):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        calls = {"n": 0}

        def missing(api_, number, head, base, tree):
            calls["n"] += 1
            if calls["n"] == 2:
                change(api)
            raise admit.source_approval.Unavailable("not ready")
        with redirect_stdout(io.StringIO()):
            line = admit.handle(api, 21, CONFIG, MAIN, BUDGET, CLOCK, sleep=lambda s: None, source_proof=missing,
                                source_attempts=4)
        return api, line

    def test_main_head_or_draft_moving_writes_nothing(self):
        def main_moved(api):
            api._send_main = "9" * 40
        for label, change in (("head", lambda api: api.prs[21]["head"].update(sha="2" * 40)),
                              ("draft", lambda api: api.prs[21].update(draft=True)),
                              ("closed", lambda api: api.prs[21].update(state="closed"))):
            api, line = self.run_wait(change)
            self.assertIn("moved; nothing reserved", line, label)
            self.assertEqual((api.statuses.get(HEAD), api.store, dispatches(api)), (None, {}, []), label)

    def test_still_current_commit_is_held_once(self):
        api, line = self.run_wait(lambda api: None)
        self.assertIn("nothing reserved", line)
        self.assertEqual([item["description"] for item in api.statuses[HEAD]], [admit.SOURCE_HOLD])
        with redirect_stdout(io.StringIO()):
            admit.handle(api, 21, CONFIG, MAIN, BUDGET, CLOCK, sleep=lambda s: None,
                         source_proof=lambda *args: (_ for _ in ()).throw(admit.source_approval.Unavailable("x")),
                         source_attempts=1)
        self.assertEqual(len(api.statuses[HEAD]), 1)


def compiler_run(run_id="900", status="completed", updated="2026-11-20T19:30:00Z", path=None, repo=None,
                 conclusion="failure", event="repository_dispatch", head_sha=None, attempt=1):
    return {"id": int(run_id), "status": status, "updated_at": updated, "conclusion": conclusion, "event": event,
            "head_sha": MAIN if head_sha is None else head_sha, "run_attempt": attempt,
            "path": admit.COMPILER_WORKFLOW if path is None else path,
            "repository": {"id": admit.REPO_ID if repo is None else repo}}


class UnmeasuredRun(unittest.TestCase):
    """A compiler check that used its reservation and then died on a machine fault is retried, within bounds."""

    def used(self, **run):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        handle(api)
        self.assertEqual(admit.ledger.claim(api, 21, HEAD, MAIN, "900", "1", CLOCK, tree=TREE), "claimed")
        api.runs = {"900": compiler_run(**run)}
        return api

    def item(self, api):
        return json.loads(api.store["ledger.json"][0])["reservations"][0]

    def status(self, api, context, state, text, creator=None):
        api.statuses.setdefault(HEAD, []).append({"context": context, "state": state, "description": text,
                                                  "id": api.next_id,
                                                  "creator": {"id": admit.BOT_ID if creator is None else creator}})
        api.next_id += 1

    def boat(self, api, state, text):
        self.status(api, admit.BOAT_CONTEXT, state, text)

    def stays_used(self, api, note=None):
        self.assertIn("reservation was already used", handle(api), note)
        self.assertEqual(len(dispatches(api)), 1, note)
        self.assertEqual(self.item(api)["claimed_by"], {"run": "900", "attempt": "1"}, note)
        self.assertNotIn("reissues", self.item(api), note)

    def test_a_finished_run_without_a_verdict_is_given_another_run(self):
        api = self.used()
        self.boat(api, "pending", "Waiting for protected intake and Boat comparison")
        self.boat(api, "error", f"Boat comparison unavailable. Re-run all jobs. (main {MAIN[:12]})")
        self.assertIn("admitted by rule", handle(api))
        self.assertEqual(len(dispatches(api)), 2)
        item = self.item(api)
        self.assertEqual((item["claimed_by"], item["admitted_by"], item["reissues"]),
                         (None, {"run": "7", "attempt": "1"}, 1))
        counts = json.loads(api.store["ledger.json"][0])["days"][DAY]
        self.assertEqual((counts["total"], counts["authors"]), (1, {"424242": 1}))
        # The compiler check can claim it again, once.
        self.assertEqual(admit.ledger.claim(api, 21, HEAD, MAIN, "901", "1", CLOCK, tree=TREE), "claimed")
        self.assertEqual(admit.ledger.claim(api, 21, HEAD, MAIN, "902", "1", CLOCK, tree=TREE), "claimed-by-other")

    def test_a_measured_verdict_is_never_retried(self):
        for state, text in (("success", "BFME II comparison passed"), ("failure", "BFME II check refused this patch")):
            api = self.used()
            self.boat(api, state, f"{text} (main {MAIN[:12]})")
            self.stays_used(api, state)

    def test_a_verdict_hidden_by_later_statuses_still_counts(self):
        api = self.used()
        self.boat(api, "failure", f"BFME II check refused this patch (main {MAIN[:12]})")
        self.boat(api, "pending", "Waiting for protected intake and Boat comparison")
        self.boat(api, "error", f"Boat comparison unavailable. Re-run all jobs. (main {MAIN[:12]})")
        self.stays_used(api, "masked by a later error")

    def test_a_verdict_pushed_off_the_first_page_still_counts(self):
        api = self.used()
        self.boat(api, "failure", f"BFME II check refused this patch (main {MAIN[:12]})")
        for index in range(108):                            # twelve reopen events, nine rule statuses each
            self.status(api, f"BFME II / G{index % 9}", "success", "passed")
        self.stays_used(api, "verdict on page 2")

    def test_more_statuses_than_can_be_read_counts_as_measured(self):
        api = self.used()
        for index in range(100 * admit.STATUS_PAGES):
            self.status(api, "BFME II / G1", "success", "passed")
        self.stays_used(api, "page cap reached")

    def test_a_verdict_by_someone_else_or_for_another_main_is_not_a_verdict(self):
        api = self.used()
        self.status(api, admit.BOAT_CONTEXT, "success", f"BFME II comparison passed (main {MAIN[:12]})", creator=5)
        self.boat(api, "success", f"BFME II comparison passed (main {'e' * 12})")
        self.assertIn("admitted by rule", handle(api))
        self.assertEqual(self.item(api)["reissues"], 1)

    def test_any_doubt_about_the_claiming_run_leaves_the_reservation_used(self):
        cases = [dict(status="in_progress"), dict(status="queued"), dict(path=".github/workflows/rules.yml"),
                 dict(repo=1), dict(updated="2026-11-20T19:39:30Z"), dict(updated="not a time"),
                 dict(conclusion="success"), dict(conclusion=None), dict(conclusion="skipped"),
                 dict(attempt=2), dict(event="pull_request_target"), dict(event="workflow_dispatch"),
                 dict(head_sha="e" * 40)]
        for run in cases:
            self.stays_used(self.used(**run), run)
        api = self.used()
        api.runs = {}                                   # the run cannot be read
        self.stays_used(api, "unreadable run")

    def test_an_aware_clock_in_another_zone_is_compared_in_utc(self):
        api = self.used(updated="2026-11-20T19:39:30Z")
        zone = datetime.timezone(datetime.timedelta(hours=-5))
        early = lambda: datetime.datetime(2026, 11, 20, 14, 40, tzinfo=zone)  # noqa: E731
        self.assertIsNone(admit.unmeasured_claim(api, 21, HEAD, MAIN, early))
        later = lambda: datetime.datetime(2026, 11, 20, 14, 45, tzinfo=zone)  # noqa: E731
        self.assertEqual(admit.unmeasured_claim(api, 21, HEAD, MAIN, later), {"run": "900", "attempt": "1"})

    def test_an_unreadable_status_list_or_ledger_leaves_it_used(self):
        api = self.used()
        api.status_failure = 502
        self.assertIsNone(admit.unmeasured_claim(api, 21, HEAD, MAIN, CLOCK))
        api.status_failure = None
        api.ledger_failure = 502
        self.assertIsNone(admit.unmeasured_claim(api, 21, HEAD, MAIN, CLOCK))

    def test_at_most_four_retries_then_a_plain_hold(self):
        api = self.used()
        api_budget = {"daily_total": 1, "daily_per_author": 1}
        for number, run in enumerate(("901", "902", "903", "904"), start=1):
            self.assertIn("admitted by rule", handle(api, budget=api_budget))
            self.assertEqual(self.item(api)["reissues"], number)
            self.assertEqual(admit.ledger.claim(api, 21, HEAD, MAIN, run, "1", CLOCK, tree=TREE), "claimed")
            api.runs[run] = compiler_run(run)
        self.assertIn("unmeasured five times", handle(api, budget=api_budget))
        self.assertEqual(len(dispatches(api)), 5)
        self.assertEqual((latest(api)["state"], latest(api)["description"]), ("pending", admit.UNMEASURED))
        writes = len(api.writes)
        self.assertIn("unmeasured five times", handle(api, budget=api_budget))
        self.assertEqual(len(api.writes), writes)          # said once, written once

    def test_an_infra_retry_does_not_double_charge_daily_limits(self):
        api = self.used()
        self.assertIn("admitted by rule", handle(api, budget={"daily_total": 1, "daily_per_author": 1}))
        self.assertEqual(len(dispatches(api)), 2)
        self.assertEqual(json.loads(api.store["ledger.json"][0])["days"][DAY]["total"], 1)

    def test_boat_outage_circuit_breaker_pauses_admissions_without_burning_retries(self):
        api = self.used()
        orig_call = api.call
        def outage_call(path):
            if path.startswith("actions/workflows/managed-pr.yml/runs"):
                return {"workflow_runs": [
                    {"id": 1000 + i, "conclusion": "failure", "updated_at": "2026-11-20T19:44:00Z"}
                    for i in range(4)
                ]}
            if path.startswith("actions/runs/100"):
                return {"jobs": [
                    {"name": "Isolated Boat compiler and native comparison", "conclusion": "failure"},
                    {"name": "Report on the pull request head", "conclusion": "failure"},
                ]}
            return orig_call(path)
        api.call = outage_call
        msg = handle(api, budget={"daily_total": 9, "daily_per_author": 9})
        self.assertIn("compiler runners unavailable", msg)
        self.assertEqual((latest(api)["state"], latest(api)["description"]), ("pending", admit.OUTAGE))
        self.assertNotIn("reissues", self.item(api))

    def test_only_the_inspected_claim_is_released(self):
        api = self.used()
        other = {"run": "555", "attempt": "1"}
        self.assertEqual(admit.ledger.reissue_unmeasured(api, BUDGET, 21, HEAD, MAIN, other, "7", "1", CLOCK), "changed")
        self.assertEqual(admit.ledger.reissue_unmeasured(api, BUDGET, 21, HEAD, MAIN, None, "7", "1", CLOCK), "changed")
        self.assertEqual(admit.ledger.reissue_unmeasured(api, BUDGET, 22, HEAD, MAIN, other, "7", "1", CLOCK), "missing")
        self.assertEqual(self.item(api)["claimed_by"], {"run": "900", "attempt": "1"})
        self.assertNotIn("reissues", self.item(api))

    def test_an_unclaimed_reservation_is_not_counted_as_a_retry(self):
        api = LedgerApi(True, [fork_pr()], {21: ["src/a.cpp"]})
        handle(api)
        self.assertIn("admitted by rule", handle(api, refresh=True))      # stranded before its claim: re-dispatched on refresh
        self.assertNotIn("reissues", self.item(api))
        self.assertEqual(json.loads(api.store["ledger.json"][0])["days"][DAY]["total"], 1)



class DormantRefusal(unittest.TestCase):
    def _api_with_refusal(self, compare_files):
        old_main = "1" * 40
        pr = fork_pr(number=34)
        api = LedgerApi(True, [pr], {34: ["src/Game/Foo.cpp"]})
        api.statuses[HEAD] = [{
            "id": 99, "context": admit.BOAT_CONTEXT, "state": "failure",
            "description": f"BFME II check refused this patch (main {old_main[:12]})",
            "creator": {"id": admit.BOT_ID},
        }]
        orig_call = api.call
        def fake_call(path):
            if path == f"compare/{old_main[:12]}...{MAIN}":
                return {"status": "ahead", "files": [{"filename": f} for f in compare_files]}
            return orig_call(path)
        api.call = fake_call
        return api

    def test_unrelated_main_move_stays_dormant_on_refresh(self):
        api = self._api_with_refusal(["src/Game/Unrelated.cpp", "docs/agents.md"])
        with redirect_stdout(io.StringIO()):
            out = admit.handle(api, 34, CONFIG, MAIN, BUDGET, CLOCK, refresh=True, source_proof=lambda *_: None)
        self.assertIn("waiting for author push", out)
        self.assertEqual(dispatches(api), [])

    def test_relevant_main_move_wakes_up_refused_pr(self):
        for changed in (["include/Common/Thing.h"], ["ci/managed-check/compare.py"], ["src/Game/Foo.cpp"]):
            with self.subTest(changed=changed):
                api = self._api_with_refusal(changed)
                with redirect_stdout(io.StringIO()):
                    out = admit.handle(api, 34, CONFIG, MAIN, BUDGET, CLOCK, refresh=True, source_proof=lambda *_: None)
                self.assertIn("admitted by rule", out)


    def test_malformed_compare_payload_fails_open(self):
        api = self._api_with_refusal([])
        orig = api.call
        for bad in (
            {"status": "ahead", "files": [{"filename": 12345}]},
            {"status": "ahead", "files": [{}]},
            {"status": "ahead", "files": [{"filename": None}]},
            {"status": ["ahead"], "files": []},
        ):
            api = self._api_with_refusal([])
            orig = api.call
            api.call = lambda path, b=bad, o=orig: b if path.startswith("compare/") else o(path)
            with redirect_stdout(io.StringIO()):
                out = admit.handle(api, 34, CONFIG, MAIN, BUDGET, CLOCK, refresh=True, source_proof=lambda *_: None)
            self.assertIn("admitted by rule", out)
        return
        api.call = lambda path: {"status": "ahead", "files": [{"filename": 12345}]} if path.startswith("compare/") else orig(path)
        with redirect_stdout(io.StringIO()):
            out = admit.handle(api, 34, CONFIG, MAIN, BUDGET, CLOCK, refresh=True, source_proof=lambda *_: None)
        self.assertIn("admitted by rule", out)


    def test_header_pr_wakes_when_any_src_consumer_changes(self):
        old_main = "1" * 40
        pr = fork_pr(number=35)
        api = LedgerApi(True, [pr], {35: ["include/Game/Foo.h"]})
        api.statuses[HEAD] = [{
            "id": 99, "context": admit.BOAT_CONTEXT, "state": "failure",
            "description": f"BFME II check refused this patch (main {old_main[:12]})",
            "creator": {"id": admit.BOT_ID},
        }]
        orig = api.call
        api.call = lambda path: {"status": "ahead", "files": [{"filename": "src/Game/Consumer.cpp"}]} if path.startswith("compare/") else orig(path)
        with redirect_stdout(io.StringIO()):
            out = admit.handle(api, 35, CONFIG, MAIN, BUDGET, CLOCK, auto_headers=True,
                               wide_budget={"daily_total": 20, "daily_per_author": 5},
                               refresh=True, source_proof=lambda *_: None)
        self.assertIn("admitted by rule", out)



class Cut11SwarmTests(unittest.TestCase):
    def test_reserve_does_not_double_charge_same_head_when_main_moves(self):
        api = LedgerApi(True, [], {})
        limits = {"daily_total": 1, "daily_per_author": 1}
        m1, m2 = "1" * 40, "2" * 40
        r1 = admit.ledger.reserve(api, limits, 42, 7, HEAD, m1, "100", "1", CLOCK, tree="3" * 40, kind="wide")
        self.assertEqual(r1, "reserved")
        # Main moves to m2 while head is unchanged: must NOT consume a second quota slot.
        r2 = admit.ledger.reserve(api, limits, 42, 7, HEAD, m2, "101", "1", CLOCK, tree="4" * 40, kind="wide")
        self.assertEqual(r2, "reserved")
        data, _ = admit.ledger._read(api)
        today = admit.ledger.days(CLOCK)[0]
        self.assertEqual(data["days"][today]["wide_total"], 1)
        self.assertEqual(data["days"][today]["wide_authors"]["42"], 1)

    def test_retry_skips_boat_on_build_neutral_or_disjoint_ancestor_but_request_honors_landing(self):
        old_main = "1" * 40
        pr = fork_pr(number=44)
        api = LedgerApi(True, [pr], {44: ["src/Code/GameEngine/A.cpp"]})
        api.statuses[HEAD] = [
            {"id": 10, "context": admit.CONTEXT, "state": "success",
             "description": admit.ADMITTED, "creator": {"id": admit.BOT_ID}},
            {"id": 11, "context": admit.BOAT_CONTEXT, "state": "success",
             "description": f"BFME II comparison passed (main {old_main[:12]})",
             "creator": {"id": admit.BOT_ID}},
        ]
        orig = api.call
        def fake_call(path):
            if path.startswith("compare/"):
                return {"status": "ahead", "ahead_by": 2, "behind_by": 0,
                        "files": [{"filename": "docs/guide.md", "status": "modified"},
                                  {"filename": "src/Code/GameEngine/B.cpp", "status": "modified"}]}
            return orig(path)
        api.call = fake_call
        with redirect_stdout(io.StringIO()):
            out_retry = admit.handle(api, 44, CONFIG, MAIN, BUDGET, CLOCK, refresh=True,
                                     allow_ancestor_skip=True, source_proof=lambda *_: None)
        self.assertIn("Boat already passed on disjoint ancestor", out_retry)
        # Explicit landing request (allow_ancestor_skip=False) MUST still admit/remeasure.
        with redirect_stdout(io.StringIO()):
            out_req = admit.handle(api, 44, CONFIG, MAIN, BUDGET, CLOCK, refresh=True,
                                   allow_ancestor_skip=False, source_proof=lambda *_: None)
        self.assertIn("admitted by rule", out_req)

    def test_fetch_retry_candidates_batch_graphql_and_fallback(self):
        class GqlApi:
            def __init__(self, fail=False):
                self.fail = fail
            def query(self, q, variables):
                if self.fail:
                    raise admit.ApiError(502, "bad gateway")
                return {
                    "repository": {
                        "pullRequests": {
                            "pageInfo": {"hasNextPage": False, "endCursor": None},
                            "nodes": [{
                                "number": 50, "title": "Fix", "state": "OPEN", "isDraft": False,
                                "authorAssociation": "NONE", "baseRefName": "main", "headRefOid": HEAD,
                                "author": {"login": "bob", "databaseId": 77},
                                "headRepository": {"databaseId": 999}, "baseRepository": {"databaseId": 1464323350},
                                "files": {"totalCount": 1, "nodes": [{"path": "src/Code/A.cpp", "changeType": "MODIFIED"}]},
                                "commits": {"nodes": [{"commit": {"status": {"contexts": []}}}]},
                            }, {
                                "number": 51, "title": "Draft", "state": "OPEN", "isDraft": True,
                                "authorAssociation": "NONE", "baseRefName": "main", "headRefOid": HEAD,
                                "author": {"login": "bob", "databaseId": 77},
                                "headRepository": {"databaseId": 999}, "baseRepository": {"databaseId": 1464323350},
                                "files": {"totalCount": 1, "nodes": [{"path": "src/Code/B.cpp", "changeType": "MODIFIED"}]},
                                "commits": {"nodes": [{"commit": {"status": {"contexts": []}}}]},
                            }],
                        }
                    }
                }
        res = admit.fetch_retry_candidates_batch(GqlApi(fail=False))
        self.assertEqual([item["number"] for item in res], [50])
        self.assertEqual(res[0]["files"], ["src/Code/A.cpp"])
        self.assertIsNone(admit.fetch_retry_candidates_batch(GqlApi(fail=True)))



    def test_quota_waiver_does_not_slide_across_days(self):
        api = LedgerApi(True, [], {})
        limits = {"daily_total": 1, "daily_per_author": 1}
        day1 = lambda: datetime.datetime(2026, 11, 20, 12, 0, tzinfo=datetime.timezone.utc)
        day2 = lambda: datetime.datetime(2026, 11, 21, 12, 0, tzinfo=datetime.timezone.utc)
        day3 = lambda: datetime.datetime(2026, 11, 22, 12, 0, tzinfo=datetime.timezone.utc)
        self.assertEqual(admit.ledger.reserve(api, limits, 42, 7, HEAD, "1" * 40, "1", "1", day1, tree="a" * 40, kind="wide"), "reserved")
        # Same day, new base: free!
        self.assertEqual(admit.ledger.reserve(api, limits, 42, 7, HEAD, "2" * 40, "2", "1", day1, tree="b" * 40, kind="wide"), "reserved")
        # Next day: paid_day != today, so charges 1 on day2; a second distinct head on day2 hits over-budget!
        self.assertEqual(admit.ledger.reserve(api, limits, 42, 7, HEAD, "3" * 40, "3", "1", day2, tree="c" * 40, kind="wide"), "reserved")
        self.assertEqual(admit.ledger.reserve(api, limits, 42, 8, "9" * 40, "3" * 40, "4", "1", day2, tree="d" * 40, kind="wide"), "over-budget")
        # Day 3: charges 1 on day3 (never slides infinitely for free)
        self.assertEqual(admit.ledger.reserve(api, limits, 42, 7, HEAD, "4" * 40, "5", "1", day3, tree="e" * 40, kind="wide"), "reserved")
        data, _ = admit.ledger._read(api)
        self.assertEqual(data["days"]["2026-11-22"]["wide_total"], 1)

    def test_retry_does_not_skip_when_boat_pass_is_expired_or_beyond_32_commits(self):
        old_main = "1" * 40
        pr = fork_pr(number=45)
        api = LedgerApi(True, [pr], {45: ["src/Code/GameEngine/A.cpp"]})
        # 22 hours old -> outside 18h freshness window, must re-measure
        api.statuses[HEAD] = [
            {"id": 10, "context": admit.CONTEXT, "state": "success",
             "description": admit.ADMITTED, "creator": {"id": admit.BOT_ID}},
            {"id": 11, "context": admit.BOAT_CONTEXT, "state": "success",
             "description": f"BFME II comparison passed (main {old_main[:12]})",
             "created_at": "2026-11-19T14:00:00Z",
             "creator": {"id": admit.BOT_ID}},
        ]
        orig = api.call
        api.call = lambda path: ({"status": "ahead", "ahead_by": 2, "behind_by": 0,
                                  "files": [{"filename": "docs/guide.md", "status": "modified"}]}
                                 if path.startswith("compare/") else orig(path))
        with redirect_stdout(io.StringIO()):
            out_old = admit.handle(api, 45, CONFIG, MAIN, BUDGET, CLOCK, refresh=True,
                                   allow_ancestor_skip=True, source_proof=lambda *_: None)
        self.assertIn("admitted by rule", out_old)
        # Fresh timestamp but ahead_by=33 (>32): must NOT skip
        api2 = LedgerApi(True, [pr], {45: ["src/Code/GameEngine/A.cpp"]})
        api2.statuses[HEAD] = [
            {"id": 10, "context": admit.CONTEXT, "state": "success",
             "description": admit.ADMITTED, "creator": {"id": admit.BOT_ID}},
            {"id": 11, "context": admit.BOAT_CONTEXT, "state": "success",
             "description": f"BFME II comparison passed (main {old_main[:12]})",
             "created_at": "2026-11-20T11:00:00Z",
             "creator": {"id": admit.BOT_ID}},
        ]
        orig2 = api2.call
        api2.call = lambda path: ({"status": "ahead", "ahead_by": 33, "behind_by": 0,
                                   "files": [{"filename": "docs/guide.md", "status": "modified"}]}
                                  if path.startswith("compare/") else orig2(path))
        with redirect_stdout(io.StringIO()):
            out_far = admit.handle(api2, 45, CONFIG, MAIN, BUDGET, CLOCK, refresh=True,
                                   allow_ancestor_skip=True, source_proof=lambda *_: None)
        self.assertIn("admitted by rule", out_far)

    def test_prior_boat_pass_accepts_src_unk_and_nbench_and_rejects_code_tools(self):
        old_main = "1" * 40
        boat_st = {
            "id": 11, "context": admit.BOAT_CONTEXT, "state": "success",
            "description": f"BFME II comparison passed (main {old_main[:12]})",
            "created_at": "2026-11-20T11:00:00Z",
            "creator": {"id": admit.BOT_ID},
        }
        api = LedgerApi(True, [fork_pr(number=46)], {46: ["src/unk/O00123456.cpp"]})
        orig = api.call
        api.call = lambda path: ({"status": "ahead", "ahead_by": 3, "behind_by": 0,
                                  "files": [{"filename": "src/unk/O00999999.cpp", "status": "modified"},
                                            {"filename": "src/nbench/Benchmark/emfloat.c", "status": "modified"}]}
                                 if path.startswith("compare/") else orig(path))
        self.assertEqual(
            admit.prior_boat_pass(api, HEAD, MAIN, ["src/unk/O00123456.cpp"], boat_status=boat_st, clock=CLOCK),
            old_main[:12],
        )
        self.assertEqual(
            admit.prior_boat_pass(api, HEAD, MAIN, ["src/nbench/Benchmark/nbench0.c"], boat_status=boat_st, clock=CLOCK),
            old_main[:12],
        )
        self.assertIsNone(
            admit.prior_boat_pass(api, HEAD, MAIN, ["src/Code/Tools/WW3D/pluglib/matrix4.cpp"], boat_status=boat_st, clock=CLOCK),
        )


if __name__ == "__main__":
    unittest.main()
