import sys
import tempfile
import unittest
from pathlib import Path
from unittest import mock

ROOT = Path(__file__).resolve().parents[1]
STATE_ROOT = Path(__file__).resolve().parents[2] / "managed-state"
sys.path.insert(0, str(ROOT))
sys.path.insert(0, str(STATE_ROOT))

import compare
import managed_matrix
import recovery_broker
import wide
import wide_recovery


class FakeJudge:
    def __init__(self, bodies):
        self.bodies = bodies

    def read_body(self, obj, symbol):
        return self.bodies[(Path(obj).name, symbol)]


class Cut2OptimizationTests(unittest.TestCase):
    def _lbl(self, n):
        return "$" + "L" + str(n)

    def test_local_label_renumber_matches_and_preserves_addends_and_targets(self):
        with tempfile.TemporaryDirectory() as tmp:
            base_obj = Path(tmp) / "base.obj"
            head_obj = Path(tmp) / "head.obj"
            base_obj.write_bytes(b"stub")
            head_obj.write_bytes(b"stub")

            l_b1, l_b2 = self._lbl(9622), self._lbl(9623)
            l_h1, l_h2 = self._lbl(9624), self._lbl(9625)
            body_a = bytes([85, 139, 236, 233, 0, 0, 0, 0, 195])
            body_addend = bytes([85, 139, 236, 233, 4, 0, 0, 0, 195])
            target_same = bytes([144, 195])
            target_diff = bytes([204, 195])

            judge = FakeJudge({
                ("base.obj", l_b1): (body_a, [(4, 20, l_b2)]),
                ("base.obj", l_b2): (target_same, []),
                ("head.obj", l_h1): (body_a, [(4, 20, l_h2)]),
                ("head.obj", l_h2): (target_same, []),
            })
            base_out = {"rows": [], "expected": [], "gates": [], "verification_blockers": [],
                        "rejected_extras": [{"symbol": l_b1, "comdat_selection": 2, "referenced_by_kept": True, "why": "unwind"}]}
            head_out = {"rows": [], "expected": [], "gates": [], "verification_blockers": [],
                        "rejected_extras": [{"symbol": l_h1, "comdat_selection": 2, "referenced_by_kept": True, "why": "unwind"}]}
            self.assertEqual(
                compare.native_blockers(base_out, judge=judge, obj=base_obj),
                compare.native_blockers(head_out, judge=judge, obj=head_obj),
            )

            # Different relocation addend must NOT match.
            judge_addend = FakeJudge({
                ("base.obj", l_b1): (body_a, [(4, 20, l_b2)]),
                ("base.obj", l_b2): (target_same, []),
                ("head.obj", l_h1): (body_addend, [(4, 20, l_h2)]),
                ("head.obj", l_h2): (target_same, []),
            })
            self.assertNotEqual(
                compare.native_blockers(base_out, judge=judge_addend, obj=base_obj),
                compare.native_blockers(head_out, judge=judge_addend, obj=head_obj),
            )

            # Different local relocation target body must NOT match.
            judge_target = FakeJudge({
                ("base.obj", l_b1): (body_a, [(4, 20, l_b2)]),
                ("base.obj", l_b2): (target_same, []),
                ("head.obj", l_h1): (body_a, [(4, 20, l_h2)]),
                ("head.obj", l_h2): (target_diff, []),
            })
            self.assertNotEqual(
                compare.native_blockers(base_out, judge=judge_target, obj=base_obj),
                compare.native_blockers(head_out, judge=judge_target, obj=head_obj),
            )

            # Aliasing: two refs to SAME local cell vs TWO DISTINCT cells with identical bytes must NOT match.
            body_two = bytes([85, 139, 236, 161, 0, 0, 0, 0, 163, 0, 0, 0, 0, 195])
            l_b3, l_h3 = self._lbl(9626), self._lbl(9627)
            judge_alias = FakeJudge({
                ("base.obj", l_b1): (body_two, [(4, 6, l_b2), (9, 6, l_b2)]),
                ("base.obj", l_b2): (target_same, []),
                ("head.obj", l_h1): (body_two, [(4, 6, l_h2), (9, 6, l_h3)]),
                ("head.obj", l_h2): (target_same, []),
                ("head.obj", l_h3): (target_same, []),
            })
            self.assertNotEqual(
                compare.native_blockers(base_out, judge=judge_alias, obj=base_obj),
                compare.native_blockers(head_out, judge=judge_alias, obj=head_obj),
            )

            # Cross-root aliasing: two roots sharing ONE target vs TWO distinct byte-identical targets must NOT match.
            l_b4, l_h4 = self._lbl(9628), self._lbl(9629)
            judge_cross = FakeJudge({
                ("base.obj", l_b1): (body_a, [(4, 20, l_b3)]),
                ("base.obj", l_b2): (body_a, [(4, 20, l_b3)]),
                ("base.obj", l_b3): (target_same, []),
                ("head.obj", l_h1): (body_a, [(4, 20, l_h3)]),
                ("head.obj", l_h2): (body_a, [(4, 20, l_h4)]),
                ("head.obj", l_h3): (target_same, []),
                ("head.obj", l_h4): (target_same, []),
            })
            base_two = {"rows": [], "expected": [], "gates": [], "verification_blockers": [],
                        "rejected_extras": [{"symbol": l_b1, "comdat_selection": 2, "referenced_by_kept": True, "why": "unwind"},
                                            {"symbol": l_b2, "comdat_selection": 2, "referenced_by_kept": True, "why": "unwind"}]}
            head_two = {"rows": [], "expected": [], "gates": [], "verification_blockers": [],
                        "rejected_extras": [{"symbol": l_h1, "comdat_selection": 2, "referenced_by_kept": True, "why": "unwind"},
                                            {"symbol": l_h2, "comdat_selection": 2, "referenced_by_kept": True, "why": "unwind"}]}
            self.assertNotEqual(
                compare.native_blockers(base_two, judge=judge_cross, obj=base_obj),
                compare.native_blockers(head_two, judge=judge_cross, obj=head_obj),
            )

            # 4-root 2-site partner-swap: (R1,R2) sharing BOTH sites vs 4-cycle sharing must NOT match.
            ta, tb, tc, td = self._lbl(101), self._lbl(102), self._lbl(103), self._lbl(104)
            r1, r2, r3, r4 = self._lbl(201), self._lbl(202), self._lbl(203), self._lbl(204)
            judge_four = FakeJudge({
                ("base.obj", r1): (body_two, [(4, 6, ta), (9, 6, tb)]),
                ("base.obj", r2): (body_two, [(4, 6, ta), (9, 6, tb)]),
                ("base.obj", r3): (body_two, [(4, 6, tc), (9, 6, td)]),
                ("base.obj", r4): (body_two, [(4, 6, tc), (9, 6, td)]),
                ("base.obj", ta): (target_same, []), ("base.obj", tb): (target_same, []),
                ("base.obj", tc): (target_same, []), ("base.obj", td): (target_same, []),
                ("head.obj", r1): (body_two, [(4, 6, ta), (9, 6, td)]),
                ("head.obj", r2): (body_two, [(4, 6, ta), (9, 6, tb)]),
                ("head.obj", r3): (body_two, [(4, 6, tc), (9, 6, tb)]),
                ("head.obj", r4): (body_two, [(4, 6, tc), (9, 6, td)]),
                ("head.obj", ta): (target_same, []), ("head.obj", tb): (target_same, []),
                ("head.obj", tc): (target_same, []), ("head.obj", td): (target_same, []),
            })
            four_rows = [{"symbol": r, "comdat_selection": 2, "referenced_by_kept": True, "why": "unwind"} for r in (r1, r2, r3, r4)]
            four_out = {"rows": [], "expected": [], "gates": [], "verification_blockers": [], "rejected_extras": four_rows}
            self.assertNotEqual(
                compare.native_blockers(four_out, judge=judge_four, obj=base_obj),
                compare.native_blockers(four_out, judge=judge_four, obj=head_obj),
            )

            # Root-vs-non-root target: R1->R2 (both rejected roots) vs R1->T_nonroot (same bytes as R2) must NOT match.
            t_nr = self._lbl(301)
            judge_root_vs_nr = FakeJudge({
                ("base.obj", r1): (body_a, [(4, 20, r2)]),
                ("base.obj", r2): (target_same, []),
                ("head.obj", r1): (body_a, [(4, 20, t_nr)]),
                ("head.obj", r2): (target_same, []),
                ("head.obj", t_nr): (target_same, []),
            })
            two_r = {"rows": [], "expected": [], "gates": [], "verification_blockers": [],
                     "rejected_extras": [{"symbol": r1, "comdat_selection": 2, "referenced_by_kept": True, "why": "unwind"},
                                         {"symbol": r2, "comdat_selection": 2, "referenced_by_kept": True, "why": "unwind"}]}
            self.assertNotEqual(
                compare.native_blockers(two_r, judge=judge_root_vs_nr, obj=base_obj),
                compare.native_blockers(two_r, judge=judge_root_vs_nr, obj=head_obj),
            )

            # Unreadable COFF evidence must fail closed to raw symbol names.
            judge_missing = FakeJudge({})
            self.assertNotEqual(
                compare.native_blockers(base_out, judge=judge_missing, obj=base_obj),
                compare.native_blockers(head_out, judge=judge_missing, obj=head_obj),
            )

    def test_unexpected_symbol_cap_never_hides_new_offender(self):
        # 40 pre-existing base symbols + 1 new head symbol whose digest could sort last.
        base_extras = [{"symbol": f"?baseExtra{i:02d}@@YAXXZ"} for i in range(40)]
        new_sym = "?newOffender@@YAXXZ"
        base_raw = {"rows": [], "expected": [], "gates": [], "verification_blockers": [], "rejected_extras": base_extras}
        head_raw = {"rows": [], "expected": [], "gates": [], "verification_blockers": [], "rejected_extras": base_extras + [{"symbol": new_sym}]}

        base_clean = wide.sanitize_outcome(base_raw, "src/Foo.cpp", {"src/Foo.cpp"}, include_symbols=False)
        head_clean = wide.sanitize_outcome(head_raw, "src/Foo.cpp", {"src/Foo.cpp"}, baseline=base_clean, include_symbols=True)
        self.assertNotIn("unexpected_symbols", base_clean)
        self.assertEqual(len(head_clean["unexpected_symbols"]), 1)
        self.assertEqual(head_clean["unexpected_symbols"][0][1], new_sym)
        managed_matrix.check_native(head_clean)

        old_idx = compare.index({"src/Foo.cpp": base_clean})
        new_idx = compare.index({"src/Foo.cpp": head_clean})
        _, rules, hints = compare.reduction(old_idx, new_idx)
        self.assertIn("unexpected-code", rules)
        self.assertTrue(any(new_sym in h for h in hints))

    def test_dynamic_shard_sizing_uses_production_function(self):
        self.assertEqual(wide.choose_runner_count(23, max_runners=16, target_per_shard=10), 3)
        self.assertEqual(wide.choose_runner_count(9, max_runners=16, target_per_shard=10), 1)
        self.assertEqual(wide.choose_runner_count(160, max_runners=16, target_per_shard=10), 16)
        self.assertEqual(wide.choose_runner_count(512, max_runners=16, target_per_shard=10), 16)
        self.assertEqual(wide.choose_runner_count(512, max_runners=32, target_per_shard=10), 32)
        with self.assertRaises(ValueError):
            wide.choose_runner_count(10, max_runners=0)
        with self.assertRaises(ValueError):
            wide.choose_runner_count(33, max_runners=1)

    def test_reserve_window_includes_on_demand_boot_margin(self):
        self.assertGreaterEqual(wide_recovery.RESERVE_SECONDS, 450)

    def test_plan_anchor_clock_and_staleness_guard(self):
        decision = {
            "version": 1, "repo": recovery_broker.REPO_ID, "number": 17,
            "base": "a" * 40, "head": "b" * 40, "tree": "c" * 40,
            "kind": "wide", "plan": "d" * 64,
        }
        with tempfile.TemporaryDirectory() as tmp:
            out_file = Path(tmp) / "out.txt"
            with mock.patch.dict("os.environ", {"GITHUB_OUTPUT": str(out_file), "GITHUB_SHA": "a" * 40}), \
                 mock.patch.object(recovery_broker, "source_free_decision", return_value=decision), \
                 mock.patch.object(recovery_broker, "Actions") as actions_cls, \
                 mock.patch.object(recovery_broker, "time", return_value=2000.0):
                actions_cls.return_value.plan_anchor.return_value = (1500, 1980)
                recovery_broker.anchor(tmp)
                lines = dict(line.split("=", 1) for line in out_file.read_text().splitlines())
                self.assertEqual(lines["started"], "1980")
                self.assertEqual(lines["primary_cutoff"], str(1980 + recovery_broker.PRIMARY_SECONDS))
                self.assertEqual(lines["reserve_cutoff"], str(1980 + recovery_broker.PRIMARY_SECONDS + wide_recovery.RESERVE_SECONDS))

            with mock.patch.object(recovery_broker, "source_free_decision", return_value=decision), \
                 mock.patch.object(recovery_broker, "Actions") as actions_cls, \
                 mock.patch.object(recovery_broker, "time", return_value=5000.0):
                actions_cls.return_value.plan_anchor.return_value = (1000, 4990)
                with self.assertRaises(ValueError):
                    recovery_broker.anchor(tmp)




