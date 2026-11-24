"""Protected main state: an over-cap refresh skips cleanly, and the ancestor state lookup lists artifacts once."""
import contextlib
import io
import itertools
import os
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]
STATE_ROOT = Path(__file__).resolve().parents[2] / "managed-state"
REPO_ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(STATE_ROOT))

from common import REPO, REPO_ID  # noqa: E402
import state_fetch  # noqa: E402
import state_workflow  # noqa: E402

MAIN = "c" * 40
UNITS = [f"src/T{i:04}.cpp" for i in range(51 * 64)]


def plan_env(output, **extra):
    env = {"GITHUB_SHA": MAIN, "GITHUB_REPOSITORY": REPO, "GITHUB_REPOSITORY_ID": str(REPO_ID),
           "GITHUB_REF": "refs/heads/main", "GITHUB_EVENT_NAME": "push", "GITHUB_OUTPUT": str(output),
           "STATE_BUDGET": "8"}
    return env | extra


class OverBudgetPlan(unittest.TestCase):
    def plan(self, **extra):
        """Run `state_workflow.py plan` for a full refresh of 51 runners; return (error, outputs, notice, wrote)."""
        with tempfile.TemporaryDirectory() as td:
            tmp = Path(td)
            output = tmp / "github_output"
            output.write_text("")
            args = (str(tmp / "state-plan.json"), str(tmp / "carry.json"), str(tmp / "previous-state.json"))
            stdout = io.StringIO()
            error = None
            with mock.patch.dict(os.environ, plan_env(output, **extra), clear=True), \
                    mock.patch.object(state_workflow, "authoritative_units", return_value=UNITS), \
                    mock.patch.object(state_workflow, "inherited", return_value={}), \
                    mock.patch.object(state_workflow, "make_plan", return_value={"batches": []}), \
                    mock.patch.object(state_workflow, "validate_plan"), \
                    contextlib.redirect_stdout(stdout):
                try:
                    state_workflow.main("plan", *args)
                except ValueError as exc:
                    error = exc
            wrote = sorted(path.name for path in tmp.iterdir() if path.name != "github_output")
            return error, output.read_text(), stdout.getvalue(), wrote

    def test_routine_refresh_over_the_cap_measures_nothing_and_succeeds(self):
        error, outputs, notice, wrote = self.plan()
        self.assertIsNone(error)
        self.assertEqual(outputs, "active=no\n")
        self.assertEqual(wrote, [])
        self.assertIn("::notice title=Protected main state not measured::", notice)
        self.assertIn("requires 51 runners, above the 8-runner cap", notice)
        self.assertIn("seed enabled", notice)

    def test_an_explicit_seed_over_its_cap_still_fails(self):
        error, outputs, _, wrote = self.plan(GITHUB_EVENT_NAME="workflow_dispatch", BFME_STATE_SEED="true")
        self.assertIsInstance(error, state_workflow.OverBudget)
        self.assertEqual(str(error), "This full refresh requires 51 runners, above the 8-runner cap.")
        self.assertEqual((outputs, wrote), ("", []))

    def test_an_explicit_seed_uses_its_own_cap(self):
        env = {"BFME_STATE_SEED": "true", "STATE_SEED_BUDGET": "64", "STATE_BUDGET": "8"}
        self.assertEqual(state_workflow.plan_budget(env), "64")
        self.assertEqual(state_workflow.plan_budget(env | {"STATE_SEED_BUDGET": ""}), "8")
        self.assertEqual(state_workflow.plan_budget(env | {"BFME_STATE_SEED": "false"}), "8")
        self.assertEqual(state_workflow.plan_budget({}), "")
        with mock.patch.object(state_workflow, "make_plan", return_value={"batches": []}), \
                mock.patch.object(state_workflow, "validate_plan"):
            plan = state_workflow.make_checked_plan(Path("."), MAIN, "64", measured=UNITS)
            self.assertEqual(len(plan["batches"]), 51)
            with self.assertRaises(state_workflow.OverBudget):
                state_workflow.make_checked_plan(Path("."), MAIN, "50", measured=UNITS)
            with self.assertRaisesRegex(ValueError, "between 1 and 128"):
                state_workflow.make_checked_plan(Path("."), MAIN, "", measured=UNITS)

    def test_the_workflow_follows_the_plan_step(self):
        text = (REPO_ROOT / ".github/workflows/state.yml").read_text(encoding="utf-8")
        self.assertIn("active: '${{ steps.plan.outputs.active == ''yes'' && ''yes'' || ''no'' }}'", text)
        self.assertIn("STATE_SEED_BUDGET: '${{ vars.BFME_STATE_SEED_BUDGET_JOBS }}'", text)
        upload = text.split("name: bfme-state-plan-${{ github.run_attempt }}")[0].rsplit("- uses:", 1)[1]
        self.assertIn("if: steps.plan.outputs.active == 'yes'", upload)


