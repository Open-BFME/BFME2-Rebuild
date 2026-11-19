"""Named allowance for inline assembly (reviewed list: only maintainers
change it; agents request entries in team_findings.md). Everything else that is banned stays banned.

An entry allows `blocks` (default 1) `__asm` blocks in one file, inside one function, for one instruction the compiler has no form for.
Two header-only kinds name a construct instead (no evidence comment; the entry itself carries the evidence):
  kind "macro":           the one `#define NAME(...) BODY` line in that file whose body is exactly the entry's body;
  kind "inline-function": the one inline function NAME in that file (inline, __inline, __forceinline or WWINLINE) that
                          holds __asm, whose assembly is exactly the entry's `asm` list, in order: one `__asm X`
                          statement per line, or with "form": "block" one braced `__asm { ... }` block, one
                          instruction per line (EA's wwmath.h form). Other definitions of NAME without __asm (an
                          #else fallback) are allowed.
Two more kinds name one function in one translation unit (each entry carries its evidence; coordinator 2026-11-17,
PPMalloc import):
  kind "function-statement": inside the one definition of NAME in that file, exactly one __asm, on a line that is exactly
                          the entry's `statement` (upstream's form, e.g. `{ __asm int 3 }`), and no other __asm there;
  kind "pragma-pair":     the file's only two `#pragma optimize` lines are exactly the entry's `open` line, followed
                          (blank and comment lines between) by the head of the one definition of NAME, and its `close`
                          line, right after that definition's closing brace (blank lines between); a comment carrying
                          `BFME-RETAIL <the entry's RVA>` sits within the three lines above `open`.
The same text anywhere else (a .cpp, a second header, another function) stays banned. judge.py applies this to the
translation unit and to every repository file it includes.
The block must carry, within the three lines above it, the evidence comment
    // asm-evidence: <Qualified::Name> <0xRVA> <instruction>: <why no C++ form yields it>
with exactly the entry's function, RVA and instruction, and a reason of at least 20 characters; its instructions must
all be among the entry's mnemonics, at most max_instructions of them. A file may not hold more allowed blocks than it
has entries.
"""
import ast, re
from pathlib import Path
ALLOWLIST = Path(__file__).resolve().parent / 'asm_allowlist.py'
EVIDENCE = re.compile(r'//\s*asm-evidence:\s*(\S.*?)\s+(0x[0-9A-Fa-f]+)\s+([^:]+?)\s*:\s*(.+)$')   # function names with spaces

REPO_ENTRIES = []
# a hard ceiling only; the per-entry cap is the coordinator's approval row, enforced by the landing gate.
REPO_MAX_INSTRUCTIONS, REPO_MAX_BLOCKS = 64, 4

def repo_entry_ok(e):
    """a repository entry (config/asm_allowlist.json) is the plain kind or function-statement, inside one
    named function of one file, coordinator-approved, within its declared cap (the landing gate holds that cap to the
    coordinator's approval row) and under the hard ceiling of 64 instructions and 4 blocks."""
    if not isinstance(e, dict) or e.get('kind') not in (None, 'function-statement'):
        return False
    name = e.get('function') if e.get('kind') is None else e.get('name')
    if not all(isinstance(e.get(k), str) and e.get(k) for k in ('file', 'rva', 'approved')) or not isinstance(name, str) or not name:
        return False
    if not e['approved'].startswith('coordinator') or not re.fullmatch(r'0x[0-9A-Fa-f]+', e['rva']):
        return False
    if e.get('kind') is None:
        try:
            n, b = int(e.get('max_instructions', 4)), int(e.get('blocks', 1))
        except (TypeError, ValueError):
            return False
        if not (1 <= n <= REPO_MAX_INSTRUCTIONS and 1 <= b <= REPO_MAX_BLOCKS) or not isinstance(e.get('mnemonics'), list) or not e.get('instruction'):
            return False
    return True

