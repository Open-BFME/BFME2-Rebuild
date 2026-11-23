"""The compiler check's receiver (ci/managed-check/fork_admission.py) claims what admit.py reserved.

Skipped until that receiver is installed. When it is, this drives its real claim_compiler() against this
directory's real ledger, so the seam (field names, issuer identity, single use) is proven together.
"""
import io
from contextlib import redirect_stdout
from pathlib import Path
import sys
import unittest

ROOT = Path(__file__).resolve().parents[3]
sys.path.insert(0, str(ROOT / "ci/admission/tests"))
sys.path.insert(0, str(ROOT / "ci/managed-check"))
from test_admit import BUDGET, CLOCK, CONFIG, HEAD, MAIN, TREE, LedgerApi, admit, fork_pr  # noqa: E402

try:
    import fork_admission  # noqa: E402
except ImportError:
    fork_admission = None

ADMISSION_RUN = "7"


class Receiver:
    """The receiver's view of GitHub: the pull request and the admission run that issued the reservation."""

    def __init__(self, pr, event="pull_request_target"):
        self.pr, self.event = pr, event

    def current(self, binding):
        return self.pr

    def call(self, path):
        assert path == f"actions/runs/{ADMISSION_RUN}", path
        return {"id": int(ADMISSION_RUN), "run_attempt": 1, "repository": {"id": admit.REPO_ID},
                "path": ".github/workflows/admission.yml", "status": "in_progress", "conclusion": None,
                "event": self.event, "head_sha": HEAD, "head_branch": "main"}


def receiver_env(run="900", attempt="1"):
    return {"BFME_AUTO_ADMISSION": "yes", "BFME_PR_COMPACT": "yes", "BFME_ADMISSION_DAILY_TOTAL": "150",
            "BFME_ADMISSION_DAILY_PER_AUTHOR": "30", "GITHUB_EVENT_NAME": "repository_dispatch",
            "GITHUB_ACTOR_ID": "41898282", "BFME_TRIGGERING_ACTOR": "github-actions[bot]",
            "GITHUB_RUN_ATTEMPT": attempt, "GITHUB_RUN_ID": run}


