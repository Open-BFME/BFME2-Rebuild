"""Offline tests for the state branch publisher, the parity check, the resolver and the alarms.

Run from the repository root:  python3 -B -m unittest discover -s ci/state/tests -v
No network, no compiler: a local bare repository stands in for GitHub.
"""
import contextlib
import io
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import unittest

HERE = Path(__file__).resolve().parent
ROOT = HERE.parents[2]
sys.path.insert(0, str(HERE.parent))
sys.path.insert(0, str(ROOT / "ci/alarms"))

import alarm  # noqa: E402
import layout  # noqa: E402
import parity  # noqa: E402
import publish  # noqa: E402
import resolve  # noqa: E402
import store  # noqa: E402

CHECKER = "a" * 64
ENV = {"GIT_AUTHOR_NAME": "t", "GIT_AUTHOR_EMAIL": "t@users.noreply.github.com",
       "GIT_COMMITTER_NAME": "t", "GIT_COMMITTER_EMAIL": "t@users.noreply.github.com",
       "GIT_AUTHOR_DATE": "1700000000 +0000", "GIT_COMMITTER_DATE": "1700000000 +0000",
       "GIT_CONFIG_NOSYSTEM": "1", "GIT_CONFIG_GLOBAL": os.devnull}


def git(cwd, *args):
    done = subprocess.run(["git", "-C", str(cwd), *args], capture_output=True, text=True,
                          env=os.environ | ENV, check=True)
    return done.stdout.strip()


def unit(*rows, proven=(), verified=False, error=False):
    return {"rows": {rva: [v, p] for rva, v, p in rows}, "proven": list(proven),
            "verified": verified, "error": error, "gate_sha256": []}


class Repo:
    """A tiny main with two source files that have expected/ records."""

    def __init__(self, folder):
        self.path = Path(folder) / "main"
        self.path.mkdir()
        git(self.path, "init", "-q", "-b", "main")
        for name in ("a", "b"):
            self.write(f"src/{name}.cpp", f"int {name}() {{ return 1; }}\n")
            self.write(f"expected/{name}.cpp.json", "{}\n")
        self.commits = [self.commit("first")]

    def write(self, rel, text):
        target = self.path / rel
        target.parent.mkdir(parents=True, exist_ok=True)
        target.write_text(text)

    def commit(self, message):
        git(self.path, "add", "-A")
        git(self.path, "commit", "-q", "-m", message)
        return git(self.path, "rev-parse", "HEAD")


def snapshot(sha, b_pct=50.0, a_proven=True):
    return {"schema": 1, "sha": sha, "checker": CHECKER, "tus": {
        "src/a.cpp": unit(("0x00401000", "MATCH", 100), proven=["0x00401000"] if a_proven else [], verified=True),
        "src/b.cpp": unit(("0x00402000", "PARTIAL", b_pct), ("0x00402100", "MISSING", 0)),
    }}


class Base(unittest.TestCase):
    def setUp(self):
        self.tmp = tempfile.TemporaryDirectory()
        self.folder = Path(self.tmp.name)
        self.repo = Repo(self.folder)
        self.remote = self.folder / "remote.git"
        git(self.folder, "init", "-q", "--bare", str(self.remote))
        self.lines = []

    def tearDown(self):
        self.tmp.cleanup()

    def save(self, doc, name="state.json"):
        path = self.folder / name
        path.write_text(json.dumps(doc))
        return path

    def publish(self, doc, sha, remote=None, **kw):
        return publish.publish(self.save(doc), sha, self.repo.path, str(remote or self.remote), "",
                               out=self.lines.append, **kw)


class LayoutTests(Base):
    def test_split_join_round_trip_and_same_bytes(self):
        sha = self.repo.commits[0]
        tree = git(self.repo.path, "rev-parse", sha + "^{tree}")
        first = layout.split(snapshot(sha), tree)
        second = layout.split(json.loads(json.dumps(snapshot(sha))), tree)
        self.assertEqual(first, second)
        self.assertEqual(layout.join(first.__getitem__), layout.normalized(snapshot(sha)))
        record = json.loads(first["files/src/a.cpp.json"])
        self.assertEqual(record["functions"], {"0x00401000": ["MATCH", 100, True]})
        self.assertNotIn(sha, first["files/src/a.cpp.json"].decode())

    def test_refusals_say_expected_and_found(self):
        sha = self.repo.commits[0]
        units = ["src/a.cpp", "src/b.cpp"]
        bad = snapshot(sha)
        del bad["tus"]["src/b.cpp"]
        with self.assertRaisesRegex(layout.Refused, r"expected 2 source files .* found 1 \(missing 1"):
            layout.check_snapshot(bad, sha, units)
        with self.assertRaisesRegex(layout.Refused, r"expected main commit"):
            layout.check_snapshot(snapshot("f" * 40), sha, units)
        broken = snapshot(sha)
        broken["tus"]["src/b.cpp"]["proven"] = ["0x00402000"]  # proof of a PARTIAL row
        with self.assertRaisesRegex(layout.Refused, r"expected a valid record for src/b.cpp"):
            layout.check_snapshot(broken, sha, units)


