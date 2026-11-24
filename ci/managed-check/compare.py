"""Compare a protected base and validated source-only candidate on the sealed Boat image."""
from concurrent.futures import ThreadPoolExecutor
from contextlib import contextmanager
import hashlib
from time import monotonic
import os
from collections import Counter
from itertools import permutations
from pathlib import Path
import re
import struct
import sys
import tempfile
from types import SimpleNamespace

from common import PIN, digest, encoded, read, validate_binding, write
from intake import reconstruct
from gitrepo import Git
from pool_compare import TrustedPaths, projection
from pool_protocol import decode
import measure as canary_measure
from measure import audit, pin, setup_home
from edited_functions import edited
from diff_scope import diff_lines
from finished_index import Owners, RealHeaders
from gateway import projection as public_projection
from progress_delta import drafts_in_use, progress
from reporting import GATES, SYMBOL, from_landing
from emitted_data import record_pairs
from provider_state import witnesses


def native_commands(git, judge, revision, paths, tools):
    sources, hashes = projection(git, revision)
    portable = TrustedPaths(tools, hashes)
    result = []
    for path in paths:
        command = portable.plan(judge, path, decode(sources[path], 1024**2), hashes[path])
        command["contract"] = digest(encoded({"compiler": "vc71-sealed-v1", "policy": PIN, **{
            key: command[key] for key in ("flags", "env")}}))
        result.append(command)
    return result


def protected_judge(work, roots, baseline):
    os.environ.update(LANG="C", LC_ALL="C")
    judge = setup_home(work, roots, baseline)
    # The sealed compiler supplies SDK headers only. Reuse the native
    # inventory verifier against that header-only view.
    judge.build.DXSDK_DIR = work / "bfme2-runner/repo/vendor/dxsdk-aug2005"
    judge.build.DXSDK_MANIFEST = Path(__file__).with_name("dxsdk-includes.manifest")
    import stlport_overlay
    judge.stlport46_overlay = lambda: Path(roots["overlay"])
    stlport_overlay.TREE = Path(roots["stlport46"])
    original = judge.build.compiler_environment

    def compiler_environment(root, source=None):
        env = original(root, source)
        env["WINEPATH"] = ";".join(judge.build.wine_path(path) for path in
                                   (root / "Vc7/bin", root / "Common7/IDE", Path(roots["vcruntime"])))
        return env

    judge.build.compiler_environment = compiler_environment
    if judge.checker_hash() != PIN["checker"]:
        raise ValueError("The protected native engine identity differs from the approved engine.")
    return judge


LOCAL_LABEL = re.compile(r"\$L\d+\Z")
MAX_LOCAL_DEPTH = 8
SHARED_EH_NO_IDENTITY = "extra is reachable from kept code/data, an associative parent or a linker root; no retail identity"


def _verified_shared_eh_locations(outcome):
    """COFF (section, offset) pairs already proven by relocation_proof as ICF-folded shared EH code.

    When multiple matched functions in one TU share a single retail ICF-folded EH handler/unwind
    funclet, only one function's handler/funclet pairs with the single expected row; the other
    parents' handlers/funclets remain in rejected_extras with SHARED_EH_NO_IDENTITY even though
    relocation_proof already verified their exact bytes and relocations inside the MATCH-proven
    parent row. Fail closed: require status == MATCH and relocation_check.status == MATCH, plus
    both a binary-shared-EH-parent/content-shared-EH proof and a matching code proof for the
    same retail RVA.
    """
    locations = set()
    for row in outcome.get("rows") or ():
        if not isinstance(row, dict) or row.get("status") != "MATCH":
            continue
        rc = row.get("relocation_check")
        if not isinstance(rc, dict) or rc.get("status") != "MATCH":
            continue
        proofs = rc.get("proofs")
        if not isinstance(proofs, list):
            continue
        shared_rvas = {
            p["rva"] for p in proofs
            if isinstance(p, dict)
            and p.get("kind") in ("binary-shared-EH-parent", "content-shared-EH")
            and isinstance(p.get("rva"), str)
        }
        if not shared_rvas:
            continue
        for p in proofs:
            if (isinstance(p, dict) and p.get("kind") == "code" and p.get("rva") in shared_rvas
                    and type(p.get("compiled_section")) is int and type(p.get("compiled_offset")) is int):
                locations.add((p["compiled_section"], p["compiled_offset"]))
    return locations