def set_job(job):
    """Read the judged revision's config/asm_allowlist.json (a JSON list); missing or malformed allows nothing more."""
    global REPO_ENTRIES
    REPO_ENTRIES = []
    try:
        import json
        value = json.loads((Path(job) / 'config' / 'asm_allowlist.json').read_text())
        REPO_ENTRIES = [e for e in value if repo_entry_ok(e)] if isinstance(value, list) else []
    except (OSError, ValueError):
        REPO_ENTRIES = []

def entries():
    return checker_entries() + list(REPO_ENTRIES)

def checker_entries():
    """ENTRIES from the allowlist file, by literal evaluation only; a missing or malformed file allows nothing."""
    try:
        tree = ast.parse(ALLOWLIST.read_text())
        for node in tree.body:
            if isinstance(node, ast.Assign) and [t.id for t in node.targets if isinstance(t, ast.Name)] == ['ENTRIES']:
                value = ast.literal_eval(node.value)
                return [e for e in value if isinstance(e, dict)]
    except (OSError, ValueError, SyntaxError):
        pass
    return []

def _blocks(text):
    """(start, end, instructions) for each __asm occurrence: a braced block or the rest of its line."""
    out = []
    for m in re.finditer(r'__asm\b', text):
        rest = text[m.end():]
        brace = re.match(r'\s*\{', rest)
        if brace:
            close = rest.find('}', brace.end())
            if close < 0:
                out.append((m.start(), len(text), None)); continue
            body = rest[brace.end():close]; end = m.end() + close + 1
            instructions = [line.split(';')[0].strip() for line in body.replace('\r', '').split('\n')]
        else:
            nl = rest.find('\n'); end = m.end() + (nl if nl >= 0 else len(rest))
            instructions = [rest[:nl if nl >= 0 else len(rest)].split(';')[0].strip()]
        out.append((m.start(), end, [i for i in instructions if i]))
    return out

def _norm(s):
    return ' '.join(s.split())

def _strip_named(text, entry):
    """TEXT with ENTRY's macro or inline function asm blanked, or (text, problem)."""
    name = re.escape(entry['name'])
    if entry['kind'] == 'macro':
        lines = [m for m in re.finditer(r'^[ \t]*#[ \t]*define[ \t]+' + name + r'\b(\([^)]*\))?[ \t]*(.*)$', text, re.M)]
        if len(lines) != 1:
            return text, '%s: macro %s must be defined exactly once (found %d)' % (entry['file'], entry['name'], len(lines))
        m = lines[0]
        if _norm(m.group(2)) != _norm(entry['body']):
            return text, '%s: macro %s expands to something other than its allowlisted body' % (entry['file'], entry['name'])
        return text[:m.start()] + ' ' * (m.end() - m.start()) + text[m.end():], None
    spans = []
    for head in re.finditer(r'\b(?:inline|__inline|__forceinline|WWINLINE)\b[^;{}()]*\b' + name + r'\s*\([^)]*\)\s*\{', text):
        start = head.end(); depth, i = 1, start
        while i < len(text) and depth:
            depth += {'{': 1, '}': -1}.get(text[i], 0); i += 1
        spans.append((start, i))
    # Only the definition holding __asm is the allowed one; a definition without it (an #else fallback) is ordinary code.
    heads = [x for x in spans if '__asm' in text[x[0]:x[1] - 1]] if entry.get('form') == 'block' else spans
    if entry.get('form') == 'block' and not heads:
        return text, None                                # not defined (yet) with its block: nothing to allow
    if len(heads) != 1:
        return text, '%s: inline function %s must be defined exactly once (found %d)' % (entry['file'], entry['name'], len(heads))
    start, i = heads[0]
    body = text[start:i - 1]
    if entry.get('form') == 'block':
        blocks = _blocks(body)
        braced = len(blocks) == 1 and re.match(r'__asm\s*\{', body[blocks[0][0]:])
        if not braced or [_norm(x) for x in (blocks[0][2] or [])] != [_norm(a) for a in entry['asm']]:
            return text, '%s: inline function %s holds other assembly than its allowlisted block' % (entry['file'], entry['name'])
        a, b, _ = blocks[0]
        out = body[:a] + ' ' * (b - a) + body[b:]
        return text[:start] + out + text[i - 1:], None
    statements = [m for m in re.finditer(r'^[ \t]*__asm[ \t]+([^\n{}]+?)[ \t]*;?[ \t]*$', body, re.M)]
    if [_norm(m.group(1)) for m in statements] != [_norm(a) for a in entry['asm']] or len(re.findall(r'__asm\b', body)) != len(statements):
        return text, '%s: inline function %s holds other assembly than its allowlisted statements' % (entry['file'], entry['name'])
    out = body
    for m in reversed(statements):
        out = out[:m.start()] + ' ' * (m.end() - m.start()) + out[m.end():]
    return text[:start] + out + text[i - 1:], None