class PublishTests(Base):
    def test_publish_is_incremental_idempotent_and_ordered(self):
        first = self.repo.commits[0]
        commit, changed = self.publish(snapshot(first), first)
        self.assertTrue(commit)
        self.assertEqual(changed, 2)
        self.assertEqual(git(self.remote, "log", "-1", "--format=%s", "state"), "State for main " + first)
        again, _ = self.publish(snapshot(first), first)
        self.assertEqual(again, commit)
        self.assertIn("Already published", self.lines[-1])
        self.repo.write("src/b.cpp", "int b() { return 2; }\n")
        second = self.repo.commit("second")
        commit2, changed2 = self.publish(snapshot(second, b_pct=75.0), second)
        self.assertEqual(changed2, 1)
        blob = lambda c, p: git(self.remote, "rev-parse", f"{c}:{p}")  # noqa: E731
        self.assertEqual(blob(commit, "files/src/a.cpp.json"), blob(commit2, "files/src/a.cpp.json"))
        self.assertNotEqual(blob(commit, "files/src/b.cpp.json"), blob(commit2, "files/src/b.cpp.json"))
        late, _ = self.publish(snapshot(first), first)
        self.assertEqual(late, "")
        self.assertIn("newer than", self.lines[-1])
        self.assertEqual(git(self.remote, "rev-parse", "state"), commit2)

    def test_new_flag_starts_progress_only_once(self):
        sha = self.repo.commits[0]
        path = self.save(snapshot(sha))
        flags = []
        for _ in range(2):
            out = self.folder / "out.txt"
            out.write_text("")
            done = subprocess.run([sys.executable, "-B", str(HERE.parent / "publish.py"), "--snapshot", str(path),
                                   "--main", sha, "--checkout", str(self.repo.path), "--remote", str(self.remote)],
                                  capture_output=True, text=True, env=os.environ | ENV | {"GITHUB_OUTPUT": str(out)})
            self.assertEqual(done.returncode, 0, done.stdout + done.stderr)
            flags.append(re.search(r"^new=(\w+)$", out.read_text(), re.M).group(1))
        self.assertEqual(flags, ["true", "false"])

    def test_same_main_same_commit_bytes(self):
        sha = self.repo.commits[0]
        other = self.folder / "other.git"
        git(self.folder, "init", "-q", "--bare", str(other))
        one, _ = self.publish(snapshot(sha), sha)
        two, _ = self.publish(snapshot(sha), sha, remote=other)
        self.assertEqual(one, two)

    def test_same_main_with_different_verdicts_is_refused(self):
        sha = self.repo.commits[0]
        self.publish(snapshot(sha), sha)
        with self.assertRaisesRegex(layout.Refused, r"expected the published state .* found differences"):
            self.publish(snapshot(sha, b_pct=10.0), sha)

    def test_drill_breaks_the_input_and_is_refused(self):
        sha = self.repo.commits[0]
        with self.assertRaisesRegex(layout.Refused, r"missing 1"):
            self.publish(snapshot(sha), sha, drill=True)
        self.assertIn("DRILL", self.lines[0])
        self.assertEqual(subprocess.run(["git", "-C", str(self.remote), "rev-parse", "-q", "--verify", "state"],
                                        capture_output=True).returncode, 1)


