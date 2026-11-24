"""Verify that every snapshot-backed Boat workflow job waits for lazyfs hydration."""

from pathlib import Path
import os
import re
import stat
import subprocess
import tempfile
import unittest

import yaml

ROOT = Path(__file__).resolve().parents[3]
WORKFLOWS = ROOT / ".github" / "workflows"
STEP_NAME = "Wait for runner filesystem hydration"

EXPECTED_SNAPSHOT_JOBS = {
    ".github/workflows/landing-rehearsal.yml": {"rehearse"},
    ".github/workflows/managed-compiler-canary.yml": {"managed-compiler-canary"},
    ".github/workflows/managed-pr.yml": {
        "compare",
        "wide-plan",
        "wide-compare",
        "wide-reserve",
        "wide-join",
        "wide-fallback",
    },
    ".github/workflows/merge-queue-boat.yml": {"boat"},
    ".github/workflows/nightly-trial.yml": {"compile", "result"},
    ".github/workflows/nightly.yml": {"compile"},
    ".github/workflows/nt2-managed-match.yml": {"compare"},
    ".github/workflows/state-parity.yml": {"measure"},
    ".github/workflows/state.yml": {"measure", "recover"},
}

SNAPSHOT_RUNNER_VARS = ("vars.BFME_STATE_RECOVERY_RUNNER",)


def is_snapshot_boat_runner(runs_on):
    text = str(runs_on or "")
    if "from=" in text and "boat" in text:
        return True
    return any(var in text for var in SNAPSHOT_RUNNER_VARS)


def load_workflow(path):
    return yaml.safe_load(path.read_text(encoding="utf-8")) or {}


def extract_wait_script():
    wf = load_workflow(WORKFLOWS / "managed-pr.yml")
    step = wf["jobs"]["compare"]["steps"][0]
    assert step.get("name") == STEP_NAME, f"unexpected first step: {step!r}"
    return step["run"]


class BoatHydrationWorkflowShapeTest(unittest.TestCase):
    def test_every_snapshot_boat_job_is_covered_and_waits_first(self):
        discovered = {}
        scripts = {}
        for wf_path in sorted(WORKFLOWS.glob("*.yml")) + sorted(WORKFLOWS.glob("*.yaml")):
            rel = wf_path.relative_to(ROOT).as_posix()
            data = load_workflow(wf_path)
            jobs = data.get("jobs") or {}
            for job_name, job_def in jobs.items():
                if not isinstance(job_def, dict):
                    continue
                if not is_snapshot_boat_runner(job_def.get("runs-on")):
                    continue
                discovered.setdefault(rel, set()).add(job_name)
                steps = job_def.get("steps") or []
                self.assertTrue(steps, f"{rel}:{job_name} has no steps")
                first = steps[0]
                self.assertIsInstance(first, dict, f"{rel}:{job_name} first step is not a mapping")
                self.assertEqual(first.get("name"), STEP_NAME, f"{rel}:{job_name} missing hydration wait as step 0")
                self.assertEqual(first.get("shell"), "bash", f"{rel}:{job_name} wait step must specify shell: bash")
                self.assertNotIn("uses", first, f"{rel}:{job_name} wait step must not invoke an action")
                self.assertNotIn("env", first, f"{rel}:{job_name} wait step must not accept tokens or env")
                self.assertEqual(set(first.keys()), {"name", "shell", "run"}, f"{rel}:{job_name} unexpected step keys")

                # Every actions/checkout must come strictly after the wait step.
                for idx, step in enumerate(steps):
                    if isinstance(step, dict) and str(step.get("uses", "")).startswith("actions/checkout"):
                        self.assertGreater(idx, 0, f"{rel}:{job_name} runs checkout before hydration wait")

                run_text = first.get("run", "")
                self.assertIn("/proc/self/mountinfo", run_text, f"{rel}:{job_name} must read /proc/self/mountinfo")
                self.assertIn("ascii-lazyfs", run_text, f"{rel}:{job_name} must check ascii-lazyfs mounts")
                self.assertIn(r"\/var\/lib\/ascii-lazy\/retired", run_text, f"{rel}:{job_name} must ignore retired mounts")
                self.assertIn("deadline=$((SECONDS + 90))", run_text, f"{rel}:{job_name} must enforce a 90s bound")
                self.assertIn("exit 1", run_text, f"{rel}:{job_name} must fail closed on timeout")
                for forbidden in ("github.token", "secrets.", "sudo", "curl", "wget", "git ", "python"):
                    self.assertNotIn(forbidden, run_text, f"{rel}:{job_name} wait step must not use {forbidden!r}")
                scripts[f"{rel}:{job_name}"] = run_text

        self.assertEqual(discovered, EXPECTED_SNAPSHOT_JOBS)
        self.assertEqual(len(set(scripts.values())), 1, "All snapshot-backed Boat jobs must share the exact same wait step")


