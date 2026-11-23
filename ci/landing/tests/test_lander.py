"""Offline tests for the landing job, driven by captured read-only GitHub API payloads.

fixtures/live-pr2-pr3.json holds the open pull requests 2 to 7 as GitHub returned them
on 2026-11-20 (trimmed to the fields the lander reads) while main was 89db7cafe448,
plus the three receipts pull request 2's checks produced on main 879a5be4f21d.
Run from the repository root:  python3 -m unittest discover -s ci/landing/tests -v
"""
import copy
import io
import json
import os
from contextlib import redirect_stdout
from pathlib import Path
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "ci/landing"))
import lander  # noqa: E402
from api import Api, ReadOnly  # noqa: E402

FIXTURE = json.loads((Path(__file__).parent / "fixtures/live-pr2-pr3.json").read_text())
LIVE_MAIN = FIXTURE["main"]                              # main when captured; PR 2 is stale on it
MEASURED = "879a5be4f21dfdcabe5fef6e73d2b6a01fd77f94"    # main PR 2 and PR 3 were measured on
PR2 = "7eb1cdbb67eb375af71626adfc89f05b72c9b393"
PR3 = "e0cd7fef8dd4"
TREE = "44fa7dfb9e1d25379e7c64aa22069d5e03770223"
NEW = "ab" * 20
CONFIG = {"approvers": set(), "exceptions": {(2, PR2)}, "remeasure": "repository_dispatch"}


class FakeApi(Api):
    def __init__(self, live, main=LIVE_MAIN, edit=None):
        self._token, self.live, self.writes = "x" * 20, live, []
        self.fx = copy.deepcopy(FIXTURE["api"])
        self.fx["git/ref/heads/main"] = {"object": {"sha": main}}
        self.responses = {}
        self.after_write = {}
        self.after_read = []
        if edit:
            edit(self.fx)

    def _send(self, method, path, body):
        if method == "GET":
            if self.after_read:
                self.after_read.pop(0)(self.fx)
            if path in self.fx:
                return 200, copy.deepcopy(self.fx[path])
            if path.startswith("git/commits/"):
                return 200, {"sha": path[12:], "message": "An ordinary commit\n", "parents": [], "tree": {"sha": "0" * 40}}
            if path.startswith("commits/") and "/statuses" in path:
                return 200, []
            return 404, {"message": "Not Found"}
        key = (method, path.split("/")[0] if method == "POST" else path)
        code, payload = self.responses.get(key, ({"POST": 201, "PUT": 200}[method], {}))
        if path == "dispatches":
            code = 204
        hook = self.after_write.get(key)
        if hook:
            hook(self.fx)
        return code, payload


def env(main=MEASURED, mode="dry-run", pr="", run="1"):
    return {"GITHUB_REPOSITORY": "Open-BFME/BFME2-Rebuild", "GITHUB_REPOSITORY_ID": "1464323350",
            "GITHUB_REF": "refs/heads/main", "GITHUB_SHA": main, "BFME_LANDING_MODE": mode,
            "BFME_LANDING_PR": pr, "GITHUB_RUN_ID": run}


def put_receipts(directory, drop_inline=False):
    from gate import policy_digest
    live_inline_policy = lander.lr.policy()
    live_rules_policy = policy_digest(lander.lr.TOOLS / "config.json")
    for name, record in FIXTURE["receipts"].items():
        if drop_inline and name.startswith("inline/"):
            continue
        rec = copy.deepcopy(record)
        if name == "inline/inline-status.json":
            rec["policy_sha256"] = live_inline_policy
        elif name == "rules/receipt.json":
            rec["policy_sha256"] = live_rules_policy
        path = Path(directory, name)
        path.parent.mkdir(parents=True, exist_ok=True)
        path.write_text(json.dumps(rec))


def receipts_match_checkout():
    """The real receipts carry the policy digests of ci/managed-check and tools/gates at 879a5be4."""
    inline = FIXTURE["receipts"]["inline/inline-status.json"]["policy_sha256"]
    return lander.lr.policy() == inline


def quiet(fn, *args, **kwargs):
    with redirect_stdout(io.StringIO()) as out:
        result = fn(*args, **kwargs)
    return result, out.getvalue()


class Select(unittest.TestCase):
    def test_dry_run_over_all_open_pull_requests_on_the_captured_main(self):
        api = FakeApi(live=False)
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=LIVE_MAIN), api=api, config=CONFIG)
        self.assertEqual(code, 0)
        self.assertEqual(api.writes, [])
        self.assertIn("Would send repository_dispatch bfme-remeasure for #2 head 7eb1cdbb67eb on main 89db7cafe448", out)
        for number in (3, 4, 5, 6, 7):
            self.assertRegex(out, rf"\| #{number} \|.*Held: refused-branch")

    def test_pull_request_2_measured_on_current_main_is_the_candidate(self):
        api = FakeApi(live=False, main=MEASURED)
        decision = lander.evaluate(api, 2, MEASURED, CONFIG)
        self.assertEqual((decision["stage"], decision["reasons"], decision["inline"]), ("land", [], "success"))

    def test_pull_request_3_is_held_with_named_reasons(self):
        api = FakeApi(live=False, main=MEASURED)
        decision = lander.evaluate(api, 3, MEASURED, CONFIG)
        codes = [item.code for item in decision["reasons"]]
        self.assertEqual(decision["stage"], "hold")
        self.assertEqual(codes[0], "refused-branch")
        self.assertIn("check-failure", codes)
        self.assertEqual(decision["checks"][lander.BOAT], "fresh-failure")
        self.assertEqual(decision["checks"][lander.INLINE], "pending")

    def test_githubs_old_conflict_flag_does_not_hold_a_head_passed_on_this_main(self):
        def conflicted(fx):
            fx["pulls/2"]["mergeable"] = False
        api = FakeApi(live=False, main=MEASURED, edit=conflicted)
        decision = lander.evaluate(api, 2, MEASURED, CONFIG)
        self.assertEqual((decision["stage"], decision["reasons"]), ("land", []))
        api = FakeApi(live=False, main=LIVE_MAIN, edit=conflicted)          # measured on an older main only
        decision = lander.evaluate(api, 2, LIVE_MAIN, CONFIG)
        self.assertIn("merge-conflict", [item.code for item in decision["reasons"]])

    def test_rehearsal_branch_is_refused_without_its_exact_exception(self):
        api = FakeApi(live=False, main=MEASURED)
        decision = lander.evaluate(api, 2, MEASURED, CONFIG | {"exceptions": set()})
        self.assertEqual(decision["reasons"][0].code, "refused-branch")
        decision = lander.evaluate(api, 2, MEASURED, CONFIG | {"exceptions": {(2, "0" * 40)}})
        self.assertEqual(decision["reasons"][0].code, "refused-branch")

    def test_superseded_run_does_nothing(self):
        api = FakeApi(live=True, main=LIVE_MAIN)
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=MEASURED, mode="live"), api=api, config=CONFIG)
        self.assertEqual((code, api.writes), (0, []))
        self.assertIn("Superseded", out)

    def test_header_change_needs_a_listed_approver(self):
        def header(fx):
            fx["pulls/2/files?per_page=100&page=1"].append({"filename": "include/GameLogic/Object.h", "status": "modified"})
        api = FakeApi(live=False, main=MEASURED, edit=header)
        decision = lander.evaluate(api, 2, MEASURED, CONFIG)
        self.assertEqual(decision["reasons"][0].code, "header-approvers-empty")
        decision = lander.evaluate(api, 2, MEASURED, CONFIG | {"approvers": {99}})
        self.assertEqual(decision["reasons"][0].code, "header-approval")
        api.fx["pulls/2/reviews?per_page=100&page=1"] = [
            {"id": 5, "user": {"id": 99}, "state": "APPROVED", "commit_id": "0" * 40},
            {"id": 6, "user": {"id": 99}, "state": "APPROVED", "commit_id": PR2}]
        decision = lander.evaluate(api, 2, MEASURED, CONFIG | {"approvers": {99}})
        self.assertEqual((decision["stage"], decision["reasons"]), ("land", []))
        author = api.fx["pulls/2"]["user"]["id"]
        api.fx["pulls/2/reviews?per_page=100&page=1"] = [
            {"id": 7, "user": {"id": author}, "state": "APPROVED", "commit_id": PR2}]
        decision = lander.evaluate(api, 2, MEASURED, CONFIG | {"approvers": {author}})
        self.assertEqual(decision["reasons"][0].code, "header-approval")

    def test_workflow_change_is_refused(self):
        def wf(fx):
            fx["pulls/2/files?per_page=100&page=1"].append({"filename": ".github/workflows/x.yml", "status": "added"})
        decision = lander.evaluate(FakeApi(live=False, main=MEASURED, edit=wf), 2, MEASURED, CONFIG)
        self.assertEqual(decision["reasons"][0].code, "workflow-change")

    def test_failed_landing_on_main_stops_everything(self):
        def failed(fx):
            fx[f"commits/{MEASURED}/statuses?per_page=100&page=1"] = [
                {"id": 1, "context": lander.LANDING_CONTEXT, "state": "failure", "description": "x",
                 "creator": {"id": lander.lr.BOT_ID}}]
        api = FakeApi(live=True, main=MEASURED, edit=failed)
        with self.assertRaises(lander.Failed) as caught, tempfile.TemporaryDirectory() as tmp:
            lander.run(["select", tmp + "/w"], env(mode="live"), api=api, config=CONFIG)
        self.assertEqual(caught.exception.code, "main-unverified")

    def test_inline_virtual_policy(self):
        def pending(fx):
            for item in fx[f"commits/{PR2}/statuses?per_page=100&page=1"]:
                if item["context"] == lander.INLINE:
                    item["state"], item["description"] = "pending", "Waiting for admitted inline-virtual evidence"
        api = FakeApi(live=False, main=MEASURED, edit=pending)
        self.assertEqual(lander.INLINE_VIRTUAL_POLICY, "report-only")
        decision = lander.evaluate(api, 2, MEASURED, CONFIG)
        self.assertEqual((decision["stage"], decision["inline"]), ("land", "pending"))
        try:
            lander.INLINE_VIRTUAL_POLICY = "required"
            decision = lander.evaluate(api, 2, MEASURED, CONFIG)
            self.assertEqual(decision["reasons"][0].code, "measuring")
        finally:
            lander.INLINE_VIRTUAL_POLICY = "report-only"

        def failed(fx):
            for item in fx[f"commits/{PR2}/statuses?per_page=100&page=1"]:
                if item["context"] == lander.INLINE:
                    item["state"] = "failure"
        decision = lander.evaluate(FakeApi(live=False, main=MEASURED, edit=failed), 2, MEASURED, CONFIG)
        self.assertEqual(decision["reasons"][0].code, "inline-virtual-failure")


