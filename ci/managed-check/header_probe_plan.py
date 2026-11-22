"""Recognize a conservative subset of changes and generate compile-only header probes."""

import re
from collections import defaultdict
from difflib import SequenceMatcher

from header_decl_check import PLAIN_NAME, Unobserved, canonical_header, sha256


COMMENTS = re.compile(r"/\*[\s\S]*?\*/|//[^\r\n]*|'(?:\\.|[^'\\])*'|\"(?:\\.|[^\"\\])*\"")
IDENT = r"[A-Za-z_]\w*"
QIDENT = r"(?:::)?[A-Za-z_]\w*(?:::[A-Za-z_]\w*)*"
TYPE = rf"{QIDENT}(?:[ \t]+{QIDENT})*(?:[ \t]*[*&][ \t]*(?:const\b|volatile\b)?)*"
DECL_SEPARATOR = r"(?:[ \t]+|(?<=[*&]))"
ARG = rf"{TYPE}(?:{DECL_SEPARATOR}{IDENT})?"
MOD = r"(?:(?:static|virtual|inline|explicit)\s+)*"
CALL = r"(?:__(?:cdecl|stdcall|fastcall|thiscall)\s+)?"
MEMBER = re.compile(
    rf"(?P<mod>{MOD})(?:(?P<ret>{TYPE}){DECL_SEPARATOR})?(?P<call>{CALL})(?P<name>~?{IDENT})\s*"
    rf"\((?P<args>[^()]*)\)(?P<qual>(?:\s+(?:const|volatile))*)\s*(?:=\s*0\s*)?;",
    re.ASCII,
)
ARRAY = r"\s*\[\s*(?:0[xX][0-9a-fA-F]+|[0-9]+|[()+-]|\s)*\s*\]"
FIELD = re.compile(rf"(?P<static>static\s+)?(?P<type>{TYPE}){DECL_SEPARATOR}(?P<name>{IDENT})(?P<array>{ARRAY})?;", re.ASCII)
FORWARD_TYPE = re.compile(rf"(?:class|struct|union)\s+{IDENT}\s*;",re.ASCII)
FRIEND_TYPE = re.compile(rf"friend\s+(?:class|struct|union)\s+{QIDENT}\s*;",re.ASCII)
ACCESS_SPEC = re.compile(r"(?P<access>public|protected|private)\s*:", re.ASCII)
INLINE_ACCESS_SPEC = re.compile(r"(?:public|protected|private)\s*:\s*\S", re.ASCII)
NON_TYPES = {"typedef", "using", "enum", "class", "struct", "union", "friend", "template",
             "extern", "return", "delete", "mutable", "namespace", "public", "private", "protected"}


def _comments(line_source):
    def clean(token):
        value = token.group()
        if value.startswith(("//", "/*")):
            return "".join(c if c in "\r\n" else " " for c in value)
        return value

    return COMMENTS.sub(clean, line_source)


def _profile_view(text, parser):
    normalized = _comments(text)
    if re.search(r"(?:\\|\?\?/)[ \t]*\r?\n|^\s*#\s*(?:line|include_next)\b", normalized, re.M):
        raise Unobserved("preprocessor splice or remapped source requires a complete declaration inventory")
    # Strip only a conventional full-file guard from the scope parser. Keep
    # the actual bytes in the emitted compile and in the source diff.
    directives = list(re.finditer(r"^[ \t]*#[ \t]*(\w+)([^\r\n]*)", normalized, re.M))
    controls = [m for m in directives if m[1] not in ("include", "pragma")]
    scope = text
    if controls:
        if len(controls) != 3 or [m[1] for m in controls] != ["ifndef", "define", "endif"]:
            raise Unobserved("conditional header needs compiler-context inventory")
        first, define, end = controls
        macro = first[2].strip()
        prefix = normalized[:first.start()]
        if (not re.fullmatch(IDENT, macro) or define[2].strip() != macro or end[2].strip()
                or normalized[define.end():end.start()].strip() == ""
                or normalized[first.end():define.start()].strip()
                or normalized[end.end():].strip()
                or re.sub(r"^[ \t]*#\s*pragma\s+once[ \t]*$", "", prefix, flags=re.M).strip()):
            raise Unobserved("guard does not cover the whole header")
        for guard in reversed(controls):
            scope = scope[:guard.start()] + " " * (guard.end() - guard.start()) + scope[guard.end():]
    identities = parser.class_identities(scope)
    if identities is None:
        raise Unobserved("qualified type inventory is ambiguous")
    offsets, where = [], 0
    for line in normalized.splitlines(keepends=True):
        offsets.append(where + len(line) - len(line.lstrip()))
        where += len(line)
    return [line.strip() for line in normalized.splitlines()], offsets, identities, parser._masked(text)


