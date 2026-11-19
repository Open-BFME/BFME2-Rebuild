# Contributing

The project recovers one maintainable engine with shared types, original
translation units where the evidence supports them, and behavior that matches
the reference build. Changes need both good source and a passing check.

## Choose and prepare a change

1. Start with the [worklist](worklist/README.md) and check the latest `main`.
   A small function family with real headers and a corresponding Zero Hour
   body makes a good first task. Draft promotion and resolving an evidence
   question are useful contributions too.
2. Search open pull requests for the relative virtual address (RVA), source
   file and class. An RVA is the offset from the reference executable's
   load base. Follow the [claim instructions](worklist/README.md#claim-an-item).
3. Search both `src/` and `include/` for a body, including an inline body
   in a header. Follow the [ledger lookup](docs/ledger-and-checks.md#find-a-row-and-its-owner)
   to the current owning file and accepted name. Compare the [Zero Hour
   source](reference/README.md) with the target evidence in the checkout.
   Cite the two independently. If a necessary address, field or slot is
   unconfirmed, open a focused evidence issue before writing a body.
4. Write ordinary C or C++ against the shared headers. Follow the
   [seven source-quality rules](docs/source-quality.md). Keep existing
   third-party notices and add the attribution in [NOTICE](NOTICE) to each file
   containing text adapted from the Electronic Arts release.
5. Update the pull request with the function or header, the evidence for names
   and layout, and any unresolved references or assumptions. Ask for a
   [ledger change](docs/ledger-and-checks.md#request-a-ledger-change) where
   annotations, ownership or accepted names need correction.

## Review and check

The [pull-request guide](ci/README.md) lists eligible source paths and
limits. Submit regular UTF-8 `.c` or `.cpp` files under `src/` and `.h`,
`.hh`, `.hpp` or `.inl` files under `include/`. Keep ledger, checker,
generated metadata and documentation proposals in a maintainer issue or
a separately coordinated change.

**BFME II / Boat** can check a draft after first-time `boat-ok` approval.
Look for Pending, Pass or Fail and the report in the conversation. The
report identifies the compared head and base, affected source files,
intake or compiler failures, and losses of match credit, reference proof
or verified object results. A header edit reaches both its direct users
and units that use it through other headers. When available, a partial
row shows the first differing offset and a short excerpt of the changed
function compiled from your source. Reference disassembly stays private.

Update against current `main` when the check asks. Mark the pull request
ready for review once it passes. Maintainers review the current head and
the service reruns the landing gate before updating main. Public
admissions open after the dedicated compiler-only pool is verified.

A local check is optional. [Checker inputs and methods](tools/README.md)
describes the toolchain and comparison. Never attach your game executable or
raw reference bytes to a pull request. Point to the patch version, address,
caller, slot, offset or other evidence that a reviewer can verify with their
own reference copy.

A numerically better result does not excuse a duplicate class, lost proof,
invented name or unexplained change in runtime behavior. Keep unsuccessful
bodies out of a source change. Add useful attempts to the pull request
description or an issue. Link any related entry in the [hard-case index](docs/hard_rows.md).

## Further reading

- [Evidence, shared headers, and source quality](docs/source-quality.md)
- [Ledger, verdicts, and ownership requests](docs/ledger-and-checks.md)
- [Techniques that have worked](docs/recipes.md)
- [Recorded blockers](docs/hard_rows.md)
