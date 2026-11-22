"""Offline tests for ci/header/collect.py: what is written, and when nothing is."""
import io
import json
import os
from pathlib import Path
import sys
import tempfile
import subprocess
import unittest
import unittest.mock

sys.path.insert(0, str(Path(__file__).resolve().parents[1]))
import collect  # noqa: E402

INSTALLED_READER = collect.H2_READER
collect.H2_READER = Path("/nonexistent/ci/managed-check/header_h2_action.py")   # tests choose the reader themselves
collect.H5_READER = Path("/nonexistent/ci/header-witness/h5_reader.py")

FIXTURE = json.loads((Path(__file__).parent / "fixtures/preview-13-15.json").read_text())
BASE = FIXTURE["base"]
PR13 = next(item for item in FIXTURE["results"] if item["pr"] == 13)


class FakeApi(collect.Api):
    def __init__(self, head=PR13["head"], main=BASE, statuses=(), pulls=(), files=None, pr=None, file_codes=None):
        self.token, self.writes = "x" * 20, []
        self.head, self.main, self.statuses = head, main, list(statuses)
        self.pulls, self.files, self.pr, self.file_codes = list(pulls), files or {}, pr or {}, file_codes or {}

    def send(self, method, path, body=None):
        if method != "GET":
            self.writes.append((method, path, body))
            return 201, {}
        if path == "git/ref/heads/main":
            return 200, {"object": {"sha": self.main}}
        if path.startswith("pulls?"):
            page = int(path.rsplit("=", 1)[1])
            return 200, [{"number": n, "draft": d} for n, d in self.pulls][(page - 1) * 100:page * 100]
        if "/files?" in path:
            number, page = int(path.split("/")[1]), int(path.rsplit("=", 1)[1])
            if (number, page) in self.file_codes:
                return self.file_codes[(number, page)], None
            names = self.files.get(number, [])
            return 200, [f if isinstance(f, dict) else {"filename": f} for f in names][(page - 1) * 100:page * 100]
        if path.startswith("pulls/"):
            number = int(path.split("/")[1])
            pr = {"state": "open", "draft": False, "head": {"sha": self.head},
                  "base": {"ref": "main", "repo": {"id": collect.REPO_ID}},
                  "changed_files": len(self.files.get(number, []))}
            pr.update(self.pr)
            return 200, pr
        if path.startswith("commits/"):
            return 200, self.statuses
        return 404, None


def record(checks=True, tools=BASE):
    item = {"number": 13, "head": PR13["head"], "base": BASE, "tree": PR13["tree"], "tools": tools,
            "header_paths": PR13["header_paths"]}
    item["checks"] = {"preview": PR13["preview"], "explicit_removed": PR13["explicit_removed"]} if checks else None
    return item


def publish(rec, api, workflow=BASE):
    with tempfile.TemporaryDirectory() as tmp, unittest.mock.patch.dict(os.environ, {"GITHUB_SHA": workflow}):
        path = Path(tmp, "p.json")
        path.write_text(json.dumps(rec))
        lines = []
        collect.publish(path, api=api, out=lines.append)
        return lines


