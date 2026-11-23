"""Tests for disjoint-ancestor publication in github_status, github_io, and inline_status."""
import json
import os
from pathlib import Path
import sys
import tempfile
import types
import unittest
from unittest.mock import patch

ROOT = Path(__file__).resolve().parents[1]
ENGINE = ROOT.parent / "managed-nt2"
sys.path.insert(0, str(ROOT))
sys.path.insert(1, str(ENGINE))

import common  # noqa: E402
import github_io  # noqa: E402
import github_status  # noqa: E402
import inline_status  # noqa: E402

BASE = "a" * 40
NEW_MAIN = "f" * 40
HEAD = "b" * 40
TREE = "c" * 40
PLAN = "d" * 64
EVIDENCE = "e" * 64


class StubGitHub(github_status.GitHub):
    """Real GitHub status/disjoint logic with only HTTP transport stubbed."""

    responses = {}
    posted = []

    def __init__(self):
        self.token = "ghp_" + "x" * 36

    def call(self, path, value=None):
        if value is not None and path.startswith("statuses/"):
            StubGitHub.posted.append((path, dict(value)))
            return {"state": value["state"], "context": value["context"]}
        if path not in StubGitHub.responses:
            raise ValueError(f"The GitHub API returned HTTP 404. ({path})")
        res = StubGitHub.responses[path]
        if isinstance(res, Exception):
            raise res
        return res


def _pr_doc(*, number=560, head=HEAD, state="open", base_ref="main", author_id=777,
            head_repo_id=999999, association="NONE"):
    return {
        "number": number,
        "state": state,
        "draft": False,
        "author_association": association,
        "user": {"id": author_id, "login": "contrib"},
        "base": {"ref": base_ref, "repo": {"id": common.REPO_ID}},
        "head": {"sha": head, "repo": {"id": head_repo_id}},
        "labels": [],
    }


def _compare_doc(*, base=BASE, ahead_by=3, behind_by=0, status="ahead",
                 merge_base=None, base_commit=None, files=None):
    return {
        "status": status,
        "ahead_by": ahead_by,
        "behind_by": behind_by,
        "base_commit": {"sha": base if base_commit is None else base_commit},
        "merge_base_commit": {"sha": base if merge_base is None else merge_base},
        "files": files if files is not None else [
            {"filename": "src/unk/O00999999.cpp", "status": "modified"},
        ],
    }


