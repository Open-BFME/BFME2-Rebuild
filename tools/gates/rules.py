"""The eight source-policy checks. Inputs are Git blobs and trusted policy."""

from collections import Counter, defaultdict
from dataclasses import dataclass
from difflib import SequenceMatcher
from functools import lru_cache
import hashlib
import json
from pathlib import PurePosixPath
import posixpath
import re

from cpp import Source, decode, include_sites, matching
from definitions import definitions, functions, type_sites
from git_objects import ObjectError
import ratchets
import void_abi


REGULAR = 'Keep the change as a regular, non-executable UTF-8 source or header file.'
SCOPE = 'Keep this pull request in existing C/C++ source and eligible headers. Ask a maintainer for other changes.'
FORBIDDEN_FIX = 'Use ordinary C/C++ and the declared symbol. Ask a maintainer about an existing approved exception.'
C_LINKAGE_FIX = ('Use C++ linkage. A C name the ledger puts on a C++ object is a ledger error: file a ledger request '
                 'with the C++ name, or with the evidence that the object is a .c file.')
PROMOTION_FIX = 'Keep this pull request to one item. Move every Draft includer to the permanent header.'
BAD_TEXT = re.compile(r"[\x00-\x08\x0b-\x1f\x7f\x85\u2028\u2029]")
FILE_COMPONENT = re.compile(r'[A-Za-z0-9_][A-Za-z0-9_.+-]*')
WINDOWS_DEVICE = re.compile(r'(?:CON|PRN|AUX|NUL|COM[0-9]|LPT[0-9])(?:\.|$)', re.I)
DRAFT = 'include/Draft/'


@dataclass(frozen=True, order=True)
class Finding:
    path: str
    line: int
    rule: str
    message: str
    remedy: str


@dataclass
class Edit:
    path: str
    old: Source
    new: Source
    old_exists: bool
    new_exists: bool
    new_raw: bytes = b''

    def added_lines(self):
        differ = SequenceMatcher(None, self.old.lines, self.new.lines)
        return {n + 1 for tag, _, _, start, stop in differ.get_opcodes()
                if tag != 'equal' for n in range(start, stop)}


class Policy:
    def __init__(self, values):
        self.limits = values['limits']
        if any(type(self.limits[k]) is not int or self.limits[k] < 1 for k in (
             'changed_paths', 'bytes_per_file', 'changed_source_bytes', 'source_files',
            'path_characters', 'component_characters',
        )):
            raise ObjectError('A trusted gate limit is invalid.')
        self.source = tuple(values['source_extensions'])
        self.header = tuple(values['header_extensions'])
        self.vendored = tuple(values['vendored_prefixes'])
        self.forbidden_source_prefixes = tuple(values.get('forbidden_source_prefixes', ()))
        self.annotations = re.compile(values['annotation_pattern'])
        self.compiler_controls = re.compile(values['compiler_directive_pattern'])
        self.addable_controls = frozenset(values.get('compiler_directive_additions', ()))
        self.allowed_pragmas = {scope: frozenset(names) for scope, names in values['allowed_pragmas'].items()}
        self.runtime_names = frozenset(values.get('runtime_function_names', ()))
        self.platform_names = frozenset(values.get('platform_function_names', ()))
        self.approved = {path: Counter(allowances)
                         for path, allowances in values['approved_forbidden'].items()}
        self.address = re.compile('(?:' + '|'.join(values['address_identifiers']) + ')')
        self.comment = {label: re.compile(expr, re.I)
                        for label, expr in values['comment_patterns'].items()}

    def kind(self, path):
        ext = PurePosixPath(path).suffix.lower()
        if path.startswith('src/') and ext in self.source:
            return 'source'
        if path.startswith('include/') and ext in self.header:
            return 'header'
        return None

    def valid_path(self, path):
        return len(path) <= self.limits['path_characters'] and all(
            len(part) <= self.limits['component_characters'] and FILE_COMPONENT.fullmatch(part)
            and not part.endswith('.') and not WINDOWS_DEVICE.match(part)
            for part in path.split('/')
        )


class Comparison:
    def __init__(self, repo, policy, index, base, head):
        self.repo, self.policy, self.index = repo, policy, index
        self.base, self.head = base, head
        self.deltas = sorted(repo.diff(base, head), key=lambda item: item.path)
        self.edits = {}
        self.uninspectable = False

    def snapshot(self):
        self.index.move(self.base)
        return self.index


def g1(ctx):
    """Source/header scope, mode, encoding, and byte/path budgets."""
    limits = ctx.policy.limits
    out = []
    if len(ctx.deltas) > limits['changed_paths']:
        out.append(Finding(ctx.deltas[limits['changed_paths']].path, 1, 'G1',
                           f'{len(ctx.deltas)} changed paths exceed the limit of {limits["changed_paths"]}.',
                           'Split the change into smaller pull requests.'))
    total = 0
    for delta in ctx.deltas:
        path, old, new = delta.path, delta.old, delta.new
        kind = ctx.policy.kind(path)
        problem = None
        if kind is None:
            problem = 'This path is maintainer-owned or outside the eligible source and header roots.'
        elif (matched := next((p for p in ctx.policy.forbidden_source_prefixes
                               if path.casefold().startswith(p.casefold())), None)):
            problem = f'This path is under a non-game tool tree ({matched}) and is not linked into game.dat.'
        elif not ctx.policy.valid_path(path):
            problem = 'The path does not follow compiler CI filename limits.'
        elif kind == 'source' and not old:
            problem = 'This change adds a source translation unit.'
        elif kind == 'source' and not new:
            problem = 'This change deletes an existing source translation unit.'
        elif kind == 'header' and not old and path.startswith(DRAFT):
            problem = 'This change adds a Draft header.'
        elif kind == 'header' and not new and not path.startswith(DRAFT):
            problem = 'This change deletes a permanent header.'
        if problem:
            out.append(Finding(path, 1, 'G1', problem, SCOPE))
            # Still inspect allowed source suffixes for history and contributor feedback.
            if kind is None:
                continue
        if any(entry and not entry.regular for entry in (old, new)):
            ctx.uninspectable = True
            out.append(Finding(path, 1, 'G1', 'The path has a non-source Git file mode.', REGULAR))
            continue
        limit = limits['bytes_per_file']
        new_size, new_raw = ctx.repo.blob(new.oid, limit) if new else (0, b'')
        total += new_size
        if total > limits['changed_source_bytes']:
            out.append(Finding(path, 1, 'G1', f'Changed source exceeds {limits["changed_source_bytes"]} bytes.',
                               'Split the change into smaller pull requests.'))
            ctx.uninspectable = True
            break
        if new_raw is None:
            ctx.uninspectable = True
            out.append(Finding(path, 1, 'G1', f'The file exceeds {limit} bytes.', 'Keep the file within the source size limit.'))
            continue
        old_raw = ctx.repo.blob(old.oid, limit)[1] if old else b''
        if old_raw is None:
            ctx.uninspectable = True
            out.append(Finding(path, 1, 'G1', f'The base file exceeds the inspectable limit of {limit} bytes.',
                               'Ask a maintainer to review the oversized base file.'))
            continue
        sources = []
        for raw in (old_raw, new_raw):
            try:
                content = decode(raw)
            except UnicodeDecodeError as error:
                out.append(Finding(path, raw.count(b'\n', 0, error.start)+1, 'G1', 'The blob is not UTF-8.', REGULAR))
                break
            invalid = BAD_TEXT.search(content)
            if invalid:
                out.append(Finding(path, content.count('\n', 0, invalid.start()) + 1, 'G1',
                                   'The blob contains a control character or unsupported line separator.', REGULAR))
                break
            sources.append(Source(content))
        if len(sources) == 2:
            ctx.edits[path] = Edit(path, *sources, old is not None, new is not None, new_raw)
        else:
            ctx.uninspectable = True
    out.extend(moved_bodies(ctx))
    return out


