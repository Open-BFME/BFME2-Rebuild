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
import time
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
        write(include / "a.h", b"a")
        write(include / "sub/b.h", b"b")
        # Ordered by path (find | LC_ALL=C sort -z | xargs -0 sha256sum), never by digest:
        # sha256(b"a") = ca97... sorts after sha256(b"b") = 3e23..., so a line sort would swap these two.
        lines = [f"{sha(b'a')}  tools/vc/Vc7/include/a.h\n".encode(),
                 f"{sha(b'b')}  tools/vc/Vc7/include/sub/b.h\n".encode()]
        self.assertGreater(lines[0], lines[1])
        self.assertEqual(vt.listing_digest(include, "tools/vc/Vc7/include"), (2, sha(b"".join(lines))))

    def test_real_compiler_bin_listing(self):
        """The four pinned binaries alone reproduce the manifest's tools/vc/Vc7/bin listing digest."""
        manifest = vt.load_manifest()
        names = sorted(path for path in manifest.binaries if path.startswith("tools/vc/Vc7/bin/"))
        listing = "".join(f"{manifest.binaries[path]}  {path}\n" for path in names).encode()
        self.assertEqual(manifest.subtrees["tools/vc/Vc7/bin"], (4, sha(listing)))

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
        self.assertEqual(command[5:7], ["-e", "VC71_ROOT=/opt/toolchain/vs/vc71"])
        self.assertIn(r"type=bind,source=C:\VS2003,target=/opt/toolchain/vs/vc71,readonly", text)
        self.assertIn(r"type=bind,source=C:\repo,target=/work,readonly", text)
        self.assertIn(r"type=bind,source=C:\repo\.git,target=/git,readonly", text)
        self.assertIn(r"type=bind,source=D:\open-bfme-2,target=/opt/runner,readonly", text)
        self.assertIn("type=volume,source=bfme2-judge-state,target=/state", text)
        self.assertEqual(command[command.index("bfme2-local-check"):], ["bfme2-local-check", "--json", "abc",
                                                                       "src/a.cpp"])

    def test_docker_command_mounts_runtime_dlls_where_judge_looks(self):
        """judge.py's environment adds VC71_ROOT.parents[1] to WINEPATH; msvcp71.dll must be there."""
        config = {"roots": {**self.CONFIG["roots"], "vcruntime": r"C:\vs2003"}}
        command = backends.docker_judge_command("abc", ["src/a.cpp"], config, repo_root=r"C:\repo",
                                                git_common_dir=r"C:\repo\.git", docker="podman",
                                                mounts=[(r"C:\objects", "/objects")])
        text = " ".join(command)
        self.assertEqual(command[0], "podman")
        self.assertEqual(Path(backends.VC71_MOUNT).parents[1].as_posix(), backends.VCRUNTIME_MOUNT)
        for name in ("msvcp71.dll", "msvcr71.dll"):
            self.assertIn(f"source={Path(r'C:\vs2003') / name},target=/opt/toolchain/{name},readonly", text)
        self.assertIn(r"type=bind,source=C:\objects,target=/objects,readonly", text)

    def test_diff_arguments_replace_json(self):
        self.assertEqual(backends.judge_arguments("abc", ["src/a.cpp"], ("--diff", "0x10")),
                         ["--diff", "0x10", "abc", "src/a.cpp"])
        command = backends.host_judge_command("abc", ["src/a.cpp"], python="python3", repo_root="/r",
                                              extra=("--diff", "0x10"))
        self.assertNotIn("--json", command)

    def test_docker_build_uses_tools_context(self):
        command = backends.docker_build_command(repo_root=backends.REPO_ROOT)
        self.assertEqual(command[-1], str(backends.REPO_ROOT / "tools"))
        self.assertEqual(command[command.index("-f") + 1], str(backends.REPO_ROOT / "tools/local_check/Dockerfile"))
        self.assertEqual(command[command.index("--label") + 1],
                         f"{backends.IMAGE_LABEL}={backends.image_digest()}")

    def test_wsl_script_drops_tmpdir_for_wine(self):
        script = backends._WSL_SCRIPT
        self.assertIn("unset TMPDIR", script)
        self.assertLess(script.index('PATH="$d:$PATH"'), script.index('exec python3'))
        dockerfile = (backends.REPO_ROOT / "tools/local_check/Dockerfile").read_text(encoding="utf-8")
        self.assertIn("unset TMPDIR", dockerfile)
        self.assertIn("ENV VC71_ROOT=" + backends.VC71_MOUNT, dockerfile)

    def test_container_engine(self):
        self.assertEqual(backends.container_engine({}, which=lambda n: "/x" if n == "podman" else None), "podman")
        self.assertEqual(backends.container_engine({}, which=lambda n: "/x"), "docker")
        self.assertEqual(backends.container_engine({"BFME_CONTAINER_ENGINE": "nerdctl"}, which=lambda n: None),
                         "nerdctl")

    def test_default_backend(self):
        none = lambda name: None  # noqa: E731
        has = lambda name: "/usr/bin/" + name  # noqa: E731
        self.assertEqual(backends.default_backend(compile_only=True, os_name="nt", which=none), "native")
        self.assertEqual(backends.default_backend(compile_only=False, os_name="nt", which=has,
                                                  wsl_wine=lambda: True), "wsl")
        self.assertEqual(backends.default_backend(compile_only=False, os_name="nt", which=has,
                                                  wsl_wine=lambda: False), "docker")
        wsl_only = lambda name: "/w" if name.startswith("wsl") else None  # noqa: E731
        self.assertEqual(backends.default_backend(compile_only=False, os_name="nt", which=wsl_only,
                                                  wsl_wine=lambda: False), "docker")
        self.assertEqual(backends.default_backend(compile_only=False, os_name="nt", which=wsl_only), "wsl")
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


