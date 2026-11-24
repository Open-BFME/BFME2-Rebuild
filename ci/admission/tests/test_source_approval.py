"""Check the real protected pre-reservation gate and the controller/receiver seam."""
import copy
import io
from contextlib import redirect_stdout
from pathlib import Path
import sys
from unittest.mock import patch
import json
import unittest

from test_admit import BUDGET, CLOCK, CONFIG, HEAD, MAIN, TREE, LedgerApi, admit, dispatches, fork_pr
from source_approval import CONTEXTS, Unavailable, require_source

POLICY = "d" * 64
RUN = "314159"
FILE = "src/example.cpp"


class ProtectedProof(LedgerApi):
    def __init__(self, pr=None, files=None):
        super().__init__(True, [pr or fork_pr()], {21: files if files is not None else [FILE]})
        target = f"https://github.com/{admit.REPO}/actions/runs/{RUN}"
        self.statuses[HEAD] = [
            {"context": context, "state": "success", "creator": {"id": 41898282}, "target_url": target,
             "description": f"Pass (main {MAIN[:12]})", "id": i}
            for i, context in enumerate(CONTEXTS, start=100)
        ]
        self.run = {"id": int(RUN), "run_attempt": 1, "path": ".github/workflows/managed-pr.yml",
                    "repository": {"id": admit.REPO_ID}, "head_sha": HEAD, "event": "pull_request_target",
                    "status": "completed", "conclusion": "success"}
        self.artifact = {"id": 8, "name": "bfme-source-receipt-1", "expired": False, "size_in_bytes": 211}
        self.receipt = {"repo": admit.REPO_ID, "number": 21, "base": MAIN, "head": HEAD, "tree": TREE,
                        "tools_commit": MAIN, "policy_sha256": POLICY, "run": RUN, "attempt": "1", "state": "pass"}

    def call(self, path):
        if path == f"actions/runs/{RUN}":
            return copy.deepcopy(self.run)
        if path == f"actions/runs/{RUN}/artifacts?per_page=100":
            return {"artifacts": [copy.deepcopy(self.artifact)]}
        return super().call(path)

    def proof(self, api, number, head, base, tree):
        return require_source(api, number, head, base, tree, POLICY, self.receive)

    def receive(self, artifact):
        if artifact != self.artifact["id"]:
            raise AssertionError("wrong artifact")
        return copy.deepcopy(self.receipt)


def handle(api, *, proof=None, **kwargs):
    return admit.handle(api, 21, CONFIG, MAIN, BUDGET, CLOCK,
                        source_proof=proof if proof is not None else api.proof, **kwargs)