def _is_verified_shared_eh_extra(row, proven_shared_eh, coff):
    if not proven_shared_eh or coff is None or row.get("why") != SHARED_EH_NO_IDENTITY:
        return False
    sym = row.get("symbol")
    if not isinstance(sym, str) or not (sym.startswith("__ehhandler$") or LOCAL_LABEL.fullmatch(sym)):
        return False
    try:
        idx = coff.index(sym)
        entry = coff.symbols[idx]
        return (entry["section"], entry["value"]) in proven_shared_eh
    except _COFF_ERRORS:
        return False


_COFF_ERRORS = (OSError, KeyError, TypeError, ValueError, AttributeError, IndexError, struct.error)


def _read_local_target(judge, obj, sym):
    try:
        return judge.read_body(obj, sym)
    except _COFF_ERRORS:
        coff_fn = getattr(judge, "coff", None)
        if not callable(coff_fn):
            return None
        try:
            coff = coff_fn(obj)
            idx = coff.index(sym)
            loc = coff.loc(idx)
            n = coff.extent(idx, code=False)
            body = coff.bytes(loc, n)
            relocs = [(o, k, coff.symbols[j]["name"]) for o, k, j in coff.relocs(loc, n)]
            return body, relocs
        except _COFF_ERRORS:
            return None


def _local_structural_digest(sym, judge, obj, visiting, seen_locals):
    if len(visiting) > MAX_LOCAL_DEPTH:
        return None
    if len(visiting) == 1:
        try:
            body, relocs = judge.read_body(obj, sym)
        except _COFF_ERRORS:
            return None
    else:
        loaded = _read_local_target(judge, obj, sym)
        if loaded is None:
            return None
        body, relocs = loaded
    if not isinstance(body, (bytes, bytearray)):
        return None
    ordered_relocs = sorted(
        ((int(off), int(rtype), target) for off, rtype, target in relocs),
        key=lambda item: (item[0], item[1], "" if isinstance(item[2], str) and LOCAL_LABEL.fullmatch(item[2]) else str(item[2])),
    )
    norm_relocs = []
    for off_i, rtype_i, target in ordered_relocs:
        addend = bytes(body[off_i:off_i + 4]).hex() if 0 <= off_i <= len(body) else ""
        if isinstance(target, str) and LOCAL_LABEL.fullmatch(target):
            if target in visiting:
                target_id = ["cycle", visiting.index(target)]
            elif target in seen_locals:
                target_id = ["local-ref", seen_locals[target]]
            else:
                cid = len(seen_locals)
                seen_locals[target] = cid
                target_digest = _local_structural_digest(target, judge, obj, visiting + (target,), seen_locals)
                if target_digest is None:
                    return None
                target_id = ["local", cid, target_digest]
        else:
            target_id = ["sym", str(target)]
        norm_relocs.append([off_i, rtype_i, addend, target_id])
    return digest(encoded({"len": len(body), "body": digest(bytes(body)), "relocs": norm_relocs}))


def local_label_fingerprint(row, judge=None, obj=None, *, targets_out=None):
    """Structural identity for compiler-generated MSVC local labels ($L<n>).

    MSVC numbers $L<n> labels from a per-unit counter that shifts when an included
    header changes line count. Hashing exact COFF body bytes (preserving relocation
    addends) and canonical local-target aliasing graphs keeps identical shifted
    labels equal while catching changed addends, distinct targets with identical
    bytes, or changed bodies. Returns None when readable COFF evidence is
    unavailable so callers fail closed to raw names.
    """
    if judge is None or obj is None or not Path(obj).is_file():
        return None
    seen_locals = {row["symbol"]: 0}
    struct_digest = _local_structural_digest(row["symbol"], judge, obj, (row["symbol"],), seen_locals)
    if struct_digest is None:
        return None
    if targets_out is not None:
        targets_out.update(seen_locals)
    return encoded({
        "kind": "local-label",
        "selection": int(row.get("comdat_selection", 0) or 0),
        "live": bool(row.get("referenced_by_kept")),
        "why": str(row.get("why") or ""),
        "struct": struct_digest,
    })


MAX_SHARED_COMPONENT = 6


def _canonical_component_digest(comp_items, judge, obj):
    """Graph-isomorphism-complete canonical digest for a connected component of local roots."""
    if len(comp_items) > MAX_SHARED_COMPONENT:
        return None
    best = None
    for order in permutations(comp_items):
        shared_seen = {row["symbol"]: idx for idx, (row, _s, _f, _t) in enumerate(order)}
        seq = []
        ok = True
        for row, _sym, _fp, _targets in order:
            sd = _local_structural_digest(row["symbol"], judge, obj, (row["symbol"],), shared_seen)
            if sd is None:
                ok = False
                break
            seq.append([
                int(row.get("comdat_selection", 0) or 0),
                bool(row.get("referenced_by_kept")),
                str(row.get("why") or ""),
                sd,
            ])
        if not ok:
            return None
        cand = encoded(seq)
        if best is None or cand < best:
            best = cand
    return digest(best) if best is not None else None


