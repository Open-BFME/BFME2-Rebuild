"""Tests for state carry across ci/queue/ and ci/control-plane/ changes,
the R1 overcap-exit state.yml migration, and the artifact-first ancestor lookup.

Run from the repository root:  python3 -B -m unittest discover -s ci/state/tests -v
"""
from pathlib import Path
import sys
import unittest

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
sys.path.insert(0, str(ROOT / "ci/managed-state"))
sys.path.insert(0, str(ROOT / "ci/managed-check"))

import state_delta  # noqa: E402
import state_fetch  # noqa: E402


class ReportDirsTest(unittest.TestCase):
    def test_queue_and_control_plane_in_report_dirs(self):
        self.assertIn("ci/queue/", state_delta.REPORT_DIRS)
        self.assertIn("ci/control-plane/", state_delta.REPORT_DIRS)

    def test_queue_path_is_publication(self):
        self.assertTrue("ci/queue/enqueue.py".startswith(state_delta.REPORT_DIRS))

    def test_control_plane_path_is_publication(self):
        self.assertTrue("ci/control-plane/dispatch.py".startswith(state_delta.REPORT_DIRS))


class OvercapExitMigrationTest(unittest.TestCase):
    def test_overcap_exit_migration_registered(self):
        pair = state_delta.OVERCAP_EXIT_MIGRATION
        self.assertIn(pair, state_delta.STATE_FLOW_MIGRATIONS)
        self.assertEqual(pair[0], "ec1ec3a1489c459a4513cfef8937101998a740667c0662d15e4df00f102bce61")
        self.assertEqual(pair[1], "ad0a65303832e04fc813d9a49d875b645f47e6d70b3422ba50d141914487e62d")

    def test_overcap_exit_chains_from_hydration(self):
        self.assertEqual(state_delta.OVERCAP_EXIT_MIGRATION[0], state_delta.HYDRATION_MIGRATION[1])


class ArtifactFirstLookupTest(unittest.TestCase):
    def test_artifact_first_fallback_finds_artifact_past_run_window(self):
        from common import REPO_ID
        target_sha = "b" * 40

        class FakeApi:
            def __init__(self):
                self.calls = []

            def call(self, path):
                self.calls.append(path)
                if path.startswith("actions/workflows/state.yml/runs?"):
                    return {"workflow_runs": []}
                if "actions/artifacts?" in path:
                    return {"artifacts": [
                        {"name": "bfme-exact-main-state-1", "expired": False, "id": 999,
                         "size_in_bytes": 1000, "digest": "sha256:" + "a" * 64,
                         "workflow_run": {"id": 100}},
                    ]}
                if path == "actions/runs/100":
                    return {"id": 100, "run_attempt": 1, "path": ".github/workflows/state.yml",
                            "head_sha": target_sha, "head_branch": "main", "status": "completed",
                            "event": "push", "conclusion": "success",
                            "repository": {"id": REPO_ID}}
                if path.startswith("actions/runs/") and "/artifacts" in path:
                    return {"artifacts": []}
                return {"workflow_runs": []}

        api = FakeApi()
        shas = ["c" * 40] * 30 + [target_sha]
        result = state_fetch.select_ancestor_artifact(api, shas)
        self.assertIsNotNone(result)
        self.assertEqual(result[0], 999)

    def test_no_artifacts_returns_none(self):
        class EmptyApi:
            def call(self, path):
                if "actions/artifacts?" in path:
                    return {"artifacts": []}
                return {"workflow_runs": []}

        result = state_fetch.select_ancestor_artifact(EmptyApi(), ["a" * 40])
        self.assertIsNone(result)


if __name__ == "__main__":
    unittest.main()
