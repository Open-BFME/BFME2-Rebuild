"""Unit tests for the GitHub Native Merge Queue auto-enqueuer (ci/queue/enqueue.py)."""
import datetime
from pathlib import Path
import sys
import tempfile
import unittest
from unittest import mock

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "ci/queue"))

import enqueue  # noqa: E402

MAIN_SHA = "a" * 40
NOW = datetime.datetime(2026, 11, 23, 12, 0, tzinfo=datetime.timezone.utc)
OLD = "2026-11-23T10:00:00Z"      # two hours before NOW: outside the cooldown
RECENT = "2026-11-23T11:50:00Z"   # ten minutes before NOW: inside the cooldown


def make_pr(number, *, oid=None, draft=False, in_queue=False, mergeable="MERGEABLE",
            branch="feat/test", author_id=100, labels=(), files=("src/Foo.cpp",),
            total_files=None, statuses=None, reviews=(), removed=None):
    """statuses maps a context to a state, or to (state, description[, createdAt])."""
    oid = oid or f"{number:040x}"
    contexts = []
    for context, value in (statuses or {}).items():
        state, description, created = (value, "", OLD) if isinstance(value, str) or value is None else (
            tuple(value) + (OLD,))[:3]
        contexts.append({"context": context, "state": state, "description": description, "createdAt": created})
    return {
        "id": f"PR_node_{number}",
        "number": number,
        "isDraft": draft,
        "headRefOid": oid,
        "headRefName": branch,
        "mergeable": mergeable,
        "isInMergeQueue": in_queue,
        "author": {"login": f"user{author_id}", "databaseId": author_id},
        "labels": {"nodes": [{"name": lbl} for lbl in labels]},
        "files": {
            "totalCount": len(files) if total_files is None else total_files,
            "nodes": [{"path": p} for p in files],
        },
        "commits": {"nodes": [{"commit": {"status": {"contexts": contexts}}}]},
        "timelineItems": {"nodes": [{"createdAt": removed}] if removed else []},
        "latestOpinionatedReviews": {"nodes": list(reviews)},
    }


class FakeApi:
    def __init__(self, pages=(), *, active_in_progress=0, active_queued=0,
                 rules_in_progress=0, rules_queued=0, ratelimit=4000, fail_enqueue=(), fail_status=False,
                 fail_quota=False):
        self.pages = pages
        self.active_in_progress = active_in_progress
        self.active_queued = active_queued
        self.rules_in_progress = rules_in_progress
        self.rules_queued = rules_queued
        self.ratelimit_remaining = ratelimit
        self.fail_enqueue = set(fail_enqueue)
        self.fail_status = fail_status
        self.fail_quota = fail_quota
        self.quota_reads = 0
        self.enqueued = []
        self.dispatches = []
        self.statuses = []
        self.gets = []

    def graphql(self, query, variables):
        if "enqueuePullRequest" in query:
            if variables["id"] in self.fail_enqueue:
                raise enqueue.ApiError("GraphQL error: Pull request is already in queue")
            self.enqueued.append((variables["id"], variables["oid"]))
            return {"enqueuePullRequest": {"mergeQueueEntry": {"id": "MQE_1", "position": 1}}}
        cursor = variables.get("cursor")
        idx = 0 if cursor is None else int(cursor)
        nodes = self.pages[idx]
        has_next = idx + 1 < len(self.pages)
        return {
            "repository": {
                "defaultBranchRef": {"target": {"oid": MAIN_SHA}},
                "pullRequests": {
                    "pageInfo": {"hasNextPage": has_next, "endCursor": str(idx + 1) if has_next else None},
                    "nodes": nodes,
                },
            }
        }

    def get(self, path):
        self.gets.append(path)
        if "workflows/managed-pr.yml/runs" in path:
            if "status=in_progress" in path:
                return {"total_count": self.active_in_progress}
            if "status=queued" in path:
                return {"total_count": self.active_queued}
        if "workflows/rules.yml/runs" in path:
            if "status=in_progress" in path:
                return {"total_count": self.rules_in_progress}
            if "status=queued" in path:
                return {"total_count": self.rules_queued}
        if "workflows/admission.yml/runs" in path:
            return {"total_count": 0}
        raise AssertionError(f"Unexpected GET {path}")

    def core_remaining(self):
        self.quota_reads += 1
        if self.fail_quota:
            raise enqueue.ApiError("GET rate_limit returned HTTP 500")
        return self.ratelimit_remaining

    def post(self, path, body):
        if path.startswith("statuses/"):
            if self.fail_status:
                raise enqueue.ApiError(f"POST {path} returned HTTP 403")
            self.statuses.append((path, body))
            return {}
        self.dispatches.append((path, body))
        return {}


