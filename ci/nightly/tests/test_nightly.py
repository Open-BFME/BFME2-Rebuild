"""Offline tests of the nightly package: python3 -B -m unittest discover -s ci/nightly/tests"""
from datetime import datetime, timedelta, timezone
import json
import os
from pathlib import Path
import re
import subprocess
import sys
import tempfile
import unittest

HERE = Path(__file__).resolve().parent
NIGHTLY = HERE.parent
REPO = NIGHTLY.parent.parent
sys.path.insert(0, str(NIGHTLY))

import alarms  # noqa: E402
import checks  # noqa: E402
import plan_shards  # noqa: E402
import result  # noqa: E402

SHA = "a" * 40
CLEAN = {"rc": 0}


def record(frames, bad=()):
    return {"crcs": {str(f): (f * 7 + (1 if f in bad else 0)) for f in frames}}


class Shards(unittest.TestCase):
    def test_assignment_is_complete_disjoint_and_deterministic(self):
        sizes = {f"src/f{i}.cpp": (i * 37) % 101 + 1 for i in range(300)}
        a = plan_shards.assign(sizes, 8)
        self.assertEqual(a, plan_shards.assign(dict(reversed(list(sizes.items()))), 8))
        flat = [p for g in a for p in g]
        self.assertEqual(sorted(flat), sorted(sizes))
        self.assertEqual(len(flat), len(set(flat)))
        loads = [sum(sizes[p] for p in g) for g in a]
        self.assertLessEqual(max(loads) - min(loads), max(sizes.values()))

    def test_source_files_reads_ls_tree(self):
        class G:
            def value(self, *a):
                return ("100644 blob 1111 120\tsrc/a.cpp\n100644 blob 2222 30\tsrc/b.c\n"
                        "100644 blob 3333 9\tsrc/c.h\n100644 blob 4444 9\tsrc/d.CPP")
        self.assertEqual(plan_shards.source_files(G(), SHA), {"src/a.cpp": 120, "src/b.c": 30})

    def test_zero_shards_refused(self):
        with self.assertRaises(ValueError):
            plan_shards.assign({"src/a.cpp": 1}, 0)


class Compare(unittest.TestCase):
    frames = range(25, 1001, 25)

    def test_equal_run_passes(self):
        f = checks.crc_compare(record(self.frames), record(self.frames), 1000)
        self.assertEqual((f["frames_compared"], f["frames_equal"], f["first_divergent_frame"]), (40, 40, None))
        v = checks.judge_test(checks.TESTS[0], f, 0, CLEAN)
        self.assertEqual(v["status"], "pass")

    def test_differing_frame_is_reported(self):
        f = checks.crc_compare(record(self.frames), record(self.frames, bad={300, 400}), 1000)
        v = checks.judge_test(checks.TESTS[0], f, 0, CLEAN)
        self.assertEqual((v["status"], v["first_divergent_frame"]), ("differs", 300))
        self.assertIn("first differing frame 300", v["detail"])

    def test_short_run_is_a_crash(self):
        f = checks.crc_compare(record(self.frames), record(range(25, 501, 25)), 1000)
        v = checks.judge_test(checks.TESTS[0], f, 1, CLEAN)
        self.assertEqual(v["status"], "crash")
        self.assertIn("expected 40 compared frames; found 20", v["detail"])

    def test_no_record_is_a_crash(self):
        self.assertEqual(checks.judge_test(checks.TESTS[0], {}, 1, CLEAN)["status"], "crash")

    def test_control_caught_at_its_frame_passes(self):
        ctl = next(t for t in checks.TESTS if t["name"] == "control")
        f = checks.crc_compare(record(self.frames), record(self.frames, bad=set(range(500, 1001, 25))), 1000)
        self.assertEqual(checks.judge_test(ctl, f, 0, CLEAN)["status"], "pass")

    def test_control_not_caught_fails(self):
        ctl = next(t for t in checks.TESTS if t["name"] == "control")
        f = checks.crc_compare(record(self.frames), record(self.frames), 1000)
        self.assertEqual(checks.judge_test(ctl, f, 0, CLEAN)["status"], "control-blind")
        f = checks.crc_compare(record(self.frames), record(self.frames, bad={250}), 1000)
        self.assertEqual(checks.judge_test(ctl, f, 0, CLEAN)["status"], "control-blind")

    def test_until_limits_the_control(self):
        f = checks.crc_compare(record(range(25, 5001, 25)), record(range(25, 1001, 25)), 1000)
        self.assertEqual((f["frames_compared"], f["control_frames"]), (40, 40))

    def test_list_form_records(self):
        f = checks.crc_compare({"crcs": [[25, 1], [50, 2]]}, {"crcs": [["25", 1], ["50", 3]]})
        self.assertEqual(f["first_divergent_frame"], 50)

    def test_reload(self):
        save = next(t for t in checks.TESTS if t["name"] == "save")
        ctl = {"crcs": {str(f): f for f in range(625, 1126, 25)}, "crcs_before_load": {str(f): f for f in range(25, 176, 25)}}
        ok = checks.reload_compare(ctl, dict(ctl, outcome="pass"))
        self.assertEqual(checks.judge_test(save, ok, 0, CLEAN)["status"], "pass")
        bad = checks.reload_compare(ctl, dict(ctl, outcome="pass", crcs=dict(ctl["crcs"], **{"650": 9})))
        self.assertEqual(checks.judge_test(save, bad, 0, CLEAN)["status"], "differs")
        crash = checks.reload_compare(ctl, {"outcome": "crash", "crcs_before_load": ctl["crcs_before_load"]})
        self.assertEqual(checks.judge_test(save, crash, 0, CLEAN)["status"], "crash")
        self.assertEqual(checks.judge_test(save, {}, 0, CLEAN)["status"], "crash")


def write_shard(root, i, n, files, sha=SHA, matching=()):
    d = Path(root) / f"nightly-shard-{i}"
    d.mkdir(parents=True)
    with open(d / "judge.jsonl", "w") as f:
        for tu in files:
            f.write(json.dumps({"tu": tu, "functions": 2, "function_match": 2 if tu in matching else 1}) + "\n")
    (d / "shard.json").write_text(json.dumps({"sha": sha, "shard": i, "shards": n, "files": len(files), "compiled": len(files),
                                              "compile_errors": 0, "fully_matching": len(set(files) & set(matching)),
                                              "seconds": 10.0 + i}))