def legacy_select_ancestor_artifact(api, shas):
    """The lookup before the artifact listing: one artifact call per nearest successful run."""
    filters = state_fetch.urllib.parse.urlencode({"branch": "main", "status": "success", "per_page": 100})
    runs = api.call("actions/workflows/state.yml/runs?" + filters)["workflow_runs"]
    by_base = {}
    for row in runs:
        sha = row.get("head_sha")
        if sha in set(shas) and state_fetch.verified_run(row, sha):
            by_base.setdefault(sha, (row["id"], row["run_attempt"]))
    for run, attempt in [by_base[sha] for sha in shas if sha in by_base][:25]:
        if artifact := state_fetch.select_artifact(api, run, attempt):
            return artifact
    return None


def sha(index):
    return f"{index:040x}"


def run_row(index, attempt=1, **extra):
    return {"id": 1000 + index, "run_attempt": attempt, "path": state_fetch.WORKFLOW, "head_sha": sha(index),
            "head_branch": "main", "status": "completed", "event": "push", "conclusion": "success",
            "repository": {"id": REPO_ID}} | extra


def artifact_row(run, attempt=1, artifact_id=None, **extra):
    return {"id": artifact_id or run + 500000, "name": f"bfme-exact-main-state-{attempt}", "expired": False,
            "size_in_bytes": 4096, "digest": "sha256:" + "d" * 64, "workflow_run": {"id": run}} | extra


class FakeApi:
    def __init__(self, runs, artifacts, *, fail_listing=False):
        self.runs, self.artifacts, self.fail_listing = runs, artifacts, fail_listing
        self.calls = []

    def call(self, path):
        self.calls.append(path)
        if path.startswith("actions/workflows/state.yml/runs?"):
            return {"workflow_runs": self.runs}
        query = dict(state_fetch.urllib.parse.parse_qsl(path.split("?", 1)[1]))
        if path.startswith("actions/artifacts?"):
            if self.fail_listing:
                raise ValueError("The GitHub API returned HTTP 502.")
            rows = [row for row in self.artifacts if row["name"] == query["name"]]
            return {"artifacts": rows[:int(query["per_page"])]}
        run = int(path.split("/")[2])
        return {"artifacts": [row for row in self.artifacts
                              if row["workflow_run"]["id"] == run and row["name"] == query["name"]]}