class ParityTests(Base):
    def test_equal_differs_inconclusive(self):
        sha = self.repo.commits[0]
        tree = git(self.repo.path, "rev-parse", sha + "^{tree}")
        self.publish(snapshot(sha), sha)
        out = []
        result, _ = parity.compare(self.save(snapshot(sha), "full.json"), sha, tree, str(self.remote), "", out=out.append)
        self.assertEqual(result, "equal")
        result, lines = parity.compare(self.save(snapshot(sha, b_pct=51.0), "full.json"), sha, tree,
                                       str(self.remote), "", out=out.append)
        self.assertEqual(result, "differs")
        self.assertTrue(any("src/b.cpp: 0x00402000 expected ['PARTIAL', 51.0] found ['PARTIAL', 50.0]" in l
                            for l in lines), lines)
        self.repo.write("src/a.cpp", "int a() { return 3; }\n")
        later = self.repo.commit("later")
        result, _ = parity.compare(self.save(snapshot(later), "full.json"), later, tree, str(self.remote), "",
                                   out=out.append)
        self.assertEqual(result, "inconclusive")
        self.assertEqual(parity.target(str(self.remote), ""), sha)

    def test_sample_rotation_covers_every_file_once(self):
        units = [f"src/f{i:04d}.cpp" for i in range(3253)]
        seen = []
        for day in range(parity.SAMPLE_GROUPS):
            picked = parity.sample(units, (__import__("datetime").date(2001, 1, 1)
                                           + __import__("datetime").timedelta(days=day)).isoformat())
            self.assertTrue(31 <= len(picked) <= 32, len(picked))
            self.assertEqual(picked, sorted(picked))
            seen += picked
        self.assertEqual(sorted(seen), units)

    def test_sample_compare(self):
        sha = self.repo.commits[0]
        self.publish(snapshot(sha), sha)
        shard = {"schema": 1, "main": sha, "checker": CHECKER, "plan": "x", "batch": 0,
                 "units": {"src/b.cpp": snapshot(sha)["tus"]["src/b.cpp"]}}
        path = self.save(shard, "shard.json")
        out = []
        self.assertEqual(parity.compare_sample(path, sha, str(self.remote), "", out=out.append)[0], "equal")
        shard["units"]["src/b.cpp"]["rows"]["0x00402000"] = ["PARTIAL", 49.0]
        path = self.save(shard, "shard.json")
        result, lines = parity.compare_sample(path, sha, str(self.remote), "", out=out.append)
        self.assertEqual(result, "differs")
        self.assertIn("src/b.cpp: 0x00402000 expected ['PARTIAL', 49.0] found ['PARTIAL', 50.0]", lines[0])
        shard["checker"] = "b" * 64
        path = self.save(shard, "shard.json")
        self.assertEqual(parity.compare_sample(path, sha, str(self.remote), "", out=out.append)[0], "inconclusive")


class FakeIssues:
    def __init__(self):
        self.issues, self.comments, self.labels = {}, [], set()

    def call(self, method, path, body=None):
        if method == "GET" and path.startswith("issues?"):
            return [dict(i) for _, i in sorted(self.issues.items())] if "page=1" in path else []
        if method == "GET" and path.startswith("labels/"):
            if path[7:] not in self.labels:
                import urllib.error
                raise urllib.error.HTTPError(path, 404, "nf", {}, None)
            return {}
        if method == "POST" and path == "labels":
            self.labels.add(body["name"])
            return {}
        if method == "POST" and path == "issues":
            number = len(self.issues) + 1
            self.issues[number] = {"number": number, "state": "open", "title": body["title"], "body": body["body"]}
            return self.issues[number]
        if method == "POST" and path.endswith("/comments"):
            self.comments.append((int(path.split("/")[1]), body["body"]))
            return {}
        if method == "PATCH":
            self.issues[int(path.split("/")[1])].update(body)
            return {}
        raise AssertionError((method, path))


class AlarmTests(unittest.TestCase):
    def run_alarm(self, api, *argv):
        with contextlib.redirect_stdout(io.StringIO()) as out:
            self.assertEqual(alarm.main(list(argv), api=api), 0)
        return out.getvalue()

    def test_one_issue_per_kind_open_update_close_reopen(self):
        api = FakeIssues()
        self.assertIn("opened issue #1", self.run_alarm(api, "raise", "state-publish", "--detail", "expected a, found b"))
        self.assertIn("alarm", api.labels)
        self.assertIn("updated", self.run_alarm(api, "raise", "state-publish", "--detail", "expected a, found c"))
        self.assertEqual(len(api.issues), 1)
        self.assertIn("found c", api.issues[1]["body"])
        self.assertIn("opened issue #2", self.run_alarm(api, "raise", "state-parity"))
        self.assertIn("closed issue #1", self.run_alarm(api, "clear", "state-publish"))
        self.assertEqual(api.issues[1]["state"], "closed")
        self.assertIn("nothing open", self.run_alarm(api, "clear", "state-publish"))
        self.assertIn("reopened issue #1", self.run_alarm(api, "set", "state-publish", "--failed", "true"))
        self.assertEqual(api.issues[1]["state"], "open")
        self.assertEqual(len(api.issues), 2)

    def test_no_word_gate_in_issue_text(self):
        for kind in alarm.KINDS:
            text = alarm.body(kind, "x", "") + alarm.KINDS[kind][0]
            self.assertNotRegex(text.lower(), r"\bgate\b")


