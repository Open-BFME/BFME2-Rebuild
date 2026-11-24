"""Build a source-only delta against the protected revision or an actionable refusal."""
from collections import defaultdict, deque
import os
from pathlib import Path, PurePosixPath
import re
import sys

from common import PIN, REPO_ID, digest, policy, read, validate_binding, write
from gitrepo import Change, Git, Refusal, safe_path
from fork_admission import narrow, wide_eligible
from pool_compare import projection
from pool_protocol import DIRECTIVE, INCLUDE, compiler_source_problem, decode, encode
from cpp import SPLICE, TOKEN, TRIGRAPH

MAX_AFFECTED = 512
INCLUDE_HINT = re.compile(r"\binclude\b[^\n]*")
INCLUDE_DIGRAPH = re.compile(r"^([ \t]*)%:")


class Refused(Exception):
    def __init__(self, rule, location=None):
        super().__init__(rule)
        self.rule = rule
        self.location = location


def blob(git, rev, path):
    return git.run("cat-file", "blob", f"{rev}:{path}").stdout


def tu(path):
    return path.startswith("src/") and PurePosixPath(path).suffix.lower() in {".c", ".cpp"}


def literal_includes(content):
    raw = decode(content, 1024**2)
    if b"#" not in raw and b"??=" not in raw and b"%:" not in raw:
        return ()
    text = raw.decode("utf-8-sig", "replace").replace("\r\n", "\n").replace("\r", "\n")
    text = re.sub(r"\?\?([=/'()!<>-])", lambda match: TRIGRAPH[match[1]], text) if "??" in text else text
    text = SPLICE.sub("", text)
    # Find a conservative upper bound on logical directives. A block comment
    # can span physical lines before or after the include word.
    last = 0
    for hint in INCLUDE_HINT.finditer(text):
        last = hint.end()
    parts, previous, position = [], 0, 0
    while position < last and (match := TOKEN.search(text, position)):
        start, end = match.span()
        if start >= last:
            break
        if match.lastgroup == "raw":
            # VC7.1 reads this as a word followed by an ordinary literal.
            end = text.index('"', start, end)
        if match.lastgroup in {"line", "block"}:
            parts.extend((text[previous:start], " "))
            previous = end
            if end > last:
                newline = text.find("\n", end)
                last = len(text) if newline < 0 else newline
        position = end
    parts.append(text[previous:last])
    names = []
    for line in "".join(parts).split("\n"):
        directive = DIRECTIVE.fullmatch(INCLUDE_DIGRAPH.sub(r"\1#", line, count=1))
        if directive and directive[1] == "include":
            operand = INCLUDE.fullmatch(directive[2])
            if operand:
                names.append((operand[1] or operand[2]).replace("\\", "/").casefold())
    return tuple(names)


def affected(files, changed, *, include_cache=None):
    """Follow reverse include edges on either exact source tree."""
    if include_cache is None:
        include_cache = {}
    aliases = defaultdict(set)
    includers = defaultdict(set)
    for path in files:
        parts = path.casefold().split("/")
        for start in range(len(parts)):
            aliases["/".join(parts[start:])].add(path)
    for path, content in files.items():
        if content not in include_cache:
            include_cache[content] = literal_includes(content)
        for name in include_cache[content]:
            pieces = name.split("/")
            label = "/".join(part for part in pieces if part not in {"", ".", ".."})
            targets = aliases[label]
            if ".." in pieces:
                targets = targets | aliases[pieces[-1]]
            for target in targets:
                includers[target].add(path)
    seen, todo = set(changed), deque(changed)
    while todo:
        for includer in includers[todo.popleft()] - seen:
            seen.add(includer)
            todo.append(includer)
    return {path for path in seen if tu(path)}


def refusal_rule(reason):
    if reason.startswith("Protected path:"):
        return "protected-path"
    if "conflict" in reason.lower():
        return "conflict"
    if "no measurable source changes" in reason or "no remaining source change" in reason:
        return "empty"
    for fragment, rule in (
        ("60 changed paths", "source-file-count"),
        ("1 MiB per file", "source-byte-count"),
        ("source naming rules", "source-name"),
        ("regular C/C++", "source-file-type"),
        ("UTF-8", "source-encoding"),
        ("collides", "source-case"),
        ("identity", "source-attribution"),
        ("one merge base", "conflict"),
        ("additional paths", "conflict"),
    ):
        if fragment in reason:
            return rule
    return "source-intake"