class Publish(unittest.TestCase):
    def test_writes_the_composed_decision_once(self):
        api = FakeApi()
        publish(record(), api)
        self.assertEqual(len(api.writes), 1)
        body = api.writes[0][2]
        self.assertEqual((body["context"], body["state"]), ("BFME II / Header rules", "pending"))
        self.assertTrue(body["description"].startswith("Held: rule not built"))
        api = FakeApi(statuses=[{"id": 9, "context": "BFME II / Header rules", "state": "pending",
                                 "description": body["description"], "creator": {"id": collect.BOT_ID}}])
        publish(record(), api)
        self.assertEqual(api.writes, [])

    def test_checks_not_installed_hold_every_source_rule_as_not_built(self):
        api = FakeApi()
        publish(record(checks=False), api)
        self.assertEqual(api.writes[0][2]["description"],
                         f"Held: rule not built: H2, H3, H4, H5, H6, H7, H8 (main {BASE[:12]})")

    def test_moved_head_or_main_writes_nothing(self):
        for api in (FakeApi(head="9" * 40), FakeApi(main="8" * 40)):
            lines = publish(record(), api)
            self.assertEqual(api.writes, [])
            self.assertIn("moved", lines[0])

    def test_a_status_from_anyone_but_the_bot_is_ignored(self):
        forged = {"id": 9, "context": "BFME II / Boat", "state": "success",
                  "description": f"BFME II comparison passed (main {BASE[:12]})", "creator": {"id": 1}}
        api = FakeApi(statuses=[forged])
        rows = collect.components_for(record(), collect.latest_bot_status(api.statuses, "BFME II / Boat"))
        self.assertEqual(rows["H1"]["state"], "held")

    def test_no_header_change_writes_nothing(self):
        rec = record()
        rec["header_paths"] = []
        api = FakeApi()
        publish(rec, api)
        self.assertEqual(api.writes, [])


    def test_stale_tools_or_workflow_write_nothing_and_current_ones_write(self):
        for rec, workflow in ((record(tools="7" * 40), BASE), (record(), "7" * 40), (record(tools=None), BASE)):
            api = FakeApi()
            lines = publish(rec, api, workflow)
            self.assertEqual(api.writes, [])
            self.assertIn("newer run decides", lines[0])
        api = FakeApi()
        publish(record(), api, BASE)
        self.assertEqual(len(api.writes), 1)

    def test_draft_or_retargeted_writes_nothing(self):
        for change in ({"draft": True}, {"base": {"ref": "dev", "repo": {"id": collect.REPO_ID}}},
                       {"base": {"ref": "main", "repo": {"id": 7}}}, {"state": "closed"}):
            api = FakeApi(pr=change)
            publish(record(), api)
            self.assertEqual(api.writes, [], change)


def h2(status, *findings):
    return {"status": status, "findings": list(findings), "layout_only": []}


class DeclarationEvidence(unittest.TestCase):
    """H2 comes only from the compiler check's verified receipt; anything else holds."""

    def decide(self, receipt):
        api = FakeApi()
        with unittest.mock.patch.object(collect, "read_h2", return_value=receipt):
            publish(record(checks=False), api)
        return api.writes[0][2]["state"], api.writes[0][2]["description"]

    def test_reader_not_on_main_is_not_built(self):
        self.assertEqual(self.decide(None)[1], f"Held: rule not built: H2, H3, H4, H5, H6, H7, H8 (main {BASE[:12]})")

    def test_a_refused_declaration_fails_h2(self):
        state, text = self.decide(h2("refuse", {"status": "refuse", "declaration": "Foo::bar",
                                                "reason": "compiles to a different name", "header": "include/f.h"}))
        self.assertEqual(state, "failure")
        self.assertTrue(text.startswith("H2: Foo::bar: compiles to a different name."), text)

    def test_a_ledger_request_holds_and_never_fails(self):
        state, text = self.decide(h2("held", {"status": "pass", "declaration": "A::a"},
                                     {"status": "ledger_request", "declaration": "B::b", "reason": "placeholder"}))
        self.assertEqual((state, text), ("pending", f"Held: ledger request B::b (main {BASE[:12]})"))

    def test_missing_or_moved_evidence_holds_h2_and_passes_nothing(self):
        for receipt in ({"error": "no published declaration evidence for this commit (ValueError)"},
                        h2("held", {"status": "held", "reason": "probe budget"}), h2("weird"), {}):
            api = FakeApi()
            rows = collect.components_for(record(), None, receipt)
            self.assertEqual(rows["H2"]["state"], "held", receipt)
            self.assertNotIn("H9", rows)

    def test_a_passing_receipt_passes_h2_only(self):
        rows = collect.components_for(record(), None, h2("pass", {"status": "pass", "declaration": "A::a"}))
        self.assertEqual(rows["H2"]["state"], "pass")
        self.assertEqual(rows["H5"]["state"], "missing")

    def test_the_reader_is_called_with_the_exact_binding_and_any_refusal_holds(self):
        self.assertIsNone(collect.read_h2(record()))
        with tempfile.TemporaryDirectory() as tmp:
            reader = Path(tmp, "header_h2_action.py")
            reader.write_text(
                "seen = []\n"
                "def read_published(binding):\n"
                "    seen.append(binding)\n"
                "    if binding['number'] == 13:\n"
                "        raise ValueError('moved')\n"
                "    return {'status': 'pass', 'findings': [], 'layout_only': []}\n")
            sys.modules.pop("header_h2_action", None)
            with (unittest.mock.patch.object(collect, "H2_READER", reader),
                  unittest.mock.patch("sys.stderr", new_callable=io.StringIO) as log):
                held = collect.read_h2(record())
                passed = collect.read_h2(dict(record(), number=14))
            module = sys.modules.pop("header_h2_action")
            sys.path.remove(tmp)
        self.assertEqual(held, {"error": "no published declaration evidence for this commit (ValueError)"})
        self.assertEqual(passed["status"], "pass")
        self.assertIn("H2 evidence read refused: ValueError: moved", log.getvalue())
        self.assertEqual(module.seen[0], {key: record()[key] for key in ("number", "base", "head", "tree")})