class FakeRuns:
    def __init__(self, runs, artifacts=()):
        self.runs = runs
        self.artifacts = list(artifacts)

    def get(self, path):
        if "/artifacts?" in path:
            return {"artifacts": self.artifacts}
        return {"workflow_runs": self.runs}


def run(i, status="completed", conclusion="success", sha="1" * 40, created="2001-01-03T10:00:00Z"):
    return {"id": i, "run_attempt": 1, "status": status, "conclusion": conclusion, "head_sha": sha,
            "head_branch": "main", "event": "push", "path": resolve.WORKFLOW, "created_at": created}


class ResolveTests(unittest.TestCase):
    def test_workflow_run_event(self):
        env = {"RUN_SHA": "1" * 40, "RUN_CONCLUSION": "success", "RUN_BRANCH": "main", "RUN_EVENT": "push",
               "RUN_PATH": resolve.WORKFLOW, "RUN_ID": "5", "RUN_ATTEMPT": "2"}
        self.assertEqual(resolve.from_workflow_run(env)["action"], "publish")
        self.assertEqual(resolve.from_workflow_run(env | {"RUN_CONCLUSION": "failure"})["action"], "measure-failed")
        self.assertEqual(resolve.from_workflow_run(env | {"RUN_CONCLUSION": "cancelled"})["action"], "wait")
        self.assertEqual(resolve.from_workflow_run(env | {"RUN_BRANCH": "other"})["action"], "ignore")
        self.assertEqual(resolve.from_workflow_run(env | {"RUN_EVENT": "repository_dispatch"})["action"], "publish")
        self.assertEqual(resolve.from_workflow_run(env | {"RUN_EVENT": "pull_request"})["action"], "ignore")

    def test_schedule_fallback(self):
        main = "1" * 40
        self.assertEqual(resolve.from_main(FakeRuns([run(1)]), main, 60)["run_id"], "1")
        newest_failed = [run(1), run(2, conclusion="failure", created="2001-01-03T11:00:00Z")]
        self.assertEqual(resolve.from_main(FakeRuns(newest_failed), main, 60)["action"], "measure-failed")
        self.assertEqual(resolve.from_main(FakeRuns([run(3, status="in_progress")]), main, 60)["action"], "wait")
        self.assertEqual(resolve.from_main(FakeRuns([]), main, 60)["action"], "wait")
        self.assertEqual(resolve.from_main(FakeRuns([]), main, 4 * 3600)["action"], "stale")
        found = resolve.from_main(FakeRuns([run(1)]), main, 60)
        with_artifact = FakeRuns([], [{"name": "bfme-numeric-main-state-1", "expired": False}])
        self.assertEqual(resolve.require_artifact(with_artifact, found)["action"], "publish")
        skipped = resolve.require_artifact(FakeRuns([], []), found)
        self.assertEqual(skipped["action"], "wait")
        self.assertIn("without artifact", skipped["why"])
        cancelled_then_ok = [run(1), run(2, conclusion="cancelled", created="2001-01-03T11:00:00Z")]
        self.assertEqual(resolve.from_main(FakeRuns(cancelled_then_ok), main, 60)["run_id"], "1")


class ResolveStalenessTests(unittest.TestCase):
    def test_recent_no_artifact_stays_wait(self):
        main = "1" * 40
        found = resolve.from_main(FakeRuns([run(1)]), main, 60)
        self.assertEqual(found["action"], "publish")
        no_artifact = resolve.require_artifact(FakeRuns([], []), found, age_seconds=60)
        self.assertEqual(no_artifact["action"], "wait")
        self.assertIn("without artifact", no_artifact["why"])

    def test_old_no_artifact_raises_stale(self):
        main = "1" * 40
        found = resolve.from_main(FakeRuns([run(1)]), main, 60)
        self.assertEqual(found["action"], "publish")
        stale = resolve.require_artifact(FakeRuns([], []), found, age_seconds=4 * 3600)
        self.assertEqual(stale["action"], "stale")
        self.assertIn("no published measurement", stale["why"])

    def test_old_with_artifact_publishes(self):
        main = "1" * 40
        found = resolve.from_main(FakeRuns([run(1)]), main, 4 * 3600)
        self.assertEqual(found["action"], "publish")
        with_artifact = FakeRuns([], [{"name": "bfme-numeric-main-state-1", "expired": False}])
        result = resolve.require_artifact(with_artifact, found, age_seconds=4 * 3600)
        self.assertEqual(result["action"], "publish")