class Reconcile(unittest.TestCase):
    """A landing whose merge response was lost is verified by the next run before anything else."""

    def commit(self, fx, tree=TREE, parents=(MEASURED,)):
        fx[f"git/commits/{NEW}"] = {"sha": NEW, "parents": [{"sha": p} for p in parents], "tree": {"sha": tree},
                                    "message": ("Object: write clearWeaponSetFlagForHorde (#2)\n\n"
                                                f"Measured on main {MEASURED}, head {PR2}, tree {TREE}.\n"
                                                "Pull request #2.\n"
                                                "Landed by the BFME II landing workflow, run 1.")}

    def test_title_written_by_the_author_is_never_parsed(self):
        # A pull request title is the author's text. Fake landing lines in it must not drive reconciliation.
        fake = f"Measured on main {'1' * 40}, head {'2' * 40}, tree {'3' * 40}. Pull request #7."

        def titled(fx):
            self.commit(fx)
            message = fx[f"git/commits/{NEW}"]["message"]
            fx[f"git/commits/{NEW}"]["message"] = fake + " (#2)" + message[message.index("\n\n"):]
        api = FakeApi(live=True, main=NEW, edit=titled)
        with tempfile.TemporaryDirectory() as tmp:
            quiet(lander.run, ["select", tmp + "/w"], env(main=NEW, mode="live", pr="3"), api=api, config=CONFIG)
        self.assertEqual(api.writes[0][2]["client_payload"], {
            "number": 2, "head": PR2, "parent": MEASURED, "commit": NEW, "tree": TREE})

        def title_only(fx):
            fx[f"git/commits/{NEW}"] = {"sha": NEW, "parents": [{"sha": MEASURED}], "tree": {"sha": TREE},
                                        "message": fake + " Landed by the BFME II landing workflow, run 1.\n\nplain"}
        api = FakeApi(live=True, main=NEW, edit=title_only)
        with tempfile.TemporaryDirectory() as tmp:
            code, _ = quiet(lander.run, ["select", tmp + "/w"], env(main=NEW, mode="live", pr="3"),
                            api=api, config=CONFIG)
        self.assertEqual(code, 0)
        self.assertNotIn(f"statuses/{NEW}", [w[1] for w in api.writes])

    def test_live_run_verifies_marks_and_hands_off_an_unconfirmed_landing(self):
        api = FakeApi(live=True, main=NEW, edit=self.commit)
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=NEW, mode="live", pr="3"),
                              api=api, config=CONFIG)
        self.assertEqual(code, 0)
        self.assertEqual(api.writes[1][:2], ("POST", f"statuses/{NEW}"))
        self.assertEqual(api.writes[1][2]["state"], "success")
        self.assertEqual(api.writes[0], ("POST", "dispatches", {"event_type": "bfme-landed", "client_payload": {
            "number": 2, "head": PR2, "parent": MEASURED, "commit": NEW, "tree": TREE}}))
        self.assertIn("Reconciled the earlier landing of #2", out)

    def test_wrong_tree_found_later_stops_landing(self):
        api = FakeApi(live=True, main=NEW, edit=lambda fx: self.commit(fx, tree="ee" * 20))
        with self.assertRaises(lander.Failed) as caught, tempfile.TemporaryDirectory() as tmp:
            quiet(lander.run, ["select", tmp + "/w"], env(main=NEW, mode="live"), api=api, config=CONFIG)
        self.assertEqual(caught.exception.code, "post-merge")
        self.assertEqual(api.writes[0][2]["state"], "failure")
        self.assertNotIn("dispatches", [w[1] for w in api.writes])

    def test_dry_run_reports_and_writes_nothing(self):
        api = FakeApi(live=False, main=NEW, edit=self.commit)
        with self.assertRaises(lander.Held) as caught, tempfile.TemporaryDirectory() as tmp:
            quiet(lander.run, ["select", tmp + "/w"], env(main=NEW), api=api, config=CONFIG)
        self.assertEqual((caught.exception.code, api.writes), ("unreconciled-landing", []))

    def test_already_marked_landing_is_left_alone(self):
        def marked(fx):
            self.commit(fx)
            fx[f"commits/{NEW}/statuses?per_page=100&page=1"] = [
                {"id": 1, "context": lander.LANDING_CONTEXT, "state": "success", "description": "Verified",
                 "creator": {"id": lander.lr.BOT_ID}}]
        api = FakeApi(live=True, main=NEW, edit=marked)
        with tempfile.TemporaryDirectory() as tmp:
            code, _ = quiet(lander.run, ["select", tmp + "/w"], env(main=NEW, mode="live", pr="3"),
                            api=api, config=CONFIG)
        self.assertEqual(code, 0)
        self.assertNotIn(f"statuses/{NEW}", [w[1] for w in api.writes])