@unittest.skipIf(fork_admission is None, "the compiler check's receiver (fork_admission.py) is not installed here")
class Seam(unittest.TestCase):
    def setUp(self):
        self.pr = fork_pr()
        self.book = LedgerApi(True, [self.pr], {21: ["src/a.cpp"]})
        self.book.run_id = ADMISSION_RUN
        with redirect_stdout(io.StringIO()):
            admit.handle(self.book, 21, CONFIG, MAIN, BUDGET, CLOCK, source_proof=lambda *_: None)
        self.binding = {"number": 21, "head": HEAD, "base": MAIN}
        self.decision = {"version": 1, "repo": admit.REPO_ID, "number": 21, "base": MAIN, "head": HEAD,
                         "tree": TREE, "kind": "compile", "plan": "5" * 64, "automatic_author": 424242}

    def claim(self, env):
        original = admit.ledger.days
        admit.ledger.days = lambda clock=None: original(CLOCK)
        try:
            return fork_admission.claim_compiler(Receiver(self.pr), self.book, admit.ledger, self.binding,
                                                 self.decision, env)
        finally:
            admit.ledger.days = original

    def test_the_receiver_claims_the_reservation_once(self):
        self.assertTrue(self.claim(receiver_env()))
        self.assertEqual(admit.ledger.find(self.book, 21, HEAD, MAIN, CLOCK)["claimed_by"],
                         {"run": "900", "attempt": "1"})

    def test_a_rerun_or_a_second_delivery_gets_nothing(self):
        self.assertTrue(self.claim(receiver_env()))
        self.assertFalse(self.claim(receiver_env(attempt="2")))
        self.assertFalse(self.claim(receiver_env(run="901")))

    def test_another_base_has_no_reservation(self):
        self.binding["base"] = self.decision["base"] = "e" * 40
        self.assertFalse(self.claim(receiver_env()))


    def test_wide_header_reservation_claimed_by_receiver(self):
        wide_pr = fork_pr(number=22)
        book = LedgerApi(True, [wide_pr], {22: ["include/Game/Foo.h"]})
        book.run_id = ADMISSION_RUN
        wide_budget = {"daily_total": 20, "daily_per_author": 5}
        with redirect_stdout(io.StringIO()):
            out = admit.handle(book, 22, CONFIG, MAIN, BUDGET, CLOCK, auto_headers=True,
                               wide_budget=wide_budget, source_proof=lambda *_: None)
        self.assertIn("admitted by rule", out)
        binding = {"number": 22, "head": HEAD, "base": MAIN}
        decision = {"version": 1, "repo": admit.REPO_ID, "number": 22, "base": MAIN, "head": HEAD,
                    "tree": TREE, "kind": "wide", "plan": "5" * 64, "automatic_author": 424242,
                    "automatic_kind": "wide"}
        env = receiver_env() | {"BFME_AUTO_HEADERS": "yes", "BFME_ADMISSION_WIDE_DAILY_TOTAL": "20",
                                "BFME_ADMISSION_WIDE_DAILY_PER_AUTHOR": "5"}
        original = admit.ledger.days
        admit.ledger.days = lambda clock=None: original(CLOCK)
        try:
            # Missing BFME_AUTO_HEADERS must refuse.
            self.assertFalse(fork_admission.claim_compiler(Receiver(wide_pr), book, admit.ledger, binding,
                                                           decision, receiver_env()))
            # With BFME_AUTO_HEADERS and wide budget, receiver claims once.
            self.assertTrue(fork_admission.claim_compiler(Receiver(wide_pr), book, admit.ledger, binding,
                                                          decision, env))
            self.assertEqual(admit.ledger.find(book, 22, HEAD, MAIN, CLOCK)["claimed_by"],
                             {"run": "900", "attempt": "1"})
            self.assertFalse(fork_admission.claim_compiler(Receiver(wide_pr), book, admit.ledger, binding,
                                                           decision, env | {"GITHUB_RUN_ID": "901"}))
        finally:
            admit.ledger.days = original

    def test_trusted_wide_header_reservation_claimed_by_receiver(self):
        trusted_pr = fork_pr(number=317)
        trusted_pr["head"]["repo"] = {"id": admit.REPO_ID}
        trusted_pr["author_association"] = "OWNER"
        book = LedgerApi(True, [trusted_pr], {317: ["include/Game/Foo.h"]})
        book.run_id = ADMISSION_RUN
        with redirect_stdout(io.StringIO()):
            out = admit.handle(book, 317, CONFIG, MAIN, None, CLOCK, auto_headers=True,
                               wide_budget=None, source_proof=lambda *_: None)
        self.assertIn("admitted by rule", out)
        binding = {"number": 317, "head": HEAD, "base": MAIN, "event_association": "OWNER"}
        decision = {"version": 1, "repo": admit.REPO_ID, "number": 317, "base": MAIN, "head": HEAD,
                    "tree": TREE, "kind": "wide", "plan": "5" * 64, "automatic_author": 424242,
                    "automatic_kind": "wide"}
        env_no_headers = receiver_env()
        env_headers = receiver_env() | {"BFME_AUTO_HEADERS": "yes"}
        # authority() grants automatic_wide_allowed for trusted PRs only when BFME_AUTO_HEADERS=yes.
        self.assertNotIn("automatic_wide_allowed", fork_admission.authority(trusted_pr, binding, env_no_headers))
        self.assertTrue(fork_admission.authority(trusted_pr, binding, env_headers).get("automatic_wide_allowed"))
        original = admit.ledger.days
        admit.ledger.days = lambda clock=None: original(CLOCK)
        try:
            self.assertFalse(fork_admission.claim_compiler(Receiver(trusted_pr), book, admit.ledger, binding,
                                                           decision, env_no_headers))
            # Budget-free trusted-wide claims cleanly even without wide daily limit env vars.
            self.assertTrue(fork_admission.claim_compiler(Receiver(trusted_pr), book, admit.ledger, binding,
                                                          decision, env_headers))
            self.assertEqual(admit.ledger.find(book, 317, HEAD, MAIN, CLOCK)["claimed_by"],
                             {"run": "900", "attempt": "1"})
            self.assertFalse(fork_admission.claim_compiler(Receiver(trusted_pr), book, admit.ledger, binding,
                                                           decision, env_headers | {"GITHUB_RUN_ID": "901"}))
        finally:
            admit.ledger.days = original


if __name__ == "__main__":
    unittest.main()