def moved_bodies(ctx):
    """Refuse literal cross-file moves of explicitly defined external functions."""
    edits = [edit for edit in ctx.edits.values()
             if ctx.policy.kind(edit.path) == 'source'
             and edit.old_exists and edit.new_exists]
    if len(edits) < 2:
        return
    old_owners, new_owners = defaultdict(set), defaultdict(dict)
    for edit in edits:
        if edit.old.ordered == edit.new.ordered:
            continue
        old = {key for key, _ in functions(edit.old)}
        new = dict(functions(edit.new))
        for removed in old - new.keys():
            old_owners[removed].add(edit.path)
        for added in new.keys() - old:
            new_owners[added][edit.path] = new[added]
    for key in old_owners.keys() & new_owners.keys():
        for path, line in new_owners[key].items():
            old = sorted(old_owners[key] - {path})
            if old:
                name = key[0]
                yield Finding(path, line, 'G1', f'Function {name} moved from {old[0]}.',
                              f'Restore its body in {old[0]}. Request an owner change through the ledger process.')


def annotations(source, policy):
    return [(c.line, source.lines[c.line-1]) for c in source.comments
            if c.kind == 'line' and c.end_line == c.line
            and policy.annotations.match(source.lines[c.line-1])]


def paired_surplus(before, after):
    """Each appearance is (line, value). Report unmatched appearances in stable order."""
    remaining = Counter(value for _, value in before)
    for line, value in after:
        if remaining[value]:
            remaining[value] -= 1
        else:
            yield line, value


def compiler_controls(source, policy):
    # Compiler readers use physical lines, even inside a block comment or
    # a continued C++ comment. Do not filter these through the C++ lexer.
    return [(line, text) for line, text in enumerate(source.lines, 1)
            if policy.compiler_controls.fullmatch(text)]


def _raw_line_has_cr(raw, line_number):
    if not raw:
        return False
    raw_lines = raw.splitlines(keepends=True)
    if not 1 <= line_number <= len(raw_lines):
        return False
    return b"\r" in raw_lines[line_number - 1]


def changed_compiler_controls(edit, policy):
    old, new = (compiler_controls(source, policy) for source in (edit.old, edit.new))
    before, after = [text for _, text in old], [text for _, text in new]
    restore = "Restore the original compiler directive. Ask a maintainer for compiler setting changes."
    if before != after:
        added = list(paired_surplus(old, new))
        removed = list(paired_surplus(new, old))
        is_non_vendored_source = (
            policy.kind(edit.path) == 'source'
            and edit.old_exists and edit.new_exists
            and not any(edit.path.startswith(prefix) for prefix in policy.vendored)
        )
        before_set = {t.rstrip(" \t") for t in before}
        accepted_added_lines = set()
        seen_added = set()
        rejected_added = []
        guard_failed = False
        for line, text in added:
            canon = text.rstrip(" \t")
            if canon in policy.addable_controls:
                if text != canon:
                    guard_failed = True
                    yield Finding(edit.path, line, "G2",
                                  f"SDK directive {canon} must not have trailing whitespace.",
                                  f"Write {canon} with no trailing spaces or tabs.")
                elif not is_non_vendored_source:
                    guard_failed = True
                    yield Finding(edit.path, line, "G2",
                                  f"SDK directive {canon} may only be added to an existing non-vendored C/C++ source file.",
                                  "Move the SDK directive to the .cpp/.c source file that needs it.")
                elif canon in before_set or canon in seen_added:
                    guard_failed = True
                    yield Finding(edit.path, line, "G2",
                                  f"Duplicate SDK directive {text}.",
                                  "Keep at most one copy of each SDK directive per source file.")
                elif _raw_line_has_cr(edit.new_raw, line):
                    guard_failed = True
                    yield Finding(edit.path, line, "G2",
                                  f"SDK directive {text} must use LF line endings (not CRLF).",
                                  "Convert the directive line to LF line endings.")
                else:
                    seen_added.add(canon)
                    accepted_added_lines.add(line)
            else:
                rejected_added.append((line, text))
        after_existing = [text for line, text in new if line not in accepted_added_lines]
        if not rejected_added and not guard_failed and not removed and before != after_existing:
            first = next(i for i, (a, b) in enumerate(zip(before, after_existing)) if a != b)
            existing_new = [(line, text) for line, text in new if line not in accepted_added_lines]
            yield Finding(edit.path, existing_new[first][0], "G2", "Compiler directives changed order.", restore)
        for direction, changes in (("added or changed", rejected_added), ("removed (base line)", removed)):
            for line, _ in changes:
                yield Finding(edit.path, line, "G2", f"A protected compiler directive was {direction}.", restore)
        if rejected_added or removed or before != after_existing or len(accepted_added_lines) != len(added):
            return
    # The build driver also treats this raw substring anywhere in a file
    # as enabling STLport. Match its actual boolean without reserving
    # unrelated prose occurrences when the setting is already enabled.
    mentions = [[line for line, text in enumerate(source.lines, 1) if "// stlport" in text]
                for source in (edit.old, edit.new)]
    if bool(mentions[0]) != bool(mentions[1]):
        line = (mentions[1] or mentions[0])[0]
        location = "" if mentions[1] else " (base line)"
        yield Finding(edit.path, line, "G2", f"The STLport compiler setting changed{location}.", restore)


