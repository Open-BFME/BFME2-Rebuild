"""Exact-head start status and disjoint-main tolerance in the protected intake (github_io start, bind and fetch)."""
import json
import os
from pathlib import Path
import subprocess
import sys
import tempfile
import unittest
from unittest.mock import patch

sys.path.insert(0, str(Path(__file__).resolve().parent))

from test_disjoint_ancestor_publish import (  # noqa: E402
    BASE, HEAD, NEW_MAIN, StubGitHub, _compare_doc, _pr_doc, common, github_io, github_status,
)

NUMBER = 560


class TrustedCheckout:
    def __init__(self, path):
        self.path = path

    def rev(self, ref):
        return BASE


class FetchedRefs:
    """The two refs the intake fetch wrote, and whether the bound main is an ancestor of the fetched main."""

    def __init__(self, head, main, ancestor):
        self.refs = {"refs/ci/head": head, "refs/ci/actual-main": main}
        self.ancestor = ancestor
        self.commands = []

    def run(self, *args, check=True, **kwargs):
        self.commands.append(args)
        code = 0 if args[0] == "fetch" or self.ancestor else 1
        return subprocess.CompletedProcess(args, code, b"", b"")

    def rev(self, ref):
        return self.refs[ref]


class IntakeFreshness(unittest.TestCase):
    def setUp(self):
        StubGitHub.posted = []
        self.scratch = tempfile.TemporaryDirectory()
        self.addCleanup(self.scratch.cleanup)
        self.tmp = Path(self.scratch.name)

    def environment(self, event_name="repository_dispatch", *, pr=None):
        event_path = self.tmp / "event.json"
        if event_name == "repository_dispatch":
            payload = {"action": "bfme-remeasure", "client_payload": {"number": NUMBER, "head": HEAD, "base": BASE}}
        else:
            payload = {"action": "synchronize", "pull_request": pr}
        event_path.write_text(json.dumps(payload), encoding="utf-8")
        return {"GITHUB_REPOSITORY": common.REPO, "GITHUB_REPOSITORY_ID": str(common.REPO_ID),
                "GITHUB_REF": "refs/heads/main", "GITHUB_SHA": BASE, "GITHUB_EVENT_NAME": event_name,
                "GITHUB_EVENT_PATH": str(event_path), "GITHUB_RUN_ID": "999001", "GITHUB_RUN_ATTEMPT": "1",
                "GITHUB_ACTOR_ID": "1760627", "BFME_TRIGGERING_ACTOR": "Ancalgonn",
                "BFME_AUTO_ADMISSION": "yes", "BFME_PR_COMPACT": "yes",
                "GITHUB_OUTPUT": str(self.tmp / "output"), "BFME_STATUS_TOKEN": "ghp_" + "x" * 36}

    def responses(self, *, actual_main, pr_files=None, compare_doc=None, pr=None):
        StubGitHub.responses = {
            f"pulls/{NUMBER}": pr or _pr_doc(head_repo_id=common.REPO_ID, association="OWNER"),
            "git/ref/heads/main": {"object": {"sha": actual_main}},
            f"pulls/{NUMBER}/files?per_page=100&page=1": (
                pr_files if pr_files is not None else [{"filename": "src/unk/O00123456.cpp", "status": "modified"}]),
            f"compare/{BASE}...{actual_main}": compare_doc if compare_doc is not None else _compare_doc(),
        }

    def start(self, pr=None):
        # The owner's own pull request event is the one that starts an exact-head status under automatic admission.
        pr = pr or StubGitHub.responses[f"pulls/{NUMBER}"]
        with patch.dict(os.environ, self.environment("pull_request_target", pr=pr), clear=False), \
                patch.object(github_io, "GitHub", StubGitHub):
            github_io.publish("start")
        return list(StubGitHub.posted)

    def bind(self):
        work = self.tmp / "receive"
        with patch.dict(os.environ, self.environment(), clear=False), \
                patch.object(github_io, "GitHub", StubGitHub), patch.object(github_io, "Git", TrustedCheckout):
            github_io.bind(work)
        return json.loads((work / "event.json").read_text(encoding="utf-8"))

    def test_start_writes_only_the_boat_pending_status(self):
        self.responses(actual_main=BASE)
        posted = self.start()
        self.assertEqual([(path, row["context"], row["state"]) for path, row in posted],
                         [(f"statuses/{HEAD}", common.CONTEXT, "pending")])

    def test_start_writes_nothing_for_an_automatic_fork_event(self):
        pr = _pr_doc()
        self.responses(actual_main=BASE, pr=pr)
        self.assertEqual(self.start(pr), [])

    def test_start_and_bind_accept_a_main_that_moved_only_on_disjoint_files(self):
        self.responses(actual_main=NEW_MAIN)
        self.assertEqual(len(self.start()), 1)
        binding = self.bind()
        self.assertEqual((binding["base"], binding["head"], binding["number"]), (BASE, HEAD, NUMBER))

    def test_bind_refuses_a_main_that_touched_the_change_or_a_shared_input(self):
        for name, files in (("same file", [{"filename": "src/unk/o00123456.CPP", "status": "modified"}]),
                            ("header", [{"filename": "include/GameLogic/Object.h", "status": "modified"}]),
                            ("tools", [{"filename": "tools/gates/rules.py", "status": "modified"}])):
            with self.subTest(name):
                self.responses(actual_main=NEW_MAIN, compare_doc=_compare_doc(files=files))
                with self.assertRaises(github_status.Superseded):
                    self.bind()
                with self.assertRaises(github_status.Superseded):
                    self.start()
        self.assertEqual(StubGitHub.posted, [])

    def test_bind_still_refuses_a_moved_head_or_a_closed_pull_request(self):
        for pr in (_pr_doc(head="9" * 40, head_repo_id=common.REPO_ID, association="OWNER"),
                   _pr_doc(state="closed", head_repo_id=common.REPO_ID, association="OWNER")):
            with self.subTest(pr=pr["state"]):
                self.responses(actual_main=BASE, pr=pr)
                with self.assertRaises(github_status.Superseded):
                    self.bind()

    def test_fetch_accepts_a_fetched_main_ahead_of_the_bound_main(self):
        binding = {"number": NUMBER, "base": BASE, "head": HEAD}
        for main, ancestor in ((BASE, False), (NEW_MAIN, True)):
            with self.subTest(main=main):
                github_io.receive_refs(FetchedRefs(HEAD, main, ancestor), "https://example.invalid/r.git", binding)
        for head, main, ancestor in ((HEAD, NEW_MAIN, False), ("9" * 40, BASE, True)):
            with self.subTest(head=head, main=main), self.assertRaises(github_status.Superseded):
                github_io.receive_refs(FetchedRefs(head, main, ancestor), "https://example.invalid/r.git", binding)


if __name__ == "__main__":
    unittest.main()
