# Running Agent Swarms on BFME2-Rebuild

This guide is for running **1 to N autonomous coding agents** safely and productively against `Open-BFME/BFME2-Rebuild`.
Every eligible Ready PR is verified in a sealed, network-isolated Visual C++ 7.1 (`cl.exe 13.10.3077`) compiler sandbox (Boat) and eligible green PRs auto-merge into `main`.

## 1. Before launching a swarm

1. **Assign disjoint slices up front** so agents never collide on the same TU or RVA:
   - Slice `worklist/unwritten_with_zero_hour_counterpart.tsv` (filter `ready_at_snapshot=yes`, `reference_kind=def`, `body_check=none-found`, small `size_bytes`) or `worklist/files.tsv` by **file** (1 file = 1 agent).
   - Always check open PRs before starting:
     ```sh
     gh pr list -R Open-BFME/BFME2-Rebuild --state open --search "<file or RVA>"
     ```
2. **Respect daily admission budgets:**
   - **Narrow (`.cpp`-only, 1–8 files, <= 8 affected TUs):** fastest lane with the largest daily budget. Per-commit daily deduplication means `main` moving does **not** burn extra quota.
   - **Wide (`include/` edits):** lower daily budget (H10). Prefer `.cpp`-only rows first, and only edit `include/` when a declaration is genuinely missing or promoting an `include/Draft/` header.
3. **Never burn Boat runs on local failures:**
   - Every agent MUST commit locally and run `python3 tools/gates/gate.py . <upstream-main-ref> HEAD --json /tmp/gate-<slug>.json` (using a unique `<slug>` per worker!) until it exits `0` with `"status": "pass"` BEFORE pushing or marking a PR Ready.
4. **Never retry indefinitely:**
   - Cap hosted Boat iterations at **3 rounds per function** (local `precheck.py` runs are unlimited). If a function doesn't match after 3 Boat rounds, record the compiler/ABI finding in an issue or `docs/hard_rows.md`, close the PR, and move on.
5. **Handling `Held: merge-forbidden` (HTTP 403):**
   - If a PR passed Boat (and Header rules if applicable) but `BFME II / Landing` shows `Held: merge-forbidden`, a workflow file may have changed on `main` since the branch was created. Sync the fork branch with upstream `main` (`git merge upstream/main` or GitHub Update Branch) and push once.

---

## 2. Ready-to-use prompts

### Prompt A — Single Autonomous Contributor Agent (copy-paste ready)

```text
You are contributing to https://github.com/Open-BFME/BFME2-Rebuild (a clean-room
1-to-1 C++ rebuild of Battle for Middle-earth II 1.06 compiling under Visual C++ 7.1).

Read only AGENTS.md, RULES.md and docs/anti-patterns.md, and follow the AGENTS.md checklist.

Your task: implement and auto-land ONE clean, byte-matching function (or one small
cohesive set of functions in a single TU).

Workflow:
1. Sync your fork with upstream `main` (https://github.com/Open-BFME/BFME2-Rebuild).
2. Pick one unclaimed target from `worklist/unwritten_with_zero_hour_counterpart.tsv`
   (`ready_at_snapshot=yes`, `reference_kind=def`, `body_check=none-found`,
   `size_bytes <= 200`) — pick randomly or from a slice rather than row 1.
   Verify no open PR mentions its RVA or TU (`gh pr list -R Open-BFME/BFME2-Rebuild --state open --search "<RVA>"`)
   and that the owning `.cpp` still has an empty body below its `// FUNCTION:` line.
3. Work only on the slice your swarm planner assigned; do not open Draft claim PRs.
   Iterate locally with `tools/local_check/precheck.py` (unlimited) before any push.
4. Disassemble the exact bytes in `baseline/game.dat` at `VA = RVA + 0x400000`,
   and read the Zero Hour counterpart under `reference/` if present. Where Zero Hour
   and `baseline/game.dat` disagree, follow `baseline/game.dat`.