def _definitions(text, name):
    """(head start, body start, body end) of each definition of the function NAME (Class::member, optionally further
    qualified) in TEXT: a head `NAME(...)` followed by `{`, not a declaration or a call."""
    out = []
    for head in re.finditer(r'(?<![\w:])(?:\w+::)*' + re.escape(name) + r'\s*\([^;{}]*\)\s*(?:const\s*)?\{', text):
        start = head.end(); depth, i = 1, start
        while i < len(text) and depth:
            depth += {'{': 1, '}': -1}.get(text[i], 0); i += 1
        line_start = text.rfind('\n', 0, head.start()) + 1
        out.append((line_start, start, i))
    return out

def _blank(text, a, b):
    return text[:a] + ''.join(c if c == '\n' else ' ' for c in text[a:b]) + text[b:]

def _code(line):
    return _norm(line.split('//', 1)[0])

def _strip_statement(text, entry):
    """TEXT with ENTRY's one statement blanked inside its one function, or (text, problem)."""
    defs = [d for d in _definitions(text, entry['name']) if '__asm' in text[d[1]:d[2]]]
    if not defs:
        return text, None                                 # not written (yet): nothing to allow
    if len(defs) != 1:
        return text, '%s: %s must be defined exactly once with its statement (found %d)' % (entry['file'], entry['name'], len(defs))
    _h, start, end = defs[0]
    hits = [m.start() + start for m in re.finditer(r'__asm\b', text[start:end])]
    if len(hits) != 1:
        return text, '%s: %s holds %d __asm; its entry allows exactly one statement' % (entry['file'], entry['name'], len(hits))
    a = text.rfind('\n', 0, hits[0]) + 1; b = text.find('\n', hits[0]); b = len(text) if b < 0 else b
    if _code(text[a:b]) != _norm(entry['statement']) or '//' in text[a:b] and '__asm' in text[a:b].split('//', 1)[1]:
        return text, '%s: %s holds other assembly than its allowlisted statement %s' % (entry['file'], entry['name'], entry['statement'])
    return _blank(text, a, b), None

PRAGMA_OPTIMIZE = re.compile(r'^[ \t]*#[ \t]*pragma[ \t]+optimize\b.*$', re.M)

