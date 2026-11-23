"""Unit tests for Native GitHub Merge Queue Boat planning and reporting."""
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "ci/managed-check"))
sys.path.insert(1, str(ROOT / "ci/managed-nt2"))

import common  # noqa: E402
import intake  # noqa: E402
import merge_queue_plan as mq_plan  # noqa: E402
import merge_queue_report as mq_report  # noqa: E402
from gitrepo import Git  # noqa: E402

QUEUE_REF = "refs/heads/gh-readonly-queue/main/pr-42-0123456789abcdef"
GIT_ENV = {
    "PATH": os.environ.get("PATH", "/usr/bin:/bin"),
    "LC_ALL": "C",
    "GIT_CONFIG_NOSYSTEM": "1",
    "GIT_CONFIG_GLOBAL": os.devnull,
    "GIT_AUTHOR_NAME": "Test",
    "GIT_COMMITTER_NAME": "Test",
    "GIT_AUTHOR_EMAIL": "test@example.invalid",
    "GIT_COMMITTER_EMAIL": "test@example.invalid",
    "GIT_AUTHOR_DATE": "2026-01-01T00:00:00Z",
    "GIT_COMMITTER_DATE": "2026-01-01T00:00:00Z",
}


def run_git(cwd, *args):
    return subprocess.run(
        ["git", "-C", str(cwd), "-c", f"core.hooksPath={os.devnull}", *args],
        env=GIT_ENV, capture_output=True, check=True, text=True,
    ).stdout.strip()


def make_repo(root, files):
    work = root / "work"
    work.mkdir()
    run_git(work, "init", "-q", "-b", "main")
    for rel, content in files.items():
        dest = work / rel
        dest.parent.mkdir(parents=True, exist_ok=True)
        if isinstance(content, bytes):
            dest.write_bytes(content)
        else:
            dest.write_text(content, encoding="utf-8", newline="\n")
    run_git(work, "add", ".")
    run_git(work, "commit", "-q", "-m", "base")
    base_sha = run_git(work, "rev-parse", "HEAD")

    remote = root / "remote.git"
    run_git(root, "clone", "-q", "--bare", str(work), str(remote))
    return work, remote, base_sha


def commit_queue(work, remote, edits, *, deletes=(), queue_ref=QUEUE_REF):
    for rel, content in edits.items():
        dest = work / rel
        dest.parent.mkdir(parents=True, exist_ok=True)
        if isinstance(content, bytes):
            dest.write_bytes(content)
        else:
            dest.write_text(content, encoding="utf-8", newline="\n")
        run_git(work, "add", rel)
    for rel in deletes:
        run_git(work, "rm", "-q", rel)
    run_git(work, "commit", "-q", "--allow-empty", "-m", "queue batch")
    head_sha = run_git(work, "rev-parse", "HEAD")
    run_git(remote, "fetch", "-q", str(work), f"{head_sha}:{queue_ref}")
    return head_sha


def mq_env(tmp, base_sha, head_sha, *, head_ref=QUEUE_REF, event_name="merge_group", base_ref="refs/heads/main"):
    event_path = tmp / "event.json"
    event_path.write_text(json.dumps({
        "action": "checks_requested",
        "merge_group": {
            "base_sha": base_sha,
            "base_ref": base_ref,
            "head_sha": head_sha,
            "head_ref": head_ref,
        },
    }), encoding="utf-8")
    return {
        "GITHUB_EVENT_NAME": event_name,
        "GITHUB_EVENT_PATH": str(event_path),
        "GITHUB_OUTPUT": str(tmp / "outputs.txt"),
        "GITHUB_STEP_SUMMARY": str(tmp / "summary.md"),
        "GITHUB_RUN_ID": "123456",
    }


def read_outputs(path):
    if not Path(path).is_file():
        return {}
    return dict(line.split("=", 1) for line in Path(path).read_text(encoding="utf-8").splitlines() if "=" in line)