def _direct_class(pos, records, masked):
    for qualified, _, args, template, start, end, _ in reversed(records):
        if not start < pos < end:
            continue
        if args is not None or template or not PLAIN_NAME.fullmatch(qualified):
            raise Unobserved(f"{qualified}: template or nonordinary class context")
        opening = masked.find("{", start, pos)
        segment = masked[opening:pos] if opening >= 0 else ""
        # A direct member starts after a class/member/access boundary.
        if segment.count("{") - segment.count("}") == 1 and segment.rstrip().endswith(("{", "}", ";", ":")):
            return qualified
        break
    raise Unobserved("change is outside a proved direct class-member declaration")


def _select(line, index, view):
    _, offsets, records, masked = view
    if not line:
        return None
    if line.startswith("#"):
        raise Unobserved("changed preprocessor directive")
    if FORWARD_TYPE.fullmatch(line):
        return None  # a type forward declaration has no external COFF key
    owner = _direct_class(offsets[index], records, masked)
    if FRIEND_TYPE.fullmatch(line):
        return None  # friendship has no external COFF key
    if ACCESS_SPEC.fullmatch(line):
        return None  # access specifier itself has no COFF key; affected members are re-probed below
    function = MEMBER.fullmatch(line)
    if function:
        parts = function.groupdict()
        raw_type = parts["ret"]
        member = parts["name"]
        short = owner.rsplit("::", 1)[-1]
        if ((raw_type is None) != (member in (short, "~" + short))
                or set(re.findall(IDENT, raw_type or "")) & NON_TYPES
                or any(not re.fullmatch(ARG, arg.strip(), re.ASCII)
                       for arg in parts["args"].split(",") if parts["args"].strip())):
            raise Unobserved(f"{owner}::{member}: unsupported declarator")
        prefix = (raw_type + " ") if raw_type else ""
        qualified = f"{owner}::{member}"
        definition = f"{prefix}{parts['call']}{qualified}({parts['args']}){parts['qual']} {{ for (;;) {{}} }}"
        return {"qualified": qualified, "definition": definition, "line": index + 1, "kind": "function"}
    data = FIELD.fullmatch(line)
    if data and not set(re.findall(IDENT, data["type"])) & NON_TYPES:
        if not data["static"]:
            return {"qualified": f"{owner}::{data['name']}", "line": index + 1, "kind": "instance_data"}
        qualified = f"{owner}::{data['name']}"
        array = data["array"]
        definition = (f"{data['type']} {qualified}{array} = {{0}};" if array else
                      f"{data['type']} {qualified} = {{0}};")
        return {"qualified": qualified, "definition": definition,
                "line": index + 1, "kind": "static_data"}
    raise Unobserved(f"{owner}: declaration, access section or inline body needs a stronger inventoried probe")


