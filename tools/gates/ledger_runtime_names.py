#!/usr/bin/env python3
"""Ledger lint: refuse C runtime import names on local bodies.

Retail imports the C runtime from msvcr71.dll. A name the program imports
cannot also name a body linked into game.dat: the import library member that
supplies the import also defines the plain symbol, so a second definition
fails to link (LNK2005). A ledger row whose name or accepted alias is a
msvcr71 import, or another export of the same import entry (strcpy and
_mbscpy, _strdup and _mbsdup, _stricmp and _strcmpi, abs and labs, getc and
fgetc, putc and fputc), is therefore mislabelled unless it is

* the linker's import thunk for that entry: the only import_thunk row whose
  bytes are ``jmp [slot]`` for a slot the import directory names with that
  import, and whose name is the thunk's symbol (``_`` + the import name), or
* a body from a C runtime library object (``msvcrt.lib/...``), or
* listed in ``ALLOWED`` with the reason it cannot clash.

The names come from the import directory of baseline/game.dat and the alias
classes in ci/managed-nt2/engine/gen/import_aliases.py. A spelling hits when it
equals a runtime name or is the C symbol of one (``_free`` for ``free``).

Usage: ledger_runtime_names.py [ROOT] [--json PATH]
Exit status: 0 no refused rows, 1 refused rows (listed), 2 unreadable input.
"""

import argparse
import ast
import json
import struct
import sys
from pathlib import Path

RUNTIME_DLL = 'msvcr71.dll'
CRT_OBJECT_PREFIXES = ('msvcrt.lib/',)
IMAGE_BASE = 0x400000

# Reviewed exceptions, keyed by (RVA, spelling). Keep each one specific.
ALLOWED = {
    ('0x00755F00', '_stat'):
        'Lua 4.0.1 lparser.c defines a static stat(); internal linkage cannot clash with the import.',
    # A transitional caller alias, not an identity: the row's name is freeBlock. Translation units
    # that still declare the runtime free (/D_CRTIMP= profiles, local extern "C" declarations and the
    # STLport allocator overlay's ::free) keep resolving to the body until they are migrated.
    ('0x00030830', '_free'):
        'transitional caller alias of freeBlock, the game heap free; remove once no caller spells free',
}


class LedgerError(Exception):
    pass


def sections(image):
    """[(virtual address, raw size, virtual size, raw offset)] of a PE32 image."""
    if image[:2] != b'MZ':
        raise LedgerError('The reference image is not a PE file.')
    pe = struct.unpack_from('<I', image, 0x3C)[0]
    if image[pe:pe + 4] != b'PE\0\0':
        raise LedgerError('The reference image has no PE header.')
    count = struct.unpack_from('<H', image, pe + 6)[0]
    optional = struct.unpack_from('<H', image, pe + 20)[0]
    if struct.unpack_from('<H', image, pe + 24)[0] != 0x10B:
        raise LedgerError('The reference image is not PE32.')
    table = []
    for index in range(count):
        entry = pe + 24 + optional + index * 40
        vsize, va, rawsize, raw = struct.unpack_from('<IIII', image, entry + 8)
        table.append((va, rawsize, vsize, raw))
    return pe, table


def reader(image):
    """read(rva, size) -> bytes or None, from the raw data of the image's sections."""
    _, table = sections(image)

    def read(rva, size):
        for va, rawsize, _, raw in table:
            if va <= rva and rva + size <= va + rawsize:
                return image[raw + rva - va:raw + rva - va + size]
        return None
    return read


def pe_imports(image):
    """{dll lower-case: {IAT slot RVA: import name}} from the image's import directory."""
    try:
        pe, _ = sections(image)
        read = reader(image)
        directory = struct.unpack_from('<I', image, pe + 24 + 104)[0]

        def dword(rva):
            data = read(rva, 4)
            if data is None:
                raise LedgerError(f'RVA 0x{rva:X} lies outside the image sections.')
            return struct.unpack('<I', data)[0]

        def cstring(rva):
            out = bytearray()
            while True:
                data = read(rva + len(out), 1)
                if data is None:
                    raise LedgerError(f'RVA 0x{rva:X} lies outside the image sections.')
                if data == b'\0':
                    return out.decode('ascii')
                out += data

        result = {}
        descriptor = directory
        while True:
            lookup, name, iat = dword(descriptor), dword(descriptor + 12), dword(descriptor + 16)
            if not (lookup or name or iat):
                return result
            slots = result.setdefault(cstring(name).lower(), {})
            cursor, slot = lookup or iat, iat
            while (thunk := dword(cursor)):
                if not thunk & 0x80000000:
                    slots[slot] = cstring(thunk + 2)
                cursor += 4
                slot += 4
            descriptor += 20
    except (struct.error, UnicodeDecodeError) as error:
        raise LedgerError('The reference image has an unreadable import directory.') from error