def candidate(git, event):
    base, head = (event[key] for key in ("base", "head"))
    try:
        prepared = git.prepare(base, head, event["number"], event["login"], event["uid"])
    except Refusal as rejected:
        raise Refused(refusal_rule(str(rejected))) from None
    changes = []
    for change in prepared.changes:
        path, mode, oid = change.path, change.mode, change.oid
        if tu(path):
            expected = "expected/" + path[4:] + ".json"
            if git.run("cat-file", "-e", f"{base}:{expected}", check=False).returncode:
                raise Refused("new_tu")
            if mode == "000000":
                raise Refused("deletion")
        data = blob(git, prepared.commit, path) if mode != "000000" else None
        if data is not None:
            issue = compiler_source_problem(data)
            if issue:
                rule, line = issue
                raise Refused(rule, f"{path}:{line}")
        changes.append({"path": path, "mode": mode, "oid": oid,
                        "blob": encode(data) if data is not None else None})
    changed = sorted(row["path"] for row in changes)
    old_files, _ = projection(git, base)
    new_files, _ = projection(git, prepared.commit)
    include_cache = {}
    paths = affected(old_files, changed, include_cache=include_cache)
    paths |= affected(new_files, changed, include_cache=include_cache)
    if not paths:
        raise Refused("no-includers")
    if len(paths) > MAX_AFFECTED:
        raise Refused("wide-cap")
    for path in paths:
        for rev, name in ((base, "expected/" + path[4:] + ".json"), (base, path), (prepared.commit, path)):
            if git.run("cat-file", "-e", f"{rev}:{name}", check=False).returncode:
                raise Refused("new_tu")
    return {"version": 1, "repo": REPO_ID, "number": event["number"], "base": base, "head": head,
            "tree": prepared.tree, **PIN, "policy": policy(),
            "changed_paths": changed, "request_paths": sorted(paths),
            "changes": changes}


def reconstruct(git, plan):
    validate_binding(plan)
    changes, total = [], 0
    for row in plan["changes"]:
        path, mode, oid = (row[k] for k in ("path", "mode", "oid"))
        if not safe_path(path) or mode not in {"000000", "100644"}:
            raise ValueError("The compiler payload contains an unsupported source path.")
        if mode == "000000":
            if oid != "0" * 40 or row["blob"] is not None:
                raise ValueError("The compiler payload contains an invalid deleted path.")
        else:
            data = decode(row["blob"], 1024**2)
            total += len(data)
            if (len(data) > 1024**2 or total > 8 * 1024**2 or compiler_source_problem(data)
                    or git.value("hash-object", "-w", "--stdin", data=data) != oid):
                raise ValueError("The compiler payload failed its source validation.")
        changes.append(Change(path, mode, oid))
    changed = sorted({change.path for change in changes})
    if len(changed) != len(changes) or not 0 < len(changed) <= 60 or changed != plan["changed_paths"]:
        raise ValueError("The compiler payload has an inconsistent path list.")
    tree = git.tree_with(plan["base"], changes)
    if tree != plan["tree"]:
        raise ValueError("The rebuilt tree differs from the admitted tree.")
    git.case_check(tree, changes)
    # This synthetic commit exists only in this job. Only its validated tree is measured.
    return git.commit(tree, plan["base"], "Apply admitted source tree\n", "bfme-boat", 1)


def main(work, out):
    work, out = Path(work), Path(out)
    out.mkdir(mode=0o700, exist_ok=True)
    event = read(work / "event.json", 4096)
    git = Git(work / "source.git")
    try:
        plan = candidate(git, event)
    except Refused as rejected:
        write(out / "decision.json", {key: event[key] for key in ("number", "head", "base")}
              | {"version": 1, "repo": REPO_ID, "kind": "gate", "rules": [rejected.rule]}
              | ({"location": rejected.location} if rejected.location else {}))
        return
    is_narrow = narrow(plan)
    is_wide_auto = (not is_narrow) and wide_eligible(plan) and bool(event.get("automatic_wide_allowed"))
    auto_eligible = is_narrow or is_wide_auto
    automatic = event.get("automatic_author") if auto_eligible else None
    if not event["approved"] and automatic is None:
        kind = "awaiting-automatic" if auto_eligible and event.get("automatic_pending") else "awaiting-approval"
        write(out / "decision.json", {key: event[key] for key in ("number", "head", "base")}
              | {"version": 1, "repo": REPO_ID, "kind": kind})
        return
    route = "compile" if len(plan["request_paths"]) <= 8 else "wide"
    from provider_state import for_plan
    plan.update(for_plan(git, plan["base"], plan["request_paths"], plan["changed_paths"]))
    write(out / "plan.json", plan)
    extra_auto = {}
    if automatic is not None:
        extra_auto["automatic_author"] = automatic
        if is_wide_auto:
            extra_auto["automatic_kind"] = "wide"
    write(out / "decision.json", {key: plan[key] for key in ("number", "head", "base", "tree")}
          | {"version": 1, "repo": REPO_ID, "kind": route, "plan": digest((out / "plan.json").read_bytes())}
          | extra_auto)
    out_route = route
    if automatic is not None:
        out_route = "automatic-wide" if route == "wide" else "automatic"
    with Path(os.environ["GITHUB_OUTPUT"]).open("a") as output:
        output.write("route=" + out_route + "\n")



if __name__ == "__main__":
    main(*sys.argv[1:])

