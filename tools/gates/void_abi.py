"""Recognize exact x86 decorated signatures for explicit void-pointer types."""
from bisect import bisect_left
from collections import Counter
import re

from cpp import matching
from definitions import CLASS_HEAD, NAMESPACE


MARKER = re.compile(r"^//[ \t]*FUNCTION:[ \t]+BFME2[ \t]+0x([0-9a-fA-F]{8})[ \t]+.*?\bname=(\S+)")
DECORATED = re.compile(r"^\?([A-Za-z_]\w*(?:@[A-Za-z_]\w*)*)@@"
                       r"(Y[AEGI]|[CKS][AGI]|[AEIMQU][ABCD]E)(\S+)$", re.ASCII)
ID = re.compile(r"[A-Za-z_]\w*\Z", re.ASCII)
CONVENTIONS = {"__cdecl": "A", "__stdcall": "G", "__fastcall": "I", "__thiscall": "E"}
MODIFIERS = {"inline", "__inline", "static", "virtual", "extern"} | CONVENTIONS.keys()
PLAIN_TYPES = {
    ("void",): "X", ("signed", "char"): "C", ("char",): "D", ("unsigned", "char"): "E",
    ("short",): "F", ("short", "int"): "F", ("unsigned", "short"): "G",
    ("unsigned", "short", "int"): "G", ("int",): "H", ("signed", "int"): "H",
    ("unsigned",): "I", ("unsigned", "int"): "I", ("long",): "J", ("long", "int"): "J",
    ("unsigned", "long"): "K", ("unsigned", "long", "int"): "K", ("float",): "M",
    ("double",): "N", ("long", "double"): "O", ("bool",): "_N", ("__int64",): "_J",
    ("unsigned", "__int64"): "_K",
}
ELABORATED_TAGS = {
    "class": ("V",),
    "struct": ("U",),
    "union": ("T",),
    "enum": ("W4",),
}
TYPE_KEYWORDS = frozenset({
    "void", "signed", "unsigned", "char", "short", "int", "long", "float", "double",
    "bool", "__int64", "const", "volatile", "class", "struct", "union", "enum",
    "typename", "auto", "register", "mutable", *MODIFIERS,
})
MAX_VARIANTS = 32


def _parse_qualified_type(words):
    """Parse an optionally-elaborated qualified type name into (tags, innermost-first parts)."""
    if not words:
        return None
    if words[0] in ELABORATED_TAGS:
        tags = ELABORATED_TAGS[words[0]]
        qual = words[1:]
    else:
        tags = None
        qual = words
    if not qual or len(qual) % 2 == 0:
        return None
    parts = []
    for idx, piece in enumerate(qual):
        if idx % 2 == 0:
            if not ID.fullmatch(piece) or piece in TYPE_KEYWORDS:
                return None
            parts.append(piece)
        elif piece != "::":
            return None
    return tags, tuple(reversed(parts))


def _encode_qualified_name(parts, name_table):
    """Encode a qualified name using MSVC's 10-entry identifier back-reference table."""
    table = list(name_table)
    out = []
    for part in parts:
        if part in table:
            out.append(str(table.index(part)))
        else:
            out.append(part + "@")
            if len(table) < 10:
                table.append(part)
    out.append("@")
    return "".join(out), tuple(table)