class GitRepoCase(unittest.TestCase):
    """A scratch Git repository with one unit, its expected record and a minimal retail image."""

    def setUp(self):
        self.scratch = tempfile.TemporaryDirectory()
        self.repo = Path(self.scratch.name) / "repo"
        self.repo.mkdir()
        self.git("init", "-q", "-b", "main")
        write(self.repo / "src/a.cpp", b"// FUNCTION: BFME2 0x1000\nint f() { return 1; }\n")
        write(self.repo / "expected/a.cpp.json", json.dumps({"rows": [
            {"rva": "0x00001000", "size": 4, "section": ".text", "role": "owned", "name": "?f@@YAHXZ"},
            {"rva": "0x00001004", "size": 2, "section": ".text", "role": "comdat", "name": "_g"},
            {"rva": "0x00002000", "size": 4, "section": ".data", "role": "data", "name": "_d"}]}).encode())
        write(self.repo / "config/map.json", json.dumps({
            "objects": [{"object_id": "O1", "tu": "src/a.cpp"}, {"object_id": "O2", "tu": "src/b.cpp"}],
            "rows": [{"rva": "0x00001006", "size": 2, "section": ".text", "object": "O1", "name": "_owned"},
                     {"rva": "0x00001000", "size": 4, "section": ".text", "object": "O1", "name": "?f@@YAHXZ"},
                     {"rva": "0x00001008", "size": 2, "section": ".text", "object": "O2", "name": "_other"}]}).encode())
        write(self.repo / "baseline/game.dat", make_pe(b"\xb8\x01\x00\x00\x00\xc3\x90\x90" + bytes(0x1F8)))
        self.head = self.commit("base")

    def tearDown(self):
        self.scratch.cleanup()

    def git(self, *args):
        return subprocess.run(["git", "-C", str(self.repo), "-c", "user.name=t", "-c", "user.email=t@t",
                               *args], capture_output=True, text=True, check=True).stdout.strip()

    def commit(self, message):
        self.git("add", "-A")
        self.git("commit", "-q", "-m", message)
        return self.git("rev-parse", "HEAD")