def g2(ctx):
    """Keep compiler controls, generated annotations and existing anchors."""
    for edit in ctx.edits.values():
        yield from changed_compiler_controls(edit, ctx.policy)
        old, new = (annotations(source, ctx.policy) for source in (edit.old, edit.new))
        old_sequence = [text for _, text in old]
        new_sequence = [text for _, text in new]
        if old_sequence == new_sequence:
            if old_sequence:
                yield from changed_anchors(edit, old, new)
            continue
        if Counter(old_sequence) == Counter(new_sequence):
            mismatch = next(i for i, (left, right) in enumerate(zip(old_sequence, new_sequence))
                            if left != right)
            yield Finding(edit.path, new[mismatch][0], 'G2', 'Generated annotations changed order.',
                          'Restore their original order and keep each record above its function body.')
            continue
        added = list(paired_surplus(old, new))
        removed = list(paired_surplus(new, old))
        for direction, lines in (('added or changed', added), ('removed', removed)):
            for line, text in lines:
                label = text.lstrip().removeprefix('//').strip().split(':', 1)[0].split(maxsplit=1)[0]
                location = ' (base line)' if direction == 'removed' else ''
                fix = ('Restore the removed annotation verbatim.' if direction == 'removed'
                       else 'Remove this new annotation or restore the original verbatim.')
                yield Finding(edit.path, line, 'G2', f'A generated {label} annotation was {direction}{location}.',
                              f'{fix} Route ownership or name changes through a ledger request.')


def changed_anchors(edit, old, new):
    def owners(source, markers):
        # Bind uniquely located recognizable definitions to the row marker
        # above them. Header provenance and other markers are irrelevant.
        row_markers = [(line, text) for line, text in markers
                       if text.lstrip().removeprefix('//').lstrip().startswith('FUNCTION:')]
        line_map = defaultdict(list)
        definitions_in_order = sorted((line, key) for key, line in functions(source, include_internal=True))
        cursor = 0
        for line, key in definitions_in_order:
            while cursor < len(row_markers) and row_markers[cursor][0] < line:
                cursor += 1
            # Retain only the declaration head so body edits do not mask
            # movement of a marker past the same function declaration.
            name, payload = key
            brace = payload.index('{')
            line_map[(name, payload[:brace])].append((cursor-1, line))
        return line_map, [marker_owner(text) for _, text in row_markers]

    if not any('FUNCTION:' in text for _, text in old):
        return
    before, _ = owners(edit.old, old)
    after, records = owners(edit.new, new)
    record_counts = Counter(records)
    for key in before.keys() & after.keys():
        if len(before[key]) != 1 or len(after[key]) != 1:
            continue
        origin, _ = before[key][0]
        destination, line = after[key][0]
        if origin == destination:
            continue
        # A historical misplaced record can be repaired in order if the
        # destination uniquely spells this exact qualified source name.
        if destination >= 0 and records[destination] == key[0] and record_counts[key[0]] == 1:
            continue
        yield Finding(edit.path, line, 'G2',
                      f'The generated FUNCTION annotation for {key[0]} moved relative to the body.',
                      'Keep the original annotation above this body, or reattach it to its uniquely named record.')


def marker_owner(text):
    symbol = re.search(r'\bname=(\S+)', text)
    if symbol is None:
        return None
    name = symbol[1]
    if not name.startswith('?'):
        simple = re.fullmatch(r'[_@]?([A-Za-z_]\w*?)(?:@\d+)?', name)
        return simple[1] if simple else None
    kind = name[:3]
    special = kind in ('??0', '??1')
    if name.startswith('??') and not special:
        return None
    if '@@' not in name:
        return None
    parts = name[3 if special else 1:].split('@@', 1)[0].split('@')
    if not all(part.isidentifier() for part in parts):
        return None
    if special:
        parts.insert(0, ('~' if kind == '??1' else '') + parts[0])
    return '::'.join(reversed(parts))


ASM = {'asm', '__asm', '_asm', '__asm__'}
EMIT = {'_emit', '__emit', '__emit__'}
DECO = {'__declspec', '__attribute', '__attribute__'}
PRAGMA = {'optimize', 'optimization_level', 'optimize_for', 'GCC', 'clang'}
ALIAS = {'__alias', '__alias__', 'alias', 'weakref', '__weakref__', 'selectany', '__selectany__'}


def pragma_kind(values, policy, path):
    directive = values[0] if values else ''
    if directive in policy.allowed_pragmas['all'] or directive in policy.allowed_pragmas.get(
        policy.kind(path), ()
    ):
        return None
    if directive in ('weak', 'redefine_extname', 'alias'):
        return 'symbol alias pragma'
    if directive == 'comment' and any(word in ('linker', 'lib') for word in values[1:4]):
        return 'linker pragma'
    if directive in PRAGMA and (directive not in ('GCC', 'clang') or any(
        word in ('optimize', 'optimize_for', 'optimization_level') for word in values[1:3]
    )):
        return 'optimization pragma'
    return 'compiler pragma' if not directive else f'compiler pragma {directive}'


PASTED_CONTROLS = tuple(sorted(ASM | EMIT | DECO | ALIAS | {
    "naked", "__naked__", "__pragma", "_Pragma", "noinline", "__noinline",
    "__noinline__", "__assume", "__forceinline", "WWINLINE",
}))


def paste_can_form(parts, parameters, names):
    """Match literal pieces and possible macro argument spellings in bounded time."""
    for name in names:
        offsets = {0}
        for part in parts:
            if part in parameters:
                # A preprocessing argument may also be empty.
                offsets = set(range(min(offsets), len(name) + 1))
            else:
                offsets = {at + len(part) for at in offsets if name.startswith(part, at)}
            if not offsets:
                break
        if len(name) in offsets:
            return True
    return False


def unsafe_paste(source, start, end):
    """Decide whether any direct paste or argument can form a protected control."""
    values = source.ordered
    if values[start + 1:start + 2] != ("define",) or end < start + 4:
        return False
    replacement = start + 3
    parameters = set()
    if (values[replacement:replacement + 1] == ("(",)
            and source.tokens[replacement - 1].stop == source.tokens[replacement].offset):
        close = matching(values, replacement)
        if close is None or close >= end:
            return "##" in values[replacement:end]
        parameters.update(word for word in values[replacement + 1:close] if word.isidentifier())
        if "..." in values[replacement + 1:close]:
            parameters.add("__VA_ARGS__")
        replacement = close + 1
    cursor = replacement
    while cursor < end:
        if values[cursor] != "##":
            cursor += 1
            continue
        if cursor == replacement:
            return True
        parts = [values[cursor - 1]]
        while cursor < end and values[cursor] == "##":
            if cursor + 1 == end or values[cursor + 1] == "##":
                return True
            parts.append(values[cursor + 1])
            cursor += 2
        if paste_can_form(parts, parameters, PASTED_CONTROLS):
            return True
    return False


