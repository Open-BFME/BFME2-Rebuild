"""Transactional Authority Store and write-once WORM store contracts.

Enforces DynamoDB TransactWriteItems semantics:
- No two actions in one transaction may target the same (table, key).
- Every mutating Authority Store transaction carries ONE Update on authority_state
  combining store_instance_id, store_sealed_for_reset, role predicates, and
  atomic authority_version += 1.
- ReturnValuesOnConditionCheckFailure = "ALL_OLD" with deterministic multi-item
  CancellationReasons precedence:
    1. Terminal / fence predicates (store_instance_mismatch -> store_sealed_for_reset
       -> rollback_in_progress -> quarantined_status_sha / retired_pr_node / precutover_fenced)
    2. Optimistic CAS (authority_version mismatch)
    3. Transient TransactionConflict
"""

from __future__ import annotations

import copy
import hashlib
import json
from dataclasses import dataclass, field
from typing import Any, Callable


RESET_CHECKED_TABLES = (
    "sealed_submissions",
    "intents",
    "queue_op_intents",
    "dispatch_intents",
    "pending_status_posts",
    "status_tokens",
    "ref_publish_intents",
    "bootstrap_seed_intents",
    "pending_audit_outcomes",
    "pending_landed_commits",
    "worm_outbox",
    "audit_verdicts",
    "granted_group_shas",
    "evicted_granted_pairs",
    "retired_grants",
    "retired_grants_by_pr",
    "retired_pr_attribution",
    "generic_disposed_commits",
    "fenced_status_shas",
    "fenced_precutover_prs",
    "fenced_precutover_shas",
    "precutover_cleared_prs",
)


class WormPreconditionFailed(Exception):
    """Raised on If-None-Match: * collision (HTTP 412)."""


class ConflictError(Exception):
    """Typed HTTP 409 conflict returned by the Authority Mediator."""

    def __init__(self, code: str, message: str = "") -> None:
        super().__init__(message or code)
        self.code = code


@dataclass
class CancellationReason:
    index: int
    table: str
    key: str
    code: str  # "None" | "ConditionalCheckFailed" | "TransactionConflict"
    fence_code: str | None = None
    old_item: dict[str, Any] | None = None


class TransactionCanceledError(Exception):
    """Mirrors DynamoDB TransactionCanceledException with per-action reasons."""

    def __init__(self, reasons: list[CancellationReason]) -> None:
        super().__init__(
            "TransactionCanceledException: "
            + ", ".join(f"[{r.index}:{r.table}/{r.key}={r.code}]" for r in reasons if r.code != "None")
        )
        self.reasons = reasons


@dataclass
class TxAction:
    kind: str  # "ConditionCheck" | "Put" | "Update" | "Delete"
    table: str
    key: str
    condition: Callable[[dict[str, Any] | None], bool] | None = None
    mutator: Callable[[dict[str, Any] | None], dict[str, Any] | None] | None = None
    fence_code: str | None = None


class WormStore:
    """In-memory write-once object store enforcing If-None-Match: * semantics."""

    def __init__(self) -> None:
        self._objects: dict[str, tuple[bytes, str]] = {}

    def put_once(self, key: str, payload: dict[str, Any] | bytes) -> str:
        raw = payload if isinstance(payload, bytes) else json.dumps(payload, sort_keys=True, separators=(",", ":")).encode("utf-8")
        etag = hashlib.sha256(raw).hexdigest()
        if key in self._objects:
            raise WormPreconditionFailed(f"412 Precondition Failed: {key}")
        self._objects[key] = (raw, etag)
        return etag

    def get_json(self, key: str) -> dict[str, Any] | None:
        entry = self._objects.get(key)
        if entry is None:
            return None
        return json.loads(entry[0].decode("utf-8"))

    def get_etag(self, key: str) -> str | None:
        entry = self._objects.get(key)
        return entry[1] if entry is not None else None

    def exists(self, key: str) -> bool:
        return key in self._objects

    def list_prefix(self, prefix: str) -> list[str]:
        return sorted(k for k in self._objects if k.startswith(prefix))