def make_pe(code, rva=0x1000):
    """A minimal PE32 image with one .text section at RVA, enough for binary_model.PE."""
    import struct
    pe, optional_size, raw = 0x40, 224, 0x200
    data = bytearray(raw + len(code))
    struct.pack_into("<I", data, 0x3C, pe)
    data[pe:pe + 4] = b"PE\0\0"
    struct.pack_into("<HHIIIHH", data, pe + 4, 0x14C, 1, 0, 0, 0, optional_size, 0x102)
    struct.pack_into("<H", data, pe + 24, 0x10B)
    struct.pack_into("<I", data, pe + 24 + 28, 0x400000)
    section = pe + 24 + optional_size
    struct.pack_into("<8sIIIIIIHHI", data, section, b".text", len(code), rva, len(code), raw, 0, 0, 0, 0,
                     0x60000020)
    data[raw:] = code
    return bytes(data)


import bootstrap  # noqa: E402
import objdiff_project  # noqa: E402
import parity  # noqa: E402
import viewer  # noqa: E402


class BootstrapTests(unittest.TestCase):
    def test_sources_are_pinned_public_commits(self):
        for source in bootstrap.SOURCES:
            self.assertRegex(source.commit, r"^[0-9a-f]{40}$")
            self.assertTrue(source.url.startswith("https://github.com/Open-BFME/"))
            self.assertTrue(all(pattern.startswith("/") for pattern in source.patterns))
        self.assertEqual(bootstrap.SUPPORT.commit, vt.runner_revision())

    def test_sparse_patterns_cover_every_pinned_binary(self):
        manifest = vt.load_manifest()
        layout = bootstrap.Layout(Path("/cache"))
        roots = vt.Roots(vc71=layout.vc71, vcruntime=layout.vcruntime)
        for path in manifest.binaries:
            local = vt.local_path(roots, path).as_posix()
            relative = local[len(layout.compiler_checkout.as_posix()):]
            self.assertTrue(any(relative == p or (p.endswith("/") and relative.startswith(p))
                                for p in bootstrap.COMPILER.patterns), path)

    def test_layout_puts_the_runtime_where_the_judge_looks(self):
        """judge.py's compiler environment adds VC71_ROOT.parents[1] to the DLL path (msvcp71.dll)."""
        layout = bootstrap.Layout(Path("/cache"))
        self.assertEqual(layout.vc71.parents[1], layout.vcruntime)

    def test_cache_root(self):
        self.assertEqual(bootstrap.cache_root({bootstrap.CACHE_ENV: "/x"}), Path("/x"))
        self.assertEqual(bootstrap.cache_root({"LOCALAPPDATA": r"C:\L"}, os_name="nt"),
                         Path(r"C:\L") / "bfme2-rebuild" / "cache")
        self.assertEqual(bootstrap.cache_root({}, os_name="posix", platform="darwin", home="/h"),
                         Path("/h/Library/Caches/bfme2-rebuild"))
        self.assertEqual(bootstrap.cache_root({"XDG_CACHE_HOME": "/xdg"}, os_name="posix", platform="linux"),
                         Path("/xdg/bfme2-rebuild"))

    def test_fetch_is_sparse_shallow_and_idempotent(self):
        with tempfile.TemporaryDirectory() as scratch:
            destination = Path(scratch) / "open-bfme-1"
            seen = []

            def run(argv, **kwargs):
                seen.append((argv, kwargs.get("input")))
                out = bootstrap.COMPILER.commit + "\n" if "rev-parse" in argv else ""
                return subprocess.CompletedProcess(argv, 0, out, "")

            self.assertTrue(bootstrap.fetch(bootstrap.COMPILER, destination, run=run, log=lambda *a: None))
            fetch = next(argv for argv, _ in seen if "fetch" in argv)
            self.assertIn("--depth", fetch)
            self.assertIn("--filter=blob:none", fetch)
            self.assertEqual(fetch[-1], bootstrap.COMPILER.commit)
            self.assertIn("core.autocrlf=false", " ".join(fetch))
            sparse = next(text for argv, text in seen if "sparse-checkout" in argv)
            self.assertEqual(sparse.split(), [p for p in " ".join(bootstrap.COMPILER.patterns).split()])
            seen.clear()
            self.assertFalse(bootstrap.fetch(bootstrap.COMPILER, destination, run=run, log=lambda *a: None))
            self.assertEqual(len(seen), 1)

    def test_fetch_failure_is_a_setup_error(self):
        with tempfile.TemporaryDirectory() as scratch:
            def run(argv, **kwargs):
                code = 128 if "fetch" in argv else 0
                return subprocess.CompletedProcess(argv, code, "", "fatal: unable to access")

            with self.assertRaises(bootstrap.BootstrapError):
                bootstrap.fetch(bootstrap.SUPPORT, Path(scratch) / "x", run=run, log=lambda *a: None)

    def test_run_bootstrap_verifies_and_writes_the_configuration(self):
        with tempfile.TemporaryDirectory() as scratch:
            cache = Path(scratch) / "cache"
            layout = bootstrap.Layout(cache.resolve())
            placed = {"vc/": layout.vc71, "vcruntime/": layout.vcruntime}

            def fetcher(source, destination, **kwargs):
                if source is bootstrap.COMPILER:
                    for path, data in FAKE_FILES.items():
                        prefix = next(p for p in placed if path.startswith(p))
                        write(placed[prefix] / path[len(prefix):], data)
                else:
                    (destination / "tools").mkdir(parents=True, exist_ok=True)

            overlay = Path(scratch) / "overlay"
            overlay.mkdir()
            with mock.patch.object(vt, "load_manifest", return_value=vt.parse_manifest(fake_manifest_text())), \
                    mock.patch.object(vt, "check_runner"):
                result = bootstrap.run_bootstrap(cache=cache, backend="host", config_file=Path(scratch) / "t.json",
                                                 fetcher=fetcher, overlay=lambda runner, run: overlay,
                                                 demangler_builder=lambda backend, layout, run: ("/d", "built"),
                                                 log=lambda *a: None)
            self.assertTrue(result.ok, result.report.errors)
            config = vt.load_config(Path(scratch) / "t.json")
            self.assertEqual(Path(config["roots"]["vc71"]), layout.vc71)
            self.assertEqual(Path(config["roots"]["vcruntime"]), layout.vcruntime)
            self.assertEqual(config["demangler"], "/d")
            self.assertEqual(config["bootstrap"]["sources"]["open-bfme-2"]["commit"], bootstrap.SUPPORT_REVISION)

    def test_cache_inside_the_checkout_is_refused(self):
        with self.assertRaises(bootstrap.BootstrapError):
            bootstrap.run_bootstrap(cache=bootstrap.REPO_ROOT / "cache", fetcher=None)

    def test_demangler_per_backend(self):
        layout = bootstrap.Layout(Path("/cache"))
        self.assertIsNone(bootstrap.build_demangler("docker", layout)[0])
        ok = lambda argv, **kw: subprocess.CompletedProcess(argv, 0, "/cache/bin/demangle\n", "")  # noqa: E731
        self.assertEqual(bootstrap.build_demangler("host", layout, run=ok), ("/cache/bin/demangle", "built"))
        bad = lambda argv, **kw: subprocess.CompletedProcess(argv, 3, "", "no llvm-config-18\n")  # noqa: E731
        path, note = bootstrap.build_demangler("wsl", layout, run=bad)
        self.assertIsNone(path)
        self.assertIn("llvm-18-dev", note)