# Workarounds for the C runtime, the SDK and STLport. The original was built
# from the stock headers with one setting per library, so a source file never
# needs to declare a runtime function, reach an import slot or reconfigure a
# library header itself.
LIBRARY_MACRO = re.compile(r'_CRTIMP\w*|_BFME_\w+|_STLP_BFME_\w+|BFME_PARTICLE_LIST_NODE_TAIL'
                           r'|BFME_HASHTABLE_NEW_NODE_NOFORCEINLINE|_DLL|_MT|_STL|__STL_\w*|_INC_\w+'
                           r'|dllimport|DECLSPEC_IMPORT'
                           r'|WIN(?:BASE|USER|GDI|ADV|MM|OLE|SHELL)API')
# STLport settings are refused in source files only. A shared header may hold the
# library-wide setting, as Zero Hour's PreRTS.h does with _STLP_USE_NEWALLOC.
SOURCE_ONLY_MACRO = re.compile(r'_STLP_\w+')
IMPORT_MACROS = {'_CRTIMP', 'DECLSPEC_IMPORT', 'WINBASEAPI', 'WINUSERAPI', 'WINGDIAPI', 'WINADVAPI',
                 'WINMMAPI', 'WINOLEAPI', 'WINSHELLAPI', 'WINSOCK_API_LINKAGE'}
DECLSPEC = {'__declspec', '_declspec'}
CALLING = {'__cdecl', '_cdecl', 'cdecl', '__stdcall', '_stdcall', 'WINAPI', 'APIENTRY', 'CALLBACK',
           '__fastcall', 'PASCAL', 'WSAAPI', 'STDAPICALLTYPE'} | IMPORT_MACROS
NOT_DECLARATOR = {'return', 'else', 'do', 'case', 'throw', 'new', 'delete', 'sizeof', 'goto'}
REPEATABLE = {'assert.h', 'cassert'}


def linkage_blocks(values):
    """Per token: true inside an `extern "C" { ... }` block."""
    stack, flags = [], []
    for i, value in enumerate(values):
        flags.append(any(stack))
        if value == '{':
            stack.append(i > 1 and values[i-1].startswith('"') and values[i-2] == 'extern')
        elif value == '}' and stack:
            stack.pop()
    return flags


def statement_start(values, i):
    while i > 0 and values[i-1] not in (';', '{', '}'):
        i -= 1
    return i


def condition_frames(source):
    """Yield (directive start, end, branch path, inside #if 0) for each directive."""
    values = source.ordered
    stack, serial = [], 0
    for start, end in source.directives():
        word = values[start+1] if start + 1 < end else ''
        if word in ('if', 'ifdef', 'ifndef'):
            serial += 1
            stack.append((serial, 0, values[start+1:end] == ('if', '0')))
        elif word in ('elif', 'else') and stack:
            serial_id, branch, _ = stack[-1]
            stack[-1] = (serial_id, branch + 1, False)
        elif word == 'endif' and stack:
            stack.pop()
        yield start, end, tuple(x[:2] for x in stack), any(x[2] for x in stack)


def include_spelling(values):
    """The header an include names, the same for "..." and <...>."""
    if len(values) == 1 and values[0].startswith('"') and values[0].endswith('"'):
        name = values[0][1:-1]
    elif len(values) > 2 and values[0] == '<' and values[-1] == '>':
        name = ''.join(values[1:-1])
    else:
        return None
    return name.replace('\\', '/').lower()


def import_at(values, i, end):
    """An import declaration or import-slot name starting at values[i], before `end`."""
    value = values[i]
    if value.startswith(('__imp_', '_imp__')) and value.isidentifier():
        return i, i + 1, 'import table reference'
    if value in IMPORT_MACROS:
        return i, i + 1, 'hand-written import declaration'
    if value in DECLSPEC and values[i+1:i+2] == ('(',):
        close = matching(values, i+1)
        if close is not None and close < end:
            args = values[i+2:close]
            if 'dllimport' in args:
                return i, close + 1, 'hand-written import declaration'
            if 'nothrow' in args:
                return i, close + 1, 'nothrow declaration'
    return None


def without_declspecs(values):
    out, i = [], 0
    while i < len(values):
        if values[i] in DECLSPEC and values[i+1:i+2] == ('(',):
            close = matching(values, i+1)
            if close is not None:
                i = close + 1
                continue
        out.append(values[i])
        i += 1
    return out


def workaround_spans(source, policy, path):
    """Yield (token start, end, kind) for runtime, SDK and library workarounds."""
    if path.startswith(policy.vendored) and '/shims/' not in path:
        return
    is_source = policy.kind(path) == 'source'
    values = source.ordered
    includes = defaultdict(list)
    previous = ()
    directive_tokens = set()
    for start, end, frame, dead in condition_frames(source):
        directive_tokens.update(range(start, end))
        words = values[start+1:end]
        word = words[0] if words else ''
        if word in ('define', 'undef') and len(words) > 1:
            name = words[1]
            guard = word == 'define' and len(words) == 2 and previous == ('ifndef', name)
            if not guard and (LIBRARY_MACRO.fullmatch(name)
                              or (is_source and SOURCE_ONLY_MACRO.fullmatch(name))):
                yield start, end, 'library configuration macro'
            elif is_source and name in policy.runtime_names:
                yield start, end, 'runtime function macro'
            else:
                # A macro body can spell an import as well as a declaration can.
                for i in range(start + 3, end):
                    found = import_at(values, i, end)
                    if found:
                        yield found
        elif word == 'include' and not dead:
            spelling = include_spelling(words[1:])
            if (spelling and spelling.endswith(('.h', '.hpp', '.hh'))
                    and spelling.rsplit('/', 1)[-1] not in REPEATABLE):
                includes[spelling].append((start, end, frame))
        previous = tuple(words[:2])
    # Every copy of a header included twice on one branch path is reported, so the
    # copy a change adds is refused wherever it is placed.
    for sites in includes.values():
        for start, end, frame in sites:
            if any(other is not start and (frame[:len(path_)] == path_ or path_[:len(frame)] == frame)
                   for other, _, path_ in sites):
                yield start, end, 'repeated include'

    names = policy.runtime_names | policy.platform_names
    in_linkage = linkage_blocks(values)
    for i, value in enumerate(values):
        if i in directive_tokens:
            continue
        found = import_at(values, i, len(values))
        if found:
            yield found
            continue
        if value not in names or values[i+1:i+2] != ('(',):
            continue
        close = matching(values, i+1)
        after = values[close+1:close+2] if close is not None else ()
        if after not in ((';',), ('throw',), ('{',)):
            continue
        head = without_declspecs(values[statement_start(values, i):i])
        before = values[i-1] if i else ''
        declarator = before in CALLING or before in ('*', '&') or (
            before.isidentifier() and before not in NOT_DECLARATOR)
        # A runtime or SDK name is only taken as a hand declaration when the
        # statement also asks for C linkage, a calling convention or an import.
        marked = in_linkage[i] or 'extern' in head or any(x in CALLING for x in head)
        if declarator and marked and '=' not in head and '(' not in head:
            # A local body cannot be the imported function of the same name.
            yield i, close + 1, ('local body with a runtime name' if after == ('{',)
                                 else 'runtime function declaration')