def h5doc(atoms=(), reads=(), compatible=None, held=()):
    layout = {"complete": True, "compatible_shape": (not atoms) if compatible is None else compatible,
              "atoms": list(atoms)}
    return {"version": 1, "held": list(held),
            "headers": {path: {"layout": layout, "access": {"reads": list(reads)}, "held": []}
                        for path in PR13["header_paths"]}}


ATOM = {"header": "include/GameClient/W3DView.h", "qualified_class": "W3DView", "kind": "member",
        "identity": "m_x", "before": {"offset": 8, "width": 4, "type": "int"},
        "after": {"offset": 12, "width": 4, "type": "int"}}
READ = {k: ATOM[k] for k in ("header", "qualified_class", "kind", "identity")} | {
    "receiver_type": "W3DView", "observed": ATOM["after"], "operation": "access", "rva": 4096}


class LayoutEvidence(unittest.TestCase):
    def h5(self, doc):
        return collect.components_for(record(), None, None, None, doc)["H5"]

    def test_unchanged_or_compatible_layout_passes(self):
        self.assertEqual(self.h5(h5doc())["state"], "pass")

    def test_a_moved_member_passes_only_with_its_witness(self):
        self.assertEqual(self.h5(h5doc([ATOM], [READ]))["state"], "pass")
        state = self.h5(h5doc([ATOM]))
        self.assertEqual(state["state"], "held")
        self.assertIn("W3DView::m_x moved", state["message"])
        wrong = dict(READ, observed={"offset": 8, "width": 4, "type": "int"})
        self.assertEqual(self.h5(h5doc([ATOM], [wrong]))["state"], "held")

    def test_size_base_and_slot_changes_and_any_hold_wait(self):
        for doc in (h5doc([dict(ATOM, kind="size")], [READ]), h5doc(held=["4 headers changed"]),
                    h5doc(compatible=False), {"version": 1, "held": [], "headers": {}}):
            self.assertEqual(self.h5(doc)["state"], "held")

    def test_reader_absent_is_not_built_and_a_refusal_holds(self):
        self.assertEqual(collect.components_for(record(), None)["H5"]["state"], "missing")
        self.assertEqual(self.h5({"error": "no published layout evidence"})["state"], "held")

    def test_both_readers_get_the_read_token(self):
        seen = []

        def reader(name):
            def read(rec):
                seen.append((name, os.environ.pop("BFME_RECOVERY_READ_TOKEN", None)))
                return None
            return read
        real = collect.components_for

        def composing(*args):
            seen.append(("compose", os.environ.get("BFME_RECOVERY_READ_TOKEN")))
            return real(*args)
        with unittest.mock.patch.object(collect, "read_h2", side_effect=reader("h2")), \
                unittest.mock.patch.object(collect, "read_h5", side_effect=reader("h5")), \
                unittest.mock.patch.object(collect, "components_for", side_effect=composing), \
                unittest.mock.patch.dict(os.environ, {"BFME_RECOVERY_READ_TOKEN": "t" * 20}):
            publish(record(), FakeApi())
        self.assertEqual(seen, [("h2", "t" * 20), ("h5", "t" * 20), ("compose", None)])