def _encode_type_variants(tokens, name_table, *, allow_by_value_user=True):
    """Return (variants, void_offset) where each variant is (code, backref_eligible, next_name_table)."""
    values = tuple(token.value for token in tokens)
    if not values or "volatile" in values:
        return None
    if values in PLAIN_TYPES:
        return ((PLAIN_TYPES[values], False, name_table),), None
    if values[-1] in ("*", "&"):
        if values.count("*") + values.count("&") != 1:
            return None
        target = values[:-1]
        if not target or target.count("const") > 1:
            return None
        is_const = "const" in target
        prefix = ("PB" if is_const else "PA") if values[-1] == "*" else ("AB" if is_const else "AA")
        base = tuple(word for word in target if word != "const")
        if base in PLAIN_TYPES:
            code = PLAIN_TYPES[base]
            if values[-1] == "&" and code == "X":
                return None
            offset = tokens[values.index("void")].offset if code == "X" else None
            return ((prefix + code, True, name_table),), offset
        parsed = _parse_qualified_type(base)
        if parsed is None:
            return None
        tags, parts = parsed
        tags = tags or ("V", "U")
        encoded_qual, next_names = _encode_qualified_name(parts, name_table)
        return tuple((prefix + tag + encoded_qual, True, next_names) for tag in tags), None
    if "*" in values or "&" in values:
        return None
    if values.count("const") > 1:
        return None
    base = tuple(word for word in values if word != "const")
    if base in PLAIN_TYPES:
        return ((PLAIN_TYPES[base], False, name_table),), None
    if not allow_by_value_user:
        return None
    parsed = _parse_qualified_type(base)
    if parsed is None:
        return None
    tags, parts = parsed
    tags = tags or ("V", "U", "W4")
    encoded_qual, next_names = _encode_qualified_name(parts, name_table)
    return tuple((tag + encoded_qual, True, next_names) for tag in tags), None


def encode_type(tokens):
    """Encode a standalone primitive or void-pointer type (legacy helper)."""
    result = _encode_type_variants(tokens, (), allow_by_value_user=False)
    if result is None:
        return None
    variants, offset = result
    if len(variants) != 1:
        return None
    return variants[0][0], offset


def parameter_type_variants(tokens, initial_names):
    """Return (tuple_of_encoded_param_lists, void_offsets) with MSVC back-references."""
    if not tokens or tuple(token.value for token in tokens) == ("void",):
        return ("XZ",), ()
    pieces, start = [], 0
    for i, token in enumerate(tokens):
        if token.value == ",":
            pieces.append(tokens[start:i])
            start = i + 1
    pieces.append(tokens[start:])
    # States are (encoded_prefix, param_backref_table, name_backref_table).
    states = [("", (), tuple(initial_names))]
    offsets = []
    for part in pieces:
        if not part:
            return None
        probe = _encode_type_variants(part, states[0][2])
        stripped = part
        if probe is None and len(part) > 1 and ID.fullmatch(part[-1].value) and part[-1].value not in TYPE_KEYWORDS:
            stripped = part[:-1]
            probe = _encode_type_variants(stripped, states[0][2])
        if probe is None or any(code == "X" for code, _, _ in probe[0]):
            return None
        if probe[1] is not None:
            offsets.append(probe[1])
        next_states = []
        for prefix, param_refs, name_table in states:
            encoded = _encode_type_variants(stripped, name_table)
            if encoded is None:
                return None
            variants, _ = encoded
            for code, eligible, next_names in variants:
                if eligible:
                    if code in param_refs:
                        piece_code = str(param_refs.index(code))
                        next_refs = param_refs
                    else:
                        piece_code = code
                        next_refs = (*param_refs, code) if len(param_refs) < 10 else param_refs
                else:
                    piece_code = code
                    next_refs = param_refs
                next_states.append((prefix + piece_code, next_refs, next_names))
                if len(next_states) > MAX_VARIANTS:
                    return None
        states = next_states
    return tuple(dict.fromkeys(prefix + "@Z" for prefix, _, _ in states)), tuple(offsets)


def parameter_types(tokens):
    """Encode a parameter list without enclosing name context (legacy helper)."""
    result = parameter_type_variants(tokens, ())
    if result is None:
        return None
    variants, offsets = result
    if len(variants) != 1:
        return None
    return variants[0], offsets


def _signature_variants(return_tokens, param_tokens, initial_names):
    """Encode all (return_code + param_list_code) variants and collect void-pointer offsets."""
    ret = _encode_type_variants(return_tokens, tuple(initial_names))
    if ret is None:
        return None
    ret_variants, ret_offset = ret
    spellings = []
    param_offsets = None
    for ret_code, _, after_ret_names in ret_variants:
        params = parameter_type_variants(param_tokens, after_ret_names)
        if params is None:
            return None
        param_codes, offsets = params
        if param_offsets is None:
            param_offsets = offsets
        for param_code in param_codes:
            spellings.append(ret_code + param_code)
            if len(spellings) > MAX_VARIANTS:
                return None
    all_offsets = param_offsets if ret_offset is None else (ret_offset, *param_offsets)
    return tuple(dict.fromkeys(spellings)), all_offsets


