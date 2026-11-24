"""The ledger lint that refuses C runtime import names on local bodies."""

from pathlib import Path
import struct
import unittest

import ledger_runtime_names as lint

ROOT = Path(__file__).resolve().parents[2]
ALIASES = '''CLASSES = [
 {"dll": "msvcr71.dll", "names": ["_mbsdup", "_strdup"], "kind": "export-address", "evidence": "test"},
 {"dll": "msvcr71.dll", "names": ["_mbscpy", "strcpy"], "kind": "export-address", "evidence": "test"},
 {"dll": "wsock32.dll", "names": ["htonl", "ntohl"], "kind": "retail-witnessed", "evidence": "test"},
]
'''
SECTION_RVA = 0x1000
SLOTS = {'free': 0x1100, '_mbsdup': 0x1104, '_mbscpy': 0x1108}
THUNKS = {'free': 0x1200, '_mbsdup': 0x1206, '_mbscpy': 0x120C}
LOCAL_JUMP = 0x1300        # a local function whose body is jmp [free slot]


def image():
    """A PE32 image with one section: msvcr71 imports free, _mbsdup and _mbscpy, their linker
    thunks, and one more jmp [free slot] at LOCAL_JUMP."""
    data = bytearray(0x400)
    def put(rva, raw):
        data[rva - SECTION_RVA:rva - SECTION_RVA + len(raw)] = raw
    names = list(SLOTS)
    lookup, name_table, dll_name = 0x1040, 0x1060, 0x10F0
    cursor = name_table
    for index, name in enumerate(names):
        put(cursor, struct.pack('<H', 0) + name.encode() + b'\0')
        put(lookup + index * 4, struct.pack('<I', cursor))
        put(SLOTS[name], struct.pack('<I', cursor))
        cursor += len(name) + 4
    put(dll_name, b'MSVCR71.dll\0')
    put(0x1000, struct.pack('<IIIII', lookup, 0, 0, dll_name, SLOTS[names[0]]))
    for name, rva in THUNKS.items():
        put(rva, b'\xff\x25' + struct.pack('<I', SLOTS[name] + lint.IMAGE_BASE))
    put(LOCAL_JUMP, b'\xff\x25' + struct.pack('<I', SLOTS['free'] + lint.IMAGE_BASE))

    header = bytearray(0x200)
    header[:2] = b'MZ'
    struct.pack_into('<I', header, 0x3C, 0x40)
    header[0x40:0x44] = b'PE\0\0'
    struct.pack_into('<HH', header, 0x44, 0x14C, 1)
    struct.pack_into('<H', header, 0x54, 0xE0)
    struct.pack_into('<H', header, 0x58, 0x10B)
    struct.pack_into('<I', header, 0x58 + 104, 0x1000)
    section = 0x58 + 0xE0
    header[section:section + 8] = b'.text\0\0\0'
    struct.pack_into('<IIII', header, section + 8, len(data), SECTION_RVA, len(data), 0x200)
    return bytes(header + data)


def row(rva, name, role='owned', accepted=None, object_id='O1'):
    return {'rva': '0x%08X' % rva, 'name': name, 'role': role, 'object_id': object_id,
            'accepted_names': accepted if accepted is not None else [name]}


def ledger(*rows, objects=()):
    return {'rows': list(rows), 'objects': [{'object_id': 'O1', 'path': 'Code/Game/Example.cpp'}, *objects]}


class ImportTableTests(unittest.TestCase):
    def test_reads_names_by_slot(self):
        self.assertEqual(lint.pe_imports(image())['msvcr71.dll'], {v: k for k, v in SLOTS.items()})

    def test_rejects_a_non_pe_image(self):
        with self.assertRaises(lint.LedgerError):
            lint.pe_imports(b'XX' + bytes(100))