5. Write clean, idiomatic, properly typed C++ directly below the owning `// FUNCTION:`
   line using shared headers under `include/`.
   - Never add inline asm, `__emit`, `#pragma optimize`, `void *` stand-ins,
     duplicate class/global definitions, or address/date/author comments in source.
   - Never edit `// FUNCTION:` or `// cl:` lines, `expected/`, `config/`, `tools/`,
     `ci/`, or anything under `src/Code/Tools/`.
   - Only add `// dxsdk` or `// platformsdk` (non-vendored `.c`/`.cpp`, LF-only) if
     an included header genuinely requires DirectX or Platform SDK headers.
   - Only edit an `include/` header if a required member declaration is missing
     (keep edits within H1-H10's supported subset).
   - Add the EA/GPL `NOTICE` line if adapting Zero Hour code or comments.
6. Commit on top of fresh upstream `main` with an imperative subject (<= 72 chars)
   and evidence in the commit body (RVA, size, retail vtable slot for virtuals,
   Zero Hour `path:line`, and any retail differences). Do NOT paste raw disassembly dumps.
7. Run the local source gate on your committed tree BEFORE pushing:
   `python3 tools/gates/gate.py . <upstream-main-ref> HEAD --json /tmp/gate-<slug>.json`
   Fix every G1-G8 finding until it exits 0 with `"status": "pass"`.
8. Push to your fork and mark the PR **Ready for review** against
   `Open-BFME/BFME2-Rebuild:main` with `Claim: <RVA>` and your evidence summary.
   (Keep `Allow edits by maintainers` enabled.)
9. Monitor the PR statuses (`Admission`, `source rules`, `Boat`, `Header rules`, `Landing`):
   - Wait patiently on `Held: daily limit` or `checking the source rules` (auto-retried).
   - If `Landing` says `Held: merge-forbidden` (HTTP 403) on a green PR, merge
     upstream `main` into your branch and push once.
   - If `Boat` refuses the patch, inspect the diff report, refine the C++, re-run
     `gate.py`, re-run `precheck.py` locally as often as needed (no limit), and push — at most 3 hosted Boat rounds.
   - Eligible green PRs auto-merge without human approval. If still blocked after
     3 rounds, record the concrete blocker in an issue or `docs/hard_rows.md`,
     close the PR, and report the outcome.
```

---

### Prompt B — Parent Agent + Parallel Subagents Swarm (N workers in isolated worktrees)

```text
You are the parent agent running a swarm of N parallel subagents contributing to
https://github.com/Open-BFME/BFME2-Rebuild (a clean-room 1-to-1 C++ rebuild of
Battle for Middle-earth II 1.06 compiling under Visual C++ 7.1).

Read only AGENTS.md, RULES.md and docs/anti-patterns.md; each subagent follows the AGENTS.md checklist.
Never run with the repository owner's GitHub token — always push from a non-owner
contributor/bot fork.

Setup (parent agent):
1. Clone (or update) your fork locally alongside upstream `https://github.com/Open-BFME/BFME2-Rebuild.git`
   and fetch fresh `upstream/main`.
2. Query all currently open PRs on `Open-BFME/BFME2-Rebuild` (titles + bodies):
   `gh pr list -R Open-BFME/BFME2-Rebuild --state open --limit 200 --json number,title,body,files`
   Build an exclusion set of every claimed RVA, `.cpp` TU, and `include/` header.
3. Select N strictly disjoint targets from `worklist/unwritten_with_zero_hour_counterpart.tsv`
   (`ready_at_snapshot=yes`, `reference_kind=def`, `body_check=none-found`, `size_bytes <= 200`)
   or `worklist/files.tsv`:
   - Aim for ~80% narrow `.cpp`-only targets (high daily budget, fastest auto-land)
     and at most ~20% clean header/Draft targets (wide budget H10).
   - Never assign two subagents to the same `.cpp` file or the same `include/` header.
4. Create ONE isolated git worktree & branch per subagent off `upstream/main`:
   `git worktree add ../wt-<slug> -b swarm/<slug> upstream/main`

Subagent instructions (pass one disjoint target + worktree path to each subagent):
- Work ONLY inside your assigned worktree (`../wt-<slug>`). Keep any scratch scripts,
  layout probes, or compile outputs in `/tmp/bfme-<slug>/` (never inside the repo).
- Confirm the target RVA still has an empty body below its `// FUNCTION:` line and
  work only on your assigned slice (no Draft claim PRs).