class AdmissionStatus(unittest.TestCase):
    def test_only_the_bot_admission_status_counts_for_h10(self):
        forged = {"id": 9, "context": "BFME II / Admission", "state": "success",
                  "description": "Admitted by rule: header change, wide check", "creator": {"id": 1}}
        rows = collect.components_for(record(), None, None, collect.latest_bot_status([forged], "BFME II / Admission"))
        self.assertEqual(rows["H10"]["state"], "held")
        real = dict(forged, creator={"id": collect.BOT_ID})
        rows = collect.components_for(record(), None, None, collect.latest_bot_status([real], "BFME II / Admission"))
        self.assertEqual(rows["H10"]["state"], "pass")


class LateMove(unittest.TestCase):
    """A move of main or the head while the rules are read (after the first check) writes nothing."""

    def run_with_move(self, move):
        api = FakeApi()
        real = collect.read_h2

        def moving(*args, **kwargs):
            move(api)
            return real(*args, **kwargs)

        with unittest.mock.patch.object(collect, "read_h2", side_effect=moving):
            lines = publish(record(), api)
        return api, lines

    def test_main_or_head_moving_during_the_reads_writes_nothing(self):
        for move in (lambda api: setattr(api, "main", "8" * 40), lambda api: setattr(api, "head", "9" * 40),
                     lambda api: api.pr.update(draft=True)):
            api, lines = self.run_with_move(move)
            self.assertEqual(api.writes, [])
            self.assertIn("moved while the rules were read", lines[-1])

    def test_nothing_moving_still_writes_once(self):
        api, _ = self.run_with_move(lambda api: None)
        self.assertEqual(len(api.writes), 1)


ENV = {"GITHUB_EVENT_NAME": "schedule", "GITHUB_SHA": BASE, "GITHUB_REPOSITORY_ID": str(collect.REPO_ID)}


