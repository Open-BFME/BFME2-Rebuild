"""Offline checks for the source workflow using GitHub event layouts and real Git."""

from copy import deepcopy
import json
import os
from pathlib import Path
import shutil
import subprocess
from tempfile import TemporaryDirectory
import unittest
from unittest.mock import patch

import workflow as source
from gate import POLICY_FILES
from source_report import contributor_copy, public_report


REPO = {"id": source.REPOSITORY_ID, "full_name": source.REPOSITORY, "private": False}
FORK = {"id": 218064526, "full_name": "contributor/rebuild",
        "clone_url": "https://example.invalid/contributor/rebuild.git", "fork": True, "private": False}
PR = 17
QUEUE = "refs/heads/gh-readonly-queue/main/pr-17-0123456789abcdef"
TOKEN = "fixture_token_never_sent"
HERE = Path(__file__).resolve().parent
RUN = subprocess.run
GIT_ENV = {"PATH": os.environ.get("PATH", "/usr/bin:/bin"), "LC_ALL": "C",
           "GIT_CONFIG_NOSYSTEM": "1", "GIT_CONFIG_GLOBAL": os.devnull,
           "GIT_AUTHOR_NAME": "Source fixture", "GIT_COMMITTER_NAME": "Source fixture",
           "GIT_AUTHOR_EMAIL": "fixture@example.invalid", "GIT_COMMITTER_EMAIL": "fixture@example.invalid",
           "GIT_AUTHOR_DATE": "2026-01-01T00:00:00Z", "GIT_COMMITTER_DATE": "2026-01-01T00:00:00Z"}


def git(path, *args, input=None):
    return RUN(["git", "-C", str(path), "-c", f"core.hooksPath={os.devnull}", *args],
               env=GIT_ENV, input=input, capture_output=True, check=True, text=True).stdout.strip()


def pull(base, head, fork=True):
    # Layout from pull_request_target and GET /pulls/{number}.
    return {"number": PR, "state": "open", "title": "A source edit", "body": "Change one return value",
            "draft": False, "author_association": "FIRST_TIME_CONTRIBUTOR", "labels": [],
            "merge_commit_sha": "f" * 40,
            "base": {"ref": "main", "sha": "f" * 40, "repo": deepcopy(REPO)},
            "head": {"ref": "feature/size", "sha": head, "repo": deepcopy(FORK if fork else REPO),
                     "label": "contributor:feature/size"},
            "user": {"login": "contributor"}}


class FakeApi(source.GitHub):
    def __init__(self, case):
        self.case = case
        self.reads = []
        self.posts = []

    def call(self, path, body=None, *, missing=None):
        if body is not None:
            self.posts.append((path, body))
            return body
        self.reads.append(path)
        if path == "":
            return self.case.metadata
        if path == f"pulls/{PR}":
            return self.case.pr
        if path == f"pulls/{PR}/files?per_page=100":
            return [{"filename": name, "status": "modified"}
                    for name in git(self.case.origin, "diff", "--name-only", self.case.base,
                                    self.case.pr["head"]["sha"]).splitlines()]
        if path == "git/ref/heads/main":
            return {"object": {"sha": self.case.main}}
        if path == "git/ref/" + QUEUE.removeprefix("refs/"):
            if self.case.queue_head is None:
                raise source.CheckUnavailable(missing)
            return {"object": {"sha": self.case.queue_head}}
        if path.startswith("compare/") and "..." in path:
            base, head = path.removeprefix("compare/").split("...", 1)
            if base == head:
                return {"status": "identical"}
            if RUN(["git", "-C", str(self.case.origin), "merge-base", "--is-ancestor", base, head],
                   env=GIT_ENV, capture_output=True).returncode == 0:
                files = git(self.case.origin, "diff", "--name-only", base, head).splitlines()
                return {"status": "ahead", "behind_by": 0,
                        "ahead_by": int(git(self.case.origin, "rev-list", "--count", f"{base}..{head}")),
                        "base_commit": {"sha": base}, "merge_base_commit": {"sha": base},
                        "files": [{"filename": name, "status": "modified"} for name in files]}
            if RUN(["git", "-C", str(self.case.origin), "merge-base", "--is-ancestor", head, base],
                   env=GIT_ENV, capture_output=True).returncode == 0:
                return {"status": "behind"}
            return {"status": "diverged"}
        raise AssertionError(path)


