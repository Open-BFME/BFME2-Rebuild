"""Produce and strictly load protected H4/H6 header inventories."""
import re

from cpp import Source
from header_checks import TrustedObservations
from header_inline import InlineBody
from header_inline_scan import changed_discovery, scan as scan_inline_bodies
from header_inventory import (
    Binding, FileIdentity, VerifiedInventory, changed_headers, valid_binding,
)
from header_lexical_inventory import compare_inventories
from header_removal import Declaration

DOC_KEYS = frozenset({"version", "declarations", "bodies"})
INV_KEYS = frozenset({"rule", "format_revision", "complete", "files", "before", "after"})
FILE_KEYS = frozenset({"path", "before_blob", "after_blob"})
DECL_KEYS = frozenset({"path", "line", "qualified", "identity"})
BODY_KEYS = frozenset({
    "path", "line", "qualified", "signature", "body",
    "fields", "parameters", "direct_bases", "initializers",
})
IDENT = re.compile(r"[A-Za-z_]\w*\Z", re.ASCII)
CONDITIONAL_KW = frozenset({"if", "ifdef", "ifndef", "elif", "else", "endif"})
ENV_KW = frozenset({"include", "define", "undef"})


def _ser_file(ident):
    return {"path": ident.path, "before_blob": ident.before_blob, "after_blob": ident.after_blob}


def _ser_decl(item):
    return {"path": item.path, "line": item.line, "qualified": item.qualified, "identity": item.identity}


def _ser_body(item):
    return {
        "path": item.path,
        "line": item.line,
        "qualified": item.qualified,
        "signature": item.signature,
        "body": item.body,
        "fields": list(item.fields),
        "parameters": list(item.parameters),
        "direct_bases": list(item.direct_bases),
        "initializers": item.initializers,
    }


def _directive_profile(text, decl_lines):
    """Return (guard_macro, ordered_env_directives) if header has no body conditionals or unsafe directives, else None."""
    source = Source(text)
    raw_dirs = []
    for start, end in source.directives():
        toks = source.tokens[start:end]
        if len(toks) < 2 or toks[0].value != "#":
            return None
        kw = toks[1].value
        vals = tuple(t.value for t in toks)
        spaces = tuple(l.stop != r.offset for l, r in zip(toks, toks[1:]))
        raw_dirs.append((start, end, toks[0].line, kw, vals, spaces))

    # Strip at most one conventional full-file #ifndef GUARD / #define GUARD ... #endif guard.
    conds = [d for d in raw_dirs if d[3] in CONDITIONAL_KW]
    guard_macro = None
    guard_indices = set()
    if conds:
        if len(conds) != 2 or conds[0][3] != "ifndef" or conds[1][3] != "endif":
            return None
        first_c, last_c = conds[0], conds[1]
        if len(first_c[4]) != 3 or not IDENT.fullmatch(first_c[4][2]) or len(last_c[4]) != 2:
            return None
        macro = first_c[4][2]
        # Find the matching #define GUARD immediately after #ifndef GUARD (no non-directive tokens between).
        idx_first = raw_dirs.index(first_c)
        idx_last = raw_dirs.index(last_c)
        if idx_first + 1 >= idx_last:
            return None
        def_d = raw_dirs[idx_first + 1]
        if def_d[3] != "define" or len(def_d[4]) != 3 or def_d[4][2] != macro:
            return None
        # Ensure nothing outside the guard (except #pragma once before #ifndef) and nothing between #ifndef and #define.
        before_tokens = source.tokens[:first_c[0]]
        for idx_prev in range(idx_first):
            d = raw_dirs[idx_prev]
            if d[3] != "pragma" or d[4] != ("#", "pragma", "once"):
                return None
        allowed_before = sum(raw_dirs[i][1] - raw_dirs[i][0] for i in range(idx_first))
        if len(before_tokens) != allowed_before:
            return None
        if first_c[1] != def_d[0]:
            return None
        if last_c[1] != len(source.tokens):
            return None
        guard_macro = macro
        guard_indices = {idx_first, idx_first + 1, idx_last}

    env_dirs = []
    for idx, (_start, _end, line, kw, vals, spaces) in enumerate(raw_dirs):
        if idx in guard_indices:
            continue
        if kw in CONDITIONAL_KW:
            return None
        if kw == "pragma":
            if vals == ("#", "pragma", "once"):
                continue
            return None
        if kw not in ENV_KW:
            return None
        decls_before = sum(1 for dl in decl_lines if dl < line)
        env_dirs.append((kw, vals, spaces, decls_before))
    return guard_macro, tuple(env_dirs)


def _preprocessor_safe(before_text, after_text, before_decl_lines, after_decl_lines):
    p_before = _directive_profile(before_text, before_decl_lines)
    p_after = _directive_profile(after_text, after_decl_lines)
    return p_before is not None and p_after is not None and p_before == p_after


