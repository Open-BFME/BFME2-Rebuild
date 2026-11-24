"""Offline tests for tools/local_check (no compiler, Wine, Docker or network needed).

Run: python -m unittest discover -s tools/local_check -v
"""
import hashlib
import io
import json
import os
import subprocess
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE))
import backends  # noqa: E402
import precheck  # noqa: E402
import verify_toolchain as vt  # noqa: E402

CL_SHA = "2ecf86a3edfd3deae498e08298e210e984537ce9e11759930561e43f40bd2515"


def sha(data):
    return hashlib.sha256(data).hexdigest()


def write(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    path.write_bytes(data)
    return path


FAKE_FILES = {
    "vc/Vc7/bin/cl.exe": b"cl", "vc/Vc7/bin/c1.dll": b"c1", "vc/Vc7/bin/c1xx.dll": b"c1xx",
    "vc/Vc7/bin/c2.dll": b"c2", "vc/Common7/IDE/mspdb71.dll": b"mspdb", "vc/Common7/IDE/msvcr71.dll": b"crt",
    "vcruntime/msvcp71.dll": b"cpp", "vcruntime/msvcr71.dll": b"crt",
}


def fake_manifest_text(files=FAKE_FILES, subtrees=""):
    rows = "\n".join(f"| `tools/{path}` | `{sha(data)}` |" for path, data in files.items())
    return f"""# Compiler Image

### {vt.SECTION_BINARIES}

| Relative path under `/opt/boat-compiler/tools/` | SHA-256 |
| --- | --- |
{rows}

### {vt.SECTION_SUBTREES}

| Subtree | File count | Listing SHA-256 |
| --- | ---: | --- |
{subtrees}
| **All `tools/` combined** | **8** | **`{"0" * 64}`** |

### {vt.SECTION_FILES}

| Relative path under `/opt/boat-compiler/tools/` | SHA-256 |
| --- | --- |

## Directory layout
"""


class ManifestTests(unittest.TestCase):
    def test_real_document(self):
        manifest = vt.load_manifest()
        self.assertEqual(manifest.binaries["tools/vc/Vc7/bin/cl.exe"], CL_SHA)
        self.assertEqual(len(manifest.binaries), 8)
        self.assertIn("tools/vcruntime/msvcp71.dll", manifest.binaries)
        self.assertEqual(manifest.subtrees["tools/vc/Vc7/include"][0], 155)
        self.assertEqual(manifest.subtrees["tools/vc/Vc7/PlatformSDK"][0], 1136)
        self.assertEqual(len(manifest.subtrees), 14)   # the combined row is not a subtree
        self.assertEqual(len(manifest.files), 15)

    def test_synthetic_document(self):
        manifest = vt.parse_manifest(fake_manifest_text())
        self.assertEqual(manifest.binaries["tools/vc/Vc7/bin/cl.exe"], sha(b"cl"))
        self.assertEqual(manifest.subtrees, {})

    def test_missing_section_fails_closed(self):
        with self.assertRaises(vt.ManifestError):
            vt.parse_manifest("# nothing here\n")

    def test_missing_cl_pin_fails_closed(self):
        files = {k: v for k, v in FAKE_FILES.items() if not k.endswith("cl.exe")}
        with self.assertRaises(vt.ManifestError):
            vt.parse_manifest(fake_manifest_text(files))

    def test_dxsdk_manifest(self):
        entries = vt.parse_dxsdk_manifest(f"{'a' * 64}    88071  Include/d3d.h\nnot a row\n")
        self.assertEqual(entries, {"Include/d3d.h": "a" * 64})
        real = vt.parse_dxsdk_manifest(vt.DXSDK_MANIFEST.read_text(encoding="utf-8"))
        self.assertEqual(len(real), 62)


class VerifyTests(unittest.TestCase):
    def setUp(self):
        self.scratch = tempfile.TemporaryDirectory()
        self.root = Path(self.scratch.name)
        for path, data in FAKE_FILES.items():
            write(self.root / path, data)
        self.manifest = vt.parse_manifest(fake_manifest_text())

    def tearDown(self):
        self.scratch.cleanup()

    def test_pass(self):
        report = vt.verify(vt.Roots(vc71=self.root / "vc"), self.manifest)
        self.assertTrue(report.ok, report.errors)
        self.assertEqual(report.cl_sha256, sha(b"cl"))
        self.assertEqual(report.roots.vcruntime, (self.root / "vcruntime").resolve())
        self.assertEqual(report.checked["tools/vc/Vc7/bin/c1xx.dll"], "ok")

    def test_tools_root_layout(self):
        report = vt.verify(vt.Roots.from_tools_root(self.root), self.manifest)
        self.assertTrue(report.ok, report.errors)

    def test_missing_file(self):
        (self.root / "vc/Vc7/bin/c2.dll").unlink()
        report = vt.verify(vt.Roots(vc71=self.root / "vc"), self.manifest)
        self.assertFalse(report.ok)
        self.assertEqual(report.checked["tools/vc/Vc7/bin/c2.dll"], "missing")
        self.assertIn("missing tools/vc/Vc7/bin/c2.dll", report.errors[0])

    def test_wrong_digest(self):
        write(self.root / "vc/Vc7/bin/cl.exe", b"a service-pack compiler")
        report = vt.verify(vt.Roots(vc71=self.root / "vc"), self.manifest)
        self.assertFalse(report.ok)
        self.assertEqual(report.checked["tools/vc/Vc7/bin/cl.exe"], "mismatch")
        self.assertIn(sha(b"cl"), report.errors[0])
        self.assertEqual(report.cl_sha256, "")

    def test_not_a_vc_root(self):
        report = vt.verify(vt.Roots(vc71=self.root / "elsewhere"), self.manifest)
        self.assertFalse(report.ok)
        self.assertIn("not a Visual Studio .NET 2003 root", report.errors[0])

    def test_listing_digest_matches_sha256sum_format(self):
        include = self.root / "vc/Vc7/include"
        write(include / "b.h", b"b")
        write(include / "sub/a.h", b"a")
        lines = sorted([f"{sha(b'b')}  tools/vc/Vc7/include/b.h\n".encode(),
                        f"{sha(b'a')}  tools/vc/Vc7/include/sub/a.h\n".encode()])
        self.assertEqual(vt.listing_digest(include, "tools/vc/Vc7/include"), (2, sha(b"".join(lines))))

    def test_subtree_difference_warns_or_fails_when_strict(self):
        include = self.root / "vc/Vc7/include"
        write(include / "a.h", b"a")
        count, digest = vt.listing_digest(include, "tools/vc/Vc7/include")
        exact = vt.parse_manifest(fake_manifest_text(subtrees=f"| `tools/vc/Vc7/include` | {count} | `{digest}` |"))
        self.assertEqual(vt.verify(vt.Roots(vc71=self.root / "vc"), exact).checked["tools/vc/Vc7/include"], "ok")
        write(include / "extra.h", b"x")
        loose = vt.verify(vt.Roots(vc71=self.root / "vc"), exact)
        self.assertTrue(loose.ok)
        self.assertEqual(loose.checked["tools/vc/Vc7/include"], "differs")
        self.assertFalse(vt.verify(vt.Roots(vc71=self.root / "vc"), exact, strict=True).ok)

    def test_overlay_files_are_required_once_the_root_is_given(self):
        files = dict(FAKE_FILES)
        text = fake_manifest_text(files).replace(
            "| --- | --- |\n\n## Directory layout",
            f"| --- | --- |\n| `tools/overlay/base/stl/_list.h` | `{sha(b'list')}` |\n\n## Directory layout")
        manifest = vt.parse_manifest(text)
        self.assertTrue(vt.verify(vt.Roots(vc71=self.root / "vc"), manifest).ok)   # not given: skipped
        report = vt.verify(vt.Roots(vc71=self.root / "vc", overlay=self.root / "overlay"), manifest)
        self.assertFalse(report.ok)
        write(self.root / "overlay/base/stl/_list.h", b"list")
        self.assertTrue(vt.verify(vt.Roots(vc71=self.root / "vc", overlay=self.root / "overlay"), manifest).ok)

    def test_compiler_inside_checkout_is_refused(self):
        with mock.patch.object(vt, "REPO_ROOT", self.root):
            report = vt.verify(vt.Roots(vc71=self.root / "vc"), self.manifest)
        self.assertFalse(report.ok)
        self.assertIn("inside this checkout", report.errors[-1])

    def test_main_exit_codes(self):
        config = self.root / "cfg" / "toolchain.json"
        with mock.patch.object(vt, "load_manifest", return_value=self.manifest), \
                mock.patch("sys.stdout", new_callable=io.StringIO):
            self.assertEqual(vt.main(["--vc71-root", str(self.root / "vc"), "--config", str(config)]), 0)
            self.assertTrue(config.is_file())
            write(self.root / "vc/Vc7/bin/c1.dll", b"tampered")
            self.assertEqual(vt.main(["--vc71-root", str(self.root / "vc"), "--no-save"]), 1)
        with mock.patch("sys.stderr", new_callable=io.StringIO):
            self.assertEqual(vt.main([]), 2)


class ConfigTests(unittest.TestCase):
    def test_locations(self):
        self.assertEqual(vt.config_path({"LOCALAPPDATA": r"C:\Users\me\AppData\Local"}, "nt"),
                         Path(r"C:\Users\me\AppData\Local") / "bfme2-rebuild" / "toolchain.json")
        self.assertEqual(vt.config_path({}, "posix", home="/u/me"),
                         Path("/u/me/.config/bfme2-rebuild/toolchain.json"))
        self.assertEqual(vt.config_path({"XDG_CONFIG_HOME": "/xdg"}, "posix"),
                         Path("/xdg/bfme2-rebuild/toolchain.json"))
        self.assertEqual(vt.config_path({vt.CONFIG_ENV: "/custom.json", "LOCALAPPDATA": "x"}, "nt"),
                         Path("/custom.json"))

    def test_round_trip(self):
        with tempfile.TemporaryDirectory() as scratch:
            report = vt.Report(vt.Roots(vc71=Path(scratch) / "vc"), cl_sha256=CL_SHA)
            path = Path(scratch) / "deep" / "toolchain.json"
            vt.save_config(report, path, demangler="/usr/local/bin/demangle")
            data = vt.load_config(path)
            self.assertEqual(data["cl_sha256"], CL_SHA)
            self.assertEqual(data["demangler"], "/usr/local/bin/demangle")
            self.assertEqual(vt.roots_from_config(data).vc71, Path(scratch) / "vc")

    def test_refuses_checkout_location_and_foreign_files(self):
        report = vt.Report(vt.Roots(vc71=Path("/vc")))
        with self.assertRaises(ValueError):
            vt.save_config(report, vt.REPO_ROOT / "toolchain.json")
        with tempfile.TemporaryDirectory() as scratch:
            other = Path(scratch) / "other.json"
            other.write_text(json.dumps({"version": 99}), encoding="utf-8")
            with self.assertRaises(ValueError):
                vt.load_config(other)


class BackendCommandTests(unittest.TestCase):
    CONFIG = {"roots": {"vc71": r"C:\VS2003", "runner": r"D:\open-bfme-2"}, "demangler": "/opt/demangle"}

    def test_wsl_paths(self):
        self.assertEqual(backends.wsl_path(r"C:\Users\me\repo"), "/mnt/c/Users/me/repo")
        self.assertEqual(backends.wsl_path(r"\\wsl$\Ubuntu\opt\vc"), "/opt/vc")
        self.assertEqual(backends.wsl_path(r"\\wsl.localhost\Ubuntu\opt"), "/opt")
        self.assertEqual(backends.wsl_path("/already/posix"), "/already/posix")
        with self.assertRaises(ValueError):
            backends.wsl_path(r"\\server\share\x")

    def test_judge_environment(self):
        env = backends.judge_environment(self.CONFIG, repo_git_dir=r"C:\repo\.git", retail=r"C:\repo\game.dat",
                                         convert=backends.wsl_path)
        self.assertEqual(env["VC71_ROOT"], "/mnt/c/VS2003")
        self.assertEqual(env["BFME_JUDGE_RUNNER"], "/mnt/d/open-bfme-2")
        self.assertEqual(env["BFME_JUDGE_REPO"], "/mnt/c/repo/.git")
        self.assertEqual(env["LC_ALL"], "C")
        self.assertNotIn("BFME_JUDGE_STATE", env)

    def test_host_command(self):
        command = backends.host_judge_command("abc", ["src/a.cpp"], python="python3", repo_root="/r")
        self.assertEqual(command, ["python3", str(Path("/r") / "tools/judge/bin/judge.py"), "--json", "abc",
                                   "src/a.cpp"])

    def test_wsl_command(self):
        command = backends.wsl_judge_command("abc", ["src/a.cpp"], {"B": "2", "A": "1"},
                                             repo_root=r"C:\repo")
        self.assertEqual(command[:5], ["wsl.exe", "-e", "env", "A=1", "B=2"])
        self.assertIn("wineboot -i", command[command.index("-c") + 1])
        self.assertEqual(command[-4:], ["/mnt/c/repo/tools/judge/bin/judge.py", "--json", "abc", "src/a.cpp"])

    def test_docker_command(self):
        command = backends.docker_judge_command("abc", ["src/a.cpp"], self.CONFIG, repo_root=r"C:\repo",
                                                git_common_dir=r"C:\repo\.git")
        text = " ".join(command)
        self.assertEqual(command[:5], ["docker", "run", "--rm", "--network", "none"])
        self.assertIn(r"type=bind,source=C:\VS2003,target=/opt/vc71,readonly", text)
        self.assertIn(r"type=bind,source=C:\repo,target=/work,readonly", text)
        self.assertIn(r"type=bind,source=C:\repo\.git,target=/git,readonly", text)
        self.assertIn(r"type=bind,source=D:\open-bfme-2,target=/opt/runner,readonly", text)
        self.assertIn("type=volume,source=bfme2-judge-state,target=/state", text)
        self.assertEqual(command[command.index("bfme2-local-check"):], ["bfme2-local-check", "--json", "abc",
                                                                       "src/a.cpp"])

    def test_docker_build_uses_tools_context(self):
        command = backends.docker_build_command(repo_root="/r")
        self.assertEqual(command[-1], str(Path("/r/tools")))
        self.assertEqual(command[command.index("-f") + 1], str(Path("/r/tools/local_check/Dockerfile")))

    def test_default_backend(self):
        none = lambda name: None  # noqa: E731
        has = lambda name: "/usr/bin/" + name  # noqa: E731
        self.assertEqual(backends.default_backend(compile_only=True, os_name="nt", which=none), "native")
        self.assertEqual(backends.default_backend(compile_only=False, os_name="nt", which=has), "wsl")
        self.assertEqual(backends.default_backend(compile_only=False, os_name="nt", which=none), "docker")
        self.assertEqual(backends.default_backend(compile_only=False, os_name="posix", which=has), "host")
        self.assertEqual(backends.default_backend(compile_only=False, os_name="posix", which=none), "docker")
        self.assertEqual(backends.default_backend(compile_only=True, os_name="posix", which=has), "wine")


class CompilerBackendTests(unittest.TestCase):
    """The judge's own compile rules, executed for native cl.exe and for wine cl.exe."""

    def setUp(self):
        self.scratch = tempfile.TemporaryDirectory()
        root = Path(self.scratch.name)
        self.roots = vt.Roots(vc71=root / "vc", vcruntime=root / "vcruntime", stlport46=root / "stlport-4.6",
                              overlay=root / "overlay", dxsdk=root / "dx")
        (root / "vc/Vc7/PlatformSDK/Include").mkdir(parents=True)
        self.root = root

    def tearDown(self):
        self.scratch.cleanup()

    def backend(self, kind):
        return backends.CompilerBackend(kind, self.roots, overlay=self.root / "overlay")

    def test_rule_loader_refuses_drift(self):
        with tempfile.TemporaryDirectory() as scratch:
            stale = Path(scratch) / "judge.py"
            stale.write_text("BASE = []\ndef compiler_base(text):\n    return BASE\n", encoding="utf-8")
            with self.assertRaises(RuntimeError) as caught:
                backends.load_judge_rules({}, stale)
            self.assertIn("compile_settings", str(caught.exception))

    def test_native_command_default_stlport46(self):
        backend = self.backend("native")
        argv, env = backend.command("src/Game/A.cpp", "// FUNCTION: BFME2 0x1\nint a;\n", self.root / "o.obj")
        self.assertEqual(argv[0], str(self.root / "vc/Vc7/bin/cl.exe"))
        self.assertEqual(argv[1:13], backend.rules["BASE"])
        self.assertEqual(argv[13], "-showIncludes")
        self.assertIn("-D_STLP_USE_STATIC_LIB", argv)
        self.assertIn("-I" + str(self.root / "overlay" / "base"), argv)
        self.assertIn("-I" + str(self.root / "stlport-4.6" / "stlport"), argv)
        self.assertEqual(argv[-3:], ["-Iinclude", "-Fo" + str(self.root / "o.obj"), r"src\Game\A.cpp"])
        self.assertEqual(env["INCLUDE"], str(self.root / "vc/Vc7/include"))
        self.assertTrue(env["PATH"].startswith(str(self.root / "vc/Vc7/bin")))
        self.assertIn(str(self.root / "vcruntime"), env["PATH"])
        self.assertNotIn("WINEPATH", env)

    def test_wine_command_matches_boat_spelling(self):
        backend = self.backend("wine")
        text = "// cl-arch: x87\n// msvcstl\n// psdk\n// cl: /DFOO -W3\n"
        argv, env = backend.command("src/zlib-1.1.4/x.c", text, self.root / "o.obj")
        self.assertEqual(argv[0], "wine")
        self.assertNotIn("-arch:SSE", argv)
        self.assertIn("-DFOO", argv)
        self.assertIn("-W3", argv)
        self.assertIn("-I" + backends.wine_z_path((self.root / "vc/Vc7/PlatformSDK/Include").resolve()), argv)
        self.assertTrue(argv[-2].startswith("-FoZ:"))
        self.assertEqual(env["WINEDEBUG"], "-all")
        self.assertEqual(env["WINEDLLOVERRIDES"], "mscoree,mshtml=")
        self.assertEqual(env["WINEPATH"].count(";"), 2)

    def test_upstream_profile_and_refused_flag(self):
        backend = self.backend("native")
        argv, _ = backend.command("src/zlib-1.1.4/x.c", "// cl-profile: upstream\n", self.root / "o.obj")
        self.assertEqual(argv[1:4], ["-nologo", "-c", "-Gy"])
        with self.assertRaises(ValueError):
            backend.command("src/a.cpp", "// msvcstl\n// cl: /Fe:evil\n", self.root / "o.obj")

    def test_environment_drops_option_variables(self):
        backend = self.backend("native")
        env = backend.environment(base={"CL": "/Od", "_CL_": "/X", "PATH": "p", "KEEP": "1"})
        self.assertNotIn("CL", env)
        self.assertNotIn("_CL_", env)
        self.assertEqual(env["KEEP"], "1")

    def test_compile_unit_reports_errors_and_success(self):
        backend = self.backend("native")
        job = self.root / "job"
        write(job / "src/a.cpp", b"// msvcstl\nint a;\n")

        def failing(argv, **kwargs):
            return subprocess.CompletedProcess(argv, 2, b"src\\a.cpp(2) : error C2065: 'x' : undeclared\n")

        def succeeding(argv, **kwargs):
            write(Path(argv[-2][3:]), b"obj")
            return subprocess.CompletedProcess(argv, 0, b"")

        self.assertEqual(backends.compile_unit(backend, job, "src/a.cpp", run=failing),
                         (False, ["src\\a.cpp(2) : error C2065: 'x' : undeclared"]))
        self.assertEqual(backends.compile_unit(backend, job, "src/a.cpp", run=succeeding), (True, []))


def judge_doc(rows, *, error=None, tu="src/a.cpp"):
    item = {"tu": tu, "rows": rows, "gates": [], "expected": [], "verification_blockers": [],
            "object_verified": False}
    if error:
        item["error"] = error
    return {"summary": {}, "results": [item]}


class OrchestrationTests(unittest.TestCase):
    def run_main(self, *, gate_status="pass", base_rows=None, head_rows=None, head_error=None, refusals=(),
                 extra=()):
        calls = []
        plan = precheck.Plan("b", "h", changed=["src/a.cpp"], affected=["src/a.cpp"], refusals=list(refusals))

        def judge_factory(backend, config, repo, image):
            def judge(revision, tus):
                calls.append((backend, revision, tuple(tus)))
                return judge_doc(base_rows if len(calls) == 1 else head_rows,
                                 error=None if len(calls) == 1 else head_error)
            return judge

        with mock.patch.object(precheck, "check_local_main"), \
                mock.patch("sys.stdout", new_callable=io.StringIO):
            code = precheck.main(["--backend", "host", "--base", "HEAD", *extra],
                                 gate=lambda repo, base, head: (gate_status, "report"),
                                 planner=lambda repo, base, head: plan, judge_factory=judge_factory,
                                 toolchain=lambda path: {"roots": {"vc71": "/vc"}})
        return code, calls

    def test_gate_failure_stops_before_judging(self):
        code, calls = self.run_main(gate_status="fail")
        self.assertEqual(code, 1)
        self.assertEqual(calls, [])

    def test_intake_refusal_stops_before_judging(self):
        code, calls = self.run_main(refusals=[("new_tu", "src/b.cpp")])
        self.assertEqual(code, 1)
        self.assertEqual(calls, [])

    def test_no_regression_passes(self):
        rows = [{"rva": "0x401000", "status": "MATCH", "name": "A::f"}]
        code, calls = self.run_main(base_rows=[{"rva": "0x401000", "status": "PARTIAL", "pct": 50.0}],
                                    head_rows=rows)
        self.assertEqual(code, 0)
        head = precheck.git(precheck.REPO_ROOT, "rev-parse", "HEAD")
        self.assertEqual(calls, [("host", head, ("src/a.cpp",)), ("host", head, ("src/a.cpp",))])

    def test_lost_match_fails(self):
        code, _ = self.run_main(base_rows=[{"rva": "0x401000", "status": "MATCH"}],
                                head_rows=[{"rva": "0x401000", "status": "PARTIAL", "pct": 90.0}])
        self.assertEqual(code, 1)

    def test_new_compile_error_fails(self):
        code, _ = self.run_main(base_rows=[{"rva": "0x401000", "status": "MATCH"}],
                                head_rows=[{"rva": "0x401000", "status": "COMPILE_ERROR"}],
                                head_error="error C2065")
        self.assertEqual(code, 1)

    def test_json_report(self):
        with tempfile.TemporaryDirectory() as scratch:
            out = Path(scratch) / "r.json"
            code, _ = self.run_main(base_rows=[{"rva": "0x401000", "status": "MATCH"}],
                                    head_rows=[{"rva": "0x401000", "status": "MATCH"}], extra=["--json", str(out)])
            self.assertEqual(code, 0)
            data = json.loads(out.read_text(encoding="utf-8"))
            self.assertEqual(data["rules"], [])
            self.assertEqual(data["functions"][0]["label"], "PASS")

    def test_setup_error_exit_code(self):
        def missing(path):
            raise precheck.SetupError("no toolchain configuration")

        with mock.patch("sys.stdout", new_callable=io.StringIO), \
                mock.patch("sys.stderr", new_callable=io.StringIO):
            code = precheck.main(["--backend", "host", "--base", "HEAD"], gate=lambda *a: ("pass", ""),
                                 planner=lambda *a: precheck.Plan("b", "h", ["src/a.cpp"], ["src/a.cpp"]),
                                 toolchain=missing)
        self.assertEqual(code, 2)

    def test_verdict_labels(self):
        verdict = precheck.judge_verdict(
            judge_doc([{"rva": "0x1", "status": "MATCH"}, {"rva": "0x2", "status": "PARTIAL", "pct": 10.0},
                       {"rva": "0x3", "status": "MATCH"}]),
            judge_doc([{"rva": "0x1", "status": "MATCH"}, {"rva": "0x2", "status": "MATCH"},
                       {"rva": "0x3", "status": "MISSING"}]),
            ["src/a.cpp"])
        labels = {row["rva"]: row["label"] for row in verdict["functions"]}
        self.assertEqual(labels, {"0x1": "PASS", "0x2": "PASS (gained)", "0x3": "MISMATCH (lost MATCH)"})
        self.assertIn("regression", verdict["rules"])
        self.assertEqual(verdict["counts"]["gained"], 1)
        self.assertEqual(verdict["counts"]["lost_credit"], 1)

    def test_host_runner_uses_a_minimal_environment(self):
        with tempfile.TemporaryDirectory() as scratch:
            seen = []

            def run(argv, **kwargs):
                seen.append((argv, kwargs))
                return subprocess.CompletedProcess(argv, 0, json.dumps({"summary": {}, "results": []}), "")

            config = {"roots": {"vc71": "/vc", "runner": "/runner"}, "demangler": "/d"}
            with mock.patch.dict(os.environ, {"BFME_JUDGE_STATE": scratch, "SOME_DISPATCH_SETTING": "1"}):
                runner = precheck.JudgeRunner("host", config, precheck.REPO_ROOT, run=run)
                self.assertEqual(runner("abc", ["src/a.cpp"]), {"summary": {}, "results": []})
            # A fresh state directory: the Wine prefix is initialized first, then the judge runs.
            self.assertEqual([call[0][0] for call in seen[:2]], ["wineboot", "wineserver"])
            self.assertEqual(Path(seen[0][1]["env"]["WINEPREFIX"]), Path(scratch) / "wine")
            argv, kwargs = seen[2]
            self.assertEqual(argv[2:], ["--json", "abc", "src/a.cpp"])
            self.assertNotIn("SOME_DISPATCH_SETTING", kwargs["env"])
            self.assertEqual(kwargs["env"]["BFME_JUDGE_DEMANGLER"], "/d")
            self.assertEqual(Path(kwargs["env"]["BFME_JUDGE_WINEPREFIX"]), Path(scratch) / "wine")

    def test_parse_judge_output(self):
        doc = {"summary": {}, "results": []}
        self.assertEqual(precheck.parse_judge_output("[farm] note\n" + json.dumps(doc) + "\n"), doc)
        with self.assertRaises(precheck.SetupError):
            precheck.parse_judge_output("no json")

    def test_compile_only_regression(self):
        plan = precheck.Plan("b", "h", ["src/a.cpp"], ["src/a.cpp", "src/b.cpp"])
        outcomes = {("base", "src/a.cpp"): (True, []), ("head", "src/a.cpp"): (False, ["error C2065"]),
                    ("base", "src/b.cpp"): (False, ["old"]), ("head", "src/b.cpp"): (False, ["old"])}
        verdict = precheck.compile_verdict(
            None, "/repo", plan, export=lambda repo, revision, job: None,
            compile_unit=lambda backend, job, tu: outcomes[(Path(job).name, tu)])
        self.assertEqual(verdict["regressions"], ["src/a.cpp"])
        self.assertEqual(verdict["rules"], ["compile"])
        self.assertEqual(verdict["units"]["src/b.cpp"]["base"], "COMPILE ERROR")


class PlanTests(unittest.TestCase):
    """The include closure and refusals come from ci/managed-check/intake.py on a real Git repository."""

    def setUp(self):
        self.scratch = tempfile.TemporaryDirectory()
        self.repo = Path(self.scratch.name)
        self.git("init", "-q", "-b", "main")
        write(self.repo / "include/x.h", b"int x();\n")
        write(self.repo / "src/a.cpp", b'#include "x.h"\nint a;\n')
        write(self.repo / "src/b.cpp", b"int b;\n")
        write(self.repo / "expected/a.cpp.json", b"{}\n")
        write(self.repo / "expected/b.cpp.json", b"{}\n")
        self.base = self.commit("base")

    def tearDown(self):
        self.scratch.cleanup()

    def git(self, *args):
        return subprocess.run(["git", "-C", str(self.repo), "-c", "user.name=t", "-c", "user.email=t@t",
                               *args], capture_output=True, text=True, check=True).stdout.strip()

    def commit(self, message):
        self.git("add", "-A")
        self.git("commit", "-q", "-m", message)
        return self.git("rev-parse", "HEAD")

    def test_header_change_affects_includers(self):
        write(self.repo / "include/x.h", b"int x(int);\n")
        plan = precheck.plan_changes(self.repo, self.base, self.commit("head"))
        self.assertEqual(plan.changed, ["include/x.h"])
        self.assertEqual(plan.affected, ["src/a.cpp"])
        self.assertEqual(plan.refusals, [])
        self.assertEqual(plan.route, "compile")

    def test_new_unit_is_refused(self):
        write(self.repo / "src/c.cpp", b"int c;\n")
        plan = precheck.plan_changes(self.repo, self.base, self.commit("head"))
        self.assertIn(("new_tu", "src/c.cpp"), plan.refusals)

    def test_non_source_paths_are_left_to_the_gate(self):
        write(self.repo / "docs/note.md", b"x\n")
        plan = precheck.plan_changes(self.repo, self.base, self.commit("head"))
        self.assertEqual((plan.changed, plan.affected), ([], []))


if __name__ == "__main__":
    unittest.main()