class Delivery(unittest.TestCase):
    @classmethod
    def setUpClass(cls):
        cls.scratch = TemporaryDirectory(prefix="bfme-source-fixture-", dir=os.environ.get("BFME_SOURCE_TEST_TMP"))
        cls.addClassCleanup(cls.scratch.cleanup)
        cls.root = Path(cls.scratch.name)
        cls.trusted = cls.root / "protected"
        cls.trusted.mkdir()
        git(cls.trusted, "init", "-q", "-b", "main")
        source_file = cls.trusted / "src/Size.cpp"
        source_file.parent.mkdir()
        source_file.write_text("int Size() { return 1; }\n")
        ledger = cls.trusted / "config/map.json"
        ledger.parent.mkdir()
        ledger.write_text('{"rows": []}\n')
        cls.trusted_tools = cls.trusted / "tools/gates"
        cls.trusted_tools.mkdir(parents=True)
        for basename in (*POLICY_FILES, "workflow.py", "source_report.py", "shadow_types.py"):
            shutil.copyfile(HERE / basename, cls.trusted_tools / basename)
        git(cls.trusted, "add", "--", "src", "config", "tools")
        git(cls.trusted, "commit", "-q", "-m", "Protected base")
        cls.base = git(cls.trusted, "rev-parse", "HEAD")
        cls.other = cls.root / "fork"
        git(cls.root, "clone", "-q", str(cls.trusted), str(cls.other))
        fork_source = cls.other / "src/Size.cpp"
        fork_source.write_text("int Size() { return 2; }\n")
        git(cls.other, "add", "src/Size.cpp")
        git(cls.other, "commit", "-q", "-m", "Source edit from fork")
        cls.head = git(cls.other, "rev-parse", "HEAD")
        cls.marker = cls.root / "candidate-code-ran"
        (cls.other / "tools/gates/workflow.py").write_text(
            f"from pathlib import Path\nPath({str(cls.marker)!r}).touch()\n")
        (cls.other / ".github/workflows").mkdir(parents=True)
        (cls.other / ".github/workflows/rules.yml").write_text("name: a different workflow\n")
        git(cls.other, "add", "--", "tools/gates/workflow.py", ".github/workflows/rules.yml")
        git(cls.other, "commit", "-q", "-m", "Protected path changes from fork")
        cls.protected_head = git(cls.other, "rev-parse", "HEAD")
        cls.origin = cls.root / "target.git"
        git(cls.root, "clone", "-q", "--bare", str(cls.trusted), str(cls.origin))
        git(cls.origin, "fetch", "-q", str(cls.other), f"{cls.head}:refs/fixture/source",
            f"{cls.protected_head}:refs/fixture/protected")
        tree = git(cls.origin, "rev-parse", cls.head + "^{tree}")
        cls.group = git(cls.origin, "commit-tree", tree, "-p", cls.base, input="Queue candidate")
        cls.unrelated = git(cls.origin, "commit-tree", tree, input="Independent candidate")
        changed_tools = git(cls.origin, "rev-parse", cls.protected_head + "^{tree}")
        cls.protected_group = git(cls.origin, "commit-tree", changed_tools, "-p", cls.base, input="Queue protected path")
        cls.bad_tools = cls.other / "tools/gates/workflow.py"

    def setUp(self):
        self.case = TemporaryDirectory(prefix="run-", dir=self.root)
        self.addCleanup(self.case.cleanup)
        self.run_root = Path(self.case.name)
        self.metadata = deepcopy(REPO)
        self.main = self.base
        self.queue_head = self.group
        self.pr = pull(self.base, self.head)
        self.fetched = []
        self.commands = []
        self.api = FakeApi(self)
        self.start_patch(patch.object(source, "__file__", str(self.trusted_tools / "workflow.py")))
        self.start_patch(patch.object(source, "GitHub", side_effect=self.api_fixture))
        self.start_patch(patch.object(source.subprocess, "run", side_effect=self.record))
        self.receive = source.receive
        self.start_patch(patch.object(source, "receive", side_effect=self.fetch_fixture))
        git(self.origin, "update-ref", "refs/heads/main", self.base)
        git(self.origin, "update-ref", f"refs/pull/{PR}/head", self.head)
        git(self.origin, "update-ref", QUEUE, self.group)

    def start_patch(self, mock):
        self.addCleanup(mock.stop)
        return mock.start()

    def api_fixture(self, binding, token):
        self.assertEqual(token, TOKEN)
        self.api.binding = binding
        return self.api

    def record(self, args, **kwargs):
        if "fetch" in args or (len(args) > 5 and str(args[4]).endswith("gate.py")):
            self.commands.append((args, kwargs.get("env", {})))
        return RUN(args, **kwargs)

    def fetch_fixture(self, git_store, binding, remote, token=None):
        self.fetched.append((binding, remote, token))
        self.assertEqual(remote, f"https://github.com/{source.REPOSITORY}.git")
        self.receive(git_store, binding, str(self.origin), token)

    def environment(self, name="pull_request_target", *, head=None, protected=None, action=None):
        head = head or self.head
        workspace = self.run_root / str(len(tuple(self.run_root.iterdir())))
        workspace.mkdir()
        event = workspace / "event.json"
        if name == "merge_group":
            payload = {"action": action or "checks_requested", "repository": deepcopy(REPO),
                       "merge_group": {"base_sha": protected or self.base, "base_ref": source.MAIN_REF,
                                       "head_sha": head, "head_ref": QUEUE,
                                       "head_commit": {"id": head, "message": "A queue fixture"}}}
            sha, ref = head, QUEUE
        elif name == "repository_dispatch":
            payload = {"action": action or "bfme-remeasure", "repository": deepcopy(REPO),
                       "client_payload": {"number": PR, "head": head, "base": protected or self.base}}
            sha, ref = self.base, source.MAIN_REF
        else:
            self.pr["head"]["sha"] = head
            payload = {"action": action or "opened", "number": PR, "repository": deepcopy(REPO),
                       "pull_request": self.pr, "sender": {"login": "contributor", "id": 101}}
            sha, ref = protected or self.base, source.MAIN_REF
        event.write_text(json.dumps(payload))
        return {"GITHUB_API_URL": source.API, "GITHUB_SERVER_URL": "https://github.com",
                "GITHUB_REPOSITORY": source.REPOSITORY, "GITHUB_REPOSITORY_ID": str(source.REPOSITORY_ID),
                "GITHUB_SHA": sha, "GITHUB_REF": ref, "GITHUB_EVENT_NAME": name,
                "GITHUB_EVENT_PATH": str(event), "GITHUB_OUTPUT": str(workspace / "outputs"),
                "GITHUB_STEP_SUMMARY": str(workspace / "summary"), "GITHUB_RUN_ID": "102",
                "RUNNER_TEMP": str(workspace)}

    def output(self, env):
        result = Path(env["GITHUB_OUTPUT"])
        return dict(line.split("=", 1) for line in result.read_text().splitlines()) if result.is_file() else {}

    def authorize(self, env):
        source.authorize(env | {"GH_TOKEN": TOKEN})
        return self.output(env)

    def evaluate(self, env):
        choices = self.authorize(env)
        if "transport" in choices:
            fetch = env | {"SOURCE_TRANSPORT": choices["transport"]}
            if choices["transport"] == "private":
                fetch["GH_TOKEN"] = TOKEN
            source.acquire(fetch)
        choices = self.output(env)
        source.evaluate(env | {"SOURCE_OBJECTS": choices.get("scratch", ""),
                               "FETCH_REASON": choices.get("reason", "")})
        data = json.loads(self.output(env)["status"])
        markdown = Path(env["GITHUB_STEP_SUMMARY"]).read_text()
        return data, markdown

    def publish(self, env, receipt):
        return source.publish(env | {"RULE_STATUS": json.dumps(receipt), "GH_TOKEN": TOKEN})

    def assert_statuses(self, head, failures=()):
        # One aggregate status per run; the failing rule IDs are folded into its description.
        self.assertEqual(len(self.api.posts), 1)
        path, row = self.api.posts[0]
        self.assertEqual(path, f"statuses/{head}")
        self.assertEqual(row["context"], "BFME II / source rules")
        self.assertEqual(row["state"], "failure" if failures else "success")
        text = f"Fix needed in {', '.join(failures)}" if failures else "Source rules passed"
        self.assertEqual(row["description"], f"{text} (main {self.base[:12]})")
        self.assertEqual(row["target_url"], f"https://github.com/{source.REPOSITORY}/actions/runs/102")

    def advance_main(self, edits):
        """A new main commit on top of the protected base, published to the fixture origin and the fake API."""
        work = self.run_root / f"main-{len(tuple(self.run_root.iterdir()))}"
        git(self.root, "clone", "-q", str(self.trusted), str(work))
        for rel, content in edits.items():
            (work / rel).parent.mkdir(parents=True, exist_ok=True)
            (work / rel).write_text(content)
            git(work, "add", rel)
        git(work, "commit", "-q", "-m", "Main moved")
        commit = git(work, "rev-parse", "HEAD")
        git(self.origin, "fetch", "-q", str(work), f"{commit}:refs/heads/main")
        self.main = commit
        return commit

    def test_public_fork_passes_before_admission(self):
        env = self.environment()
        result, markdown = self.evaluate(env)
        self.assertEqual(result["base"], self.base)
        self.assertEqual(result["head"], self.head)
        self.assertEqual(result["policy_sha256"], "8a3f65ae59a671b962868a8b156207ab7c5c8a483ef02615f3ea238f01740454")
        self.assertIn("| G1 Paths | Pass |", markdown)
        self.assertEqual(self.publish(env, result), 0)
        self.assert_statuses(self.head)
        self.assertEqual(len(self.fetched), 1)
        store = Path(self.output(env)["scratch"]) / "objects.git"
        self.assertEqual(git(store, "rev-parse", "--is-bare-repository"), "true")
        self.assertIsNone(self.fetched[0][2])
        fetches = [(args, env) for args, env in self.commands if "fetch" in args and "prlimit" in args]
        self.assertEqual(len(fetches), 2)  # Candidate and optional merge ref.
        self.assertTrue(any(f"+refs/pull/{PR}/head:refs/source-rules/head" in args for args, _ in fetches))
        for _, env in self.commands:
            self.assertTrue({"GH_TOKEN", "GITHUB_TOKEN", "ACTIONS_RUNTIME_TOKEN", "GIT_CONFIG_COUNT"}.isdisjoint(env))

    def test_fork_metadata_cannot_steer_binding_transfer_or_verdict(self):
        env = self.environment()
        original, markdown = self.evaluate(env)
        self.pr.update(title="Text about a different branch", body="Text with $(literal) and a different base",
                       labels=[{"name": "boat-ok"}, {"name": "try a different path"}], author_association="NONE")
        self.pr["head"] |= {"label": "contributor:gh-readonly-queue/main/forged",
                           "ref": "gh-readonly-queue/main/forged",
                           "repo": {"id": 17, "clone_url": "https://example.invalid/changed.git"}}
        self.pr["base"]["sha"] = self.protected_head
        env = self.environment(action="synchronize")
        changed, changed_markdown = self.evaluate(env)
        self.assertEqual(changed, original)
        self.assertEqual(changed_markdown, markdown)
        self.assertEqual(self.fetched[0], self.fetched[1])

    def test_private_fork_never_receives_a_transport_token(self):
        self.metadata["private"] = True
        result, markdown = self.evaluate(self.environment())
        self.assertEqual(result["reason"], "fork-unavailable")
        self.assertIn("while the repository is private", markdown)
        self.assertEqual(self.fetched, [])

    def test_private_same_repository_route_still_evaluates(self):
        self.metadata["private"] = True
        self.pr = pull(self.base, self.head, fork=False)
        env = self.environment()
        result, _ = self.evaluate(env)
        self.assertEqual(self.fetched[0][2], TOKEN)
        self.assertEqual(self.publish(env, result), 0)
        self.assert_statuses(self.head)

    def test_public_fetch_rejects_an_unexpected_token(self):
        env = self.environment()
        source.acquire(env | {"SOURCE_TRANSPORT": "public", "GH_TOKEN": TOKEN})
        self.assertEqual(self.output(env)["reason"], "fetch-unavailable")
        self.assertEqual(self.fetched, [])

    def test_fork_change_cannot_replace_protected_tools(self):
        git(self.origin, "update-ref", f"refs/pull/{PR}/head", self.protected_head)
        env = self.environment(head=self.protected_head)
        result, markdown = self.evaluate(env)
        self.assertEqual(result["states"]["G1"], "failure")
        self.assertIn(".github/workflows/rules.yml:1", markdown)
        self.assertIn("Remove this path from your pull request", markdown)
        self.assertFalse(self.marker.exists())
        self.assertEqual(self.publish(env, result), 1)
        self.assert_statuses(self.protected_head, ("G1",))

    def test_shadow_or_fork_tools_checkout_is_rejected(self):
        env = self.environment()
        with patch.object(source, "__file__", str(self.bad_tools)):
            self.assertEqual(self.authorize(env)["reason"], "tools-unavailable")
        self.assertEqual(self.fetched, [])

    def test_main_has_to_match_the_protected_event_sha(self):
        env = self.environment(protected=self.head)
        self.assertEqual(self.authorize(env)["reason"], "tools-unavailable")
        env = self.environment()
        self.main = self.head
        self.assertEqual(self.authorize(env)["reason"], "main-moved")
        self.assertEqual(self.fetched, [])

    def test_repository_dispatch_uses_the_same_anonymous_fork_route(self):
        env = self.environment("repository_dispatch")
        result, _ = self.evaluate(env)
        self.assertEqual(result["base"], self.base)
        self.assertIsNone(self.fetched[0][2])
        self.assertEqual(self.publish(env, result), 0)
        self.assert_statuses(self.head)

    def test_source_refresh_dispatch_takes_the_same_anonymous_fork_route(self):
        env = self.environment("repository_dispatch", action="bfme-source-refresh")
        result, _ = self.evaluate(env)
        self.assertEqual((result["base"], result["state"]), (self.base, "pass"))
        self.assertIsNone(self.fetched[0][2])
        self.assertEqual(self.publish(env, result), 0)
        self.assert_statuses(self.head)

    def test_an_unknown_dispatch_action_is_refused(self):
        env = self.environment("repository_dispatch", action="bfme-something-else")
        self.assertEqual(self.authorize(env)["reason"], "unexpected-event")
        self.assertEqual(self.fetched, [])

    def test_a_conflict_is_named_on_the_source_rules_status(self):
        env = self.environment("repository_dispatch", action="bfme-source-refresh")
        receipt = source.pending(source.event(env), "merge-conflict")
        self.assertEqual(self.publish(env, receipt), 1)
        rows = {row["context"]: row for _, row in self.api.posts}
        self.assertEqual(rows["BFME II / source rules"]["state"], "error")
        self.assertEqual(rows["BFME II / source rules"]["description"],
                         f"{source.CONFLICT_OUTCOME} (main {self.base[:12]})")
        self.assertEqual(list(rows), ["BFME II / source rules"])
        self.api.posts.clear()
        self.publish(env, source.pending(source.event(env), "merge-history"))
        rows = {row["context"]: row for _, row in self.api.posts}
        self.assertEqual(rows["BFME II / source rules"]["description"],
                         f"{source.HISTORY_OUTCOME} (main {self.base[:12]})")
        other = source.pending(source.event(env), "fetch-unavailable")
        self.api.posts.clear()
        self.publish(env, other)
        rows = {row["context"]: row for _, row in self.api.posts}
        self.assertEqual(rows["BFME II / source rules"]["description"],
                         f"Source rules could not finish (main {self.base[:12]})")

    def test_old_manual_route_cannot_select_an_unprotected_base(self):
        env = self.environment() | {"GITHUB_EVENT_NAME": "workflow_dispatch"}
        self.assertEqual(self.authorize(env)["reason"], "unexpected-event")
        self.assertEqual(self.fetched, [])

    def test_repository_dispatch_cannot_choose_a_different_base(self):
        env = self.environment("repository_dispatch", protected=self.head)
        self.assertEqual(self.authorize(env)["reason"], "main-moved")
        self.assertEqual(self.fetched, [])

    def test_pr_head_move_after_evaluation_writes_no_statuses(self):
        env = self.environment()
        result, _ = self.evaluate(env)
        self.pr["head"]["sha"] = self.protected_head
        self.assertEqual(self.publish(env, result), 1)
        self.assertEqual(self.api.posts, [])

    def test_publish_reads_main_once_and_writes_one_status(self):
        env = self.environment()
        result, markdown = self.evaluate(env)
        self.api.reads.clear()
        self.assertEqual(self.publish(env, result), 0)
        self.assertEqual(self.api.reads, [f"pulls/{PR}", "git/ref/heads/main"])
        self.assert_statuses(self.head)
        # The full rules table stays in the job summary.
        for rule in source.RULES:
            self.assertRegex(markdown, rf"\n\| {rule} [^|\n]+ \| Pass \|\n")

    def test_failing_rules_are_named_on_the_one_status(self):
        env = self.environment()
        result, _ = self.evaluate(env)
        failed = result | {"state": "fail", "overall": "failure",
                           "states": result["states"] | {"G1": "failure", "G5": "failure"}}
        self.assertEqual(self.publish(env, failed), 1)
        self.assert_statuses(self.head, ("G1", "G5"))

    def test_pr_check_publishes_for_its_main_when_main_moved_on_other_paths(self):
        env = self.environment()
        result, _ = self.evaluate(env)
        self.advance_main({"docs/Notes.md": "Unrelated notes\n"})
        self.assertEqual(self.publish(env, result), 0)
        self.assert_statuses(self.head)
        receipt = json.loads(self.output(env)["receipt"])
        self.assertEqual((receipt["base"], receipt["tools_commit"], receipt["state"]), (self.base, self.base, "pass"))

    def test_pr_check_started_after_a_disjoint_main_move_still_judges_its_main(self):
        self.advance_main({"docs/Notes.md": "Unrelated notes\n"})
        env = self.environment()
        result, _ = self.evaluate(env)
        self.assertEqual((result["base"], result["tools_commit"], result["state"]), (self.base, self.base, "pass"))
        self.assertEqual(self.publish(env, result), 0)
        self.assert_statuses(self.head)

    def test_pr_check_writes_nothing_when_main_moved_on_its_paths_or_rule_inputs(self):
        for edits in ({"src/Size.cpp": "int Size() { return 3; }\n"}, {"SRC/size.cpp": "int Other();\n"},
                      {"tools/gates/extra.py": "VALUE = 1\n"}, {"config/symbols.json": "{}\n"}):
            with self.subTest(edits=edits):
                self.main = self.base
                git(self.origin, "update-ref", "refs/heads/main", self.base)
                env = self.environment()
                result, _ = self.evaluate(env)
                self.advance_main(edits)
                self.api.posts.clear()
                self.assertEqual(self.publish(env, result), 1)
                self.assertEqual(self.api.posts, [])
                self.assertEqual(json.loads(self.output(env)["receipt"])["reason"], "main-moved")

    def test_pr_check_writes_nothing_when_main_was_replaced(self):
        env = self.environment()
        result, _ = self.evaluate(env)
        self.main = self.unrelated
        self.assertEqual(self.publish(env, result), 1)
        self.assertEqual(self.api.posts, [])

    def test_queue_uses_candidate_tree_and_writes_to_queue_sha(self):
        env = self.environment("merge_group", head=self.group)
        result, markdown = self.evaluate(env)
        self.assertEqual(result["base"], self.base)
        self.assertEqual(result["tools_commit"], self.base)
        self.assertEqual(result["head"], self.group)
        self.assertEqual(result["tree"], git(self.origin, "rev-parse", self.group + "^{tree}"))
        self.assertEqual(result["projection"], "queue-candidate")
        self.assertIn("| G1 Paths | Pass |", markdown)
        self.assertIsNone(self.fetched[0][2])
        self.assertEqual(self.publish(env, result), 0)
        self.assert_statuses(self.group)
        self.assertNotIn(f"pulls/{PR}", self.api.reads)
        fetches = [args for args, _ in self.commands if "fetch" in args and "prlimit" in args]
        self.assertEqual(len(fetches), 1)
        self.assertIn(f"+{QUEUE}:refs/source-rules/head", fetches[0])

    def test_queue_protected_change_fails_on_candidate_sha(self):
        git(self.origin, "update-ref", QUEUE, self.protected_group)
        self.queue_head = self.protected_group
        env = self.environment("merge_group", head=self.protected_group)
        result, markdown = self.evaluate(env)
        self.assertEqual(result["states"]["G1"], "failure")
        self.assertIn("tools/gates/workflow.py:1", markdown)
        self.assertFalse(self.marker.exists())
        self.assertEqual(self.publish(env, result), 1)
        self.assert_statuses(self.protected_group, ("G1",))

    def test_queue_event_binds_both_platform_fields_and_the_repository(self):
        for key, value in (("base_ref", "refs/heads/other"), ("head_ref", "refs/heads/main"),
                           ("head_sha", self.head), ("base_sha", self.group), ("base_sha", ""),
                           ("head_ref", "refs/heads/gh-readonly-queue/main/other..route")):
            with self.subTest(key=key, value=value):
                env = self.environment("merge_group", head=self.group)
                path = Path(env["GITHUB_EVENT_PATH"])
                payload = json.loads(path.read_bytes())
                payload["merge_group"][key] = value
                path.write_text(json.dumps(payload))
                self.assertEqual(self.authorize(env)["reason"], "unexpected-event")
        env = self.environment("merge_group", head=self.group)
        self.assertEqual(self.authorize(env | {"GITHUB_REPOSITORY_ID": "2"})["reason"], "unexpected-event")
        self.assertEqual(self.fetched, [])

    def test_queue_advanced_or_deleted_ref_writes_no_statuses(self):
        env = self.environment("merge_group", head=self.group)
        result, _ = self.evaluate(env)
        for actual in (self.head, None):
            self.queue_head = actual
            self.assertEqual(self.publish(env, result), 1)
        self.assertEqual(self.api.posts, [])

    def test_queue_wrong_ancestry_cannot_produce_success(self):
        self.queue_head = self.unrelated
        git(self.origin, "update-ref", QUEUE, self.unrelated)
        result, markdown = self.evaluate(self.environment("merge_group", head=self.unrelated))
        self.assertEqual(result["reason"], "queue-history")
        self.assertIn("missing its recorded main commit", markdown)

    def test_queue_pending_if_main_changes_before_publish(self):
        env = self.environment("merge_group", head=self.group)
        result, _ = self.evaluate(env)
        self.main = self.head
        self.assertEqual(self.publish(env, result), 1)
        self.assertEqual(self.api.posts, [])

    def test_status_writer_rejects_extra_fields_and_wrong_candidate_kind(self):
        env = self.environment("merge_group", head=self.group)
        result, _ = self.evaluate(env)
        for altered in (result | {"notes": "untrusted extra text"},
                        result | {"projection": "verified-merge-ref"}):
            self.assertEqual(self.publish(env, altered), 1)
        self.assertEqual(self.api.posts, [])

    def _make_batch_commit(self, edits, *, parent=None):
        parent = parent or self.base
        work = self.run_root / f"batch-{len(tuple(self.run_root.iterdir()))}"
        git(self.root, "clone", "-q", str(self.trusted), str(work))
        for rel, content in edits.items():
            dest = work / rel
            dest.parent.mkdir(parents=True, exist_ok=True)
            dest.write_text(content)
            git(work, "add", rel)
        git(work, "commit", "-q", "-m", "Batch edit")
        commit = git(work, "rev-parse", "HEAD")
        git(self.origin, "fetch", "-q", str(work), f"{commit}:refs/fixture/batch-{commit[:8]}")
        tree = git(self.origin, "rev-parse", f"{commit}^{{tree}}")
        return git(self.origin, "commit-tree", tree, "-p", parent, input="Batched queue candidate")

    def test_queue_batched_merge_group_overrides_g6_when_g1_passes(self):
        # Set up 5 existing src/*.cpp files on base so G1 passes while per-PR G6 (limit 3) triggers.
        base_work = self.run_root / "multi-base"
        git(self.root, "clone", "-q", str(self.trusted), str(base_work))
        for i in range(5):
            (base_work / f"src/Unit{i}.cpp").write_text(f"int Unit{i}() {{ return {i}; }}\n")
        (base_work / "include").mkdir(exist_ok=True)
        (base_work / "include/Shared.h").write_text("class SharedThing { int x; };\n")
        git(base_work, "add", "src", "include")
        git(base_work, "commit", "-q", "-m", "Add five units and shared header")
        multi_base = git(base_work, "rev-parse", "HEAD")
        git(self.origin, "fetch", "-q", str(base_work), f"+{multi_base}:refs/heads/main")
        self.main = multi_base

        # Edit all 5 units in one batch (exceeds G6 limit of 3 substantive files).
        batch_work = self.run_root / "multi-batch"
        git(self.root, "clone", "-q", str(base_work), str(batch_work))
        for i in range(5):
            (batch_work / f"src/Unit{i}.cpp").write_text(f"int Unit{i}() {{ return {i + 10}; }}\n")
        git(batch_work, "add", "src")
        git(batch_work, "commit", "-q", "-m", "Batched 5-unit merge group")
        batch_head = git(batch_work, "rev-parse", "HEAD")
        git(self.origin, "fetch", "-q", str(batch_work), f"+{batch_head}:{QUEUE}")
        git(self.origin, "update-ref", f"refs/pull/{PR}/head", batch_head)

        # In merge_group, G6 is overridden to pass when G1 passes.
        self.queue_head = batch_head
        with patch.object(source, "verified_tools", lambda _b: None):
            env = self.environment("merge_group", head=batch_head, protected=multi_base)
            result, markdown = self.evaluate(env)
            self.assertEqual(result["state"], "pass")
            self.assertEqual(result["overall"], "success")
            self.assertEqual(result["states"]["G6"], "success")
            self.assertIn("| G6 Change scope | Pass |", markdown)
            self.assertEqual(self.publish(env, result), 0)

            # On a normal pull_request_target, the same 5-file change MUST still fail G6.
            self.api.posts.clear()
            pr_env = self.environment("pull_request_target", head=batch_head, protected=multi_base)
            pr_result, pr_markdown = self.evaluate(pr_env)
            self.assertEqual(pr_result["state"], "fail")
            self.assertEqual(pr_result["overall"], "failure")
            self.assertEqual(pr_result["states"]["G6"], "failure")
            self.assertIn("| G6 Change scope | Fail |", pr_markdown)

            # In merge_group, G7 (repo-wide one-definition) MUST STILL FAIL even when G6 is overridden.
            g7_work = self.run_root / "multi-g7"
            git(self.root, "clone", "-q", str(batch_work), str(g7_work))
            (g7_work / "src/Unit0.cpp").write_text("class SharedThing { int y; };\nint Unit0() { return 10; }\n")
            git(g7_work, "add", "src/Unit0.cpp")
            git(g7_work, "commit", "-q", "-m", "Duplicate class in batch")
            g7_head = git(g7_work, "rev-parse", "HEAD")
            git(self.origin, "fetch", "-q", str(g7_work), f"+{g7_head}:{QUEUE}")
            self.queue_head = g7_head
            g7_env = self.environment("merge_group", head=g7_head, protected=multi_base)
            g7_result, _ = self.evaluate(g7_env)
            self.assertEqual(g7_result["state"], "fail")
            self.assertEqual(g7_result["overall"], "failure")
            self.assertEqual(g7_result["states"]["G6"], "success")
            self.assertEqual(g7_result["states"]["G7"], "failure")

            # In merge_group, G1 failure MUST NOT override G6.
            g1_work = self.run_root / "multi-g1"
            git(self.root, "clone", "-q", str(batch_work), str(g1_work))
            (g1_work / "README.md").write_text("disallowed\n")
            git(g1_work, "add", "README.md")
            git(g1_work, "commit", "-q", "-m", "G1 violation in batch")
            g1_head = git(g1_work, "rev-parse", "HEAD")
            git(self.origin, "fetch", "-q", str(g1_work), f"+{g1_head}:{QUEUE}")
            self.queue_head = g1_head
            g1_env = self.environment("merge_group", head=g1_head, protected=multi_base)
            g1_result, _ = self.evaluate(g1_env)
            self.assertEqual(g1_result["state"], "fail")
            self.assertEqual(g1_result["states"]["G1"], "failure")
            self.assertEqual(g1_result["states"]["G6"], "failure")

    def test_stacked_queue_uses_source_tools_sha_as_trusted_main_base(self):
        # Stacked merge_group: main -> q1 (group.base_sha) -> q2 (group.head_sha),
        # with SOURCE_TOOLS_SHA pointing to protected main.
        q1 = self._make_batch_commit({"src/Size.cpp": "int Size() { return 2; }\n"}, parent=self.base)
        q2 = self._make_batch_commit({"src/Size.cpp": "int Size() { return 3; }\n"}, parent=q1)
        git(self.origin, "update-ref", QUEUE, q2)
        self.queue_head = q2

        env = self.environment("merge_group", head=q2, protected=q1) | {"SOURCE_TOOLS_SHA": self.base}
        result, markdown = self.evaluate(env)
        self.assertEqual(result["state"], "pass")
        self.assertEqual(result["overall"], "success")
        self.assertEqual(result["base"], self.base)
        self.assertEqual(result["tools_commit"], self.base)
        self.assertEqual(result["head"], q2)
        self.assertEqual(result["projection"], "queue-candidate")
        self.assertIn("| G1 Paths | Pass |", markdown)
        self.assertEqual(self.publish(env, result), 0)
        self.assert_statuses(q2)

        # Invalid or head-equal SOURCE_TOOLS_SHA is still rejected.
        for bad_sha in ("not-a-sha", q2):
            bad_env = self.environment("merge_group", head=q2, protected=q1) | {"SOURCE_TOOLS_SHA": bad_sha}
            self.assertEqual(self.authorize(bad_env)["reason"], "unexpected-event")

    def test_stacked_queue_survives_main_advancing_to_queue_ancestor(self):
        # Batch #1 (q1) merges into main while Batch #2 (q2, stacked on q1) is in flight.
        q1 = self._make_batch_commit({"src/Size.cpp": "int Size() { return 2; }\n"}, parent=self.base)
        q2 = self._make_batch_commit({"src/Size.cpp": "int Size() { return 3; }\n"}, parent=q1)
        git(self.origin, "update-ref", QUEUE, q2)
        git(self.origin, "update-ref", "refs/heads/main", q1)
        self.queue_head = q2
        self.main = q1

        env = self.environment("merge_group", head=q2, protected=q1) | {"SOURCE_TOOLS_SHA": self.base}
        result, _ = self.evaluate(env)
        self.assertEqual(result["state"], "pass")
        self.assertEqual(result["overall"], "success")
        self.assertEqual(result["base"], self.base)
        self.assertEqual(result["tools_commit"], self.base)
        self.assertEqual(result["head"], q2)
        self.assertEqual(self.publish(env, result), 0)
        self.assert_statuses(q2)