class Merge(unittest.TestCase):
    def test_complete_set_merges(self):
        with tempfile.TemporaryDirectory() as d:
            write_shard(d, 0, 2, ["src/a.cpp"], matching=["src/a.cpp"])
            write_shard(d, 1, 2, ["src/b.cpp"])
            res, info = checks.merge_shards(d, SHA, ["src/a.cpp", "src/b.cpp"])
            self.assertEqual([r["tu"] for r in res], ["src/a.cpp", "src/b.cpp"])
            self.assertEqual((info["files"], info["fully_matching"], info["slowest_shard_s"]), (2, 1, 11.0))

    def test_missing_shard_refused(self):
        with tempfile.TemporaryDirectory() as d:
            write_shard(d, 0, 2, ["src/a.cpp"])
            with self.assertRaisesRegex(ValueError, "every compile shard"):
                checks.merge_shards(d, SHA, ["src/a.cpp"])

    def test_foreign_commit_refused(self):
        with tempfile.TemporaryDirectory() as d:
            write_shard(d, 0, 1, ["src/a.cpp"], sha="b" * 40)
            with self.assertRaisesRegex(ValueError, "compile shards of"):
                checks.merge_shards(d, SHA, ["src/a.cpp"])

    def test_missing_file_refused(self):
        with tempfile.TemporaryDirectory() as d:
            write_shard(d, 0, 1, ["src/a.cpp"])
            with self.assertRaisesRegex(ValueError, "missing 1"):
                checks.merge_shards(d, SHA, ["src/a.cpp", "src/b.cpp"])

    def test_duplicate_refused(self):
        with tempfile.TemporaryDirectory() as d:
            write_shard(d, 0, 2, ["src/a.cpp"])
            write_shard(d, 1, 2, ["src/a.cpp"])
            with self.assertRaisesRegex(ValueError, "twice"):
                checks.merge_shards(d, SHA, ["src/a.cpp"])


def stage2(statuses=None, stopped=None, image=True, blocking=0):
    statuses = statuses or {}
    tests = {}
    for t in checks.TESTS:
        st = statuses.get(t["name"], "pass")
        tests[t["name"]] = {"name": t["name"], "status": st, "detail": f"{t['name']} {st}",
                            "frames_compared": 40, "frames_equal": 40 if st == "pass" else 10,
                            "first_divergent_frame": 300 if st == "differs" else None}
    return {"sha": SHA, "stopped": stopped, "tests": tests, "timings": {"stage_s": 1},
            "plan": {"files": 323, "functions": 5600}, "link": {"image": image, "unresolved": 0, "duplicates": 0},
            "size_check": {"blocking": blocking}, "image_sha256": "c" * 64}


def meta(compile_job="success", game_job="success", live=True):
    return {"sha": SHA, "compile_job": compile_job, "game_job": game_job, "game_live": live, "machine": None}


CINFO = {"files": 2, "compiled": 2, "compile_errors": 0, "fully_matching": 1, "shards": 8, "expected_shards": 8,
         "slowest_shard_s": 3, "problem": None}


class Result(unittest.TestCase):
    def test_clean_night(self):
        n = result.build(CINFO, stage2(), meta())
        self.assertEqual(n["result"], "pass")
        self.assertEqual((n["files_substituted"], n["functions"]), (323, 5600))
        self.assertEqual(n["checks"], {"starts": "pass", "menu": "pass", "maps": "pass", "save_load": "pass", "control": "pass"})
        a = result.alarms(n)
        self.assertEqual({k: v["failed"] for k, v in a.items()},
                         {"nightly-build": False, "nightly-crash": False, "nightly-differs": False, "nightly-control": False})
        self.assertIn("**PASS**", result.summary(n))
        self.assertEqual({k: n[k] for k in ("starts", "menu", "skirmish", "save_load", "simulation_equal", "control_caught")},
                         dict.fromkeys(("starts", "menu", "skirmish", "save_load", "simulation_equal", "control_caught"), True))
        self.assertEqual(n["equal_frames"], 160)

    def test_game_stage_not_live_is_partial_and_touches_only_build_alarm(self):
        n = result.build(CINFO, None, meta(game_job="skipped", live=False))
        self.assertEqual((n["result"], n["stages"]["link_and_play"]), ("partial", "not live"))
        self.assertEqual(set(result.alarms(n)), {"nightly-build"})
        self.assertIn("not live yet", result.summary(n))
        self.assertEqual((n["starts"], n["simulation_equal"], n["control_caught"], n["equal_frames"]), (None, None, None, None))

    def test_compile_failure(self):
        n = result.build(CINFO, None, meta(compile_job="failure", game_job="skipped"))
        self.assertEqual(n["result"], "fail")
        a = result.alarms(n)
        self.assertTrue(a["nightly-build"]["failed"])
        self.assertIn("expected the compile stage to succeed; found failure", a["nightly-build"]["detail"])

    def test_link_failure(self):
        n = result.build(CINFO, dict(stage2(), tests={}, stopped="expected a linked image; found none", image=False,
                                     link={"image": False}), meta(game_job="failure"))
        a = result.alarms(n)
        self.assertEqual(n["result"], "fail")
        self.assertTrue(a["nightly-build"]["failed"])
        self.assertNotIn("nightly-crash", a)

    def test_game_job_lost_without_result(self):
        n = result.build(CINFO, None, meta(game_job="failure"))
        self.assertEqual(n["result"], "fail")
        self.assertTrue(result.alarms(n)["nightly-build"]["failed"])

    def test_differs_and_crash(self):
        n = result.build(CINFO, stage2({"fords": "differs", "save": "crash"}), meta())
        a = result.alarms(n)
        self.assertEqual(n["result"], "fail")
        self.assertTrue(a["nightly-differs"]["failed"] and a["nightly-crash"]["failed"])
        self.assertFalse(a["nightly-control"]["failed"])
        self.assertEqual(n["frames"]["first_divergent"], {"fords": 300})
        self.assertEqual(n["checks"]["maps"], "fail")
        self.assertEqual((n["simulation_equal"], n["skirmish"], n["save_load"]), (False, False, False))

    def test_blind_control(self):
        n = result.build(CINFO, stage2({"control": "control-blind"}), meta())
        self.assertEqual(n["result"], "fail")
        self.assertTrue(result.alarms(n)["nightly-control"]["failed"])

    def test_size_check_blocks(self):
        n = result.build(CINFO, stage2(blocking=2), meta())
        self.assertEqual(n["result"], "fail")
        self.assertTrue(result.alarms(n)["nightly-build"]["failed"])

    def test_machine_time(self):
        def at(minutes):
            return (datetime(2026, 1, 1) + timedelta(minutes=minutes)).strftime(result.STAMP)
        jobs = [{"name": "Compile shard 0", "started_at": at(0), "completed_at": at(20), "labels": ["private-label"]},
                {"name": "Compile shard 1", "started_at": at(1), "completed_at": at(11), "labels": ["private-label"]},
                {"name": "Link, check and play the image", "started_at": at(21), "completed_at": at(81),
                 "labels": ["private-label"]}]
        m = result.machine_time(jobs)
        self.assertEqual(m["wall_minutes"], 81.0)
        self.assertEqual(m["runner_minutes"], {"compile": 30.0, "link_and_play": 60.0})
        self.assertNotIn("private-label", json.dumps(m))

    def test_output_is_deterministic(self):
        a = json.dumps(result.build(CINFO, stage2(), meta()), sort_keys=True)
        b = json.dumps(result.build(CINFO, stage2(), meta()), sort_keys=True)
        self.assertEqual(a, b)