class Remeasure(unittest.TestCase):
    def test_live_remeasure_dispatches_non_blocking_and_suppresses_duplicates(self):
        api = FakeApi(live=True, main=LIVE_MAIN)
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=LIVE_MAIN, mode="live", pr="2"),
                              api=api, config=CONFIG)
        self.assertEqual(code, 0)
        self.assertEqual(api.writes[0], ("POST", "dispatches", {"event_type": "bfme-remeasure", "client_payload": {
            "number": 2, "base": LIVE_MAIN, "head": PR2}}))
        self.assertNotIn("PUT", [w[0] for w in api.writes])
        self.assertIn("non-blocking re-measurement", out)
        # A subsequent run while still measuring on the same main does not re-dispatch.
        measuring_desc = f"Measuring on main {LIVE_MAIN[:12]} before landing"
        api.fx[f"commits/{PR2}/statuses?per_page=100&page=1"].insert(0, {
            "id": 10**11, "context": lander.LANDING_CONTEXT, "state": "pending",
            "description": measuring_desc, "creator": {"id": lander.lr.BOT_ID},
        })
        api.writes.clear()
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=LIVE_MAIN, mode="live", pr="2"),
                              api=api, config=CONFIG)
        self.assertEqual((code, [w for w in api.writes if w[1] == "dispatches"]), (0, []))
        self.assertIn("Already measuring", out)

    def test_expired_remeasure_times_out_with_a_named_reason(self):
        api = FakeApi(live=True, main=LIVE_MAIN)
        measuring_desc = f"Measuring on main {LIVE_MAIN[:12]} before landing"
        api.fx[f"commits/{PR2}/statuses?per_page=100&page=1"].insert(0, {
            "id": 10**11, "context": lander.LANDING_CONTEXT, "state": "pending",
            "description": measuring_desc, "created_at": "2020-01-01T00:00:00Z",
            "creator": {"id": lander.lr.BOT_ID},
        })
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=LIVE_MAIN, mode="live", pr="2"),
                              api=api, config=CONFIG)
        self.assertEqual(code, 0)
        self.assertIn("measurement-timeout", out)
        posted = [w for w in api.writes if w[1] == f"statuses/{PR2}"][-1][2]["description"]
        self.assertIn(f"(main {LIVE_MAIN[:12]})", posted)
        # The next run does not re-dispatch after measurement-timeout on the same main.
        api.fx[f"commits/{PR2}/statuses?per_page=100&page=1"].insert(0, {
            "id": 10**12, "context": lander.LANDING_CONTEXT, "state": "pending", "description": posted,
            "creator": {"id": lander.lr.BOT_ID}})
        api.writes.clear()
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=LIVE_MAIN, mode="live", pr="2"),
                              api=api, config=CONFIG)
        self.assertEqual((code, [w for w in api.writes if w[1] == "dispatches"]), (0, []))
        self.assertIn("Nothing to land.", out)

    def test_infrastructure_error_is_non_final_hold(self):
        def boat_err(fx):
            for item in fx[f"commits/{PR2}/statuses?per_page=100&page=1"]:
                if item["context"] == lander.BOAT:
                    item["state"] = "error"
                    item["description"] = f"Boat comparison unavailable. (main {MEASURED[:12]})"
        decision = lander.evaluate(FakeApi(live=False, main=MEASURED, edit=boat_err), 2, MEASURED, CONFIG)
        self.assertEqual(decision["stage"], "hold")
        self.assertEqual(decision["checks"][lander.BOAT], "fresh-error")
        self.assertEqual((decision["reasons"][0].code, decision["reasons"][0].final), ("infrastructure-error", False))

    def test_tiered_header_landing_allows_maintainer_override_of_hold_but_not_failure(self):
        def add_header(fx, state, desc, approved=False):
            fx["pulls/2/files?per_page=100&page=1"].append({"filename": "include/Game/Foo.h", "status": "modified"})
            fx[f"commits/{PR2}/statuses?per_page=100&page=1"].append({
                "id": 10**10, "context": lander.HEADER_RULES_CONTEXT, "state": state,
                "description": desc, "creator": {"id": lander.lr.BOT_ID},
            })
            fx["pulls/2/reviews?per_page=100&page=1"] = (
                [{"id": 1, "state": "APPROVED", "commit_id": PR2, "user": {"id": 99}}] if approved else []
            )

        tiered_cfg = CONFIG | {"auto_headers": True, "approvers": {99}}
        # Pending/held H5 + maintainer approval -> clears to land.
        d1 = lander.evaluate(FakeApi(live=False, main=MEASURED, edit=lambda fx: add_header(
            fx, "pending", f"Held: H5 layout needs witness (main {MEASURED[:12]})", approved=True)), 2, MEASURED, tiered_cfg)
        self.assertEqual((d1["stage"], d1["reasons"]), ("land", []))
        # Hard failure -> still blocked even with maintainer approval.
        d2 = lander.evaluate(FakeApi(live=False, main=MEASURED, edit=lambda fx: add_header(
            fx, "failure", f"H3: forbidden macro (main {MEASURED[:12]})", approved=True)), 2, MEASURED, tiered_cfg)
        self.assertEqual(d2["stage"], "hold")
        self.assertEqual(d2["reasons"][0].code, "header-rules")

    def test_parallel_remeasure_caps_total_in_flight_across_invocations(self):
        api = FakeApi(live=True, main=LIVE_MAIN)
        measuring_desc = f"Measuring on main {LIVE_MAIN[:12]} before landing"
        stale_pr = dict(api.fx["pulls/2"])
        open_list = []
        for num in range(10, 16):
            pr_copy = json.loads(json.dumps(stale_pr))
            pr_copy["number"] = num
            head_sha = f"{num:040x}"
            pr_copy["head"]["sha"] = head_sha
            api.fx[f"pulls/{num}"] = pr_copy
            api.fx[f"pulls/{num}/files?per_page=100&page=1"] = list(api.fx["pulls/2/files?per_page=100&page=1"])
            sts = json.loads(json.dumps(api.fx[f"commits/{PR2}/statuses?per_page=100&page=1"]))
            if num < 15:
                sts.insert(0, {"id": 10**11 + num, "context": lander.LANDING_CONTEXT, "state": "pending",
                               "description": measuring_desc, "creator": {"id": lander.lr.BOT_ID}})
            api.fx[f"commits/{head_sha}/statuses?per_page=100&page=1"] = sts
            open_list.append(pr_copy)
        api.fx["pulls?state=open&base=main&sort=created&direction=asc&per_page=100&page=1"] = open_list
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=LIVE_MAIN, mode="live"),
                              api=api, config=CONFIG | {"exceptions": {(n, f"{n:040x}") for n in range(10, 16)}})
        self.assertEqual(code, 0)
        self.assertEqual([w for w in api.writes if w[1] == "dispatches"], [])
        self.assertIn("Waiting for in-flight measurements", out)

    def test_pending_boat_prs_count_toward_cap_and_keep_measuring_marker(self):
        api = FakeApi(live=True, main=LIVE_MAIN)
        measuring_desc = f"Measuring on main {LIVE_MAIN[:12]} before landing"
        stale_pr = dict(api.fx["pulls/2"])
        open_list = []
        for num in range(20, 26):
            pr_copy = json.loads(json.dumps(stale_pr))
            pr_copy["number"] = num
            head_sha = f"{num:040x}"
            pr_copy["head"]["sha"] = head_sha
            api.fx[f"pulls/{num}"] = pr_copy
            api.fx[f"pulls/{num}/files?per_page=100&page=1"] = list(api.fx["pulls/2/files?per_page=100&page=1"])
            sts = json.loads(json.dumps(api.fx[f"commits/{PR2}/statuses?per_page=100&page=1"]))
            if num < 25:
                for item in sts:
                    if item["context"] == lander.BOAT:
                        item["state"] = "pending"
                sts.insert(0, {"id": 10**11 + num, "context": lander.LANDING_CONTEXT, "state": "pending",
                               "description": measuring_desc, "creator": {"id": lander.lr.BOT_ID}})
            api.fx[f"commits/{head_sha}/statuses?per_page=100&page=1"] = sts
            open_list.append(pr_copy)
        api.fx["pulls?state=open&base=main&sort=created&direction=asc&per_page=100&page=1"] = open_list
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=LIVE_MAIN, mode="live"),
                              api=api, config=CONFIG | {"exceptions": {(n, f"{n:040x}") for n in range(20, 26)}})
        self.assertEqual(code, 0)
        self.assertEqual([w for w in api.writes if w[1] == "dispatches"], [])
        self.assertEqual([w for w in api.writes if w[1].startswith("statuses/")], [])
        # If those five pending markers have expired, the sixth stale PR is no longer starved.
        for num in range(20, 25):
            head_sha = f"{num:040x}"
            api.fx[f"commits/{head_sha}/statuses?per_page=100&page=1"][0]["created_at"] = "2020-01-01T00:00:00Z"
        api.writes.clear()
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=LIVE_MAIN, mode="live"),
                              api=api, config=CONFIG | {"exceptions": {(n, f"{n:040x}") for n in range(20, 26)}})
        self.assertEqual(code, 0)
        self.assertEqual(len([w for w in api.writes if w[1] == "dispatches"]), 1)

    def test_remeasure_off_names_the_manual_route(self):
        api = FakeApi(live=True, main=LIVE_MAIN)
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=LIVE_MAIN, mode="live", pr="2"),
                              api=api, config=CONFIG | {"remeasure": "off"})
        self.assertEqual([w for w in api.writes if w[1] == "dispatches"], [])
        self.assertIn("automatic re-measurement is off", out)