class ObjdiffTests(GitRepoCase):
    def test_coff_round_trip(self):
        long_name = "?f@@YAHXZ_with_a_name_longer_than_eight"
        data = objdiff_project.coff_object([(long_name, b"\x55\x8b\xec\xc3"), ("_g", b"\xc3")])
        bodies = objdiff_project.function_bodies(data)
        self.assertEqual(bodies[long_name], (b"\x55\x8b\xec\xc3", []))
        self.assertEqual(bodies["_g"], (b"\xc3", []))

    def test_project_holds_retail_bytes_and_objdiff_config(self):
        directory = Path(self.scratch.name) / "objdiff"
        path = objdiff_project.write_project(directory, "src/a.cpp", repo=self.repo, revision=self.head)
        config = json.loads(path.read_text(encoding="utf-8"))
        self.assertEqual(config["$schema"], objdiff_project.SCHEMA_URL)
        unit = config["units"][0]
        self.assertEqual(unit["name"], "src/a.cpp")
        self.assertTrue(config["build_base"])
        self.assertFalse(config["build_target"])
        self.assertIn("build", config["custom_args"])
        bodies = objdiff_project.function_bodies((directory / unit["target_path"]).read_bytes())
        self.assertEqual(sorted(bodies), ["?f@@YAHXZ", "_g", "_owned"])   # ledger rows plus expected/ rows
        self.assertEqual(bodies["?f@@YAHXZ"][0], b"\xb8\x01\x00\x00")
        self.assertEqual(bodies["_g"][0], b"\x00\xc3")
        self.assertNotIn("_d", bodies)   # data rows stay out of the code comparison

    def test_masked_zeroes_relocated_operands(self):
        self.assertEqual(objdiff_project.masked(b"\xe8\x01\x02\x03\x04\xc3", [1]), b"\xe8\x00\x00\x00\x00\xc3")