class Summaries(unittest.TestCase):
    def make(self, d, specs):
        for k, (sha, idx, n) in enumerate(specs):
            p = Path(d) / f"nightly-shard-summary-{k}-a1"
            p.mkdir()
            (p / "shard.json").write_text(json.dumps({"sha": sha, "shard": idx, "shards": n, "files": 1, "compiled": 1,
                                                      "compile_errors": 0, "fully_matching": 0, "seconds": 2.0}))

    def night(self, d, compile_job="success"):
        info = result.compile_summary(d, SHA, 2)
        n = result.build(info, None, meta(compile_job=compile_job, game_job="skipped", live=False))
        return info, n, result.alarms(n)

    def test_complete(self):
        with tempfile.TemporaryDirectory() as d:
            self.make(d, [(SHA, 0, 2), (SHA, 1, 2)])
            info, n, a = self.night(d)
            self.assertIsNone(info["problem"])
            self.assertEqual((n["result"], a["nightly-build"]["failed"]), ("partial", False))

    def test_wrong_sha_opens_build_alarm_even_when_jobs_succeed(self):
        with tempfile.TemporaryDirectory() as d:
            self.make(d, [(SHA, 0, 2), ("b" * 40, 1, 2)])
            info, n, a = self.night(d)
            self.assertIn("expected compile shards of main", info["problem"])
            self.assertEqual(n["result"], "fail")
            self.assertTrue(a["nightly-build"]["failed"])
            self.assertEqual(a["nightly-build"]["detail"], info["problem"])

    def test_duplicate_index(self):
        with tempfile.TemporaryDirectory() as d:
            self.make(d, [(SHA, 0, 2), (SHA, 0, 2)])
            info, n, a = self.night(d)
            self.assertEqual(info["problem"], "expected shard indices 0 to 1 once each; found [0, 0]")
            self.assertTrue(a["nightly-build"]["failed"])

    def test_foreign_plan(self):
        with tempfile.TemporaryDirectory() as d:
            self.make(d, [(SHA, 0, 3), (SHA, 1, 3), (SHA, 2, 3)])
            info, _, a = self.night(d)
            self.assertIn("2-shard plan", info["problem"])
            self.assertTrue(a["nightly-build"]["failed"])

    def test_missing_artifacts_open_build_alarm(self):
        with tempfile.TemporaryDirectory() as d:
            self.make(d, [(SHA, 0, 2)])
            info, n, a = self.night(d)
            self.assertEqual(info["problem"], "expected shard indices 0 to 1 once each; found [0]")
            self.assertTrue(a["nightly-build"]["failed"])
        with tempfile.TemporaryDirectory() as d:
            info, n, a = self.night(d)
            self.assertIn("found none", info["problem"])
            self.assertTrue(a["nightly-build"]["failed"])


class Alarms(unittest.TestCase):
    def test_commands(self):
        with tempfile.TemporaryDirectory() as d:
            cmds = alarms.commands({"nightly-build": {"failed": True, "detail": "x"},
                                    "nightly-differs": {"failed": False, "detail": "y"}}, "https://run", d)
            self.assertEqual([(c[4], c[6]) for c in cmds], [("nightly-build", "true"), ("nightly-differs", "false")])
            self.assertEqual(Path(cmds[0][8]).read_text(), "x\n")

    def test_unknown_kind_refused(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d) / "a.json"
            p.write_text(json.dumps({"nightly-other": {"failed": True, "detail": ""}}))
            self.assertEqual(alarms.main(str(p), ""), 1)


class Publish(unittest.TestCase):
    def test_publish_to_a_local_branch(self):
        sys.path.insert(0, str(NIGHTLY))
        import publish_branch
        with tempfile.TemporaryDirectory() as d:
            remote = Path(d) / "remote.git"
            subprocess.run(["git", "init", "-q", "--bare", str(remote)], check=True)
            res = Path(d) / "res"
            res.mkdir()
            n = result.build(CINFO, stage2(), meta())
            (res / "nightly.json").write_text(json.dumps(n))
            (res / "summary.md").write_text(result.summary(n))
            env = dict(os.environ, NIGHTLY_PUSH_TOKEN="t", GITHUB_REPOSITORY="o/r", NIGHTLY_PUSH_URL=str(remote))
            for _ in range(2):
                subprocess.run([sys.executable, "-B", str(NIGHTLY / "publish_branch.py"), str(res)], env=env, check=True,
                               capture_output=True)
            log = subprocess.run(["git", "--git-dir", str(remote), "log", "--format=%s", "nightly"],
                                 capture_output=True, text=True, check=True).stdout.split("\n")
            self.assertEqual(log[0], f"Nightly pass for main {SHA}")
            self.assertEqual(len([x for x in log if x]), 2)
            files = subprocess.run(["git", "--git-dir", str(remote), "ls-tree", "-r", "--name-only", "nightly"],
                                   capture_output=True, text=True, check=True).stdout.split()
            self.assertIn("latest.json", files)
            self.assertTrue(any(f.startswith("history/") for f in files))


class Package(unittest.TestCase):
    CHECK = REPO / "ci/public-text/check.py"

    def test_no_internal_text(self):
        if not self.CHECK.exists():
            self.skipTest("ci/public-text/check.py not beside the package")
        paths = [str(NIGHTLY)] + [str(p) for p in (REPO / ".github/workflows/nightly.yml", self.CHECK) if p.exists()]
        r = subprocess.run([sys.executable, "-B", str(self.CHECK), *paths], cwd=REPO, capture_output=True, text=True)
        self.assertEqual(r.returncode, 0, r.stdout)

    def test_check_finds_planted_text(self):
        if not self.CHECK.exists():
            self.skipTest("ci/public-text/check.py not beside the package")
        planted = ["see ~/" + "pi" + "lot/link", "ask n" + "b1 at 16:" + "06Z", "mail " + "someone" + "@" + "example.org",
                   "the co" + "ordinator said", "run on s" + "rv1"]
        with tempfile.TemporaryDirectory() as d:
            for i, text in enumerate(planted):
                Path(d, f"f{i}.md").write_text(text + "\n")
            stamp = datetime(2026, 1, 1, 3, 17).strftime(result.STAMP)
            Path(d, "ok.md").write_text(f"stamp {stamp} and noreply 1+x@users.noreply.github.com\n")
            r = subprocess.run([sys.executable, "-B", str(self.CHECK), d], cwd=REPO, capture_output=True, text=True)
        self.assertEqual(r.returncode, 1)
        named = sorted(set(re.findall(r"(f\d+\.md|ok\.md):\d+: ", r.stdout)))
        self.assertEqual(named, [f"f{i}.md" for i in range(len(planted))])

    def test_workflow_rules(self):
        wf = (REPO / ".github/workflows/nightly.yml")
        if not wf.exists():
            self.skipTest("workflow not beside the package")
        text = wf.read_text()
        self.assertIn("\npermissions: {}\n", text)
        for line in text.splitlines():
            if "uses:" in line:
                self.assertRegex(line, r"uses: [\w-]+/[\w-]+@[0-9a-f]{40}$")
        # the public workflow names no machine image that holds game files, and only the PR runner var is allowed
        for line in text.splitlines():
            if "runs-on:" in line:
                self.assertIn(line.strip(), (
                    "runs-on: '${{ vars.BFME_PR_RUNNER || ''boat-small/from=bfme2-compiler-v1'' }}'",
                    "runs-on: boat-small/from=bfme2-compiler-v1",
                    "runs-on: ubuntu-24.04",
                ))
        self.assertNotIn("bfme2-nightly", text)
        self.assertNotIn("bfme2-game", text)
        compile_job = text.split("\n  compile:")[1].split("\n  publish:")[0]
        self.assertNotIn("github.token", compile_job)
        self.assertNotIn("contents: write", compile_job)
        self.assertIn("timeout-minutes: 40", compile_job)
        shards = re.search(r"shard: \[([\d, ]+)\]", compile_job).group(1)
        self.assertEqual([int(x) for x in shards.split(",")], list(range(int(re.search(r"SHARDS: '(\d+)'", text).group(1)))))

    def test_manifest_matches(self):
        m = NIGHTLY / "files.sha256"
        if not m.exists():
            self.skipTest("manifest written at packaging")
        r = subprocess.run(["sha256sum", "-c", "--quiet", "files.sha256"], cwd=NIGHTLY, capture_output=True, text=True)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)

    def test_vendored_planner_imports(self):
        with tempfile.TemporaryDirectory() as d:
            env = dict(os.environ, NIGHTLY_WORK=d, NIGHTLY_GIT_DIR=d, NIGHTLY_BASELINE=d, NIGHTLY_VC_ROOT=d)
            code = ("import sys; sys.path.insert(0, sys.argv[1]); import hybrid, resolve, coffsym, localize2, padobj, "
                    "libguard; print(hybrid.kind_of('??_7Foo@@6B@'), hybrid.kind_of('?f@X@@QAEHXZ'))")
            r = subprocess.run([sys.executable, "-B", "-c", code, str(NIGHTLY / "planner")], env=env,
                               capture_output=True, text=True)
            self.assertEqual(r.stdout.strip(), "vtable function", r.stderr)

    def test_provenance_lists_every_vendored_file(self):
        prov = json.loads((NIGHTLY / "planner/PROVENANCE.json").read_text())
        names = sorted(p.name for p in (NIGHTLY / "planner").glob("*.py"))
        self.assertEqual(sorted(prov), names)
        import hashlib
        for name, v in prov.items():
            self.assertEqual(hashlib.sha256((NIGHTLY / "planner" / name).read_bytes()).hexdigest(), v["output_sha256"])