def run(api, **kwargs):
    kwargs.setdefault("env", {})
    return enqueue.run(api, now=NOW, **kwargs)


class EnqueueTests(unittest.TestCase):
    def test_enqueues_ready_source_and_header_prs_and_skips_ineligible(self):
        oid_hdr_ok = "1" * 40
        oid_hdr_rev = "2" * 40
        oid_hdr_self = "3" * 40
        oid_hdr_stale = "4" * 40
        ok_src = {"BFME II / source rules": "SUCCESS", "BFME II / Boat": "SUCCESS", "BFME II / Inline virtual": "PENDING"}
        prs = [
            # Eligible narrow source PR
            make_pr(101, statuses=ok_src),
            # Eligible header PR via Header rules SUCCESS
            make_pr(102, oid=oid_hdr_ok, files=("include/A.h", "src/A.cpp"),
                    statuses=ok_src | {"BFME II / Header rules": "SUCCESS"}),
            # Eligible header PR via listed approver on exact headRefOid
            make_pr(103, oid=oid_hdr_rev, files=("include/B.h",), statuses=ok_src, reviews=[
                {"state": "APPROVED", "commit": {"oid": oid_hdr_rev}, "author": {"databaseId": 1760627}},
            ]),
            # Rejected: header PR self-approved by author
            make_pr(104, oid=oid_hdr_self, author_id=1760627, files=("include/C.h",), statuses=ok_src, reviews=[
                {"state": "APPROVED", "commit": {"oid": oid_hdr_self}, "author": {"databaseId": 1760627}},
            ]),
            # Rejected: header PR approved on stale commit
            make_pr(105, oid=oid_hdr_stale, files=("include/D.h",), statuses=ok_src, reviews=[
                {"state": "APPROVED", "commit": {"oid": "f" * 40}, "author": {"databaseId": 1760627}},
            ]),
            # Rejected: Inline virtual failure
            make_pr(106, statuses=ok_src | {"BFME II / Inline virtual": "FAILURE"}),
            # Rejected: draft, already queued, conflicting, held, canary, refused branch, bad paths, >60 files
            make_pr(107, draft=True, statuses=ok_src),
            make_pr(108, in_queue=True, statuses=ok_src),
            make_pr(109, mergeable="CONFLICTING", statuses=ok_src),
            make_pr(110, labels=("landing-hold",), statuses=ok_src),
            make_pr(21, statuses=ok_src),
            make_pr(111, branch="canary/test", statuses=ok_src),
            make_pr(112, files=("src/Code/Tools/exp.cpp",), statuses=ok_src),
            make_pr(113, files=("tools/gates/gate.py",), statuses=ok_src),
            make_pr(114, files=("src/A.cpp",), total_files=61, statuses=ok_src),
        ]
        api = FakeApi([prs[:8], prs[8:]])
        with tempfile.TemporaryDirectory() as td:
            summary = Path(td) / "summary.md"
            result = run(api, env={"GITHUB_STEP_SUMMARY": str(summary)})
            # 103 waits: one header change in the merge queue at a time.
            self.assertEqual(result["enqueued"], [101, 102])
            # 104 and 105 passed Boat but have no Header rules yet: a wide check on the current main is requested.
            self.assertEqual(result["backfilled"], [104, 105])
            self.assertIn("#101, #102", summary.read_text(encoding="utf-8"))

    def test_caps_enqueue_at_16_and_survives_individual_rejection(self):
        ok_src = {"BFME II / source rules": "SUCCESS", "BFME II / Boat": "SUCCESS"}
        prs = [make_pr(200 + i, statuses=ok_src) for i in range(20)]
        api = FakeApi([prs], fail_enqueue={"PR_node_201"})
        result = run(api)
        # A rejected entry does not use one of the 16 places.
        self.assertEqual(len(result["enqueued"]), 16)
        self.assertNotIn(201, result["enqueued"])
        self.assertEqual(len(result["errors"]), 1)
        self.assertEqual(result["errors"][0][0], 201)

    def test_backfills_unmeasured_pending_and_error_prs_up_to_available_slots(self):
        boat_states = (None, "PENDING", "ERROR")
        prs = [
            make_pr(300 + i, statuses={"BFME II / Boat": boat_states[i % 3]} if i < 20 else {})
            for i in range(22)
        ]
        # PR with failed Boat status should NOT be backfilled.
        prs.insert(0, make_pr(297, statuses={"BFME II / Boat": "FAILURE"}))
        # PR with failed source rules should NOT be backfilled (even if Boat is ERROR).
        prs.insert(0, make_pr(299, statuses={"BFME II / source rules": "FAILURE", "BFME II / Boat": "ERROR"}))
        # PR touching disallowed path should NOT be backfilled.
        prs.insert(0, make_pr(298, files=("docs/x.md",), statuses={}))

        # With 0 active runs, fills up to MAX_BACKFILL (18), including ERROR Boat PRs; never-touched heads first.
        idle = FakeApi([prs], active_in_progress=0, active_queued=0, ratelimit=2500)
        idle_result = run(idle)
        self.assertEqual(idle_result["enqueued"], [])
        self.assertEqual(len(idle_result["backfilled"]), 18)
        self.assertEqual(idle_result["backfilled"], [320, 321] + [300 + i for i in range(16)])
        self.assertIn(302, idle_result["backfilled"])  # ERROR Boat status
        self.assertEqual(idle.dispatches[0][0], "dispatches")
        self.assertEqual(idle.dispatches[0][1]["event_type"], "bfme-source-refresh")
        self.assertEqual(idle.dispatches[0][1]["client_payload"]["base"], MAIN_SHA)

        # With 12 active runs (6 in_progress + 6 queued), only fills remaining 6 slots (18 - 12).
        partial = FakeApi([prs], active_in_progress=6, active_queued=6, ratelimit=2500)
        partial_result = run(partial)
        self.assertEqual(partial_result["enqueued"], [])
        self.assertEqual(len(partial_result["backfilled"]), 6)
        self.assertEqual(partial_result["backfilled"], [320, 321, 300, 301, 302, 303])

    def test_skips_backfill_when_ratelimit_low_or_boat_busy(self):
        prs = [make_pr(400, statuses={}), make_pr(401, statuses={"BFME II / Boat": "ERROR"})]
        low_rl = FakeApi([prs], active_in_progress=0, active_queued=0, ratelimit=500)
        self.assertEqual(run(low_rl)["backfilled"], [])
        self.assertEqual(low_rl.dispatches, [])

        at_cap = FakeApi([prs], active_in_progress=10, active_queued=8, ratelimit=3000)
        self.assertEqual(run(at_cap)["backfilled"], [])
        self.assertEqual(at_cap.dispatches, [])

        over_cap = FakeApi([prs], active_in_progress=12, active_queued=8, ratelimit=3000)
        self.assertEqual(run(over_cap)["backfilled"], [])
        self.assertEqual(over_cap.dispatches, [])

    def test_dispatch_api_receives_backfill_while_api_receives_enqueue(self):
        ok_src = {"BFME II / source rules": "SUCCESS", "BFME II / Boat": "SUCCESS"}
        prs = [
            make_pr(501, statuses=ok_src),
            make_pr(502, statuses={}),
            make_pr(503, statuses={"BFME II / Boat": "PENDING"}),
        ]
        queue_api = FakeApi([prs], ratelimit=3000)
        dispatch_api = FakeApi(ratelimit=800)
        result = run(queue_api, dispatch_api=dispatch_api)
        self.assertEqual(result["enqueued"], [501])
        self.assertEqual(result["backfilled"], [502, 503])
        self.assertEqual([node for node, _ in queue_api.enqueued], ["PR_node_501"])
        self.assertEqual(queue_api.dispatches, [])
        self.assertEqual(dispatch_api.enqueued, [])
        self.assertEqual(dispatch_api.gets, [])
        self.assertEqual(len(dispatch_api.dispatches), 2)
        self.assertEqual(dispatch_api.dispatches[0][0], "dispatches")
        self.assertEqual(dispatch_api.dispatches[0][1]["event_type"], "bfme-source-refresh")
        self.assertEqual(
            [d[1]["client_payload"]["number"] for d in dispatch_api.dispatches],
            [502, 503],
        )

    def test_env_dispatch_token_builds_separate_dispatch_api(self):
        prs = [make_pr(510, statuses={})]
        queue_api = FakeApi([prs], ratelimit=3000)
        dispatch_api = FakeApi(ratelimit=800)
        tokens = []

        def fake_github_api(token, token_name="BFME_QUEUE_TOKEN"):
            tokens.append((token, token_name))
            return queue_api if token_name == "BFME_QUEUE_TOKEN" else dispatch_api

        with mock.patch.object(enqueue, "GitHubApi", side_effect=fake_github_api):
            result = enqueue.run(env={
                "BFME_QUEUE_TOKEN": "pat_123456789012345",
                "BFME_DISPATCH_TOKEN": "ghs_123456789012345",
            })
        self.assertEqual(tokens, [
            ("pat_123456789012345", "BFME_QUEUE_TOKEN"),
            ("ghs_123456789012345", "BFME_DISPATCH_TOKEN"),
        ])
        self.assertEqual(result["backfilled"], [510])
        self.assertEqual(queue_api.dispatches, [])
        self.assertEqual(len(dispatch_api.dispatches), 1)

    def test_in_flight_rules_runs_count_toward_occupied_backfill_slots(self):
        prs = [make_pr(600 + i, statuses={}) for i in range(20)]
        # 4 boat in_progress + 2 boat queued + 5 rules in_progress + 3 rules queued = 14 occupied -> 4 slots left.
        partial = FakeApi(
            [prs],
            active_in_progress=4,
            active_queued=2,
            rules_in_progress=5,
            rules_queued=3,
            ratelimit=3000,
        )
        result = run(partial)
        self.assertEqual(result["backfilled"], [600, 601, 602, 603])
        self.assertEqual(len(partial.dispatches), 4)
        self.assertTrue(any("workflows/managed-pr.yml/runs" in p for p in partial.gets))
        self.assertTrue(any("workflows/rules.yml/runs" in p for p in partial.gets))

        # 0 boat runs, but 18 in-flight rules runs -> 0 slots left (prevents double-dispatch).
        rules_full = FakeApi(
            [prs],
            active_in_progress=0,
            active_queued=0,
            rules_in_progress=12,
            rules_queued=6,
            ratelimit=3000,
        )
        self.assertEqual(run(rules_full)["backfilled"], [])
        self.assertEqual(rules_full.dispatches, [])

    def test_backfill_writes_a_marker_first_and_skips_heads_touched_within_the_cooldown(self):
        prs = [
            make_pr(700, statuses={"BFME II / Boat": ("PENDING", "", RECENT)}),                # in flight
            make_pr(701, statuses={"BFME II / Queue backfill": ("SUCCESS", "", RECENT)}),      # just requested
            make_pr(702, statuses={"BFME II / Admission": ("SUCCESS", "", RECENT)}),           # just admitted
            make_pr(703, statuses={"BFME II / Boat": ("ERROR", "", OLD)}),
            make_pr(704, statuses={}),
        ]
        api = FakeApi([prs])
        result = run(api)
        self.assertEqual(result["backfilled"], [704, 703])
        self.assertEqual([path for path, _ in api.statuses], [f"statuses/{704:040x}", f"statuses/{703:040x}"])
        marker = api.statuses[0][1]
        self.assertEqual((marker["context"], marker["state"]), ("BFME II / Queue backfill", "success"))
        self.assertEqual(marker["description"], f"Requested bfme-source-refresh on main {MAIN_SHA[:12]}")

        # The next run sees the markers and requests nothing again.
        touched = [make_pr(704, statuses={"BFME II / Queue backfill": ("SUCCESS", "", RECENT)}),
                   make_pr(703, statuses={"BFME II / Boat": ("ERROR", "", OLD),
                                          "BFME II / Queue backfill": ("SUCCESS", "", RECENT)})]
        again = FakeApi([prs[:3] + touched])
        self.assertEqual(run(again)["backfilled"], [])
        self.assertEqual(again.dispatches, [])

    def test_no_dispatch_without_a_marker(self):
        api = FakeApi([[make_pr(710, statuses={})]], fail_status=True)
        self.assertEqual(run(api)["backfilled"], [])
        self.assertEqual(api.dispatches, [])

    def test_retries_rotate_least_recently_touched_first(self):
        prs = [
            make_pr(720, statuses={"BFME II / Boat": ("ERROR", "", "2026-11-23T11:00:00Z")}),
            make_pr(721, statuses={"BFME II / Boat": ("ERROR", "", "2026-11-23T09:00:00Z")}),
            make_pr(722, statuses={"BFME II / Boat": ("ERROR", "", "2026-11-23T10:00:00Z")}),
        ]
        api = FakeApi([prs], active_in_progress=16)
        self.assertEqual(run(api)["backfilled"], [721, 722])

    def test_passed_boat_with_unfinished_source_rules_gets_a_source_only_recheck(self):
        boat = {"BFME II / Boat": "SUCCESS"}
        conflict = "Conflicts with main: merge or rebase main into your branch and push (main x)"
        prs = [
            make_pr(730, statuses=boat | {"BFME II / source rules": ("ERROR", "Source rules could not finish (main x)")}),
            make_pr(731, statuses=boat),
            make_pr(732, statuses=boat | {"BFME II / source rules": "FAILURE"}),
            make_pr(733, statuses=boat | {"BFME II / source rules": ("ERROR", conflict)}),
            make_pr(734, statuses=boat | {"BFME II / Inline virtual": "FAILURE"}),
        ]
        api = FakeApi([prs])
        result = run(api)
        self.assertEqual((result["enqueued"], result["backfilled"]), ([], [730, 731]))
        self.assertEqual({body["event_type"] for _, body in api.dispatches}, {"bfme-source-recheck"})
        self.assertEqual(api.dispatches[0][1]["client_payload"],
                         {"number": 730, "head": f"{730:040x}", "base": MAIN_SHA})

    def test_passed_header_change_needs_a_wide_check_on_the_current_main_only_for_h1_or_h10(self):
        ok = {"BFME II / Boat": "SUCCESS", "BFME II / source rules": "SUCCESS"}
        hdr = ("include/A.h",)
        prs = [
            make_pr(740, files=hdr, statuses=ok | {"BFME II / Header rules": (
                "PENDING", "Held: H10 no wide check admitted for this commit yet (main x)")}),
            make_pr(741, files=hdr, statuses=ok | {"BFME II / Header rules": (
                "PENDING", "Held: H1 The wide comparison has not reported on this main. (main x)")}),
            make_pr(742, files=hdr, statuses=ok),
            make_pr(743, files=hdr, statuses=ok | {"BFME II / Header rules": (
                "PENDING", "Held: ledger request Foo::Bar (main x)")}),
            make_pr(744, files=hdr, statuses=ok | {"BFME II / Header rules": ("FAILURE", "H4: removes X (main x)")}),
        ]
        api = FakeApi([prs])
        result = run(api)
        self.assertEqual((result["enqueued"], result["backfilled"]), ([], [740, 741, 742]))
        self.assertEqual({body["event_type"] for _, body in api.dispatches}, {"bfme-source-refresh"})

    def test_merge_queue_removal_needs_a_newer_boat_verdict(self):
        ok = {"BFME II / Boat": ("SUCCESS", "", "2026-11-23T09:00:00Z"), "BFME II / source rules": "SUCCESS"}
        prs = [
            make_pr(750, statuses=ok, removed="2026-11-23T10:30:00Z"),   # removed after its Boat: measure again
            make_pr(751, statuses=ok, removed="2026-11-23T08:30:00Z"),   # measured again since: enqueue
        ]
        result = run(FakeApi([prs]))
        self.assertEqual((result["enqueued"], result["backfilled"]), ([751], [750]))

    def test_final_holds_are_left_alone(self):
        prs = [
            make_pr(760, statuses={"BFME II / Admission": (
                "PENDING", "Held: the compiler check failed to run five times; a maintainer will look")}),
            make_pr(761, statuses={"BFME II / Admission": (
                "PENDING", "Held: source-only admission covers at most eight paths")}),
            make_pr(762, statuses={"BFME II / Boat": "FAILURE"}),
            make_pr(763, statuses={"BFME II / Admission": (
                "PENDING", "Held: checking the source rules on main 0123456789ab, retried automatically")}),
        ]
        self.assertEqual(run(FakeApi([prs]))["backfilled"], [763])

    def test_enqueue_keeps_the_merge_queue_inside_the_group_planner_limits(self):
        ok = {"BFME II / Boat": "SUCCESS", "BFME II / source rules": "SUCCESS"}
        prs = [
            make_pr(800, in_queue=True, files=tuple(f"src/Q{i}.cpp" for i in range(50))),
            make_pr(801, files=tuple(f"src/A{i}.cpp" for i in range(8)), statuses=ok),   # 58: fits
            make_pr(802, files=tuple(f"src/B{i}.cpp" for i in range(3)), statuses=ok),   # 61: over the limit
            make_pr(803, files=("src/C.cpp", "src/D.cpp"), statuses=ok),                  # 60: fits
        ]
        self.assertEqual(run(FakeApi([prs]))["enqueued"], [801, 803])

        headers = [
            make_pr(810, in_queue=True, files=("include/Q.h",)),
            make_pr(811, files=("include/A.h",), statuses=ok | {"BFME II / Header rules": "SUCCESS"}),
            make_pr(812, statuses=ok),
        ]
        self.assertEqual(run(FakeApi([headers]))["enqueued"], [812])

    def test_queue_wakeups_cover_every_completion_and_skip_needless_runs(self):
        managed = (ROOT / ".github/workflows/managed-pr.yml").read_text(encoding="utf-8")
        notify = managed.split("\n  notify-landing:\n")[1].split("\n  notify-unmeasured:\n")[0]
        self.assertIn("(needs.report.result != 'skipped' || needs.fallback-report.result != 'skipped')", notify)
        self.assertNotIn("outputs.passed", notify)
        rules = (ROOT / ".github/workflows/rules.yml").read_text(encoding="utf-8")
        self.assertIn("types: [bfme-source-refresh, bfme-source-recheck]\n", rules)
        admission = rules.split("\n  notify-admission:\n")[1].split("\n  notify-landing:\n")[0]
        self.assertIn('"bfme-admit-request"', admission)
        self.assertIn('"bfme-land-next"', admission)
        push = (ROOT / ".github/workflows/admission-push.yml").read_text(encoding="utf-8")
        self.assertIn("&& vars.BFME_MERGE_QUEUE_ENABLED != 'yes'", push)
        enq_wf = (ROOT / ".github/workflows/enqueue.yml").read_text(encoding="utf-8")
        self.assertIn("      statuses: write\n", enq_wf)

    def test_backfill_is_paced_by_the_actions_token_quota_not_the_queue_token(self):
        boat = {"BFME II / Boat": "SUCCESS"}
        prs = ([make_pr(900 + i, statuses={}) for i in range(5)]
               + [make_pr(910 + i, statuses=boat | {"BFME II / source rules": ("ERROR", "", OLD)}) for i in range(3)])
        queue_api = FakeApi([prs], ratelimit=4900)

        # 400 kept in reserve; 600 left pays for two refreshes (100 each), not a third.
        actions = FakeApi(ratelimit=600)
        self.assertEqual(run(queue_api, dispatch_api=actions)["backfilled"], [900, 901])
        self.assertEqual(actions.quota_reads, 1)

        # Rechecks (25 each) fit where a refresh would not: all three heads are rechecks, 475 left pays for three.
        rechecks = FakeApi([prs[5:]], ratelimit=4900)
        self.assertEqual(run(rechecks, dispatch_api=FakeApi(ratelimit=475))["backfilled"], [910, 911, 912])

        # At or under the reserve, or when the quota cannot be read, nothing is dispatched and no runs are listed.
        for actions in (FakeApi(ratelimit=400), FakeApi(ratelimit=0), FakeApi(ratelimit=4000, fail_quota=True)):
            with self.subTest(remaining=actions.ratelimit_remaining, fail=actions.fail_quota):
                queue_api = FakeApi([prs], ratelimit=4900)
                self.assertEqual(run(queue_api, dispatch_api=actions)["backfilled"], [])
                self.assertEqual((actions.dispatches, actions.statuses), ([], []))
                self.assertEqual(queue_api.gets, [])

    def test_marker_and_dispatch_use_the_actions_token(self):
        queue_api, actions = FakeApi([[make_pr(920, statuses={})]], ratelimit=4900), FakeApi(ratelimit=900)
        self.assertEqual(run(queue_api, dispatch_api=actions)["backfilled"], [920])
        self.assertEqual(([p for p, _ in actions.statuses], len(actions.dispatches)), ([f"statuses/{920:040x}"], 1))
        self.assertEqual((queue_api.statuses, queue_api.dispatches), ([], []))

    def test_workflows_have_atomic_cutover_guards(self):
        enq_wf = (ROOT / ".github/workflows/enqueue.yml").read_text(encoding="utf-8")
        land_wf = (ROOT / ".github/workflows/landing.yml").read_text(encoding="utf-8")
        self.assertIn("vars.BFME_MERGE_QUEUE_ENABLED == 'yes'", enq_wf)
        self.assertIn("BFME II Boat comparison", enq_wf)
        self.assertIn("BFME II header rules", enq_wf)
        self.assertNotIn("BFME II source rules", enq_wf)
        self.assertIn("bfme-land-next", enq_wf)
        self.assertIn("github.event.workflow_run.conclusion == 'failure'", enq_wf)
        self.assertIn("BFME_DISPATCH_TOKEN: '${{ github.token }}'", enq_wf)
        self.assertNotIn("\nconcurrency:\n", enq_wf)
        self.assertIn("\n    concurrency:\n", enq_wf)
        self.assertEqual(land_wf.count("vars.BFME_MERGE_QUEUE_ENABLED != 'yes'"), 2)


if __name__ == "__main__":
    unittest.main()