def _run_finish(*, pr_files=None, compare_doc=None, actual_main=NEW_MAIN, pr_doc=None,
                decision_kind="compile", automatic=True, automatic_kind=None,
                verdict_kind="ok", event_name="repository_dispatch",
                decision_extra=None):
    StubGitHub.posted = []
    pr = pr_doc if pr_doc is not None else _pr_doc(
        head_repo_id=999999 if automatic else common.REPO_ID,
        association="NONE" if automatic else "OWNER",
    )
    StubGitHub.responses = {
        "pulls/560": pr,
        "git/ref/heads/main": {"object": {"sha": actual_main}},
        "pulls/560/files?per_page=100&page=1": (
            pr_files if pr_files is not None else [{"filename": "src/unk/O00123456.cpp", "status": "modified"}]
        ),
        f"compare/{BASE}...{actual_main}": compare_doc if compare_doc is not None else _compare_doc(),
    }
    with tempfile.TemporaryDirectory() as tmp:
        tmp = Path(tmp)
        adm, res, rep = tmp / "adm", tmp / "res", tmp / "report"
        adm.mkdir()
        res.mkdir()
        event_path = tmp / "event.json"
        if event_name == "repository_dispatch":
            event_payload = {
                "action": "bfme-remeasure",
                "client_payload": {"number": 560, "head": HEAD, "base": BASE},
            }
        else:
            event_payload = {"action": "synchronize", "pull_request": pr}
        event_path.write_text(json.dumps(event_payload), encoding="utf-8")

        decision = {
            "version": 1, "repo": common.REPO_ID, "number": 560,
            "base": BASE, "head": HEAD, "tree": TREE, "plan": PLAN,
            "kind": decision_kind,
        }
        if automatic:
            decision["automatic_author"] = pr["user"]["id"]
        if automatic_kind is not None:
            decision["automatic_kind"] = automatic_kind
        if decision_extra:
            decision.update(decision_extra)
        (adm / "decision.json").write_text(json.dumps(decision), encoding="utf-8")

        verdict = {
            "version": 1, "repo": common.REPO_ID, "number": 560,
            "base": BASE, "head": HEAD, "tree": TREE, "plan": PLAN,
            "report": {"kind": verdict_kind, "messages": [], "judged": 1},
            "counts": {}, "functions": [],
        }
        if decision_kind == "wide":
            verdict["matrix"] = {"runners": 1, "paired_results": 2}
        (res / "verdict.json").write_text(json.dumps(verdict), encoding="utf-8")

        out_file, sum_file = tmp / "output", tmp / "summary"
        env = {
            "GITHUB_REPOSITORY": common.REPO,
            "GITHUB_REPOSITORY_ID": str(common.REPO_ID),
            "GITHUB_REF": "refs/heads/main",
            "GITHUB_SHA": BASE,
            "GITHUB_EVENT_NAME": event_name,
            "GITHUB_EVENT_PATH": str(event_path),
            "GITHUB_RUN_ID": "999001",
            "GITHUB_RUN_ATTEMPT": "1",
            "GITHUB_ACTOR_ID": "41898282" if automatic else "1760627",
            "BFME_TRIGGERING_ACTOR": "github-actions[bot]" if automatic else "Ancalgonn",
            "BFME_AUTO_ADMISSION": "yes",
            "BFME_PR_COMPACT": "yes",
            "BFME_AUTO_HEADERS": "yes",
            "BFME_AUTOMATIC_CLAIM": "yes" if automatic else "",
            "GITHUB_OUTPUT": str(out_file),
            "GITHUB_STEP_SUMMARY": str(sum_file),
            "MANAGED_PUBLIC_REPORT": str(rep),
        }
        fake_quality = types.SimpleNamespace(quality_report=lambda base, head, record: "report")
        code = 0
        with patch.dict(os.environ, env, clear=False), \
                patch.dict(sys.modules, {"quality_format": fake_quality}), \
                patch.object(github_io, "GitHub", StubGitHub):
            try:
                github_io.publish("finish", adm, res)
            except SystemExit as exc:
                code = exc.code
            except github_status.Superseded:
                code = 2
        output_text = out_file.read_text(encoding="utf-8") if out_file.exists() else ""
        verdict_written = (rep / "verdict.json").is_file()
        return code, output_text, list(StubGitHub.posted), verdict_written


class DisjointTuPathTests(unittest.TestCase):
    def test_is_disjoint_tu_path_rules(self):
        for good in (
            "src/Code/GameEngine/Source/Common/Thing.cpp",
            "src/Code/Libraries/Source/WWVegas/WWLib/crc.c",
            "src/unk/O00123456.cpp",
            "src/unk/G00001000.CPP",
            "src/nbench/Benchmark/emfloat.c",
        ):
            with self.subTest(good=good):
                self.assertTrue(github_status.is_disjoint_tu_path(good))

        for bad in (
            "",
            "include/GameLogic/Object.h",
            "config/map.json",
            "expected/unk/O00123456.cpp.json",
            "tools/gates/gate.py",
            "ci/landing/lander.py",
            ".github/workflows/managed-pr.yml",
            "src/Code/Tools/WW3D/pluglib/matrix4.cpp",
            "src/code/tools/foo.cpp",
            "src/Code/TOOLS/bar.c",
            "src/stlport-4.6/STLport-4.6/src/c_locale.c",
            "src/unk/O00123456.h",
            "src/unk/../include/Foo.cpp",
            "/src/unk/O00123456.cpp",
        ):
            with self.subTest(bad=bad):
                self.assertFalse(github_status.is_disjoint_tu_path(bad))