class Land(unittest.TestCase):
    def prepare(self, tmp, api, drop_inline=False):
        code, _ = quiet(lander.run, ["select", tmp + "/w"], env(pr="2", mode="live" if api.live else "dry-run"),
                        api=api, config=CONFIG)
        self.assertEqual(code, 0)
        self.assertTrue(Path(tmp, "w/binding.json").is_file())
        put_receipts(tmp + "/w", drop_inline)

    def merged(self, fx):
        fx["git/ref/heads/main"] = {"object": {"sha": NEW}}
        fx[f"git/commits/{NEW}"] = {"sha": NEW, "parents": [{"sha": MEASURED}], "tree": {"sha": TREE}}

    def test_dry_run_verifies_real_receipts_and_merges_nothing(self):
        api = FakeApi(live=False, main=MEASURED)
        with tempfile.TemporaryDirectory() as tmp:
            self.prepare(tmp, api)
            binding = json.loads(Path(tmp, "w/binding.json").read_text())
            self.assertEqual(binding["runs"], {"match": ["44770182794", "1"], "rules": ["44770182973", "1"], "inline": "success"})
            code, out = quiet(lander.run, ["land", tmp + "/w"], env(), api=api, config=CONFIG)
        self.assertEqual((code, api.writes), (0, []))
        self.assertIn("Would merge #2 (squash) with expected head 7eb1cdbb67eb onto main 879a5be4f21d, "
                      "expecting one parent 879a5be4f21d and tree 44fa7dfb9e1d.", out)

    def test_waits_for_a_fresh_status_whose_run_has_not_completed(self):
        # Live trial 2026-11-20 run 44777584236: Boat posted success at 16:55:06, its run completed at 16:55:46.
        api = FakeApi(live=True, main=MEASURED)
        api.fx["actions/runs/44770182794"]["status"] = "in_progress"
        api.fx["actions/runs/44770182794"]["conclusion"] = None
        reads = {"n": 0}

        def complete(fx):
            reads["n"] += 1
            if reads["n"] == 40:
                fx["actions/runs/44770182794"]["status"] = "completed"
                fx["actions/runs/44770182794"]["conclusion"] = "success"
        api.after_read = [complete] * 10000
        ticks = iter(range(0, 10**6, 20))
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(pr="2", mode="live"), api=api, config=CONFIG,
                              clock=lambda: next(ticks), sleep=lambda s: None)
            self.assertTrue(Path(tmp, "w/binding.json").is_file())
        self.assertEqual(code, 0)
        self.assertIn("waiting for their runs to complete", out)
        self.assertIn("collecting its receipts", out)

    def test_dry_run_does_not_wait_on_an_incomplete_run(self):
        api = FakeApi(live=False, main=MEASURED)
        api.fx["actions/runs/44770182794"]["status"] = "in_progress"
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(pr="2"), api=api, config=CONFIG,
                              clock=lambda: 0, sleep=lambda s: self.fail("dry run slept"))
        self.assertEqual((code, api.writes), (0, []))
        self.assertIn("held while waiting for its results. run-binding", out)

    def test_dry_run_api_refuses_every_write(self):
        with self.assertRaises(ReadOnly):
            FakeApi(live=False).write("PUT", "pulls/2/merge", {})

    def test_live_landing_merges_with_expected_head_and_verifies_parent_and_tree(self):
        api = FakeApi(live=True, main=MEASURED)
        api.responses[("PUT", "pulls/2/merge")] = (200, {"sha": NEW, "merged": True})
        api.after_write[("PUT", "pulls/2/merge")] = self.merged
        with tempfile.TemporaryDirectory() as tmp:
            self.prepare(tmp, api)
            api.writes.clear()
            code, out = quiet(lander.run, ["land", tmp + "/w"], {**env(mode="live"), "BFME_LANDING_CHAIN": "1"},
                              api=api, config=CONFIG, sleep=lambda s: None)
        self.assertEqual(code, 0)
        # The clearance the ruleset requires comes first, for this exact head only.
        self.assertEqual(api.writes[0][:2], ("POST", f"statuses/{PR2}"))
        self.assertEqual((api.writes[0][2]["state"], api.writes[0][2]["description"]),
                         ("success", f"Cleared to land on main {MEASURED[:12]}"))
        method, path, body = api.writes[1]
        self.assertEqual((method, path, body["sha"], body["merge_method"]), ("PUT", "pulls/2/merge", PR2, "squash"))
        self.assertTrue(body["commit_title"].endswith("(#2)"))
        kinds = [(w[0], w[1]) for w in api.writes[2:]]
        self.assertEqual(kinds, [("POST", "dispatches"), ("POST", f"statuses/{NEW}"), ("POST", f"statuses/{PR2}"),
                                 ("POST", "issues/2/comments"), ("POST", "dispatches")])
        self.assertEqual(api.writes[2][2], {"event_type": "bfme-landed", "client_payload": {
            "number": 2, "head": PR2, "parent": MEASURED, "commit": NEW, "tree": TREE}})
        self.assertEqual(list(api.writes[2][2]["client_payload"]), ["number", "head", "parent", "commit", "tree"])
        # The body this workflow writes is exactly what a later run parses to reconcile this landing.
        lines = body["commit_message"].splitlines()
        self.assertEqual(lander.MEASURED_LINE.fullmatch(lines[0]).groups(), (MEASURED, PR2, TREE))
        self.assertEqual(lander.NUMBER_LINE.fullmatch(lines[1])[1], "2")
        self.assertTrue(lander.TRAILER_LINE.fullmatch(lines[2]))
        self.assertEqual(api.writes[6][2]["event_type"], "bfme-land-next")
        self.assertIn(f"Landed #2 as {NEW[:12]}", out)
        self.assertIn(f"::notice title=BFME II landing::Landed #2 as {NEW[:12]} on main", out)

    def test_main_moving_before_the_merge_stops_without_merging(self):
        api = FakeApi(live=True, main=MEASURED)
        with tempfile.TemporaryDirectory() as tmp:
            self.prepare(tmp, api)
            api.writes.clear()
            # Main moves after selection; the receipts still verify, and the read just before the merge sees it.
            api.fx["git/ref/heads/main"] = {"object": {"sha": "cd" * 20}}
            with self.assertRaises(lander.Held) as caught:
                quiet(lander.run, ["land", tmp + "/w"], env(mode="live"), api=api, config=CONFIG)
            out = caught.exception.line()
        self.assertEqual([w for w in api.writes if w[0] == "PUT"], [])
        self.assertIn("main-moved", out)
        # The clearance is withdrawn at once: the last Landing status on the head names the hold.
        last = [w for w in api.writes if w[1] == f"statuses/{PR2}"][-1][2]
        self.assertEqual(last["state"], "pending")
        self.assertTrue(last["description"].startswith("Held: main-moved."))

    def test_wrong_tree_after_merge_fails_and_marks_main(self):
        api = FakeApi(live=True, main=MEASURED)
        api.responses[("PUT", "pulls/2/merge")] = (200, {"sha": NEW, "merged": True})

        def bad(fx):
            self.merged(fx)
            fx[f"git/commits/{NEW}"]["tree"]["sha"] = "ee" * 20
        api.after_write[("PUT", "pulls/2/merge")] = bad
        with tempfile.TemporaryDirectory() as tmp:
            self.prepare(tmp, api)
            api.writes.clear()
            with self.assertRaises(lander.Failed) as caught:
                quiet(lander.run, ["land", tmp + "/w"], env(mode="live"), api=api, config=CONFIG, sleep=lambda s: None)
        self.assertEqual(caught.exception.code, "post-merge")
        self.assertEqual(api.writes[2], ("POST", f"statuses/{NEW}", {
            "state": "failure", "context": lander.LANDING_CONTEXT, "target_url":
            "https://github.com/Open-BFME/BFME2-Rebuild/actions/runs/1",
            "description": f"Landing verification failed: expected tree {TREE[:12]}"}))

    def test_merge_conflict_is_held_and_explained(self):
        api = FakeApi(live=True, main=MEASURED)
        api.responses[("PUT", "pulls/2/merge")] = (405, {"message": "Pull Request is not mergeable"})
        with tempfile.TemporaryDirectory() as tmp:
            self.prepare(tmp, api)
            api.writes.clear()
            with self.assertRaises(lander.Held) as caught:
                quiet(lander.run, ["land", tmp + "/w"], env(mode="live"), api=api, config=CONFIG)
        self.assertEqual(caught.exception.code, "merge-refused")
        self.assertIn(("POST", "issues/2/comments"), [(w[0], w[1]) for w in api.writes])
        last = [w for w in api.writes if w[1] == f"statuses/{PR2}"][-1][2]
        self.assertEqual((last["state"], last["description"][:22]), ("failure", "Held: merge-refused. E"))

    def test_report_only_inline_pending(self):
        def pending(fx):
            for item in fx[f"commits/{PR2}/statuses?per_page=100&page=1"]:
                if item["context"] == lander.INLINE:
                    item["state"], item["description"] = "pending", "Waiting for admitted inline-virtual evidence"
        api = FakeApi(live=False, main=MEASURED, edit=pending)
        with tempfile.TemporaryDirectory() as tmp:
            if not lander.optional_inline_supported():
                code, out = quiet(lander.run, ["select", tmp + "/w"], env(pr="2"), api=api, config=CONFIG)
                self.assertEqual((code, api.writes), (0, []))
                self.assertIn("inline-virtual-pending", out)
                return
            self.prepare(tmp, api, drop_inline=True)
            self.assertEqual(json.loads(Path(tmp, "w/binding.json").read_text())["runs"]["inline"], "pending")
            code, out = quiet(lander.run, ["land", tmp + "/w"], env(), api=api, config=CONFIG)
        self.assertEqual((code, api.writes), (0, []))
        self.assertIn("Would merge #2 (squash)", out)

    def test_changed_receipt_is_refused(self):
        api = FakeApi(live=False, main=MEASURED)
        with tempfile.TemporaryDirectory() as tmp:
            self.prepare(tmp, api)
            path = Path(tmp, "w/rules/receipt.json")
            record = json.loads(path.read_text())
            record["tree"] = "ee" * 20
            path.write_text(json.dumps(record))
            with self.assertRaises(lander.Held) as caught:
                quiet(lander.run, ["land", tmp + "/w"], env(), api=api, config=CONFIG)
        self.assertEqual(caught.exception.code, "receipt")