class ViewerTests(unittest.TestCase):
    def test_side_by_side(self):
        rows = viewer.side_by_side(["0: a 1 push ebp", "1: b mov", "2: c ret"], ["0: a 1 push ebp", "1: x xor",
                                                                                  "2: c ret", "3: d nop"])
        self.assertEqual([r["kind"] for r in rows], ["same", "change", "same", "right"])

    def test_unified_to_rows(self):
        header, rows = viewer.unified_to_rows("--- retail 0x10\n+++ compiled f\n@@ -1,3 +1,3 @@\n a\n-b\n+c\n d\n")
        self.assertEqual(len(header), 2)
        self.assertEqual([r["kind"] for r in rows], ["gap", "same", "change", "same"])
        self.assertEqual((rows[2]["left"], rows[2]["right"]), ("b", "c"))

    def test_status_and_server(self):
        import urllib.request
        with tempfile.TemporaryDirectory() as scratch:
            status = viewer.Status(scratch, mode="unit")
            status.step("judge")
            status.unit("src/a.cpp", state="done", functions=[{"rva": "0x10", "label": "PASS", "name": "f"}])
            calls = []

            def provider(tu, rva):
                calls.append((tu, rva))
                return {"state": "ready", "rows": [], "source": "test"}

            server, url = viewer.serve(scratch, port=0, provider=provider)
            try:
                page = urllib.request.urlopen(url + "/").read().decode()
                self.assertIn("status.json", page)
                self.assertNotIn("http://", page.replace("http://localhost", ""))   # no CDN
                data = json.loads(urllib.request.urlopen(url + "/status.json").read())
                self.assertEqual(data["units"]["src/a.cpp"]["functions"][0]["label"], "PASS")
                self.assertEqual(data["steps"][0]["state"], "running")
                for _ in range(50):
                    diff = json.loads(urllib.request.urlopen(url + "/diff?tu=src/a.cpp&rva=0x10").read())
                    if diff["state"] != "pending":
                        break
                    time.sleep(0.05)
                self.assertEqual(diff["state"], "ready")
                self.assertEqual(calls, [("src/a.cpp", "0x10")])   # computed once, then cached
                self.assertEqual(server.server_address[0], "127.0.0.1")
            finally:
                server.shutdown()
                server.server_close()
            status.finish("done", "1/1 PASS")
            self.assertEqual(json.loads((Path(scratch) / "status.json").read_text())["state"], "done")

    def test_provisional_diffs_are_not_cached(self):
        with tempfile.TemporaryDirectory() as scratch:
            answers = [{"state": "ready", "rows": [], "provisional": True}, {"state": "ready", "rows": [1]}]
            diffs = viewer.Diffs(scratch, lambda tu, rva: answers.pop(0))

            def settle():
                for _ in range(100):
                    result = diffs.get("src/a.cpp", "0x10")
                    if result["state"] != "pending":
                        return result
                    time.sleep(0.02)

            self.assertEqual(settle()["rows"], [])     # the stand-in is served once ...
            self.assertEqual(settle()["rows"], [1])    # ... then recomputed and cached
            self.assertEqual(settle()["rows"], [1])
            self.assertEqual(answers, [])

    def test_object_diff_without_objdump_shows_bytes(self):
        with tempfile.TemporaryDirectory() as scratch:
            retail, base = Path(scratch) / "game.dat", Path(scratch) / "b.obj"
            retail.write_bytes(make_pe(b"\x55\x8b\xec\x5d\xc3" + bytes(11)))
            base.write_bytes(objdiff_project.coff_object([("_f", b"\x55\x8b\xec\x5d\xc3\xcc\xcc")]))
            with mock.patch.object(viewer, "objdump", return_value=None):
                diff = viewer.object_diff(retail, "0x1000", 5, base, "_f")
                unit = {"functions": [{"rva": "0x1000", "size": 5, "symbol": "_f"}], "base_obj": str(base)}
                self.assertEqual(viewer.row_diff(retail, unit, "0x1000")["rows"], diff["rows"])
                missing = viewer.object_diff(retail, "0x1000", 5, base, None)
            self.assertTrue(diff["equal"])
            self.assertEqual(diff["sizes"], [5, 5])
            self.assertEqual({row["kind"] for row in diff["rows"]}, {"same"})
            self.assertEqual(missing["source"], "retail only")