CPLUSPLUS_GUARDS = {('ifdef', '__cplusplus'), ('if', 'defined', '__cplusplus'),
                    ('if', 'defined', '(', '__cplusplus', ')')}


def declared_name(values):
    """The name one declaration or definition statement declares, else None."""
    depth, stop = 0, len(values)
    for i, value in enumerate(values):
        if value == '(' and depth == 0:
            inner = values[i+1:i+2]
            if inner and (inner[0] in CALLING or inner[0] == '*'):
                # A pointer declarator: int (__cdecl *name)(...).
                close = matching(values, i)
                names = [v for v in values[i+1:close] if v.isidentifier() and v not in CALLING]
                return names[-1] if names else None
            before = values[i-1] if i else ''
            return before if before.isidentifier() and before not in CALLING else None
        if value in ('=', '[', '{') and depth == 0:
            stop = i
            break
        if value in ('(', '['):
            depth += 1
        elif value in (')', ']'):
            depth -= 1
    names = [v for v in values[:stop] if v.isidentifier()]
    return names[-1] if names else None


def linkage_statements(values, start, stop, skip):
    """Yield [start, end) token intervals of the statements in values[start:stop]."""
    i = start
    while i < stop:
        if i in skip or values[i] == ';':
            i += 1
            continue
        j = i
        while j < stop and values[j] != ';' and j not in skip:
            if values[j] == '{':
                close = matching(values, j)
                if close is None or close >= stop:
                    return
                j = close
                # A function body ends its statement; an initializer runs on to its ';'.
                if values[close+1:close+2] != (';',) and values[j-1:j] != ('=',):
                    break
            j += 1
        yield i, min(j + 1, stop)
        i = j + 1


def c_linkage_spans(source, policy, path, c_names=frozenset()):
    """Yield (token start, end) for C linkage given to code that is not C.

    Exempt are C files and vendored libraries, `extern "C"` wrapped only around
    #include lines (a C header), the `#ifdef __cplusplus` form of a header that
    C code includes too, typedefs, and declarations whose name is a runtime or
    SDK function (refused separately when hand-written) or a C name the base
    ledger places on a C object, a prebuilt library or an import.
    """
    if path.startswith(policy.vendored) or path.lower().endswith('.c') or not policy.kind(path):
        return
    values = source.ordered
    directive_tokens, guarded = set(), set()
    for start, end in source.directives():
        directive_tokens.update(range(start, end))
        if values[start+1:end] in CPLUSPLUS_GUARDS:
            guarded.add(end)
    names = policy.runtime_names | policy.platform_names | c_names
    for i in range(len(values) - 1):
        if values[i] != 'extern' or values[i+1] != '"C"' or i in directive_tokens or i in guarded:
            continue
        if values[i+2:i+3] == ('{',):
            close = matching(values, i + 2)
            if close is None:
                continue
            spans = list(linkage_statements(values, i + 3, close, directive_tokens))
        else:
            spans = list(linkage_statements(values, i, len(values), directive_tokens))[:1]
        for start, end in spans:
            statement = values[start:end]
            body = statement[2:] if statement[:2] == ('extern', '"C"') else statement
            if 'typedef' in body or any(import_at(values, n, end) for n in range(start, end)):
                continue
            if declared_name(body) in names:
                continue
            yield start, end


def forbidden(source, policy, path, c_names=frozenset()):
    """Yield (physical start, end, construct kind, exact lexical fingerprint)."""
    values = source.ordered

    def occurrence(start, end, kind):
        fingerprint = values[start:end]
        if kind in ('inline assembly', 'emitted bytes'):
            # A line break can delimit an instruction inside MS inline asm.
            # Spaces or comments around unchanged instructions are irrelevant.
            pairs = zip(source.tokens[start:end-1], source.tokens[start+1:end])
            line_breaks = tuple('\n' in source.code[a.stop:b.offset] for a, b in pairs)
            fingerprint = (fingerprint, line_breaks)
        payload = json.dumps(fingerprint, ensure_ascii=True, separators=(',', ':')).encode('ascii')
        signature = hashlib.sha256(payload).hexdigest()
        return source.tokens[start].line, source.tokens[end-1].end_line, kind, signature

    has_paste = '##' in values
    for start, end in source.directives():
        if has_paste and unsafe_paste(source, start, end):
            yield occurrence(start, end, 'token pasting that can form a forbidden compiler control')
        if values[start+1:start+2] == ('pragma',):
            kind = pragma_kind(values[start+2:end], policy, path)
            if kind:
                yield occurrence(start, end, kind)

    for i, value in enumerate(values):
        following = values[i+1:i+2]
        if value in ASM or value in EMIT and not (i and values[i-1] in ASM):
            end = i + 1
            if following and following[0] in ('{', '('):
                close = matching(values, end)
                if close is not None:
                    end = close + 1
            elif end < len(values) and following[0] not in (';', '}'):
                line = source.tokens[end-1].line
                while end < len(values) and source.tokens[end].line == line and values[end] not in (';', '}'):
                    end += 1
            yield occurrence(i, end, 'inline assembly' if value in ASM else 'emitted bytes')
        elif value in DECO | {'__pragma', '_Pragma'} and following == ('(',):
            close = matching(values, i+1)
            if close is None:
                continue
            args = values[i+2:close]
            kind = None
            if value == '__pragma':
                kind = pragma_kind(args, policy, path)
            elif value == '_Pragma':
                if len(args) == 1:
                    literal = re.fullmatch(r'(?:[LuU]|u8)?"(.*)"', args[0], re.S)
                    if literal:
                        decoded = re.sub(r'\\(["\\])', r'\1', literal[1])
                        kind = pragma_kind(Source(decoded).ordered, policy, path)
                if kind is None and (len(args) != 1 or not re.fullmatch(r'(?:[LuU]|u8)?".*"', args[0], re.S)):
                    kind = 'unrecognized compiler pragma'
            elif value in DECO:
                if 'naked' in args or '__naked__' in args:
                    kind = 'naked function'
                elif ALIAS.intersection(args):
                    kind = 'symbol alias'
            if kind:
                yield occurrence(i, close+1, kind)
        elif values[i:i+2] == ('[', '['):
            close = matching(values, i)
            if close is not None and ALIAS.intersection(values[i+2:close]):
                yield occurrence(i, close + 1, 'symbol alias')

    for start, end, kind in workaround_spans(source, policy, path):
        yield occurrence(start, end, kind)
    for start, end in c_linkage_spans(source, policy, path, c_names):
        yield occurrence(start, end, 'C linkage outside C code')