class LandingResilienceTests(unittest.TestCase):
    def test_api_call_retries_transient_5xx_and_connection_errors(self):
        from api import Api, ApiError, TransientApiError
        api = Api("Open-BFME/BFME2-Rebuild", "ghp_" + "a" * 30, live=False)
        sleeps = []
        api._sleep = sleeps.append
        responses = [(500, None), TransientApiError("conn"), (200, {"ok": True})]
        def fake_send(method, path, body):
            item = responses.pop(0)
            if isinstance(item, Exception):
                raise item
            return item
        api._send = fake_send
        self.assertEqual(api.call("pulls/18/files"), {"ok": True})
        self.assertEqual(sleeps, [1, 2])

    def test_api_call_does_not_retry_404_or_exhausted_500(self):
        from api import Api, ApiError
        api = Api("Open-BFME/BFME2-Rebuild", "ghp_" + "a" * 30, live=False)
        sleeps = []
        api._sleep = sleeps.append
        calls = []
        def fake_404(method, path, body):
            calls.append(path)
            return (404, None)
        api._send = fake_404
        with self.assertRaises(ApiError):
            api.call("pulls/99/files")
        self.assertEqual(len(calls), 1)
        self.assertEqual(sleeps, [])

        calls.clear()
        def fake_500(method, path, body):
            calls.append(path)
            return (500, None)
        api._send = fake_500
        with self.assertRaises(ApiError):
            api.call("pulls/18/files")
        self.assertEqual(len(calls), 3)
        self.assertEqual(sleeps, [1, 2])

    def test_held_header_rules_pending_preserves_reason_text(self):
        def add_held_header(fx):
            fx["pulls/2/files?per_page=100&page=1"].append({"filename": "include/A.h", "status": "modified"})
            head = fx["pulls/2"]["head"]["sha"]
            fx[f"commits/{head}/statuses?per_page=100&page=1"].append({
                "id": 99,
                "context": lander.HEADER_RULES_CONTEXT,
                "state": "pending",
                "description": f"Held: H2 no exact protected key (main {MEASURED[:12]})",
                "creator": {"id": 41898282},
            })
        cfg = {**CONFIG, "auto_headers": True, "approvers": [1760627]}
        decision = lander.evaluate(FakeApi(live=False, main=MEASURED, edit=add_held_header), 2, MEASURED, cfg)
        first = decision["reasons"][0]
        self.assertEqual((first.code, first.final), ("header-rules", False))
        self.assertIn("H2 no exact protected key", first.found)
        self.assertIn("listed header approval", first.expected)

    def test_evaluate_fetches_commit_statuses_only_once_per_pr(self):
        api = FakeApi(live=False, main=MEASURED)
        seen = []
        orig_send = api._send
        def counting_send(method, path, body):
            if method == "GET" and "/statuses" in path:
                seen.append(path)
            return orig_send(method, path, body)
        api._send = counting_send
        lander.evaluate(api, 2, MEASURED, {**CONFIG, "auto_headers": True})
        page1 = [p for p in seen if p.endswith("&page=1")]
        self.assertEqual(len(page1), 1, f"Expected 1 page=1 statuses GET, got {seen}")

    def test_parallel_select_preserves_deterministic_pr_order(self):
        import threading
        api = FakeApi(live=False, main=MEASURED)
        pr2 = api.fx["pulls/2"]
        api.fx["pulls?state=open&base=main&sort=created&direction=asc&per_page=100&page=1"] = [
            {"number": 2}, {"number": 3},
        ]
        api.fx["pulls/3"] = copy.deepcopy(pr2)
        api.fx["pulls/3"]["number"] = 3
        api.fx["pulls/3/files?per_page=100&page=1"] = copy.deepcopy(api.fx["pulls/2/files?per_page=100&page=1"])
        api.thread_safe_reads = True
        barrier = threading.Barrier(2, timeout=5)
        first_pass = set()
        lock = threading.Lock()
        orig_send = api._send
        def parallel_send(method, path, body):
            wait_now = False
            if method == "GET" and path in ("pulls/2", "pulls/3"):
                with lock:
                    if path not in first_pass:
                        first_pass.add(path)
                        wait_now = True
            if wait_now:
                barrier.wait()
            return orig_send(method, path, body)
        api._send = parallel_send
        with tempfile.TemporaryDirectory() as tmp:
            quiet(lander.run, ["select", tmp + "/w"], env(mode="dry-run"), api=api, config=CONFIG)