class Scrub(unittest.TestCase):
    def test_paths_and_flagged_text(self):
        import scrub
        out = scrub.scrub({"stopped": "expected X; found OSError: /opt/bfme2-nightly/ref/index.pkl missing",
                           "files": ["src/Code/GameEngine/A.cpp"], "n": 3,
                           "detail": "see " + "~/" + "pi" + "lot/link/x and ask " + "n" + "b1"})
        self.assertEqual(out["n"], 3)
        self.assertEqual(out["files"], ["src/Code/GameEngine/A.cpp"])
        self.assertIn("index.pkl missing", out["stopped"])
        self.assertNotIn("/opt/", out["stopped"])
        self.assertNotIn("pi" + "lot", out["detail"])
        self.assertIn("[removed]", out["detail"])


class External(unittest.TestCase):
    def test_compile_night_when_game_runs_elsewhere(self):
        m = dict(meta(game_job="skipped", live=False), game_external=True)
        n = result.build(CINFO, None, m)
        self.assertEqual((n["result"], n["stages"]["link_and_play"]), ("partial", "runs separately"))
        self.assertTrue(n["headline"].endswith("game run follows separately"))
        self.assertEqual(result.alarms(n), {})
        self.assertIn("runs later in the private game run", result.summary(n))

    def test_compile_failure_still_raises(self):
        m = dict(meta(compile_job="failure", game_job="skipped", live=False), game_external=True)
        a = result.alarms(result.build(CINFO, None, m))
        self.assertTrue(a["nightly-build"]["failed"])

    def test_headlines(self):
        self.assertTrue(result.build(CINFO, None, meta(game_job="skipped", live=False))["headline"]
                        .endswith("game run not live yet"))
        self.assertTrue(result.build(CINFO, stage2(), meta())["headline"].startswith("plays identically"))
        self.assertTrue(result.build(CINFO, stage2({"fords": "differs"}), meta())["headline"]
                        .startswith("game run failed: fords differs"))


class Machines(unittest.TestCase):
    def test_per_shard_facts(self):
        with tempfile.TemporaryDirectory() as d:
            for i, (sec, steal) in enumerate(((600.0, 0.5), (1500.0, 48.0))):
                p = Path(d) / f"nightly-shard-summary-{i}-a1"
                p.mkdir()
                (p / "shard.json").write_text(json.dumps({"sha": SHA, "shard": i, "shards": 2, "files": 1, "compiled": 1,
                                                          "compile_errors": 0, "fully_matching": 0, "seconds": sec,
                                                          "batch_median_s": sec / 25,
                                                          "host": {"cpus": 2, "steal_pct": steal}}))
            info = result.compile_summary(d, SHA, 2)
        self.assertEqual((info["fastest_shard_s"], info["slowest_shard_s"]), (600.0, 1500.0))
        self.assertEqual([s["steal_pct"] for s in info["per_shard"]], [0.5, 48.0])
        n = result.build(info, None, meta(game_job="skipped", live=False))
        self.assertIn("Compile shards took 10.0 to 25.0 min", result.summary(n))

    def test_cpu_times_shape(self):
        import compile_main
        t = compile_main.cpu_times()
        if t is None:
            self.skipTest("no /proc/stat here")
        facts = compile_main.host_facts(t, (t[0] + 50, t[1] + 50, t[2] + 200))
        self.assertEqual((facts["busy_pct"], facts["steal_pct"]), (25.0, 25.0))


class LinkOnly(unittest.TestCase):
    def test_skip_game_is_partial_and_leaves_game_alarms(self):
        s2 = dict(stage2(), tests={}, games_skipped=True)
        n = result.build(CINFO, s2, meta())
        self.assertEqual(n["result"], "partial")
        self.assertTrue(n["headline"].startswith("links and checks: 323 rebuilt files"))
        self.assertEqual(result.alarms(n), {"nightly-build": {"failed": False, "detail": "built and checked"}})