def unexpected_extra_facts(outcome, *, judge=None, obj=None):
    """Return (facts_set, full_symbol_map) for rejected extra code symbols."""
    facts = set()
    symbols = {}
    local_items = []
    proven_shared_eh = _verified_shared_eh_locations(outcome)
    coff = None
    if proven_shared_eh and judge is not None and obj is not None and Path(obj).is_file():
        coff_fn = getattr(judge, "coff", None)
        if callable(coff_fn):
            try:
                coff = coff_fn(obj)
            except _COFF_ERRORS:
                coff = None
    for row in outcome.get("rejected_extras") or ():
        if _is_verified_shared_eh_extra(row, proven_shared_eh, coff):
            continue
        sym = row["symbol"]
        targets = {}
        fp = (local_label_fingerprint(row, judge=judge, obj=obj, targets_out=targets)
              if isinstance(sym, str) and LOCAL_LABEL.fullmatch(sym) else None)
        if fp is not None:
            local_items.append((row, sym, fp, set(targets)))
        else:
            fact_hash = digest(encoded(sym))
            facts.add(("unexpected-code", fact_hash))
            if isinstance(sym, str) and SYMBOL.fullmatch(sym):
                symbols.setdefault(fact_hash, sym)

    # Partition normalized local roots into connected components sharing any local target.
    n = len(local_items)
    visited = [False] * n
    comp_fps = []
    for i in range(n):
        if visited[i]:
            continue
        comp_idx = []
        stack = [i]
        visited[i] = True
        while stack:
            cur = stack.pop()
            comp_idx.append(cur)
            cur_nodes = local_items[cur][3] | {local_items[cur][1]}
            for j in range(n):
                if not visited[j] and (cur_nodes & (local_items[j][3] | {local_items[j][1]})):
                    visited[j] = True
                    stack.append(j)
        comp = [local_items[k] for k in comp_idx]
        if len(comp) == 1:
            comp_fps.append((comp[0][2], comp[0][1]))
        else:
            cd = _canonical_component_digest(comp, judge, obj)
            if cd is None:
                # Too large or unreadable: fail closed to exact raw symbol names.
                for _row, sym, _fp, _targets in comp:
                    fact_hash = digest(encoded(sym))
                    facts.add(("unexpected-code", fact_hash))
                    if isinstance(sym, str) and SYMBOL.fullmatch(sym):
                        symbols.setdefault(fact_hash, sym)
            else:
                comp_fps.append((encoded(["local-component", cd]), comp[0][1]))

    local_counts = Counter()
    for fp, sym in sorted(comp_fps):
        occ = local_counts[fp]
        local_counts[fp] += 1
        fact_hash = digest(encoded(["local-label", digest(fp), occ]))
        facts.add(("unexpected-code", fact_hash))
        if isinstance(sym, str) and SYMBOL.fullmatch(sym):
            symbols.setdefault(fact_hash, sym)
    return facts, symbols


def native_blockers(outcome, *, judge=None, obj=None):
    """Keep stable failure identities, never native details or scratch paths."""
    if "blockers" in outcome:
        return outcome["blockers"]
    facts = {
        ("emitted-row", digest(encoded([int(row["rva"], 16), row["status"]])))
        for row in outcome["expected"] if row["status"] != "MATCH"
    }
    extra_facts, _ = unexpected_extra_facts(outcome, judge=judge, obj=obj)
    facts.update(extra_facts)
    for text in outcome["verification_blockers"]:
        if text == "compiled owned function order differs from retail RVA order":
            facts.add(("owned-order", digest(encoded(text))))
        elif text.startswith(("placeholder virtual in ", "unresolved draft ")):
            facts.add(("placeholder-virtual", digest(encoded(text))))
        else:
            receipt = next((message for message in (
                "no compiler input/object receipt", "source changed since compilation",
                "compiled object changed after its receipt", "header changed since compilation",
            ) if text.startswith(message)), None)
            if receipt:
                facts.add(("native-object", digest(encoded(receipt))))
            elif text.endswith("has ambiguous/missing COFF position"):
                facts.add(("native-object", digest(encoded(text))))
    return [list(fact) for fact in sorted(facts)]