def alias_classes(source):
    for node in ast.parse(source).body:
        if isinstance(node, ast.Assign) and any(isinstance(t, ast.Name) and t.id == 'CLASSES' for t in node.targets):
            return [c for c in ast.literal_eval(node.value) if c.get('dll', '').lower() == RUNTIME_DLL]
    raise LedgerError('The import alias table has no CLASSES list.')


def runtime_names(imported, classes):
    """Every spelling of a msvcr71 import entry: the imported names and their alias-class members."""
    names = set(imported)
    for entry in classes:
        if names & set(entry['names']):
            names |= set(entry['names'])
    return names


def c_identifier(symbol):
    """The C identifier behind a __cdecl C symbol (``_free`` -> ``free``), else None."""
    if symbol.startswith('_') and '@' not in symbol and '?' not in symbol:
        return symbol[1:]
    return None


def thunk_slot(row, read, slots):
    """The IAT slot of an import_thunk row that jumps through a msvcr71 slot under that import's
    thunk symbol, else None."""
    if row.get('role') != 'import_thunk':
        return None
    code = read(int(row['rva'], 16), 6)
    if code is None or code[:2] != b'\xff\x25':
        return None
    slot = struct.unpack_from('<I', code, 2)[0] - IMAGE_BASE
    imported = slots.get(slot)
    return slot if imported is not None and row['name'] == '_' + imported else None


def check(ledger, image, aliases_source):
    """Return (refused, allowed) findings for the ledger's rows."""
    slots = pe_imports(image).get(RUNTIME_DLL, {})
    if not slots:
        raise LedgerError('The reference image imports nothing by name from msvcr71.dll.')
    names = runtime_names(slots.values(), alias_classes(aliases_source))
    read = reader(image)
    paths = {o['object_id']: o.get('path') or '' for o in ledger.get('objects', ())}
    # The linker emits one thunk per import, so a slot claimed by several rows proves none of them.
    claims = {}
    for row in ledger['rows']:
        slot = thunk_slot(row, read, slots)
        if slot is not None:
            claims.setdefault(slot, []).append(row['rva'])
    thunks = {rvas[0] for rvas in claims.values() if len(rvas) == 1}
    refused, allowed = [], []
    for row in ledger['rows']:
        spellings = [row['name']] + [n for n in row.get('accepted_names', ()) if n != row['name']]
        for spelling in spellings:
            plain = c_identifier(spelling)
            hit = spelling if spelling in names else plain if plain in names else None
            if hit is None:
                continue
            if spelling == row['name'] and row['rva'] in thunks:
                continue
            if paths.get(row.get('object_id'), '').startswith(CRT_OBJECT_PREFIXES):
                continue
            finding = {'rva': row['rva'], 'name': spelling, 'primary': spelling == row['name'],
                       'runtime_name': hit, 'role': row.get('role'), 'object_id': row.get('object_id')}
            reason = ALLOWED.get((row['rva'], spelling))
            if reason:
                allowed.append({**finding, 'reason': reason})
            else:
                refused.append(finding)
    return refused, allowed


def main(argv=None):
    parser = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    parser.add_argument('root', nargs='?', default='.', help='repository checkout (default: .)')
    parser.add_argument('--json', help='also write the findings to this file')
    args = parser.parse_args(argv)
    root = Path(args.root)
    try:
        ledger = json.loads((root / 'config/map.json').read_text(encoding='utf-8'))
        image = (root / 'baseline/game.dat').read_bytes()
        aliases = (root / 'ci/managed-nt2/engine/gen/import_aliases.py').read_text(encoding='utf-8')
        refused, allowed = check(ledger, image, aliases)
    except (OSError, ValueError, KeyError, TypeError, SyntaxError, LedgerError) as error:
        print(f'ledger runtime names: {error}', file=sys.stderr)
        return 2
    for item in allowed:
        print(f"allowed  {item['rva']} {item['name']}: {item['reason']}")
    for item in refused:
        kind = 'name' if item['primary'] else 'accepted alias'
        print(f"REFUSED  {item['rva']} {kind} {item['name']} is the msvcr71 import {item['runtime_name']} "
              f"(role {item['role']}, object {item['object_id']}); give the body its own name")
    if args.json:
        Path(args.json).write_text(json.dumps({'refused': refused, 'allowed': allowed}, indent=2) + '\n',
                                   encoding='utf-8')
    print(f'{len(refused)} refused, {len(allowed)} allowed')
    return 1 if refused else 0


if __name__ == '__main__':
    sys.exit(main())
