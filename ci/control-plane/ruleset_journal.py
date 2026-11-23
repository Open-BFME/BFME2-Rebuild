"""Write-ahead ruleset mutation journal and 3-case deterministic settlement protocol.

Enforces V75 guarantees:
- Explicit lifecycle: journal_state in {'in_flight', 'awaiting_audit_claim', 'settled'}.
- Global per-App single-unsettled-write serialization (zero entries with
  journal_state != 'settled' before any next ruleset PUT may be issued).
- Mandatory >= 2s monotonic-second spacing between consecutive PUTs to the same ruleset_id.
- Per-operation Audit Log correlation using exact ruleset_id, event_id not in
  prior_claimed_event_ids, and created_at > pre_put_updated_at.
- Case 2 causal safety backed by token death + >= 2 min post-death settle horizon +
  direct primary GET /rulesets/{id} readbacks + endpoint-specific
  ruleset_put_barrier_staging_verified == True.
"""

from __future__ import annotations

import json
import os
from dataclasses import asdict, dataclass
from pathlib import Path
from typing import Any, Callable


SETTLE_HORIZON_SECONDS = 120
SAME_RULESET_MIN_SPACING_SECONDS = 2
AUDIT_CLAIM_WINDOW_SECONDS = 2


class JournalError(Exception):
    """Raised when ruleset journal invariants or settlement fail closed."""

    def __init__(self, code: str, message: str = "") -> None:
        super().__init__(message or code)
        self.code = code


@dataclass
class RulesetOpEntry:
    op_id: str
    cutover_attempt_id: str
    rollback_attempt_id: int | None
    step: str
    ruleset_id: int
    ruleset_name: str
    actor_login: str
    pre_put_updated_at: int
    pre_put_ruleset_sha256: str
    target_sha256: str
    token_id: str
    token_expires_at: int
    started_at: int
    journal_state: str = "in_flight"  # "in_flight" | "awaiting_audit_claim" | "settled"
    http_status: int | None = None
    post_put_updated_at: int | None = None
    audit_log_event_id: str | None = None
    settled_outcome: str | None = None  # "http_200_claimed" | "applied_observed" | "not_applied_token_dead"


@dataclass
class AuditEvent:
    event_id: str
    ruleset_id: int
    actor_login: str
    created_at: int


@dataclass
class RulesetReadback:
    ruleset_id: int
    updated_at: int
    ruleset_sha256: str