def produce(repo, policy, binding):
    """Build deterministic H4 declaration and H6 inline-body inventories from bound Git objects."""
    if not valid_binding(binding):
        raise ValueError("Invalid binding for trusted header inventory.")
    files = changed_headers(repo, binding, policy)
    file_rows = [_ser_file(f.identity) for f in files]

    before_d, after_d, unclassified = compare_inventories(files)
    before_lines_by_path = {}
    for item in before_d:
        before_lines_by_path.setdefault(item.path, []).append(item.line)
    after_lines_by_path = {}
    for item in after_d:
        after_lines_by_path.setdefault(item.path, []).append(item.line)

    pp_safe = True
    for item in files:
        if item.identity.before_blob is None or item.identity.after_blob is None:
            pp_safe = False
            break
        if not _preprocessor_safe(
            item.before, item.after,
            before_lines_by_path.get(item.identity.path, ()),
            after_lines_by_path.get(item.identity.path, ()),
        ):
            pp_safe = False
            break

    decl_doc = {
        "rule": "H4",
        "format_revision": 1,
        "complete": pp_safe and not unclassified,
        "files": file_rows,
        "before": [_ser_decl(item) for item in before_d],
        "after": [_ser_decl(item) for item in after_d],
    }

    before_b, after_b = [], []
    bodies_complete = pp_safe
    for item in files:
        if item.identity.before_blob is not None and item.identity.after_blob is not None:
            pb, ab, unresolved_changed = changed_discovery(item.identity.path, item.before, item.after)
            before_b.extend(pb)
            after_b.extend(ab)
            if unresolved_changed:
                bodies_complete = False
        else:
            if item.identity.before_blob is not None:
                obs = scan_inline_bodies(item.identity.path, item.before)
                before_b.extend(obs.bodies)
                if obs.unresolved:
                    bodies_complete = False
            if item.identity.after_blob is not None:
                obs = scan_inline_bodies(item.identity.path, item.after)
                after_b.extend(obs.bodies)
                if obs.unresolved:
                    bodies_complete = False

    body_doc = {
        "rule": "H6",
        "format_revision": 1,
        "complete": bodies_complete,
        "files": file_rows,
        "before": [_ser_body(item) for item in before_b],
        "after": [_ser_body(item) for item in after_b],
    }
    return {"version": 1, "declarations": decl_doc, "bodies": body_doc}


def _str(value, *, allow_empty=False):
    if not isinstance(value, str) or (not allow_empty and not value):
        raise ValueError("Expected a string field in trusted header observations.")
    return value


def _opt_str(value):
    if value is not None and (not isinstance(value, str) or not value):
        raise ValueError("Expected an optional string field in trusted header observations.")
    return value


def _int_pos(value):
    if type(value) is not int or value < 1:
        raise ValueError("Expected a positive integer in trusted header observations.")
    return value


def _str_tuple(value):
    if not isinstance(value, list) or not all(isinstance(v, str) for v in value):
        raise ValueError("Expected a list of strings in trusted header observations.")
    return tuple(value)


def _load_files(raw_files):
    if not isinstance(raw_files, list):
        raise ValueError("Expected a file identity list.")
    out = []
    for item in raw_files:
        if not isinstance(item, dict) or set(item) != FILE_KEYS:
            raise ValueError("Unexpected file identity schema.")
        out.append(FileIdentity(
            _str(item["path"]),
            _opt_str(item["before_blob"]),
            _opt_str(item["after_blob"]),
        ))
    return tuple(out)


def _load_decl(item):
    if not isinstance(item, dict) or set(item) != DECL_KEYS:
        raise ValueError("Unexpected H4 declaration schema.")
    return Declaration(
        _str(item["path"]),
        _int_pos(item["line"]),
        _str(item["qualified"]),
        _str(item["identity"]),
    )


def _load_body(item):
    if not isinstance(item, dict) or set(item) != BODY_KEYS:
        raise ValueError("Unexpected H6 inline-body schema.")
    return InlineBody(
        path=_str(item["path"]),
        line=_int_pos(item["line"]),
        qualified=_str(item["qualified"]),
        signature=_str(item["signature"]),
        body=_str(item["body"]),
        fields=_str_tuple(item["fields"]),
        parameters=_str_tuple(item["parameters"]),
        direct_bases=_str_tuple(item["direct_bases"]),
        initializers=_str(item["initializers"], allow_empty=True),
    )


def _load_inventory(raw, expected_rule, binding, item_loader):
    if not isinstance(raw, dict) or set(raw) != INV_KEYS:
        raise ValueError(f"Unexpected {expected_rule} inventory schema.")
    if raw["rule"] != expected_rule or type(raw["format_revision"]) is not int or raw["format_revision"] != 1:
        raise ValueError(f"Invalid {expected_rule} rule or format revision.")
    if type(raw["complete"]) is not bool:
        raise ValueError(f"Invalid {expected_rule} completeness flag.")
    if not isinstance(raw["before"], list) or not isinstance(raw["after"], list):
        raise ValueError(f"Expected before/after item lists for {expected_rule}.")
    return VerifiedInventory(
        binding=binding,
        rule=expected_rule,
        files=_load_files(raw["files"]),
        before=tuple(item_loader(x) for x in raw["before"]),
        after=tuple(item_loader(x) for x in raw["after"]),
        format_revision=1,
        complete=raw["complete"],
    )


def load(doc, binding):
    """Strictly validate serialized H4/H6 observations and bind them to the caller's Binding."""
    if not valid_binding(binding):
        raise ValueError("Caller supplied an invalid Binding.")
    if not isinstance(doc, dict) or set(doc) != DOC_KEYS or type(doc["version"]) is not int or doc["version"] != 1:
        raise ValueError("Unexpected trusted header observations envelope.")
    declarations = _load_inventory(doc["declarations"], "H4", binding, _load_decl)
    bodies = _load_inventory(doc["bodies"], "H6", binding, _load_body)
    return TrustedObservations(binding=binding, declarations=declarations, bodies=bodies)
