import sys
import unittest
from pathlib import Path

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "ci/managed-check"))
sys.path.insert(1, str(ROOT / "ci/managed-nt2"))
import header_trusted_action as hta


class HeaderTrustedActionTests(unittest.TestCase):
    def test_verified_receipt_accepts_exact_binding_and_rejects_tampering(self):
        decision = {
            "number": 43,
            "base": "a" * 40,
            "head": "b" * 40,
            "tree": "c" * 40,
            "plan": "d" * 64,
        }
        run = {"id": "999", "attempt": "1"}
        doc = hta.receipt_binding(decision, run) | {
            "base_tree": "e" * 40,
            "observations": {"version": 1, "declarations": {}, "bodies": {}},
        }
        self.assertEqual(hta.verified_receipt(doc, decision, run), doc)
        for key, bad_val in (
            ("component", "H2"),
            ("base", "0" * 40),
            ("base_tree", "not-a-sha"),
            ("source_policy", "0" * 64),
        ):
            tampered = dict(doc, **{key: bad_val})
            with self.assertRaises(ValueError):
                hta.verified_receipt(tampered, decision, run)
        with self.assertRaises(ValueError):
            hta.verified_receipt(doc | {"extra": 1}, decision, run)


if __name__ == "__main__":
    unittest.main()