def unexpected_symbol_pairs(outcome, *, judge=None, obj=None, baseline=None):
    """Return bounded, allowlisted [digest, symbol] pairs for rejected extra code symbols.

    When `baseline` is supplied, filter out facts already present on base before applying
    the 32-symbol cap so pre-existing base symbols never crowd out a newly introduced one.
    """
    if "unexpected_symbols" in outcome and isinstance(outcome["unexpected_symbols"], list):
        if all(isinstance(pair, list) and len(pair) == 2 for pair in outcome["unexpected_symbols"]):
            return outcome["unexpected_symbols"][:32]
    _, symbols = unexpected_extra_facts(outcome, judge=judge, obj=obj)
    if baseline is not None:
        base_hashes = {fact[1] for fact in native_blockers(baseline) if fact[0] == "unexpected-code"}
        symbols = {h: s for h, s in symbols.items() if h not in base_hashes}
    return [[h, s] for h, s in sorted(symbols.items())[:32]]


def emitted_row_pairs(outcome, *, baseline=None):
    """Return bounded [digest, rva, status, name] tuples for non-MATCH expected rows."""
    if "emitted_rows" in outcome and isinstance(outcome["emitted_rows"], list):
        if all(isinstance(p, list) and len(p) == 4 for p in outcome["emitted_rows"]):
            return outcome["emitted_rows"][:32]
    base_hashes = ({fact[1] for fact in native_blockers(baseline) if fact[0] == "emitted-row"}
                   if baseline is not None else set())
    pairs = []
    for row in outcome.get("expected") or ():
        if not isinstance(row, dict):
            continue
        status, rva = row.get("status"), row.get("rva")
        if not status or status == "MATCH" or not rva:
            continue
        try:
            fact_hash = digest(encoded([int(rva, 16), status]))
        except (TypeError, ValueError):
            continue
        if fact_hash in base_hashes:
            continue
        name = row.get("symbol") or row.get("name") or ""
        pairs.append([fact_hash, str(rva), str(status), str(name)[:120]])
    return sorted(pairs)[:32]


def attach_blockers(judge, side_dir, table, *, baseline=None, include_symbols=True):
    """Precompute structural blockers and safe symbol labels using compiled COFF objects."""
    for path, outcome in table.items():
        if "blockers" in outcome:
            continue
        obj = side_dir / "obj" / (path.replace("/", "_") + ".obj")
        target = obj if obj.is_file() else None
        outcome["blockers"] = native_blockers(outcome, judge=judge, obj=target)
        if include_symbols:
            base_outcome = baseline.get(path) if isinstance(baseline, dict) else None
            pairs = unexpected_symbol_pairs(outcome, judge=judge, obj=target, baseline=base_outcome)
            if pairs:
                outcome["unexpected_symbols"] = pairs
            epairs = emitted_row_pairs(outcome, baseline=base_outcome)
            if epairs:
                outcome["emitted_rows"] = epairs


def index(table):
    return {path: {"verified": item.get("object_verified", False),
                   "error": bool(item.get("error")),
                   "gates": set(item["gates"]),
                   "blockers": {tuple(fact) for fact in native_blockers(item)},
                   "unexpected_symbols": {pair[0]: pair[1] for pair in unexpected_symbol_pairs(item)},
                   "emitted_rows": {pair[0]: (pair[1], pair[2], pair[3]) for pair in emitted_row_pairs(item)},
                   "proven": {row["rva"] for row in item["rows"]
                              if row.get("credited", row["status"] == "MATCH") and row.get("proof") == "proven"},
                   "rows": {row["rva"]: (row["status"], row.get("pct", 100.0 if row["status"] == "MATCH" else 0.0))
                            for row in item["rows"]}}
            for path, item in table.items()}