class BoatHydrationStepBehaviorTest(unittest.TestCase):
    def _run_wait(self, mountinfo_path, *, timeout_s=None, fake_sleep=None):
        script = extract_wait_script().replace("/proc/self/mountinfo", str(mountinfo_path))
        if timeout_s is not None:
            script = script.replace("deadline=$((SECONDS + 90))", f"deadline=$((SECONDS + {timeout_s}))")
            script = script.replace("runner_grace=$((SECONDS + 15))", f"runner_grace=$((SECONDS + {min(timeout_s, 1)}))")
        env = dict(os.environ)
        env["HOME"] = "/runner-home"
        with tempfile.TemporaryDirectory() as bindir:
            if fake_sleep is not None:
                sleep_bin = Path(bindir) / "sleep"
                sleep_bin.write_text(fake_sleep, encoding="utf-8")
                sleep_bin.chmod(sleep_bin.stat().st_mode | stat.S_IXUSR)
                env["PATH"] = f"{bindir}:{env.get('PATH', '')}"
            return subprocess.run(
                ["bash", "-c", script],
                capture_output=True,
                text=True,
                env=env,
                timeout=15,
            )

    def test_noop_on_real_host_without_lazyfs(self):
        # On a standard Linux host (no active ascii-lazyfs), the exact verbatim script exits 0 immediately.
        proc = subprocess.run(
            ["bash", "-c", extract_wait_script()],
            capture_output=True,
            text=True,
            timeout=10,
        )
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertEqual(proc.stdout, "")
        self.assertEqual(proc.stderr, "")

    def test_noop_on_hosted_and_already_retired_mountinfo(self):
        mountinfo = "\n".join([
            "29 1 252:1 / / rw,relatime shared:1 - ext4 /dev/vda1 rw",
            "45 29 0:40 / /mnt/remote rw - fuse.sshfs remote:/data rw",
            "1036 29 0:51 / /var/lib/ascii-lazy/retired/work rw,relatime shared:664 - fuse ascii-lazyfs rw",
            "1044 29 0:57 / /var/lib/ascii-lazy/retired/opt rw,relatime shared:665 - fuse ascii-lazyfs-sys rw",
        ]) + "\n"
        with tempfile.TemporaryDirectory() as d:
            mp = Path(d) / "mountinfo"
            mp.write_text(mountinfo, encoding="utf-8")
            proc = self._run_wait(mp)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertEqual(proc.stdout, "")
        self.assertEqual(proc.stderr, "")

    def test_waits_until_all_active_lazyfs_mounts_retire(self):
        tick0 = "\n".join([
            "29 1 252:1 / / rw,relatime shared:1 - ext4 /dev/vda1 rw",
            "1035 708 0:51 / /workspace rw,nosuid,nodev,relatime - fuse ascii-lazyfs rw",
            "1044 776 0:57 / /opt/boat-compiler rw,relatime - fuse ascii-lazyfs-sys rw",
            "1050 777 0:58 / /usr rw,relatime - fuse.ascii-lazyfs-sys lazy rw",
        ]) + "\n"
        tick1 = "\n".join([
            "29 1 252:1 / / rw,relatime shared:1 - ext4 /dev/vda1 rw",
            "1035 708 0:51 / /workspace rw,nosuid,nodev,relatime - fuse ascii-lazyfs rw",
            "1044 29 0:57 / /var/lib/ascii-lazy/retired/opt rw,relatime shared:665 - fuse ascii-lazyfs-sys rw",
            "1050 29 0:58 / /var/lib/ascii-lazy/retired/usr rw,relatime shared:666 - fuse.ascii-lazyfs-sys lazy rw",
        ]) + "\n"
        tick2 = "\n".join([
            "29 1 252:1 / / rw,relatime shared:1 - ext4 /dev/vda1 rw",
            "1035 29 0:51 / /var/lib/ascii-lazy/retired/work rw,relatime shared:664 - fuse ascii-lazyfs rw",
            "1044 29 0:57 / /var/lib/ascii-lazy/retired/opt rw,relatime shared:665 - fuse ascii-lazyfs-sys rw",
            "1050 29 0:58 / /var/lib/ascii-lazy/retired/usr rw,relatime shared:666 - fuse.ascii-lazyfs-sys lazy rw",
        ]) + "\n"
        with tempfile.TemporaryDirectory() as d:
            dp = Path(d)
            mp = dp / "mountinfo"
            mp.write_text(tick0, encoding="utf-8")
            (dp / "t1").write_text(tick1, encoding="utf-8")
            (dp / "t2").write_text(tick2, encoding="utf-8")
            count_file = dp / "count"
            count_file.write_text("0\n", encoding="utf-8")
            fake_sleep = (
                "#!/bin/bash\n"
                f"n=$(cat {count_file})\n"
                "n=$((n + 1))\n"
                f"echo \"$n\" > {count_file}\n"
                f"if [ \"$n\" -eq 1 ]; then cp {dp / 't1'} {mp}; fi\n"
                f"if [ \"$n\" -eq 2 ]; then cp {dp / 't2'} {mp}; fi\n"
            )
            proc = self._run_wait(mp, fake_sleep=fake_sleep)
            ticks = int(count_file.read_text(encoding="utf-8").strip())
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertEqual(ticks, 2)
        self.assertIn("Runner filesystem hydration finished after", proc.stdout)

    def test_fails_closed_when_bound_is_reached(self):
        stuck = "\n".join([
            "29 1 252:1 / / rw,relatime shared:1 - ext4 /dev/vda1 rw",
            "1035 708 0:51 / /workspace rw,nosuid,nodev,relatime - fuse ascii-lazyfs rw",
            "1044 776 0:57 / /opt/boat-compiler rw,relatime - fuse ascii-lazyfs-sys rw",
        ]) + "\n"
        with tempfile.TemporaryDirectory() as d:
            mp = Path(d) / "mountinfo"
            mp.write_text(stuck, encoding="utf-8")
            proc = self._run_wait(mp, timeout_s=0)
        self.assertEqual(proc.returncode, 1)
        self.assertIn("Expected lazy runner mounts to retire within 90s; still mounted:", proc.stderr)
        self.assertIn("/workspace", proc.stderr)
        self.assertIn("/opt/boat-compiler", proc.stderr)

    def test_fails_closed_when_mountinfo_missing(self):
        with tempfile.TemporaryDirectory() as d:
            missing = Path(d) / "does-not-exist"
            proc = self._run_wait(missing)
        self.assertNotEqual(proc.returncode, 0)


    def test_persistent_home_runner_alone_proceeds_after_grace(self):
        only_runner = "\n".join([
            "29 1 252:1 / / rw,relatime shared:1 - ext4 /dev/vda1 rw",
            "1036 29 0:51 / /var/lib/ascii-lazy/retired/opt rw,relatime - fuse ascii-lazyfs-sys rw",
            "1044 776 0:57 / /runner-home rw,relatime - fuse ascii-lazyfs rw",
        ]) + "\n"
        with tempfile.TemporaryDirectory() as d:
            mp = Path(d) / "mountinfo"
            mp.write_text(only_runner, encoding="utf-8")
            proc = self._run_wait(mp, timeout_s=3)
        self.assertEqual(proc.returncode, 0, proc.stderr)
        self.assertIn("persistent lazy mount /runner-home", proc.stdout)

    def test_home_runner_plus_compiler_mount_still_fails_closed(self):
        stuck_both = "\n".join([
            "29 1 252:1 / / rw,relatime shared:1 - ext4 /dev/vda1 rw",
            "1044 776 0:57 / /runner-home rw,relatime - fuse ascii-lazyfs rw",
            "1045 776 0:58 / /opt/boat-compiler rw,relatime - fuse ascii-lazyfs-sys rw",
        ]) + "\n"
        with tempfile.TemporaryDirectory() as d:
            mp = Path(d) / "mountinfo"
            mp.write_text(stuck_both, encoding="utf-8")
            proc = self._run_wait(mp, timeout_s=1)
        self.assertEqual(proc.returncode, 1)
        self.assertIn("/opt/boat-compiler", proc.stderr)



    def test_compiler_mount_never_excused_even_if_home(self):
        with tempfile.TemporaryDirectory() as td:
            mountinfo = Path(td) / "mountinfo"
            mountinfo.write_text(
                "42 1 0:40 / /opt/boat-compiler rw,relatime - fuse.ascii-lazyfs ascii-lazyfs rw\n",
                encoding="utf-8",
            )
            script = extract_wait_script().replace("/proc/self/mountinfo", str(mountinfo))
            script = script.replace("deadline=$((SECONDS + 90))", "deadline=$((SECONDS + 2))")
            script = script.replace("runner_grace=$((SECONDS + 15))", "runner_grace=$((SECONDS + 1))")
            env = dict(os.environ)
            env["HOME"] = "/opt/boat-compiler"
            proc = subprocess.run(["bash", "-c", script], capture_output=True, text=True, env=env, timeout=15)
            self.assertNotEqual(proc.returncode, 0)
            self.assertIn("/opt/boat-compiler", proc.stderr)


if __name__ == "__main__":
    unittest.main()
