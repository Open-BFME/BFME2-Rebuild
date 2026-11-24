# AGENTS.md

Rebuild *Battle for Middle-earth II* (`game.dat` 1.06) as clean, readable C++ that compiles byte for byte with
Visual C++ 7.1 (`cl.exe 13.10.3077`).

**Read exactly three files: this one, [RULES.md](RULES.md) and [docs/anti-patterns.md](docs/anti-patterns.md).**
Do not read `docs/maintainers/`, `ci/` or `tools/` internals; they describe the machinery, not your task.
[docs/hard_rows.md](docs/hard_rows.md) is the dead-end log: read the entry for your item if one exists.
Maintainers add to it; you report a dead end on your pull request (step 15).

Facts: `baseline/game.dat` is the ground truth (`VA = RVA + 0x400000`). `reference/` is Zero Hour source,
evidence for names, types and shape only; where it and the bytes differ, the bytes win. You edit `src/` and
`include/` only. Never touch `config/`, `expected/`, `tools/`, `ci/`, `.github/` or the canary pull requests
`#3`, `#4`, `#21`, `#27`, `#29`.

## Checklist

1. **Fork and clone** (Windows needs long paths):
   ```sh
   gh repo fork Open-BFME/BFME2-Rebuild --clone -- -c core.longpaths=true -c core.autocrlf=input
   ```
2. **Branch from fresh upstream `main`** (one worktree per parallel agent):
   ```sh
   git fetch upstream && git switch -c work/<slug> upstream/main
   ```
3. **Pick one item.** A small function with a Zero Hour counterpart; pick at random, not the top row:
   ```sh
   awk -F'\t' '$10=="yes" && $8=="def" && $11=="none-found" && $4<100' worklist/unwritten_with_zero_hour_counterpart.tsv | shuf -n 5
   ```
   Columns: `rva`, `function`, `owner_file`, `size_bytes`, `zero_hour_file`, `zero_hour_line`.
4. **Check nobody has it.** No open pull request, and the owner file has an empty body under its
   `// FUNCTION:` line:
   ```sh
   gh pr list -R Open-BFME/BFME2-Rebuild --state open --search "<rva>" && git grep -n -F "<rva>" -- src include
   ```
   If either shows work in progress, go back to step 3. Do not open draft pull requests to claim.
5. **Read the original bytes** at `VA = RVA + 0x400000`, then the Zero Hour body at `zero_hour_file:zero_hour_line`:
   ```sh
   objdump -d -M intel --start-address=<va> --stop-address=<va+size> baseline/game.dat
   ```
6. **Write the body** directly under its `// FUNCTION:` line, with real types from the shared headers, in
   ordinary C++. Check every line against [RULES.md](RULES.md) and [docs/anti-patterns.md](docs/anti-patterns.md).
   Touch `include/` only when a declaration is genuinely missing. Add the `NOTICE` line as the first line of
   the file if you adapted Zero Hour text. Keep scratch files and disassembly outside the repository.
7. **Commit.** Imperative subject of at most 72 characters; body with the evidence (RVA, size, Zero Hour
   `path:line`, vtable slot for a virtual, what you inferred); one `Removed-Declaration: Class::member` line per
   removed or renamed declaration:
   ```sh
   git commit -am "Write FrameData::allCommandsReady"
   ```
8. **Run the source gate** on the committed tree. It must exit `0` with `"status": "pass"`:
   ```sh
   python3 tools/gates/gate.py . upstream/main HEAD --json /tmp/gate.json
   ```
9. **Run the local match check.** There is no limit on local runs, turns or retries. Repeat steps 6–9 as
   many times as you need, until every function prints `PASS` and the exit status is `0`. Only then push.
   ```sh
   git branch -f main upstream/main && python3 tools/local_check/precheck.py --base upstream/main
   ```
   It needs a licensed Visual Studio .NET 2003, set up once by you or your operator (tools/README.md,
   "Fast local pre-check"). Without it, the gate is your only local check and every push spends a hosted round.
10. **Stop if it will not match cleanly.** If the only way to match is a row of
    [docs/anti-patterns.md](docs/anti-patterns.md), do not push. Go to step 15.
11. **Push and open a Ready pull request** (not a draft) using the template, with `Claim:` lines first and a
    `Confidence:` line:
    ```sh
    git push -u origin work/<slug> && gh pr create -R Open-BFME/BFME2-Rebuild --base main --body-file /tmp/pr.md
    ```
12. **Read the statuses:**
    ```sh
    gh pr checks <number> -R Open-BFME/BFME2-Rebuild
    ```
    - `Held: daily limit`, `checking the source rules`, `runners unavailable`: wait; they retry by themselves.
    - `BFME II / source rules` failed: fix the named rule, then steps 7–9 again.
    - `BFME II / Boat` failed: open **Details** for the first differing offset, fix locally (steps 6–9), push.
    - `Landing: Held: merge-forbidden`: `git merge upstream/main && git push`, once.
    - `Header rules` held (not failed): it waits for a maintainer; leave it.
13. **Hosted Boat limit: three rounds.** At most three pushes that run **BFME II / Boat** per pull request.
    The limit is for the hosted check only; local runs in step 9 do not count.
14. **Landing.** When source rules and Boat pass (and Header rules for `include/` changes), the pull request
    joins the merge queue and lands by itself. Nothing else to do.
15. **Close or hand off.** If it does not match after three Boat rounds, or only matches with an anti-pattern:
    close the pull request with a dead-end comment (RVA, what you tried, the smallest open question).
    The source rules accept only `src/` and `include/`, so do not edit `docs/hard_rows.md` yourself;
    a maintainer copies the comment there:
    ```sh
    gh pr close <number> -R Open-BFME/BFME2-Rebuild -c "Dead end: <rva>. Tried: <...>. Open question: <...>. Confidence: 3/10 — <reason>"
    ```
    Hand-off report, last line: `PR <url>, head <sha>, checks <state>, Confidence: N/10`.

## Lanes

- **Narrow:** only `src/`, 1–8 files, at most 8 affected units, at most 3 substantive. Fastest.
- **Wide:** any `include/` change, up to 60 paths. Also needs **BFME II / Header rules**; anything outside the
  provable subset waits for a maintainer.
- Daily budgets apply per author; a budget hold waits by itself.

## Confidence score

Every pull request description, and every pull request or review comment you write, contains one line:

```text
Confidence: N/10 — <one-sentence reason>
```

| N | Meaning |
| --- | --- |
| 10 | Matches, and every name and type is evidenced. |
| 7 | Matches; one name or type is inferred. |
| 5 | Matches, but names or types are guessed. |
| 3 | Partial match. |
| 1 | Does not match. |

Use the nearest value; state what is inferred or guessed in the reason.
