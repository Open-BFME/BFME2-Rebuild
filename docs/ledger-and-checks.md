# Ledger and checks

The ledger is `config/map.json`. Each row identifies a reference item by its
**relative virtual address**, its offset from the executable image base. The
row records its extent, section, role, accepted symbol names, owner and evidence.
One role marks an owned function. Other roles describe things such as shared
emitted code, folded identities and compiler-generated contributions. The
`expected/` sidecar for a translation unit records expected emitted rows and
separate files can describe its neighboring ranges. Assignments and boundaries
can be hypotheses even when the function matches.

## Find a row and its owner

Search the exact RVA in tracked source first. The owning translation unit
usually has a generated `// FUNCTION: BFME2` annotation. For a ledger
lookup, match `rows[].rva` and read `object_id`, `name`, and
`accepted_names`. Match that ID with `objects[].object_id` for the
recorded object path and its placement evidence. The original-path
hypothesis can differ from the tracked annotation owner.

```sh
git grep -n -F '0x00253390' -- src include
python3 -c 'import json; d=json.load(open("config/map.json")); rows=[r for r in d["rows"] if r["rva"]=="0x00253390"]; print(*rows, sep="\n"); ids={r["object_id"] for r in rows}; print(*[(o["object_id"],o["path"]) for o in d["objects"] if o["object_id"] in ids],sep="\n")'
```

That example points to `src/unk/O0024EEBB.cpp`. The expected-file path is
`expected/unk/O0024EEBB.cpp.json`. Replace the `src/` prefix with `expected/`
and append `.json`. A neighboring-range file uses `.neighbours.json`.
The expected-file rows describe additional emissions. The main ledger
and the owning annotation can identify a function even when the expected
file has no row for it. If the ledger, source and worklist disagree, report
the discrepancy rather than changing the annotation.

## Read the verdict

| Result | Meaning | Evidence still needed |
| --- | --- | --- |
| **MATCH** | An owned row compiled with equal comparison bytes after applying the checker's relocation treatment and name policy. Without a proven wrong reference, the row can be credited while its reference proof remains open. | Inspect reference proof and the result's scope. |
| **Proven** | A credited MATCH also passed reference verification for the checked references. | Runtime behavior, uncovered bodies and original file boundaries have separate checks. |
| **VERIFIED** | The checker found no blocker for the assigned object. It checks owned and expected emitted rows, references, compile receipts, emitted order, extra symbols, applicable gates and unresolved Draft dependencies. | Unassigned contributions and original source completeness remain open where the ledger lacks evidence. |
| **PARTIAL** | The selected compiled body differs under byte comparison. | Check the reported symbol and RVA first. For an unresolved placeholder, a near-match percentage can describe a different compiled body. |
| **MISNAMED** | An otherwise plausible emitted body conflicts with the accepted identity at that address. | Establish the real class, signature and spelling. Request a ledger correction when the ledger is wrong. |
| **WRONG_REFERENCE** | The comparison bytes agree and a relocation points to a target the checker proves wrong. The row receives no Function MATCH credit. | Identify the intended reference and the actual emitted target, including its owning data, slot or symbol. |
| **MISSING / COMPILE_ERROR** | The checker found no usable emitted body for a row, or compilation failed. | Check declarations, flags and output. Inspect current source before assuming the body is unwritten. |

Identical machine code can be shared between several source functions.
A matching target address alone does not transfer a folded function's name
to another slot or class. Read the relocation details and relevant ownership
before changing code.

## Request a ledger change

Open an issue or put a section titled **Ledger change request** in the source
pull request. Give maintainers:

- The current base revision, exact RVA, current accepted name and owner, and
  the requested name, role, extent, alias or owner.
- Direct reference evidence such as the class's slot, one or more callers,
  parser fields, emitted symbol identity, section position or extent.
  Cite the corresponding public ancestor separately where it helped.
- The affected source files, headers, expected rows and dependent calls or
  globals. Give the check outcome and exact blocker for an existing proposal.
- Whether the source and map changes must be measured and landed together.
  For a unit move, explain both the positive path evidence and nearby
  boundaries or competing assignments.

Maintainers review and generate the ledger, expected records and source
annotations together. Keep source changes off the old identity until the
maintainer explains how the pair can be checked. An existing placeholder
can sometimes receive credit under a supported real name. Document the
evidence and let the checker and maintainer handle adoption.

## What a pull-request check covers

The check compiles changed units and units affected by changed headers,
checks the source and generated metadata rules, and compares the result
against the recorded base for losses of existing credit or proof. It reports
compile errors and gate failures. A green check is scoped to the revision,
affected units and checked rules. No pull-request result claims a successful
standalone link or game run.
