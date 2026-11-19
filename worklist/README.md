# Find a contribution

These tables are dated tab-separated files. Their first line gives the
measurement date and revision. Compare it with your checked-out `main`.
The function tables may come from different revisions, and a ready entry
can have a new body or owner by the time you read it.

## Choose a small item

1. [Unwritten functions with a Zero Hour counterpart](unwritten_with_zero_hour_counterpart.tsv).
   Start with `ready_at_snapshot=yes` and `reference_kind=def`, a lead with a
   body in the public source. `decl` points to a declaration and can help
   identify a function. Open `zero_hour_file` at `zero_hour_line` and confirm
   `zero_hour_function`. If the line or spelling is ambiguous, search
   `reference/CnC_Generals_Zero_Hour/GeneralsMD/Code`. A zero `link_referrers`
   count means the recorded link report had no referrers for that item.
2. [Draft headers](draft_headers.tsv). A Draft is a provisional declaration
   or layout. Start with a class you can justify from the target evidence and
   a set of includers you can review. The first hundred rows are the priority
   shortlist. `direct_including_files` counts source files and headers with
   a literal include. A header that includes the Draft can have further
   callers. The check determines that wider set.
3. [Functions missing from the standalone link](link_ranked.tsv). A standalone
   link tries to join the rebuilt object files into a game image. The
   referrer count ranks demand for the symbol. `symbol` is the compiler's
   encoded spelling, which includes type and class information. `state`
   summarizes the snapshot's provider and check outcome. For example,
   `placeholder-owned:MISSING` means a provisional provider assignment
   and a missing emitted body at that snapshot. It can still have an
   accepted real name. `first_referrer` is a file to inspect. A reference
   can come through a base constructor, generated code, or a header.

## Check the current owner, header and evidence

Search the exact relative virtual address (RVA) in the checkout. The RVA
is measured from the reference executable's load base. Search the class
and specific function name separately in `src/` and `include/`. A body can
live in a header even if the owning file has an empty `FUNCTION` annotation.

```sh
git grep -n -F '0x000B2E14' -- src include
git grep -n -F 'releaseShadows' -- src include
git grep -n -F 'W3DModelDraw::releaseShadows' -- reference/CnC_Generals_Zero_Hour/GeneralsMD/Code
git grep -n -F -e 'Draft/View.h' -e '#include "View.h"' -e '#include <View.h>' -- src include
```

For example, the `Get_Bounding_Sphere` lead at `0x0006D028` has an empty
annotation in `BaseHeightMap.cpp` and an inline definition in
`include/WW3D2/RenderObjClass.h`. Skip a second body. Review whether the
existing definition emits the required symbol if that evidence is missing.

Use the [ledger lookup](../docs/ledger-and-checks.md#find-a-row-and-its-owner)
for the current accepted names and assignment. An inferred original path,
the caller's path and the tracked source that holds the annotation can
differ. Ask for a ledger change before moving a row. In the `releaseShadows`
example, the tracked owner includes `include/GameClient/W3DScriptedModelDraw.h`.
The target class and the older `W3DModelDraw` class can have different layouts.
If the target field or called function is unconfirmed, ask for the missing
slot, layout or call evidence. A plain clone provides the checked-in notes
and ledger, and a local binary comparison is optional.

For a Draft promotion, find the current shared type and every direct include,
then follow headers to their users. Read the [promotion
procedure](../docs/source-quality.md#promote-a-draft-header). If a legacy
comment mentions an unavailable investigation note or a rejected path,
describe the intended path and ask a maintainer to confirm it before doing
the move. Keep slot behavior and dependent translations in the same review.

The [pull-request guide](../ci/README.md) explains the source paths the check
accepts and its statuses. A new body is measured for the row's name, code
and checked references. A shared-header change is measured across the
affected object files. The gate can refuse a compile error, prohibited
source shape, or loss of old match credit, reference proof or object
verification. An eligible pass measures the proposed change against its
base. Standalone link and runtime evidence come from separate checks.

## Claim an item

After choosing an address, search open draft pull requests by RVA and file
name. Open a draft pull request and put one `Claim: 0x00ABCDEF` line per
address in its description. Substitute the actual RVA. When the bot is
enabled, switch to the `claims` branch on the repository's GitHub page and
open `worklist/claims.json`. A plain clone of main can omit that branch. If
the index has yet to be published, use the open drafts to check for overlap.
Claims are advisory. Follow the [claim rules](../ci/README.md#claim-a-worklist-item)
for the cap, expiry and release. Coordinate headers by path in an issue or
draft description. The bot tracks function addresses only. Updating the
address list refreshes its expiry. Mark the pull request ready or close
it to clear the draft claims. Eligibility to run the checker starts with
maintainer admission. Claims only help people avoid duplicate work.

## Worked examples

### Coord2D normalization

Zero Hour's
[`Coord2D::normalize`](../reference/CnC_Generals_Zero_Hour/GeneralsMD/Code/Libraries/Include/Lib/BaseType.h#L271)
is inline, returns void and guards against a zero length before dividing.
The landed
[`Coord2D::Normalize`](../src/Code/GameEngine/Source/Common/System/UnicodeString.cpp)
uses the translation unit's square helper, multiplies by the reciprocal
and returns the length. The shape comes from the reference function and
its caller contract. It would change if the older zero-length guard were
copied across. Source recovery record, 2026-11-18.

### AsciiString construction

Zero Hour's
[`AsciiString(const char*)`](../reference/CnC_Generals_Zero_Hour/GeneralsMD/Code/GameEngine/Include/Common/AsciiString.h#L363)
initializes its own member and calls its setter. The landed
[`AsciiString` wrapper](../include/Common/AsciiString.h)
delegates construction to `StringBase<char>`. Reference callers mostly use
the base constructor directly while some reach an emitted wrapper copy.
That pattern supports one inline wrapper and preserves the reference's
by-value temporary setup. Source recovery record, 2026-11-16.

### Model-draw transform hook

Zero Hour's
[`W3DModelDraw::adjustTransformMtx`](../reference/CnC_Generals_Zero_Hour/GeneralsMD/Code/GameEngineDevice/Source/W3DDevice/GameClient/Drawable/Draw/W3DModelDraw.cpp#L1994)
has a nontrivial body. The landed
[`W3DScriptedModelDraw::adjustTransformMtx`](../include/GameClient/W3DScriptedModelDraw.h)
has an empty base hook, with a derived override where the table calls for
one. The reference constructor calls `DrawModule` directly, and the
scripted model's table makes that class the shared model-draw base.
Zero Hour supports the hook's name and signature, while the reference class
and slot establish ownership and behavior. Source recovery record, 2026-11-18.

## Refresh the lists

From a clean checkout, run:

```sh
python3 worklist/generate.py
```

This rebuilds the Draft include ranking from the checked-out source.
Use the included TSV snapshots for the other lists. Maintainers produce
the dated correspondence and standalone-link reports outside the checkout.
With both reports available, rebuild the lists as follows.

```sh
python3 worklist/generate.py \
  --unwritten /path/to/zh_named_unwritten.tsv \
  --ranked /path/to/functions_ranked.tsv
```

The first two lists depend on reference correspondence, current checker
results and a standalone link report. Refresh those measurements before
running the generator to reflect a new landing. The generator preserves
each source report's original date and revision independently. Missing
measurements should leave the previous dated snapshot in place.