class TransactionalAuthorityStore:
    """Transactional key-value store faithfully simulating DynamoDB TransactWriteItems."""

    SINGLETON_TABLE = "authority_state"
    SINGLETON_KEY = "singleton"
    ATTESTATIONS_TABLE = "staging_attestations"

    def __init__(self) -> None:
        self._tables: dict[str, dict[str, dict[str, Any]]] = {
            self.SINGLETON_TABLE: {},
            self.ATTESTATIONS_TABLE: {},
        }
        for name in RESET_CHECKED_TABLES:
            self._tables[name] = {}

    def get_item(self, table: str, key: str) -> dict[str, Any] | None:
        item = self._tables[table].get(key)
        return copy.deepcopy(item) if item is not None else None

    def scan_table(self, table: str) -> dict[str, dict[str, Any]]:
        return copy.deepcopy(self._tables[table])

    def all_reset_checked_tables_empty(self) -> tuple[bool, list[str]]:
        non_empty = [t for t in RESET_CHECKED_TABLES if self._tables[t]]
        return (len(non_empty) == 0, non_empty)

    def transact_write(self, actions: list[TxAction]) -> dict[tuple[str, str], dict[str, Any] | None]:
        if len(actions) > 100:
            raise ValueError("DynamoDB TransactWriteItems supports at most 100 actions")
        seen_targets: set[tuple[str, str]] = set()
        for act in actions:
            target = (act.table, act.key)
            if target in seen_targets:
                raise ValueError(f"DynamoDB forbids multiple actions on the same item in one transaction: {target}")
            seen_targets.add(target)

        reasons: list[CancellationReason] = []
        failed = False
        for idx, act in enumerate(actions):
            current = self._tables[act.table].get(act.key)
            ok = True if act.condition is None else bool(act.condition(copy.deepcopy(current)))
            if not ok:
                failed = True
                reasons.append(
                    CancellationReason(
                        index=idx,
                        table=act.table,
                        key=act.key,
                        code="ConditionalCheckFailed",
                        fence_code=act.fence_code,
                        old_item=copy.deepcopy(current),
                    )
                )
            else:
                reasons.append(CancellationReason(index=idx, table=act.table, key=act.key, code="None"))

        if failed:
            raise TransactionCanceledError(reasons)

        # Apply atomically
        results: dict[tuple[str, str], dict[str, Any] | None] = {}
        for act in actions:
            if act.kind == "ConditionCheck":
                continue
            current = self._tables[act.table].get(act.key)
            if act.kind == "Delete":
                self._tables[act.table].pop(act.key, None)
                results[(act.table, act.key)] = None
            elif act.kind in ("Put", "Update"):
                assert act.mutator is not None
                updated = act.mutator(copy.deepcopy(current))
                if updated is None:
                    self._tables[act.table].pop(act.key, None)
                else:
                    self._tables[act.table][act.key] = copy.deepcopy(updated)
                results[(act.table, act.key)] = copy.deepcopy(updated)
            else:
                raise ValueError(f"Unknown action kind: {act.kind}")
        return results

    def classify_cancellation(
        self,
        exc: TransactionCanceledError,
        *,
        expected_store_instance_id: str | None = None,
        expected_rollback_in_progress: bool | None = None,
        expected_version: int | None = None,
    ) -> ConflictError:
        """Apply fixed V75 multi-item CancellationReasons precedence.

        1. Terminal / fence predicates FIRST (never retried):
           (a) store_instance_mismatch
           (b) store_sealed_for_reset
           (c) rollback_in_progress mismatch
           (d) item-level enforcement fence (quarantined_status_sha, retired_pr_node, precutover_fenced, ...)
        2. Optimistic CAS (authority_version mismatch) SECOND
        3. Transient TransactionConflict THIRD
        """
        failed = [r for r in exc.reasons if r.code != "None"]
        # Inspect authority_state snapshot (from ALL_OLD or ConsistentRead fallback)
        auth_reason = next((r for r in failed if r.table == self.SINGLETON_TABLE), None)
        if auth_reason is not None:
            snap = auth_reason.old_item if auth_reason.old_item is not None else self.get_item(self.SINGLETON_TABLE, self.SINGLETON_KEY)
            if snap is not None:
                if expected_store_instance_id is not None and snap.get("store_instance_id") != expected_store_instance_id:
                    return ConflictError("store_instance_mismatch")
                if snap.get("store_sealed_for_reset") is True:
                    return ConflictError("store_sealed_for_reset")
                if expected_rollback_in_progress is not None and snap.get("rollback_in_progress") != expected_rollback_in_progress:
                    return ConflictError("rollback_in_progress")

        # 1(d): Item-level Mediator enforcement fences ALWAYS take precedence over optimistic authority_version CAS!
        for r in failed:
            if r.code == "ConditionalCheckFailed" and r.fence_code:
                return ConflictError(r.fence_code)

        # 2: Optimistic CAS
        if auth_reason is not None:
            snap = auth_reason.old_item if auth_reason.old_item is not None else self.get_item(self.SINGLETON_TABLE, self.SINGLETON_KEY)
            if snap is not None and expected_version is not None and snap.get("authority_version") != expected_version:
                return ConflictError("authority_version_conflict")
            if snap is None:
                return ConflictError("authority_state_missing")

        if any(r.code == "TransactionConflict" for r in failed):
            return ConflictError("transaction_conflict")

        return ConflictError("conditional_check_failed")