class FoldIn(unittest.TestCase):
    def setUp(self):
        import fold_in
        from datetime import timezone as tz
        self.f = fold_in
        self.now = datetime(2026, 1, 2, 7, 17).replace(tzinfo=tz.utc)
        self.meta = {"sha": SHA, "public_run_id": 44, "public_run_attempt": 1, "game_job": "success",
                     "workflow": ".github/workflows/nightly.yml",
                     "finished_at": datetime(2026, 1, 2, 6, 30).strftime(result.STAMP)}
        self.values = {"sha": SHA, "run_id": 44, "attempt": 1, "game_job": "success"}
        self.run = {"path": ".github/workflows/nightly.yml", "head_sha": SHA, "head_branch": "main",
                    "run_attempt": 1, "conclusion": "success"}
        self.jobs = [{"name": f"Compile shard {i}", "conclusion": "success"} for i in range(3)]

    def test_fresh(self):
        ok, why, out = self.f.decide(self.meta, {"sha": SHA}, self.now)
        self.assertTrue(ok, why)
        self.assertEqual(out, self.values)

    def test_refused_results(self):
        old = dict(self.meta, finished_at=datetime(2026, 1, 1, 6, 30).strftime(result.STAMP))
        for meta, stage2 in ((old, None), (self.meta, {"sha": "b" * 40}), (self.meta, {"tests": {}}),
                             (dict(self.meta, sha="zz"), None), (dict(self.meta, workflow=".github/workflows/x.yml"), None),
                             ({k: v for k, v in self.meta.items() if k != "public_run_id"}, None)):
            self.assertFalse(self.f.decide(meta, stage2, self.now)[0], (meta, stage2))

    def test_unknown_job_outcome_counts_as_failure(self):
        self.assertEqual(self.f.decide(dict(self.meta, game_job="weird"), None, self.now)[2]["game_job"], "failure")

    def test_bind(self):
        self.assertTrue(self.f.bind(self.values, self.run, self.jobs, 3)[0])
        for run in (dict(self.run, path=".github/workflows/other.yml"), dict(self.run, head_sha="b" * 40),
                    dict(self.run, run_attempt=2), dict(self.run, conclusion="failure"), dict(self.run, head_branch="x")):
            self.assertFalse(self.f.bind(self.values, run, self.jobs, 3)[0], run)
        self.assertFalse(self.f.bind(self.values, self.run, self.jobs[:2], 3)[0])
        self.assertFalse(self.f.bind(self.values, self.run, [dict(self.jobs[0], conclusion="failure")] + self.jobs[1:], 3)[0])

    def test_both_files_read_at_one_commit_while_the_branch_moves(self):
        commits = iter(["c" * 40, "d" * 40])
        seen = []

        def fake_get(api, path, token, raw=False):
            seen.append(path)
            if path.endswith("git/ref/heads/results"):
                return {"object": {"sha": next(commits)}}
            if "contents/meta.json" in path:
                return dict(self.meta, finished_at=datetime.now(timezone.utc).strftime(result.STAMP))
            if "contents/stage2.json" in path:
                return {"sha": SHA, "tests": {}}
            if path.endswith("/jobs?per_page=100"):
                return {"jobs": [{"name": f"Compile shard {i}", "conclusion": "success"} for i in range(16)]}
            return self.run
        saved = self.f.get
        self.f.get = fake_get
        env = {"GAME_REPO": "o/game", "GAME_TOKEN": "t", "GH_TOKEN": "u", "GITHUB_REPOSITORY": "o/pub", "SHARDS": "16"}
        old = {k: os.environ.get(k) for k in env}
        os.environ.update(env)
        try:
            with tempfile.TemporaryDirectory() as d:
                out = self.f.main(d)
        finally:
            self.f.get = saved
            for k, v in old.items():
                if v is None:
                    os.environ.pop(k, None)
                else:
                    os.environ[k] = v
        self.assertIn("fresh=true", out)
        contents = [p for p in seen if "/contents/" in p]
        self.assertEqual(contents, [f"repos/o/game/contents/meta.json?ref={'c' * 40}",
                                    f"repos/o/game/contents/stage2.json?ref={'c' * 40}"])
        self.assertEqual(sum(p.endswith("git/ref/heads/results") for p in seen), 1)

    def test_without_tokens_nothing_is_folded(self):
        env = {k: v for k, v in os.environ.items() if k not in ("GAME_REPO", "GAME_TOKEN", "GH_TOKEN", "GITHUB_OUTPUT")}
        with tempfile.TemporaryDirectory() as d:
            r = subprocess.run([sys.executable, "-B", str(NIGHTLY / "fold_in.py"), d], env=env, capture_output=True, text=True)
        self.assertEqual(r.returncode, 0)
        self.assertIn("fresh=false", r.stdout)

    def test_game_failure_without_stage2_fails_the_night(self):
        n = result.build(CINFO, None, meta(game_job="failure"))
        self.assertEqual(n["result"], "fail")
        self.assertTrue(result.alarms(n)["nightly-build"]["failed"])


class Linking(unittest.TestCase):
    def fake(self, table):
        import link_census
        saved = link_census.coffsym.obj_symbols
        link_census.coffsym.obj_symbols = lambda p: table[Path(p).name]
        self.addCleanup(setattr, link_census.coffsym, "obj_symbols", saved)
        return link_census

    def test_census_counts(self):
        def obj(defs, undefs, weak=None):
            return {"defs": {n: [False, "code", 1, ".text", 1] for n in defs}, "undefs": undefs,
                    "weak": weak or {}, "secs": {}}
        table = {"a.obj": obj(["?f@@YAXXZ", "_shared"], ["?g@@YAXXZ", "_memcpy", "?gone@@YAXXZ", "___ImageBase"]),
                 "b.obj": obj(["?g@@YAXXZ"], ["?f@@YAXXZ", "?gone@@YAXXZ", "__imp__CreateFileA@28"])}
        lc = self.fake(table)
        r = lc.census(["b.obj", "a.obj"], ["_memcpy", "__imp__CreateFileA@28"])
        self.assertEqual((r["needed"], r["missing"], r["provided"], r["pair"]), (5, 1, 4, [4, 5]))
        self.assertEqual(r["missing_by_kind"], {"c++": 1})

    def test_census_is_order_independent(self):
        def obj(defs, undefs):
            return {"defs": {n: [False, "code", 1, ".text", 1] for n in defs}, "undefs": undefs, "weak": {}, "secs": {}}
        table = {"a.obj": obj(["_x"], ["_y"]), "b.obj": obj(["_y"], ["_z"])}
        lc = self.fake(table)
        self.assertEqual(lc.census(["a.obj", "b.obj"], []), lc.census(["b.obj", "a.obj"], []))

    def test_result_field_and_summary(self):
        with tempfile.TemporaryDirectory() as d:
            p = Path(d, "link.json")
            p.write_text(json.dumps({"objects": 3253, "needed": 8267, "missing": 3130, "provided": 5137,
                                     "duplicates": 7, "missing_by_kind": {"c++": 3094, "c": 34, "import": 2}}))
            link = result.read_link(str(p))
        full = dict(CINFO, files=3253, compiled=3253)
        n = result.build(full, None, dict(meta(game_job="skipped", live=False), link=link))
        self.assertEqual(n["provided_required_symbols"], [5137, 8267])
        self.assertIn("| Linking | 5,137 of 8,267 names", result.summary(n))
        short = dict(full, files=3254)
        n = result.build(short, None, dict(meta(game_job="skipped", live=False), link=link))
        self.assertIsNone(n["provided_required_symbols"])
        self.assertIn("expected one object for each of 3254 source files; found 3253", result.summary(n))
        extra = dict(full, files=3252, compiled=3252)
        self.assertIsNone(result.build(extra, None, dict(meta(game_job="skipped", live=False), link=link))
                          ["provided_required_symbols"])

    def test_bad_or_missing_census_means_not_measured(self):
        self.assertIsNone(result.read_link(None))
        self.assertIsNone(result.read_link("/nonexistent/link.json"))
        with tempfile.TemporaryDirectory() as d:
            p = Path(d, "link.json")
            for bad in ({"needed": 10, "missing": 3, "provided": 8, "objects": 1, "duplicates": 0},
                        {"needed": 0, "missing": 0, "provided": 0, "objects": 1, "duplicates": 0}, [1, 2]):
                p.write_text(json.dumps(bad))
                self.assertIsNone(result.read_link(str(p)), bad)
        n = result.build(CINFO, None, meta(game_job="skipped", live=False))
        self.assertIsNone(n["provided_required_symbols"])
        self.assertIn("| Linking | not measured this run |", result.summary(n))

    def test_names_file_is_sorted_and_unique(self):
        names = [l for l in (NIGHTLY / "link-names/library-names.txt").read_text().splitlines()
                 if l and not l.startswith("#")]
        self.assertGreater(len(names), 1000)
        self.assertEqual(names, sorted(set(names)))

    def test_workflow_schedules_and_linking_steps(self):
        wf = REPO / ".github/workflows/nightly.yml"
        if not wf.exists():
            self.skipTest("workflow not beside the package")
        text = wf.read_text()
        crons = re.findall(r"cron: '([^']+)'", text)
        self.assertEqual(len(crons), 2)
        compile_cron, fold_cron = crons
        self.assertEqual(text.count(f"github.event.schedule != '{fold_cron}'"), 2)
        self.assertEqual(text.count(f"github.event.schedule == '{fold_cron}'"), 1)
        self.assertNotIn(f"'{compile_cron}' ||", text)
        self.assertEqual(text.count("ci/nightly/link_census.py"), 1)
        self.assertEqual(text.count('--link "$RUNNER_TEMP/link.json"'), 1)
        self.assertEqual(text.count('--previous "$RUNNER_TEMP/previous.json"'), 2)
        self.assertIn("--map config/map.json", text)
        self.assertIn("link-missing.tsv", text)
        self.assertNotIn("BFME_NIGHTLY_WORKERS", text)