class Interface(unittest.TestCase):
    def test_measuring_workflow_name_and_artifact(self):
        """The publisher follows the measuring workflow by name and reads its artifact by name."""
        text = (ROOT / ".github/workflows/state.yml").read_text()
        self.assertIn("name: Measure protected main state", text)
        self.assertIn("bfme-numeric-main-state-${{ github.run_attempt }}", text)
        self.assertIn("state.json", text)
        mine = (ROOT / ".github/workflows/state-publish.yml").read_text()
        self.assertIn('workflows: ["Measure protected main state"]', mine)
        self.assertIn("bfme-numeric-main-state-${{ steps.resolve.outputs.attempt }}", mine)


class WriteJobs:
    def assert_main_only(self, text, name):
        """Every job that holds a write permission runs only on refs/heads/main; every checkout can read."""
        jobs = re.split(r"(?m)^  (?=[a-z-]+:\n)", text.split("\njobs:\n", 1)[1])
        for job in filter(None, jobs):
            if re.search(r": write", job):
                self.assertIn("github.ref == 'refs/heads/main'", job, f"{name}: {job.splitlines()[0]}")
            if "actions/checkout" in job:
                self.assertRegex(job, r"contents: (read|write)", f"{name}: {job.splitlines()[0]}")


class WorkflowRules(unittest.TestCase, WriteJobs):
    FILES = [ROOT / ".github/workflows/state-publish.yml", ROOT / ".github/workflows/state-parity.yml"]

    def test_rules(self):
        for path in self.FILES:
            text = path.read_text()
            self.assertRegex(text, r"(?m)^permissions: \{\}$", path.name)
            for line in re.findall(r"uses: (\S+)", text):
                if line.startswith("./.github/workflows/"):
                    continue  # a workflow of this repository, read from the same commit
                self.assertRegex(line, r"@[0-9a-f]{40}$", f"{path.name}: action not pinned: {line}")
            self.assertNotIn("secrets.", text, path.name)
            self.assertNotIn("pull_request", text, path.name)
            self.assert_main_only(text, path.name)
            self.assertNotRegex(text.lower(), r"\bgate\b")
            for block in re.findall(r"persist-credentials: (\S+)", text):
                self.assertEqual(block, "false")

    def test_only_the_measuring_job_uses_the_compiler_image(self):
        """Runner starts on the compiler image are paid; every other job runs on a standard runner."""
        expected = {"state-publish.yml": 0, "state-parity.yml": 1, "progress.yml": 0}
        for name, boat in expected.items():
            text = (ROOT / ".github/workflows" / name).read_text()
            labels = re.findall(r"runs-on: (.+)", text)
            self.assertEqual(sum(label.startswith("boat") for label in labels), boat, f"{name}: {labels}")
            standard = "'${{ vars.BFME_STATE_PUBLISH_RUNNER || ''ubuntu-24.04'' }}'"
            self.assertEqual(sum(label == standard for label in labels), len(labels) - boat, f"{name}: {labels}")

    def test_measuring_workflow_can_call_the_publisher(self):
        text = (ROOT / ".github/workflows/state-publish.yml").read_text()
        for item in ("  workflow_call:\n", "measured_run_id:", "measured_attempt:", "measured_conclusion:",
                     "RUN_ID: '${{ github.event.workflow_run.id || inputs.measured_run_id }}'",
                     "RUN_CONCLUSION: '${{ github.event.workflow_run.conclusion || inputs.measured_conclusion }}'",
                     "EVENT: '${{ inputs.measured_run_id && ''workflow_run'' || github.event_name }}'"):
            self.assertIn(item, text)