def g3(ctx):
    """Refuse forbidden C/C++ constructs added by this change."""
    c_names = None
    for edit in ctx.edits.values():
        if c_names is None and 'extern' in edit.new.ordered and ctx.policy.kind(edit.path):
            oid = ctx.repo.ledger_oid(ctx.base, optional=True)
            c_names = ctx.index.base_ledger(oid).c_linkage if oid else frozenset()
        names = c_names or frozenset()
        candidates = list(forbidden(edit.new, ctx.policy, edit.path, names))
        if not candidates:
            continue
        added = edit.added_lines()
        old = Counter(key for _, _, _, key in forbidden(edit.old, ctx.policy, edit.path, names))
        allowed = ctx.policy.approved.get(edit.path, Counter())
        unchanged = Counter()
        for start, end, _, key in candidates:
            if not any(n in added for n in range(start, end+1)):
                unchanged[key] += 1
        # Exact approved forms can remain in edited code, up to both the
        # maintainer cap and their original count. New copies cannot use it.
        remaining = (old & allowed) - unchanged
        for start, end, label, key in candidates:
            lines = [n for n in range(start, end+1) if n in added]
            if not lines:
                continue
            if remaining[key]:
                remaining[key] -= 1
                continue
            if old[key] and edit.old.ordered == edit.new.ordered:
                continue
            yield Finding(edit.path, lines[0], 'G3', f'Added {label}.',
                          C_LINKAGE_FIX if label == 'C linkage outside C code' else FORBIDDEN_FIX)


def stand_in_counts(source, path, index, base):
    """Discount only exact built-in void-pointer slots proved by a protected row."""
    kind = index.policy.kind(path)
    if kind == "header":
        claims = void_abi.header_candidates(source)
    else:
        claims = void_abi.candidates(source) if path.endswith(".cpp") else ()
    if not claims:
        return ratchets.counts(source)
    ledger = index.base_ledger(index.repo.ledger_oid(base))
    owned = ledger.unique_header_void_names if kind == "header" else ledger.void_abi_rows
    exempt = {offset for binding, spans in claims if binding in owned for offset in spans}
    return ratchets.counts(source, void_exempt=exempt)


def g4(ctx):
    """Report positive deltas of explicit stand-in patterns across touched files."""
    totals = Counter()
    added = defaultdict(list)
    for edit in ctx.edits.values():
        old, new = (stand_in_counts(source, edit.path, ctx.index, ctx.base)
                    for source in (edit.old, edit.new))
        lines = edit.added_lines()
        for kind in ratchets.RULES:
            change = len(new[kind]) - len(old[kind])
            totals[kind] += change
            if change > 0:
                hits = [n for n in new[kind] if n in lines] or new[kind]
                added[kind].extend((edit.path, n) for n in hits[:change])
    for kind in ratchets.RULES:
        delta = totals[kind]
        if delta > 0:
            for path, line in added[kind][:delta]:
                yield Finding(path, line, 'G4', f'{kind.replace("_", " ")} increased by {delta} across changed files.',
                              ratchets.FIXES[kind])


def comment_lines(source, policy):
    generated = {line for line, _ in annotations(source, policy)}
    for token in source.comments:
        body = token.value[2:-2] if token.kind == 'block' and token.value.endswith('*/') else token.value[2:]
        for offset, text in enumerate(body.split('\n')):
            if token.line + offset not in generated:
                yield min(token.line + offset, token.end_line), text.strip()


def g5(ctx):
    """Check newly introduced comment text, independent of code on the same line."""
    for edit in ctx.edits.values():
        if edit.path.startswith(ctx.policy.vendored):
            continue
        old = list(comment_lines(edit.old, ctx.policy))
        new = list(comment_lines(edit.new, ctx.policy))
        matcher = SequenceMatcher(None, [s for _, s in old], [s for _, s in new])
        for tag, _, _, start, stop in matcher.get_opcodes():
            if tag == 'equal':
                continue
            for line, text in new[start:stop]:
                for category, pattern in ctx.policy.comment.items():
                    # A trailing address next to a declaration is a concise
                    # evidence pointer. A free-standing row label just
                    # repeats the ownership metadata.
                    if category == 'bare address tag' and not (
                        edit.new.lines[line-1].lstrip().startswith(('//', '/*', '*'))
                    ):
                        continue
                    if pattern.search(text):
                        yield Finding(edit.path, line, 'G5', f'Added comment contains {category}.',
                                      'Explain the evidence or intent. Put reconstruction notes in the pull request description.')


def include_targets(path, spelling, quoted):
    spelling = spelling.replace('\\', '/')
    if quoted:
        yield posixpath.normpath(posixpath.join(posixpath.dirname(path), spelling))
    yield posixpath.normpath(posixpath.join('include', spelling))


def includes_path(path, spelling, quoted, candidates):
    return any(item in candidates for item in include_targets(path, spelling, quoted))


def primary_type(path, names):
    leaf_names = {name.rsplit('::', 1)[-1] for name in names}
    basename = PurePosixPath(path).stem
    if basename in leaf_names:
        return basename
    if len(leaf_names) == 1:
        return next(iter(leaf_names))
    return None


def promoted(ctx):
    removed, added = {}, {}
    for edit in ctx.edits.values():
        if edit.old_exists and not edit.new_exists and edit.path.startswith(DRAFT):
            removed[edit.path] = {d.name for d in definitions(edit.old) if d.kind == 'type'}
        if not edit.old_exists and edit.new_exists and ctx.policy.kind(edit.path) == 'header' and not edit.path.startswith(DRAFT):
            added[edit.path] = {d.name for d in definitions(edit.new) if d.kind == 'type'}
    pairs = {(old, new) for old, old_names in removed.items() for new, new_names in added.items()
             if old_names & new_names or posixpath.basename(old) == posixpath.basename(new)}
    # A rename can legitimately change both the header basename and the
    # class spelling. Require an explicit include switch and either an
    # unambiguous primary type per header or no type names in either.
    swapped = set()
    for edit in ctx.edits.values():
        old_paths = {h for _, p, q in include_sites(edit.old)
                     for h in include_targets(edit.path, p, q) if h in removed}
        new_paths = {h for _, p, q in include_sites(edit.new)
                     for h in include_targets(edit.path, p, q) if h in added}
        if len(old_paths) == len(new_paths) == 1:
            old, = old_paths
            new, = new_paths
            if ((not removed[old] and not added[new]) or
                (primary_type(old, removed[old]) and primary_type(new, added[new]))):
                swapped.add((old, new))
    for old, new in sorted(swapped):
        if (sum(a == old for a, _ in swapped) == 1
            and sum(b == new for _, b in swapped) == 1):
            pairs.add((old, new))
    names = {}
    for old, new in sorted(pairs):
        before = primary_type(old, removed[old])
        after = primary_type(new, added[new])
        if before and after and before != after:
            if before in names and names[before] != after:
                names[before] = None
            else:
                names[before] = after
    names = {before:after for before,after in names.items() if after}
    return {old for old, _ in pairs}, {new for _, new in pairs}, names