class DisjointAncestorAndGraphQLTests(unittest.TestCase):
    """Cut 8: disjoint-TU ancestor landing (with fresh G1-G8/G7 source rules), header priority, GraphQL batching."""

    def _setup_ancestor_fixture(self, *, pr_file="src/Code/A.cpp", intervening_files=None,
                                pr_status="modified", inline_state="success",
                                rules_on_current=True, ahead_by=2, mergeable=True):
        ancestor_main = MEASURED
        new_main = "c" * 40
        head = PR2
        measured_tree = TREE
        new_tree = "d" * 40
        new_commit = "e" * 40
        blob_sha = "b" * 40
        src_tree_m, code_tree_m = "1" * 40, "2" * 40
        src_tree_t, code_tree_t = "3" * 40, "4" * 40
        if intervening_files is None:
            intervening_files = [{"filename": "src/Code/Other.cpp", "status": "modified"}]

        def edit(fx):
            fx["pulls/2/files?per_page=100&page=1"] = [{"filename": pr_file, "status": pr_status}]
            fx["pulls/2"]["mergeable"] = mergeable
            fx["pulls/2"]["head"]["ref"] = "contrib/match-a"
            fx[f"compare/{ancestor_main[:12]}...{new_main}"] = {
                "status": "ahead",
                "ahead_by": ahead_by,
                "behind_by": 0,
                "base_commit": {"sha": ancestor_main},
                "merge_base_commit": {"sha": ancestor_main},
                "files": intervening_files,
            }
            fx[f"git/commits/{new_commit}"] = {
                "sha": new_commit,
                "parents": [{"sha": new_main}],
                "tree": {"sha": new_tree},
                "message": (f"Title (#2)\n\n"
                            f"Measured on main {new_main}, head {head}, tree {new_tree}.\n"
                            f"Boat measured on disjoint ancestor {ancestor_main}, measured tree {measured_tree}.\n"
                            f"Pull request #2.\n"
                            f"{lander.TRAILER}, run 99."),
            }
            leaf = pr_file.split("/")[-1]
            fx[f"git/trees/{measured_tree}"] = {"truncated": False, "tree": [{"path": "src", "type": "tree", "sha": src_tree_m}]}
            fx[f"git/trees/{src_tree_m}"] = {"truncated": False, "tree": [{"path": "Code", "type": "tree", "sha": code_tree_m}]}
            fx[f"git/trees/{code_tree_m}"] = {"truncated": False, "tree": [{"path": leaf, "type": "blob", "sha": blob_sha}]}
            fx[f"git/trees/{new_tree}"] = {"truncated": False, "tree": [{"path": "src", "type": "tree", "sha": src_tree_t}]}
            fx[f"git/trees/{src_tree_t}"] = {"truncated": False, "tree": [{"path": "Code", "type": "tree", "sha": code_tree_t}]}
            fx[f"git/trees/{code_tree_t}"] = {"truncated": False, "tree": [{"path": leaf, "type": "blob", "sha": blob_sha}]}
            statuses = fx[f"commits/{head}/statuses?per_page=100&page=1"]
            for st in statuses:
                if st.get("context") == lander.INLINE:
                    st["state"] = inline_state
                if st.get("context") == lander.RULES and rules_on_current:
                    st["description"] = f"Source rules passed (main {new_main[:12]})"
                    st["target_url"] = f"https://github.com/{lander.REPO}/actions/runs/999002"
            fx["actions/runs/999002"] = {
                "id": 999002,
                "path": ".github/workflows/rules.yml",
                "event": "repository_dispatch",
                "head_sha": new_main,
                "repository": {"id": lander.REPO_ID},
                "status": "completed",
                "conclusion": "success",
                "run_attempt": 1,
            }

        api = FakeApi(live=True, main=new_main, edit=edit)
        api.responses[("PUT", "pulls/2/merge")] = (200, {"merged": True, "sha": new_commit})
        api.after_write[("PUT", "pulls/2/merge")] = lambda fx: fx["git/ref/heads/main"]["object"].update({"sha": new_commit})
        return api, new_main, ancestor_main, head, measured_tree, new_tree, new_commit, blob_sha

    def test_disjoint_ancestor_requires_fresh_source_rules_and_lands_with_whole_tree_verify(self):
        # 1. Stale RULES on disjoint ancestor -> requests bfme-source-recheck (0 Boat VMs), does NOT land!
        api_stale, new_main, anc, head, *_ = self._setup_ancestor_fixture(rules_on_current=False)
        cfg = CONFIG | {"exceptions": set(), "auto_admission": True}
        dec_stale = lander.evaluate(api_stale, 2, new_main, cfg)
        self.assertEqual(dec_stale["stage"], "remeasure")
        self.assertTrue(dec_stale.get("source_recheck_only"))
        with tempfile.TemporaryDirectory() as tmp:
            code, _ = quiet(lander.run, ["select", tmp + "/w"], env(main=new_main, mode="live", pr="2"), api=api_stale, config=cfg)
            self.assertEqual(code, 0)
            events = [b["event_type"] for _, path, b in api_stale.writes if path == "dispatches"]
            self.assertEqual(events, [lander.SOURCE_RECHECK_EVENT])

        # 2. Fresh RULES on current_main + Boat on disjoint ancestor -> selects and lands with exact target_tree verify!
        api, new_main, anc, head, mtree, ntree, ncommit, _ = self._setup_ancestor_fixture(rules_on_current=True)
        decision = lander.evaluate(api, 2, new_main, cfg)
        self.assertEqual(decision["stage"], "land")
        self.assertTrue(decision["ancestor_landing"])
        self.assertEqual(decision["measured_base"], anc)

        with tempfile.TemporaryDirectory() as tmp:
            w = tmp + "/w"
            code, out = quiet(lander.run, ["select", w], env(main=new_main, mode="live", pr="2"), api=api, config=cfg)
            self.assertEqual(code, 0)
            self.assertIn("disjoint ancestor", out)
            saved = json.loads(Path(w, "binding.json").read_text())
            self.assertEqual(saved["base"], anc)
            self.assertEqual(saved["target_main"], new_main)
            self.assertNotIn("pr_files", saved)  # keeps binding.json compact (<4 KiB)

            if True:
                put_receipts(w)
                # Update rules receipt to match fresh source rules on new_main + ntree + run 999002
                rp = Path(w, "rules/receipt.json")
                rdoc = json.loads(rp.read_text())
                rdoc.update({"base": new_main, "tools_commit": new_main, "tree": ntree, "run": "999002", "attempt": "1"})
                rp.write_text(json.dumps(rdoc))
                code, out = quiet(lander.run, ["land", w], env(main=new_main, mode="live", pr="2", run="99"),
                                  api=api, config=cfg, sleep=lambda _: None)
                self.assertEqual(code, 0)
                self.assertIn(f"Landed #2 as {ncommit[:12]}", out)
                # Verify bfme-landed dispatch happens BEFORE status(commit, "success") for crash safety
                write_ops = [(path, (b.get("event_type") if path == "dispatches" else b.get("state")))
                             for _, path, b in api.writes if path in ("dispatches", f"statuses/{ncommit}")]
                idx_dispatch = write_ops.index(("dispatches", lander.LANDED_EVENT))
                idx_status = write_ops.index((f"statuses/{ncommit}", "success"))
                self.assertLess(idx_dispatch, idx_status)

    def test_disjoint_ancestor_refuses_overlapping_or_non_tu_or_spoofed_inline_failure(self):
        cfg = CONFIG | {"exceptions": set()}
        # Overlapping file (case-insensitive)
        api, new_main, *_ = self._setup_ancestor_fixture(
            pr_file="src/Code/A.cpp",
            intervening_files=[{"filename": "src/Code/a.cpp", "status": "modified"}],
        )
        self.assertEqual(lander.evaluate(api, 2, new_main, cfg)["stage"], "remeasure")

        # Intervening header or stlport .c or removed file
        for bad_intervening in (
            [{"filename": "include/Game/Foo.h", "status": "modified"}],
            [{"filename": "src/stlport-4.6/STLport-4.6/src/c_locale.c", "status": "modified"}],
            [{"filename": "src/Code/Other.cpp", "status": "removed"}],
        ):
            api, new_main, *_ = self._setup_ancestor_fixture(intervening_files=bad_intervening)
            self.assertEqual(lander.evaluate(api, 2, new_main, cfg)["stage"], "remeasure")

        # Non-bot status posted AFTER a real bot Inline virtual failure cannot hide the failure
        api_spoof, new_main, anc, head, *_ = self._setup_ancestor_fixture(inline_state="failure")
        api_spoof.fx[f"commits/{head}/statuses?per_page=100&page=1"].append({
            "id": 999999, "context": lander.INLINE, "state": "success",
            "description": f"spoofed (main {anc[:12]})", "creator": {"id": 123456},
        })
        self.assertEqual(lander.evaluate(api_spoof, 2, new_main, cfg)["stage"], "remeasure")

        # Exact-main non-bot status also cannot hide a fresh bot Inline virtual failure
        api_exact = FakeApi(live=False, main=MEASURED)
        for st in api_exact.fx[f"commits/{PR2}/statuses?per_page=100&page=1"]:
            if st.get("context") == lander.INLINE:
                st["state"] = "failure"
        api_exact.fx[f"commits/{PR2}/statuses?per_page=100&page=1"].append({
            "id": 999999, "context": lander.INLINE, "state": "success",
            "description": f"spoofed (main {MEASURED[:12]})", "creator": {"id": 123456},
        })
        dec_exact = lander.evaluate(api_exact, 2, MEASURED, CONFIG)
        self.assertEqual(dec_exact["stage"], "hold")
        self.assertIn("inline-virtual-failure", [r.code for r in dec_exact["reasons"]])

    def test_header_priority_prevents_starvation_and_ignores_stale_boat_pending(self):
        cfg = CONFIG | {"exceptions": set(), "auto_headers": True}
        # 1. Stale header PR #5 waiting to re-measure takes priority over ready narrow PR #2 (no starvation!)
        def edit_stale_header(fx):
            pr2 = copy.deepcopy(fx["pulls/2"])
            pr2["head"]["ref"] = "contrib/narrow"
            fx["pulls/2"] = pr2
            fx["pulls/2/files?per_page=100&page=1"] = [{"filename": "src/Code/A.cpp", "status": "modified"}]
            pr5 = copy.deepcopy(pr2)
            pr5["number"] = 5
            pr5["head"]["ref"] = "contrib/header"
            pr5["head"]["sha"] = "5" * 40
            fx["pulls/5"] = pr5
            fx["pulls/5/files?per_page=100&page=1"] = [{"filename": "include/Game/A.h", "status": "modified"}]
            fx[f"commits/{'5'*40}/statuses?per_page=100&page=1"] = [
                {"id": 1, "context": lander.BOAT, "state": "success",
                 "description": "BFME II comparison passed (main 000000000000)", "creator": {"id": lander.lr.BOT_ID}},
                {"id": 2, "context": lander.RULES, "state": "success",
                 "description": "Source rules passed (main 000000000000)", "creator": {"id": lander.lr.BOT_ID}},
                {"id": 3, "context": lander.HEADER_RULES_CONTEXT, "state": "success",
                 "description": "Header rules passed (main 000000000000)", "creator": {"id": lander.lr.BOT_ID}},
            ]
            fx["pulls?state=open&base=main&sort=created&direction=asc&per_page=100&page=1"] = [{"number": 2}, {"number": 5}]

        api = FakeApi(live=True, main=MEASURED, edit=edit_stale_header)
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=MEASURED, mode="live"), api=api, config=cfg)
            self.assertEqual(code, 0)
            self.assertFalse(Path(tmp, "w/binding.json").exists())
            dispatches = [b for _, path, b in api.writes if path == "dispatches"]
            self.assertEqual(dispatches[0]["client_payload"]["number"], 5)

        # 2. Stale Boat pending from an OLD main does NOT pause ready narrow PR #2!
        now_s = lander.datetime.datetime.now(lander.datetime.timezone.utc).strftime("%Y-%m-%dT%H:%M:%SZ")
        def edit_old_pending(fx):
            pr2 = copy.deepcopy(fx["pulls/2"])
            pr2["head"]["ref"] = "contrib/narrow"
            fx["pulls/2"] = pr2
            fx["pulls/2/files?per_page=100&page=1"] = [{"filename": "src/Code/A.cpp", "status": "modified"}]
            pr5 = copy.deepcopy(pr2)
            pr5["number"] = 5
            pr5["head"]["ref"] = "contrib/header"
            pr5["head"]["sha"] = "5" * 40
            fx["pulls/5"] = pr5
            fx["pulls/5/files?per_page=100&page=1"] = [{"filename": "include/Game/A.h", "status": "modified"}]
            fx[f"commits/{'5'*40}/statuses?per_page=100&page=1"] = [
                {"id": 1, "context": lander.BOAT, "state": "pending", "created_at": now_s,
                 "description": "Measuring (main 000000000000)", "creator": {"id": lander.lr.BOT_ID}},
                {"id": 2, "context": lander.RULES, "state": "success",
                 "description": f"Source rules passed (main {MEASURED[:12]})", "creator": {"id": lander.lr.BOT_ID}},
            ]
            fx["pulls?state=open&base=main&sort=created&direction=asc&per_page=100&page=1"] = [{"number": 2}, {"number": 5}]

        api2 = FakeApi(live=False, main=MEASURED, edit=edit_old_pending)
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=MEASURED, mode="dry-run"), api=api2, config=cfg)
            self.assertEqual(code, 0)
            self.assertTrue(Path(tmp, "w/binding.json").is_file())
            self.assertEqual(json.loads(Path(tmp, "w/binding.json").read_text())["number"], 2)

    def test_graphql_batch_reader_includes_author_id_for_header_review_and_auto_admission_text(self):
        real_api = lander.Api("Open-BFME/BFME2-Rebuild", "ghp_" + "a" * 36, live=False)
        with self.assertRaises(lander.ApiError):
            real_api.query("mutation { foo }", {})

        head = PR2
        api = FakeApi(live=False, main=MEASURED)
        api.fx["pulls/2/reviews?per_page=100&page=1"] = [
            {"id": 1, "user": {"id": 1760627}, "state": "APPROVED", "commit_id": head},
        ]
        def fake_query(q, variables):
            return {"repository": {"pullRequests": {
                "pageInfo": {"hasNextPage": False, "endCursor": None},
                "nodes": [{
                    "number": 2, "id": "PR_2", "title": "Header A", "state": "OPEN",
                    "isDraft": False, "authorAssociation": "CONTRIBUTOR", "mergeable": "MERGEABLE",
                    "baseRefName": "main", "headRefName": "contrib/a", "headRefOid": head,
                    "author": {"login": "alice", "databaseId": 55555},
                    "headRepository": {"databaseId": 9999}, "baseRepository": {"databaseId": lander.REPO_ID},
                    "labels": {"nodes": []},
                    "files": {"totalCount": 1, "nodes": [{"path": "include/Game/A.h", "changeType": "MODIFIED"}]},
                    "commits": {"nodes": [{"commit": {"status": {"contexts": [
                        {"context": lander.ADMISSION_CONTEXT, "state": "SUCCESS",
                         "description": f"Admitted by rule: header change (main {MEASURED[:12]})",
                         "targetUrl": "", "createdAt": "", "creator": {"databaseId": lander.lr.BOT_ID}},
                        {"context": lander.RULES, "state": "SUCCESS",
                         "description": f"Source rules passed (main {MEASURED[:12]})",
                         "targetUrl": "", "createdAt": "", "creator": {"databaseId": lander.lr.BOT_ID}},
                    ]}}}]},
                }],
            }}}
        api.query = fake_query
        cfg = CONFIG | {"exceptions": set(), "auto_admission": True, "auto_headers": True, "approvers": {1760627}}
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=MEASURED, mode="dry-run"), api=api, config=cfg)
        self.assertEqual(code, 0)
        self.assertIn("awaiting-measurement", out)
        self.assertNotIn("remove and add the boat-ok label", out)

    def test_graphql_falls_back_to_rest_when_non_bot_masks_inline_failure(self):
        api = FakeApi(live=False, main=MEASURED)
        for st in api.fx[f"commits/{PR2}/statuses?per_page=100&page=1"]:
            if st.get("context") == lander.INLINE:
                st["state"] = "failure"
        def fake_query(q, variables):
            return {"repository": {"pullRequests": {
                "pageInfo": {"hasNextPage": False, "endCursor": None},
                "nodes": [{
                    "number": 2, "id": "PR_2", "title": "Match A", "state": "OPEN",
                    "isDraft": False, "authorAssociation": "OWNER", "mergeable": "MERGEABLE",
                    "baseRefName": "main", "headRefName": "contrib/a", "headRefOid": PR2,
                    "author": {"login": "alice", "databaseId": 1},
                    "headRepository": {"databaseId": lander.REPO_ID}, "baseRepository": {"databaseId": lander.REPO_ID},
                    "labels": {"nodes": []},
                    "files": {"totalCount": 1, "nodes": [{"path": "src/Code/A.cpp", "changeType": "MODIFIED"}]},
                    "commits": {"nodes": [{"commit": {"status": {"contexts": [
                        {"context": lander.BOAT, "state": "SUCCESS",
                         "description": f"BFME II comparison passed (main {MEASURED[:12]})",
                         "targetUrl": "", "createdAt": "", "creator": {"databaseId": lander.lr.BOT_ID}},
                        {"context": lander.RULES, "state": "SUCCESS",
                         "description": f"Source rules passed (main {MEASURED[:12]})",
                         "targetUrl": "", "createdAt": "", "creator": {"databaseId": lander.lr.BOT_ID}},
                        {"context": lander.INLINE, "state": "SUCCESS",
                         "description": f"spoofed (main {MEASURED[:12]})",
                         "targetUrl": "", "createdAt": "", "creator": {"databaseId": 999999}},
                    ]}}}]},
                }],
            }}}
        api.query = fake_query
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=MEASURED, mode="dry-run"),
                              api=api, config=CONFIG | {"exceptions": set()})
        self.assertEqual(code, 0)
        self.assertIn("inline-virtual-failure", out)

    def test_stale_old_main_pendings_do_not_starve_header_dispatch_or_block_narrow_land(self):
        def edit_stale_pendings(fx):
            pr2 = copy.deepcopy(fx["pulls/2"])
            pr2["head"]["ref"] = "contrib/narrow"
            fx["pulls/2"] = pr2
            fx["pulls/2/files?per_page=100&page=1"] = [{"filename": "src/Code/A.cpp", "status": "modified"}]
            pr_list = [{"number": 2}]
            for idx in range(10, 15):
                p = copy.deepcopy(pr2)
                p["number"] = idx
                p["head"]["sha"] = f"{idx:040x}"
                fx[f"pulls/{idx}"] = p
                fx[f"pulls/{idx}/files?per_page=100&page=1"] = [{"filename": f"src/Code/P{idx}.cpp", "status": "modified"}]
                fx[f"commits/{p['head']['sha']}/statuses?per_page=100&page=1"] = [
                    {"id": 1, "context": lander.BOAT, "state": "pending",
                     "description": "Measuring (main 000000000000)", "creator": {"id": lander.lr.BOT_ID}},
                    {"id": 2, "context": lander.RULES, "state": "success",
                     "description": f"Source rules passed (main {MEASURED[:12]})", "creator": {"id": lander.lr.BOT_ID}},
                ]
                pr_list.append({"number": idx})
            pr5 = copy.deepcopy(pr2)
            pr5["number"] = 5
            pr5["head"]["sha"] = "5" * 40
            fx["pulls/5"] = pr5
            fx["pulls/5/files?per_page=100&page=1"] = [{"filename": "include/Game/A.h", "status": "modified"}]
            fx[f"commits/{'5'*40}/statuses?per_page=100&page=1"] = [
                {"id": 1, "context": lander.BOAT, "state": "success",
                 "description": "BFME II comparison passed (main 000000000000)", "creator": {"id": lander.lr.BOT_ID}},
                {"id": 2, "context": lander.RULES, "state": "success",
                 "description": "Source rules passed (main 000000000000)", "creator": {"id": lander.lr.BOT_ID}},
                {"id": 3, "context": lander.HEADER_RULES_CONTEXT, "state": "success",
                 "description": "Header rules passed (main 000000000000)", "creator": {"id": lander.lr.BOT_ID}},
            ]
            pr_list.append({"number": 5})
            fx["pulls?state=open&base=main&sort=created&direction=asc&per_page=100&page=1"] = pr_list

        api = FakeApi(live=True, main=MEASURED, edit=edit_stale_pendings)
        cfg = CONFIG | {"exceptions": set(), "auto_headers": True}
        with tempfile.TemporaryDirectory() as tmp:
            code, out = quiet(lander.run, ["select", tmp + "/w"], env(main=MEASURED, mode="live"), api=api, config=cfg)
            self.assertEqual(code, 0)
            dispatches = [b for _, path, b in api.writes if path == "dispatches"]
            self.assertTrue(dispatches and dispatches[0]["client_payload"]["number"] == 5,
                            f"Expected stale header #5 to dispatch despite 5 old-main pendings, got {dispatches}")

    def test_merge_http_403_end_to_end_replaces_cleared_status_and_chains_next(self):
        api = FakeApi(live=True, main=MEASURED)
        api.responses[("PUT", "pulls/2/merge")] = (403, {"message": "Resource not accessible by integration"})
        api.last_headers = {"x-accepted-github-permissions": "contents=write", "x-github-request-id": "REQ123"}
        with tempfile.TemporaryDirectory() as tmp:
            w = tmp + "/w"
            code, _ = quiet(lander.run, ["select", w], env(pr="2", mode="live"), api=api, config=CONFIG)
            self.assertEqual(code, 0)
            put_receipts(w)
            run_env = env(pr="2", mode="live") | {"BFME_LANDING_CHAIN": "1"}
            with self.assertRaises(lander.Held) as ctx:
                quiet(lander.run, ["land", w], run_env, api=api, config=CONFIG, sleep=lambda _: None)
        self.assertEqual(ctx.exception.code, "merge-forbidden")
        self.assertTrue(ctx.exception.final)
        head_statuses = [b for _, path, b in api.writes if path == f"statuses/{PR2}"]
        self.assertEqual(head_statuses[-1]["state"], "failure")
        self.assertTrue(head_statuses[-1]["description"].startswith("Held: merge-forbidden."))
        comments = [b["body"] for _, path, b in api.writes if path == "issues/2/comments"]
        self.assertTrue(any("HTTP 403" in c for c in comments))
        dispatches = [b["event_type"] for _, path, b in api.writes if path == "dispatches"]
        self.assertIn(lander.NEXT_EVENT, dispatches)

    def test_disjoint_ancestor_allows_build_neutral_intervening_changes(self):
        api = FakeApi(live=True, main=MEASURED)
        older = "7" * 40
        api.fx[f"compare/{older[:12]}...{MEASURED}"] = {
            "status": "ahead", "ahead_by": 2, "behind_by": 0,
            "base_commit": {"sha": older}, "merge_base_commit": {"sha": older},
            "files": [
                {"filename": "docs/agents.md", "status": "modified"},
                {"filename": "ci/progress/card.py", "status": "modified"},
                {"filename": "src/Code/GameEngine/Other.cpp", "status": "modified"},
            ],
        }
        anc = lander.resolve_disjoint_ancestor(api, older[:12], MEASURED, ["src/Code/GameEngine/Mine.cpp"])
        self.assertEqual(anc, older)

    def test_disjoint_ancestor_supports_src_unk_and_nbench_and_rejects_code_tools(self):
        api = FakeApi(live=True, main=MEASURED)
        older = "7" * 40
        api.fx[f"compare/{older[:12]}...{MEASURED}"] = {
            "status": "ahead", "ahead_by": 4, "behind_by": 0,
            "base_commit": {"sha": older}, "merge_base_commit": {"sha": older},
            "files": [
                {"filename": "src/unk/O00999999.cpp", "status": "modified"},
                {"filename": "src/nbench/Benchmark/emfloat.c", "status": "modified"},
            ],
        }
        self.assertEqual(
            lander.resolve_disjoint_ancestor(api, older[:12], MEASURED, ["src/unk/O00123456.cpp"]),
            older,
        )
        self.assertEqual(
            lander.resolve_disjoint_ancestor(api, older[:12], MEASURED, ["src/nbench/Benchmark/nbench0.c"]),
            older,
        )
        # Overlap in src/unk (case-insensitive) must be rejected.
        self.assertIsNone(
            lander.resolve_disjoint_ancestor(api, older[:12], MEASURED, ["src/unk/o00999999.cpp"]),
        )
        # src/Code/Tools/ is always excluded from disjoint TUs.
        self.assertFalse(lander._is_disjoint_tu_path("src/Code/Tools/WW3D/pluglib/matrix4.cpp"))
        self.assertIsNone(
            lander.resolve_disjoint_ancestor(api, older[:12], MEASURED, ["src/Code/Tools/WW3D/pluglib/matrix4.cpp"]),
        )

    def test_disjoint_ancestor_missing_rules_triggers_source_recheck_prioritized_over_full_remeasure(self):
        api, new_main, anc, head, *_ = self._setup_ancestor_fixture(
            pr_file="src/unk/O00123456.cpp",
            intervening_files=[{"filename": "src/unk/O00999999.cpp", "status": "modified"}],
            rules_on_current=False,
        )
        # Simulate compact trusted PR whose source rules were never published because main moved.
        api.fx[f"commits/{head}/statuses?per_page=100&page=1"] = [
            st for st in api.fx[f"commits/{head}/statuses?per_page=100&page=1"]
            if st.get("context") != lander.RULES
        ]
        cfg = CONFIG | {"exceptions": set(), "auto_admission": True}
        dec = lander.evaluate(api, 2, new_main, cfg)
        self.assertEqual(dec["stage"], "remeasure")
        self.assertTrue(dec.get("source_recheck_only"))
        self.assertEqual(dec["measured_base"], anc)

        # Add an older PR #1 needing a full remeasure; PR #2 (source_recheck_only) dispatches first.
        pr1 = copy.deepcopy(api.fx["pulls/2"])
        pr1["number"] = 1
        pr1["head"]["sha"] = "1" * 40
        api.fx["pulls/1"] = pr1
        api.fx["pulls/1/files?per_page=100&page=1"] = [{"filename": "src/unk/O00000001.cpp", "status": "modified"}]
        api.fx[f"commits/{'1'*40}/statuses?per_page=100&page=1"] = [
            {"id": 1, "context": lander.BOAT, "state": "success",
             "description": "BFME II comparison passed (main 000000000000)", "creator": {"id": lander.lr.BOT_ID}},
            {"id": 2, "context": lander.RULES, "state": "success",
             "description": "Source rules passed (main 000000000000)", "creator": {"id": lander.lr.BOT_ID}},
        ]
        api.fx["pulls?state=open&base=main&sort=created&direction=asc&per_page=100&page=1"] = [{"number": 1}, {"number": 2}]
        with tempfile.TemporaryDirectory() as tmp:
            code, _ = quiet(lander.run, ["select", tmp + "/w"], env(main=new_main, mode="live"), api=api, config=cfg)
        self.assertEqual(code, 0)
        dispatches = [b for _, path, b in api.writes if path == "dispatches"]
        self.assertGreaterEqual(len(dispatches), 2)
        self.assertEqual(dispatches[0]["event_type"], lander.SOURCE_RECHECK_EVENT)
        self.assertEqual(dispatches[0]["client_payload"]["number"], 2)
        self.assertEqual(dispatches[1]["event_type"], lander.ADMIT_REQUEST_EVENT)
        self.assertEqual(dispatches[1]["client_payload"]["number"], 1)

    def test_merge_http_403_holds_pr_without_blocking_other_candidates(self):
        api = FakeApi(live=True, main=MEASURED)
        api.fx[f"commits/{PR2}/statuses?per_page=100&page=1"].append({
            "id": 99999, "context": lander.LANDING_CONTEXT, "state": "failure",
            "description": "Held: merge-forbidden. Expected GitHub to allow the landing workflow to merge.",
            "creator": {"id": lander.lr.BOT_ID},
        })
        dec = lander.evaluate(api, 2, MEASURED, CONFIG)
        self.assertEqual(dec["stage"], "hold")
        self.assertEqual(dec["reasons"][0].code, "merge-forbidden")
        self.assertTrue(dec["reasons"][0].final)


if __name__ == "__main__":
    unittest.main()