class TuMoveMigrationTests(unittest.TestCase):
    def test_pinned_tu_move_carries_untouched_units_and_survives_followup_edit(self):
        from hashlib import sha256
        from unittest import mock
        for extra in (str(ROOT / "ci/managed-state"), str(ROOT / "ci/managed-check")):
            if extra not in sys.path:
                sys.path.insert(0, extra)
        import state_delta

        with tempfile.TemporaryDirectory() as td:
            repo = Path(td)
            def git_cmd(*args):
                subprocess.run(["git", "-C", str(repo), *args], check=True, capture_output=True)
            git_cmd("init", "-b", "main")
            git_cmd("config", "user.name", "Test")
            git_cmd("config", "user.email", "test@example.com")
            for p, content in [
                ("src/keep.cpp", b"int keep() { return 1; }\n"),
                ("expected/keep.cpp.json", b"{}\n"),
                ("src/Code/Tools/old.cpp", b"int moved() { return 2; }\n"),
                ("expected/Code/Tools/old.cpp.json", b"{}\n"),
                ("config/map.json", b'{"tu":"src/Code/Tools/old.cpp"}\n'),
            ]:
                fp = repo / p
                fp.parent.mkdir(parents=True, exist_ok=True)
                fp.write_bytes(content)
            git_cmd("add", ".")
            git_cmd("commit", "-m", "base")
            parent = subprocess.check_output(["git", "-C", str(repo), "rev-parse", "HEAD"], text=True).strip()

            (repo / "src/Code/Tools/old.cpp").unlink()
            (repo / "expected/Code/Tools/old.cpp.json").unlink()
            for p, content in [
                ("src/unk/O001.cpp", b"int moved() { return 2; }\n"),
                ("expected/unk/O001.cpp.json", b"{}\n"),
                ("config/map.json", b'{"tu":"src/unk/O001.cpp"}\n'),
            ]:
                fp = repo / p
                fp.parent.mkdir(parents=True, exist_ok=True)
                fp.write_bytes(content)
            git_cmd("add", "-A")
            git_cmd("commit", "-m", "move")
            moved_sha = subprocess.check_output(["git", "-C", str(repo), "rev-parse", "HEAD"], text=True).strip()

            # Follow-up edit to the relocated TU before state publishes (Sol's P1 scenario)
            (repo / "src/unk/O001.cpp").write_bytes(b"int moved() { return 3; }\n")
            git_cmd("add", "src/unk/O001.cpp")
            git_cmd("commit", "-m", "followup edit to relocated tu")
            followup_sha = subprocess.check_output(["git", "-C", str(repo), "rev-parse", "HEAD"], text=True).strip()

            def bsha(rev, path):
                r = subprocess.run(["git", "-C", str(repo), "show", f"{rev}:{path}"], capture_output=True)
                return sha256(r.stdout).hexdigest() if r.returncode == 0 else None

            mig = {
                "old_tus": frozenset({"src/Code/Tools/old.cpp"}),
                "new_tus": frozenset({"src/unk/O001.cpp"}),
                "files": {
                    p: (bsha(parent, p), bsha(moved_sha, p))
                    for p in ("config/map.json", "src/Code/Tools/old.cpp",
                              "expected/Code/Tools/old.cpp.json", "expected/unk/O001.cpp.json")
                },
            }
            with mock.patch.object(state_delta, "TU_MOVE_MIGRATIONS", (mig,)):
                self.assertEqual(state_delta.source_changes(repo, parent, moved_sha), {"src/unk/o001.cpp"})
                self.assertEqual(state_delta.source_changes(repo, parent, followup_sha), {"src/unk/o001.cpp"})
                bad = dict(mig, files=dict(mig["files"], **{"config/map.json": ("0" * 64, "1" * 64)}))
                with mock.patch.object(state_delta, "TU_MOVE_MIGRATIONS", (bad,)):
                    self.assertIsNone(state_delta.source_changes(repo, parent, moved_sha))



    def test_require_artifact_checks_published_state_age_even_when_main_is_fresh(self):
        class StubApi:
            def __init__(self, state_date):
                self.state_date = state_date
            def get(self, path):
                if path.startswith("actions/runs/"):
                    return {"artifacts": []}
                if path == "commits/state":
                    return {"commit": {"committer": {"date": self.state_date}}}
                raise KeyError(path)
        result = {"action": "publish", "run_id": "99", "attempt": "1", "sha": "a" * 40}
        import datetime as _dt
        now_ts = int(_dt.datetime(2027, 1, 15, 8, 0, 0, tzinfo=_dt.timezone.utc).timestamp())
        out_stale = resolve.require_artifact(StubApi("2027-01-15T00:00:00Z"), result, age_seconds=60, now=now_ts)
        self.assertEqual(out_stale["action"], "stale")
        out_fresh = resolve.require_artifact(StubApi("2027-01-15T07:30:00Z"), result, age_seconds=60, now=now_ts)
        self.assertEqual(out_fresh["action"], "wait")

if __name__ == "__main__":
    unittest.main()
