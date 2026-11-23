"""Role-Checked Authority Mediator implementing V75 Phase 0 state machines.

Covers:
- Staging attestation lifecycle (init, reserve, complete, lock/confirm deploy push,
  begin_rollback_a_claim, begin_rollback_b_claim with 409 route_to_rollback_a,
  record/confirm rollback_a_fence_sha, seal_store_for_reset with separate A/B
  preconditions, cancel_staging_attestation, store_instance_id-pinned
  teardown_sealed_store, and staging-only staging_rearm_after_rollback).
- Universal Forward-Cutover Rollback Fence & Step 4(b)(i) Router (R1-R4 + 7 markers).
- Replay dispatch_intents 5-state machine + consumer-side claim_execution fence
  (409 superseded_dispatch_attempt) + supersede_replay_item.
- WORM-first quarantine_pending_status_post & B2 commit_active_grant dual-index
  CancellationReasons precedence.
"""

from __future__ import annotations

import hashlib
import uuid
from dataclasses import dataclass
from typing import Any, Callable

from store import (
    RESET_CHECKED_TABLES,
    ConflictError,
    TransactionalAuthorityStore,
    TransactionCanceledError,
    TxAction,
    WormPreconditionFailed,
    WormStore,
)


PRODUCTION_REPO_ID = 1464323350


@dataclass
class MainHeadProof:
    live_head: str
    first_parent_chain: list[str]  # [live_head, parent1, parent2, ...]

    def reaches_on_first_parent(self, target_sha: str, max_depth: int = 256) -> bool:
        return target_sha in self.first_parent_chain[: max_depth + 1]


