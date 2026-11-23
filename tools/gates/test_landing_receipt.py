"""Unit tests for PR and merge_group source-rule receipts."""

import json
import os
import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

sys.path.insert(0, str(Path(__file__).resolve().parent))
import landing_receipt  # noqa: E402


BASE = "a" * 40
HEAD = "b" * 40
TREE = "c" * 40
POLICY = "d" * 64
QUEUE_REF = "refs/heads/gh-readonly-queue/main/pr-42-1234567890"


def _decision():
    return {
        "base": BASE,
        "head": HEAD,
        "tree": TREE,
        "tools_commit": BASE,
        "policy_sha256": POLICY,
        "state": "pass",
        "overall": "success",
    }


class LandingReceiptTests(unittest.TestCase):
    def test_pr_receipt_preserves_legacy_schema(self):
        rec = landing_receipt.keep(_decision(), BASE, "1464323350", "42", "999", "1")
        self.assertEqual(rec["number"], 42)
        self.assertEqual(rec["repo"], 1464323350)
        self.assertNotIn("kind", rec)
        self.assertNotIn("queue_ref", rec)

    def test_merge_group_receipt_schema(self):
        rec = landing_receipt.keep(
            _decision(), BASE, "1464323350", None, "1001", "2", queue_ref=QUEUE_REF
        )
        self.assertEqual(rec["kind"], "merge_group")
        self.assertEqual(rec["queue_ref"], QUEUE_REF)
        self.assertEqual(rec["repo"], 1464323350)
        self.assertNotIn("number", rec)

    def test_merge_group_main_end_to_end(self):
        with tempfile.TemporaryDirectory() as tmp:
            tmp_path = Path(tmp)
            event_path = tmp_path / "event.json"
            event_path.write_text(
                json.dumps(
                    {
                        "action": "checks_requested",
                        "repository": {"id": 1464323350},
                        "merge_group": {
                            "base_ref": "refs/heads/main",
                            "base_sha": BASE,
                            "head_ref": QUEUE_REF,
                            "head_sha": HEAD,
                        },
                    }
                ),
                encoding="utf-8",
            )
            env = {
                "GITHUB_REPOSITORY": "Open-BFME/BFME2-Rebuild",
                "GITHUB_REPOSITORY_ID": "1464323350",
                "GITHUB_SHA": HEAD,
                "GITHUB_EVENT_NAME": "merge_group",
                "GITHUB_REF": QUEUE_REF,
                "GITHUB_SERVER_URL": "https://github.com",
                "GITHUB_API_URL": "https://api.github.com",
                "GITHUB_EVENT_PATH": str(event_path),
                "GITHUB_RUN_ID": "5555",
                "GITHUB_RUN_ATTEMPT": "1",
                "SOURCE_RULE_RECEIPT": json.dumps(_decision()),
            }
            out_dir = tmp_path / "out"
            with mock.patch.dict(os.environ, env, clear=True):
                landing_receipt.main(out_dir)
            saved = json.loads((out_dir / "receipt.json").read_text(encoding="utf-8"))
            self.assertEqual(saved["kind"], "merge_group")
            self.assertEqual(saved["queue_ref"], QUEUE_REF)
            self.assertEqual(saved["head"], HEAD)

    def test_merge_group_rejects_bad_queue_ref(self):
        with self.assertRaises(ValueError):
            landing_receipt.keep(
                _decision(), BASE, "1464323350", None, "1001", "1", queue_ref="refs/heads/main"
            )


if __name__ == "__main__":
    unittest.main()
