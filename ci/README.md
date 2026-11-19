# BFME II / Boat

The project checks pull requests against the retail game on Boat. Look for
**BFME II / Boat** in the pull request's checks and for the bot's report in the
conversation. The report identifies the pull request head and the main commit
used for the check.

## Submit a source change

1. Fork the project and branch from main.
2. Change regular **.c** or **.cpp** files under **src/**, or **.h**, **.hh**,
   **.hpp** or **.inl** headers under **include/**. Keep existing FUNCTION
   annotations intact. The server uses the compiler, checker and ledger
   from trusted project inputs.
3. Open a pull request to main. Describe the change and give the evidence for
   names, types, layout and code shape. For an intentional removal of a recently
   added declaration, include one line like **Removed-Declaration: Class::member**
   in the tip commit message and explain the removal in the PR description.
4. Wait for the check. A maintainer must add the **boat-ok** label before your
   first farm check. After approval, later eligible submissions queue
   automatically. Push fixes to the same branch to run again.
5. Request maintainer review when the check passes. The landing service reruns
   the gate before updating main. The bot links the landed commit in the PR
   conversation.

The check accepts up to 60 changed paths, 1 MiB per changed file and 8 MiB of
changed source in total. Use regular UTF-8 text files. Each account can have
one farm check in flight and start up to six distinct head checks per hour.
An edit under **ci/**, **tools/**, **config/** or **expected/** makes the pull
request ineligible for this check. Ask a maintainer about a checker or ledger
change. A draft can reserve a worklist item and is eligible for a check, but
landing waits until you mark it ready for review. The maintainers open
public checks after verifying the compiler-only Boat pool. Until then,
the service holds check admissions.

## Read the result

- **Pending** means the request is queued, needs first-time approval, or is
  waiting for Boat or the checker. A service outage leaves the check pending.
- **Pass** means the measured change meets the landing gates on the reported
  main commit. A maintainer review applies to the current pull request head.
- **Fail** identifies source intake issues, conflicts, compiler diagnostics,
  lost credit, lost reference proof, lost Object VERIFIED or another gate.
  Resolve a conflict against current main and push the fix.

Rows show file, function when available, address, size, and before/after
verdicts. A partial row can show the match percentage, compiled and retail
sizes, and first differing offset within the function. For a function edited
in the pull request, the report can show up to eight instructions from its
compiled output near the offset. Diagnostics for edited files contain the
source location, compiler diagnostic code and a brief description. The report
is capped at 8 KiB. Retail bytes and retail disassembly stay private.

## Claim a worklist item

In an open draft pull request, put one **Claim: 0x00ABCDEF** line in the
description for each address you are working on. Use up to ten lines. Claims
are advisory and expire seven days after the claim list changes. Other people
can still contribute to those functions. Remove a claim line when you give up
an item. Marking the pull request ready or closing it clears its draft claims.

The bot publishes the active claims in **worklist/claims.json** on the
**claims** branch.