class ObjectSet(unittest.TestCase):
    def make(self, root, shards):
        import hashlib
        for i, files in enumerate(shards):
            d = Path(root) / f"nightly-shard-{i}-a1"
            (d / "obj").mkdir(parents=True)
            lines = []
            for tu, body in files:
                lines.append(json.dumps({"tu": tu}))
                if body is None:
                    continue
                name = tu.replace("/", "_") + ".obj"
                (d / "obj" / name).write_bytes(body)
                (d / "obj" / (name + ".compile.json")).write_text(json.dumps(
                    {"object_sha256": hashlib.sha256(body).hexdigest()}))
            (d / "judge.jsonl").write_text("".join(line + "\n" for line in lines))

    def run_set(self, shards, mutate=None):
        import link_census
        with tempfile.TemporaryDirectory() as d:
            self.make(d, shards)
            if mutate:
                mutate(Path(d))
            return link_census.object_set(d)

    def test_exact_set(self):
        got = self.run_set([[("src/a.cpp", b"A"), ("src/b.cpp", b"B")], [("src/c.c", b"C")]])
        self.assertEqual([Path(p).name for p in got], ["src_a.cpp.obj", "src_b.cpp.obj", "src_c.c.obj"])

    def test_extra_object_refused(self):
        def add(d):
            (d / "nightly-shard-0-a1/obj/src_x.cpp.obj").write_bytes(b"X")
        with self.assertRaisesRegex(ValueError, "1 unrecorded"):
            self.run_set([[("src/a.cpp", b"A")]], add)

    def test_duplicate_replacing_missing_refused(self):
        def swap(d):
            (d / "nightly-shard-1-a1/obj/src_b.cpp.obj").unlink()
            (d / "nightly-shard-1-a1/obj/src_a.cpp.obj").write_bytes(b"A")
        with self.assertRaisesRegex(ValueError, "found src_a.cpp.obj 2 times"):
            self.run_set([[("src/a.cpp", b"A")], [("src/b.cpp", b"B")]], swap)

    def test_missing_object_refused(self):
        def drop(d):
            (d / "nightly-shard-0-a1/obj/src_b.cpp.obj").unlink()
        with self.assertRaisesRegex(ValueError, "1 absent"):
            self.run_set([[("src/a.cpp", b"A"), ("src/b.cpp", b"B")]], drop)

    def test_changed_object_refused(self):
        def change(d):
            (d / "nightly-shard-0-a1/obj/src_a.cpp.obj").write_bytes(b"Z")
        with self.assertRaisesRegex(ValueError, "match its compile receipt"):
            self.run_set([[("src/a.cpp", b"A")]], change)

    def test_file_judged_twice_refused(self):
        with self.assertRaisesRegex(ValueError, "judged once"):
            self.run_set([[("src/a.cpp", b"A")], [("src/a.cpp", None)]])

    def test_compile_error_has_no_object(self):
        def errored(d):
            p = d / "nightly-shard-0-a1/judge.jsonl"
            p.write_text(p.read_text().replace('{"tu": "src/b.cpp"}', '{"tu": "src/b.cpp", "error": "C2065"}'))
        got = self.run_set([[("src/a.cpp", b"A"), ("src/b.cpp", None)]], errored)
        self.assertEqual([Path(p).name for p in got], ["src_a.cpp.obj"])


class GameExit(unittest.TestCase):
    frames = range(25, 1001, 25)

    def test_equal_frames_but_crash_at_exit_is_a_crash(self):
        f = checks.crc_compare(record(self.frames), record(self.frames), 1000)
        v = checks.judge_test(checks.TESTS[0], f, 0, {"rc": 3221225477})
        self.assertEqual(v["status"], "crash")
        self.assertIn("expected the game to exit with code 0; found 3221225477", v["detail"])

    def test_missing_exit_record_is_a_crash(self):
        f = checks.crc_compare(record(self.frames), record(self.frames), 1000)
        self.assertEqual(checks.judge_test(checks.TESTS[0], f, 0, None)["status"], "crash")
        self.assertEqual(checks.judge_test(checks.TESTS[0], f, 0, {})["status"], "crash")

    def test_differs_stays_differs_whatever_the_exit(self):
        f = checks.crc_compare(record(self.frames), record(self.frames, bad={300}), 1000)
        self.assertEqual(checks.judge_test(checks.TESTS[0], f, 0, {"rc": 1})["status"], "differs")

    def test_reload_needs_minimum_frames(self):
        save = next(t for t in checks.TESTS if t["name"] == "save")
        ctl = {"crcs": {str(f): f for f in range(625, 701, 25)}, "crcs_before_load": {str(f): f for f in range(25, 176, 25)}}
        short = checks.reload_compare(ctl, dict(ctl, outcome="pass"))
        v = checks.judge_test(save, short, 0, {"rc": 0})
        self.assertEqual(v["status"], "crash")
        self.assertIn("expected at least 20 checksum frames after the load; found 4", v["detail"])


class Projection(unittest.TestCase):
    def test_projection_and_packing_built_once(self):
        import types
        import compile_main
        calls = {"projection": 0, "pack": 0}

        def projection(git, revision):
            calls["projection"] += 1
            return {"src/a.cpp": "x"}, {"src/a.cpp": "h"}

        def pack(files):
            calls["pack"] += 1
            return {"zlib": "z", "sha256": "s"}
        measure_mod = types.SimpleNamespace(projection=projection, pack_sources=pack)
        compare_mod = types.SimpleNamespace(projection=projection)
        git = types.SimpleNamespace(path=Path("/repo.git"))
        built = compile_main.memoize_projection(measure_mod, compare_mod)
        for _ in range(26):
            sources, _hashes = measure_mod.projection(git, SHA)
            measure_mod.pack_sources(sources)
        compare_mod.projection(git, SHA)
        self.assertEqual(calls, {"projection": 1, "pack": 1})
        self.assertEqual(len(built), 1)
        measure_mod.projection(git, "b" * 40)
        self.assertEqual(calls["projection"], 2)