class FakeGitHub:
    def __init__(self):
        self.calls = []

    def status(self, binding, state, text, *, context=common.CONTEXT):
        self.calls.append((binding["head"], state, text, context))


class MergeQueuePlanTests(unittest.TestCase):
    def test_chunks_10_affected_tus_into_two_valid_compare_shards(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            base_files = {"include/Common.h": "int common_val();\n"}
            for i in range(10):
                base_files[f"src/Unit{i}.cpp"] = f'#include "Common.h"\nint f{i}() {{ return {i}; }}\n'
                base_files[f"expected/Unit{i}.cpp.json"] = "{}\n"
            work, remote, base_sha = make_repo(root, base_files)
            head_sha = commit_queue(work, remote, {"include/Common.h": "int common_val(); // updated\n"})

            # Check out base_sha in a separate shallow clone to mirror Job 1 checkout!
            checkout = root / "checkout"
            run_git(root, "clone", "-q", "--depth=1", f"file://{remote.as_posix()}", str(checkout))
            self.assertEqual(run_git(checkout, "rev-parse", "HEAD"), base_sha)

            out = root / "plans"
            env = mq_env(root, base_sha, head_sha)
            manifest = mq_plan.plan_merge_group(out, env=env, repo_dir=checkout, remote=str(remote))

            self.assertEqual(manifest["status"], "planned")
            self.assertEqual(manifest["shards"], [0, 1])
            self.assertEqual(manifest["total_tus"], 10)
            self.assertEqual(manifest["changed_count"], 1)
            outputs = read_outputs(env["GITHUB_OUTPUT"])
            self.assertEqual(outputs["shards"], "[0,1]")
            self.assertEqual(outputs["has_shards"], "yes")

            # Verify every shard matches compare.py + common.validate_binding + intake.reconstruct verbatim!
            rebuilt_git = Git(root / "verify.git", initialize=True)
            (rebuilt_git.path / "objects/info/alternates").write_text((remote / "objects").resolve().as_posix() + "\n")
            all_tus = []
            for idx, expected_count in ((0, 8), (1, 2)):
                shard_dir = out / f"shard-{idx}"
                plan = common.validate_binding(common.read(shard_dir / "plan.json"))
                decision = common.read(shard_dir / "decision.json")
                plan_hash = common.digest((shard_dir / "plan.json").read_bytes())
                self.assertEqual(decision["plan"], plan_hash)
                self.assertEqual(manifest["shard_plans"][str(idx)], plan_hash)
                self.assertEqual(len(plan["request_paths"]), expected_count)
                self.assertEqual(decision["kind"], "compile")
                for key in ("repo", "base", "head", "tree", "number"):
                    self.assertEqual(decision[key], plan[key])
                tip = intake.reconstruct(rebuilt_git, plan)
                self.assertEqual(rebuilt_git.value("rev-parse", f"{tip}^{{tree}}"), plan["tree"])
                all_tus.extend(plan["request_paths"])
            self.assertEqual(sorted(all_tus), [f"src/Unit{i}.cpp" for i in range(10)])

    def test_noop_when_header_affects_zero_tus(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            work, remote, base_sha = make_repo(root, {
                "include/Unused.h": "int unused_decl();\n",
                "src/Other.cpp": "int other() { return 0; }\n",
                "expected/Other.cpp.json": "{}\n",
            })
            head_sha = commit_queue(work, remote, {"include/Unused.h": "int unused_decl(); // doc\n"})
            out = root / "plans"
            env = mq_env(root, base_sha, head_sha)
            manifest = mq_plan.plan_merge_group(out, env=env, repo_dir=remote, remote=str(remote))
            self.assertEqual(manifest["status"], "noop")
            self.assertEqual(manifest["shards"], [])
            self.assertEqual(manifest["total_tus"], 0)
            outputs = read_outputs(env["GITHUB_OUTPUT"])
            self.assertEqual(outputs["has_shards"], "no")
            self.assertEqual(outputs["shards"], "[]")

    def test_refuses_too_many_affected_tus(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            base_files = {"include/Big.h": "int big();\n"}
            for i in range(193):
                base_files[f"src/U{i:03d}.cpp"] = f'#include "Big.h"\nint u{i}() {{ return 0; }}\n'
                base_files[f"expected/U{i:03d}.cpp.json"] = "{}\n"
            work, remote, base_sha = make_repo(root, base_files)
            head_sha = commit_queue(work, remote, {"include/Big.h": "int big(); // change\n"})
            out = root / "plans"
            env = mq_env(root, base_sha, head_sha)
            manifest = mq_plan.plan_merge_group(out, env=env, repo_dir=remote, remote=str(remote))
            self.assertEqual(manifest["status"], "refused")
            self.assertIn(">192", manifest["reason"])
            self.assertEqual(read_outputs(env["GITHUB_OUTPUT"])["has_shards"], "no")

    def test_g1_defense_in_depth_refusals(self):
        cases = [
            ("tools_dir", {"src/Code/Tools/exp.cpp": "int e() { return 1; }\n"}, (), "Disallowed path"),
            ("outside_root", {"README.md": "hi\n"}, (), "Disallowed path"),
            ("pragma_optimize", {"src/A.cpp": '#pragma optimize("", off)\nint a() { return 1; }\n'}, (), "Compiler source policy"),
            ("missing_expected", {"src/New.cpp": "int n() { return 1; }\n"}, (), "Missing expected ledger"),
            ("deleted_tu", {}, ("src/A.cpp",), "Affected TU missing on head"),
            ("empty_diff", {}, (), "no changed files"),
        ]
        for name, edits, deletes, fragment in cases:
            with self.subTest(case=name), tempfile.TemporaryDirectory() as td:
                root = Path(td)
                work, remote, base_sha = make_repo(root, {
                    "include/Stub.h": "int stub();\n",
                    "src/A.cpp": "int a() { return 0; }\n",
                    "expected/A.cpp.json": "{}\n",
                })
                head_sha = commit_queue(work, remote, edits, deletes=deletes)
                out = root / "plans"
                env = mq_env(root, base_sha, head_sha)
                manifest = mq_plan.plan_merge_group(out, env=env, repo_dir=remote, remote=str(remote))
                self.assertEqual(manifest["status"], "refused")
                self.assertIn(fragment, manifest["reason"])
                self.assertEqual(read_outputs(env["GITHUB_OUTPUT"])["has_shards"], "no")

    def test_refuses_over_60_changed_files(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            base_files = {}
            edits = {}
            for i in range(61):
                base_files[f"src/F{i:02d}.cpp"] = f"int f{i}() {{ return 0; }}\n"
                base_files[f"expected/F{i:02d}.cpp.json"] = "{}\n"
                edits[f"src/F{i:02d}.cpp"] = f"int f{i}() {{ return 1; }}\n"
            work, remote, base_sha = make_repo(root, base_files)
            head_sha = commit_queue(work, remote, edits)
            out = root / "plans"
            env = mq_env(root, base_sha, head_sha)
            manifest = mq_plan.plan_merge_group(out, env=env, repo_dir=remote, remote=str(remote))
            self.assertEqual(manifest["status"], "refused")
            self.assertIn("60", manifest["reason"])

    def test_refuses_non_ancestor_head(self):
        with tempfile.TemporaryDirectory() as td:
            root = Path(td)
            work, remote, base_sha = make_repo(root, {
                "src/A.cpp": "int a() { return 0; }\n",
                "expected/A.cpp.json": "{}\n",
            })
            tree = run_git(remote, "rev-parse", f"{base_sha}^{{tree}}")
            orphan = subprocess.run(
                ["git", "-C", str(remote), "commit-tree", tree, "-m", "orphan"],
                env=GIT_ENV, capture_output=True, check=True, text=True,
            ).stdout.strip()
            run_git(remote, "update-ref", QUEUE_REF, orphan)
            env = mq_env(root, base_sha, orphan)
            manifest = mq_plan.plan_merge_group(root / "plans", env=env, repo_dir=remote, remote=str(remote))
            self.assertEqual(manifest["status"], "refused")
            self.assertIn("ancestor", manifest["reason"])

    def test_parse_event_rejects_invalid_inputs(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = Path(td)
            good_base, good_head = "a" * 40, "b" * 40
            for kwargs in (
                {"event_name": "pull_request_target"},
                {"head_ref": "refs/heads/main"},
                {"head_ref": "refs/heads/gh-readonly-queue/main/a..b"},
                {"base_ref": "refs/heads/other"},
            ):
                env = mq_env(tmp, good_base, good_head, **kwargs)
                with self.assertRaises(ValueError):
                    mq_plan.parse_event(env)


class MergeQueueReportTests(unittest.TestCase):
    BASE = "1" * 40
    HEAD = "2" * 40
    TREE = "3" * 40
    PLAN0 = "a" * 64
    PLAN1 = "b" * 64

    def _verdict(self, plan_hash, kind="ok", *, base=None, head=None, tree=None, messages=()):
        return {
            "version": 1,
            "repo": common.REPO_ID,
            "number": 1,
            "base": base or self.BASE,
            "head": head or self.HEAD,
            "tree": tree or self.TREE,
            "plan": plan_hash,
            "report": {"kind": kind, "judged": 4, "messages": list(messages)},
            "counts": {"gained": 2, "lost_credit": 0},
            "functions": [],
        }

    def test_missing_manifest_reports_planner_failure(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = Path(td)
            api = FakeGitHub()
            env = mq_env(tmp, self.BASE, self.HEAD)
            rc = mq_report.report_merge_group(tmp / "plans", tmp / "verdicts", env=env, api=api)
            self.assertEqual(rc, 1)
            self.assertEqual(api.calls, [(self.HEAD, "failure",
                                          "Merge queue planner failed before producing a plan", common.CONTEXT)])

    def test_noop_manifest_reports_success(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = Path(td)
            plans = tmp / "plans"
            plans.mkdir()
            common.write(plans / "manifest.json", {
                "status": "noop", "shards": [], "base": self.BASE, "head": self.HEAD,
                "tree": self.TREE, "total_tus": 0,
            })
            api = FakeGitHub()
            env = mq_env(tmp, self.BASE, self.HEAD)
            rc = mq_report.report_merge_group(plans, tmp / "verdicts", env=env, api=api)
            self.assertEqual(rc, 0)
            self.assertEqual(api.calls[0][1], "success")
            self.assertIn(self.BASE[:12], api.calls[0][2])

    def test_refused_manifest_reports_failure(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = Path(td)
            plans = tmp / "plans"
            plans.mkdir()
            common.write(plans / "manifest.json", {
                "status": "refused", "reason": "Disallowed path: README.md",
                "shards": [], "base": self.BASE, "head": self.HEAD, "tree": self.TREE, "total_tus": 0,
            })
            api = FakeGitHub()
            env = mq_env(tmp, self.BASE, self.HEAD)
            rc = mq_report.report_merge_group(plans, tmp / "verdicts", env=env, api=api)
            self.assertEqual(rc, 1)
            self.assertEqual(api.calls[0][1], "failure")
            self.assertIn("Disallowed path: README.md", api.calls[0][2])

    def test_planned_all_pass_supports_both_artifact_layouts(self):
        with tempfile.TemporaryDirectory() as td:
            tmp = Path(td)
            plans, verdicts = tmp / "plans", tmp / "verdicts"
            plans.mkdir()
            common.write(plans / "manifest.json", {
                "status": "planned", "shards": [0, 1], "base": self.BASE, "head": self.HEAD,
                "tree": self.TREE, "total_tus": 8, "changed_count": 2,
                "shard_plans": {"0": self.PLAN0, "1": self.PLAN1},
            })
            (verdicts / "verdict-0").mkdir(parents=True)
            (verdicts / "bfme-mq-verdict-1").mkdir(parents=True)
            common.write(verdicts / "verdict-0/verdict.json", self._verdict(self.PLAN0, "ok"))
            common.write(verdicts / "bfme-mq-verdict-1/verdict.json", self._verdict(self.PLAN1, "ok"))

            api = FakeGitHub()
            env = mq_env(tmp, self.BASE, self.HEAD)
            rc = mq_report.report_merge_group(plans, verdicts, env=env, api=api)
            self.assertEqual(rc, 0)
            self.assertEqual(api.calls[0][1], "success")
            self.assertIn("8 TUs across 2 shard(s)", api.calls[0][2])
            summary = Path(env["GITHUB_STEP_SUMMARY"]).read_text(encoding="utf-8")
            self.assertIn("| 0 | Pass |", summary)
            self.assertIn("| 1 | Pass |", summary)

    def test_planned_regression_missing_or_mismatch_fails(self):
        for case, shard1_verdict, expected_text in (
            ("regression", self._verdict(self.PLAN1, "regression", messages=["src/Foo.cpp: compile error"]), "src/Foo.cpp: compile error"),
            ("missing", None, "Shard 1: missing verdict"),
            ("tampered_plan", self._verdict("f" * 64, "ok"), "Shard 1: verdict binding mismatch"),
            ("tampered_head", self._verdict(self.PLAN1, "ok", head="9" * 40), "Shard 1: verdict binding mismatch"),
        ):
            with self.subTest(case=case), tempfile.TemporaryDirectory() as td:
                tmp = Path(td)
                plans, verdicts = tmp / "plans", tmp / "verdicts"
                plans.mkdir()
                common.write(plans / "manifest.json", {
                    "status": "planned", "shards": [0, 1], "base": self.BASE, "head": self.HEAD,
                    "tree": self.TREE, "total_tus": 8, "changed_count": 2,
                    "shard_plans": {"0": self.PLAN0, "1": self.PLAN1},
                })
                (verdicts / "bfme-mq-verdict-0").mkdir(parents=True)
                common.write(verdicts / "bfme-mq-verdict-0/verdict.json", self._verdict(self.PLAN0, "ok"))
                if shard1_verdict is not None:
                    (verdicts / "bfme-mq-verdict-1").mkdir(parents=True)
                    common.write(verdicts / "bfme-mq-verdict-1/verdict.json", shard1_verdict)
                api = FakeGitHub()
                env = mq_env(tmp, self.BASE, self.HEAD)
                rc = mq_report.report_merge_group(plans, verdicts, env=env, api=api)
                self.assertEqual(rc, 1)
                self.assertEqual(api.calls[0][1], "failure")
                self.assertLessEqual(len(api.calls[0][2]), 140)
                self.assertIn(expected_text, api.calls[0][2])


class WorkflowSafetyTests(unittest.TestCase):
    def test_merge_queue_boat_workflow_invariants(self):
        wf = (ROOT / ".github/workflows/merge-queue-boat.yml").read_text(encoding="utf-8")
        for required in (
            "merge_group:",
            "vars.BFME_MERGE_QUEUE_ENABLED == 'yes'",
            "actions/checkout@11bd71901bbe5b1630ceea73d27597364c9af683",
            "actions/upload-artifact@ea165f8d65b6e75b540449e92b4886f43607fa02",
            "actions/download-artifact@d3f86a106a0bac45b974a628896c90dbdf5c8093",
            "actions/cache/restore@0057852bfaa89a56745cba8c7296529d2fc39830",
            "ci/managed-check/compare.py",
            "ci/managed-check/merge_queue_plan.py",
            "ci/managed-check/merge_queue_report.py",
            "boat-small/from=bfme2-compiler-v1",
        ):
            self.assertIn(required, wf)


if __name__ == "__main__":
    unittest.main()