def declaration(source, start, decorated):
    """Prove recognized signature slots from the entire directly annotated head."""
    values, tokens = source.ordered, source.tokens
    name_parts = tuple(decorated[1].split("@"))
    name = "::".join(reversed(name_parts))
    call, encoded = decorated.group(2, 3)
    left = start
    while left < len(values) and values[left] not in {"(", "#", ";", "{", "}"}:
        left += 1
    if left <= start or values[left:left + 1] != ("(",) or not ID.fullmatch(values[left - 1]):
        return ()
    name_start = left - 1
    while name_start >= start + 2 and values[name_start - 1] == "::" and ID.fullmatch(values[name_start - 2]):
        name_start -= 2
    if "".join(values[name_start:left]) != name:
        return ()
    prefixes = tokens[start:name_start]
    spelled_calls = [CONVENTIONS[token.value] for token in prefixes if token.value in CONVENTIONS]
    if spelled_calls and spelled_calls != [call[-1]]:
        return ()
    closing = matching(values, left)
    if closing is None:
        return ()
    return_tokens = tuple(token for token in prefixes if token.value not in MODIFIERS)
    initial_names = tuple(dict.fromkeys(name_parts))[:10]
    sigs = _signature_variants(return_tokens, tokens[left + 1:closing], initial_names)
    if sigs is None:
        return ()
    variants, offsets = sigs
    if encoded not in variants:
        return ()
    end = closing + 1
    while end < len(values) and values[end] in {"const", "volatile"}:
        end += 1
    suffix = values[closing + 1:end]
    if len(call) == 3:
        qualification = ("D" if "const" in suffix and "volatile" in suffix else
                         "B" if "const" in suffix else "C" if "volatile" in suffix else "A")
        if call[1] != qualification:
            return ()
    elif suffix:
        return ()
    if values[end:end + 1] != ("{",):
        return ()
    return offsets


def candidates(source):
    """Return row binding and exact void-token offsets for supported declarations."""
    if "void" not in source.ordered or not any(key in source.text for key in ("PAX", "PBX")):
        return ()
    offsets = [token.offset for token in source.tokens]
    entries = []
    for comment in source.comments:
        if comment.kind != "line" or comment.line != comment.end_line:
            continue
        annotation = MARKER.match(comment.value)
        if annotation is None or not any(key in annotation[2] for key in ("PAX", "PBX")):
            continue
        decorated = DECORATED.fullmatch(annotation[2])
        if decorated is None:
            continue
        start = bisect_left(offsets, comment.stop)
        locations = declaration(source, start, decorated)
        if locations:
            entries.append(((int(annotation[1], 16), annotation[2]), locations))
    return tuple(entries)

ACCESS = {"private": "A", "protected": "I", "public": "Q"}
QUALIFIERS = {frozenset(): "A", frozenset({"const"}): "B",
              frozenset({"volatile"}): "C", frozenset({"const", "volatile"}): "D"}


def header_tokens(source):
    """Accept direct declarations in unbranched headers, optionally guarded once."""
    directives = tuple(source.directives())
    if not directives:
        return source.tokens
    values = source.ordered
    body = [values[start + 1:end] for start, end in directives]
    if body[0] == ("pragma", "once"):
        checked, start = body[1:], 1
    else:
        checked, start = body, 0
    if len(checked) >= 3 and checked[0][:1] == ("ifndef",):
        guard = checked[0][1:]
        if (len(guard) != 1 or not ID.fullmatch(guard[0])
                or checked[1] != ("define", *guard) or checked[-1] != ("endif",)
                or directives[start][0] != (directives[start - 1][1] if start else 0)
                or directives[start][1] != directives[start + 1][0]
                or directives[-1][1] != len(values)):
            return ()
        checked = checked[2:-1]
    if any(line[:1] != ("include",) and line != ("pragma", "once") for line in checked):
        return ()
    hidden = {i for start, end in directives for i in range(start, end)}
    return tuple(token for i, token in enumerate(source.tokens) if i not in hidden)


