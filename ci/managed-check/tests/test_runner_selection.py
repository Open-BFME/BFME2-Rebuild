"""Unit tests for runner selection and cross-runner header receipt validation."""
import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "ci/managed-check"))
sys.path.insert(1, str(ROOT / "ci/managed-nt2"))

from common import (  # noqa: E402
    APPROVED_IMAGE_PROFILES,
    APPROVED_RUNNERS,
    DEFAULT_RUNNER_LABEL,
    active_runner,
)


class ActiveRunnerTest(unittest.TestCase):
    def test_default_when_unset(self):
        self.assertEqual(active_runner({}), DEFAULT_RUNNER_LABEL)

    def test_default_when_empty(self):
        self.assertEqual(active_runner({"BFME_PR_RUNNER": ""}), DEFAULT_RUNNER_LABEL)

    def test_default_when_whitespace(self):
        self.assertEqual(active_runner({"BFME_PR_RUNNER": "  "}), DEFAULT_RUNNER_LABEL)

    def test_srv1_runner(self):
        self.assertEqual(active_runner({"BFME_PR_RUNNER": "bfme2-srv1-compiler-v1"}), "bfme2-srv1-compiler-v1")

    def test_boat_runner_explicit(self):
        self.assertEqual(active_runner({"BFME_PR_RUNNER": "boat-small/from=bfme2-compiler-v1"}),
                         "boat-small/from=bfme2-compiler-v1")

    def test_unknown_runner_raises(self):
        with self.assertRaises(ValueError):
            active_runner({"BFME_PR_RUNNER": "evil-runner"})

    def test_unknown_runner_fails_closed(self):
        with self.assertRaises(ValueError):
            active_runner({"BFME_PR_RUNNER": "bfme2-srv1-compiler-v2"})


class ApprovedImageProfilesTest(unittest.TestCase):
    def test_both_runners_present(self):
        self.assertEqual(len(APPROVED_IMAGE_PROFILES), len(APPROVED_RUNNERS))

    def test_boat_pair_approved(self):
        boat = APPROVED_RUNNERS["boat-small/from=bfme2-compiler-v1"]
        self.assertIn((boat["image_sha256"], boat["profile_sha256"]), APPROVED_IMAGE_PROFILES)

    def test_srv1_pair_approved(self):
        srv1 = APPROVED_RUNNERS["bfme2-srv1-compiler-v1"]
        self.assertIn((srv1["image_sha256"], srv1["profile_sha256"]), APPROVED_IMAGE_PROFILES)

    def test_unapproved_pair_rejected(self):
        self.assertNotIn(("0" * 64, "1" * 64), APPROVED_IMAGE_PROFILES)

    def test_mixed_pair_rejected(self):
        boat = APPROVED_RUNNERS["boat-small/from=bfme2-compiler-v1"]
        srv1 = APPROVED_RUNNERS["bfme2-srv1-compiler-v1"]
        self.assertNotIn((boat["image_sha256"], srv1["profile_sha256"]), APPROVED_IMAGE_PROFILES)
        self.assertNotIn((srv1["image_sha256"], boat["profile_sha256"]), APPROVED_IMAGE_PROFILES)


class H2VerifiedReceiptTest(unittest.TestCase):
    """Test that header_h2_action.verified_receipt accepts approved images and rejects others."""

    def _make_record(self, image, profile):
        import header_h2_action as h2
        from common import PIN
        decision = {"number": 1, "base": "a" * 40, "head": "b" * 40, "tree": "c" * 40, "plan": "d" * 64}
        run = {"id": "123", "attempt": "1"}
        expected = h2.receipt_binding(decision, run)
        record = dict(expected)
        record["base_tree"] = "e" * 40
        record["probe_count"] = 0
        record["status"] = "pass"
        record["findings"] = []
        record["layout_only"] = []
        record["image"] = image
        record["profile"] = profile
        return record, decision, run

    def test_exact_pin_accepted_without_approved_images(self):
        import header_h2_action as h2
        from common import PIN
        record, decision, run = self._make_record(PIN["image_sha256"], PIN["profile_sha256"])
        result = h2.verified_receipt(record, decision, run)
        self.assertEqual(result["status"], "pass")

    def test_boat_accepted_with_approved_images(self):
        import header_h2_action as h2
        boat = APPROVED_RUNNERS["boat-small/from=bfme2-compiler-v1"]
        record, decision, run = self._make_record(boat["image_sha256"], boat["profile_sha256"])
        result = h2.verified_receipt(record, decision, run, approved_images=APPROVED_IMAGE_PROFILES)
        self.assertEqual(result["status"], "pass")

    def test_srv1_accepted_with_approved_images(self):
        import header_h2_action as h2
        srv1 = APPROVED_RUNNERS["bfme2-srv1-compiler-v1"]
        record, decision, run = self._make_record(srv1["image_sha256"], srv1["profile_sha256"])
        result = h2.verified_receipt(record, decision, run, approved_images=APPROVED_IMAGE_PROFILES)
        self.assertEqual(result["status"], "pass")

    def test_unapproved_rejected_with_approved_images(self):
        import header_h2_action as h2
        record, decision, run = self._make_record("0" * 64, "1" * 64)
        with self.assertRaises(ValueError):
            h2.verified_receipt(record, decision, run, approved_images=APPROVED_IMAGE_PROFILES)


class HTVerifiedReceiptTest(unittest.TestCase):
    """Test that header_trusted_action.verified_receipt accepts approved images and rejects others."""

    def _make_record(self, image, profile):
        import header_trusted_action as ht
        from common import PIN
        decision = {"number": 1, "base": "a" * 40, "head": "b" * 40, "tree": "c" * 40, "plan": "d" * 64}
        run = {"id": "123", "attempt": "1"}
        expected = ht.receipt_binding(decision, run)
        record = dict(expected)
        record["base_tree"] = "e" * 40
        record["observations"] = {}
        record["image"] = image
        record["profile"] = profile
        return record, decision, run

    def test_boat_accepted_with_approved_images(self):
        import header_trusted_action as ht
        boat = APPROVED_RUNNERS["boat-small/from=bfme2-compiler-v1"]
        record, decision, run = self._make_record(boat["image_sha256"], boat["profile_sha256"])
        result = ht.verified_receipt(record, decision, run, approved_images=APPROVED_IMAGE_PROFILES)
        self.assertIsInstance(result["observations"], dict)

    def test_srv1_accepted_with_approved_images(self):
        import header_trusted_action as ht
        srv1 = APPROVED_RUNNERS["bfme2-srv1-compiler-v1"]
        record, decision, run = self._make_record(srv1["image_sha256"], srv1["profile_sha256"])
        result = ht.verified_receipt(record, decision, run, approved_images=APPROVED_IMAGE_PROFILES)
        self.assertIsInstance(result["observations"], dict)

    def test_unapproved_rejected_with_approved_images(self):
        import header_trusted_action as ht
        record, decision, run = self._make_record("0" * 64, "1" * 64)
        with self.assertRaises(ValueError):
            ht.verified_receipt(record, decision, run, approved_images=APPROVED_IMAGE_PROFILES)


if __name__ == "__main__":
    unittest.main()