def _strip_pragma_pair(text, entry):
    """TEXT with ENTRY's pragma pair blanked, or (text, problem). Nothing else in the file may use #pragma optimize."""
    lines = list(PRAGMA_OPTIMIZE.finditer(text))
    if not lines:
        return text, None
    where = '%s: #pragma optimize is allowed only as %s ... %s around %s' % (entry['file'], entry['open'], entry['close'], entry['name'])
    defs = _definitions(text, entry['name'])
    if len(lines) != 2 or len(defs) != 1:
        return text, where + ' (found %d pragma lines, %d definitions)' % (len(lines), len(defs))
    opened, closed = lines
    head, _body, end = defs[0]
    if not opened.end() <= head < end <= closed.start():
        return text, where + ' (the pair must enclose that one definition and nothing else)'
    between_open = text[opened.end():head]
    between_close = text[end:closed.start()]
    if _norm(opened.group(0)) != _norm(entry['open']) or _norm(closed.group(0)) != _norm(entry['close']):
        return text, where + ' (other pragma text)'
    if any(l.strip() and not l.strip().startswith('//') for l in between_open.split('\n')) or between_close.strip():
        return text, where + ' (the pair must enclose that one definition and nothing else)'
    above = text[:opened.start()].split('\n')[-4:]
    if not any('//' in l and re.search(r'BFME-RETAIL\s+' + re.escape(entry['rva']) + r'\b', l, re.I) for l in above):
        return text, where + ' (a BFME-RETAIL %s comment must sit within the three lines above it)' % entry['rva']
    return _blank(_blank(text, closed.start(), closed.end()), opened.start(), opened.end()), None

def strip_allowed(text, relpath):
    """TEXT with the allowed asm blocks of RELPATH blanked, and the problems that kept others from being allowed."""
    mine = [e for e in entries() if e.get('file') == relpath]
    problems = []
    for e in [e for e in mine if e.get('kind') == 'pragma-pair']:
        text, problem = _strip_pragma_pair(text, e)
        if problem:
            problems.append(problem)
    mine = [e for e in mine if e.get('kind') != 'pragma-pair']
    if not mine or '__asm' not in text:
        return text, problems
    named = [e for e in mine if e.get('kind') in ('macro', 'inline-function', 'function-statement')]
    if named:
        for e in named:
            text, problem = (_strip_statement if e['kind'] == 'function-statement' else _strip_named)(text, e)
            if problem:
                problems.append(problem)
        mine = [e for e in mine if e not in named]
        if not mine or '__asm' not in text:
            return text, problems
        rest, more = strip_allowed_blocks(text, relpath, mine)
        return rest, problems + more
    rest, more = strip_allowed_blocks(text, relpath, mine)
    return rest, problems + more

def strip_allowed_blocks(text, relpath, mine):
    problems, used = [], {}
    blocks = _blocks(text)
    allowed_blocks = sum(int(e.get('blocks', 1)) for e in mine)
    if len(blocks) > allowed_blocks:
        problems.append('%s: %d inline assembly blocks but the allowlist allows %d' % (relpath, len(blocks), allowed_blocks))
    out = text
    for start, end, instructions in reversed(blocks):
        before = text[:start].split('\n')[-4:]
        evidence = [EVIDENCE.search(line) for line in before]
        evidence = [e for e in evidence if e]
        match = None
        for e in mine:
            if used.get(id(e), 0) >= int(e.get('blocks', 1)): continue
            for ev in evidence:
                fn, rva, instr, why = ev.groups()
                if fn == e['function'] and int(rva, 16) == int(e['rva'], 16) and instr.strip().lower() == e['instruction'].lower() and len(why.strip()) >= 20:
                    match = e
        if match is None:
            problems.append('%s: inline assembly without a matching asm-evidence comment and allowlist entry' % relpath); continue
        allowed = {m.lower() for m in match.get('mnemonics', [])}
        def mnemonics(instruction):
            tokens = instruction.replace(',', ' ').split()
            words = tokens[:1]
            if tokens and tokens[0].lower() in ('lock', 'rep', 'repe', 'repz', 'repne', 'repnz') and len(tokens) > 1:
                words = tokens[:2]
            return [w.lower() for w in words]
        if instructions is None or not instructions or len(instructions) > int(match.get('max_instructions', 4)) or \
                any(w not in allowed for i in instructions for w in mnemonics(i)):
            problems.append('%s: inline assembly for %s uses instructions outside its allowlist entry' % (relpath, match['function'])); continue
        used[id(match)] = used.get(id(match), 0) + 1
        out = out[:start] + ' ' * (end - start) + out[end:]
    return out, problems
