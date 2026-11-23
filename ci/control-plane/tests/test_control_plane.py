"""Executable fault-injection & invariant test suite for V75 Phase 0 control plane.

Proves in runnable code (not prose!) the core safety & recovery invariants:
1. Single-Update(authority_state) DynamoDB rule & dual-index B2 CancellationReasons precedence.
2. Pre-lock race, delayed deploy push vs F_abort, and post-confirmation Rollback A refusal.
3. begin_rollback_b_claim idempotent-first & server-side 409 route_to_rollback_a guard.
4. Rollback B(iii 1/2) pre-unfreeze exact-R seal & cancel + store_instance_id-pinned teardown.
5. Prior-attempt restoration gate blocking Attempt #2 until legacy_verified_b5.json exists.
6. Staging-only 2-table staging_rearm_after_rollback (refused on production).
7. Step 4(b)(i) Router R1-R4 + 7 durable markers + forward-cutover rollback fence.
8. Replay dispatch_intents 5-state machine + late-delivery 409 superseded_dispatch_attempt.
9. Ruleset journal: awaiting_audit_claim recovery, cross-ruleset Case 1, same-ruleset <60s Case 2,
   and endpoint-specific barrier requirement.
10. Executable CI workflow lints (a)-(j) scanner against live repo workflows.
"""

from __future__ import annotations

import hashlib
import json
import sys
import tempfile
import unittest
from pathlib import Path

CONTROL_PLANE_DIR = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(CONTROL_PLANE_DIR))

from mediator import PRODUCTION_REPO_ID, AuthorityMediator, MainHeadProof  # noqa: E402
from ruleset_journal import AuditEvent, JournalError, RulesetJournal, RulesetOpEntry, RulesetReadback  # noqa: E402
from store import ConflictError, TransactionalAuthorityStore, TxAction, WormStore  # noqa: E402
from workflow_lints import scan_workflows_dir  # noqa: E402


ANCHOR = "1" * 40
DEPLOY_TIP = "2" * 40
F_ABORT = "3" * 40
REVERT_R = "4" * 40
GROUP_SHA = "5" * 40
STAGING_REPO_ID = 999999999