def reduction(old, new):
    counts = dict.fromkeys(("gained", "lost_credit", "lost_proof", "lost_verified",
                            "new_compiler_errors", "new_source_gates"), 0)
    rules, hint = set(), []
    for path in old:
        prior, current = old[path], new[path]
        before = {address for address, row in prior["rows"].items() if row[0] == "MATCH"}
        after = {address for address, row in current["rows"].items() if row[0] == "MATCH"}
        counts["gained"] += len(after - before)
        counts["lost_credit"] += len(before - after)
        counts["lost_proof"] += len(prior["proven"] - current["proven"])
        counts["lost_verified"] += int(prior["verified"] and not current["verified"])
        counts["new_compiler_errors"] += int(not prior["error"] and current["error"])
        added = current["gates"] - prior["gates"]
        counts["new_source_gates"] += len(added)
        new_blockers = sorted(current["blockers"] - prior["blockers"])
        rules.update(kind for kind, _ in new_blockers)
        sym_map = current.get("unexpected_symbols") or {}
        emitted_by_hash = {
            digest(encoded([int(address, 16), status])): (address, status, pct)
            for address, (status, pct) in current["rows"].items()
            if status != "MATCH"
        }
        for kind, fact_hash in new_blockers:
            if kind == "unexpected-code" and fact_hash in sym_map:
                hint.append(f"{path}: unexpected emitted code symbol `{sym_map[fact_hash]}`.")
            elif kind == "emitted-row":
                row_info = emitted_by_hash.get(fact_hash)
                exp_info = (current.get("emitted_rows") or {}).get(fact_hash)
                if row_info is not None:
                    address, status, pct = row_info
                    previous = prior["rows"].get(address, ("MISSING", 0.0))[0]
                    hint.append(f"{path} at {address}: emitted function {status} ({pct:.1f}% vs retail; previously {previous}).")
                elif exp_info is not None:
                    address, status, name = exp_info
                    label = f" `{name}`" if name else ""
                    hint.append(f"{path} at {address}{label}: emitted function {status} (expected row mismatch vs retail).")
                else:
                    hint.append(f"{path}: an emitted function did not match retail.")
            elif kind == "owned-order":
                hint.append(f"{path}: compiled owned function order differs from retail RVA order.")
            elif kind == "placeholder-virtual":
                hint.append(f"{path}: placeholder virtual or unresolved draft type in compiled object.")
            elif kind == "native-object":
                hint.append(f"{path}: object verification blocker (missing/ambiguous COFF position or stale receipt).")
        for address, (status, _) in current["rows"].items():
            previous = prior["rows"].get(address, ("MISSING", 0.0))[0]
            if previous == status:
                continue
            if status in {"WRONG_REFERENCE", "MISNAMED"}:
                rules.add("wrong-reference" if status == "WRONG_REFERENCE" else "native-identity")
            elif previous == "MISSING" and status == "PARTIAL":
                rules.add("partial-body")
            else:
                continue
            hint.append(f"{path} at {address}: {previous} to {status}.")
        if prior["rows"].keys() - current["rows"].keys():
            rules.add("regression")
        for problem in sorted(added):
            reason = next((phrase for phrase in GATES if phrase in problem), None)
            if "annotations" in problem:
                rules.add("annotation")
            elif "banned construct" in problem:
                rules.add("source-construct")
            elif "assembly" in problem or " asm " in problem:
                rules.add("assembly")
            else:
                rules.add("native-source")
            if reason:
                hint.append(GATES[reason])
    for key, rule in (("lost_credit", "regression"), ("lost_proof", "reference-proof"),
                      ("lost_verified", "verified"), ("new_compiler_errors", "compile")):
        if counts[key]:
            rules.add(rule)
    return counts, rules, hint


def nearby(judge, body, first):
    lines = []
    for line in judge.disasm(body, 0):
        address, separator, text = line.strip().partition(":")
        if separator and re.fullmatch("[0-9a-f]+", address):
            lines.append({"offset": int(address, 16), "text": " ".join(text.split())})
    position = next((i - 1 for i, row in enumerate(lines) if row["offset"] > first), len(lines) - 1)
    start = max(0, min(position - 3, len(lines) - 8))
    return lines[start:start + 8]


def difference(judge, obj, row, allow_listing):
    symbol, size = row.get("symbol"), row.get("size")
    if not symbol or type(size) is not int or not 0 < size < 1024**2:
        return None
    try:
        compiled, relocs = judge.read_body(obj, symbol)
        if len(compiled) > size and all(byte == 0xCC for byte in compiled[size:]):
            compiled = compiled[:size]
        old = judge.build.read_target_bytes(int(row["rva"], 16), size)
        left = judge.masked(compiled, relocs, max(len(compiled), size))[:len(compiled)]
        right = judge.mask_like(old, left, relocs, size)
        first = next((i for i, pair in enumerate(zip(left, right)) if pair[0] != pair[1]),
                     min(len(left), len(right)) if len(left) != len(right) else None)
        result = {"first": first, "compiled_size": len(compiled), "retail_size": size}
        if allow_listing and first is not None:
            result["compiled_listing"] = nearby(judge, compiled, first)
        return result
    except (OSError, KeyError, TypeError, ValueError):
        return None