class PriorSourceGate(unittest.TestCase):
    def test_exact_receipt_reserves_once(self):
        api = ProtectedProof()
        self.assertIn("admitted by rule", handle(api))
        reservation = admit.ledger.find(api, 21, HEAD, MAIN)
        self.assertEqual((reservation["kind"], reservation["tree"]), ("narrow", TREE))
        self.assertEqual(len(dispatches(api)), 1)

    def test_different_tree_policy_or_attempt_never_debits(self):
        for key, wrong in (("tree","b"*40), ("policy_sha256","a"*64), ("attempt","2")):
            with self.subTest(key=key):
                api = ProtectedProof()
                api.receipt[key] = wrong
                self.assertIn("nothing reserved", handle(api))
                self.assertEqual((api.store, dispatches(api)), ({},[]))

    def test_named_red_does_not_spin_and_never_debits(self):
        api = ProtectedProof()
        api.statuses[HEAD][0]["state"] = "failure"
        delays = []
        self.assertIn("source rules refused", handle(api, source_attempts=5, sleep=delays.append))
        self.assertEqual((api.store, dispatches(api), delays), ({},[],[]))

    def test_an_older_runs_per_rule_statuses_cannot_outlive_a_newer_pass(self):
        # Runs before the aggregate-only publisher also wrote BFME II / G1..G8. A newer run writes only the aggregate,
        # so an older per-rule refusal on this main must not refuse the newer passing aggregate.
        api = ProtectedProof()
        older = f"https://github.com/{admit.REPO}/actions/runs/27"
        for index, context in enumerate(("BFME II / source rules", *(f"BFME II / G{i}" for i in range(1, 9)))):
            api.statuses[HEAD].append({"context": context, "state": "failure" if index in (0, 3) else "success",
                                       "creator": {"id": 41898282}, "target_url": older,
                                       "description": f"Fix needed in G3 (main {MAIN[:12]})", "id": 50 + index})
        self.assertIn("admitted by rule", handle(api))

    def test_wrong_writer_or_different_run_is_held(self):
        api = ProtectedProof()
        api.statuses[HEAD][0]["creator"]["id"] = 1234
        self.assertIn("nothing reserved", handle(api))
        api.statuses[HEAD][0]["creator"]["id"] = 41898282
        api.statuses[HEAD][0]["target_url"] += "1"
        self.assertIn("nothing reserved", handle(api))
        self.assertEqual((api.store, dispatches(api)), ({},[]))

    def test_superseded_event_never_reserves_current_head(self):
        api = ProtectedProof()
        self.assertIn("a newer head", handle(api, expected_head="e"*40))
        self.assertEqual((api.store, dispatches(api)), ({},[]))

    def test_main_moves_between_source_receipt_and_reserve(self):
        api = ProtectedProof()

        def moved(*args):
            receipt = api.proof(*args)
            api.main = "e"*40
            return receipt

        self.assertIn("main, head or admission type moved", handle(api, proof=moved))
        self.assertEqual((api.store, dispatches(api)), ({},[]))

    def test_artifact_is_only_from_protected_expected_current_run(self):
        for field, different in (("path",".github/workflows/unrelated.yml"),("head_sha","a"*40),
                                 ("conclusion","failure"),("run_attempt",2)):
            with self.subTest(field=field):
                api = ProtectedProof()
                api.run[field] = different
                self.assertIn("nothing reserved", handle(api))
                self.assertEqual((api.store, dispatches(api)), ({},[]))

    def test_missing_receipt_can_arrive_later_without_recharge(self):
        api = ProtectedProof()
        delays = []
        require = api.proof

        def delayed(*args):
            if not delays:
                raise Unavailable("source report is running")
            return require(*args)

        self.assertIn("admitted by rule", handle(api, proof=delayed, source_attempts=2, sleep=delays.append))
        self.assertEqual(delays, [5])
        self.assertEqual(len(dispatches(api)), 1)

    def test_nonowner_trusted_narrow_spends_no_quota(self):
        for origin in ("member","same-repo"):
            with self.subTest(origin=origin):
                pr = fork_pr()
                if origin == "member":
                    pr["author_association"] = "MEMBER"
                else:
                    pr["head"]["repo"]["id"] = admit.REPO_ID
                api = ProtectedProof(pr)
                book = admit.ledger
                self.assertIn("admitted by rule", admit.handle(api, 21, CONFIG, MAIN, None, CLOCK,
                                                       source_proof=api.proof))
                ledger = json.loads(api.store["ledger.json"][0])
                self.assertEqual(ledger["days"], {})
                item = book.find(api, 21, HEAD, MAIN)
                self.assertEqual(item["kind"], "trusted-narrow")
                self.assertEqual(book.claim(api, 21, HEAD, MAIN, "900", "1", CLOCK, tree=TREE,
                                            kind="narrow", admitted_by=item["admitted_by"]), "kind-mismatch")
                self.assertEqual(book.claim(api, 21, HEAD, MAIN, "900", "1", CLOCK, tree=TREE,
                                            kind="trusted-narrow", admitted_by=item["admitted_by"]), "claimed")
                self.assertEqual(len(dispatches(api)), 1)

    def test_owner_origin_trusted_stays_direct(self):
        pr = fork_pr()
        pr["author_association"] = "OWNER"
        api = ProtectedProof(pr)
        self.assertIn("protected direct", handle(api, owner_origin=True))
        self.assertEqual((api.store, dispatches(api)), ({},[]))

    def test_nine_source_paths_and_protected_workflow_cannot_debit(self):
        pr = fork_pr(labels=("boat-ok",))
        pr["author_association"] = "MEMBER"
        for files in ([f"src/{i}.cpp" for i in range(9)], [FILE,".github/workflows/rules.yml"]):
            with self.subTest(files=files):
                api = ProtectedProof(pr, files)
                handle(api, owner_origin=True)
                self.assertEqual((api.store, dispatches(api)), ({},[]))

    def test_stale_issuer_remains_ineligible_after_refresh(self):
        api = ProtectedProof()
        handle(api)
        ledger = admit.ledger
        before = ledger.find(api, 21, HEAD, MAIN)
        self.assertEqual(ledger.refresh_issuer(api, 21, HEAD, MAIN, "72", "1"), "refreshed")
        same = dict(tree=TREE, kind="narrow")
        self.assertEqual(ledger.claim(api, 21, HEAD, MAIN, "900", "1", CLOCK, **same,
                                      admitted_by=before["admitted_by"]), "issuer-mismatch")
        current = ledger.find(api, 21, HEAD, MAIN)
        self.assertIsNone(current["claimed_by"])
        self.assertEqual(ledger.claim(api, 21, HEAD, MAIN, "900", "1", CLOCK, **same,
                                      admitted_by=current["admitted_by"]), "claimed")