class QueueMainMoved(unittest.TestCase):
    OLD_MAIN = "1" * 40
    NEW_MAIN = "2" * 40
    HEAD = "3" * 40
    OTHER = "4" * 40

    def _api(self, binding, *, main_sha, queue_sha=HEAD, compare=None, pr=None, files=None):
        class StubGitHub(source.GitHub):
            def __init__(self):
                self.binding = binding
                self.calls = []

            def call(self, path, body=None, *, missing=None):
                self.calls.append(path)
                if path == f"pulls/{PR}":
                    return pr
                if path == f"pulls/{PR}/files?per_page=100" and files is not None:
                    if isinstance(files, Exception):
                        raise files
                    return files
                if path == "git/ref/heads/main":
                    return {"object": {"sha": main_sha}}
                if path == "git/ref/" + QUEUE.removeprefix("refs/"):
                    if queue_sha is None:
                        raise source.CheckUnavailable(missing)
                    return {"object": {"sha": queue_sha}}
                if path.startswith("compare/"):
                    if isinstance(compare, Exception):
                        raise compare
                    return compare
                raise AssertionError(path)

        return StubGitHub()

    def test_merge_group_current_accepts_main_ancestor_ahead_or_identical(self):
        binding = source.Binding(source.REPOSITORY, source.REPOSITORY_ID, None,
                                 self.OLD_MAIN, self.HEAD, self.OLD_MAIN, QUEUE)
        for status in ("ahead", "identical"):
            with self.subTest(status=status):
                api = self._api(binding, main_sha=self.NEW_MAIN, queue_sha=self.HEAD,
                                compare={"status": status})
                self.assertIsNone(api.current())
                self.assertIn(f"compare/{self.NEW_MAIN}...{self.HEAD}", api.calls)

    def test_merge_group_current_rejects_diverged_behind_or_failed_compare(self):
        binding = source.Binding(source.REPOSITORY, source.REPOSITORY_ID, None,
                                 self.OLD_MAIN, self.HEAD, self.OLD_MAIN, QUEUE)
        for compare in ({"status": "diverged"}, {"status": "behind"}, {}, "unexpected",
                        source.CheckUnavailable("current-state-unavailable")):
            with self.subTest(compare=compare):
                api = self._api(binding, main_sha=self.NEW_MAIN, queue_sha=self.HEAD, compare=compare)
                with self.assertRaises(source.CheckUnavailable) as ctx:
                    api.current()
                self.assertEqual(ctx.exception.reason, "main-moved")

    def test_merge_group_current_raises_queue_moved_even_when_main_is_ancestor(self):
        binding = source.Binding(source.REPOSITORY, source.REPOSITORY_ID, None,
                                 self.OLD_MAIN, self.HEAD, self.OLD_MAIN, QUEUE)
        for queue_sha in (self.OTHER, None):
            with self.subTest(queue_sha=queue_sha):
                api = self._api(binding, main_sha=self.NEW_MAIN, queue_sha=queue_sha,
                                compare={"status": "ahead"})
                with self.assertRaises(source.CheckUnavailable) as ctx:
                    api.current()
                self.assertEqual(ctx.exception.reason, "queue-moved")
                self.assertFalse(any(path.startswith("compare/") for path in api.calls))

    def test_non_merge_group_current_strictly_rejects_moved_main(self):
        binding = source.Binding(source.REPOSITORY, source.REPOSITORY_ID, PR,
                                 self.OLD_MAIN, self.HEAD, self.OLD_MAIN)
        api = self._api(binding, main_sha=self.NEW_MAIN, compare={"status": "ahead"},
                        pr=pull(self.OLD_MAIN, self.HEAD))
        with self.assertRaises(source.CheckUnavailable) as ctx:
            api.current()
        self.assertEqual(ctx.exception.reason, "main-moved")
        self.assertFalse(any(path.startswith("compare/") for path in api.calls))

    def _moved(self, **changes):
        compare = {"status": "ahead", "ahead_by": 2, "behind_by": 0,
                   "base_commit": {"sha": self.OLD_MAIN}, "merge_base_commit": {"sha": self.OLD_MAIN},
                   "files": [{"filename": "src/Other.cpp", "status": "modified"},
                             {"filename": "docs/Notes.md", "status": "added"}]}
        return compare | changes

    def _disjoint(self, *, compare, files=None, pr=None, binding=None):
        binding = binding or source.Binding(source.REPOSITORY, source.REPOSITORY_ID, PR,
                                            self.OLD_MAIN, self.HEAD, self.OLD_MAIN)
        files = [{"filename": "src/Size.cpp", "status": "modified"}] if files is None else files
        api = self._api(binding, main_sha=self.NEW_MAIN, compare=compare, files=files,
                        pr=pr or pull(self.OLD_MAIN, self.HEAD))
        return api, api.current(allow_disjoint=True)

    def test_pr_current_accepts_a_main_that_moved_on_other_paths(self):
        api, pr = self._disjoint(compare=self._moved())
        self.assertEqual(pr["head"]["sha"], self.HEAD)
        self.assertIn(f"compare/{self.OLD_MAIN}...{self.NEW_MAIN}", api.calls)

    def test_pr_current_refuses_every_main_move_it_cannot_prove_disjoint(self):
        many = [{"filename": f"src/F{index}.cpp", "status": "modified"} for index in range(100)]
        renamed = {"filename": "src/New.cpp", "previous_filename": "src/Size.cpp", "status": "renamed"}
        cases = {
            "touches the pull request path": dict(compare=self._moved(files=[{"filename": "src/size.CPP"}])),
            "renames the pull request path": dict(compare=self._moved(files=[renamed])),
            "touches the source rules": dict(compare=self._moved(files=[{"filename": "tools/gates/rules.py"}])),
            "touches the rule configuration": dict(compare=self._moved(files=[{"filename": "config/map.json"}])),
            "diverged": dict(compare=self._moved(status="diverged")),
            "behind": dict(compare=self._moved(behind_by=1)),
            "too far ahead": dict(compare=self._moved(ahead_by=source.MAX_MAIN_AHEAD + 1)),
            "different merge base": dict(compare=self._moved(merge_base_commit={"sha": self.OTHER})),
            "no listed main files": dict(compare=self._moved(files=[])),
            "truncated main files": dict(compare=self._moved(files=many * 3)),
            "unnamed main file": dict(compare=self._moved(files=[{"status": "modified"}])),
            "compare unavailable": dict(compare=source.CheckUnavailable("current-state-unavailable")),
            "pull request files unavailable": dict(compare=self._moved(),
                                                   files=source.CheckUnavailable("current-state-unavailable")),
            "too many pull request files": dict(compare=self._moved(), files=many),
            "pull request file count differs": dict(compare=self._moved(),
                                                    pr=pull(self.OLD_MAIN, self.HEAD) | {"changed_files": 2}),
        }
        for name, case in cases.items():
            with self.subTest(name):
                with self.assertRaises(source.CheckUnavailable) as ctx:
                    self._disjoint(**case)
                self.assertEqual(ctx.exception.reason, "main-moved")

    def test_pr_current_still_refuses_a_moved_head_before_looking_at_main(self):
        with self.assertRaises(source.CheckUnavailable) as ctx:
            self._disjoint(compare=self._moved(), pr=pull(self.OLD_MAIN, self.OTHER))
        self.assertEqual(ctx.exception.reason, "pull-request-moved")

    def test_merge_group_never_uses_the_pull_request_tolerance(self):
        binding = source.Binding(source.REPOSITORY, source.REPOSITORY_ID, None,
                                 self.OLD_MAIN, self.HEAD, self.OLD_MAIN, QUEUE)
        api = self._api(binding, main_sha=self.NEW_MAIN, compare={"status": "diverged"}, files=[])
        with self.assertRaises(source.CheckUnavailable) as ctx:
            api.current(allow_disjoint=True)
        self.assertEqual(ctx.exception.reason, "main-moved")
        self.assertNotIn(f"pulls/{PR}/files?per_page=100", api.calls)

    def test_verify_received_and_receive_handle_merge_group_main_ancestor(self):
        with TemporaryDirectory(prefix="bfme-queue-verify-") as tmp:
            root = Path(tmp)
            work = root / "work"
            work.mkdir()
            git(work, "init", "-q", "-b", "main")
            (work / "src").mkdir()
            (work / "src/A.cpp").write_text("int A() { return 1; }\n")
            git(work, "add", "src/A.cpp")
            git(work, "commit", "-q", "-m", "old main")
            old_main = git(work, "rev-parse", "HEAD")

            (work / "src/A.cpp").write_text("int A() { return 2; }\n")
            git(work, "add", "src/A.cpp")
            git(work, "commit", "-q", "-m", "batch 1 merged to main")
            new_main = git(work, "rev-parse", "HEAD")

            (work / "src/A.cpp").write_text("int A() { return 3; }\n")
            git(work, "add", "src/A.cpp")
            git(work, "commit", "-q", "-m", "batch 2 stacked")
            q2 = git(work, "rev-parse", "HEAD")

            # Also create an out-of-band commit diverged from old_main.
            git(work, "checkout", "-q", "-b", "diverged", old_main)
            (work / "src/A.cpp").write_text("int A() { return 99; }\n")
            git(work, "add", "src/A.cpp")
            git(work, "commit", "-q", "-m", "out of band main")
            diverged_main = git(work, "rev-parse", "HEAD")

            remote = root / "remote.git"
            git(root, "clone", "-q", "--bare", str(work), str(remote))
            git(remote, "update-ref", "refs/heads/main", new_main)
            git(remote, "update-ref", QUEUE, q2)
            git(remote, "update-ref", f"refs/pull/{PR}/head", q2)

            class LocalGit(source.Git):
                def __init__(self, repo):
                    super().__init__(repo)
                    self.environment["PATH"] = GIT_ENV["PATH"]
                    self.environment["GIT_CONFIG_GLOBAL"] = os.devnull

                def run(self, *args, extra_env=None, quota=False, timeout=120):
                    return super().run(*args, extra_env=extra_env, quota=False, timeout=timeout)

            # Stacked merge_group receive() + verify_received() passes when main moved to ancestor q1.
            queue_binding = source.Binding(source.REPOSITORY, source.REPOSITORY_ID, None,
                                           old_main, q2, old_main, QUEUE)
            bare_ok = LocalGit(root / "ok.git")
            source.receive(bare_ok, queue_binding, str(remote))
            source.verify_received(bare_ok, queue_binding)
            self.assertEqual(bare_ok.rev(old_main), old_main)

            # Out-of-band diverged main still raises main-moved.
            git(remote, "update-ref", "refs/heads/main", diverged_main)
            bare_div = LocalGit(root / "div.git")
            with self.assertRaises(source.CheckUnavailable) as ctx:
                source.receive(bare_div, queue_binding, str(remote))
            self.assertEqual(ctx.exception.reason, "main-moved")

            # Moved queue ref raises queue-moved even when main is an ancestor.
            git(remote, "update-ref", "refs/heads/main", new_main)
            git(remote, "update-ref", QUEUE, new_main)
            bare_qmoved = LocalGit(root / "qmoved.git")
            with self.assertRaises(source.CheckUnavailable) as ctx:
                source.receive(bare_qmoved, queue_binding, str(remote))
            self.assertEqual(ctx.exception.reason, "queue-moved")

            # Missing binding.base when main moved to ancestor raises queue-history.
            missing_base_binding = source.Binding(source.REPOSITORY, source.REPOSITORY_ID, None,
                                                  "a" * 40, q2, "a" * 40, QUEUE)
            with self.assertRaises(source.CheckUnavailable) as ctx:
                source.verify_received(bare_ok, missing_base_binding)
            self.assertEqual(ctx.exception.reason, "queue-history")

            # A pull request check still judges its own main when main only moved ahead of it; the publish step
            # decides whether the new commits kept clear of the pull request.
            pr_binding = source.Binding(source.REPOSITORY, source.REPOSITORY_ID, PR,
                                        old_main, q2, old_main)
            source.verify_received(bare_ok, pr_binding)

            # A pull request check whose main is not behind the fetched main still raises main-moved.
            for checked in (diverged_main, q2):
                with self.subTest(checked=checked):
                    pr_binding = source.Binding(source.REPOSITORY, source.REPOSITORY_ID, PR,
                                                checked, q2, checked)
                    with self.assertRaises(source.CheckUnavailable) as ctx:
                        source.verify_received(bare_ok, pr_binding)
                    self.assertEqual(ctx.exception.reason, "main-moved")