class ParityTests(GitRepoCase):
    def test_library_and_sample(self):
        self.assertEqual(parity.library("src/zlib-1.1.4/Compression/ZLib/trees.c"), "src/zlib-1.1.4")
        self.assertEqual(parity.library("src/Code/GameEngine/Source/Common/A.cpp"), "src/Code/GameEngine/Source")
        units = [f"src/lib{i % 4}/f{i}.c" for i in range(40)]
        picked = parity.sample(units, 8, seed=1)
        self.assertEqual(len(picked), 8)
        self.assertEqual({parity.library(tu) for tu in picked}, {f"src/lib{i}" for i in range(4)})
        self.assertEqual(picked, parity.sample(units, 8, seed=1))

    def test_agreement_through_compare_index(self):
        _intake, compare, *_ = precheck.ci_modules()
        item = {"tu": "src/a.cpp", "gates": [], "object_verified": False,
                "rows": [{"rva": "0x1000", "status": "MATCH", "proof": "proven"},
                         {"rva": "0x1004", "status": "PARTIAL", "pct": 87.5}]}
        hosted = {"src/a.cpp": {"functions": {"0x00001000": ["MATCH", 100, True], "0x00001004": ["PARTIAL", 87.5, False]},
                                "verified": False, "error": False}}
        result, total, agreed = parity.agreement(hosted, {"src/a.cpp": item}, compare)
        self.assertEqual((result["src/a.cpp"], total, agreed), ([], 2, 2))
        hosted["src/a.cpp"]["functions"]["0x00001000"] = ["MATCH", 100, False]
        result, total, agreed = parity.agreement(hosted, {"src/a.cpp": item}, compare)
        self.assertEqual((total, agreed), (2, 1))
        self.assertIn("0x00001000", result["src/a.cpp"][0])

    def test_prepare_scratch_git_with_main_at_the_measured_commit(self):
        write(self.repo / "tools/judge/bin/judge.py", b"# judge\n")
        head = self.commit("judge")
        workspace = Path(self.scratch.name) / "ws"
        work, gitdir, mounts = parity.prepare(self.repo, head, workspace, "docker")
        self.assertTrue((work / "tools/judge/bin/judge.py").is_file())
        self.assertTrue((work / "baseline/game.dat").is_file())
        self.assertEqual((gitdir / "refs/heads/main").read_text().strip(), head)
        self.assertEqual((gitdir / "objects/info/alternates").read_text().strip(), "/objects")
        self.assertEqual(mounts[0][1], "/objects")
        parity.prepare(self.repo, head, workspace, "host")
        alternate = (gitdir / "objects/info/alternates").read_text().strip()
        self.assertEqual(subprocess.run(["git", "--git-dir", str(gitdir), "cat-file", "-t", "main"],
                                        capture_output=True, text=True).stdout.strip(), "commit", alternate)