NEWER = "e" * 40          # main after another pull request landed; GitHub's preview stays on MAIN


class MovedMain(unittest.TestCase):
    """After main moves, GitHub keeps every open pull request's merge preview on the old main. Admission takes the
    tree from the exact source receipt for the new main, or asks the source rules for one (bounded), never waits on
    a preview GitHub does not recompute."""

    def api(self):
        api = ProtectedProof()
        api.main = NEWER                       # preview_base stays MAIN: a stale preview
        api.receipt.update(base=NEWER, tools_commit=NEWER)
        for row in api.statuses[HEAD]:
            row["description"] = f"Pass (main {NEWER[:12]})"
        return api

    @staticmethod
    def tree(api, number, head, base):
        return admit.source_approval.receipt_for(api, number, head, base, POLICY, api.receive)["tree"]

    def decide(self, api, mode="retry", **kwargs):
        return admit.handle(api, 21, CONFIG, NEWER, BUDGET, CLOCK, sleep=lambda s: None, source_proof=api.proof,
                            refresh=mode != "event", find_tree=self.tree, **kwargs)

    def refreshes(self, api):
        return [item for item in dispatches(api) if item["event_type"] == admit.REFRESH_EVENT]

    def test_a_stale_preview_with_the_exact_receipt_reserves_on_the_new_main(self):
        api = self.api()
        self.assertEqual(admit.merge_preview(api, 21, HEAD, NEWER, sleep=lambda s: None), admit.STALE)
        self.assertIn("admitted by rule", self.decide(api))
        reservation = admit.ledger.find(api, 21, HEAD, NEWER)
        self.assertEqual((reservation["tree"], reservation["kind"]), (TREE, "narrow"))
        self.assertEqual(dispatches(api), [{"event_type": "bfme-remeasure",
                                            "client_payload": {"number": 21, "head": HEAD, "base": NEWER}}])
        self.assertEqual(api.statuses[HEAD][-1]["description"], admit.admitted_text(NEWER))

    def test_no_receipt_on_the_moved_main_requests_the_source_rules_once(self):
        api = self.api()
        for row in api.statuses[HEAD]:
            row["description"] = f"Pass (main {MAIN[:12]})"     # only the old main's source rules exist
        self.assertIn("source rules requested", self.decide(api))
        self.assertEqual(self.refreshes(api), [{"event_type": admit.REFRESH_EVENT,
                                                "client_payload": {"number": 21, "head": HEAD, "base": NEWER}}])
        self.assertEqual(api.statuses[HEAD][-1]["state"], "pending")
        self.assertEqual(api.statuses[HEAD][-1]["description"], admit.refresh_text(NEWER))
        self.assertEqual(api.store, {})

    def test_the_refresh_is_bounded_to_one_per_commit_per_main_per_hour(self):
        api = self.api()
        for row in api.statuses[HEAD]:
            row["description"] = f"Pass (main {MAIN[:12]})"
        self.decide(api)
        api.statuses[HEAD][-1]["created_at"] = "2026-11-20T19:10:00Z"      # 30 minutes before CLOCK
        for _ in range(3):                                                 # hourly runs and landing requests
            self.assertIn("waiting for the source rules", self.decide(api))
        self.assertEqual(len(self.refreshes(api)), 1)
        api.statuses[HEAD][-1]["created_at"] = "2026-11-20T18:39:00Z"      # 61 minutes before CLOCK
        self.assertIn("source rules requested", self.decide(api))
        self.assertEqual(len(self.refreshes(api)), 2)
        api.statuses[HEAD][-1].pop("created_at", None)                         # unreadable time: treated as recent
        self.assertIn("waiting for the source rules", self.decide(api))
        self.assertEqual(len(self.refreshes(api)), 2)
        self.assertEqual(api.store, {})

    def test_a_conflict_with_the_moved_main_is_named_and_reserves_nothing(self):
        api = self.api()
        api.statuses[HEAD].append({"context": admit.SOURCE_CONTEXT, "state": "error", "creator": {"id": 41898282},
                                   "description": f"{admit.SOURCE_CONFLICT} (main {NEWER[:12]})", "id": 999,
                                   "target_url": f"https://github.com/{admit.REPO}/actions/runs/{RUN}"})
        self.assertIn("conflicts with main", self.decide(api))
        self.assertEqual(api.statuses[HEAD][-1]["description"], admit.CONFLICT)
        self.assertEqual((dispatches(api), api.store), ([], {}))
        # a conflict recorded for another main says nothing about this one
        api = self.api()
        api.statuses[HEAD].append({"context": admit.SOURCE_CONTEXT, "state": "error", "creator": {"id": 41898282},
                                   "description": f"{admit.SOURCE_CONFLICT} (main {MAIN[:12]})", "id": 999})
        self.assertNotIn("conflicts with main", self.decide(api))

    def test_a_branch_without_shared_history_is_named_once_and_never_re_requested(self):
        api = self.api()
        api.statuses[HEAD].append({"context": admit.SOURCE_CONTEXT, "state": "error", "creator": {"id": 41898282},
                                   "description": f"{admit.SOURCE_HISTORY} (main {NEWER[:12]})", "id": 999})
        for _ in range(3):
            self.assertIn("shares no history with main", self.decide(api))
        posted = [row for row in api.statuses[HEAD] if row.get("context") == admit.CONTEXT]
        self.assertEqual([row["description"] for row in posted], [admit.HISTORY])
        self.assertEqual((dispatches(api), api.store), ([], {}))

    def test_a_receipt_for_another_main_policy_or_pull_request_is_never_used(self):
        for key, wrong in (("base", MAIN), ("tools_commit", MAIN), ("policy_sha256", "a" * 64), ("number", 22),
                           ("head", "2" * 40), ("tree", "not-a-tree"), ("state", "fail")):
            with self.subTest(key=key):
                api = self.api()
                api.receipt[key] = wrong
                line = self.decide(api)
                self.assertIn("source rules requested", line)
                self.assertEqual(api.store, {})
                self.assertEqual([item["event_type"] for item in dispatches(api)], [admit.REFRESH_EVENT])

    def test_a_request_waits_briefly_for_the_receipt_of_the_run_that_sent_it(self):
        api = self.api()
        calls = []

        def late(api_, number, head, base):
            calls.append(1)
            if len(calls) < 3:                          # the source rules run is still finishing its last job
                raise admit.source_approval.Unavailable("in progress")
            return self.tree(api_, number, head, base)
        delays = []
        line = admit.handle(api, 21, CONFIG, NEWER, BUDGET, CLOCK, sleep=delays.append, source_proof=api.proof,
                            refresh=True, find_tree=late, refresh_attempts=12)
        self.assertIn("admitted by rule", line)
        self.assertEqual((len(calls), delays), (3, [5, 5]))
        self.assertEqual([item["event_type"] for item in dispatches(api)], ["bfme-remeasure"])
        # never found: twelve looks, then one refresh request, no more
        api, delays = self.api(), []
        for row in api.statuses[HEAD]:
            row["description"] = f"Pass (main {MAIN[:12]})"
        admit.handle(api, 21, CONFIG, NEWER, BUDGET, CLOCK, sleep=delays.append, source_proof=api.proof,
                     refresh=True, find_tree=self.tree, refresh_attempts=12)
        self.assertEqual(len(delays), 11)
        self.assertEqual([item["event_type"] for item in dispatches(api)], [admit.REFRESH_EVENT])

    def test_githubs_stale_conflict_flag_is_checked_on_the_current_main(self):
        # GitHub's mergeable flag was computed on the old main; the receipt for the new main proves a clean merge.
        api = self.api()
        api.prs[21]["mergeable"] = False
        self.assertIn("admitted by rule", self.decide(api))
        self.assertEqual(admit.ledger.find(api, 21, HEAD, NEWER)["tree"], TREE)
        # no receipt yet: one source refresh, and the conflict is named only when the source rules say so
        api = self.api()
        api.prs[21]["mergeable"] = False
        for row in api.statuses[HEAD]:
            row["description"] = f"Pass (main {MAIN[:12]})"
        self.assertIn("source rules requested", self.decide(api))
        api.statuses[HEAD].append({"context": admit.SOURCE_CONTEXT, "state": "error", "creator": {"id": 41898282},
                                   "description": f"{admit.SOURCE_CONFLICT} (main {NEWER[:12]})", "id": 999})
        self.assertIn("conflicts with main", self.decide(api))
        self.assertEqual(api.statuses[HEAD][-1]["description"], admit.CONFLICT)
        self.assertEqual(len(dispatches(api)), 1)
        # the pull request event itself still trusts a fresh flag
        api = self.api()
        api.prs[21]["mergeable"] = False
        self.assertIn("conflicts with main", self.decide(api, mode="event"))

    def fresh(self):
        api = self.api()
        api.preview_base = NEWER                                   # GitHub did recompute the preview for the new main
        for row in api.statuses[HEAD]:
            row["description"] = f"Pass (main {MAIN[:12]})"        # but only the old main's source rules exist
        return api

    def test_a_fresh_preview_without_a_receipt_requests_the_source_rules_once_an_hour(self):
        api = self.fresh()
        self.assertEqual(admit.merge_preview(api, 21, HEAD, NEWER, sleep=lambda s: None), TREE)
        self.assertIn("source rules requested", self.decide(api))
        self.assertEqual(self.refreshes(api), [{"event_type": admit.REFRESH_EVENT,
                                                "client_payload": {"number": 21, "head": HEAD, "base": NEWER}}])
        api.statuses[HEAD][-1]["created_at"] = "2026-11-20T19:10:00Z"
        self.assertIn("waiting for the source rules", self.decide(api, mode="request"))
        self.assertEqual((len(self.refreshes(api)), api.store), (1, {}))
        # the pull request event keeps waiting for its own source run
        api = self.fresh()
        self.assertIn(admit.SOURCE_HOLD, self.decide(api, mode="event"))
        self.assertEqual(dispatches(api), [])

    def test_a_fresh_preview_with_the_receipt_published_later_reserves(self):
        api = self.fresh()
        self.decide(api)
        for row in api.statuses[HEAD]:
            if row.get("context") in CONTEXTS:
                row["description"] = f"Pass (main {NEWER[:12]})"   # the refresh published for the new main
        self.assertIn("admitted by rule", self.decide(api, mode="request"))
        self.assertEqual(admit.ledger.find(api, 21, HEAD, NEWER)["tree"], TREE)

    def test_a_preview_github_never_finishes_takes_the_receipt_route(self):
        api = self.api()
        api.prs[21]["merge_commit_sha"] = None                     # mergeable null, no preview commit
        api.prs[21]["mergeable"] = None
        self.assertIsNone(admit.merge_preview(api, 21, HEAD, NEWER, sleep=lambda s: None))
        self.assertIn("admitted by rule", self.decide(api))
        api = self.api()
        api.prs[21]["merge_commit_sha"] = None
        for row in api.statuses[HEAD]:
            row["description"] = f"Pass (main {MAIN[:12]})"
        self.assertIn("source rules requested", self.decide(api))
        self.assertIn("not ready", self.decide(self.api_without_preview(), mode="event"))

    def api_without_preview(self):
        api = self.api()
        api.prs[21]["merge_commit_sha"] = None
        return api

    def test_a_receipt_whose_tree_differs_from_githubs_preview_still_reserves_its_own_tree(self):
        api = self.fresh()
        for row in api.statuses[HEAD]:
            row["description"] = f"Pass (main {NEWER[:12]})"
        api.receipt["tree"] = "b" * 40                             # the source rules' merge, not GitHub's
        self.assertIn("admitted by rule", self.decide(api))
        self.assertEqual(admit.ledger.find(api, 21, HEAD, NEWER)["tree"], "b" * 40)

    def no_receipt(self):
        api = self.api()
        for row in api.statuses[HEAD]:
            row["description"] = f"Pass (main {MAIN[:12]})"
        return api

    def test_an_intervening_event_status_does_not_reopen_the_hour(self):
        for later in (admit.PREVIEW_WAIT, admit.CONFLICT, admit.SOURCE_HOLD):
            with self.subTest(later=later):
                api = self.no_receipt()
                self.decide(api)
                api.statuses[HEAD][-1]["created_at"] = "2026-11-20T19:35:00Z"
                admit.post(api, HEAD, "pending", later)              # e.g. reopen / ready_for_review event
                self.assertIn("waiting for the source rules", self.decide(api, mode="request"))
                self.assertEqual(len(self.refreshes(api)), 1)

    def test_the_marker_counts_on_any_page_and_an_unreadable_list_fails_closed(self):
        api = self.no_receipt()
        self.decide(api)
        api.statuses[HEAD][-1]["created_at"] = "2026-11-20T19:35:00Z"
        for index in range(105):                                     # push the marker to page 2
            api.statuses[HEAD].append({"context": "other", "state": "success", "creator": {"id": 7}, "id": 5000 + index})
        self.assertIn("waiting for the source rules", self.decide(api))
        self.assertEqual(len(self.refreshes(api)), 1)
        api = self.no_receipt()
        for index in range(admit.STATUS_PAGES * 100):                 # more statuses than are read: no new request
            api.statuses[HEAD].append({"context": "other", "state": "success", "creator": {"id": 7}, "id": 9000 + index})
        self.assertIn("waiting for the source rules", self.decide(api))
        self.assertEqual(self.refreshes(api), [])

    def test_a_marker_or_status_from_anyone_but_the_bot_does_not_count(self):
        api = self.no_receipt()
        self.decide(api)
        api.statuses[HEAD][-1]["created_at"] = "2026-11-20T19:35:00Z"
        api.statuses[HEAD].append({"context": admit.CONTEXT, "state": "success", "creator": {"id": 1234},
                                   "description": "anything", "id": 7777})
        self.assertIn("waiting for the source rules", self.decide(api))
        self.assertEqual(len(self.refreshes(api)), 1)
        api = self.no_receipt()                                      # a forged marker does not suppress the request
        api.statuses[HEAD].append({"context": admit.CONTEXT, "state": "pending", "creator": {"id": 1234},
                                   "description": admit.refresh_text(NEWER), "id": 7777,
                                   "created_at": "2026-11-20T19:35:00Z"})
        self.assertIn("source rules requested", self.decide(api))
        self.assertEqual(len(self.refreshes(api)), 1)

    def test_a_request_on_a_fresh_preview_waits_for_the_run_that_sent_it(self):
        api = self.fresh()
        for row in api.statuses[HEAD]:
            row["description"] = f"Pass (main {NEWER[:12]})"
        api.receipt["tree"] = "b" * 40                            # GitHub's preview tree has no proof
        calls, delays = [], []

        def late(api_, number, head, base):
            calls.append(1)
            if len(calls) < 3:                                    # the source run is still being marked completed
                raise admit.source_approval.Unavailable("in progress")
            return self.tree(api_, number, head, base)
        line = admit.handle(api, 21, CONFIG, NEWER, BUDGET, CLOCK, sleep=delays.append, source_proof=api.proof,
                            refresh=True, find_tree=late, refresh_attempts=12)
        self.assertIn("admitted by rule", line)
        self.assertEqual((len(calls), delays), (3, [5, 5]))
        self.assertEqual([item["event_type"] for item in dispatches(api)], ["bfme-remeasure"])

    def test_a_dead_compiler_run_does_not_cost_the_commit_its_source_proof(self):
        # Live case, pull 34 on main 9fa818c2b3: the refresh run (rules.yml, success) published the source status and
        # the receipt; the compiler check then re-published it with its own run, and its Boat job died (failure).
        api = self.fresh()
        for row in api.statuses[HEAD]:
            row["description"] = f"Pass (main {NEWER[:12]})"        # the refresh run's statuses, older
        dead = "271828"
        api.dead_run = {"id": int(dead), "run_attempt": 1, "path": ".github/workflows/managed-pr.yml",
                        "repository": {"id": admit.REPO_ID}, "head_sha": NEWER, "event": "repository_dispatch",
                        "status": "completed", "conclusion": "failure"}
        for index, context in enumerate(CONTEXTS):
            api.statuses[HEAD].append({"context": context, "state": "success", "creator": {"id": 41898282},
                                       "target_url": f"https://github.com/{admit.REPO}/actions/runs/{dead}",
                                       "description": f"Pass (main {NEWER[:12]})", "id": 500 + index})
        original = api.call

        def call(path):
            if path == f"actions/runs/{dead}":
                return dict(api.dead_run)
            return original(path)
        api.call = call
        self.assertEqual(admit.source_approval.statuses(api, HEAD, NEWER), dead)      # the newest names the dead run
        self.assertEqual(self.tree(api, 21, HEAD, NEWER), TREE)                       # the earlier receipt is used
        api.receipt["tree"] = "b" * 40                                                # GitHub preview differs
        self.assertIn("admitted by rule", self.decide(api))
        self.assertEqual(admit.ledger.find(api, 21, HEAD, NEWER)["tree"], "b" * 40)
        self.assertEqual([item["event_type"] for item in dispatches(api)], ["bfme-remeasure"])

    def test_no_older_run_can_hide_a_newer_refusal_or_lend_another_mains_receipt(self):
        api = self.api()
        for index, context in enumerate(CONTEXTS):                   # a newer run refused this commit on this main
            api.statuses[HEAD].append({"context": context, "state": "failure",
                                       "creator": {"id": 41898282}, "id": 600 + index,
                                       "target_url": f"https://github.com/{admit.REPO}/actions/runs/31",
                                       "description": f"Pass (main {NEWER[:12]})"})
        self.assertIn("source rules refused", self.decide(api))
        api = self.api()
        for row in api.statuses[HEAD]:
            row["description"] = f"Pass (main {MAIN[:12]})"           # only another main's complete run
        self.assertIn("source rules requested", self.decide(api))
        self.assertEqual(api.store, {})

    def test_a_refused_source_verdict_on_the_moved_main_is_final(self):
        api = self.api()
        api.statuses[HEAD][0]["state"] = "failure"
        self.assertIn("source rules refused", self.decide(api))
        self.assertEqual((dispatches(api), api.store), ([], {}))

    def test_the_pull_request_event_keeps_waiting_for_its_own_source_run(self):
        api = self.api()
        self.assertIn("not ready", self.decide(api, mode="event"))
        self.assertEqual((dispatches(api), api.store), ([], {}))

    def test_retry_and_request_runs_take_the_moved_main_route(self):
        import tempfile as _tempfile
        from test_admit import env as make_env
        for mode, event in (("retry", "schedule"), ("request", "repository_dispatch")):
            with self.subTest(mode=mode):
                api = self.api()
                with _tempfile.NamedTemporaryFile("w", suffix=".json", delete=False) as stream:
                    json.dump({"action": "bfme-admit-request", "client_payload": {"number": 21}}, stream)
                with patch.object(admit, "source_tree", self.tree), patch.object(admit, "check_source", api.proof):
                    with redirect_stdout(io.StringIO()) as out:
                        admit.run([mode], make_env(event=event, sha=NEWER, path=stream.name), api=api,
                                  config=CONFIG, clock=CLOCK)
                self.assertIn("admitted by rule", out.getvalue())
                self.assertEqual(admit.ledger.find(api, 21, HEAD, NEWER)["tree"], TREE)

    def test_the_conflict_text_is_the_one_the_source_rules_write(self):
        sys.path.insert(0, str(Path(admit.__file__).resolve().parents[2] / "tools/gates"))
        import workflow as source_workflow
        self.assertEqual(admit.SOURCE_CONFLICT, source_workflow.CONFLICT_OUTCOME)
        self.assertEqual(admit.SOURCE_HISTORY, source_workflow.HISTORY_OUTCOME)
        self.assertIn("bfme-source-refresh", source_workflow.DISPATCH_ACTIONS)
        self.assertEqual(admit.REFRESH_EVENT, "bfme-source-refresh")


if __name__ == "__main__":
    unittest.main(verbosity=2)