def plan_existing_header(header, before, after, parser):
    """Plan direct existing-class changes. Every unsupported change is a named hold."""
    canonical_header(header)
    proof = {"header": header, "before_sha256": sha256(before.encode()),
             "after_sha256": sha256(after.encode())}
    try:
        old_view, new_view = _profile_view(before, parser), _profile_view(after, parser)
    except Unobserved as exc:
        return dict(proof, status="held", reason=str(exc))

    def _class_at_depth_one(pos, records, masked):
        for qualified, _name, args, template, start, end, key in reversed(records):
            if not start < pos < end:
                continue
            if args is not None or template or not PLAIN_NAME.fullmatch(qualified):
                return None, None
            opening = masked.find("{", start, pos)
            segment = masked[opening:pos] if opening >= 0 else ""
            if segment.count("{") - segment.count("}") == 1:
                return (qualified, start), ("public" if key == "struct" else "private")
            return None, None
        return None, None

    def _effective_access(view, watched_classes):
        lines, offsets, records, masked = view
        current = {}
        mapping = {}
        acc_issues = []
        for idx, text in enumerate(lines):
            if not text or text.startswith("}"):
                continue
            owner_key, default_acc = _class_at_depth_one(offsets[idx], records, masked)
            if owner_key is None or owner_key[0] not in watched_classes:
                continue
            if text.startswith("#"):
                acc_issues.append({"line": idx + 1,
                                   "reason": f"{owner_key[0]}: preprocessor directive inside class with changed access section needs complete inventory"})
                continue
            if INLINE_ACCESS_SPEC.match(text):
                acc_issues.append({"line": idx + 1,
                                   "reason": f"{owner_key[0]}: inline access label inside class with changed access section needs complete inventory"})
                continue
            acc_match = ACCESS_SPEC.fullmatch(text)
            if acc_match:
                current[owner_key] = acc_match.group("access")
                continue
            if FORWARD_TYPE.fullmatch(text):
                continue
            mapping[idx] = (owner_key[0], current.get(owner_key, default_acc))
        return mapping, acc_issues

    found = {"before": [], "after": []}
    issues = []
    old_lines, new_lines = old_view[0], new_view[0]
    opcodes = SequenceMatcher(None, old_lines, new_lines, autojunk=False).get_opcodes()
    watched_classes = set()
    for op, a, b, c, d in opcodes:
        if op == "equal":
            continue
        for side, lines, indexes, view in (("before", old_lines, range(a, b), old_view),
                                          ("after", new_lines, range(c, d), new_view)):
            for i in indexes:
                try:
                    if ACCESS_SPEC.fullmatch(lines[i]):
                        owner = _direct_class(view[1][i], view[2], view[3])
                        watched_classes.add(owner)
                    statement = _select(lines[i], i, view)
                    if statement:
                        found[side].append(statement)
                except Unobserved as exc:
                    issues.append({"side": side, "line": i + 1, "reason": str(exc)})
    if watched_classes and not issues:
        old_access, old_acc_issues = _effective_access(old_view, watched_classes)
        new_access, new_acc_issues = _effective_access(new_view, watched_classes)
        for item in old_acc_issues:
            issues.append({"side": "before", **item})
        for item in new_acc_issues:
            issues.append({"side": "after", **item})
        if not issues:
            for op, a, b, c, d in opcodes:
                if op != "equal":
                    continue
                for oi, ni in zip(range(a, b), range(c, d)):
                    if old_access.get(oi) != new_access.get(ni):
                        for side, lines, idx, view in (("before", old_lines, oi, old_view),
                                                       ("after", new_lines, ni, new_view)):
                            try:
                                statement = _select(lines[idx], idx, view)
                                if statement:
                                    found[side].append(statement)
                            except Unobserved as exc:
                                issues.append({"side": side, "line": idx + 1, "reason": str(exc)})
    if issues:
        return dict(proof, status="held", issues=issues)
    prior = defaultdict(list)
    for item in found["before"]:
        prior[item["qualified"]].append(item)
    current = defaultdict(list)
    for item in found["after"]:
        current[item["qualified"]].append(item)
    for item in found["after"]:
        name = item["qualified"]
        if item["kind"] == "instance_data":
            continue
        if name in prior and len(prior[name]) == len(current[name]) == 1:
            item["before"] = prior[name][0]
        elif name in prior:
            return dict(proof, status="held", reason=f"{name}: changed overloads need individual proof")
    return dict(proof, status="planned", **found)


def compile_source(header, declaration=None):
    """Keep the actual access and compiler settings. Generated routines are never run."""
    canonical_header(header)
    source = '#include "' + header.removeprefix("include/") + '"\n'
    if declaration is not None:
        source += declaration["definition"] + "\n"
    return source