class EmittedRowMessageTests(unittest.TestCase):
    def test_emitted_row_blocker_produces_actionable_hint(self):
        old_table = {
            "src/Code/A.cpp": {
                "object_verified": True, "error": None, "gates": [],
                "expected": [], "verification_blockers": [], "unexpected_code": [],
                "rows": [{"rva": "0x00100000", "status": "MISSING", "pct": 0.0}],
            }
        }
        new_table = {
            "src/Code/A.cpp": {
                "object_verified": False, "error": None, "gates": [],
                "expected": [{"rva": "0x00100000", "status": "MISMATCH"}],
                "verification_blockers": [], "unexpected_code": [],
                "rows": [{"rva": "0x00100000", "status": "MISMATCH", "pct": 87.5}],
            }
        }
        _, rules, hints = compare.reduction(compare.index(old_table), compare.index(new_table))
        self.assertIn("emitted-row", rules)
        self.assertTrue(any("src/Code/A.cpp at 0x00100000: emitted function MISMATCH (87.5%" in h for h in hints),
                        f"Expected actionable emitted-row hint, got {hints!r}")



    def test_emitted_row_blocker_falls_back_to_expected_when_not_in_rows(self):
        old_table = {
            "src/Code/B.cpp": {
                "object_verified": True, "error": None, "gates": [],
                "expected": [], "verification_blockers": [], "unexpected_code": [],
                "rows": [],
            }
        }
        new_table = {
            "src/Code/B.cpp": {
                "object_verified": False, "error": None, "gates": [],
                "expected": [{"rva": "0x00200000", "status": "MISMATCH", "symbol": "Foo::bar"}],
                "verification_blockers": [], "unexpected_code": [],
                "rows": [],
            }
        }
        _, rules, hints = compare.reduction(compare.index(old_table), compare.index(new_table))
        self.assertIn("emitted-row", rules)
        self.assertTrue(any("src/Code/B.cpp at 0x00200000 `Foo::bar`: emitted function MISMATCH" in h for h in hints),
                        f"Expected fallback emitted-row hint, got {hints!r}")