class LintTests(unittest.TestCase):
    def check(self, *rows, objects=()):
        return lint.check(ledger(*rows, objects=objects), image(), ALIASES)

    def test_linker_thunks_named_for_their_import_pass(self):
        refused, allowed = self.check(row(THUNKS['free'], '_free', 'import_thunk'),
                                      row(THUNKS['_mbsdup'], '__mbsdup', 'import_thunk'))
        self.assertEqual((refused, allowed), ([], []))

    def test_local_body_with_a_c_runtime_name_is_refused(self):
        refused, _ = self.check(row(0x1400, '_free'))
        self.assertEqual([(f['name'], f['runtime_name'], f['primary']) for f in refused], [('_free', 'free', True)])

    def test_alias_class_members_count_as_the_import(self):
        # _strdup is only an alias of the imported _mbsdup; strcpy only of _mbscpy.
        refused, _ = self.check(row(0x1400, '_strdup'), row(0x1410, '_strcpy'))
        self.assertEqual([f['runtime_name'] for f in refused], ['_strdup', 'strcpy'])

    def test_accepted_alias_is_checked_too(self):
        refused, _ = self.check(row(0x1400, '?freeBlock@@YAXPAX@Z', accepted=['?freeBlock@@YAXPAX@Z', '_free']))
        self.assertEqual([(f['name'], f['primary']) for f in refused], [('_free', False)])

    def test_a_slot_claimed_twice_proves_neither_thunk(self):
        # The linker emits one thunk per import; which jump is a local function needs a reviewer.
        refused, _ = self.check(row(THUNKS['free'], '_free', 'import_thunk'),
                                row(LOCAL_JUMP, '_free', 'import_thunk'))
        self.assertEqual([f['rva'] for f in refused], ['0x%08X' % THUNKS['free'], '0x%08X' % LOCAL_JUMP])

    def test_local_jump_renamed_leaves_the_thunk_alone(self):
        refused, _ = self.check(row(THUNKS['free'], '_free', 'import_thunk'),
                                row(LOCAL_JUMP, '?rva00001300@@YAXPAX@Z'))
        self.assertEqual(refused, [])

    def test_thunk_role_with_the_wrong_spelling_is_refused(self):
        # The thunk for the import _mbsdup is the symbol __mbsdup; _mbsdup would be C mbsdup.
        refused, _ = self.check(row(THUNKS['_mbsdup'], '_mbsdup', 'import_thunk'))
        self.assertEqual(len(refused), 1)

    def test_crt_library_objects_may_carry_runtime_names(self):
        crt = {'object_id': 'C1', 'path': 'msvcrt.lib/build/intel/dll_obj/example.obj'}
        refused, _ = self.check(row(0x1400, '_free', object_id='C1'), objects=(crt,))
        self.assertEqual(refused, [])

    def test_cplusplus_names_and_other_dlls_are_ignored(self):
        refused, allowed = self.check(row(0x1400, '?free@@YAXPAX@Z'), row(0x1410, '_htonl'),
                                      row(0x1420, '?print@File@@UAA_NPBDZZ'))
        self.assertEqual((refused, allowed), ([], []))

    def test_reviewed_exception_is_reported_as_allowed(self):
        refused, allowed = self.check(row(0x30830, '?freeBlock@@YAXPAX@Z',
                                          accepted=['?freeBlock@@YAXPAX@Z', '_free']))
        self.assertEqual(refused, [])
        self.assertEqual([(a['rva'], a['name']) for a in allowed], [('0x00030830', '_free')])


@unittest.skipUnless((ROOT / 'baseline/game.dat').is_file(), 'needs the reference image')
class CommittedLedgerTests(unittest.TestCase):
    def test_committed_ledger_has_no_refused_rows(self):
        refused, _ = lint.check(
            __import__('json').loads((ROOT / 'config/map.json').read_text(encoding='utf-8')),
            (ROOT / 'baseline/game.dat').read_bytes(),
            (ROOT / 'ci/managed-nt2/engine/gen/import_aliases.py').read_text(encoding='utf-8'))
        self.assertEqual(refused, [], '\n'.join(f"{f['rva']} {f['name']}" for f in refused))


if __name__ == '__main__':
    unittest.main()