class Plan(unittest.TestCase):
    def test_only_ready_pull_requests_that_change_include(self):
        api = FakeApi(pulls=[(3, False), (4, False), (13, True), (21, False)],
                      files={3: ["ci/fork-probe.txt"], 4: ["include/a.h", "src/a.cpp"], 21: ["src/b.cpp"]})
        rows = collect.plan(api=api, env=ENV)
        self.assertEqual([row["number"] for row in rows], ["4"])
        self.assertEqual(rows[0]["base"], BASE)

    def test_a_run_whose_tools_are_not_current_main_judges_nothing(self):
        api = FakeApi(pulls=[(4, False)], files={4: ["include/a.h"]})
        self.assertEqual([r["number"] for r in collect.plan(api=api, env=ENV)], ["4"])
        self.assertEqual(collect.plan(api=api, env=dict(ENV, GITHUB_SHA="7" * 40)), [])

    def test_another_repository_is_refused(self):
        with self.assertRaises(collect.Stop):
            collect.plan(api=FakeApi(), env=dict(ENV, GITHUB_REPOSITORY_ID="1"))

    def test_a_header_past_the_first_page_is_found(self):
        late = [f"src/f{i}.cpp" for i in range(250)] + ["include/late.h"]
        api = FakeApi(pulls=[(5, False), (6, False)], files={5: late, 6: [f"src/g{i}.cpp" for i in range(250)]})
        self.assertEqual([r["number"] for r in collect.plan(api=api, env=ENV)], ["5"])

    def test_a_rename_out_of_include_is_found(self):
        api = FakeApi(pulls=[(7, False)], files={7: [{"filename": "src/a.h", "previous_filename": "include/a.h"}]})
        self.assertEqual([r["number"] for r in collect.plan(api=api, env=ENV)], ["7"])

    def test_an_incomplete_or_unreadable_file_list_is_judged(self):
        api = FakeApi(pulls=[(8, False), (9, False), (10, False)],
                      files={8: ["src/a.cpp"], 9: ["src/b.cpp"], 10: ["src/c.cpp"]}, file_codes={(9, 1): 500})
        self.assertEqual([r["number"] for r in collect.plan(api=api, env=ENV)], ["9"])
        api.pr = {"changed_files": 4000}
        self.assertEqual([r["number"] for r in collect.plan(api=api, env=ENV)], ["8", "9", "10"])

    def test_more_than_one_page_of_pull_requests_is_read(self):
        api = FakeApi(pulls=[(n, False) for n in range(100, 230)], files={229: ["include/z.h"]})
        self.assertEqual([r["number"] for r in collect.plan(api=api, env=ENV)], ["229"])

    def test_dispatch_names_one_pull_request(self):
        api = FakeApi(files={4: ["include/a.h"]})
        with tempfile.TemporaryDirectory() as tmp:
            event = Path(tmp, "e.json")
            event.write_text(json.dumps({"client_payload": {"number": 4}}))
            rows = collect.plan(api=api, env=dict(ENV, GITHUB_EVENT_NAME="repository_dispatch",
                                                   GITHUB_EVENT_PATH=str(event)))
        self.assertEqual([row["number"] for row in rows], ["4"])

    def test_bad_dispatch_payload_is_refused(self):
        with tempfile.TemporaryDirectory() as tmp:
            event = Path(tmp, "e.json")
            event.write_text(json.dumps({"client_payload": {"number": "4; rm -rf"}}))
            with self.assertRaises(collect.Stop):
                collect.plan(api=FakeApi(), env=dict(ENV, GITHUB_EVENT_NAME="repository_dispatch",
                                                     GITHUB_EVENT_PATH=str(event)))


class Preview(unittest.TestCase):
    def test_inputs_are_validated_before_any_fetch(self):
        for args in (("13; x", "1" * 40, "2" * 40), ("13", "zz", "2" * 40), ("13", "1" * 40, "short")):
            with self.assertRaises(collect.Stop):
                collect.preview(*args, os.devnull)

    def test_tools_not_at_the_named_main_are_refused_before_any_fetch(self):
        with unittest.mock.patch.object(collect, "tools_commit", return_value="7" * 40), \
                unittest.mock.patch.object(collect, "fetch", side_effect=AssertionError("fetched")):
            with self.assertRaises(collect.Stop):
                collect.preview("13", "1" * 40, "2" * 40, os.devnull)


GIT_ENV = {"GIT_AUTHOR_NAME": "t", "GIT_AUTHOR_EMAIL": "t", "GIT_COMMITTER_NAME": "t",
           "GIT_COMMITTER_EMAIL": "t"}


def run(repo, *args):
    subprocess.run(["git", "-C", repo, *args], check=True, capture_output=True,
                   env={"PATH": os.environ["PATH"], "HOME": repo, **GIT_ENV})
    return subprocess.run(["git", "-C", repo, "rev-parse", "HEAD"], capture_output=True, text=True).stdout.strip()


class HeaderPaths(unittest.TestCase):
    def test_both_ends_of_a_rename_and_odd_names_are_reported(self):
        with tempfile.TemporaryDirectory() as tmp:
            run(tmp, "init", "-q")
            Path(tmp, "include").mkdir()
            Path(tmp, "src").mkdir()
            Path(tmp, "include/a.h").write_text("struct A { int x; };\n" * 20)
            Path(tmp, "src/b.cpp").write_text("int b;\n")
            run(tmp, "add", "-A")
            base = run(tmp, "commit", "-qm", "base")
            Path(tmp, "include/a.h").rename(Path(tmp, "src/a.h"))
            Path(tmp, "include/new\nline.h").write_text("int c;\n")
            run(tmp, "add", "-A")
            head = run(tmp, "commit", "-qm", "head")
            tree = subprocess.run(["git", "-C", tmp, "rev-parse", head + "^{tree}"], capture_output=True,
                                  text=True).stdout.strip()
            git_dir = Path(tmp, ".git")
            self.assertEqual(collect.header_paths(git_dir, base, tree), ["include/a.h", "include/new\nline.h"])
            self.assertEqual(collect.header_paths(git_dir, base, base), [])