class RulesetJournal:
    """Append-and-fsync local write-ahead journal for forward or rollback ruleset PUTs."""

    def __init__(self, path: Path) -> None:
        self.path = path
        self._entries: list[RulesetOpEntry] = []
        if path.exists():
            self._load()

    def _load(self) -> None:
        by_op: dict[str, RulesetOpEntry] = {}
        order: list[str] = []
        for line in self.path.read_text(encoding="utf-8").splitlines():
            if not line.strip():
                continue
            data = json.loads(line)
            entry = RulesetOpEntry(**data)
            if entry.op_id not in by_op:
                order.append(entry.op_id)
            by_op[entry.op_id] = entry
        self._entries = [by_op[op_id] for op_id in order]

    def _persist(self, entry: RulesetOpEntry) -> None:
        self.path.parent.mkdir(parents=True, exist_ok=True)
        with self.path.open("a", encoding="utf-8", newline="\n") as fh:
            fh.write(json.dumps(asdict(entry), sort_keys=True) + "\n")
            fh.flush()
            os.fsync(fh.fileno())

    def entries(self) -> list[RulesetOpEntry]:
        return list(self._entries)

    def unsettled_entries(self) -> list[RulesetOpEntry]:
        """Unified entry predicate across A(0), B(0), A(iii), B(iv), and R2 Step 4(b)(vi)."""
        return [e for e in self._entries if e.journal_state != "settled"]

    def claimed_event_ids(self) -> set[str]:
        return {e.audit_log_event_id for e in self._entries if e.audit_log_event_id}

    def begin_put(self, entry: RulesetOpEntry, now: int) -> RulesetOpEntry:
        """Write-ahead fsync before sending any ruleset PUT."""
        unsettled = self.unsettled_entries()
        if unsettled:
            raise JournalError(
                "unsettled_ruleset_op",
                f"Cannot start {entry.op_id}: unsettled entry {unsettled[0].op_id} in state {unsettled[0].journal_state}",
            )
        same_ruleset = [e for e in self._entries if e.ruleset_id == entry.ruleset_id and e.post_put_updated_at is not None]
        if same_ruleset:
            last_ts = max(e.post_put_updated_at for e in same_ruleset if e.post_put_updated_at is not None)
            if now - last_ts < SAME_RULESET_MIN_SPACING_SECONDS:
                raise JournalError(
                    "same_ruleset_spacing_unmet",
                    f"Must wait >= {SAME_RULESET_MIN_SPACING_SECONDS}s after {last_ts} before next PUT to ruleset {entry.ruleset_id}",
                )
        entry.journal_state = "in_flight"
        self._entries.append(entry)
        self._persist(entry)
        return entry

    def record_http_200(self, op_id: str, post_put_updated_at: int) -> RulesetOpEntry:
        """Transition in_flight -> awaiting_audit_claim upon HTTP 200."""
        entry = self._by_id(op_id)
        if entry.journal_state != "in_flight":
            raise JournalError("invalid_journal_transition", f"{op_id} is {entry.journal_state}, expected in_flight")
        entry.http_status = 200
        entry.post_put_updated_at = post_put_updated_at
        entry.journal_state = "awaiting_audit_claim"
        self._persist(entry)
        return entry

    def settle_all(
        self,
        *,
        now: int,
        ruleset_put_barrier_staging_verified: bool,
        get_token_dead_at: Callable[[str], int | None],
        read_ruleset_twice: Callable[[int], tuple[RulesetReadback, RulesetReadback]],
        query_audit_events: Callable[[int, str, int], list[AuditEvent]],
    ) -> list[RulesetOpEntry]:
        """Settle every journal_state != 'settled' entry deterministically.

        1. First settles any 'awaiting_audit_claim' entry by matching its unclaimed
           Org Audit Log event within +-2s of post_put_updated_at.
        2. Then settles any 'in_flight' entry under the 3-case protocol:
           Case 1 (applied_observed), Case 2 (not_applied_token_dead), Case 3 (halt).
        """
        settled_now: list[RulesetOpEntry] = []
        # Step 1: awaiting_audit_claim FIRST
        for entry in [e for e in self._entries if e.journal_state == "awaiting_audit_claim"]:
            claimed = self.claimed_event_ids()
            events = query_audit_events(entry.ruleset_id, entry.actor_login, entry.started_at - 60)
            candidates = [
                ev
                for ev in events
                if ev.ruleset_id == entry.ruleset_id
                and ev.actor_login == entry.actor_login
                and ev.event_id not in claimed
                and entry.post_put_updated_at is not None
                and abs(ev.created_at - entry.post_put_updated_at) <= AUDIT_CLAIM_WINDOW_SECONDS
            ]
            if not candidates:
                raise JournalError("audit_claim_pending", f"No unclaimed audit event yet for 200 OK {entry.op_id}")
            candidates.sort(key=lambda ev: (abs(ev.created_at - (entry.post_put_updated_at or 0)), ev.event_id))
            entry.audit_log_event_id = candidates[0].event_id
            entry.journal_state = "settled"
            entry.settled_outcome = "http_200_claimed"
            self._persist(entry)
            settled_now.append(entry)

        # Step 2: in_flight entries via 3-case deterministic protocol
        for entry in [e for e in self._entries if e.journal_state == "in_flight"]:
            token_dead_at = get_token_dead_at(entry.token_id)
            if token_dead_at is None:
                if now > entry.token_expires_at + 300:
                    token_dead_at = entry.token_expires_at + 300
                else:
                    raise JournalError("token_not_dead", f"Token {entry.token_id} for {entry.op_id} is not proven dead")
            settle_horizon = token_dead_at + SETTLE_HORIZON_SECONDS
            if now < settle_horizon:
                raise JournalError(
                    "settle_horizon_not_elapsed",
                    f"Must wait until {settle_horizon} (now={now}) for {entry.op_id}",
                )

            rb1, rb2 = read_ruleset_twice(entry.ruleset_id)
            if rb1 != rb2:
                raise JournalError("unstable_ruleset_readback", f"Readbacks differed for ruleset {entry.ruleset_id}")

            claimed = self.claimed_event_ids()
            events = query_audit_events(entry.ruleset_id, entry.actor_login, entry.started_at - 60)
            unclaimed = [
                ev
                for ev in events
                if ev.ruleset_id == entry.ruleset_id
                and ev.actor_login == entry.actor_login
                and ev.event_id not in claimed
                and ev.created_at > entry.pre_put_updated_at
            ]

            # Case 1 — Applied
            if rb1.updated_at > entry.pre_put_updated_at and rb1.ruleset_sha256 == entry.target_sha256:
                if not unclaimed:
                    raise JournalError("audit_event_missing_for_applied", f"Primary shows applied for {entry.op_id} without audit event")
                entry.post_put_updated_at = rb1.updated_at
                entry.audit_log_event_id = unclaimed[0].event_id
                entry.journal_state = "settled"
                entry.settled_outcome = "applied_observed"
                self._persist(entry)
                settled_now.append(entry)
                continue

            # Case 2 — Never Applied / Crash-Before-Send / Mid-Flight-Revoked
            if rb1.updated_at == entry.pre_put_updated_at and rb1.ruleset_sha256 == entry.pre_put_ruleset_sha256:
                if not ruleset_put_barrier_staging_verified:
                    raise JournalError("ruleset_put_barrier_unverified", "Case 2 requires Step 0(b) ruleset_put_barrier_staging_verified")
                if unclaimed:
                    raise JournalError(
                        "ruleset_audit_divergence",
                        f"Unclaimed audit event {unclaimed[0].event_id} exists despite unchanged primary readback for {entry.op_id}",
                    )
                entry.journal_state = "settled"
                entry.settled_outcome = "not_applied_token_dead"
                self._persist(entry)
                settled_now.append(entry)
                continue

            # Case 3 — Divergent
            raise JournalError(
                "ruleset_audit_divergence",
                f"Divergent state on ruleset {entry.ruleset_id}: updated_at={rb1.updated_at} sha={rb1.ruleset_sha256}",
            )

        return settled_now

    def _by_id(self, op_id: str) -> RulesetOpEntry:
        for e in self._entries:
            if e.op_id == op_id:
                return e
        raise KeyError(op_id)