class AncestorArtifactLookup(unittest.TestCase):
    SHAS = [sha(i) for i in range(30)]

    def assert_same_as_legacy(self, runs, artifacts, **api_options):
        new_api, old_api = FakeApi(runs, artifacts, **api_options), FakeApi(runs, artifacts, **api_options)
        found = state_fetch.select_ancestor_artifact(new_api, self.SHAS)
        self.assertEqual(found, legacy_select_ancestor_artifact(old_api, self.SHAS))
        return found, new_api.calls, old_api.calls

    def test_runs_without_state_are_answered_by_one_listing(self):
        runs = [run_row(i) for i in range(25)]
        found, calls, legacy_calls = self.assert_same_as_legacy(runs, [artifact_row(1020)])
        self.assertEqual(found, (1020 + 500000, 4096, "d" * 64))
        self.assertEqual(len(calls), 2)
        self.assertEqual(len(legacy_calls), 1 + 21)
        self.assertIn("actions/artifacts?name=bfme-exact-main-state-1&per_page=100&page=1", calls)

    def test_nothing_retained_costs_two_calls_instead_of_twenty_six(self):
        found, calls, legacy_calls = self.assert_same_as_legacy([run_row(i) for i in range(30)], [])
        self.assertIsNone(found)
        self.assertEqual((len(calls), len(legacy_calls)), (2, 26))

    def test_the_nearest_verified_run_wins_and_unverified_runs_are_ignored(self):
        runs = [run_row(0, conclusion="failure"), run_row(1, path=".github/workflows/other.yml"),
                run_row(2, event="pull_request"), run_row(3), run_row(4)]
        artifacts = [artifact_row(1000), artifact_row(1001), artifact_row(1002), artifact_row(1003),
                     artifact_row(1004)]
        found, _, _ = self.assert_same_as_legacy(runs, artifacts)
        self.assertEqual(found[0], 1003 + 500000)

    def test_expired_duplicate_foreign_and_malformed_artifacts_match_the_per_run_rules(self):
        runs = [run_row(i) for i in range(6)]
        artifacts = [
            artifact_row(1000, expired=True),
            artifact_row(1001), artifact_row(1001, artifact_id=9),             # two copies: neither is trusted
            artifact_row(1002, name="bfme-numeric-main-state-1"),
            artifact_row(1003, digest="sha1:abc"),
            artifact_row(1004, size_in_bytes=0),
            artifact_row(1005),
        ]
        found, _, _ = self.assert_same_as_legacy(runs, artifacts)
        self.assertEqual(found[0], 1005 + 500000)

    def test_rerun_attempts_use_their_own_listing(self):
        runs = [run_row(0, attempt=2), run_row(1), run_row(2, attempt=3)]
        artifacts = [artifact_row(1000, attempt=1), artifact_row(1001, attempt=2), artifact_row(1002, attempt=3)]
        found, calls, _ = self.assert_same_as_legacy(runs, artifacts)
        self.assertEqual(found[0], 1002 + 500000)
        self.assertEqual(calls[1:], ["actions/artifacts?name=bfme-exact-main-state-2&per_page=100&page=1",
                                     "actions/artifacts?name=bfme-exact-main-state-1&per_page=100&page=1",
                                     "actions/artifacts?name=bfme-exact-main-state-3&per_page=100&page=1"])

    def test_a_full_or_unreadable_listing_falls_back_to_asking_each_run(self):
        runs = [run_row(i) for i in range(5)]
        # A newer run elsewhere in the listing proves the listing reaches past this run.
        newer = [artifact_row(5000 + i) for i in range(100)]
        artifacts = newer + [artifact_row(1003)]
        # A listing with as many rows as asked for may be cut short, so it cannot prove a run kept nothing.
        found, calls, _ = self.assert_same_as_legacy(runs, artifacts)
        self.assertEqual(found[0], 1003 + 500000)
        self.assertIn("actions/runs/1003/artifacts?name=bfme-exact-main-state-1&per_page=5", calls)
        found, calls, legacy_calls = self.assert_same_as_legacy(runs, artifacts, fail_listing=True)
        self.assertEqual(found[0], 1003 + 500000)
        self.assertEqual(calls[2:], legacy_calls[1:])

    def test_every_small_combination_matches_the_per_run_lookup(self):
        variants = [None, {}, {"expired": True}, {"digest": ""}, "duplicate"]
        for combo in itertools.product(range(len(variants)), repeat=4):
            runs, artifacts = [], []
            for index, choice in enumerate(combo):
                runs.append(run_row(index, attempt=1 + index % 2))
                variant = variants[choice]
                if variant is None:
                    continue
                if variant == "duplicate":
                    artifacts += [artifact_row(1000 + index, 1 + index % 2),
                                  artifact_row(1000 + index, 1 + index % 2, artifact_id=7 + index)]
                else:
                    artifacts.append(artifact_row(1000 + index, 1 + index % 2, **variant))
            with self.subTest(combo=combo):
                self.assert_same_as_legacy(runs, artifacts)


if __name__ == "__main__":
    unittest.main()