- Use local tools on this machine freely to verify before pushing:
  * Disassemble `baseline/game.dat` at `VA = RVA + 0x400000` (`objdump`, Python, etc.)
    and verify offsets, constants, vtable slots, and calling conventions; where Zero Hour
    (`reference/`) disagrees with `baseline/game.dat`, follow `baseline/game.dat`.
  * Run local compiler/layout sanity checks (`clang++ -m32`, `g++ -m32`, `cl.exe`, or
    `tools/judge/bin/judge.py` if local VC7.1 is configured per `tools/README.md`) to
    catch syntax, ABI, and byte mismatches before spending a hosted Boat run.
- Write clean, idiomatic C++ (no inline asm, `__emit`, `#pragma optimize`, `void *`
  stand-ins, duplicate class/global definitions, address/date/author comments, or
  edits under `src/Code/Tools/`, `config/`, `expected/`, `tools/`, `ci/`).
- Commit on top of `upstream/main` (imperative subject <= 72 chars; evidence in body,
  no raw disassembly dumps; add `NOTICE` attribution if adapting Zero Hour code).
- Run the local source gate on the committed tree until it exits 0 with `"status": "pass"`:
  `python3 tools/gates/gate.py . upstream/main HEAD --json /tmp/gate-<slug>.json`
- Push to your fork and mark the PR Ready for review (with `Allow edits by maintainers`).
- Return a structured handoff to the parent agent containing:
  `pr_url`, `head_sha`, `gate_status`, `gate_policy_digest`, `local_check_summary`,
  and any CI status.

Parent orchestration loop:
1. Verify every subagent handoff reports `gate_status == pass` on its exact pushed
   `head_sha` before monitoring Boat. (Hosted Boat remains the sole authoritative proof;
   local checks only filter errors before spending Boat slots.)
2. Poll PR statuses (`Admission`, `source rules`, `Boat`, `Header rules`, `Landing`):
   - Wait on `Held: daily limit`, `checking the source rules`, or `runners unavailable`.
   - If a green PR hits `Landing: Held: merge-forbidden` (HTTP 403), have its subagent
     merge `upstream/main` and push once.
   - If `Boat` refuses a PR, send the exact diff report back to that subagent for
     refinement — cap at 3 Boat rounds per function. If still blocked after 3 rounds,
     record the blocker in an issue or `docs/hard_rows.md` and close the PR.
3. As subagents finish (auto-merged or cleanly closed), remove their worktrees
   (`git worktree remove ../wt-<slug>`), re-fetch `upstream/main`, re-scan open PRs,
   and launch the next wave until the target count or daily budget is reached.
```

---

### Prompt C — Header & Draft Promotion Specialist

```text
You are promoting one `include/Draft/` header (or adding missing verified
declarations to an existing shared header) on https://github.com/Open-BFME/BFME2-Rebuild.

Read only AGENTS.md, RULES.md and docs/anti-patterns.md.

Rules for H1-H10 auto-landing:
1. Pick one unclaimed entry from `worklist/draft_headers_in_use.tsv` (or a header
   blocking an unwritten `.cpp` function). Confirm no open PR claims it
   (no Draft claim PRs).
2. Verify every class size, field offset, and vtable slot against `baseline/game.dat`
   (and Zero Hour `reference/` only where retail agrees).
3. For Draft promotions: create the permanent header under `include/`, update every
   includer `.cpp` to include the permanent header, and delete the `include/Draft/`
   file in the same PR.
4. For existing shared headers:
   - Adding non-virtual method declarations whose compiled symbol matches the ledger
     (H2) or plain non-inline, non-pure virtual overrides of existing base virtual
     slots (without default arguments or private-base visibility errors, H5) can
     auto-pass H1-H10 without human approval.
   - Moving or adding a data member requires a newly matched function in the same PR
     that reads/writes that member (H5 typed-access witness).
   - Cite the retail vtable slot in the commit message / PR description for any
     virtual method declaration.
   - Never remove an existing declaration without a `Removed-Declaration: <Name>`
     commit trailer.
   - Never add forced inlining (`__forceinline` / `WWINLINE`) unless it corresponds
     to an existing Zero Hour inline declaration (H8).
5. Run `python3 tools/gates/gate.py . <upstream-main-ref> HEAD --json /tmp/gate-<slug>.json`
   and `python3 tools/local_check/precheck.py --base upstream/main` until both pass (local retries are
   unlimited), mark the PR Ready with full evidence, and use at most 3 hosted Boat rounds.
```