def make_report(git, judge, plan, tip, old, new, find_object, *, lookup_difference=None):
    from header_definitions import global_class_defs

    base = plan["base"]
    indexed_old, indexed_new = index(old), index(new)
    counts, rules, hints = reduction(indexed_old, indexed_new)
    owners = Owners(git, base)
    adapter = SimpleNamespace(ANNOT=judge.ANNOT, land_gate9=SimpleNamespace(diff_lines=diff_lines))
    scope = edited(git, adapter, base, tip, owners)
    state = {"new": indexed_new, "gains": counts["gained"], "jt": plan["request_paths"],
             "kind": "regression" if rules else "ok"}
    changed = set(plan["changed_paths"])
    prior = {"base_results": indexed_old, "results": new}
    def first_difference(path, row, _raw, own):
        if lookup_difference:
            return lookup_difference(path, row, own)
        obj = find_object(path)
        return difference(judge, obj, row, own) if obj else None

    report = from_landing(state, prior, changed, first_difference, owners, edited=scope)
    old_headers = RealHeaders(git, base, global_class_defs)
    new_headers = RealHeaders(git, tip, global_class_defs)

    def finished(address, after):
        tree = new_headers if after else old_headers
        source, name, _ = owners.functions[address]
        return tree.ready(source, name)

    report["progress"] = progress(indexed_old, indexed_new, owners.path, finished,
                                  drafts_in_use(git, base), drafts_in_use(git, tip))
    report["progress"]["lost"] = max(report["lost_count"], report["progress"]["lost"])
    report["codes"] = sorted(rules)
    clean = public_projection(report, changed)
    clean["messages"] = list(dict.fromkeys(hints))[:8]
    extra = [(path, item["emitted_data"]) for path, item in sorted(new.items()) if item.get("emitted_data")]
    if extra:
        clean["emitted_data"] = {"count": sum(item["count"] for _, item in extra),
                                 "rows": [{"path": path, **item} for path, item in extra[:8]]}
    functions = [{"path": row["path"], "line": row["line"], "function": row["function"],
                  "rva": row["rva"], "before": row["old"], "after": row["new"]}
                 for row in clean["edited"]]
    return clean, counts, functions


@contextmanager
def memoized_projection():
    """Cache source tree projections and packed payloads per revision for this run."""
    import base_cache as base_cache_mod
    built, packed = {}, {}
    orig_m_proj = canary_measure.projection
    orig_m_pack = canary_measure.pack_sources
    orig_b_proj = base_cache_mod.projection

    def cached_projection(git, revision):
        key = (str(git.path), revision)
        if key not in built:
            built[key] = orig_m_proj(git, revision)
        return built[key]

    def cached_pack(files):
        key = id(files)
        if key not in packed or packed[key][0] is not files:
            packed[key] = (files, orig_m_pack(files))
        return packed[key][1]

    canary_measure.projection = base_cache_mod.projection = cached_projection
    canary_measure.pack_sources = cached_pack
    try:
        yield
    finally:
        canary_measure.projection = orig_m_proj
        canary_measure.pack_sources = orig_m_pack
        base_cache_mod.projection = orig_b_proj


@contextmanager
def _with_asm_allowance(git, base):
    previous = canary_measure.CONFIG
    asm = "asm_allowlist.json"
    if git.run("cat-file", "-e", f"{base}:config/{asm}", check=False).returncode == 0:
        canary_measure.CONFIG = (*previous, asm)
    try:
        yield
    finally:
        canary_measure.CONFIG = previous


def measure_batch(git, judge, side, plan, tools, job, *, timing=None):
    """Preserve the optional assembly allowances from the protected base."""
    with _with_asm_allowance(git, plan["historical_base"]):
        cache_dir = os.environ.get("BFME_BASE_CACHE") if side == "base" else None
        if cache_dir:
            import base_cache
            results = base_cache.measure_base_with_cache(
                git, judge, plan, tools, job, cache_dir,
                measure_fn=canary_measure.measure, timing=timing,
            )
        else:
            results = canary_measure.measure(git, judge, side, plan, tools, job, timing=timing)
        resolve_extras(judge, plan, results)
        return results


def measure_pair(git, judge, plan, tools, base_job, head_job, **options):
    old, new = _measure_pair(git, judge, plan, tools, base_job, head_job, **options)
    for results in (old, new):
        resolve_extras(judge, plan, results)
    return old, new


def resolve_extras(judge, plan, results):
    if any(extra.get("select_any_candidate") for result in results.values()
           for extra in result.get("rejected_extras", ())):
        providers = witnesses(plan["select_any"], judge) if plan.get("select_any") else ()
        judge.resolve_batch_extras(results, providers)


