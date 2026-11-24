"""A compiler check started by repository_dispatch wakes the landing and enqueue workflows after it reported.

GitHub starts no workflow_run for a run whose trigger was sent with the Actions token, so landing.yml (which listens
to workflow_run of the comparison) never heard of those checks. ci/managed-check/github_io.py now writes
verdict=success to the step output after it posted a passing status for the current main. managed-pr.yml's
notify-landing job sends bfme-land-next after any report, whatever the verdict: a refusal or an outage frees a slot
for the merge queue backfill (ci/queue/enqueue.py), which never re-requests a head within its cooldown.
"""
import io
import json
import os
from pathlib import Path
import sys
import tempfile
import types
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "ci/managed-check"))
sys.path.insert(1, str(ROOT / "ci/managed-nt2"))
import github_io  # noqa: E402
from common import REPO_ID  # noqa: E402

BASE, HEAD, TREE, PLAN = "a" * 40, "b" * 40, "c" * 40, "d" * 64
BINDING = {"base": BASE, "head": HEAD, "number": 21}


class FakeGitHub:
    statuses = []

    def current(self, binding):
        return {"user": {"id": 1}}

    def status(self, binding, state, description, context=None):
        FakeGitHub.statuses.append((state, description))


def finish(kind, *, approved=True, decision_kind="compile"):
    FakeGitHub.statuses = []
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        (tmp / "adm").mkdir()
        (tmp / "res").mkdir()
        decision = {"version": 1, "repo": REPO_ID, "base": BASE, "head": HEAD, "number": 21,
                    "kind": decision_kind, "tree": TREE, "plan": PLAN}
        (tmp / "adm/decision.json").write_text(json.dumps(decision))
        verdict = {"version": 1, "repo": REPO_ID, "base": BASE, "head": HEAD, "number": 21, "tree": TREE,
                   "plan": PLAN, "report": {"kind": kind, "messages": []}, "counts": {}, "functions": []}
        (tmp / "res/verdict.json").write_text(json.dumps(verdict))
        output, summary = tmp / "output", tmp / "summary"
        env = {"GITHUB_OUTPUT": str(output), "GITHUB_STEP_SUMMARY": str(summary),
               "MANAGED_PUBLIC_REPORT": str(tmp / "report")}
        fake_quality = types.SimpleNamespace(quality_report=lambda base, head, record: "report")

        def authority(pr, binding, environ):
            if not approved:
                return {"approved": False}
            return {"approved": True}
        code = 0
        with patch.dict(os.environ, env), patch.dict(sys.modules, {"quality_format": fake_quality}), \
                patch.object(github_io, "GitHub", FakeGitHub), patch.object(github_io, "event", lambda api: BINDING), \
                patch.object(github_io, "authority", authority):
            try:
                github_io.publish("finish", tmp / "adm", tmp / "res")
            except SystemExit as exc:
                code = exc.code
            except github_io.Superseded:
                code = 2
        return code, output.read_text() if output.exists() else "", list(FakeGitHub.statuses)


class Verdict(unittest.TestCase):
    def test_a_passing_verdict_is_announced(self):
        code, output, statuses = finish("ok")
        self.assertEqual(code, 0)
        self.assertEqual(statuses, [("success", f"BFME II comparison passed (main {BASE[:12]})")])
        self.assertIn("verdict=success", output.splitlines())

    def test_a_refusal_an_outage_or_a_wait_is_not(self):
        for kind, expected in (("gate", 1), ("regression", 1), ("infrastructure", 1), ("pending", 0)):
            with self.subTest(kind=kind):
                code, output, _ = finish(kind)
                self.assertEqual(code, expected)
                self.assertNotIn("verdict=success", output)

    def test_a_superseded_run_announces_nothing(self):
        code, output, statuses = finish("ok", approved=False)
        self.assertEqual((code, statuses), (2, []))
        self.assertNotIn("verdict=success", output)


def job(name):
    text = (ROOT / ".github/workflows/managed-pr.yml").read_text()
    start = text.index(f"\n  {name}:\n")
    end = text.find("\n  ", start + len(name) + 5)
    while end != -1 and text[end + 3] == " ":
        end = text.find("\n  ", end + 1)
    return text[start:end if end != -1 else len(text)]


class Workflow(unittest.TestCase):
    def test_notify_landing_is_guarded_and_holds_nothing_from_the_pull_request(self):
        block = job("notify-landing")
        for guard in ("needs: [report, fallback-report]",
                      "needs.report.result != 'skipped' || needs.fallback-report.result != 'skipped'",
                      "github.repository_id == '1464323350'", "github.ref == 'refs/heads/main'",
                      "github.event_name == 'repository_dispatch'", "github.actor_id == '41898282'",
                      '"bfme-land-next"', "client_payload: {}"):
            self.assertIn(guard, block)
        permissions = block.split("permissions:")[1].split("steps:")[0]
        self.assertEqual([line.strip() for line in permissions.strip().splitlines()], ["contents: write"])
        for forbidden in ("actions/checkout", "github.event.client_payload", "pull_request", "download-artifact"):
            self.assertNotIn(forbidden, block)

    def test_both_reporters_expose_a_pass_only_from_the_publishing_step(self):
        for name in ("report", "fallback-report"):
            self.assertIn("passed: ${{ steps.native.outcome == 'success' && steps.native.outputs.verdict == 'success'"
                          " && 'yes' || 'no' }}", job(name))

    def test_the_landing_workflow_listens_for_it(self):
        landing = (ROOT / ".github/workflows/landing.yml").read_text()
        self.assertIn("types: [bfme-land-next]", landing)


if __name__ == "__main__":
    unittest.main()