class Copy(unittest.TestCase):
    def test_void_pointer_guidance_preserves_an_existing_ledger_signature(self):
        _, remedy = contributor_copy("G4", "void pointer increased by 1 across changed files.", "")
        self.assertIn("symbol ledger already records this function with void *", remedy)
        self.assertIn("keep that signature and ask a maintainer", remedy)

    def test_top_refusal_copy_names_the_action_at_the_given_line(self):
        rows = (
            ("G1", "This change adds a source translation unit.", "existing .c or .cpp"),
            ("G4", "void pointer increased by 1 across changed files.", "Update the shared declaration, definition and callers together."),
            ("G6", "4 source files contain real changes beyond Draft include/use updates. The limit is 3.",
             "Move this file&#39;s edits to a separate pull request"),
        )
        for rule, message, phrase in rows:
            with self.subTest(rule=rule):
                item = dict(id=rule, status="fail", findings=[dict(path="src/Size.cpp", line=42,
                            rule=rule, message=message, remedy="Old copy.")])
                report = public_report([item], source.RULES)
                self.assertIn(f"src/Size.cpp:42</code> **{rule}**", report)
                self.assertIn(phrase, report)
                self.assertNotIn("Old copy", report)
                self.assertNotRegex(report.lower(), r"\bgates?\b")

    def test_g1_specific_repairs_cover_restore_mode_and_size(self):
        for problem, phrase in (
            ("This change deletes a permanent header.", "Restore this permanent header"),
            ("The path has a non-source Git file mode.", "regular, non-executable UTF-8"),
            ("The file exceeds 1048576 bytes.", "at most 1048576 bytes"),
        ):
            self.assertIn(phrase, contributor_copy("G1", problem, "Old copy.")[1])

    def test_build_setting_copy_distinguishes_new_removed_and_reordered_lines(self):
        for message, expected in (
            ("A protected compiler directive was added or changed.", "Remove the line if it is new"),
            ("A protected compiler directive was removed (base line).", "Copy the exact line from main"),
            ("Compiler directives changed order.", "Restore their order and exact text from main"),
            ("The STLport compiler setting changed.", "Remove the new // stlport text"),
            ("The STLport compiler setting changed (base line).", "Restore the original // stlport text"),
        ):
            with self.subTest(message=message):
                item = dict(id="G2", status="fail", findings=[dict(path="src/Control.cpp", line=2, rule="G2",
                            message=message, remedy="Old copy.")])
                report = public_report([item], source.RULES)
                self.assertIn("Build settings and annotations", report)
                self.assertIn("src/Control.cpp:2</code> **G2**", report)
                self.assertIn(expected, report)
                self.assertNotIn("Old copy", report)

    def test_remaining_failure_copy_gives_a_concrete_edit(self):
        for rule, problem, baseline_remedy, expected in (
            ("G3", "Added linker pragma.", "Old copy.", "Remove the pragma"),
            ("G3", "Added inline assembly.", "Old copy.", "Write the operation in C or C++"),
            ("G4", "cast call result increased by 1 across changed files.", "Old copy.",
             "callee and its shared declaration"),
            ("G4", "reinterpret cast increased by 2 across changed files.", "Old copy.",
             "where it is stored or returned"),
            ("G5", "Added comment contains a date.", "Explain the evidence or intent.",
             "Explain the evidence or intent."),
            ("G7", "The type Widget already has a definition in include/Widget.h.",
             "Keep one definition and include its shared declaration.", "include its shared declaration"),
            ("G8", "New address-shaped identifier rva00543210 is absent from the base ledger.",
             "Old copy.", "approve a spelling with the address"),
        ):
            with self.subTest(rule=rule, problem=problem):
                self.assertIn(expected, contributor_copy(rule, problem, baseline_remedy)[1])

    def test_adversarial_filename_is_rendered_as_text(self):
        item = dict(id="G1", status="fail", findings=[dict(path="src/<mark>@other\n.cpp",
                    line=1, rule="G1", message="A path finding.", remedy="Restore the file.")])
        report = public_report([item], source.RULES)
        self.assertNotIn("<mark>", report)
        self.assertNotIn("@other", report)
        self.assertIn("&lt;mark&gt;&#64;other", report)


if __name__ == "__main__":
    unittest.main()