class UnitModeTests(GitRepoCase):
    def run_unit(self, *extra, rows=None, error=None):
        calls = []

        def judge_factory(backend, config, repo, image):
            def judge(revision, tus):
                calls.append((revision, tuple(tus)))
                return judge_doc(rows or [{"rva": "0x1000", "status": "MATCH", "symbol": "?f@@YAHXZ",
                                           "proof": "proven"}], error=error)
            return judge

        out = io.StringIO()
        with mock.patch.object(precheck, "check_local_main"), mock.patch("sys.stdout", out):
            code = precheck.main(["--unit", "src/a.cpp", "--backend", "host", "--no-objdiff", *extra],
                                 repo=self.repo, judge_factory=judge_factory,
                                 toolchain=lambda path: {"roots": {"vc71": "/vc"}})
        return code, calls, out.getvalue()

    def test_judges_head_only_and_says_rounds_are_unlimited(self):
        code, calls, out = self.run_unit()
        self.assertEqual(code, 0)
        self.assertEqual(calls, [(self.head, ("src/a.cpp",))])
        self.assertIn("PASS", out)
        self.assertIn("Local rounds are unlimited", out)
        self.assertIn("1/1 functions PASS", out)

    def test_mismatch_and_compile_error(self):
        code, _calls, out = self.run_unit(rows=[{"rva": "0x1000", "status": "PARTIAL", "pct": 50.0}])
        self.assertEqual(code, 0)
        self.assertIn("MISMATCH 50%", out)
        code, _calls, out = self.run_unit(rows=[{"rva": "0x1000", "status": "COMPILE_ERROR"}], error="C2065")
        self.assertEqual(code, 1)
        self.assertIn("COMPILE ERROR", out)

    def test_worktree_snapshot_leaves_refs_and_index_alone(self):
        write(self.repo / "src/a.cpp", b"// FUNCTION: BFME2 0x1000\nint f() { return 2; }\n")
        commit, dirty = precheck.snapshot_worktree(self.repo)
        self.assertTrue(dirty)
        self.assertNotEqual(commit, self.head)
        self.assertEqual(self.git("rev-parse", "HEAD"), self.head)
        self.assertIn("return 2", self.git("show", f"{commit}:src/a.cpp"))
        self.assertEqual(self.git("status", "--porcelain"), "M src/a.cpp")
        code, calls, _out = self.run_unit("--worktree")
        self.assertEqual(self.git("rev-parse", calls[0][0] + "^{tree}"), self.git("rev-parse", commit + "^{tree}"))

    def test_clean_worktree_uses_head(self):
        self.assertEqual(precheck.snapshot_worktree(self.repo), (self.head, False))

    def test_unit_must_be_a_translation_unit(self):
        with mock.patch("sys.stderr", new_callable=io.StringIO):
            code = precheck.main(["--unit", "include/x.h", "--backend", "host"], repo=self.repo)
        self.assertEqual(code, 2)

    def test_view_writes_live_status(self):
        with tempfile.TemporaryDirectory() as cache, mock.patch.dict(os.environ, {bootstrap.CACHE_ENV: cache}), \
                mock.patch.object(precheck, "hold") as hold:
            code, _calls, out = self.run_unit("--view", "--port", "0")
            self.assertEqual(code, 0)
            self.assertIn("Live view: http://localhost:", out)
            server = hold.call_args[0][0]
            server.shutdown()
            server.server_close()
            status = json.loads(next(Path(cache, "runs").glob("*/status.json")).read_text())
            self.assertEqual(status["state"], "done")
            self.assertEqual(status["units"]["src/a.cpp"]["functions"][0]["label"], "PASS")
            self.assertTrue(all(step["state"] == "done" for step in status["steps"]))

    def test_bootstrap_flag_runs_the_bootstrap_first(self):
        result = bootstrap.Result(True, Path("t.json"), vt.Report(vt.Roots(vc71=Path("/vc"))), None, ["note"])
        seen = []
        with mock.patch.object(precheck, "check_local_main"), mock.patch("sys.stdout", new_callable=io.StringIO):
            precheck.main(["--unit", "src/a.cpp", "--backend", "host", "--no-objdiff", "--bootstrap"],
                          repo=self.repo, toolchain=lambda path: {"roots": {"vc71": "/vc"}},
                          judge_factory=lambda *a, **k: (lambda rev, tus: judge_doc([])),
                          bootstrapper=lambda **kwargs: seen.append(kwargs) or result)
        self.assertEqual(seen[0]["backend"], "host")


if __name__ == "__main__":
    unittest.main()