class Cut12HeaderProbeTests(unittest.TestCase):
    def test_access_specifier_addition_before_new_member_is_planned(self):
        import header_class_source, header_probe_plan
        before = "class W3DRadar {\n    int m_x;\n};\n"
        after = "class W3DRadar {\n    int m_x;\npublic:\n    virtual void refreshTerrain();\n};\n"
        plan = header_probe_plan.plan_existing_header("include/GameClient/W3DRadar.h", before, after, header_class_source)
        self.assertEqual(plan["status"], "planned", plan)
        self.assertEqual(plan["before"], [])
        self.assertEqual([d["qualified"] for d in plan["after"]], ["W3DRadar::refreshTerrain"])

    def test_access_specifier_changing_existing_member_reprobes_both_sides(self):
        import header_class_source, header_probe_plan
        before = "class Foo {\nprotected:\n    void bar();\n};\n"
        after = "class Foo {\npublic:\n    void bar();\n};\n"
        plan = header_probe_plan.plan_existing_header("include/GameClient/Foo.h", before, after, header_class_source)
        self.assertEqual(plan["status"], "planned", plan)
        self.assertEqual([d["qualified"] for d in plan["before"]], ["Foo::bar"])
        self.assertEqual([d["qualified"] for d in plan["after"]], ["Foo::bar"])
        self.assertEqual(plan["after"][0]["before"]["qualified"], "Foo::bar")



    def test_struct_private_access_change_reprobes_both_sides(self):
        import header_class_source, header_probe_plan
        before = "struct S {\n    void f();\n};\n"
        after = "struct S {\nprivate:\n    void f();\n};\n"
        plan = header_probe_plan.plan_existing_header("include/GameClient/S.h", before, after, header_class_source)
        self.assertEqual(plan["status"], "planned", plan)
        self.assertEqual([d["qualified"] for d in plan["before"]], ["S::f"])
        self.assertEqual([d["qualified"] for d in plan["after"]], ["S::f"])

    def test_inline_access_label_with_changed_access_fails_closed(self):
        import header_class_source, header_probe_plan
        before = "class Foo {\npublic: void first();\n    void bar();\n};\n"
        after = "class Foo {\npublic: void first();\nprivate:\n    void bar();\n};\n"
        plan = header_probe_plan.plan_existing_header("include/GameClient/Foo.h", before, after, header_class_source)
        self.assertEqual(plan["status"], "held", plan)
        self.assertTrue(any("inline access label" in i["reason"] for i in plan.get("issues", [])), plan)

    def test_pragma_inside_class_with_changed_access_fails_closed(self):
        import header_class_source, header_probe_plan
        before = "class Foo {\nprotected:\n#pragma pack(push, 1)\n    void bar();\n#pragma pack(pop)\n};\n"
        after = "class Foo {\npublic:\n#pragma pack(push, 1)\n    void bar();\n#pragma pack(pop)\n};\n"
        plan = header_probe_plan.plan_existing_header("include/GameClient/Foo.h", before, after, header_class_source)
        self.assertEqual(plan["status"], "held", plan)
        self.assertTrue(any("preprocessor directive" in i["reason"] for i in plan.get("issues", [])), plan)