def _measure_pair(git, judge, plan, tools, base_job, head_job, *,
                 cache_dir=None, base_timing=None, head_timing=None):
    """Compile uncached base and head sides concurrently across host vCPUs, then judge sequentially."""
    if cache_dir is None:
        cache_dir = os.environ.get("BFME_BASE_CACHE")
    with _with_asm_allowance(git, plan["historical_base"]):
        base_cache_state = None
        if cache_dir:
            import base_cache
            cache_root, base_sources, base_hashes, hit_payloads, missed_rows = base_cache.plan_base_cache(
                git, plan, tools, cache_dir,
            )
            base_cache_state = (base_cache, cache_root, base_sources, base_hashes, hit_payloads, missed_rows)
            base_plan = (
                dict(plan, request_paths=[r["tu"] for r in missed_rows],
                     compile=dict(plan["compile"], base=missed_rows))
                if missed_rows else None
            )
        else:
            base_plan = plan
        head_plan = plan
        base_count = len(base_plan["compile"]["base"]) if base_plan else 0
        head_count = len(head_plan["compile"]["head"])
        workers = canary_measure.compile_workers(base_count + head_count)

        if base_plan is None:
            base_cache_mod, cache_root, base_sources, base_hashes, hit_payloads, missed_rows = base_cache_state
            old = base_cache_mod.finish_base_cache(
                git, judge, plan, tools, base_job, cache_root, base_sources, base_hashes,
                hit_payloads, missed_rows, {}, timing=base_timing,
            )
            new = canary_measure.measure(git, judge, "head", head_plan, tools, head_job, timing=head_timing)
            return old, new

        if workers <= 1:
            # Concurrency cap of 1: base and head carry distinct source projections, so run one RPC per side serially.
            if base_cache_state is not None:
                base_cache_mod, cache_root, base_sources, base_hashes, hit_payloads, missed_rows = base_cache_state
                miss_timing = {} if base_timing is not None else None
                try:
                    miss_results = canary_measure.measure(git, judge, "base", base_plan, tools, base_job, timing=miss_timing)
                finally:
                    if base_timing is not None and miss_timing:
                        base_timing.update(miss_timing)
                old = base_cache_mod.finish_base_cache(
                    git, judge, plan, tools, base_job, cache_root, base_sources, base_hashes,
                    hit_payloads, missed_rows, miss_results, timing=base_timing,
                    compile_s=(miss_timing or {}).get("compile", 0.0),
                    compare_s=(miss_timing or {}).get("compare", 0.0),
                )
            else:
                old = canary_measure.measure(git, judge, "base", base_plan, tools, base_job, timing=base_timing)
            new = canary_measure.measure(git, judge, "head", head_plan, tools, head_job, timing=head_timing)
            return old, new

        b_sources, b_hashes, b_rows, b_packed = canary_measure.prepare_compile_side(git, "base", base_plan)
        h_sources, h_hashes, h_rows, h_packed = canary_measure.prepare_compile_side(git, "head", head_plan)
        profile = plan["profile_sha256"]

        def build_chunks(rows, max_chunks):
            count = max(1, min(len(rows), max_chunks))
            groups = [[(i, rows[i]) for i in range(slot, len(rows), count)] for slot in range(count)]
            return [g for g in groups if g]

        total_rows = len(b_rows) + len(h_rows)
        b_slots = max(1, min(len(b_rows), (workers * len(b_rows) + total_rows // 2) // total_rows))
        if b_slots >= workers:
            b_slots = workers - 1
        h_slots = max(1, min(len(h_rows), workers - b_slots))
        b_chunks = build_chunks(b_rows, b_slots)
        h_chunks = build_chunks(h_rows, h_slots)
        b_results = [None] * len(b_rows)
        h_results = [None] * len(h_rows)
        started = monotonic()
        try:
            with ThreadPoolExecutor(max_workers=workers) as pool:
                tasks = []
                for chunk in b_chunks:
                    fut = pool.submit(canary_measure.execute_compile_rpc, profile, b_packed, [r for _, r in chunk])
                    tasks.append(("base", chunk, fut))
                for chunk in h_chunks:
                    fut = pool.submit(canary_measure.execute_compile_rpc, profile, h_packed, [r for _, r in chunk])
                    tasks.append(("head", chunk, fut))
                for side_name, chunk, fut in tasks:
                    out_list = b_results if side_name == "base" else h_results
                    for (orig_idx, _), value in zip(chunk, fut.result(), strict=True):
                        out_list[orig_idx] = value
        finally:
            wall_compile = monotonic() - started
            if base_timing is not None:
                base_timing["compile"] = wall_compile

        if base_cache_state is not None:
            base_cache_mod, cache_root, base_sources, base_hashes, hit_payloads, missed_rows = base_cache_state
            miss_timing = {} if base_timing is not None else None
            miss_results = canary_measure.judge_compiled(
                git, judge, "base", base_plan, tools, base_job, b_sources, b_hashes, b_rows, b_results,
                timing=miss_timing, compile_seconds=wall_compile,
            )
            old = base_cache_mod.finish_base_cache(
                git, judge, plan, tools, base_job, cache_root, base_sources, base_hashes,
                hit_payloads, missed_rows, miss_results, timing=base_timing,
                compile_s=(miss_timing or {}).get("compile", 0.0),
                compare_s=(miss_timing or {}).get("compare", 0.0),
            )
        else:
            old = canary_measure.judge_compiled(
                git, judge, "base", base_plan, tools, base_job, b_sources, b_hashes, b_rows, b_results,
                timing=base_timing, compile_seconds=wall_compile,
            )
        new = canary_measure.judge_compiled(
            git, judge, "head", head_plan, tools, head_job, h_sources, h_hashes, h_rows, h_results,
            timing=head_timing, compile_seconds=0.0,
        )
        return old, new

def main(admission, out):
    admission, out = Path(admission), Path(out)
    plan_path = admission / "plan.json"
    plan, decision = validate_binding(read(plan_path, 18 * 1024**2)), read(admission / "decision.json", 4096)
    with plan_path.open("rb") as raw_plan:
        plan_hash = hashlib.file_digest(raw_plan, "sha256").hexdigest()
    if decision.get("plan") != plan_hash or any(decision.get(key) != plan[key] for key in ("repo", "base", "head", "tree", "number")):
        raise ValueError("The protected intake receipt and its plan differ.")
    if plan["base"] != os.environ["GITHUB_SHA"] or len(plan["request_paths"]) > 8:
        raise ValueError("This workflow requires the admitted protected revision and at most eight translation units.")
    if not plan["request_paths"] or sorted(set(plan["request_paths"])) != plan["request_paths"]:
        raise ValueError("The admitted translation-unit list is invalid.")
    baseline = (Path.cwd() / "baseline/game.dat").resolve(strict=True)
    pin(baseline, PIN["baseline_sha256"])
    roots = audit(PIN)
    os.umask(0o077)
    with tempfile.TemporaryDirectory(prefix="bfme-managed-check-") as name:
        work = Path(name)
        git = Git(work / "source.git", initialize=True)
        alternate = (Path.cwd() / ".git/objects").resolve(strict=True)
        (git.path / "objects/info/alternates").write_text(str(alternate) + "\n")
        if Git(Path.cwd() / ".git").rev("HEAD") != plan["base"]:
            raise ValueError("The trusted comparator did not receive the admitted main commit.")
        git.run("update-ref", "refs/heads/main", plan["base"])
        tip = reconstruct(git, plan)
        judge = protected_judge(work / "home", roots, baseline)
        judge.BARE = git.path
        native = dict(plan, historical_base=plan["base"], historical_head=tip,
                      compile={side: native_commands(git, judge, rev, plan["request_paths"], roots)
                               for side, rev in (("base", plan["base"]), ("head", tip))})
        with memoized_projection():
            old, new = measure_pair(git, judge, native, roots, work / "base", work / "head")
        attach_blockers(judge, work / "base", old, include_symbols=False)
        attach_blockers(judge, work / "head", new, baseline=old, include_symbols=True)
        objects = [(path, *(work / side / "obj" / (path.replace("/", "_") + ".obj")
                            for side in ("base", "head"))) for path in plan["request_paths"]]
        record_pairs(judge, work / "base", objects, new)
        report, counts, functions = make_report(git, judge, plan, tip, old, new,
            lambda path: work / "head/obj" / (path.replace("/", "_") + ".obj"))
        if advisory_root := os.environ.get("BFME_INLINE_EXPORT"):
            try:
                from inline_payload import narrow
                matrix_sha = narrow(plan, native, {"base": old, "head": new}, work, advisory_root)
                with Path(os.environ["GITHUB_OUTPUT"]).open("a") as stream:
                    stream.write("inline_matrix_sha=" + matrix_sha + "\n")
            except Exception:
                print("Inline virtual advisory evidence is unavailable for this run.", file=sys.stderr)
        if h5_root := os.environ.get("BFME_H5_EXPORT"):
            # Header layout evidence (H5), behind the header switch. A failure here never changes the comparison.
            try:
                sys.path.insert(0, str(Path(__file__).resolve().parents[1] / "header-witness"))
                from h5_bridge import from_compare
                if from_compare(plan, native, old, new, work, git, tip, roots, h5_root):
                    with Path(os.environ["GITHUB_OUTPUT"]).open("a") as stream:
                        stream.write("h5=yes\n")
            except Exception:
                print("Header layout evidence is unavailable for this run.", file=sys.stderr)
    out.mkdir(mode=0o700)
    write(out / "verdict.json", {key: plan[key] for key in ("version", "repo", "number", "base", "head", "tree")}
          | {"plan": plan_hash, "report": report, "counts": counts, "functions": functions})


if __name__ == "__main__":
    main(*sys.argv[1:])

