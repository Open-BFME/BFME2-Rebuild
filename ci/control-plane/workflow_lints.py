"""Executable Phase 0 CI Workflow Lints (a)-(j) scanner.

Audits .github/workflows/*.yml/.yaml files against Phase 0 security & replay
invariants without external dependencies (stdlib-only):
- (a) Actions pinned to 40-char commit SHAs (no mutable @vN/@main tags)
- (e) id-token: write only at job level, never workflow top-level
- (h) No workflow or job grants actions: write (moved to Broker Dispatch API)
- (i) No legacy contents: write on publisher workflows (moved to Broker Publish API)
- (j) Replay-receiving workflows declare dispatch_idempotency_key input & run-name
"""

from __future__ import annotations

import re
import sys
from dataclasses import dataclass
from pathlib import Path


USES_RE = re.compile(r"^\s*-?\s*uses:\s*([^\s#]+)")
SHA_PIN_RE = re.compile(r"@[0-9a-f]{40}$")

REPLAY_WORKFLOWS = {
    "admission.yml",
    "managed-pr.yml",
    "rules.yml",
    "header-rules.yml",
    "state.yml",
    "progress.yml",
    "nightly.yml",
}


@dataclass(frozen=True)
class LintFinding:
    rule: str
    file: str
    line: int
    message: str


def lint_workflow(path: Path) -> list[LintFinding]:
    findings: list[LintFinding] = []
    text = path.read_text(encoding="utf-8")
    lines = text.splitlines()
    name = path.name
    in_jobs = False

    for idx, line in enumerate(lines, 1):
        stripped = line.strip()
        if re.match(r"^jobs:\s*$", line):
            in_jobs = True

        # (a) Action SHA pinning
        m = USES_RE.match(line)
        if m:
            ref = m.group(1).strip("'\"")
            if not ref.startswith("./") and not SHA_PIN_RE.search(ref):
                findings.append(LintFinding("lint-a", name, idx, f"Unpinned action reference: {ref}"))

        # (e) Top-level id-token: write forbidden
        if not in_jobs and re.match(r"^\s*id-token:\s*write\b", line):
            findings.append(LintFinding("lint-e", name, idx, "id-token: write must be job-scoped, not top-level"))

        # (h) actions: write forbidden post-Phase 0
        if re.match(r"^\s*actions:\s*write\b", line):
            findings.append(LintFinding("lint-h", name, idx, "actions: write must migrate to Broker Dispatch API"))

        # (i) contents: write flagged for Broker Publish/Dispatch migration
        if re.match(r"^\s*contents:\s*write\b", line):
            findings.append(LintFinding("lint-i", name, idx, "contents: write must migrate to Broker Publish/Dispatch API"))

    # (j) Replay-receiving workflow correlation contract
    if name in REPLAY_WORKFLOWS:
        if "run-name:" not in text or "dispatch_idempotency_key" not in text:
            findings.append(
                LintFinding(
                    "lint-j",
                    name,
                    1,
                    "Replay workflow missing run-name embedding dispatch_idempotency_key",
                )
            )
        if "workflow_dispatch:" in text and "dispatch_idempotency_key:" not in text:
            findings.append(
                LintFinding(
                    "lint-j",
                    name,
                    1,
                    "Replay workflow with workflow_dispatch must declare optional dispatch_idempotency_key input",
                )
            )

    return findings


def scan_workflows_dir(workflows_dir: Path) -> list[LintFinding]:
    all_findings: list[LintFinding] = []
    for path in sorted(list(workflows_dir.glob("*.yml")) + list(workflows_dir.glob("*.yaml"))):
        all_findings.extend(lint_workflow(path))
    return all_findings


if __name__ == "__main__":
    root = Path(sys.argv[1]) if len(sys.argv) > 1 else Path(".github/workflows")
    results = scan_workflows_dir(root)
    for f in results:
        print(f"{f.rule}: {f.file}:{f.line}: {f.message}")
    print(f"Total Phase 0 migration lint findings: {len(results)}")