def semantic_items(source, path, headers=(), renames=None):
    """Retain literal and directive-boundary differences while ignoring comments."""
    omitted_lines = {line for line, target, quoted in include_sites(source)
                     if includes_path(path, target, quoted, headers)}
    starts = dict(source.directives())
    i = 0
    while i < len(source.tokens):
        token = source.tokens[i]
        if i in starts:
            end = starts[i]
            if token.line not in omitted_lines:
                directive = tuple(source.ordered[i:end])
                # Whitespace before the first opening parenthesis changes
                # whether a macro binds parameters or expands as an object.
                if directive[1:2] == ('define',) and directive[3:4] == ('(',):
                    directive += (source.tokens[i+2].stop == source.tokens[i+3].offset,)
                yield directive, token.line
            i = end
        else:
            yield renames.get(token.value, token.value) if renames else token.value, token.line
            i += 1


def source_signature(source, path, headers=(), renames=None):
    return [value for value, _ in semantic_items(source, path, headers, renames)]


def first_semantic_change(edit, old, new):
    matcher = SequenceMatcher(None, old, new)
    index = next((start for tag, _, _, start, _ in matcher.get_opcodes() if tag != 'equal'), 0)
    lines = [line for _, line in semantic_items(edit.new, edit.path)]
    return lines[min(index, len(lines)-1)] if lines else 1


def g6(ctx):
    """Bound source-file scope and recognize narrow Draft migrations."""
    old_headers, new_headers, names = promoted(ctx)
    modified = []
    for edit in ctx.edits.values():
        if ctx.policy.kind(edit.path) != 'source':
            continue
        old = source_signature(edit.old, edit.path)
        new = source_signature(edit.new, edit.path)
        if old == new:
            continue
        now_includer = any(includes_path(edit.path, p, q, new_headers)
                           for _, p, q in include_sites(edit.new))
        if now_includer and (
            source_signature(edit.old, edit.path, old_headers, names)
            == source_signature(edit.new, edit.path, new_headers)
        ):
            continue
        modified.append((edit, old, new))
    cap = ctx.policy.limits['source_files']
    for edit, old, new in modified[cap:]:
        yield Finding(edit.path, first_semantic_change(edit, old, new), 'G6',
                      f'{len(modified)} source files contain real changes beyond Draft include/use updates. The limit is {cap}.',
                      PROMOTION_FIX)
    deleted = {edit.path for edit in ctx.edits.values()
               if edit.old_exists and not edit.new_exists and edit.path.startswith(DRAFT)}
    if deleted:
        index = ctx.snapshot()
        for path, line in index.remaining_includers(ctx.deltas, deleted):
            yield Finding(path, line, 'G6', 'A literal include still points to a deleted Draft header.', PROMOTION_FIX)


@dataclass
class FileFacts:
    defs: tuple
    addresses: frozenset
    includes: tuple


def address_components(spelling, pattern):
    """Recognize exact names and whole components of compiler decorations."""
    if not isinstance(spelling, str):
        raise TypeError
    if pattern.fullmatch(spelling):
        yield spelling
    if spelling.startswith("?"):
        body = re.sub(r"^\?\?(?:[01]|_[7EGSR][0-4]?)|^\?{1,2}\$?", "", spelling, count=1)
        head = body.split("@@", 1)[0]
        yield from (item for item in head.split("@") if pattern.fullmatch(item))
        # Signature components occur after V/U/T/W type encodings.
        for item in re.findall(r"(?<!\w)[VUTW](\w+)@|[A-Z][VUTW]((?:_?G?Rva|_?rva|RVA)\w+)@", body):
            name = next(value for value in item if value)
            if pattern.fullmatch(name):
                yield name
    if spelling.startswith("_"):
        name = spelling[1:].split("@", 1)[0]
        if pattern.fullmatch(name):
            yield name

@dataclass(frozen=True)
class LedgerNames:
    accepted: frozenset[str]
    void_abi_rows: frozenset[tuple[int, str]]
    unique_header_void_names: frozenset[str]
    c_linkage: frozenset[str] = frozenset()


C_SYMBOL = re.compile(r'[_@]([A-Za-z_]\w*?)(?:@\d+)?')


def c_object(entry):
    """True for a ledger object compiled as C, supplied by a prebuilt library or imported."""
    paths = [str(entry.get('tu') or ''), str(entry.get('path') or '')]
    return any(p.lower().endswith('.c') or '.lib/' in p.lower() or p.startswith('import/') for p in paths)


