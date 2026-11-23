"""Unit tests for the GitHub Native Merge Queue auto-enqueuer (ci/queue/enqueue.py)."""
from pathlib import Path
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "ci/queue"))

import enqueue  # noqa: E402

MAIN_SHA = "a" * 40


def make_pr(number, *, oid=None, draft=False, in_queue=False, mergeable="MERGEABLE",
            branch="feat/test", author_id=100, labels=(), files=("src/Foo.cpp",),
            total_files=None, statuses=None, reviews=()):
    oid = oid or f"{number:040x}"
    contexts = [{"context": k, "state": v, "description": ""} for k, v in (statuses or {}).items()]
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
        "latestOpinionatedReviews": {"nodes": list(reviews)},
    }


class FakeApi:
    def __init__(self, pages, *, active_in_progress=0, active_queued=0, ratelimit=4000, fail_enqueue=()):
        self.pages = pages
        self.active_in_progress = active_in_progress
        self.active_queued = active_queued
        self.ratelimit_remaining = ratelimit
        self.fail_enqueue = set(fail_enqueue)
        self.enqueued = []
        self.dispatches = []
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
        if "status=in_progress" in path:
            return {"total_count": self.active_in_progress}
        if "status=queued" in path:
            return {"total_count": self.active_queued}
        raise AssertionError(f"Unexpected GET {path}")

    def post(self, path, body):
        self.dispatches.append((path, body))
        return {}


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
            result = enqueue.run(api, env={"GITHUB_STEP_SUMMARY": str(summary)})
            self.assertEqual(result["enqueued"], [101, 102, 103])
            self.assertEqual(result["backfilled"], [])
            self.assertIn("#101, #102, #103", summary.read_text(encoding="utf-8"))

    def test_caps_enqueue_at_16_and_survives_individual_rejection(self):
        ok_src = {"BFME II / source rules": "SUCCESS", "BFME II / Boat": "SUCCESS"}
        prs = [make_pr(200 + i, statuses=ok_src) for i in range(20)]
        api = FakeApi([prs], fail_enqueue={"PR_node_201"})
        result = enqueue.run(api, env={})
        self.assertEqual(len(result["enqueued"]), 15)
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

        # With 0 active runs, fills up to MAX_BACKFILL (18), including ERROR Boat PRs.
        idle = FakeApi([prs], active_in_progress=0, active_queued=0, ratelimit=1500)
        idle_result = enqueue.run(idle, env={})
        self.assertEqual(idle_result["enqueued"], [])
        self.assertEqual(len(idle_result["backfilled"]), 18)
        self.assertEqual(idle_result["backfilled"], [300 + i for i in range(18)])
        self.assertIn(302, idle_result["backfilled"])  # ERROR Boat status
        self.assertEqual(idle.dispatches[0][0], "dispatches")
        self.assertEqual(idle.dispatches[0][1]["event_type"], "bfme-source-refresh")
        self.assertEqual(idle.dispatches[0][1]["client_payload"]["base"], MAIN_SHA)

        # With 12 active runs (6 in_progress + 6 queued), only fills remaining 6 slots (18 - 12).
        partial = FakeApi([prs], active_in_progress=6, active_queued=6, ratelimit=1500)
        partial_result = enqueue.run(partial, env={})
        self.assertEqual(partial_result["enqueued"], [])
        self.assertEqual(len(partial_result["backfilled"]), 6)
        self.assertEqual(partial_result["backfilled"], [300 + i for i in range(6)])

    def test_skips_backfill_when_ratelimit_low_or_boat_busy(self):
        prs = [make_pr(400, statuses={}), make_pr(401, statuses={"BFME II / Boat": "ERROR"})]
        low_rl = FakeApi([prs], active_in_progress=0, active_queued=0, ratelimit=500)
        self.assertEqual(enqueue.run(low_rl, env={})["backfilled"], [])
        self.assertEqual(low_rl.dispatches, [])

        at_cap = FakeApi([prs], active_in_progress=10, active_queued=8, ratelimit=3000)
        self.assertEqual(enqueue.run(at_cap, env={})["backfilled"], [])
        self.assertEqual(at_cap.dispatches, [])

        over_cap = FakeApi([prs], active_in_progress=12, active_queued=8, ratelimit=3000)
        self.assertEqual(enqueue.run(over_cap, env={})["backfilled"], [])
        self.assertEqual(over_cap.dispatches, [])

    def test_workflows_have_atomic_cutover_guards(self):
        enq_wf = (ROOT / ".github/workflows/enqueue.yml").read_text(encoding="utf-8")
        land_wf = (ROOT / ".github/workflows/landing.yml").read_text(encoding="utf-8")
        self.assertIn("vars.BFME_MERGE_QUEUE_ENABLED == 'yes'", enq_wf)
        self.assertIn("BFME II Boat comparison", enq_wf)
        self.assertIn("BFME II header rules", enq_wf)
        self.assertNotIn("BFME II source rules", enq_wf)
        self.assertIn("bfme-land-next", enq_wf)
        self.assertIn("github.event.workflow_run.conclusion == 'failure'", enq_wf)
        self.assertNotIn("\nconcurrency:\n", enq_wf)
        self.assertIn("\n    concurrency:\n", enq_wf)
        self.assertEqual(land_wf.count("vars.BFME_MERGE_QUEUE_ENABLED != 'yes'"), 2)


if __name__ == "__main__":
    unittest.main()