def member_declaration(tokens, values, start, end, owner, access):
    """Encode a complete explicit x86 member prototype and recover its void slots."""
    head = values[start:end]
    if "void" not in head or "*" not in head or "(" not in head:
        return None
    left = start + head.index("(")
    right = matching(values, left)
    if left <= start or right is None or right >= end or not ID.fullmatch(values[left - 1]):
        return None
    prefixes = tokens[start:left - 1]
    words = [token.value for token in prefixes]
    convention = [word for word in words if word in CONVENTIONS]
    suffix = values[right + 1:end]
    if any(value not in {"const", "volatile"} for value in suffix) or len(suffix) != len(set(suffix)):
        return None
    storage = ("static" in words, "virtual" in words)
    if storage == (True, True) or "extern" in words:
        return None
    access_code = ACCESS[access]
    if storage[0]:
        if suffix or len(convention) > 1 or convention == ["__thiscall"]:
            return None
        call = chr(ord(access_code) + 2) + CONVENTIONS[convention[0] if convention else "__cdecl"]
    else:
        if convention not in ([], ["__thiscall"]):
            return None
        if storage[1]:
            access_code = chr(ord(access_code) + 4)
        call = access_code + QUALIFIERS[frozenset(suffix)] + "E"
    name_parts = (values[left - 1], *reversed(owner))
    initial_names = tuple(dict.fromkeys(name_parts))[:10]
    return_tokens = tuple(token for token in prefixes if token.value not in MODIFIERS)
    sigs = _signature_variants(return_tokens, tokens[left + 1:right], initial_names)
    if sigs is None:
        return None
    variants, offsets = sigs
    if not offsets:
        return None
    prefix = "?" + "@".join(name_parts) + "@@" + call
    spellings = tuple(prefix + sig for sig in variants if DECORATED.fullmatch(prefix + sig))
    return (spellings, offsets) if spellings else None


def header_candidates(source):
    """Identify declaration slots by lexical scope and access, never by comments."""
    if "void" not in source.ordered or "*" not in source.ordered:
        return ()
    tokens = header_tokens(source)
    values = tuple(token.value for token in tokens)
    candidates = []

    def walk(start, end, owner=(), access=None, depth=0):
        if depth >= 32:
            return
        i = start
        while i < end:
            value = values[i]
            if access is not None and i == start and value in ACCESS and values[i + 1:i + 2] == (":",):
                access, start, i = value, i + 2, i + 2
                continue
            if value == "{":
                head_end = i
                close = matching(values, i)
                if close is None or close >= end:
                    return
                head = " ".join(values[start:i])
                namespace = NAMESPACE.fullmatch(head) if access is None else None
                record = CLASS_HEAD.fullmatch(head) if values[start:i][:1] in (
                    ("class",), ("struct",), ("union",)
                ) else None
                if namespace and namespace[1]:
                    parts = tuple("".join(namespace[1].split()).split("::"))
                    walk(i + 1, close, (*owner, *parts), depth=depth + 1)
                elif (record and ID.fullmatch(record["name"]) and not record["args"]
                      and values[close + 1:close + 2] == (";",)):
                    default = "private" if record["kind"] == "class" else "public"
                    walk(i + 1, close, (*owner, record["name"]), default, depth=depth + 1)
                i = close + 1
                if namespace or record or "(" in values[start:head_end]:
                    start = i
                else:
                    # An initializer can extend after its closing brace.
                    continue
                continue
            if value in ("(", "["):
                close = matching(values, i)
                if close is None or close >= end:
                    return
                i = close
            elif value == ";":
                if access is not None:
                    candidate = member_declaration(tokens, values, start, i, owner, access)
                    if candidate is not None:
                        candidates.append(candidate)
                start = i + 1
            i += 1

    walk(0, len(values))
    if not candidates:
        return ()
    counts = Counter(spelling for spellings, _ in candidates for spelling in spellings)
    return tuple((spelling, offsets) for spellings, offsets in candidates
                 for spelling in spellings if counts[spelling] == 1)
