"""Git-level fixtures for the eight source gates."""

from copy import deepcopy
import json
from pathlib import Path
import subprocess
import sys
from tempfile import TemporaryDirectory
import unittest

from cpp import Source
from definitions import definitions
from gate import report, verdict
from git_objects import Git
from rules import Index, Policy, forbidden, paste_can_form
import ratchets


HERE = Path(__file__).resolve().parent
CONFIG = json.loads((HERE / 'config.json').read_text())
SEED = 'src/subject.cpp'


class Fixture(unittest.TestCase):
    def setUp(self):
        scratch = TemporaryDirectory()
        self.addCleanup(scratch.cleanup)
        self.root = Path(scratch.name)
        self.git('init', '-q', '--template=')
        self.git('config', 'user.name', 'Source gate fixture')
        self.git('config', 'user.email', 'fixture@example.invalid')
        self.git('config', 'core.autocrlf', 'false')
        self.git('config', 'core.safecrlf', 'false')
        self.git('config', 'core.hooksPath', '/dev/null')

    def git(self, *args, data=None):
        return subprocess.check_output(['git', '-C', str(self.root), *args], input=data,
                                       stderr=subprocess.PIPE)

    def snapshot(self, additions):
        for name, content in additions.items():
            path = self.root / name
            if content is None:
                path.unlink()
            else:
                path.parent.mkdir(parents=True, exist_ok=True)
                path.write_bytes(content if isinstance(content, bytes) else content.encode())
        self.git('add', '-A')
        return self.git('write-tree').decode().strip()

    def compare(self, old, new, config=None):
        base = self.snapshot({'config/map.json': json.dumps({
            'rows': [
                {'name': '?rva00123456@Class@@QAEHXZ', 'accepted_names': ['sub_00123457']},
                {'name': '??0Rva00123458Class@@QAE@XZ', 'accepted_names': []},
            ],
        }), **old})
        head = self.snapshot(new)
        policy = Policy(config or CONFIG)
        with Git(self.root) as git:
            return verdict(git, policy, Index(git, policy), git.tree_oid(base), git.tree_oid(head))

    def assert_gate(self, label, expected, result):
        gate = next(gate for gate in result['gates'] if gate['id'] == label)
        self.assertEqual(expected, gate['status'], report(result))
        for finding in gate['findings']:
            self.assertGreaterEqual(finding['line'], 1)
            self.assertTrue(finding['path'])
            self.assertTrue(finding['remedy'])
        return gate['findings']

    def test_g1_existing_source_and_promoted_header_paths(self):
        result = self.compare({SEED: 'int x;\n', 'include/Draft/A.h': 'struct A {};\n'},
                              {SEED: 'int y;\n', 'include/Draft/A.h': None,
                               'include/A.h': 'struct A {};\n'})
        self.assert_gate('G1', 'pass', result)

    def test_g1_source_rename_disguised_as_delete_and_add_and_config_edit(self):
        result = self.compare({SEED: 'int x;\n', 'tools/gates/config.json': '{}'},
                              {SEED: None, 'src/renamed.cpp': 'int x;\n',
                               'tools/gates/config.json': '{"limits":{}}'})
        findings = self.assert_gate('G1', 'fail', result)
        self.assertEqual({SEED, 'src/renamed.cpp', 'tools/gates/config.json'}, {f['path'] for f in findings})

    def test_g1_draft_addition_and_permanent_deletion(self):
        result = self.compare({'include/A.h': 'int x;\n'},
                              {'include/A.h': None, 'include/Draft/A.h': 'int x;\n'})
        self.assertEqual(2, len(self.assert_gate('G1', 'fail', result)))

    def test_g1_crlf_and_mixed_endings(self):
        result = self.compare({SEED: b'// FUNCTION: A\r\nint x;\r\n// ordinary\r\n'},
                              {SEED: b'// FUNCTION: A\nint y;\r\n// ordinary\n'})
        self.assert_gate('G1', 'pass', result)
        self.assert_gate('G2', 'pass', result)

    def test_g1_rejects_invalid_utf8_and_bare_cr(self):
        result = self.compare({SEED: 'int x;\n', 'include/A.h': ''},
                              {SEED: b'int y;\rint z;\n', 'include/A.h': b'\xff'})
        self.assertEqual(2, len(self.assert_gate('G1', 'fail', result)))
        self.assert_gate('G4', 'blocked', result)

    def test_g1_rejects_different_line_separators(self):
        for character in ("\x85", "\u2028", "\u2029"):
            with self.subTest(character=character):
                result = self.compare({SEED: "int value;\n"},
                                      {SEED: "// ordinary note" + character + "additional text\n"})
                finding, = self.assert_gate("G1", "fail", result)
                self.assertEqual((finding["path"], finding["line"]), (SEED, 1))
                self.assertIn("line separator", finding["message"])

    def test_g2_preserves_every_recognized_compiler_directive(self):
        locked = ("// cl: /W3", "// cl-arch: x87", "// cl-profile: upstream",
                  "// stlport", "// stlport46", "// msvcstl",
                  "// psdk", "// bfmealloc_alloconly")
        for marker in locked:
            with self.subTest(marker=marker):
                result = self.compare({SEED: "int value;\n"}, {SEED: marker + "\nint value;\n"})
                finding, = self.assert_gate("G2", "fail", result)
                self.assertEqual((finding["path"], finding["line"]), (SEED, 1))

    def test_g2_allows_clean_sdk_opt_in_additions_with_strict_guards(self):
        for marker in ("// dxsdk", "// platformsdk"):
            with self.subTest(add=marker):
                self.assert_gate("G2", "pass", self.compare({SEED: "// cl: /O1\nint value;\n"},
                                                            {SEED: "// cl: /O1\n" + marker + "\nint value;\n"}))
            with self.subTest(remove=marker):
                finding, = self.assert_gate("G2", "fail", self.compare({SEED: marker + "\nint value;\n"},
                                                                       {SEED: "int value;\n"}))
                self.assertIn("removed", finding["message"])
            with self.subTest(duplicate_existing=marker):
                finding, = self.assert_gate("G2", "fail", self.compare({SEED: marker + "\nint value;\n"},
                                                                       {SEED: marker + "\n" + marker + "\nint value;\n"}))
                self.assertIn("Duplicate", finding["message"])
            with self.subTest(duplicate_added=marker):
                finding, = self.assert_gate("G2", "fail", self.compare({SEED: "int value;\n"},
                                                                       {SEED: marker + "\n" + marker + "\nint value;\n"}))
                self.assertIn("Duplicate", finding["message"])
            with self.subTest(header=marker):
                finding, = self.assert_gate("G2", "fail", self.compare({"include/A.h": "struct A {};\n"},
                                                                       {"include/A.h": marker + "\nstruct A {};\n"}))
                self.assertIn("non-vendored C/C++ source", finding["message"])
            with self.subTest(crlf=marker):
                finding, = self.assert_gate("G2", "fail", self.compare({SEED: "int value;\n"},
                                                                       {SEED: (marker + "\r\nint value;\n").encode("ascii")}))
                self.assertIn("LF", finding["message"])
        # Reordering existing // cl: lines while adding // dxsdk must still fail.
        old = "// cl: /W3\n// cl: /W4\nint value;\n"
        reordered = "// cl: /W4\n// dxsdk\n// cl: /W3\nint value;\n"
        finding, = self.assert_gate("G2", "fail", self.compare({SEED: old}, {SEED: reordered}))
        self.assertIn("changed order", finding["message"])

    def test_g2_compiler_reads_raw_lines_and_preserves_order(self):
        old = "// cl: /W3\n// cl: /W4\nint value;\n"
        changed = old.replace("/W3", "/W2")
        findings = self.assert_gate("G2", "fail", self.compare({SEED: old}, {SEED: changed}))
        self.assertEqual(len(findings), 2)
        reordered = "// cl: /W4\n// cl: /W3\nint value;\n"
        finding, = self.assert_gate("G2", "fail", self.compare({SEED: old}, {SEED: reordered}))
        self.assertIn("changed order", finding["message"])
        raw = "/* note\n// cl: /W3\n*/\nint value;\n"
        finding, = self.assert_gate("G2", "fail", self.compare({SEED: "int value;\n"}, {SEED: raw}))
        self.assertEqual(finding["line"], 2)
        stable = old.replace("int value;", "int value = 1; // initialize")
        self.assert_gate("G2", "pass", self.compare({SEED: old}, {SEED: stable}))

    def test_g2_stlport_driver_substring_is_checked_on_state_change(self):
        before = "int value;\n"
        after = before + 'const char *note = "// stlport settings";\n'
        line, = self.assert_gate("G2", "fail", self.compare({SEED: before}, {SEED: after}))
        self.assertEqual(line["line"], 2)
        baseline = "// stlport46\n" + before
        self.assert_gate("G2", "pass", self.compare({SEED: baseline}, {SEED: baseline + after}))

    def test_g3_unnamed_macro_paste_requires_a_provably_safe_spelling(self):
        for paste in ("##", "%:%:", "??=??=", "#" + chr(92) + "\n#"):
            with self.subTest(paste=paste):
                macro = "#define JoinParts(first, second) first " + paste + " second\n"
                result = self.compare({SEED: "int value;\n"}, {SEED: macro + "int value;\n"})
                finding, = self.assert_gate("G3", "fail", result)
                self.assertIn("token pasting", finding["message"])
                self.assertEqual((finding["path"], finding["line"]), (SEED, 1))

    def test_paste_matcher_uses_only_complete_reserved_spellings(self):
        reserved = ("ControlName", "RestrictedWord")
        self.assertTrue(paste_can_form(("Control", "Name"), set(), reserved))
        self.assertTrue(paste_can_form(("argument", "Word"), {"argument"}, reserved))
        self.assertFalse(paste_can_form(("Prefix_", "argument"), {"argument"}, reserved))
        self.assertFalse(paste_can_form(("Control", "SystemName"), set(), reserved))

    def test_g3_safe_prefix_fixed_identifier_and_literal_paste(self):
        original = "#define Member(name) ProjectField_ ## name\n"
        ordinary = (original + "#define Count Value ## Count\n"
                    'const char *note = "a ## b";\n'
                    "// a ## b in the documentation\n")
        self.assert_gate("G3", "pass", self.compare({SEED: original}, {SEED: ordinary}))

    def test_g7_later_declarators_inherit_cpp_top_level_const(self):
        result = self.compare({"src/first.cpp": "const int Same = 7;\n", SEED: ""},
                              {SEED: "const int Different = 1, Same = 2;\n"})
        self.assert_gate("G7", "pass", result)
        source = Source("const int a = 1, b = 2;\n")
        self.assertEqual([(item.name, item.internal) for item in definitions(source)],
                         [("a", True), ("b", True)])
        self.assertEqual([(item.name, item.internal) for item in definitions(source, is_c=True)],
                         [("a", False), ("b", False)])

    def test_g7_each_declarator_keeps_its_own_pointer_const(self):
        source = Source("const int *a = 0, b = 2, *c = 0, *const d = 0;\n")
        self.assertEqual([(item.name, item.internal) for item in definitions(source)],
                         [("a", False), ("b", True), ("c", False), ("d", True)])
        result = self.compare({"src/first.cpp": "const int *same = 0;\n", SEED: ""},
                              {SEED: "const int fresh = 0, *same = 0;\n"})
        self.assertEqual(len(self.assert_gate("G7", "fail", result)), 1)

    def test_g7_common_extern_and_later_definition(self):
        source = Source("extern const int a, b = 2, c;\nconst int f(), d = 1;\n")
        self.assertEqual([(item.name, item.internal) for item in definitions(source)],
                         [("b", False), ("d", True)])

    def test_g8_exact_accepted_symbol_ledger_key_and_component(self):
        base_symbols = {"symbols": {"g_001234A1": {"candidates": [{}]},
                                    "?rva001234A2@Example@@QAEHXZ": {"candidates": [{}]}}}
        result = self.compare({SEED: "", "config/symbols.json": json.dumps(base_symbols)},
                              {SEED: "extern int g_001234A1;\nextern int rva001234A2;\n"})
        self.assert_gate("G8", "pass", result)

    def test_g8_does_not_trust_fields_empty_records_or_changes_to_the_ledger(self):
        prior = {"symbols": {"g_001234A3": {"candidates": []},
                             "Named": {"candidates": [{"name": "g_001234A4"}]}},
                 "unrelated": ["g_001234A5"]}
        head = json.dumps({"symbols": {"g_001234A3": {"candidates": [{}]}}})
        result = self.compare({SEED: "", "config/symbols.json": json.dumps(prior)},
                              {SEED: "extern int g_001234A3, g_001234A4, g_001234A5;\n",
                               "config/symbols.json": head})
        self.assert_gate("G1", "fail", result)
        self.assertEqual(len(self.assert_gate("G8", "fail", result)), 3)

    def test_g4_scalar_floating_conversions_do_not_erase_a_pointer_type(self):
        source = ("float f() { return float(getCount()); }\n"
                  "double g() { return static_cast<double>(getCount()); }\n"
                  "long double h() { return (long double)getCount(); }\n")
        result = self.compare({SEED: ""}, {SEED: source})
        self.assert_gate("G4", "pass", result)

    def test_g4_retains_integral_pointer_and_unknown_type_casts(self):
        source = ("int f() { return int(getCount()); }\n"
                  "float *g() { return (float *)address(); }\n"
                  "Q h() { return static_cast<Q>(thing()); }\n")
        result = self.compare({SEED: ""}, {SEED: source})
        findings = self.assert_gate("G4", "fail", result)
        self.assertEqual({finding["line"] for finding in findings}, {1, 2, 3})
        self.assertEqual(ratchets.counts(Source(source))["cast_call_result"], [1, 2, 3])

    def test_g4_exact_protected_void_pointer_abi_exempts_only_signature_tokens(self):
        name = "?lookup@@YAPBXPAX@Z"
        marker = f"// FUNCTION: BFME2 0x00112233 size=4 name={name}\n"
        old = marker + "int lookup(int n) { return n; }\n"
        correct = marker + "const void *lookup(void *p) { return p; }\n"
        base = {"rows": [{"rva": "0x00112233", "role": "owned", "name": name, "accepted_names": []}]}
        left = {SEED: old, "config/map.json": json.dumps(base)}
        self.assert_gate("G4", "pass", self.compare(left, {SEED: correct}))
        body = correct.replace("{ return p; }", "{ void *shadow = p; return shadow; }")
        result = self.compare(left, {SEED: body})
        hit, = self.assert_gate("G4", "fail", result)
        self.assertEqual(hit["line"], 2)
        self.assertIn("increased by 1", hit["message"])

    def test_g4_abi_does_not_follow_addresses_names_or_types_without_exact_agreement(self):
        name = "?lookup@@YAPAXH@Z"
        marker = f"// FUNCTION: BFME2 0x00112233 size=4 name={name}\n"
        old = marker + "int lookup(int n) { return n; }\n"
        correct = "void *lookup(int n) { return 0; }\n"
        legitimate = {"rva": "0x00112233", "role": "owned", "name": name, "accepted_names": []}
        for why, row, code in (
            ("wrong address", {**legitimate, "rva": "0x00112234"}, correct),
            ("unowned row", {**legitimate, "role": "comdat"}, correct),
            ("different spelling", legitimate, correct.replace("lookup", "find")),
            ("different signature", legitimate, correct.replace("int n", "void *n")),
            ("pointer to pointer", legitimate, correct.replace("void *lookup", "void **lookup")),
        ):
            with self.subTest(why=why):
                old_map = json.dumps({"rows": [row]})
                result = self.compare({SEED: old, "config/map.json": old_map}, {SEED: marker + code})
                self.assert_gate("G4", "fail", result)

    def test_g4_exact_member_void_abi_preserves_qualifiers_and_argument_slots(self):
        name = "?get@Cursor@@QBEPAXPBD@Z"
        marker = f"// FUNCTION: BFME2 0x00112233 size=4 name={name}\n"
        row = {"rva": "0x00112233", "role": "owned", "name": name, "accepted_names": []}
        old = {SEED: marker + "int Cursor::get(const char *key) const { return 0; }\n",
               "config/map.json": json.dumps({"rows": [row]})}
        current = marker + "void *Cursor::get(const char *key) const { return 0; }\n"
        self.assert_gate("G4", "pass", self.compare(old, {SEED: current}))
        self.assert_gate("G4", "fail", self.compare(old, {SEED: current.replace(") const", ")")}))

    def test_g4_header_declarations_bind_to_real_qualified_x86_slots(self):
        path = "include/Job.h"
        names = ("?move@Job@@QAEXPAX@Z", "?get@Inner@Job@@QBEPBXXZ",
                 "?set@Job@@IAEXPBX@Z", "?shared@Job@@SAPAXXZ",
                 "?step@Job@@UAEPAXXZ")
        rows = [{"rva": hex(0x100 + i), "role": "owned", "name": name, "accepted_names": []}
                for i, name in enumerate(names)]
        old = {path: "", "config/map.json": json.dumps({"rows": rows})}
        correct = ("#pragma once\n#ifndef PROJECT_JOB_H\n#define PROJECT_JOB_H\n"
                   "class Job : public Parent {\n"
                   "public:\n"
                   " void move(void *target);\n"
                   " struct Inner { const void *get() const; };\n"
                   " static void *shared();\n"
                   " virtual void *step();\n"
                   "protected:\n"
                   " void set(void const *target);\n"
                   "};\n#endif\n")
        self.assert_gate("G4", "pass", self.compare(old, {path: correct}))
        unrelated = correct.replace("protected:", " void *storage;\nprotected:")
        finding, = self.assert_gate("G4", "fail", self.compare(old, {path: unrelated}))
        self.assertEqual(finding["line"], 10)

    def test_g4_header_abi_does_not_follow_unproved_scopes_or_types(self):
        path = "include/Job.h"
        name = "?move@Job@@QAEXPAX@Z"
        entry = {"rva": "0x00000010", "role": "owned", "name": name, "accepted_names": []}
        old = {path: "", "config/map.json": json.dumps({"rows": [entry]})}
        exact = "class Job { public: void move(void *value); };"
        for invalid in (
            exact.replace("Job", "Decoy"),
            exact.replace("public", "private"),
            exact.replace("void move", "static void move"),
            exact.replace("void move", "virtual void move"),
            exact.replace("void move", "void __stdcall move"),
            exact.replace("*value", "*value, int flags"),
            exact.replace("*value", "const *value"),
            exact.replace("*value", "**value"),
            exact.replace("value);", "value) const;"),
            exact.replace("value);", "value) { return; }"),
            exact.replace("public:", "public: void wrapper() { void move(void *value); } //\n"),
            "namespace Other { " + exact + " }",
            "#if CHOOSE\n" + exact + "\n#endif",
            "#define move patch\n" + exact,
            exact.replace(" };", " void move(void *again); };"),
        ):
            with self.subTest(invalid=invalid):
                self.assert_gate("G4", "fail", self.compare(old, {path: invalid}))

    def test_g4_header_abi_requires_one_protected_owned_address(self):
        path = "include/Job.h"
        name = "?move@Job@@QAEXPAX@Z"
        entry = {"rva": "0x00000010", "role": "owned", "name": name, "accepted_names": []}
        exact = "class Job { public: void move(void *value); };"
        for rows in ([], [{**entry, "role": "comdat"}], [entry, {**entry, "rva": "0x00000011"}]):
            with self.subTest(rows=rows):
                self.assert_gate("G4", "fail", self.compare(
                    {path: "", "config/map.json": json.dumps({"rows": rows})}, {path: exact}))
        result = self.compare(
            {path: "", "config/map.json": json.dumps({"rows": []})},
            {path: exact, "config/map.json": json.dumps({"rows": [entry]})})
        self.assert_gate("G1", "fail", result)
        self.assert_gate("G4", "fail", result)

    def test_g1_limits(self):
        cfg = deepcopy(CONFIG)
        cfg['limits'].update(changed_paths=1, bytes_per_file=10, changed_source_bytes=18)
        result = self.compare({SEED: 'int x;\n', 'include/A.h': ''},
                              {SEED: 'int yy;\n', 'include/A.h': 'int elephant;\n'}, cfg)
        findings = self.assert_gate('G1', 'fail', result)
        self.assertTrue(any('paths' in f['message'] for f in findings))
        self.assertTrue(any('Changed source exceeds' in f['message'] for f in findings))

    def test_g2_duplicates_and_raw_string_decoy(self):
        result = self.compare({SEED: '// FUNCTION: A\n// FUNCTION: A\n// GLOBAL: B\n'},
                              {SEED: '// FUNCTION: A\n// FUNCTION: A\n// GLOBAL: B\n'
                                     'const char *s = R"(hello\n// FUNCTION: C\n)";\n'})
        self.assert_gate('G2', 'pass', result)

    def test_g2_removed_duplicate_and_upstream_marker(self):
        result = self.compare({SEED: '// FUNCTION: A\n// FUNCTION: A\n// UPSTREAM: original\n'},
                              {SEED: '// FUNCTION: A\n// UPSTREAM: modified\n'})
        self.assertEqual(3, len(self.assert_gate('G2', 'fail', result)))

    def test_g3_comment_and_string_keywords_are_opaque(self):
        result = self.compare({SEED: 'int x;\n'},
                              {SEED: '/* asm __emit __declspec(naked) */\n'
                                     'const char *s = "asm // __emit";\n'
                                     'const char *t = R"tag(" // _asm\n'
                                     '#pragma comment(linker, "fake")\n)tag";\n'})
        self.assert_gate('G3', 'pass', result)

    def test_g3_multiline_asm_alias_and_pragmas(self):
        result = self.compare({SEED: ''},
                              {SEED: '#pragma \\\ncomment(linker,"alias")\n'
                                     '__declspec(\nnaked) void f() {\n'
                                     '    __as\\\nm { nop }\n    __emit 0x90;\n}\n'
                                     'int x __attribute__((alias("target")));\n'
                                     '__pragma(optimize("", off))\n'})
        hits = self.assert_gate('G3', 'fail', result)
        self.assertEqual(6, len(hits))

    def test_g3_runtime_and_library_workarounds(self):
        result = self.compare({SEED: ''},
                              {SEED: '#define _CRTIMP\n'
                                     '#undef _CRTIMP\n'
                                     '#define _STLP_USE_MALLOC 1\n'
                                     '#define _STLP_BFME_COPY_TRIVIAL_EXTERN\n'
                                     '#define _strdup __bfme_strdup\n'
                                     '#define IMPORT _declspec(dllimport)\n'
                                     '#define SLOT __imp__atoi\n'
                                     '#include <string.h>\n'
                                     '#include "A.h"\n'
                                     'namespace ArmorCRT { extern "C" __declspec(dllimport) int __cdecl\n'
                                     '    _strcmpi(const char *, const char *); }\n'
                                     'extern "C" unsigned char *_mbscpy(unsigned char *, const unsigned char *);\n'
                                     'extern "C" int (__cdecl *__imp__atoi)(const char *);\n'
                                     'int __cdecl atoi(const char *);\n'
                                     'extern "C" DWORD __stdcall GetTickCount();\n'
                                     'WINBASEAPI DWORD WINAPI TlsGetValue(DWORD);\n'
                                     'extern "C" { double floor(double); }\n'
                                     'void f() { extern int __cdecl atof(const char *); }\n'
                                     '__declspec(nothrow) void g();\n'
                                     'extern "C" void __cdecl free(void *p) { }\n'
                                     '#define _INC_STDLIB\n'})
        labels = sorted(f['message'] for f in self.assert_gate('G3', 'fail', result))
        self.assertEqual(['Added hand-written import declaration.'] * 3
                         + ['Added import table reference.'] * 2
                         + ['Added library configuration macro.'] * 5
                         + ['Added local body with a runtime name.']
                         + ['Added nothrow declaration.']
                         + ['Added runtime function declaration.'] * 7
                         + ['Added runtime function macro.'], labels)

    def test_g3_repeated_include_is_refused_wherever_the_copy_goes(self):
        old = '#include "A.h"\nint x;\n'
        for new in ('#include <a.h>\n' + old, old + '#include "A.h"\n',
                    '#ifdef X\n#include "A.h"\n#endif\n' + old):
            with self.subTest(new=new):
                result = self.compare({SEED: old}, {SEED: new})
                self.assertEqual(1, len(self.assert_gate('G3', 'fail', result)))

    def test_g3_runtime_calls_members_and_ordinary_code_pass(self):
        result = self.compare({SEED: ''},
                              {SEED: '#ifndef BFME_VERSION_H\n#define BFME_VERSION_H\n#endif\n'
                                     '#define BFME_MAX_PLAYERS 8\n'
                                     '#include <string.h>\n#include <assert.h>\n#include <assert.h>\n'
                                     '#ifdef A\n#include "a.h"\n#else\n#include "a.h"\n#endif\n'
                                     '#if 0\n#include "b.h"\n#include "b.h"\n#endif\n'
                                     'namespace Audio { int log(int channel); }\n'
                                     'struct Pool { void free(void *p); static int abs(int); };\n'
                                     'void Pool::free(void *p) { strcpy((char *)p, "x"); }\n'
                                     'template class Table<int>;\n'
                                     'int f(const char *s) {\n'
                                     '    int n = abs(atoi(s));\n'
                                     '    if (n) strcat((char *)s, "y");\n'
                                     '    return atoi(s) * strlen(s);\n'
                                     '}\n'})
        self.assert_gate('G3', 'pass', result)

    def test_g3_header_may_hold_the_library_wide_stlport_setting(self):
        result = self.compare({'include/PreRTS.h': ''},
                              {'include/PreRTS.h': '#define _STLP_USE_NEWALLOC 1\n'
                                                   '#define malloc(s) DebugAlloc(s)\n'})
        self.assert_gate('G3', 'pass', result)

    def test_g3_existing_workaround_may_stay_but_not_be_copied(self):
        old = '#define _CRTIMP\nint x;\n'
        result = self.compare({SEED: old}, {SEED: old + 'int y;\n'})
        self.assert_gate('G3', 'pass', result)
        result = self.compare({SEED: old}, {SEED: old + '#define _CRTIMP\n'})
        self.assertEqual(1, len(self.assert_gate('G3', 'fail', result)))

    def test_g3_c_linkage_on_game_code_is_refused(self):
        result = self.compare({SEED: ''},
                              {SEED: 'extern "C" int gameHelper(int value);\n'
                                     'extern "C" { void first(); int second; }\n'
                                     'extern "C" int (__cdecl *callback)(float x, void *target);\n'
                                     'extern "C" Matrix g_identity;\n'
                                     'extern "C" int table[3] = { 1, 2, 0 };\n'
                                     'extern "C" unsigned __stdcall threadStart(void *param)\n'
                                     '{\n    return 0;\n}\n'})
        findings = self.assert_gate('G3', 'fail', result)
        self.assertEqual(['Added C linkage outside C code.'] * 6, [f['message'] for f in findings])
        self.assertEqual([1, 2, 3, 4, 5, 6], [f['line'] for f in findings])
        self.assertIn('ledger request', findings[0]['remedy'])

    def test_g3_c_linkage_for_c_headers_and_c_objects_passes(self):
        ledger = json.dumps({
            'objects': [{'object_id': 'O1', 'tu': 'src/lua-4.0.1/Lua/lapi.c'},
                        {'object_id': 'O2', 'path': 'd3dx9.lib/obj/math.obj'},
                        {'object_id': 'O3', 'tu': 'src/Game/Thing.cpp'}],
            'rows': [{'name': '_luaGo', 'accepted_names': [], 'object_id': 'O1'},
                     {'name': '_D3DXHelper@4', 'accepted_names': [], 'object_id': 'O2'},
                     {'name': '_thingHelper', 'accepted_names': [], 'object_id': 'O3'}],
        })
        result = self.compare({SEED: '', 'config/map.json': ledger, 'include/codec.h': '',
                               'src/codec/decode.c': ''},
                              {SEED: 'extern "C" {\n#include "lua.h"\n}\n'
                                     'extern "C" void luaGo(lua_State *L, int b);\n'
                                     'extern "C" long __stdcall D3DXHelper(int);\n'
                                     'extern "C" typedef void (*Hook)(void);\n'
                                     'extern int ordinary;\n',
                               'include/codec.h': '#ifdef __cplusplus\nextern "C" {\n#endif\n'
                                                  'int codec_is(const void *data);\n'
                                                  '#ifdef __cplusplus\n}\n#endif\n',
                               'src/codec/decode.c': 'int codec_is(const void *data) { return 0; }\n'})
        self.assert_gate('G3', 'pass', result)
        result = self.compare({SEED: '', 'config/map.json': ledger},
                              {SEED: 'extern "C" void thingHelper(void);\n'})
        self.assertEqual(1, len(self.assert_gate('G3', 'fail', result)))

    def test_g3_existing_c_linkage_may_stay_and_be_removed(self):
        old = 'extern "C" int gameHelper(int value);\nint x;\n'
        result = self.compare({SEED: old}, {SEED: old + 'int y;\n'})
        self.assert_gate('G3', 'pass', result)
        result = self.compare({SEED: old}, {SEED: 'int gameHelper(int value);\nint x;\n'})
        self.assert_gate('G3', 'pass', result)

    def test_g3_vendored_library_keeps_its_configuration_but_shims_do_not(self):
        path = 'src/stlport-4.6/STLport-4.6/src/unit.cpp'
        result = self.compare({path: 'int x;\n'},
                              {path: '#define _STLP_NO_FORCE_INSTANTIATE\nint x;\n'})
        self.assert_gate('G3', 'pass', result)
        shim = 'include/upstream/gamespy-sdk/shims/windows.h'
        result = self.compare({shim: ''}, {shim: '#define _CRTIMP\n'})
        self.assert_gate('G3', 'fail', result)

    def test_g3_allowlist_is_exact_and_does_not_allow_a_second_copy(self):
        old = '__asm nop\n'
        cfg = deepcopy(CONFIG)
        key = next(forbidden(Source(old), Policy(cfg), SEED))[-1]
        cfg['approved_forbidden'][SEED] = {key: 1}
        result = self.compare({SEED: old},
                              {SEED: '\t__asm nop\n__asm nop\n'}, cfg)
        self.assertEqual(1, len(self.assert_gate('G3', 'fail', result)))

    def test_g4_comments_strings_and_offset_free_code(self):
        result = self.compare({SEED: ''},
                              {SEED: '// void * stand-in in a comment\n'
                                     'const char *s = "reinterpret_cast<int*>(this + 4)";\n'
                                     'int C::f() { return this->x + 4; }\n'})
        self.assert_gate('G4', 'pass', result)

    def test_g4_separate_metrics_and_indirect_this(self):
        result = self.compare({SEED: ''},
                              {SEED: 'void *create();\n'
                                     'int C::f() {\n'
                                     '  char *p = reinterpret_cast<char *>(this);\n'
                                     '  p += 4;\n'
                                     '  return *((int*)p) + static_cast<int>(get());\n'
                                     '}\n'})
        hits = self.assert_gate('G4', 'fail', result)
        self.assertEqual(5, len(hits), hits)

    def test_g4_replacement_ratchet_includes_deleted_draft(self):
        result = self.compare({'include/Draft/A.h': 'void *lookup();\n', SEED: ''},
                              {'include/Draft/A.h': None, SEED: 'void *lookup();\n'})
        self.assert_gate('G4', 'pass', result)

    def test_g5_only_new_comment_text_and_vendor_exemption(self):
        result = self.compare({SEED: 'int x; // gen99: 1999-12-31\n', 'src/zlib-1.1.4/a.c': ''},
                              {SEED: 'int y; // gen99: 1999-12-31\n'
                                     '// Mask comes from 0x00ABCDEF.\n'
                                     '// Matches the selected field.\n'
                                     'const char *s = "Author: Fixture Author";\n',
                               'src/zlib-1.1.4/a.c': '// Author: Fixture Author, 1999-12-31\n'})
        self.assert_gate('G5', 'pass', result)

    def test_g5_adds_private_attempt_log(self):
        result = self.compare({SEED: ''},
                              {SEED: '/* 2026-11-19\n'
                                     ' * Author: gen99\n'
                                     ' * @author Somebody\n'
                                     ' * a/gen99/trial on srv' '88 in ~/fictional/build\n'
                                     ' * byte-exact; matches retail bytes; 98% vs retail\n'
                                     ' */\n'})
        self.assertGreaterEqual(len(self.assert_gate('G5', 'fail', result)), 5)

    def sources(self, text):
        return {f'src/a{i}.cpp': text for i in range(4)}

    def test_g6_comment_only_changes_do_not_count(self):
        result = self.compare(self.sources('int f();\n'), self.sources('// Evidence.\nint f();\n'))
        self.assert_gate('G6', 'pass', result)

    def test_g6_literal_edit_is_real_change(self):
        result = self.compare(self.sources('static const char *s = "a";\n'),
                              self.sources('static const char *s = "b";\n'))
        self.assert_gate('G6', 'fail', result)

    def test_g6_valid_promotion_with_four_includers(self):
        result = self.compare({
            **self.sources('#include "Draft/A.h"\nA *get();\n'),
            'include/Draft/A.h': 'struct A {};\n',
        }, {
            **self.sources('#include "A.h"\nA *get();\n'),
            'include/Draft/A.h': None, 'include/A.h': 'struct A {};\n',
        })
        self.assert_gate('G6', 'pass', result)
        self.assert_gate('G7', 'pass', result)

    def test_g6_promotion_fails_for_stale_includer(self):
        result = self.compare({SEED: '#include "Draft/A.h"\n', 'include/Draft/A.h': 'struct A {};\n'},
                              {'include/Draft/A.h': None, 'include/A.h': 'struct A {};\n'})
        findings = self.assert_gate('G6', 'fail', result)
        self.assertEqual(SEED, findings[0]['path'])

    def test_g6_promotion_cannot_exempt_unrelated_changes(self):
        result = self.compare({
            **self.sources('#include "Draft/A.h"\nint f();\n'),
            'include/Draft/A.h': 'struct A {};\n',
        }, {
            **self.sources('#include "A.h"\nint g();\n'),
            'include/Draft/A.h': None, 'include/A.h': 'struct A {};\n',
        })
        self.assert_gate('G6', 'fail', result)

    def test_g7_real_duplicates_with_scope_and_linkage(self):
        result = self.compare({'include/A.h': 'namespace N { union A {}; enum E { one }; }\n'
                                               'extern "C" { int global = 0; }\n',
                               SEED: ''},
                              {'include/B.h': 'namespace N {\n union A\n { int first; };\n'
                                     ' enum E { two };\n}\nint global;\n'})
        self.assertEqual(3, len(self.assert_gate('G7', 'fail', result)))

    def test_g7_forward_declarations_local_and_internal_linkage(self):
        result = self.compare({'include/A.h': 'struct A {};\nextern int global;\n'
                                               'namespace N { struct B {}; }\n'
                                               'static int hidden;\n',
                               SEED: ''},
                              {SEED: 'struct A;\nint global;\nstatic int hidden;\n'
                                     'int f() { enum A { A1 }; int global; return 0; }\n',
                               'include/Other.h': 'namespace Other { struct B {}; }\n'})
        self.assert_gate('G7', 'pass', result)

    def test_g7_duplicate_global_twice_in_one_file(self):
        result = self.compare({SEED: 'int global;\n'}, {SEED: 'int global;\nint global = 1;\n'})
        self.assert_gate('G7', 'fail', result)

    def test_g7_checks_same_identity_across_upstream_and_project_paths(self):
        upstream = 'include/upstream/Example/Shared.h'
        project = 'include/Shared.h'
        result = self.compare({upstream: 'struct Shared {};\n', project: ''},
                              {project: 'struct Shared { int payload; };\n'})
        findings = self.assert_gate('G7', 'fail', result)
        self.assertEqual(project, findings[0]['path'])
        self.assertIn(upstream, findings[0]['message'])
        result = self.compare({upstream: 'struct Shared {};\n', project: ''},
                              {upstream: 'struct Shared {};\nint Global;\nint Global = 1;\n'})
        self.assert_gate('G7', 'fail', result)

    def test_g8_accepted_mangled_exact_and_existing_names(self):
        result = self.compare({'include/A.h': 'int g_111111;\n', SEED: ''},
                              {SEED: 'int rva00123456();\nint sub_00123457();\n'
                                     'Rva00123458Class *get();\nint f() { return g_111111; }\n'})
        self.assert_gate('G8', 'pass', result)

    def test_g8_unsupported_spellings_and_candidate_ledger_edit(self):
        result = self.compare({SEED: ''},
                              {SEED: 'int sub_00123457_extra();\nint g_00FEDCBA;\n',
                               'config/map.json': '{"rows":[{"name":"g_00FEDCBA","accepted_names":[]}]}'})
        findings = self.assert_gate('G8', 'fail', result)
        self.assertEqual(2, len(findings))

    def test_g4_deref_operand_is_not_pointer_arithmetic(self):
        source = Source("""
int C::f() {
    char *p = (char *)this;
    int bytes = sizeof(*this) + 4 + *((int*)p);
    { char *p; bytes += p + 4 - p; }
    char *q = getBuffer(this);
    return bytes + (*(q + 4));
}
""")
        self.assertEqual([], ratchets.counts(source)['this_arithmetic'])

    def test_g4_real_arithmetic_and_nested_alias(self):
        source = Source("""
int C::f() {
    unsigned char *p = (unsigned char *)(this);
    auto *q = p;
    q += 4;
    return (4 + q)[1] + ((char*)this)[0] + (this + 1)->x;
}
""")
        self.assertEqual([5, 6, 6, 6], ratchets.counts(source)['this_arithmetic'])

    def test_g4_cast_target_void_is_not_a_void_declaration(self):
        source = Source("""
static_cast<void *>(p);
sizeof(void *);
(void *)p;
void *factory();
void x(void*const);
""")
        counts = ratchets.counts(source)
        self.assertEqual([5, 6], counts['void_pointer'])
        self.assertEqual([4], counts['c_pointer_cast'])

    def test_g4_cast_call_results_and_call_chain(self):
        source = Source("""
int f() {
    return int(first()) + static_cast<int>(object.get().call())
        + (int)(other()) + static_cast<int>(sum() + 1)
        + (int)other().field + (int)(callee()()[1]);
}
""")
        self.assertEqual([3, 3, 4], ratchets.counts(source)['cast_call_result'])

    def test_g4_c_style_function_pointer_cast(self):
        source = Source('Handler h = (void (__cdecl *)(int))getCallback();')
        self.assertEqual([1], ratchets.counts(source)['c_pointer_cast'])
        self.assertEqual([1], ratchets.counts(source)['cast_call_result'])

    def test_g6_distinguishes_function_like_macro(self):
        result = self.compare(self.sources('#define A(x) (x)\n'),
                              self.sources('#define A (x) (x)\n'))
        self.assert_gate('G6', 'fail', result)

    def test_g6_rename_of_a_promoted_type(self):
        result = self.compare({
            **self.sources('#include "Draft/Temporary.h"\nTemporary *get();\n'),
            'include/Draft/Temporary.h': 'struct Temporary {};\n',
        }, {
            **self.sources('#include "EngineThing.h"\nEngineThing *get();\n'),
            'include/Draft/Temporary.h': None, 'include/EngineThing.h': 'struct EngineThing {};\n',
        })
        self.assert_gate('G6', 'pass', result)

    def test_g7_const_has_external_linkage_in_c(self):
        result = self.compare({'src/a.c': 'const int global = 0;\n', 'src/b.c': ''},
                              {'src/b.c': 'const int global = 1;\n'})
        self.assert_gate('G7', 'fail', result)

    def test_g1_cross_file_move_without_moving_annotation(self):
        definition = 'void Engine::setFlag(int f) { flag = f; }\n'
        result = self.compare({SEED: '// FUNCTION: A\n' + definition, 'src/other.cpp': ''},
                              {SEED: '// FUNCTION: A\n', 'src/other.cpp': definition})
        findings = self.assert_gate('G1', 'fail', result)
        self.assertEqual('src/other.cpp', findings[0]['path'])
        self.assert_gate('G2', 'pass', result)

    def test_g8_address_shaped_member(self):
        result = self.compare({SEED: 'struct Some { int m_unk08; };\n'},
                              {SEED: 'struct Some { int m_00A123CD; int m_unk0x40; };\n'})
        findings = self.assert_gate('G8', 'fail', result)
        self.assertEqual(1, len(findings))

    def test_g5_bare_row_tag_and_address_evidence(self):
        result = self.compare({SEED: ''}, {SEED: '// RVA 0x00123456\n'
                                                   '// Mask recovered from the table at 0x00223344.\n'})
        findings = self.assert_gate('G5', 'fail', result)
        self.assertEqual([1], [f['line'] for f in findings])

    def test_g2_annotation_order_is_part_of_the_contract(self):
        result = self.compare({SEED: '// FUNCTION: first\n// FUNCTION: second\n'},
                              {SEED: '// FUNCTION: second\n// FUNCTION: first\n'})
        self.assert_gate('G2', 'fail', result)

    def test_g2_same_sequence_marker_moved_below_its_body(self):
        definition = 'void Engine::setFlag(int f) { flag = f; }\n'
        result = self.compare({SEED: '// FUNCTION: A\n' + definition + '// FUNCTION: B\n'},
                              {SEED: definition + '// FUNCTION: A\n// FUNCTION: B\n'})
        self.assert_gate('G2', 'fail', result)

    def test_g2_adding_and_editing_a_body_below_its_marker(self):
        old = '// FUNCTION: A\n// FUNCTION: B\nint foo() { return 1; }\n'
        new = ('// FUNCTION: A\nint added() { return 3; }\n'
               '// FUNCTION: B\nint foo() { return 2; }\n')
        self.assert_gate('G2', 'pass', self.compare({SEED: old}, {SEED: new}))

    def test_g3_default_denies_and_does_not_allow_source_pack(self):
        result = self.compare({SEED: ''}, {SEED: '#pragma message("inert")\n'
                                                '#pragma pack(push, 8)\n'
                                                '_Pragma("log")\n'})
        self.assertEqual(3, len(self.assert_gate('G3', 'fail', result)))

    def test_g3_allowlisted_pragma_scopes_and_source_warning(self):
        result = self.compare(
            {SEED: '', 'include/A.h': ''},
            {SEED: '#pragma warning(push)\n__pragma(warning(pop))\n',
             'include/A.h': '#pragma once\n#pragma pack(push, 4)\n_Pragma("pack(pop)")\n'},
        )
        self.assert_gate('G3', 'pass', result)

    def test_g7_new_local_and_anonymous_source_types(self):
        result = self.compare({SEED: ''}, {SEED: 'int f() { struct Point { int x; }; '
                                              'union { int i; char a[4]; } u; return u.i; }\n'})
        hits = self.assert_gate('G7', 'fail', result)
        self.assertEqual(2, len(hits))

    def test_g7_source_forward_declaration_enum_and_existing_debt(self):
        result = self.compare({SEED: 'struct Existing {};\n'},
                              {SEED: 'struct Existing { int count; };\n'
                                     'class Declared;\nenum class Mode { A };\n'
                                     'struct Existing *p = 0;\n'})
        self.assert_gate('G7', 'pass', result)

    def test_g2_can_restore_binding_to_unique_exact_mangled_name(self):
        real = '// FUNCTION: BFME2 name=?parse@Engine@@QAEXXZ\n'
        orphan = '// FUNCTION: BFME2 name=??0Other@@QAE@XZ\n'
        body = 'void Engine::parse() { close(); }\n'
        result = self.compare({SEED: real + orphan + body}, {SEED: real + body + orphan})
        self.assert_gate('G2', 'pass', result)

    def test_g3_approved_assembly_cannot_change_instruction_boundaries(self):
        old = '__asm {\n mov eax, 1\n nop\n}\n'
        cfg = deepcopy(CONFIG)
        key = next(forbidden(Source(old), Policy(cfg), SEED))[-1]
        cfg['approved_forbidden'][SEED] = {key: 1}
        result = self.compare({SEED: old}, {SEED: '__asm {\n mov eax, 1 nop\n}\n'}, cfg)
        self.assert_gate('G3', 'fail', result)

    def test_g5_trailing_rva_on_a_real_declaration_is_evidence(self):
        result = self.compare({SEED: ''}, {SEED: 'extern int value; // RVA 0x00123456\n'})
        self.assert_gate('G5', 'pass', result)

    def test_g6_promotion_of_a_header_without_a_named_type(self):
        result = self.compare({
            **self.sources('#include "Draft/Constants.h"\n'),
            'include/Draft/Constants.h': 'enum { Limit = 2 };\n',
        }, {
            **self.sources('#include "Constants.h"\n'),
            'include/Draft/Constants.h': None,
            'include/Constants.h': 'enum { Limit = 2 };\n',
        })
        self.assert_gate('G6', 'pass', result)

    def test_g6_promotion_renames_primary_type_in_a_multi_type_header(self):
        result = self.compare({
            **self.sources('#include "Draft/Temporary.h"\nTemporary *get();\n'),
            'include/Draft/Temporary.h': 'struct Temporary {};\nstruct Context {};\n',
        }, {
            **self.sources('#include "EngineThing.h"\nEngineThing *get();\n'),
            'include/Draft/Temporary.h': None,
            'include/EngineThing.h': 'struct EngineThing {};\nstruct Context {};\n',
        })
        self.assert_gate('G6', 'pass', result)

    def test_internal_error_from_missing_revision(self):
        tree = self.snapshot({SEED: ''})
        command = [sys.executable, '-B', str(HERE / 'gate.py'), str(self.root), 'missing-ref', tree,
                   '--json', str(self.root / 'result.json')]
        result = subprocess.run(command, encoding='utf-8', capture_output=True)
        self.assertEqual(result.returncode, 2)
        self.assertEqual(json.loads((self.root / 'result.json').read_text())['status'], 'error')
        self.assertIn('**INTERNAL**', result.stdout)



    def test_g1_refuses_non_game_tools_paths(self):
        # Both modifying and adding under src/Code/Tools/ (any case) must fail G1.
        r_mod = self.compare(
            {'src/Code/Tools/WW3D/max2w3d/bchannel.cpp': 'int f() { return 1; }\n'},
            {'src/Code/Tools/WW3D/max2w3d/bchannel.cpp': 'int f() { return 2; }\n'},
        )
        f_mod = self.assert_gate('G1', 'fail', r_mod)
        self.assertTrue(any('src/Code/Tools/' in f['message'] for f in f_mod))
        r_add = self.compare(
            {SEED: ''},
            {SEED: '', 'src/code/tools/WW3D/pluglib/matrix4.cpp': 'int g() { return 0; }\n'},
        )
        f_add = self.assert_gate('G1', 'fail', r_add)
        self.assertTrue(any('src/Code/Tools/' in f['message'] for f in f_add))


if __name__ == '__main__':
    unittest.main()