class Index:
    """An object-backed index. Replay advances it by tree delta to reuse parsed blobs."""

    def __init__(self, repo, policy):
        self.repo, self.policy = repo, policy
        self.revision = None
        self.files = {}
        self.defs = defaultdict(dict)
        self.addresses = Counter()

    @lru_cache(maxsize=11000)
    def facts(self, oid, is_c):
        _, raw = self.repo.blob(oid, self.policy.limits['bytes_per_file'])
        if raw is None:
            return FileFacts((), frozenset(), ())
        try:
            source = Source(decode(raw))
        except UnicodeDecodeError:
            return FileFacts((), frozenset(), ())
        return FileFacts(
            tuple(definitions(source, is_c=is_c)),
            frozenset(t.value for t in source.tokens if t.kind == 'word' and self.policy.address.fullmatch(t.value)),
            tuple(include_sites(source)),
        )

    def change(self, path, entry):
        old = self.files.pop(path, None)
        if old:
            self.addresses.subtract(old.addresses)
            for d in old.defs:
                owners = self.defs.get((d.kind, d.name, d.internal))
                if owners is not None:
                    owners.pop(path, None)
        if entry and entry.regular and self.policy.kind(path):
            facts = self.facts(entry.oid, path.lower().endswith('.c'))
            self.files[path] = facts
            self.addresses.update(facts.addresses)
            defs = Counter((d.kind, d.name, d.internal) for d in facts.defs)
            for key, count in defs.items():
                self.defs[key][path] = count

    def move(self, revision):
        if self.revision == revision:
            return
        if self.revision is None:
            for path, entry in self.repo.tree(revision, 'src', 'include'):
                self.change(path, entry)
        else:
            for delta in self.repo.diff(self.revision, revision):
                self.change(delta.path, delta.new)
        self.revision = revision

    def remaining_includers(self, deltas, deleted):
        changed = {d.path: d.new for d in deltas}
        for path in sorted(self.files.keys() | changed.keys()):
            if path in changed:
                entry = changed[path]
                if not entry or not entry.regular or not self.policy.kind(path):
                    continue
                facts = self.facts(entry.oid, path.lower().endswith('.c'))
            else:
                facts = self.files[path]
            for line, spelling, quoted in facts.includes:
                if includes_path(path, spelling, quoted, deleted):
                    # Do not report a lower-priority deleted spelling if a same-name
                    # local header exists. Literal nonlocal paths are still checked.
                    targets = list(include_targets(path, spelling, quoted))
                    actual = next((t for t in targets if t in self.files or t in deleted), None)
                    if actual in deleted:
                        yield path, line

    @lru_cache(maxsize=24)
    def base_ledger(self, oid):
        _, raw = self.repo.read_blob(oid, 1 << 30)
        accepted, native_abi, c_linkage = set(), set(), set()
        try:
            ledger = json.loads(raw)
            c_objects = {o.get("object_id") for o in ledger.get("objects", ()) if isinstance(o, dict) and c_object(o)}
            for row in ledger["rows"]:
                in_c = row.get("object_id") in c_objects or row.get("role") == "import_thunk"
                for spelling in (row["name"], *row["accepted_names"]):
                    accepted.update(address_components(spelling, self.policy.address))
                    plain = C_SYMBOL.fullmatch(spelling) if in_c else None
                    if plain:
                        c_linkage.add(plain.group(1))
                    if (row.get("role") == "owned" and ("PAX" in spelling or "PBX" in spelling)
                            and void_abi.DECORATED.fullmatch(spelling)):
                        native_abi.add((int(row["rva"], 16), spelling))
        except (TypeError, ValueError, KeyError) as error:
            raise ObjectError("The trusted base ledger has an invalid name table.") from error
        addresses_per_name = Counter(spelling for _, spelling in native_abi)
        unique = frozenset(spelling for spelling, count in addresses_per_name.items() if count == 1)
        return LedgerNames(frozenset(accepted), frozenset(native_abi), unique, frozenset(c_linkage))

    def accepted(self, oid):
        return self.base_ledger(oid).accepted

    @lru_cache(maxsize=8)
    def accepted_symbols(self, oid):
        if oid is None:
            return frozenset()
        _, raw = self.repo.read_blob(oid, 128 << 20)
        try:
            symbols = json.loads(raw)["symbols"]
            if not isinstance(symbols, dict):
                raise TypeError
            accepted = set()
            for spelling, record in symbols.items():
                candidates = record["candidates"]
                if not isinstance(candidates, list):
                    raise TypeError
                if candidates:
                    accepted.update(address_components(spelling, self.policy.address))
            return frozenset(accepted)
        except (TypeError, ValueError, KeyError) as error:
            raise ObjectError("The trusted base symbol ledger has an invalid name table.") from error

def g7(ctx):
    """Refuse source-local type definitions and duplicate shared definitions."""
    for edit in ctx.edits.values():
        if ctx.policy.kind(edit.path) == 'source':
            for line, (category, name) in paired_surplus(list(type_sites(edit.old)), type_sites(edit.new)):
                spelling = f' {name}' if name else ''
                yield Finding(edit.path, line, 'G7', f'New {category}{spelling} definition is in a source file.',
                              'Define the type once in the appropriate shared header and include that header.')
    changes = defaultdict(dict)
    added = defaultdict(list)
    for edit in ctx.edits.values():
        is_c = edit.path.lower().endswith('.c')
        old, new = (tuple(definitions(source, is_c=is_c)) for source in (edit.old, edit.new))
        grouped = defaultdict(list)
        for d in new:
            grouped[(d.kind, d.name, d.internal)].append(d.line)
        old_counts = Counter((d.kind, d.name, d.internal) for d in old)
        lines = edit.added_lines()
        for key in old_counts.keys() | grouped.keys():
            count = len(grouped[key])
            changes[key][edit.path] = count
            if count > old_counts[key]:
                positions = [line for line in grouped[key] if line in lines] or grouped[key]
                added[key].extend((edit.path, line) for line in positions[-(count-old_counts[key]):])
    if not added:
        return
    index = ctx.snapshot()
    for key, positions in sorted(added.items()):
        kind, name, internal = key
        counts = index.defs.get(key, {}) | changes[key]
        for path, line in positions:
            others = sorted(p for p, count in counts.items() if count and p != path and not internal)
            if others or counts[path] > 1:
                other = others[0] if others else path
                remedy = 'Keep one definition and include its shared declaration.'
                if other.startswith(DRAFT) and not path.startswith(DRAFT):
                    remedy = f'Move all includers to the permanent header and delete {other} in this change.'
                yield Finding(path, line, 'G7', f'The {kind} {name} already has a definition in {other}.', remedy)


def g8(ctx):
    """Refuse newly introduced address-shaped names absent from the trusted ledger."""
    pattern = ctx.policy.address
    candidates = defaultdict(list)
    for edit in ctx.edits.values():
        old = {t.value for t in edit.old.tokens if t.kind == 'word' and pattern.fullmatch(t.value)}
        for token in edit.new.tokens:
            if token.kind == 'word' and pattern.fullmatch(token.value) and token.value not in old:
                if not candidates[token.value] or candidates[token.value][-1][0] != edit.path:
                    candidates[token.value].append((edit.path, token.line))
    if not candidates:
        return
    index = ctx.snapshot()
    new = {name: places for name, places in candidates.items() if not index.addresses[name]}
    if not new:
        return
    accepted = index.accepted(ctx.repo.ledger_oid(ctx.base))
    if not new.keys() <= accepted:
        oid = ctx.repo.ledger_oid(ctx.base, 'config/symbols.json', optional=True)
        accepted |= index.accepted_symbols(oid)
    for name, places in sorted(new.items()):
        if name not in accepted:
            for path, line in places:
                yield Finding(path, line, 'G8', f'New address-shaped identifier {name} is absent from the base ledger.',
                              'Use an evidenced name or seek an accepted ledger spelling from a maintainer.')


GATES = (g1, g2, g3, g4, g5, g6, g7, g8)

