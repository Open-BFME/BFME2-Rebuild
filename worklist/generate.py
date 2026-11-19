#!/usr/bin/env python3
"""Write the public worklists from two dated report inputs and a source checkout."""
import argparse
from collections import Counter
import csv
from datetime import datetime, timezone
from pathlib import Path, PurePosixPath
import re
import subprocess

DONOR = "reference/CnC_Generals_Zero_Hour/GeneralsMD/Code"
INCLUDE = re.compile(rb'(?m)^[ \t]*#[ \t]*include[ \t]*[<"]([^"\r\n>]+)[">]')
UNWRITTEN_META = re.compile(r"generated (\S+)\s+main=([0-9a-f]{7,40})")
LINK_META = re.compile(r"generated=(\S+) standalone=([0-9a-f]{7,40})")

def snapshot(path, pattern):
    lines = path.read_text(encoding="utf-8").splitlines()
    meta = next((m for line in lines if (m := pattern.search(line))), None)
    if meta is None:
        raise ValueError(f"Input lacks measurement date and revision: {path.name}")
    rows = csv.DictReader((line for line in lines if line and not line.startswith("#")), delimiter="\t")
    return meta.groups(), list(rows)

def write(path, meta, columns, rows):
    with path.open("w", newline="", encoding="utf-8") as stream:
        stream.write(f"# measured_utc={meta[0]} revision={meta[1]}\n")
        csv_writer = csv.writer(stream, delimiter="\t", lineterminator="\n")
        csv_writer.writerow(columns)
        csv_writer.writerows(rows)

def checked_line(lines, line, qualified_name, kind):
    # The report may have been generated from a different checkout of the
    # released source. Keep the file and name and re-anchor the line.
    qualified = r"\s*::\s*".join(re.escape(part) for part in qualified_name.split("::"))
    token = qualified if kind == "def" else re.escape(qualified_name.rsplit("::", 1)[-1])
    pattern = re.compile(r"(?<![\w])" + token + r"\s*\(")
    locations = [
        index for index, source in enumerate(lines, 1)
        if not source.lstrip().startswith(("//", "/*", "*")) and pattern.search(source)
    ]
    if line in locations:
        return line
    nearest = sorted(locations, key=lambda item: (abs(item-line), item))
    if nearest and (len(nearest)==1 or abs(nearest[0]-line)<abs(nearest[1]-line)):
        return nearest[0]
    return ""


def unwritten(args):
    meta, rows = snapshot(args.unwritten, UNWRITTEN_META)
    donor_root = args.root / DONOR
    names = {}
    for file in sorted(donor_root.rglob("*")):
        if file.is_file():
            name = file.relative_to(donor_root).as_posix()
            names.setdefault(name.casefold(), []).append(name)
    public_rows = []
    lines_by_file = {}
    for row in rows:
        target, kind = row["zh_file:line,kind"].rsplit(",", 1)
        file, line = target.rsplit(":", 1)
        if not line.isdecimal():
            raise ValueError(f"Invalid reference line: {target}")
        donor_paths = names.get(file.casefold(), [])
        if file in donor_paths:
            actual = file
        elif len(donor_paths) == 1:
            actual = donor_paths[0]
        else:
            raise ValueError(f"Missing or ambiguous checked-in reference path: {file}")
        if actual not in lines_by_file:
            lines_by_file[actual] = (donor_root / actual).read_bytes().decode("latin-1").splitlines()
        anchor = checked_line(lines_by_file[actual], int(line), row["class::method"], kind)
        public_rows.append((row["rva"], row["class::method"], row["file"], int(row["size"]),
                            f"{DONOR}/{actual}", anchor, row["class::method"], kind,
                            int(row["referrers"] or 0), row["writable"]))
    write(args.output / "unwritten_with_zero_hour_counterpart.tsv", meta,
          ["rva", "function", "owner_file", "size_bytes", "zero_hour_file", "zero_hour_line",
           "zero_hour_function", "reference_kind", "link_referrers", "ready_at_snapshot"], public_rows)

def link_ranked(args):
    meta, rows = snapshot(args.ranked, LINK_META)
    columns = ["referrers", "state", "rva", "file", "symbol", "first_referrer"]
    rows.sort(key=lambda row: (-int(row["referrers"]), row["rva"], row["symbol"]))
    write(args.output / "link_ranked.tsv", meta, columns, ([row[key] for key in columns] for row in rows))

def draft_headers(args):
    git = ["git", "-C", str(args.root)]
    def command(*options):
        return subprocess.check_output([*git, *options]).decode()
    date, revision = command("show", "-s", "--format=%cI %H", args.revision).split()
    date = datetime.fromisoformat(date).astimezone(timezone.utc).isoformat().replace("+00:00", "Z")
    meta = date, revision
    files = command("ls-tree", "-r", "-z", "--name-only", args.revision, "--", "src", "include").strip("\0").split("\0")
    drafts = {file for file in files if file.startswith("include/Draft/") and PurePosixPath(file).suffix.lower() in (".h", ".hpp", ".inl")}
    counts = Counter()
    for file in files:
        source = args.root / file
        if source.suffix.lower() not in (".cpp", ".c", ".h", ".hpp", ".inl", ".cc", ".cxx"):
            continue
        used = set()
        for match in INCLUDE.finditer(source.read_bytes()):
            target = match[1].decode("utf-8", "replace").replace("\\", "/")
            for path in (target, "include/" + target, (PurePosixPath(file).parent / target).as_posix()):
                if path in drafts:
                    used.add(path)
                    break
        counts.update(used)
    ordered = sorted(drafts, key=lambda name: (-counts[name], name))
    write(args.output / "draft_headers.tsv", meta, ["header", "direct_including_files"],
          ((name, counts[name]) for name in ordered))

def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--root", type=Path, default=Path(__file__).resolve().parents[1], help="checked-out source and reference tree")
    parser.add_argument("--revision", default="HEAD", help="source revision checked out under --root")
    parser.add_argument("--output", type=Path)
    parser.add_argument("--unwritten", type=Path, help="dated correspondence report, including the header row")
    parser.add_argument("--ranked", type=Path, help="dated standalone link report")
    args = parser.parse_args()
    args.output = args.output or args.root / "worklist"
    args.output.mkdir(parents=True, exist_ok=True)
    for path in ("src", "include"):
        subprocess.run(["git", "-C", str(args.root), "diff", "--quiet", args.revision, "--", path], check=True)
    draft_headers(args)
    if args.unwritten:
        unwritten(args)
    if args.ranked:
        link_ranked(args)

if __name__ == "__main__":
    main()