class ControlPlaneInvariantTests(unittest.TestCase):
    def _make_stack(
        self,
        *,
        repo_id: int = PRODUCTION_REPO_ID,
        live_head: str = ANCHOR,
        chain: list[str] | None = None,
    ):
        store = TransactionalAuthorityStore()
        worm = WormStore()
        head_box = {"proof": MainHeadProof(live_head, chain or [live_head])}
        trees = {
            ANCHOR: ("0" * 40, "t" * 40),
            F_ABORT: (ANCHOR, "t" * 40),
            DEPLOY_TIP: (ANCHOR, "u" * 40),
            REVERT_R: (DEPLOY_TIP, "t" * 40),
        }
        mediator = AuthorityMediator(
            store,
            worm,
            bound_repository_id=repo_id,
            staging_repo_id=STAGING_REPO_ID,
            read_main_head=lambda: head_box["proof"],
            read_commit_tree=lambda sha: trees[sha],
        )
        return store, worm, mediator, head_box

    def test_dynamodb_forbids_two_actions_on_same_item(self):
        store = TransactionalAuthorityStore()
        with self.assertRaises(ValueError):
            store.transact_write(
                [
                    TxAction("ConditionCheck", "authority_state", "singleton"),
                    TxAction("Update", "authority_state", "singleton", mutator=lambda cur: cur),
                ]
            )

    def test_b2_dual_index_cancellation_prefers_quarantined_status_sha_over_version_cas(self):
        store, worm, mediator, _ = self._make_stack()
        mediator.init_staging_attestation("att-1", staging_config_digest="cfg-1")
        auth = mediator.initialize_bootstrap_store("att-1", expected_seq=0, anchor_main_sha=ANCHOR)
        sid = auth["store_instance_id"]
        # Clear bootstrap freeze so B2 preconditions hold
        store._tables["authority_state"]["singleton"]["freeze_causes"] = {}
        snap_version = store.get_item("authority_state", "singleton")["authority_version"]

        # Concurrently quarantine GROUP_SHA (bumping authority_version AND populating fenced_status_shas!)
        row = {
            "sha": GROUP_SHA,
            "op_id": "op-1",
            "kind": "group",
            "grant_key": "gk-1",
            "token_id": "tok-1",
            "expires_at": 1000,
            "started_at": 900,
            "cutover_attempt_id": "att-1",
            "state": "in_flight",
        }
        store._tables["pending_status_posts"][f"{GROUP_SHA}:op-1"] = dict(row)
        worm.put_once(f"incidents/status_quarantine/{GROUP_SHA}/op-1.json", row)
        mediator.quarantine_pending_status_post(
            sha=GROUP_SHA, op_id="op-1", expected_store_instance_id=sid, require_rollback_in_progress=False
        )

        # Now B2 commits with STALE snap_version AND quarantined GROUP_SHA -> MUST get 409 quarantined_status_sha!
        with self.assertRaises(ConflictError) as ctx:
            mediator.commit_b2_grant(
                expected_store_instance_id=sid,
                expected_version=snap_version,
                group_commit_sha=GROUP_SHA,
                grant_key="gk-new",
            )
        self.assertEqual(ctx.exception.code, "quarantined_status_sha")
        self.assertIsNone(store.get_item("authority_state", "singleton")["active_grant"])
        self.assertIsNone(store.get_item("granted_group_shas", GROUP_SHA))

    def test_pre_lock_race_and_f_abort_win_vs_rollback_b_hijack_refusal(self):
        store, worm, mediator, head_box = self._make_stack()
        mediator.init_staging_attestation("att-1", staging_config_digest="cfg-1")
        auth = mediator.initialize_bootstrap_store("att-1", expected_seq=0, anchor_main_sha=ANCHOR)

        # Lock deploy push, then Rollback A claims attempt
        mediator.lock_deploy_push("att-1", 0, ANCHOR, DEPLOY_TIP)
        att = mediator.begin_rollback_a_claim("att-1")
        self.assertEqual(att["status"], "rollback_a_claimed")
        # Step 4(b)(i) router immediately hands off to Rollback A
        self.assertEqual(mediator.route_step4b_entry("att-1", ANCHOR), "handoff:rollback_a")

        # Record F_abort, simulate F_abort landing while main is frozen
        mediator.record_rollback_a_fence_sha("att-1", F_ABORT)
        head_box["proof"] = MainHeadProof(F_ABORT, [F_ABORT, ANCHOR])
        att = mediator.confirm_rollback_a_fence_landed("att-1")
        self.assertTrue(att["rollback_a_fence_verified_on_main"])
        self.assertIsNone(att["deploy_push_locked_seq"])

        # Accidental Rollback B call MUST NOT hijack Rollback A!
        with self.assertRaises(ConflictError) as ctx:
            mediator.begin_rollback_b_claim("att-1")
        self.assertEqual(ctx.exception.code, "route_to_rollback_a")

        # Even if legacy landings later advance main 500 commits past F_abort, A(ii-post) seal + A(v) cancel succeed!
        head_box["proof"] = MainHeadProof("9" * 40, ["9" * 40] * 300)
        fresh = store.get_item("authority_state", "singleton")
        mediator.seal_store_for_reset(
            cutover_attempt_id="att-1",
            expected_store_instance_id=auth["store_instance_id"],
            expected_version=fresh["authority_version"],
            branch="rollback_a",
        )
        att = mediator.cancel_staging_attestation("att-1")
        self.assertEqual(att["status"], "cancelled")
        mediator.teardown_sealed_store("att-1", auth["store_instance_id"])
        self.assertIsNone(store.get_item("authority_state", "singleton"))

    def test_rollback_b_pre_unfreeze_seal_cancel_and_prior_restoration_gate(self):
        store, worm, mediator, head_box = self._make_stack()
        mediator.init_staging_attestation("att-1", staging_config_digest="cfg-1")
        auth = mediator.initialize_bootstrap_store("att-1", expected_seq=0, anchor_main_sha=ANCHOR)
        sid = auth["store_instance_id"]

        mediator.lock_deploy_push("att-1", 0, ANCHOR, DEPLOY_TIP)
        head_box["proof"] = MainHeadProof(DEPLOY_TIP, [DEPLOY_TIP, ANCHOR])
        mediator.confirm_deploy_push_landed("att-1", 0, DEPLOY_TIP)

        # Direct Rollback B claims ownership -> R1 blocks forward restart immediately!
        att = mediator.begin_rollback_b_claim("att-1")
        self.assertEqual(att["status"], "rollback_b_claimed")
        self.assertEqual(mediator.route_step4b_entry("att-1", ANCHOR), "handoff:rollback_b")

        auth = mediator.enter_rollback_freeze(sid)
        self.assertEqual(auth["rollback_phase"], "frozen_b0")
        rb_id = auth["rollback_attempt_id"]
        # Simulate B(iii) pushing R while main is still frozen at R
        store._tables["authority_state"]["singleton"]["rollback_head_sha"] = DEPLOY_TIP
        store._tables["authority_state"]["singleton"]["rollback_revert_sha"] = REVERT_R
        head_box["proof"] = MainHeadProof(REVERT_R, [REVERT_R, DEPLOY_TIP, ANCHOR])

        # B(iii 1/2): seal & cancel at exact live_main == R (depth 0!)
        cur_v = store.get_item("authority_state", "singleton")["authority_version"]
        sealed = mediator.seal_store_for_reset(
            cutover_attempt_id="att-1", expected_store_instance_id=sid, expected_version=cur_v, branch="rollback_b"
        )
        self.assertTrue(sealed["rollback_revert_verified_at_seal"])
        self.assertEqual(sealed["rollback_phase"], "sealed_b3_5")
        mediator.cancel_staging_attestation("att-1")

        # Attempt #2 MUST fail closed while legacy_verified_b5.json is still missing!
        with self.assertRaises(ConflictError) as ctx:
            mediator.init_staging_attestation("att-2", staging_config_digest="cfg-2")
        self.assertEqual(ctx.exception.code, "prior_rollback_incomplete")

        # Teardown also refused before legacy_verified_b5.json!
        with self.assertRaises(ConflictError) as ctx:
            mediator.teardown_sealed_store("att-1", sid)
        self.assertEqual(ctx.exception.code, "prior_rollback_incomplete")

        # After B(v) seals legacy_verified_b5.json, teardown succeeds and Attempt #2 can initialize!
        worm.put_once(f"bootstrap/rollback_progress/att-1/{rb_id}/legacy_verified_b5.json", {"verified": True})
        store._tables["pending_landed_commits"]["c1"] = {"parent_sha": ANCHOR}
        mediator.teardown_sealed_store("att-1", sid)
        self.assertIsNone(store.get_item("authority_state", "singleton"))
        self.assertTrue(store.all_reset_checked_tables_empty()[0])

        # Stale teardown with old sid fails closed against freshly initialized Attempt #2!
        mediator.init_staging_attestation("att-2", staging_config_digest="cfg-2")
        auth2 = mediator.initialize_bootstrap_store("att-2", expected_seq=0, anchor_main_sha=REVERT_R)
        with self.assertRaises(ConflictError) as ctx:
            mediator.teardown_sealed_store("att-1", sid)
        self.assertEqual(ctx.exception.code, "store_instance_mismatch")
        self.assertIsNotNone(store.get_item("authority_state", "singleton"))
        self.assertEqual(store.get_item("authority_state", "singleton")["store_instance_id"], auth2["store_instance_id"])

    def test_staging_rearm_atomic_two_table_reset_forbidden_on_prod(self):
        _, _, prod_med, _ = self._make_stack(repo_id=PRODUCTION_REPO_ID)
        with self.assertRaises(ConflictError) as ctx:
            prod_med.staging_rearm_after_rollback("att-1", "sid-1", 1)
        self.assertEqual(ctx.exception.code, "staging_only_operation")

        store, _, stg_med, head_box = self._make_stack(repo_id=STAGING_REPO_ID)
        stg_med.init_staging_attestation("s2", staging_config_digest="cfg", staging_retry_mode=True)
        auth = stg_med.initialize_bootstrap_store("s2", expected_seq=0, anchor_main_sha=ANCHOR)
        stg_med.lock_deploy_push("s2", 0, ANCHOR, DEPLOY_TIP)
        head_box["proof"] = MainHeadProof(DEPLOY_TIP, [DEPLOY_TIP, ANCHOR])
        stg_med.confirm_deploy_push_landed("s2", 0, DEPLOY_TIP)
        stg_med.begin_rollback_b_claim("s2")
        auth = stg_med.enter_rollback_freeze(auth["store_instance_id"])
        rearmed = stg_med.staging_rearm_after_rollback("s2", auth["store_instance_id"], auth["authority_version"])
        self.assertFalse(rearmed["rollback_in_progress"])
        self.assertEqual(store.get_item("staging_attestations", "s2")["status"], "passed")

    def test_step4b_router_durable_markers_and_replay_late_delivery_fence(self):
        store, worm, mediator, head_box = self._make_stack()
        mediator.init_staging_attestation("att-1", staging_config_digest="cfg-1")
        auth = mediator.initialize_bootstrap_store("att-1", expected_seq=0, anchor_main_sha=ANCHOR)
        sid = auth["store_instance_id"]
        mediator.lock_deploy_push("att-1", 0, ANCHOR, DEPLOY_TIP)
        head_box["proof"] = MainHeadProof(DEPLOY_TIP, [DEPLOY_TIP, ANCHOR])
        mediator.confirm_deploy_push_landed("att-1", 0, DEPLOY_TIP)

        # Router steps cleanly through all 7 durable markers without skipping replay or Canary 1a
        self.assertEqual(mediator.route_step4b_entry("att-1", ANCHOR), "resume:step4b_ii_fence")
        store._tables["authority_state"]["singleton"]["precutover_fence_complete"] = True
        self.assertEqual(mediator.route_step4b_entry("att-1", ANCHOR), "resume:step4b_iii_epoch")
        store._tables["authority_state"]["singleton"]["workflow_epoch_base_sha"] = DEPLOY_TIP
        self.assertEqual(mediator.route_step4b_entry("att-1", ANCHOR), "resume:step4b_iv_replay")

        # Replay dispatch_intents + late-delivery consumer fence!
        manifest_etag = worm.put_once(
            "bootstrap/step4b_progress/att-1/replay_manifest.json",
            {"items": [{"item_key": "pr-10-admit", "workflow_id": "admission.yml", "event_type": "bfme-admit-request"}]},
        )
        a1 = mediator.begin_replay_dispatch(
            cutover_attempt_id="att-1",
            expected_store_instance_id=sid,
            item_key="pr-10-admit",
            workflow_id="admission.yml",
            event_type="bfme-admit-request",
            token_id="dt-1",
            token_expires_at=500,
            started_at=100,
        )
        # Crash/timeout -> mark not_accepted_dead_token -> rotate to a2
        mediator.mark_dispatch_not_accepted(sid, "pr-10-admit", a1["active_key"])
        a2 = mediator.begin_replay_dispatch(
            cutover_attempt_id="att-1",
            expected_store_instance_id=sid,
            item_key="pr-10-admit",
            workflow_id="admission.yml",
            event_type="bfme-admit-request",
            token_id="dt-2",
            token_expires_at=900,
            started_at=300,
        )
        # Late-visible a1 run hits consumer-side item fence -> 409 superseded_dispatch_attempt!
        with self.assertRaises(ConflictError) as ctx:
            mediator.claim_dispatch_execution("pr-10-admit", a1["active_key"])
        self.assertEqual(ctx.exception.code, "superseded_dispatch_attempt")
        # Active a2 run is accepted cleanly!
        self.assertEqual(mediator.claim_dispatch_execution("pr-10-admit", a2["active_key"])["current_attempt_seq"], 2)

        # mark_step4b_replay_complete refuses until verified receipt exists!
        with self.assertRaises(ConflictError) as ctx:
            mediator.mark_step4b_replay_complete("att-1", sid, manifest_etag)
        self.assertEqual(ctx.exception.code, "replay_item_receipt_missing")

        worm.put_once(
            "bootstrap/step4b_progress/att-1/replay_items/pr-10-admit.json",
            {"disposition": "completed", "conclusion": "success", "verified_artifact_or_ref_oid": "f" * 40},
        )
        mediator.mark_step4b_replay_complete("att-1", sid, manifest_etag)
        self.assertEqual(mediator.route_step4b_entry("att-1", ANCHOR), "resume:step4b_v_canary1a")

        worm.put_once("bootstrap/step4b_progress/att-1/canary1a_probes_passed.json", {"ok": True})
        worm.put_once("bootstrap/step4b_progress/att-1/canary1a_evidence.json", {"probe_absent": True})
        mediator.mark_step4b_canary1a_passed("att-1", sid, "bootstrap/step4b_progress/att-1/canary1a_evidence.json")
        self.assertEqual(mediator.route_step4b_entry("att-1", ANCHOR), "resume:step4b_vi_handoff")

        mediator.mark_step4b_handoff_complete("att-1", sid)
        self.assertEqual(mediator.route_step4b_entry("att-1", ANCHOR), "resume:step4b_vii_unfreeze")

    def test_ruleset_journal_sequential_cross_ruleset_and_same_ruleset_within_60s(self):
        with tempfile.TemporaryDirectory() as tmp:
            jpath = Path(tmp) / "rollback_ruleset_ops.jsonl"
            journal = RulesetJournal(jpath)

            audit_stream: list[AuditEvent] = []
            ruleset_state = {
                100: RulesetReadback(100, 1000, "sha-data-0"),
                27758336: RulesetReadback(27758336, 1000, "sha-main-0"),
            }
            dead_tokens: dict[str, int] = {}

            # (3a) Completed 200 OK on data-ref ruleset 100 -> awaiting_audit_claim -> settled
            journal.begin_put(
                RulesetOpEntry(
                    op_id="op-data-1", cutover_attempt_id="att-1", rollback_attempt_id=1, step="B(0)",
                    ruleset_id=100, ruleset_name="data-ref", actor_login="bfme-rollback-admin-app[bot]",
                    pre_put_updated_at=1000, pre_put_ruleset_sha256="sha-data-0", target_sha256="sha-data-1",
                    token_id="tok-1", token_expires_at=4600, started_at=1010,
                ),
                now=1010,
            )
            ruleset_state[100] = RulesetReadback(100, 1011, "sha-data-1")
            audit_stream.append(AuditEvent("ev-data-1", 100, "bfme-rollback-admin-app[bot]", 1011))
            journal.record_http_200("op-data-1", post_put_updated_at=1011)
            # Global serialization blocks next PUT while awaiting_audit_claim!
            with self.assertRaises(JournalError) as ctx:
                journal.begin_put(
                    RulesetOpEntry(
                        op_id="op-main-early", cutover_attempt_id="att-1", rollback_attempt_id=1, step="B(0b)",
                        ruleset_id=27758336, ruleset_name="main", actor_login="bfme-rollback-admin-app[bot]",
                        pre_put_updated_at=1000, pre_put_ruleset_sha256="sha-main-0", target_sha256="sha-main-freeze",
                        token_id="tok-2", token_expires_at=4600, started_at=1015,
                    ),
                    now=1015,
                )
            self.assertEqual(ctx.exception.code, "unsettled_ruleset_op")

            # Reload from disk (simulating crash while awaiting_audit_claim) and settle!
            journal = RulesetJournal(jpath)
            self.assertEqual(len(journal.unsettled_entries()), 1)
            journal.settle_all(
                now=1012,
                ruleset_put_barrier_staging_verified=True,
                get_token_dead_at=dead_tokens.get,
                read_ruleset_twice=lambda rid: (ruleset_state[rid], ruleset_state[rid]),
                query_audit_events=lambda rid, actor, since: [e for e in audit_stream if e.created_at >= since],
            )
            self.assertEqual(len(journal.unsettled_entries()), 0)

            # (3a part 2) Lost-response PUT on main ruleset 27758336 within <60s of data-ref event -> Case 1 applied_observed
            journal.begin_put(
                RulesetOpEntry(
                    op_id="op-main-1", cutover_attempt_id="att-1", rollback_attempt_id=1, step="B(0b)",
                    ruleset_id=27758336, ruleset_name="main", actor_login="bfme-rollback-admin-app[bot]",
                    pre_put_updated_at=1000, pre_put_ruleset_sha256="sha-main-0", target_sha256="sha-main-freeze",
                    token_id="tok-2", token_expires_at=4600, started_at=1020,
                ),
                now=1020,
            )
            ruleset_state[27758336] = RulesetReadback(27758336, 1021, "sha-main-freeze")
            audit_stream.append(AuditEvent("ev-main-1", 27758336, "bfme-rollback-admin-app[bot]", 1021))
            dead_tokens["tok-2"] = 1025
            settled = journal.settle_all(
                now=1150,
                ruleset_put_barrier_staging_verified=True,
                get_token_dead_at=dead_tokens.get,
                read_ruleset_twice=lambda rid: (ruleset_state[rid], ruleset_state[rid]),
                query_audit_events=lambda rid, actor, since: [e for e in audit_stream if e.created_at >= since],
            )
            self.assertEqual(settled[0].settled_outcome, "applied_observed")
            self.assertEqual(settled[0].audit_log_event_id, "ev-main-1")

            # (3b) Same-ruleset crash-after-fsync-before-send within <60s of claimed ev-main-1 -> Case 2 not_applied_token_dead!
            journal.begin_put(
                RulesetOpEntry(
                    op_id="op-main-2", cutover_attempt_id="att-1", rollback_attempt_id=1, step="B(0b)",
                    ruleset_id=27758336, ruleset_name="main", actor_login="bfme-rollback-admin-app[bot]",
                    pre_put_updated_at=1021, pre_put_ruleset_sha256="sha-main-freeze", target_sha256="sha-main-legacy",
                    token_id="tok-3", token_expires_at=4600, started_at=1030,
                ),
                now=1030,
            )
            dead_tokens["tok-3"] = 1035
            settled = journal.settle_all(
                now=1160,
                ruleset_put_barrier_staging_verified=True,
                get_token_dead_at=dead_tokens.get,
                read_ruleset_twice=lambda rid: (ruleset_state[rid], ruleset_state[rid]),
                query_audit_events=lambda rid, actor, since: [e for e in audit_stream if e.created_at >= since],
            )
            self.assertEqual(settled[0].settled_outcome, "not_applied_token_dead")

    def test_workflow_lints_scan_repo_workflows(self):
        workflows_dir = Path(__file__).resolve().parents[3] / ".github" / "workflows"
        findings = scan_workflows_dir(workflows_dir)
        # Live pre-Phase-0 workflows have known legacy contents: write / actions: write / lint-j migration items
        self.assertGreater(len(findings), 0)
        rules_seen = {f.rule for f in findings}
        self.assertIn("lint-i", rules_seen)
        self.assertIn("lint-j", rules_seen)


if __name__ == "__main__":
    unittest.main()