class AuthorityMediator:
    def __init__(
        self,
        store: TransactionalAuthorityStore,
        worm: WormStore,
        *,
        bound_repository_id: int,
        staging_repo_id: int | None = None,
        read_main_head: Callable[[], MainHeadProof] | None = None,
        read_commit_tree: Callable[[str], tuple[str, str]] | None = None,
        is_canary1a_probe_absent: Callable[[], bool] | None = None,
        is_handoff_ruleset_active: Callable[[], bool] | None = None,
    ) -> None:
        self.store = store
        self.worm = worm
        self.bound_repository_id = bound_repository_id
        self.staging_repo_id = staging_repo_id
        self._read_main_head = read_main_head or (lambda: MainHeadProof("", []))
        self._read_commit_tree = read_commit_tree or (lambda sha: ("", ""))
        self._is_canary1a_probe_absent = is_canary1a_probe_absent or (lambda: True)
        self._is_handoff_ruleset_active = is_handoff_ruleset_active or (lambda: True)

    @property
    def is_staging(self) -> bool:
        return self.staging_repo_id is not None and self.bound_repository_id == self.staging_repo_id

    # --- Helpers -----------------------------------------------------------------

    def _single_auth_update(
        self,
        *,
        expected_store_instance_id: str,
        expected_rollback_in_progress: bool | None = None,
        expected_version: int | None = None,
        extra_condition: Callable[[dict[str, Any]], bool] | None = None,
        mutate: Callable[[dict[str, Any]], None] | None = None,
    ) -> TxAction:
        def cond(cur: dict[str, Any] | None) -> bool:
            if cur is None:
                return False
            if cur.get("store_instance_id") != expected_store_instance_id:
                return False
            if cur.get("store_sealed_for_reset") is not False:
                return False
            if expected_rollback_in_progress is not None and cur.get("rollback_in_progress") != expected_rollback_in_progress:
                return False
            if expected_version is not None and cur.get("authority_version") != expected_version:
                return False
            if extra_condition is not None and not extra_condition(cur):
                return False
            return True

        def mut(cur: dict[str, Any] | None) -> dict[str, Any]:
            assert cur is not None
            if mutate is not None:
                mutate(cur)
            cur["authority_version"] = int(cur.get("authority_version", 0)) + 1
            return cur

        return TxAction("Update", self.store.SINGLETON_TABLE, self.store.SINGLETON_KEY, condition=cond, mutator=mut)

    def _verify_prior_attempts_restored(self, current_attempt_id: str) -> None:
        if self.is_staging:
            return
        for attempt_id, row in self.store.scan_table(self.store.ATTESTATIONS_TABLE).items():
            if attempt_id == current_attempt_id:
                continue
            if row.get("status") != "cancelled":
                raise ConflictError("prior_rollback_incomplete", f"Prior attempt {attempt_id} has status {row.get('status')}")
            if self.worm.exists(f"bootstrap/rollback_progress/{attempt_id}/rollback_a_complete.json"):
                continue
            b_keys = self.worm.list_prefix(f"bootstrap/rollback_progress/{attempt_id}/")
            dirs = {k.split("/")[3] for k in b_keys if len(k.split("/")) >= 5}
            if len(dirs) > 1:
                raise ConflictError("prior_rollback_incomplete", f"Multiple rollback_attempt_ids for {attempt_id}")
            if len(dirs) == 1:
                rb_id = next(iter(dirs))
                base = f"bootstrap/rollback_progress/{attempt_id}/{rb_id}"
                if self.worm.exists(f"{base}/legacy_verified_b5.json") or self.worm.exists(f"{base}/rollback_b_complete.json"):
                    continue
            raise ConflictError("prior_rollback_incomplete", f"Prior attempt {attempt_id} lacks terminal rollback receipt")

    # --- Staging Attestation & Store Init ----------------------------------------

    def init_staging_attestation(
        self,
        cutover_attempt_id: str,
        *,
        staging_config_digest: str,
        staging_retry_mode: bool = False,
        legacy_cutoff_staging_verified: bool = True,
        head_push_cancel_staging_verified: bool = True,
        ruleset_put_barrier_staging_verified: bool = True,
        audit_log_api_verified: bool = True,
        close_reopen_cancel_staging_verified: bool = False,
    ) -> dict[str, Any]:
        if staging_retry_mode and not self.is_staging:
            raise ConflictError("staging_retry_forbidden_on_production")
        self._verify_prior_attempts_restored(cutover_attempt_id)

        row = {
            "cutover_attempt_id": cutover_attempt_id,
            "highest_seq": 0,
            "status": "passed",
            "deploy_push_locked_seq": None,
            "deploy_push_parent_sha": None,
            "deploy_push_tip_sha": None,
            "deploy_push_confirmed_tip_sha": None,
            "rollback_a_fence_sha": None,
            "rollback_a_fence_verified_on_main": False,
            "staging_retry_mode": staging_retry_mode,
            "staging_config_digest": staging_config_digest,
            "legacy_cutoff_staging_verified": legacy_cutoff_staging_verified,
            "head_push_cancel_staging_verified": head_push_cancel_staging_verified,
            "ruleset_put_barrier_staging_verified": ruleset_put_barrier_staging_verified,
            "audit_log_api_verified": audit_log_api_verified,
            "close_reopen_cancel_staging_verified": close_reopen_cancel_staging_verified,
        }
        try:
            res = self.store.transact_write(
                [
                    TxAction(
                        "Put",
                        self.store.ATTESTATIONS_TABLE,
                        cutover_attempt_id,
                        condition=lambda cur: cur is None,
                        mutator=lambda _: row,
                        fence_code="attestation_already_exists",
                    )
                ]
            )
            return res[(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)]  # type: ignore[return-value]
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(exc) from None

    def initialize_bootstrap_store(
        self,
        cutover_attempt_id: str,
        *,
        expected_seq: int,
        anchor_main_sha: str,
        store_instance_id: str | None = None,
    ) -> dict[str, Any]:
        """Step 3(a): assert all reset-checked tables empty, then atomically bind to passed attestation."""
        empty, non_empty = self.store.all_reset_checked_tables_empty()
        if not empty:
            raise ConflictError("reset_checked_tables_non_empty", f"Orphan rows in: {non_empty}")

        prior = self.store.get_item(self.store.SINGLETON_TABLE, self.store.SINGLETON_KEY)
        if prior is not None:
            if prior.get("cutover_attempt_id") != cutover_attempt_id:
                if not prior.get("store_sealed_for_reset"):
                    raise ConflictError("unsealed_prior_attempt_store")
                raise ConflictError("prior_sealed_store_not_torn_down")

        sid = store_instance_id or str(uuid.uuid4())
        state = {
            "store_instance_id": sid,
            "cutover_attempt_id": cutover_attempt_id,
            "authority_version": 1,
            "store_sealed_for_reset": False,
            "rollback_in_progress": False,
            "rollback_attempt_seq": 0,
            "rollback_attempt_id": None,
            "rollback_phase": None,
            "rollback_head_sha": None,
            "rollback_revert_sha": None,
            "rollback_revert_verified_at_seal": False,
            "last_cursor_sha": anchor_main_sha,
            "workflow_epoch_base_sha": None,
            "control_ci_tools_base_sha": None,
            "control_plane_epoch": 0,
            "enqueue_allowlist": "NONE",
            "precutover_fence_complete": False,
            "step4b_replay_complete": False,
            "step4b_replay_manifest_sha256": None,
            "step4b_canary1a_passed": False,
            "step4b_canary1a_evidence_ref": None,
            "step4b_handoff_complete": False,
            "freeze_causes": {"drain:phase0-bootstrap": {"kind": "drain", "drain_tip_sha": None}},
            "active_grant": None,
        }
        try:
            res = self.store.transact_write(
                [
                    TxAction(
                        "ConditionCheck",
                        self.store.ATTESTATIONS_TABLE,
                        cutover_attempt_id,
                        condition=lambda cur: (
                            cur is not None
                            and cur.get("highest_seq") == expected_seq
                            and cur.get("status") == "passed"
                            and cur.get("deploy_push_locked_seq") is None
                            and cur.get("deploy_push_confirmed_tip_sha") is None
                        ),
                        fence_code="attestation_claimed_or_cancelled",
                    ),
                    TxAction(
                        "Put",
                        self.store.SINGLETON_TABLE,
                        self.store.SINGLETON_KEY,
                        condition=lambda cur: cur is None,
                        mutator=lambda _: state,
                        fence_code="authority_state_already_exists",
                    ),
                ]
            )
            return res[(self.store.SINGLETON_TABLE, self.store.SINGLETON_KEY)]  # type: ignore[return-value]
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(exc) from None

    # --- Deploy Lock & Rollback A/B Claims ---------------------------------------

    def lock_deploy_push(
        self, cutover_attempt_id: str, expected_seq: int, parent_sha: str, tip_sha: str
    ) -> dict[str, Any]:
        def cond(cur: dict[str, Any] | None) -> bool:
            if cur is None:
                return False
            if cur.get("highest_seq") != expected_seq or cur.get("status") != "passed":
                return False
            if cur.get("deploy_push_confirmed_tip_sha") is not None:
                return False
            locked = cur.get("deploy_push_locked_seq")
            if locked is None:
                return True
            return (
                locked == expected_seq
                and cur.get("deploy_push_parent_sha") == parent_sha
                and cur.get("deploy_push_tip_sha") == tip_sha
            )

        def mut(cur: dict[str, Any] | None) -> dict[str, Any]:
            assert cur is not None
            cur["deploy_push_locked_seq"] = expected_seq
            cur["deploy_push_parent_sha"] = parent_sha
            cur["deploy_push_tip_sha"] = tip_sha
            return cur

        try:
            res = self.store.transact_write(
                [TxAction("Update", self.store.ATTESTATIONS_TABLE, cutover_attempt_id, condition=cond, mutator=mut, fence_code="attestation_claimed_or_cancelled")]
            )
            return res[(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)]  # type: ignore[return-value]
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(exc) from None

    def confirm_deploy_push_landed(self, cutover_attempt_id: str, expected_seq: int, tip_sha: str) -> dict[str, Any]:
        head = self._read_main_head()
        if head.live_head != tip_sha:
            raise ConflictError("main_head_mismatch", f"Expected live main {tip_sha}, found {head.live_head}")

        def cond(cur: dict[str, Any] | None) -> bool:
            if cur is None:
                return False
            if cur.get("status") in ("rollback_a_claimed", "rollback_b_claimed", "cancelled"):
                return False
            if cur.get("deploy_push_confirmed_tip_sha") == tip_sha and cur.get("deploy_push_locked_seq") is None:
                return True
            return (
                cur.get("status") == "passed"
                and cur.get("deploy_push_locked_seq") == expected_seq
                and cur.get("deploy_push_tip_sha") == tip_sha
                and cur.get("deploy_push_confirmed_tip_sha") is None
            )

        def mut(cur: dict[str, Any] | None) -> dict[str, Any]:
            assert cur is not None
            cur["deploy_push_confirmed_tip_sha"] = tip_sha
            cur["deploy_push_locked_seq"] = None
            return cur

        try:
            res = self.store.transact_write(
                [TxAction("Update", self.store.ATTESTATIONS_TABLE, cutover_attempt_id, condition=cond, mutator=mut, fence_code="attestation_claimed_or_cancelled")]
            )
            return res[(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)]  # type: ignore[return-value]
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(exc) from None

    def begin_rollback_a_claim(self, cutover_attempt_id: str) -> dict[str, Any]:
        def mut(cur: dict[str, Any] | None) -> dict[str, Any]:
            assert cur is not None
            if cur.get("status") in ("in_progress", "passed", "failed"):
                cur["status"] = "rollback_a_claimed"
            return cur

        try:
            res = self.store.transact_write(
                [TxAction("Update", self.store.ATTESTATIONS_TABLE, cutover_attempt_id, condition=lambda cur: cur is not None, mutator=mut, fence_code="attestation_not_found")]
            )
            return res[(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)]  # type: ignore[return-value]
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(exc) from None

    def begin_rollback_b_claim(self, cutover_attempt_id: str) -> dict[str, Any]:
        """Idempotent-first on rollback_b_claimed/cancelled; server-side guards against hijacking Rollback A."""
        cur = self.store.get_item(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)
        if cur is None:
            raise ConflictError("attestation_not_found")
        if cur.get("status") in ("rollback_b_claimed", "cancelled"):
            return cur

        confirmed = cur.get("deploy_push_confirmed_tip_sha")
        tip = cur.get("deploy_push_tip_sha")
        fence = cur.get("rollback_a_fence_sha")
        head = self._read_main_head()
        tip_on_main = confirmed is not None or (tip is not None and head.reaches_on_first_parent(tip))
        if not tip_on_main:
            raise ConflictError("route_to_rollback_a", "Deploy tip has not landed on first-parent main")
        if cur.get("status") == "rollback_a_claimed" and fence is not None and head.reaches_on_first_parent(fence):
            raise ConflictError("route_to_rollback_a", "Rollback A fence F_abort is on main")

        def mut(item: dict[str, Any] | None) -> dict[str, Any]:
            assert item is not None
            if item.get("status") in ("in_progress", "passed", "failed", "rollback_a_claimed"):
                item["status"] = "rollback_b_claimed"
            return item

        res = self.store.transact_write(
            [TxAction("Update", self.store.ATTESTATIONS_TABLE, cutover_attempt_id, condition=lambda item: item is not None, mutator=mut)]
        )
        return res[(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)]  # type: ignore[return-value]

    def record_rollback_a_fence_sha(self, cutover_attempt_id: str, fence_sha: str) -> dict[str, Any]:
        cur = self.store.get_item(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)
        if cur is None or cur.get("status") != "rollback_a_claimed":
            raise ConflictError("rollback_a_not_claimed")
        if cur.get("deploy_push_locked_seq") is None or cur.get("deploy_push_confirmed_tip_sha") is not None:
            raise ConflictError("invalid_fence_precondition")
        parent_sha = cur.get("deploy_push_parent_sha")
        f_parent, f_tree = self._read_commit_tree(fence_sha)
        _, p_tree = self._read_commit_tree(parent_sha)
        if f_parent != parent_sha or f_tree != p_tree:
            raise ConflictError("invalid_fence_commit")

        def cond(item: dict[str, Any] | None) -> bool:
            return (
                item is not None
                and item.get("status") == "rollback_a_claimed"
                and item.get("deploy_push_confirmed_tip_sha") is None
                and (item.get("rollback_a_fence_sha") is None or item.get("rollback_a_fence_sha") == fence_sha)
            )

        def mut(item: dict[str, Any] | None) -> dict[str, Any]:
            assert item is not None
            item["rollback_a_fence_sha"] = fence_sha
            return item

        try:
            res = self.store.transact_write(
                [TxAction("Update", self.store.ATTESTATIONS_TABLE, cutover_attempt_id, condition=cond, mutator=mut, fence_code="invalid_fence_precondition")]
            )
            return res[(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)]  # type: ignore[return-value]
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(exc) from None

    def confirm_rollback_a_fence_landed(self, cutover_attempt_id: str) -> dict[str, Any]:
        """Verify live_main == rollback_a_fence_sha at depth 0 while main is still frozen, before A(iv) live."""
        cur = self.store.get_item(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)
        if cur is None or cur.get("status") != "rollback_a_claimed" or cur.get("deploy_push_confirmed_tip_sha") is not None:
            raise ConflictError("invalid_fence_precondition")
        fence_sha = cur.get("rollback_a_fence_sha")
        if not fence_sha:
            raise ConflictError("missing_rollback_a_fence_sha")
        head = self._read_main_head()
        if head.live_head != fence_sha:
            raise ConflictError("fence_not_at_main_head")

        def mut(item: dict[str, Any] | None) -> dict[str, Any]:
            assert item is not None
            item["rollback_a_fence_verified_on_main"] = True
            item["deploy_push_locked_seq"] = None
            return item

        res = self.store.transact_write(
            [TxAction("Update", self.store.ATTESTATIONS_TABLE, cutover_attempt_id, condition=lambda item: item is not None and item.get("status") == "rollback_a_claimed", mutator=mut)]
        )
        return res[(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)]  # type: ignore[return-value]

    # --- Rollback Freeze, Seal, Cancel, Teardown, Staging Re-arm -----------------

    def enter_rollback_freeze(self, expected_store_instance_id: str) -> dict[str, Any]:
        def mutate(cur: dict[str, Any]) -> None:
            if not cur.get("rollback_in_progress"):
                cur["rollback_attempt_seq"] = int(cur.get("rollback_attempt_seq", 0)) + 1
                cur["rollback_attempt_id"] = cur["rollback_attempt_seq"]
                cur["rollback_phase"] = "frozen_b0"
                cur["rollback_head_sha"] = None
                cur["rollback_revert_sha"] = None
                cur["rollback_revert_verified_at_seal"] = False
                cur["rollback_in_progress"] = True
            elif cur.get("rollback_phase") is None:
                cur["rollback_phase"] = "frozen_b0"
            cur["enqueue_allowlist"] = "NONE"
            cur.setdefault("freeze_causes", {})["drain:phase0-bootstrap"] = {"kind": "drain", "drain_tip_sha": None}

        try:
            res = self.store.transact_write([self._single_auth_update(expected_store_instance_id=expected_store_instance_id, mutate=mutate)])
            return res[(self.store.SINGLETON_TABLE, self.store.SINGLETON_KEY)]  # type: ignore[return-value]
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(exc, expected_store_instance_id=expected_store_instance_id) from None

    def seal_store_for_reset(
        self,
        *,
        cutover_attempt_id: str,
        expected_store_instance_id: str,
        expected_version: int,
        branch: str,  # "rollback_a" | "rollback_b"
    ) -> dict[str, Any]:
        att = self.store.get_item(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)
        auth = self.store.get_item(self.store.SINGLETON_TABLE, self.store.SINGLETON_KEY)
        if att is None or auth is None:
            raise ConflictError("missing_state_for_seal")

        if branch == "rollback_a":
            if att.get("status") != "rollback_a_claimed" or att.get("deploy_push_confirmed_tip_sha") is not None:
                raise ConflictError("invalid_rollback_a_seal_state")
            no_deploy = att.get("deploy_push_tip_sha") is None and att.get("deploy_push_locked_seq") is None
            if not (no_deploy or att.get("rollback_a_fence_verified_on_main") is True):
                raise ConflictError("rollback_a_fence_unverified")
        elif branch == "rollback_b":
            if att.get("status") != "rollback_b_claimed":
                raise ConflictError("rollback_b_not_claimed")
            revert_sha = auth.get("rollback_revert_sha")
            if not auth.get("rollback_in_progress") or not revert_sha:
                raise ConflictError("rollback_b_revert_missing")
            head = self._read_main_head()
            if head.live_head != revert_sha:
                raise ConflictError("rollback_b_revert_not_at_frozen_main")
        else:
            raise ValueError(f"Unknown seal branch: {branch}")

        def cond(cur: dict[str, Any] | None) -> bool:
            if cur is None:
                return False
            if cur.get("store_instance_id") != expected_store_instance_id:
                return False
            if cur.get("store_sealed_for_reset") is not False:
                return False
            if cur.get("authority_version") != expected_version:
                return False
            if branch == "rollback_b":
                return cur.get("rollback_in_progress") is True and cur.get("rollback_revert_sha") is not None
            return "drain:phase0-bootstrap" in cur.get("freeze_causes", {})

        def mut(cur: dict[str, Any] | None) -> dict[str, Any]:
            assert cur is not None
            cur["store_sealed_for_reset"] = True
            if branch == "rollback_b":
                cur["rollback_revert_verified_at_seal"] = True
                cur["rollback_phase"] = "sealed_b3_5"
            cur["authority_version"] = int(cur.get("authority_version", 0)) + 1
            return cur

        try:
            res = self.store.transact_write([TxAction("Update", self.store.SINGLETON_TABLE, self.store.SINGLETON_KEY, condition=cond, mutator=mut)])
            return res[(self.store.SINGLETON_TABLE, self.store.SINGLETON_KEY)]  # type: ignore[return-value]
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(
                exc,
                expected_store_instance_id=expected_store_instance_id,
                expected_version=expected_version,
            ) from None

    def cancel_staging_attestation(self, cutover_attempt_id: str) -> dict[str, Any]:
        att = self.store.get_item(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)
        if att is None:
            raise ConflictError("attestation_not_found")
        if att.get("status") == "cancelled":
            return att

        status = att.get("status")
        if status == "rollback_a_claimed":
            if att.get("deploy_push_confirmed_tip_sha") is not None:
                raise ConflictError("deploy_already_confirmed")
            no_deploy = att.get("deploy_push_tip_sha") is None and att.get("deploy_push_locked_seq") is None
            if not (no_deploy or att.get("rollback_a_fence_verified_on_main") is True):
                raise ConflictError("rollback_a_fence_unverified")
        elif status == "rollback_b_claimed":
            auth = self.store.get_item(self.store.SINGLETON_TABLE, self.store.SINGLETON_KEY)
            if (
                auth is None
                or auth.get("cutover_attempt_id") != cutover_attempt_id
                or auth.get("store_sealed_for_reset") is not True
                or auth.get("rollback_revert_verified_at_seal") is not True
                or not auth.get("rollback_revert_sha")
            ):
                raise ConflictError("rollback_b_seal_required_before_cancel")
            head = self._read_main_head()
            if head.live_head != auth["rollback_revert_sha"]:
                raise ConflictError("rollback_b_revert_not_at_frozen_main")
        else:
            raise ConflictError("cannot_cancel_unclaimed_attempt")

        def mut(cur: dict[str, Any] | None) -> dict[str, Any]:
            assert cur is not None
            cur["status"] = "cancelled"
            cur["deploy_push_locked_seq"] = None
            return cur

        res = self.store.transact_write(
            [TxAction("Update", self.store.ATTESTATIONS_TABLE, cutover_attempt_id, condition=lambda cur: cur is not None, mutator=mut)]
        )
        return res[(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)]  # type: ignore[return-value]

    def teardown_sealed_store(self, cutover_attempt_id: str, expected_store_instance_id: str) -> None:
        """Store-instance-pinned table-first teardown deleting authority_state LAST."""
        att = self.store.get_item(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)
        auth = self.store.get_item(self.store.SINGLETON_TABLE, self.store.SINGLETON_KEY)
        if att is None or att.get("status") != "cancelled":
            raise ConflictError("attestation_not_cancelled")
        if auth is None or auth.get("store_instance_id") != expected_store_instance_id or auth.get("store_sealed_for_reset") is not True:
            raise ConflictError("store_instance_mismatch")
        if auth.get("rollback_phase") == "sealed_b3_5":
            rb_id = auth.get("rollback_attempt_id")
            if not self.worm.exists(f"bootstrap/rollback_progress/{cutover_attempt_id}/{rb_id}/legacy_verified_b5.json"):
                raise ConflictError("prior_rollback_incomplete")

        auth_check = TxAction(
            "ConditionCheck",
            self.store.SINGLETON_TABLE,
            self.store.SINGLETON_KEY,
            condition=lambda cur: (
                cur is not None
                and cur.get("cutover_attempt_id") == cutover_attempt_id
                and cur.get("store_instance_id") == expected_store_instance_id
                and cur.get("store_sealed_for_reset") is True
            ),
            fence_code="store_instance_mismatch",
        )

        # Delete per-key tables FIRST in batches conditioned on the exact sealed store_instance_id
        for table in RESET_CHECKED_TABLES:
            keys = list(self.store.scan_table(table).keys())
            for i in range(0, len(keys), 50):
                batch = [auth_check] + [TxAction("Delete", table, k) for k in keys[i : i + 50]]
                try:
                    self.store.transact_write(batch)
                except TransactionCanceledError as exc:
                    raise self.store.classify_cancellation(exc, expected_store_instance_id=expected_store_instance_id) from None

        empty, non_empty = self.store.all_reset_checked_tables_empty()
        if not empty:
            raise ConflictError("teardown_incomplete", f"Tables still non-empty: {non_empty}")

        # Delete authority_state LAST conditioned on exact sealed store_instance_id
        try:
            self.store.transact_write(
                [
                    TxAction(
                        "Delete",
                        self.store.SINGLETON_TABLE,
                        self.store.SINGLETON_KEY,
                        condition=auth_check.condition,
                        fence_code="store_instance_mismatch",
                    )
                ]
            )
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(exc, expected_store_instance_id=expected_store_instance_id) from None

    def staging_rearm_after_rollback(
        self, cutover_attempt_id: str, expected_store_instance_id: str, expected_version: int
    ) -> dict[str, Any]:
        """Staging-only atomic 2-table reset (authority_state + staging_attestations)."""
        if not self.is_staging:
            raise ConflictError("staging_only_operation")

        def mutate_auth(cur: dict[str, Any]) -> None:
            cur["rollback_in_progress"] = False
            cur["rollback_attempt_id"] = None
            cur["rollback_phase"] = None
            cur["rollback_head_sha"] = None
            cur["rollback_revert_sha"] = None
            cur["rollback_revert_verified_at_seal"] = False

        def mutate_att(cur: dict[str, Any] | None) -> dict[str, Any]:
            assert cur is not None
            cur["status"] = "passed"
            return cur

        try:
            res = self.store.transact_write(
                [
                    self._single_auth_update(
                        expected_store_instance_id=expected_store_instance_id,
                        expected_rollback_in_progress=True,
                        expected_version=expected_version,
                        mutate=mutate_auth,
                    ),
                    TxAction(
                        "Update",
                        self.store.ATTESTATIONS_TABLE,
                        cutover_attempt_id,
                        condition=lambda cur: (
                            cur is not None and cur.get("staging_retry_mode") is True and cur.get("status") == "rollback_b_claimed"
                        ),
                        mutator=mutate_att,
                        fence_code="invalid_staging_rearm_attestation",
                    ),
                ]
            )
            return res[(self.store.SINGLETON_TABLE, self.store.SINGLETON_KEY)]  # type: ignore[return-value]
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(
                exc,
                expected_store_instance_id=expected_store_instance_id,
                expected_rollback_in_progress=True,
                expected_version=expected_version,
            ) from None

    # --- Forward-Cutover Rollback Fence & Step 4(b)(i) Router --------------------

    def _forward_cutover_tx(
        self,
        cutover_attempt_id: str,
        expected_store_instance_id: str,
        extra_auth_cond: Callable[[dict[str, Any]], bool],
        mutate_auth: Callable[[dict[str, Any]], None],
    ) -> dict[str, Any]:
        try:
            res = self.store.transact_write(
                [
                    TxAction(
                        "ConditionCheck",
                        self.store.ATTESTATIONS_TABLE,
                        cutover_attempt_id,
                        condition=lambda cur: cur is not None and cur.get("status") == "passed",
                        fence_code="rollback_in_progress",
                    ),
                    self._single_auth_update(
                        expected_store_instance_id=expected_store_instance_id,
                        expected_rollback_in_progress=False,
                        extra_condition=lambda cur: cur.get("rollback_phase") is None and extra_auth_cond(cur),
                        mutate=mutate_auth,
                    ),
                ]
            )
            return res[(self.store.SINGLETON_TABLE, self.store.SINGLETON_KEY)]  # type: ignore[return-value]
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(
                exc,
                expected_store_instance_id=expected_store_instance_id,
                expected_rollback_in_progress=False,
            ) from None

    def mark_step4b_replay_complete(
        self, cutover_attempt_id: str, expected_store_instance_id: str, manifest_sha256: str
    ) -> dict[str, Any]:
        att = self.store.get_item(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)
        if att is None or att.get("deploy_push_confirmed_tip_sha") is None:
            raise ConflictError("deploy_not_confirmed")
        confirmed_tip = att["deploy_push_confirmed_tip_sha"]
        manifest_key = f"bootstrap/step4b_progress/{cutover_attempt_id}/replay_manifest.json"
        if self.worm.get_etag(manifest_key) != manifest_sha256:
            raise ConflictError("replay_manifest_digest_mismatch")
        manifest = self.worm.get_json(manifest_key) or {}
        all_keys = [item["item_key"] for item in manifest.get("items", [])]
        for dk in self.worm.list_prefix(f"bootstrap/step4b_progress/{cutover_attempt_id}/derived_items/"):
            derived = self.worm.get_json(dk) or {}
            if "item_key" in derived:
                all_keys.append(derived["item_key"])
        for item_key in all_keys:
            receipt = self.worm.get_json(f"bootstrap/step4b_progress/{cutover_attempt_id}/replay_items/{item_key}.json")
            if receipt is None:
                raise ConflictError("replay_item_receipt_missing", f"Missing receipt for {item_key}")
            disp = receipt.get("disposition")
            if disp == "completed":
                if receipt.get("conclusion") != "success" or not receipt.get("verified_artifact_or_ref_oid"):
                    raise ConflictError("replay_item_not_verified", f"Incomplete receipt for {item_key}")
            elif disp != "superseded":
                raise ConflictError("replay_item_invalid_disposition", f"Bad disposition for {item_key}: {disp}")

        def mutate(cur: dict[str, Any]) -> None:
            cur["step4b_replay_complete"] = True
            cur["step4b_replay_manifest_sha256"] = manifest_sha256

        return self._forward_cutover_tx(
            cutover_attempt_id,
            expected_store_instance_id,
            lambda cur: cur.get("workflow_epoch_base_sha") == confirmed_tip,
            mutate,
        )

    def mark_step4b_canary1a_passed(
        self, cutover_attempt_id: str, expected_store_instance_id: str, evidence_ref: str
    ) -> dict[str, Any]:
        if not self.worm.exists(f"bootstrap/step4b_progress/{cutover_attempt_id}/canary1a_probes_passed.json"):
            raise ConflictError("canary1a_probes_receipt_missing")
        if not self.worm.exists(evidence_ref):
            raise ConflictError("canary1a_evidence_missing")
        if not self._is_canary1a_probe_absent():
            raise ConflictError("canary1a_probe_still_installed")

        def mutate(cur: dict[str, Any]) -> None:
            cur["step4b_canary1a_passed"] = True
            cur["step4b_canary1a_evidence_ref"] = evidence_ref

        return self._forward_cutover_tx(
            cutover_attempt_id,
            expected_store_instance_id,
            lambda cur: cur.get("step4b_replay_complete") is True,
            mutate,
        )

    def mark_step4b_handoff_complete(self, cutover_attempt_id: str, expected_store_instance_id: str) -> dict[str, Any]:
        if not self._is_handoff_ruleset_active():
            raise ConflictError("handoff_ruleset_not_active")

        def mutate(cur: dict[str, Any]) -> None:
            cur["step4b_handoff_complete"] = True

        return self._forward_cutover_tx(
            cutover_attempt_id,
            expected_store_instance_id,
            lambda cur: cur.get("step4b_canary1a_passed") is True,
            mutate,
        )

    def route_step4b_entry(self, cutover_attempt_id: str, cutover_sha: str) -> str:
        """Step 4(b)(i) Router (R1-R4) evaluated strictly before pre-push checks."""
        att = self.store.get_item(self.store.ATTESTATIONS_TABLE, cutover_attempt_id)
        auth = self.store.get_item(self.store.SINGLETON_TABLE, self.store.SINGLETON_KEY)
        if att is None:
            return "halt:missing_attestation"

        # R1: Durable Rollback / Cancel Gate FIRST
        status = att.get("status")
        if status == "rollback_a_claimed":
            return "handoff:rollback_a"
        if status == "rollback_b_claimed":
            return "handoff:rollback_b"
        if status == "cancelled":
            return "halt:cancelled"
        if auth is not None and auth.get("cutover_attempt_id") == cutover_attempt_id:
            if auth.get("rollback_in_progress") is True or auth.get("rollback_phase") is not None:
                return "handoff:rollback_b"

        head = self._read_main_head()
        confirmed = att.get("deploy_push_confirmed_tip_sha")
        # R2: Confirmed deploy -> 7-marker durable resume
        if confirmed is not None:
            if att.get("deploy_push_tip_sha") != confirmed or not head.reaches_on_first_parent(confirmed):
                return "handoff:rollback_b"
            assert auth is not None
            if not auth.get("precutover_fence_complete"):
                return "resume:step4b_ii_fence"
            if auth.get("workflow_epoch_base_sha") != confirmed:
                return "resume:step4b_iii_epoch"
            if not auth.get("step4b_replay_complete"):
                return "resume:step4b_iv_replay"
            if not auth.get("step4b_canary1a_passed"):
                return "resume:step4b_v_canary1a"
            if not auth.get("step4b_handoff_complete"):
                return "resume:step4b_vi_handoff"
            if "drain:phase0-bootstrap" in auth.get("freeze_causes", {}):
                return "resume:step4b_vii_unfreeze"
            return "complete:step5"

        # R3: Active deploy lock
        if att.get("deploy_push_locked_seq") is not None:
            if head.live_head == att.get("deploy_push_tip_sha"):
                return "confirm:deploy_push_landed"
            if head.live_head == att.get("deploy_push_parent_sha"):
                return "push:deploy_series"
            return "handoff:rollback_check"

        # R4: Unexpected unconfirmed main movement
        if head.live_head != cutover_sha:
            return "halt:step4b_unexpected_main_head"
        return "push:deploy_series"

    # --- Replay dispatch_intents + Consumer-Side Execution Fence -----------------

    @staticmethod
    def _dispatch_key(cutover_attempt_id: str, item_key: str, attempt_seq: int) -> str:
        return hashlib.sha256(f"{cutover_attempt_id}:{item_key}:a{attempt_seq}".encode("utf-8")).hexdigest()

    def begin_replay_dispatch(
        self,
        *,
        cutover_attempt_id: str,
        expected_store_instance_id: str,
        item_key: str,
        workflow_id: str,
        event_type: str,
        token_id: str,
        token_expires_at: int,
        started_at: int,
    ) -> dict[str, Any]:
        """Atomically create attempt 1 OR advance from not_accepted_dead_token -> next attempt."""
        cur = self.store.get_item("dispatch_intents", item_key)
        if cur is None:
            next_seq = 1
        elif cur.get("state") == "not_accepted_dead_token":
            next_seq = int(cur["current_attempt_seq"]) + 1
            if next_seq > 3:
                raise ConflictError("step4b_replay_item_failed", f"Exhausted max_item_retries=3 for {item_key}")
        else:
            raise ConflictError("dispatch_intent_not_dispatchable", f"{item_key} in state {cur.get('state')}")

        active_key = self._dispatch_key(cutover_attempt_id, item_key, next_seq)
        new_row = {
            "item_key": item_key,
            "workflow_id": workflow_id,
            "event_type": event_type,
            "current_attempt_seq": next_seq,
            "active_key": active_key,
            "state": "in_flight",
            "workflow_run_id": None,
            "token_id": token_id,
            "token_expires_at": token_expires_at,
            "started_at": started_at,
        }
        try:
            res = self.store.transact_write(
                [
                    self._single_auth_update(
                        expected_store_instance_id=expected_store_instance_id,
                        expected_rollback_in_progress=False,
                    ),
                    TxAction(
                        "Put",
                        "dispatch_intents",
                        item_key,
                        condition=lambda item: item is None or item.get("state") == "not_accepted_dead_token",
                        mutator=lambda _: new_row,
                        fence_code="dispatch_intent_not_dispatchable",
                    ),
                ]
            )
            return res[("dispatch_intents", item_key)]  # type: ignore[return-value]
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(
                exc, expected_store_instance_id=expected_store_instance_id, expected_rollback_in_progress=False
            ) from None

    def mark_dispatch_not_accepted(self, expected_store_instance_id: str, item_key: str, expected_active_key: str) -> dict[str, Any]:
        def mut(item: dict[str, Any] | None) -> dict[str, Any]:
            assert item is not None
            item["state"] = "not_accepted_dead_token"
            return item

        try:
            res = self.store.transact_write(
                [
                    self._single_auth_update(expected_store_instance_id=expected_store_instance_id, expected_rollback_in_progress=False),
                    TxAction(
                        "Update",
                        "dispatch_intents",
                        item_key,
                        condition=lambda item: item is not None and item.get("active_key") == expected_active_key and item.get("state") == "in_flight",
                        mutator=mut,
                        fence_code="dispatch_intent_mismatch",
                    ),
                ]
            )
            return res[("dispatch_intents", item_key)]  # type: ignore[return-value]
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(
                exc, expected_store_instance_id=expected_store_instance_id, expected_rollback_in_progress=False
            ) from None

    def claim_dispatch_execution(self, item_key: str, dispatch_idempotency_key: str) -> dict[str, Any]:
        """Consumer-side item fence: rejects any late-visible run from a rotated/completed attempt."""
        row = self.store.get_item("dispatch_intents", item_key)
        if row is None:
            raise ConflictError("superseded_dispatch_attempt")
        if row.get("state") in ("completed", "superseded") or row.get("active_key") != dispatch_idempotency_key:
            raise ConflictError("superseded_dispatch_attempt")
        return row

    # --- WORM-First Status Quarantine & B2 Dual-Index Precedence -----------------

    def quarantine_pending_status_post(
        self,
        *,
        sha: str,
        op_id: str,
        expected_store_instance_id: str,
        require_rollback_in_progress: bool = True,
    ) -> dict[str, Any]:
        row_key = f"{sha}:{op_id}"
        row = self.store.get_item("pending_status_posts", row_key)
        if row is None:
            raise ConflictError("pending_status_post_not_found")
        worm_key = f"incidents/status_quarantine/{sha}/{op_id}.json"
        worm_doc = self.worm.get_json(worm_key)
        if worm_doc is None:
            raise ConflictError("worm_quarantine_record_missing")

        identity_fields = ("sha", "op_id", "kind", "grant_key", "token_id", "expires_at", "started_at", "cutover_attempt_id")
        for field_name in identity_fields:
            if worm_doc.get(field_name) != row.get(field_name):
                raise ConflictError("worm_identity_mismatch", f"Mismatch on {field_name}")

        kind = row["kind"]

        def mutate_row(cur: dict[str, Any] | None) -> dict[str, Any]:
            assert cur is not None
            cur["state"] = "quarantined_status_sha"
            return cur

        def mutate_fence(cur: dict[str, Any] | None) -> dict[str, Any]:
            base = cur or {"sha": sha, "kinds": [], "op_ids": []}
            if kind not in base["kinds"]:
                base["kinds"].append(kind)
            if op_id not in base["op_ids"]:
                base["op_ids"].append(op_id)
            return base

        try:
            res = self.store.transact_write(
                [
                    self._single_auth_update(
                        expected_store_instance_id=expected_store_instance_id,
                        expected_rollback_in_progress=require_rollback_in_progress,
                    ),
                    TxAction(
                        "Update",
                        "pending_status_posts",
                        row_key,
                        condition=lambda cur: cur is not None and cur.get("state") in ("in_flight", "quarantined_status_sha"),
                        mutator=mutate_row,
                        fence_code="pending_status_not_in_flight",
                    ),
                    TxAction("Update", "fenced_status_shas", sha, mutator=mutate_fence),
                ]
            )
            return res[("fenced_status_shas", sha)]  # type: ignore[return-value]
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(
                exc,
                expected_store_instance_id=expected_store_instance_id,
                expected_rollback_in_progress=require_rollback_in_progress,
            ) from None

    def commit_b2_grant(
        self,
        *,
        expected_store_instance_id: str,
        expected_version: int,
        group_commit_sha: str,
        grant_key: str,
    ) -> dict[str, Any]:
        """B2 atomic grant commit proving fence index beats optimistic authority_version CAS."""
        def mutate_auth(cur: dict[str, Any]) -> None:
            cur["active_grant"] = {
                "grant_key": grant_key,
                "group_commit_sha": group_commit_sha,
                "status": "cas_committed",
                "revoked": False,
                "eviction_pending": False,
            }

        try:
            res = self.store.transact_write(
                [
                    self._single_auth_update(
                        expected_store_instance_id=expected_store_instance_id,
                        expected_rollback_in_progress=False,
                        expected_version=expected_version,
                        extra_condition=lambda cur: cur.get("active_grant") is None and cur.get("freeze_causes") == {},
                        mutate=mutate_auth,
                    ),
                    TxAction(
                        "ConditionCheck",
                        "fenced_status_shas",
                        group_commit_sha,
                        condition=lambda cur: cur is None,
                        fence_code="quarantined_status_sha",
                    ),
                    TxAction(
                        "Put",
                        "granted_group_shas",
                        group_commit_sha,
                        condition=lambda cur: cur is None,
                        mutator=lambda _: {"group_commit_sha": group_commit_sha, "grant_key": grant_key},
                        fence_code="group_sha_already_granted",
                    ),
                ]
            )
            return res[(self.store.SINGLETON_TABLE, self.store.SINGLETON_KEY)]  # type: ignore[return-value]
        except TransactionCanceledError as exc:
            raise self.store.classify_cancellation(
                exc,
                expected_store_instance_id=expected_store_instance_id,
                expected_rollback_in_progress=False,
                expected_version=expected_version,
            ) from None