class SharedEhAndVtableTests(unittest.TestCase):
    def test_known_reference_names_includes_vtables(self):
        import emitted_data

        class FakeJudge:
            MAP_NAME_INDEX = {"?open@LocalFile@@UAE_NPBDH@Z": {0x605CFB}}

            def config_json(self, _path):
                return {
                    "symbols": {"?TheOpenFileCount@@3HA": {"candidates": []}},
                    "vtables": {"??_7LocalFile@@6B@": [{"rva": "0x0087AB28", "slots": "18"}]},
                }

        known = emitted_data.known_reference_names(FakeJudge(), Path("."))
        self.assertIn("??_7LocalFile@@6B@", known)
        self.assertIn("?TheOpenFileCount@@3HA", known)
        self.assertIsNone(emitted_data.compare_public_data(set(), {"??_7LocalFile@@6B@"}, known))
        self.assertEqual(
            emitted_data.compare_public_data(set(), {"??_7LocalFile@@6B@", "?UnknownGlobal@@3HA"}, known),
            {"count": 1, "names": ["?UnknownGlobal@@3HA"]},
        )

    def test_verified_shared_eh_extras_excused_only_when_strictly_proven(self):
        class FakeCoff:
            def __init__(self, entries):
                self.symbols = entries

            def index(self, name):
                for i, entry in enumerate(self.symbols):
                    if entry["name"] == name:
                        return i
                raise KeyError(name)

        class FakeJudgeWithCoff:
            def __init__(self, coff_obj):
                self._coff = coff_obj

            def coff(self, _obj):
                return self._coff

        with tempfile.TemporaryDirectory() as tmp:
            obj_path = Path(tmp) / "LocalFile.obj"
            obj_path.write_bytes(b"stub")
            lbl = "$" + "L2402"
            eh_sym = "__ehhandler$?scanReal@LocalFile@@UAE_NAAM@Z"
            other_sym = "?unexpectedHelper@@YAXXZ"
            coff_obj = FakeCoff([
                {"name": eh_sym, "section": 12, "value": 0},
                {"name": lbl, "section": 13, "value": 0},
                {"name": other_sym, "section": 14, "value": 0},
            ])
            judge = FakeJudgeWithCoff(coff_obj)

            proven_row = {
                "rva": "0x00606133",
                "status": "MATCH",
                "relocation_check": {
                    "status": "MATCH",
                    "proofs": [
                        {"kind": "binary-shared-EH-parent", "rva": "0x007A6CA4", "compiled_parent": "0x00606133"},
                        {"kind": "code", "rva": "0x007A6CA4", "size": 10, "compiled_section": 12, "compiled_offset": 0},
                        {"kind": "binary-shared-EH-parent", "rva": "0x007A6C9C", "compiled_parent": "0x00606133"},
                        {"kind": "code", "rva": "0x007A6C9C", "size": 8, "compiled_section": 13, "compiled_offset": 0},
                    ],
                },
            }
            outcome = {
                "rows": [proven_row],
                "expected": [],
                "gates": [],
                "verification_blockers": [],
                "rejected_extras": [
                    {"symbol": eh_sym, "why": compare.SHARED_EH_NO_IDENTITY},
                    {"symbol": lbl, "why": compare.SHARED_EH_NO_IDENTITY},
                ],
            }
            self.assertEqual(compare.native_blockers(outcome, judge=judge, obj=obj_path), [])
            self.assertEqual(compare.unexpected_symbol_pairs(outcome, judge=judge, obj=obj_path), [])

            # Fail closed: unproven parent row must NOT excuse shared EH extras.
            unproven = dict(outcome, rows=[dict(proven_row, relocation_check=dict(proven_row["relocation_check"], status="UNVERIFIED_RELOCATION"))])
            self.assertEqual(len(compare.native_blockers(unproven, judge=judge, obj=obj_path)), 2)

            # Fail closed: non-EH symbol at a proven section/offset must NOT be excused.
            non_eh = dict(outcome, rejected_extras=[{"symbol": other_sym, "why": compare.SHARED_EH_NO_IDENTITY}])
            self.assertEqual(len(compare.native_blockers(non_eh, judge=judge, obj=obj_path)), 1)

            # Fail closed: different rejection reason must NOT be excused.
            mismatch_why = dict(outcome, rejected_extras=[{"symbol": eh_sym, "why": "retail duplicate WRONG_RELOCATION"}])
            self.assertEqual(len(compare.native_blockers(mismatch_why, judge=judge, obj=obj_path)), 1)

            # Fail closed: without readable COFF object, keep the blockers.
            self.assertEqual(len(compare.native_blockers(outcome, judge=None, obj=None)), 2)


if __name__ == "__main__":
    unittest.main()