class Workflow(unittest.TestCase):
    def test_the_publish_step_reads_evidence_with_the_job_token(self):
        # The repository holds no Actions secret; a secrets.* read token arrives empty and every read refuses.
        workflow = (Path(__file__).resolve().parents[3] / ".github/workflows/header-rules.yml").read_text()
        self.assertIn("BFME_RECOVERY_READ_TOKEN: '${{ github.token }}'", workflow)
        self.assertNotIn("secrets.", workflow)
        self.assertIn("actions: read", workflow.split("    steps:")[-2])



class TrustedRescoreTests(unittest.TestCase):
    def test_rescore_updates_h4_h6_with_authenticated_observations(self):
        import tempfile
        with tempfile.TemporaryDirectory() as tmp:
            repo_dir = Path(tmp) / "bfme-header.git"
            subprocess.run(["git", "init", "--bare", "-q", str(repo_dir)], check=True)
            work = Path(tmp) / "work"
            subprocess.run(["git", "clone", "-q", str(repo_dir), str(work)], check=True)
            (work / "include" / "Game").mkdir(parents=True)
            (work / "include" / "Game" / "A.h").write_text("class A {\npublic:\n    void init();\n};\n")
            env = {**os.environ, "GIT_AUTHOR_NAME": "t", "GIT_AUTHOR_EMAIL": "t@t", "GIT_COMMITTER_NAME": "t", "GIT_COMMITTER_EMAIL": "t@t"}
            subprocess.run(["git", "-C", str(work), "add", "."], check=True)
            subprocess.run(["git", "-C", str(work), "commit", "-q", "-m", "base"], env=env, check=True)
            base = subprocess.check_output(["git", "-C", str(work), "rev-parse", "HEAD"], text=True).strip()
            (work / "include" / "Game" / "A.h").write_text("class A {\npublic:\n    void init();\n    void extra();\n};\n")
            subprocess.run(["git", "-C", str(work), "add", "."], check=True)
            subprocess.run(["git", "-C", str(work), "commit", "-q", "-m", "head"], env=env, check=True)
            head = subprocess.check_output(["git", "-C", str(work), "rev-parse", "HEAD"], text=True).strip()
            subprocess.run(["git", "-C", str(work), "push", "-q", "origin", f"{base}:refs/protected/main", f"{head}:refs/pull/head"], check=True)
            tree = subprocess.check_output(["git", "--git-dir", str(repo_dir), "merge-tree", "--write-tree", base, head], text=True).splitlines()[0]

            # Build real trusted observations using header_trusted.produce
            root = Path(__file__).resolve().parents[3]
            sys.path.insert(0, str(root / "tools/gates"))
            sys.path.insert(0, str(root / "ci/managed-check"))
            from common import PIN
            from gate import policy_at, policy_digest
            from git_objects import Git
            from header_inventory import Binding
            import header_trusted
            cfg = root / "tools/gates/config.json"
            binding = Binding(collect.REPO, base, head, tree, policy_digest(cfg), PIN["checker"], 777, 1)
            with Git(repo_dir) as repo:
                obs = header_trusted.produce(repo, policy_at(cfg), binding)

            preview_file = Path(tmp) / "header-preview.json"
            trusted_file = Path(tmp) / "header-trusted.json"
            preview_file.write_text(json.dumps({
                "number": 43, "head": head, "base": base, "tree": tree, "tools": base,
                "header_paths": ["include/Game/A.h"],
                "checks": collect.source_checks(repo_dir, base, head, tree),
            }))
            trusted_file.write_text(json.dumps({"available": True, "issuer": {"id": "777", "attempt": "1"}, "observations": obs}))

            old_tmp = os.environ.get("RUNNER_TEMP")
            os.environ["RUNNER_TEMP"] = tmp
            try:
                self.assertEqual(collect.rescore(str(preview_file), str(trusted_file)), 0)
            finally:
                if old_tmp is None:
                    os.environ.pop("RUNNER_TEMP", None)
                else:
                    os.environ["RUNNER_TEMP"] = old_tmp
            updated = json.loads(preview_file.read_text())
            self.assertTrue(updated["checks"]["trusted"])
            states = {r["rule"]: r["state"] for r in updated["checks"]["preview"]}
            self.assertEqual(states["H4"], "pass")
            self.assertEqual(states["H6"], "pass")



    def test_publish_drops_trusted_checks_when_boat_issuer_superseded(self):
        import tempfile
        with tempfile.TemporaryDirectory() as tmp:
            preview_file = Path(tmp) / "preview.json"
            base, head, tree = "a" * 40, "b" * 40, "c" * 40
            preview_file.write_text(json.dumps({
                "number": 43, "head": head, "base": base, "tree": tree, "tools": base,
                "header_paths": ["include/Game/A.h"],
                "trusted_issuer": {"id": "100", "attempt": "1"},
                "checks": {"preview": [
                    {"rule": "H3", "state": "pass", "findings": []},
                    {"rule": "H4", "state": "pass", "findings": []},
                    {"rule": "H6", "state": "pass", "findings": []},
                    {"rule": "H7", "state": "pass", "findings": []},
                    {"rule": "H8", "state": "pass", "findings": []},
                ], "trusted": True},
                "untrusted_checks": {"preview": [
                    {"rule": "H3", "state": "pass", "findings": []},
                    {"rule": "H4", "state": "held", "findings": [{"message": "needs complete inventory"}]},
                    {"rule": "H6", "state": "held", "findings": [{"message": "needs complete inventory"}]},
                    {"rule": "H7", "state": "pass", "findings": []},
                    {"rule": "H8", "state": "pass", "findings": []},
                ], "trusted": False},
            }))

            class FakeApi:
                def __init__(self):
                    self.posted = []
                def send(self, method, path, body=None):
                    if method == "GET" and path == "pulls/43":
                        return 200, {"state": "open", "draft": False, "base": {"ref": "main", "repo": {"id": collect.REPO_ID}}, "head": {"sha": head}}
                    if method == "GET" and path == "git/ref/heads/main":
                        return 200, {"object": {"sha": base}}
                    if method == "GET" and path.startswith(f"commits/{head}/statuses"):
                        return 200, [{"id": 1, "context": "BFME II / Boat", "state": "success",
                                      "description": f"BFME II comparison passed (main {base[:12]})",
                                      "creator": {"id": collect.BOT_ID}}]
                    if method == "POST":
                        self.posted.append(body)
                        return 201, {}
                    return 404, None

            api = FakeApi()
            old_read_h2, old_read_h5 = collect.read_h2, collect.read_h5
            old_sha = os.environ.get("GITHUB_SHA")
            os.environ["GITHUB_SHA"] = base
            # Newer Boat run 101 superseded run 100!
            collect.read_h2 = lambda _rec: {"status": "pass", "findings": [], "issuer": {"run": "101", "attempt": "1"}}
            collect.read_h5 = lambda _rec: {"headers": {"include/Game/A.h": {"atoms": []}}, "held": []}
            try:
                out = []
                self.assertEqual(collect.publish(str(preview_file), api=api, out=out.append), 0)
            finally:
                collect.read_h2, collect.read_h5 = old_read_h2, old_read_h5
                if old_sha is None:
                    os.environ.pop("GITHUB_SHA", None)
                else:
                    os.environ["GITHUB_SHA"] = old_sha
            self.assertEqual(len(api.posted), 1)
            self.assertEqual(api.posted[0]["state"], "pending")
            self.assertIn("Held: H4", api.posted[0]["description"])


if __name__ == "__main__":
    unittest.main()
