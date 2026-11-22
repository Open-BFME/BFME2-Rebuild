"""Serialize source-only header results for the protected hosted collector."""
from collections import Counter
from dataclasses import asdict

from header_checks import check_header_source
from header_inventory import changed_headers
from header_lexical_inventory import compare_inventories
from header_removal import acknowledgments


def declaration_review(repo, binding, files):
    """List apparent source differences as leads for the declaration comparison."""
    before, after, unclassified = compare_inventories(files)
    accepted = acknowledgments(repo, binding.head) if before else ()
    remaining = Counter((row.qualified, row.identity) for row in after)
    candidates = []
    for row in sorted(before):
        key = (row.qualified, row.identity)
        if remaining[key]:
            remaining[key] -= 1
        elif row.qualified not in accepted:
            candidates.append({"path": row.path, "line": row.line, "declaration": row.qualified})
    return {
        "candidates": candidates,
        "unclassified": [{"path": path, "line": line}
                         for path, line in sorted({(item.path, item.line) for item in unclassified})],
    }


def source_preview(repo, policy, binding, trusted=None):
    """Return deterministic source results with declaration leads kept outside them."""
    components = check_header_source(repo, policy, binding, trusted=trusted)
    return {
        "preview": [asdict(item) for item in components],
        "declaration_review": declaration_review(repo, binding, changed_headers(repo, binding, policy)),
        "trusted": trusted is not None,
    }