class LinkingDetail(unittest.TestCase):
    def test_missing_table_format(self):
        import link_census
        table = link_census.missing_table({"?b@@YAXXZ": ["x.obj", "y.obj"], "?a@@YAXXZ": ["y.obj"],
                                           "_c": ["x.obj", "y.obj"]},
                                          {"x.obj": "src/x.cpp", "y.obj": "src/y.cpp"}, SHA, "2026-01-01", 3253)
        lines = table.splitlines()
        self.assertEqual(lines[0], f"# link census of {SHA} on 2026-01-01 objects=3253 missing=3")
        self.assertEqual(lines[1], "symbol\treferrers\tfirst_referrer")
        self.assertEqual(lines[2:], ["?b@@YAXXZ\t2\tsrc/x.cpp", "_c\t2\tsrc/x.cpp", "?a@@YAXXZ\t1\tsrc/y.cpp"])

    def test_program_functions(self):
        import link_census
        m = {"rows": [{"rva": "0x1", "role": "owned", "name": "?f@@YAXXZ", "accepted_names": ["?f2@@YAXXZ"]},
                      {"rva": "0x1", "role": "owned", "name": "?f@@YAXXZ"},
                      {"rva": "0x2", "role": "owned", "name": "?g@@YAXXZ"},
                      {"rva": "0x3", "role": "comdat", "name": "?h@@YAXXZ"}]}
        self.assertEqual(link_census.program_functions(m, {"?f2@@YAXXZ", "?h@@YAXXZ"}), [1, 2])

    def link(self, **extra):
        return dict({"objects": 3253, "needed": 8317, "missing": 3136, "provided": 5181, "duplicates": 7,
                     "missing_by_kind": {}, "duplicate_names": ["?x@@YAXXZ"], "program_functions": [14358, 41565]}, **extra)

    def test_headline_switch_and_unit(self):
        full = dict(CINFO, files=3253, compiled=3253)
        base = dict(meta(game_job="skipped", live=False), link=self.link())
        n = result.build(full, None, base)
        self.assertEqual((n["provided_required_symbols"], n["linking_unit"]), ([5181, 8317], "names"))
        n = result.build(full, None, dict(base, linking_headline="program"))
        self.assertEqual((n["provided_required_symbols"], n["linking_unit"]), ([14358, 41565], "functions"))
        text = result.summary(n)
        self.assertIn("3,136 missing, 7 defined twice", text)
        self.assertIn("| Whole program | 14,358 of 41,565 functions", text)

    def test_carried_link_only_for_the_same_commit(self):
        prev = {"sha": SHA, "link_census": self.link()}
        self.assertEqual(result.carried_link(prev, SHA)["program_functions"], [14358, 41565])
        self.assertIsNone(result.carried_link(prev, "b" * 40))
        self.assertIsNone(result.carried_link({"sha": SHA, "link_census": {"needed": "x"}}, SHA))


class GameRecord(unittest.TestCase):
    def test_written_only_by_a_run_that_played(self):
        self.assertIsNone(result.game_record(result.build(CINFO, None, meta(game_job="skipped", live=False))))
        self.assertIsNone(result.game_record(result.build(
            CINFO, None, dict(meta(game_job="skipped", live=False), game_external=True))))
        skipped = dict(stage2(), tests={}, games_skipped=True)
        self.assertIsNone(result.game_record(result.build(CINFO, skipped, meta())))
        rec = result.game_record(result.build(CINFO, stage2(), meta()))
        self.assertEqual((rec["sha"], rec["files_substituted"], rec["simulation_equal"], rec["control_caught"]),
                         (SHA, 323, True, True))
        self.assertEqual(rec["schema"], "bfme2-nightly-game/1")

    def test_publish_writes_extra_files_only_when_present(self):
        import publish_branch
        with tempfile.TemporaryDirectory() as d:
            n = result.build(CINFO, None, meta(game_job="skipped", live=False))
            Path(d, "nightly.json").write_text(json.dumps(n))
            Path(d, "summary.md").write_text("s")
            files, _ = publish_branch.files_for(d)
            self.assertNotIn("game/latest.json", files)
            self.assertNotIn("link-missing.tsv", files)
            Path(d, "game.json").write_text("{}")
            Path(d, "link-missing.tsv").write_text("# x\n")
            files, _ = publish_branch.files_for(d)
            self.assertIn("game/latest.json", files)
            self.assertIn("link-missing.tsv", files)


class FoldInAlarm(unittest.TestCase):
    def test_not_fresh_writes_the_alarm(self):
        env = {k: v for k, v in os.environ.items() if k not in ("GAME_REPO", "GAME_TOKEN", "GH_TOKEN", "GITHUB_OUTPUT")}
        with tempfile.TemporaryDirectory() as d:
            r = subprocess.run([sys.executable, "-B", str(NIGHTLY / "fold_in.py"), d], env=env, capture_output=True,
                               text=True)
            alarm = json.loads(Path(d, "alarms.json").read_text())
        self.assertEqual(r.returncode, 0)
        self.assertIn("fresh=false", r.stdout)
        self.assertTrue(alarm["nightly-build"]["failed"])
        self.assertIn("expected a fresh result from the private game run", alarm["nightly-build"]["detail"])

class Startup(unittest.TestCase):
    """The shard start-up exactly as the workflow step runs it, in the full repository layout."""

    def test_self_check_in_repository_layout(self):
        missing = [p for p in ("ci/managed-check/common.py", "ci/managed-nt2/gitrepo.py", "baseline/game.dat", ".git")
                   if not (REPO / p).exists()]
        self.assertEqual(missing, [], "run these tests in a full checkout of the candidate tree, not the package alone")
        head = subprocess.run(["git", "-C", str(REPO), "rev-parse", "HEAD"], capture_output=True, text=True,
                              check=True).stdout.strip()
        with tempfile.TemporaryDirectory() as tmp:
            env = {"PATH": "/usr/bin:/bin", "HOME": os.environ.get("HOME", tmp), "LANG": "C", "LC_ALL": "C",
                   "TMPDIR": tmp, "GITHUB_SHA": head}
            r = subprocess.run(["python3", "-B", "ci/nightly/compile_main.py", "--self-check"], cwd=REPO, env=env,
                               capture_output=True, text=True)
        self.assertEqual(r.returncode, 0, r.stdout + r.stderr)
        self.assertIn("nightly compile self-check:", r.stdout)
        self.assertNotIn("ModuleNotFoundError", r.stderr)

    def test_self_check_reports_a_wrong_checkout(self):
        if not (REPO / "ci/managed-check/common.py").exists():
            self.skipTest("needs the full repository layout (see test_self_check_in_repository_layout)")
        with tempfile.TemporaryDirectory() as tmp:
            env = {"PATH": "/usr/bin:/bin", "HOME": os.environ.get("HOME", tmp), "LANG": "C", "LC_ALL": "C",
                   "TMPDIR": tmp, "GITHUB_SHA": "0" * 40}
            r = subprocess.run(["python3", "-B", "ci/nightly/compile_main.py", "--self-check"], cwd=REPO, env=env,
                               capture_output=True, text=True)
        self.assertEqual(r.returncode, 1)
        self.assertIn("expected the checkout at the workflow commit", r.stderr)


