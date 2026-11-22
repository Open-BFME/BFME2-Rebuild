import copy
import unittest

from git_objects import Git
from header_inventory import Binding
from header_source_preview import source_preview
import header_trusted
from test_header_checks import HEADER, POLICY
import test_header_inventory as fixtures


class HeaderTrustedTests(unittest.TestCase):
    def setUp(self):
        self.fixture = fixtures.InventoryTests(methodName="test_comment_and_whitespace_only_do_not_require_an_inventory")
        self.fixture.setUp()
        self.addCleanup(self.fixture.doCleanups)

    def test_round_trip_clears_h4_and_h6_on_additive_header_like_pr43(self):
        base = self.fixture.commit({HEADER: "class W3DSnowManager {\npublic:\n    void init();\n};\n"})
        head = self.fixture.commit({HEADER: "class W3DSnowManager {\npublic:\n    void init();\n    void ReAcquireResources();\n};\n"})
        binding, _ = self.fixture.compare(base, head)
        with Git(self.fixture.root) as repo:
            untrusted = source_preview(repo, POLICY, binding)
            self.assertFalse(untrusted["trusted"])
            u_rules = {r["rule"]: r["state"] for r in untrusted["preview"]}
            self.assertEqual(u_rules["H4"], "held")
            self.assertEqual(u_rules["H6"], "held")

            doc = header_trusted.produce(repo, POLICY, binding)
            obs = header_trusted.load(doc, binding)
            trusted = source_preview(repo, POLICY, binding, trusted=obs)
            self.assertTrue(trusted["trusted"])
            t_rules = {r["rule"]: r["state"] for r in trusted["preview"]}
            self.assertEqual(t_rules["H3"], "pass")
            self.assertEqual(t_rules["H4"], "pass")
            self.assertEqual(t_rules["H6"], "pass")
            self.assertEqual(t_rules["H7"], "pass")
            self.assertEqual(t_rules["H8"], "pass")

    def test_mismatched_binding_or_incomplete_inventory_stays_held(self):
        base = self.fixture.commit({HEADER: "class W3DSnowManager {\npublic:\n    void init();\n};\n"})
        head = self.fixture.commit({HEADER: "class W3DSnowManager {\npublic:\n    void init();\n    void extra();\n};\n"})
        binding, _ = self.fixture.compare(base, head)
        other = Binding(binding.repository, binding.base, binding.head, binding.tree,
                        binding.source_policy_sha256, binding.checker_sha256, binding.run_id + 1, binding.attempt)
        with Git(self.fixture.root) as repo:
            doc = header_trusted.produce(repo, POLICY, binding)
            # Binding mismatch must drop trusted observations and hold H4/H6.
            wrong_obs = header_trusted.load(doc, other)
            res = source_preview(repo, POLICY, binding, trusted=wrong_obs)
            rules = {r["rule"]: r["state"] for r in res["preview"]}
            self.assertEqual(rules["H4"], "held")
            self.assertEqual(rules["H6"], "held")

            # complete=False must hold H4.
            incomplete = copy.deepcopy(doc)
            incomplete["declarations"]["complete"] = False
            res2 = source_preview(repo, POLICY, binding, trusted=header_trusted.load(incomplete, binding))
            rules2 = {r["rule"]: r["state"] for r in res2["preview"]}
            self.assertEqual(rules2["H4"], "held")
            self.assertEqual(rules2["H6"], "pass")

    def test_load_rejects_bad_schema_rule_or_revision(self):
        base = self.fixture.commit({HEADER: "struct A { int x; };\n"})
        head = self.fixture.commit({HEADER: "struct A { int x; int y; };\n"})
        binding, _ = self.fixture.compare(base, head)
        with Git(self.fixture.root) as repo:
            doc = header_trusted.produce(repo, POLICY, binding)
        bad_rev = copy.deepcopy(doc)
        bad_rev["declarations"]["format_revision"] = 2
        with self.assertRaises(ValueError):
            header_trusted.load(bad_rev, binding)
        bad_rule = copy.deepcopy(doc)
        bad_rule["bodies"]["rule"] = "H4"
        with self.assertRaises(ValueError):
            header_trusted.load(bad_rule, binding)
        extra_key = copy.deepcopy(doc)
        extra_key["rogue"] = True
        with self.assertRaises(ValueError):
            header_trusted.load(extra_key, binding)



    def test_guarded_header_with_unchanged_include_passes_h4_and_h6(self):
        base = self.fixture.commit({HEADER: (
            "#pragma once\n"
            "#ifndef BFME2_SNOW_H\n"
            "#define BFME2_SNOW_H\n"
            '#include "GameClient/SnowManager.h"\n'
            "class W3DSnowManager {\npublic:\n    void init();\n};\n"
            "#endif\n"
        )})
        head = self.fixture.commit({HEADER: (
            "#pragma once\n"
            "#ifndef BFME2_SNOW_H\n"
            "#define BFME2_SNOW_H\n"
            '#include "GameClient/SnowManager.h"\n'
            "class W3DSnowManager {\npublic:\n    void init();\n    void ReAcquireResources();\n};\n"
            "#endif\n"
        )})
        binding, _ = self.fixture.compare(base, head)
        with Git(self.fixture.root) as repo:
            doc = header_trusted.produce(repo, POLICY, binding)
            self.assertTrue(doc["declarations"]["complete"])
            self.assertTrue(doc["bodies"]["complete"])
            res = source_preview(repo, POLICY, binding, trusted=header_trusted.load(doc, binding))
            states = {r["rule"]: r["state"] for r in res["preview"]}
            self.assertEqual(states["H4"], "pass")
            self.assertEqual(states["H6"], "pass")

    def test_moved_define_or_added_include_or_body_if_holds_h4_and_h6(self):
        # 1. #define moved above declaration (Opus P1 / Sol P1 #1)
        b1 = self.fixture.commit({HEADER: "void foo();\n#define foo bar\n"})
        h1 = self.fixture.commit({HEADER: "#define foo bar\nvoid foo();\n"})
        bind1, _ = self.fixture.compare(b1, h1)
        with Git(self.fixture.root) as repo:
            d1 = header_trusted.produce(repo, POLICY, bind1)
            self.assertFalse(d1["declarations"]["complete"])
            self.assertFalse(d1["bodies"]["complete"])

        # 2. #include added before declaration (Opus P1 / Sol P1 #1)
        b2 = self.fixture.commit({HEADER: "void foo();\n"})
        h2 = self.fixture.commit({HEADER: '#include "rename.h"\nvoid foo();\n'})
        bind2, _ = self.fixture.compare(b2, h2)
        with Git(self.fixture.root) as repo:
            d2 = header_trusted.produce(repo, POLICY, bind2)
            self.assertFalse(d2["declarations"]["complete"])
            self.assertFalse(d2["bodies"]["complete"])

        # 3. #include added before inline body (Opus P1)
        b3 = self.fixture.commit({HEADER: "inline int f() { return X; }\n"})
        h3 = self.fixture.commit({HEADER: '#include "x2.h"\ninline int f() { return X; }\n'})
        bind3, _ = self.fixture.compare(b3, h3)
        with Git(self.fixture.root) as repo:
            d3 = header_trusted.produce(repo, POLICY, bind3)
            self.assertFalse(d3["bodies"]["complete"])

        # 4. #if conditional inside header body switching active inline (Sol P1 #2)
        b4 = self.fixture.commit({HEADER: "#if 0\ninline int f() { return 1; }\n#endif\n"})
        h4 = self.fixture.commit({HEADER: "#if 1\ninline int f() { return 1; }\n#endif\n"})
        bind4, _ = self.fixture.compare(b4, h4)
        with Git(self.fixture.root) as repo:
            d4 = header_trusted.produce(repo, POLICY, bind4)
            self.assertFalse(d4["declarations"]["complete"])
            self.assertFalse(d4["bodies"]["complete"])


if __name__ == "__main__":
    unittest.main()