class DisjointAncestorPublishTests(unittest.TestCase):
    def test_passing_narrow_verdict_publishes_when_main_advanced_1_to_32_disjoint_commits(self):
        for pr_path, ahead_by in (
            ("src/Code/GameEngine/Source/A.cpp", 1),
            ("src/unk/O00123456.cpp", 12),
            ("src/nbench/Benchmark/emfloat.c", 32),
        ):
            with self.subTest(pr_path=pr_path, ahead_by=ahead_by):
                code, output, posted, wrote_verdict = _run_finish(
                    pr_files=[{"filename": pr_path, "status": "modified"}],
                    compare_doc=_compare_doc(
                        ahead_by=ahead_by,
                        files=[
                            {"filename": "src/Code/GameEngine/Source/Other.cpp", "status": "modified"},
                            {"filename": "src/unk/O00999999.cpp", "status": "modified"},
                            {"filename": "docs/agents.md", "status": "modified"},
                        ],
                    ),
                )
                self.assertEqual(code, 0)
                self.assertTrue(wrote_verdict)
                self.assertIn("verdict=success", output.splitlines())
                self.assertEqual(len(posted), 1)
                path, body = posted[0]
                self.assertEqual(path, f"statuses/{HEAD}")
                self.assertEqual(body["state"], "success")
                self.assertEqual(body["context"], common.CONTEXT)
                self.assertEqual(body["description"], f"BFME II comparison passed (main {BASE[:12]})")

    def test_narrow_regression_publishes_failure_on_disjoint_ancestor_without_verdict_success(self):
        code, output, posted, wrote_verdict = _run_finish(verdict_kind="regression")
        self.assertEqual(code, 1)
        self.assertTrue(wrote_verdict)
        self.assertNotIn("verdict=success", output)
        self.assertEqual(len(posted), 1)
        self.assertEqual(posted[0][1]["state"], "failure")
        self.assertEqual(posted[0][1]["description"], f"BFME II check refused this patch (main {BASE[:12]})")

    def test_superseded_when_main_touched_header_config_tools_vendored_or_non_modified(self):
        bad_intervening_cases = [
            [{"filename": "include/GameLogic/Object.h", "status": "modified"}],
            [{"filename": "config/map.json", "status": "modified"}],
            [{"filename": "expected/unk/O00123456.cpp.json", "status": "modified"}],
            [{"filename": "tools/gates/rules.py", "status": "modified"}],
            [{"filename": "ci/managed-check/compare.py", "status": "modified"}],
            [{"filename": ".github/workflows/managed-pr.yml", "status": "modified"}],
            [{"filename": "src/Code/Tools/WW3D/pluglib/matrix4.cpp", "status": "modified"}],
            [{"filename": "src/stlport-4.6/STLport-4.6/src/c_locale.c", "status": "modified"}],
            [{"filename": "src/unk/O00999999.cpp", "status": "removed"}],
            [{"filename": "src/unk/O00999999.cpp", "status": "added"}],
            [{"filename": "src/unk/O00999999.cpp", "previous_filename": "src/unk/O00888888.cpp", "status": "renamed"}],
        ]
        for intervening in bad_intervening_cases:
            with self.subTest(intervening=intervening):
                code, output, posted, wrote_verdict = _run_finish(
                    compare_doc=_compare_doc(files=intervening),
                )
                self.assertEqual(code, 2)
                self.assertEqual(posted, [])
                self.assertEqual(output, "")
                self.assertFalse(wrote_verdict)

    def test_superseded_when_main_touched_overlapping_file_case_insensitive(self):
        code, output, posted, wrote_verdict = _run_finish(
            pr_files=[{"filename": "src/unk/O00123456.cpp", "status": "modified"}],
            compare_doc=_compare_doc(
                files=[{"filename": "src/unk/o00123456.cpp", "status": "modified"}],
            ),
        )
        self.assertEqual((code, output, posted, wrote_verdict), (2, "", [], False))

    def test_superseded_when_main_over_32_commits_ahead_or_not_strict_ancestor(self):
        bad_compares = [
            _compare_doc(ahead_by=33),
            _compare_doc(ahead_by=0, status="identical"),
            _compare_doc(behind_by=1, status="diverged"),
            _compare_doc(merge_base="1" * 40),
            _compare_doc(base_commit="1" * 40),
            _compare_doc(files=[]),
            _compare_doc(files=[{"filename": f"src/unk/O{i:08X}.cpp", "status": "modified"} for i in range(300)]),
        ]
        for cmp_doc in bad_compares:
            with self.subTest(cmp_doc=cmp_doc.get("status"), ahead=cmp_doc.get("ahead_by"), nfiles=len(cmp_doc.get("files") or [])):
                code, output, posted, wrote_verdict = _run_finish(compare_doc=cmp_doc)
                self.assertEqual((code, output, posted, wrote_verdict), (2, "", [], False))

    def test_superseded_when_pr_head_moved_or_closed_or_non_main_base(self):
        for pr in (
            _pr_doc(head="9" * 40),
            _pr_doc(state="closed"),
            _pr_doc(base_ref="develop"),
        ):
            with self.subTest(head=pr["head"]["sha"][:6], state=pr["state"], base=pr["base"]["ref"]):
                code, output, posted, wrote_verdict = _run_finish(pr_doc=pr)
                self.assertEqual((code, output, posted, wrote_verdict), (2, "", [], False))

    def test_superseded_for_wide_pr_or_header_pr_files(self):
        # Wide decision kind
        code, output, posted, wrote_verdict = _run_finish(
            decision_kind="wide", automatic_kind="wide",
            pr_files=[{"filename": "include/Game/Foo.h", "status": "modified"}],
        )
        self.assertEqual((code, output, posted, wrote_verdict), (2, "", [], False))

        # Narrow compile route with automatic_kind == "wide" (header affecting <= 8 TUs)
        code, output, posted, wrote_verdict = _run_finish(
            decision_kind="compile", automatic_kind="wide",
            pr_files=[{"filename": "src/unk/O00123456.cpp", "status": "modified"}],
        )
        self.assertEqual((code, output, posted, wrote_verdict), (2, "", [], False))

        # Trusted/manual compile route whose PR files include a header or forbidden path
        for pr_files in (
            [{"filename": "include/Game/Foo.h", "status": "modified"},
             {"filename": "src/Code/A.cpp", "status": "modified"}],
            [{"filename": "src/Code/Tools/pluglib/matrix4.cpp", "status": "modified"}],
            [{"filename": f"src/unk/O{i:08X}.cpp", "status": "modified"} for i in range(9)],
        ):
            with self.subTest(pr_files=len(pr_files)):
                code, output, posted, wrote_verdict = _run_finish(
                    decision_kind="compile", automatic=False, pr_files=pr_files,
                    event_name="pull_request_target",
                )
                self.assertEqual((code, output, posted, wrote_verdict), (2, "", [], False))

    def test_default_event_and_current_remain_strict_on_moved_main(self):
        StubGitHub.responses = {
            "pulls/560": _pr_doc(),
            "git/ref/heads/main": {"object": {"sha": NEW_MAIN}},
            "pulls/560/files?per_page=100&page=1": [{"filename": "src/unk/O00123456.cpp", "status": "modified"}],
            f"compare/{BASE}...{NEW_MAIN}": _compare_doc(),
        }
        api = StubGitHub()
        with tempfile.TemporaryDirectory() as tmp:
            ev = Path(tmp) / "event.json"
            ev.write_text(json.dumps({
                "action": "bfme-remeasure",
                "client_payload": {"number": 560, "head": HEAD, "base": BASE},
            }), encoding="utf-8")
            env = {
                "GITHUB_REPOSITORY": common.REPO,
                "GITHUB_REPOSITORY_ID": str(common.REPO_ID),
                "GITHUB_REF": "refs/heads/main",
                "GITHUB_SHA": BASE,
                "GITHUB_EVENT_NAME": "repository_dispatch",
                "GITHUB_EVENT_PATH": str(ev),
            }
            with patch.dict(os.environ, env, clear=False):
                with self.assertRaises(github_status.Superseded):
                    github_status.event(api)
                binding = github_status.event(api, allow_moved_main=True)
                with self.assertRaises(github_status.Superseded):
                    api.current(binding)
                self.assertEqual(api.current_or_disjoint(binding)["number"], 560)

    def test_inline_status_publishes_on_disjoint_ancestor_for_narrow_compile_only(self):
        for decision_kind, state, expect_superseded in (
            ("compile", "pass", False),
            ("compile", "fail", False),
            ("wide", "pass", True),
        ):
            with self.subTest(decision_kind=decision_kind, state=state):
                StubGitHub.posted = []
                StubGitHub.responses = {
                    "pulls/560": _pr_doc(),
                    "git/ref/heads/main": {"object": {"sha": NEW_MAIN}},
                    "pulls/560/files?per_page=100&page=1": [{"filename": "src/unk/O00123456.cpp", "status": "modified"}],
                    f"compare/{BASE}...{NEW_MAIN}": _compare_doc(),
                }
                with tempfile.TemporaryDirectory() as tmp:
                    tmp = Path(tmp)
                    ev = tmp / "event.json"
                    ev.write_text(json.dumps({
                        "action": "bfme-remeasure",
                        "client_payload": {"number": 560, "head": HEAD, "base": BASE},
                    }), encoding="utf-8")
                    decision_p = tmp / "decision.json"
                    decision_p.write_text(json.dumps({
                        "version": 1, "repo": common.REPO_ID, "number": 560,
                        "base": BASE, "head": HEAD, "tree": TREE, "plan": PLAN,
                        "kind": decision_kind,
                    }), encoding="utf-8")
                    inline_p = tmp / "inline.json"
                    inline_p.write_text(json.dumps({
                        "version": 1, "repo": common.REPO_ID, "number": 560,
                        "run": "999001", "attempt": "1",
                        "binding": {"base": BASE, "head": HEAD, "tree": TREE},
                        "plan": PLAN, "policy_sha256": common.policy(),
                        "verdict": state, "state": state, "evidence_sha256": EVIDENCE,
                    }), encoding="utf-8")
                    env = {
                        "GITHUB_REPOSITORY": common.REPO,
                        "GITHUB_REPOSITORY_ID": str(common.REPO_ID),
                        "GITHUB_REF": "refs/heads/main",
                        "GITHUB_SHA": BASE,
                        "GITHUB_EVENT_NAME": "repository_dispatch",
                        "GITHUB_EVENT_PATH": str(ev),
                        "GITHUB_RUN_ID": "999001",
                        "GITHUB_RUN_ATTEMPT": "1",
                    }
                    with patch.dict(os.environ, env, clear=False), \
                            patch.object(inline_status, "GitHub", StubGitHub):
                        if expect_superseded:
                            with self.assertRaises(github_status.Superseded):
                                inline_status.publish(inline_p, decision_p)
                            self.assertEqual(StubGitHub.posted, [])
                        else:
                            inline_status.publish(inline_p, decision_p)
                            self.assertEqual(len(StubGitHub.posted), 1)
                            expected_state = "success" if state == "pass" else "failure"
                            self.assertEqual(StubGitHub.posted[0][1]["state"], expected_state)
                            self.assertIn(f"(main {BASE[:12]})", StubGitHub.posted[0][1]["description"])


if __name__ == "__main__":
    unittest.main()