class TrialWorkflow(unittest.TestCase):
    def test_trial_publishes_nothing(self):
        wf = REPO / ".github/workflows/nightly-trial.yml"
        if not wf.exists():
            self.skipTest("workflow not beside the package")
        text = wf.read_text()
        self.assertIn("\npermissions: {}\n", text)
        self.assertIn("workflow_dispatch:", text)
        for word in ("schedule:", "pull_request", "push:", "contents: write", "issues: write", "publish_branch.py",
                     "alarms.py", "fold_in.py", "github.token", "secrets."):
            self.assertNotIn(word, text)
        self.assertIn("group: nightly-trial", text)
        self.assertNotIn("name: nightly-shard-", text)
        for line in text.splitlines():
            if "runs-on:" in line:
                self.assertEqual(line.strip(), "runs-on: boat-small/from=bfme2-compiler-v1")
            if "uses:" in line:
                self.assertRegex(line, r"uses: [\w-]+/[\w-]+@[0-9a-f]{40}$")
        self.assertEqual(text.count("compile_main.py --self-check"), 1)

    def test_nightly_runs_the_self_check_first(self):
        text = (REPO / ".github/workflows/nightly.yml").read_text() if (REPO / ".github/workflows/nightly.yml").exists() else ""
        if not text:
            self.skipTest("workflow not beside the package")
        self.assertLess(text.index("compile_main.py --self-check"), text.index('compile_main.py "$SHARD"'))


class LastGoodLinking(unittest.TestCase):
    link = {"objects": 3253, "needed": 8317, "missing": 3136, "provided": 5181, "duplicates": 7,
            "missing_by_kind": {}, "duplicate_names": [], "program_functions": [14358, 41565]}
    prev = {"sha": "c" * 40, "date": "2026-01-01", "run_url": "https://example.invalid/run/1",
            "provided_required_symbols": [5181, 8317], "linking_unit": "names",
            "linking_measured": {"this_run": True, "sha": "c" * 40, "date": "2026-01-01",
                                 "run_url": "https://example.invalid/run/1"}}

    def test_measured_this_run(self):
        full = dict(CINFO, files=3253, compiled=3253)
        n = result.build(full, None, dict(meta(game_job="skipped", live=False), link=self.link, previous=self.prev))
        self.assertEqual(n["provided_required_symbols"], [5181, 8317])
        self.assertTrue(n["linking_measured"]["this_run"])
        self.assertEqual(n["linking_measured"]["sha"], SHA)

    def test_failed_compile_keeps_the_last_good_figure_with_its_date(self):
        n = result.build(CINFO, None, dict(meta(compile_job="failure", game_job="skipped", live=False),
                                           previous=self.prev))
        self.assertEqual(n["result"], "fail")
        self.assertEqual(n["provided_required_symbols"], [5181, 8317])
        lm = n["linking_measured"]
        self.assertEqual((lm["this_run"], lm["sha"], lm["date"]), (False, "c" * 40, "2026-01-01"))
        self.assertIn("measured no linking figure", lm["reason"])
        self.assertIn("from the earlier run of 2026-01-01", result.summary(n))
        self.assertTrue(n["headline"].startswith("compile stage failed"))

    def test_carry_keeps_the_original_measurement_across_failed_runs(self):
        once = result.build(CINFO, None, dict(meta(compile_job="failure", game_job="skipped", live=False),
                                              previous=self.prev))
        twice = result.build(CINFO, None, dict(meta(compile_job="failure", game_job="skipped", live=False),
                                               previous=dict(once, sha="d" * 40, date="2026-01-02")))
        self.assertEqual((twice["linking_measured"]["sha"], twice["linking_measured"]["date"]), ("c" * 40, "2026-01-01"))

    def test_refused_census_carries_with_the_reason(self):
        short = dict(CINFO, files=3254, compiled=3253)
        n = result.build(short, None, dict(meta(game_job="skipped", live=False), link=self.link, previous=self.prev))
        self.assertEqual(n["provided_required_symbols"], [5181, 8317])
        self.assertIn("expected one object for each of 3254", n["linking_measured"]["reason"])

    def test_no_carry_across_a_change_of_unit_or_without_history(self):
        n = result.build(CINFO, None, dict(meta(compile_job="failure", game_job="skipped", live=False),
                                           previous=self.prev, linking_headline="program"))
        self.assertIsNone(n["provided_required_symbols"])
        n = result.build(CINFO, None, meta(compile_job="failure", game_job="skipped", live=False))
        self.assertIsNone(n["provided_required_symbols"])
        self.assertFalse(n["linking_measured"]["this_run"])

class WorkflowExpressions(unittest.TestCase):
    """No dispatch input or matrix value is pasted into shell source: they reach the shell through env only."""

    def test_no_untrusted_expression_in_run_lines(self):
        import re as _re
        checked = 0
        for name in ("nightly.yml", "nightly-trial.yml"):
            wf = REPO / ".github/workflows" / name
            if not wf.exists():
                continue
            checked += 1
            in_run, indent = False, 0
            for line in wf.read_text().splitlines():
                stripped = line.lstrip()
                if in_run and stripped and len(line) - len(stripped) <= indent:
                    in_run = False
                if stripped.startswith("run:"):
                    in_run, indent = True, len(line) - len(stripped)
                    body = stripped[4:]
                else:
                    body = line if in_run else ""
                for expr in _re.findall(r"\$\{\{([^}]*)\}\}", body):
                    self.assertNotRegex(expr, r"inputs\.|matrix\.|github\.event", f"{name}: {line.strip()}")
        if not checked:
            self.skipTest("workflows not beside the package")

    def test_trial_matrix_is_fixed(self):
        wf = REPO / ".github/workflows/nightly-trial.yml"
        if not wf.exists():
            self.skipTest("workflow not beside the package")
        text = wf.read_text()
        self.assertIn("type: choice", text)
        self.assertNotIn("type: string", text)
        self.assertIn("fromJSON('[0, 1]')", text)


class MultiAttemptShardSelection(unittest.TestCase):
    def test_selects_highest_attempt_up_to_current(self):
        import select_shards
        with tempfile.TemporaryDirectory() as tmp:
            src, dst = Path(tmp) / "raw", Path(tmp) / "sel"
            for name in (
                "nightly-shard-summary-0-a1",
                "nightly-shard-summary-1-a1",
                "nightly-shard-summary-1-a2",
                "nightly-shard-summary-1-a3",
                "nightly-shard-summary-99-a1",
            ):
                d = src / name
                d.mkdir(parents=True)
                (d / "sample.obj").write_bytes(b"obj")
            winners = select_shards.select(src, dst, "nightly-shard-summary-", max_attempt=2, expected=16)
            self.assertEqual(sorted(winners), [0, 1])
            self.assertEqual(winners[0][0], 1)
            self.assertEqual(winners[1][0], 2)
            self.assertEqual(sorted(p.name for p in dst.iterdir()),
                             ["nightly-shard-summary-0-a1", "nightly-shard-summary-1-a2"])
            # Real directories (not symlinks): rglob("*.obj") used by link_census must discover every object.
            self.assertEqual(len(list(dst.rglob("*.obj"))), 2)


if __name__ == "__main__":
    unittest.main()
