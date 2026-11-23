# Find a contribution

The newest lists are on the repository's `progress` branch, regenerated automatically
after every change to `main`, beside the project's progress card. The copies
in this directory are from the last snapshot of `main`; prefer the `progress` branch when it exists.

These tables are dated tab-separated files. Their first line gives the
measurement date and revision. Compare it with your checked-out `main`.
The function tables may come from different revisions, and a ready entry
can have a new body or owner by the time you read it.

The `revision=` value names the maintainers' working revision that was
measured. It is not a commit in this repository: this repository's single
snapshot was exported from that revision or a later one. Treat
`measured_utc` as the age of the table and check each row against the
checkout before you start (search `src/` and `include/` for the RVA and the
function name).

## Choose a file to finish

The unit of work is a file: finish one source file, or replace one Draft
header that is in use. [Files to finish](files.tsv) lists every source file
that still owns functions not yet credited MATCH, closest to finished first.
A file whose remaining functions were all written and still do not match
(`tried_not_matching` equals `not_yet_matched`) is hard rather than nearly done,
so it comes after the files that still have unwritten functions.
`owned_functions`, `not_yet_matched` and `bytes_not_yet_matched` say how much
is left, `link_referrers_waiting` how much of the rebuilt game waits on it,
`tried_not_matching` how many of the remaining functions already have a body
that does not match yet, and `class_header` whether a header named like the
file, or declaring a class whose functions the file defines, exists outside
Draft (`real`), only in Draft (`draft`), or not at all (`none`), a quick
name test, not the check's FINISHED rule; or the file is still named by
address (`address-named`: identifying its real name and place is a
contribution of its own). The verdicts are the maintainers' dated checker
state, named in the first line.

The lists below help inside a file.

## Remove a stand-in type

[Stand-in types](stand_in_types.tsv) lists declarations in ordinary headers
whose return or parameter type is `void *`. Many are a type the code already
knows, erased at a module boundary, so every caller casts it back. Replace it
with the real type in the declaration, the definition and every caller in one
change; the bytes must not change. Some entries are legitimate (buffers,
allocators, user data) and stay; check the callers first. INI field parsers
(`INI *`, `void *instance`, `void *store`, `const void *`) are left out: that
signature is generic by design. Integer words used
for pointers are the same problem and are not listed yet.

## Lists for single functions and headers

1. [Unwritten functions with a Zero Hour counterpart](unwritten_with_zero_hour_counterpart.tsv).
   Start with `ready_at_snapshot=yes` and `reference_kind=def`, a lead with a
   body in the public source. `decl` points to a declaration and can help
   identify a function. Open `zero_hour_file` at `zero_hour_line` and confirm
   `zero_hour_function`. If the line or spelling is ambiguous, search
   `reference/CnC_Generals_Zero_Hour/GeneralsMD/Code`. A zero `link_referrers`
   count means the recorded link report had no referrers for that item; a
   higher count means more rebuilt code waits for it, so it ranks higher.
   Rows whose function already has a body in the checkout (in its owning
   file, in a header that file includes, or elsewhere under `src/` or
   `include/`) are left out when the list is written; the first line says
   how many. `body_check` says what the check found for the rest:
   `none-found`, `unclear` (look before you start) or `owner-file-missing`
   (find the current owner first).
2. Draft headers. A Draft is a provisional declaration or layout under
   `include/Draft/`. There are two lists:
   - [Drafts in use](draft_headers_in_use.tsv): Drafts that some source file
     or header includes. These are the ones to retire, by promoting the
     layout to a real header and moving every includer. Start with a class
     you can justify from the target evidence and a set of includers you can
     review. `direct_including_files` counts files with a literal include; a
     header that includes the Draft can have further users, and the check
     determines that wider set.
   - [Unused Drafts](draft_headers_unused.tsv): Drafts nothing includes. They
     are generated layout guesses for classes no written code uses yet, kept
     only as a lead for whoever writes that class; do not promote one on its
     own. A Draft whose class or file name already exists outside
     `include/Draft/` is a dead copy and is left out (count in the first line).
3. [Functions missing from the standalone link](link_ranked.tsv). A standalone
   link tries to join the rebuilt object files into a game image. The
   referrer count ranks demand for the symbol. `symbol` is the compiler's
   encoded spelling, which includes type and class information. `state`
   summarizes the snapshot's provider and check outcome. For example,
   `placeholder-owned:MISSING` means a provisional provider assignment
   and a missing emitted body at that snapshot. It can still have an
   accepted real name. `first_referrer` is a file to inspect. A reference
   can come through a base constructor, generated code, or a header.
   Rows already written in the checkout are left out, and `body_check` has
   the same meaning as in the first list.

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
ledger and `baseline/game.dat`, and a local check is optional.

For a Draft promotion, find the current shared type and every direct include,
then follow headers to their users. Read the [promotion
procedure](../docs/source-quality.md#promote-a-draft-header). If a legacy
comment mentions an unavailable investigation note or a rejected path,
describe the intended path and ask a maintainer to confirm it before doing
the move. Keep slot behavior and dependent translations in the same review.

The [pull-request guide](../ci/README.md) explains the source paths the check
accepts and its statuses. A new body is measured for the row's name, code
and checked references. A shared-header change is measured across the
affected object files. The check can refuse a compile error, prohibited
source shape, or loss of old match credit, reference proof or object
verification. An eligible pass measures the proposed change against its
base. Standalone link and runtime evidence come from separate checks.

## Claim an item

Claims are a convention, not a lock. Before you start, search the open pull
requests for the address, file or header. Then open a draft pull request
whose description starts with your claim: `Claim: src/path/File.cpp` for a
file, `Claim: include/Draft/X.h` for a Draft header, or one `Claim: 0x00ABCDEF`
line per function. Nothing publishes claims yet; the open pull requests are
the list. Close or finish a claimed pull request when you stop, so the item
is free again. For a header, also say in an issue which files you will touch.

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
copied across (recorded 2026-11-18).

### AsciiString construction

Zero Hour's
[`AsciiString(const char*)`](../reference/CnC_Generals_Zero_Hour/GeneralsMD/Code/GameEngine/Include/Common/AsciiString.h#L363)
initializes its own member and calls its setter. The landed
[`AsciiString` wrapper](../include/Common/AsciiString.h)
delegates construction to `StringBase<char>`. Reference callers mostly use
the base constructor directly while some reach an emitted wrapper copy.
That pattern supports one inline wrapper and preserves the reference's
by-value temporary setup (recorded 2026-11-16).

### Model-draw transform hook

Zero Hour's
[`W3DModelDraw::adjustTransformMtx`](../reference/CnC_Generals_Zero_Hour/GeneralsMD/Code/GameEngineDevice/Source/W3DDevice/GameClient/Drawable/Draw/W3DModelDraw.cpp#L1994)
has a nontrivial body. The landed
[`W3DScriptedModelDraw::adjustTransformMtx`](../include/GameClient/W3DScriptedModelDraw.h)
has an empty base hook, with a derived override where the table calls for
one. The reference constructor calls `DrawModule` directly, and the
scripted model's table makes that class the shared model-draw base.
Zero Hour supports the hook's name and signature, while the reference class
and slot establish ownership and behavior (recorded 2026-11-18).

## Refresh the lists

From a clean checkout (`src/` and `include/` clean against `HEAD`), run:

```sh
python3 worklist/generate.py
```

This rebuilds `draft_headers_in_use.tsv`, `draft_headers_unused.tsv` and
`stand_in_types.tsv` from the checked-out source, and leaves existing function
tables untouched.

To refresh `unwritten_with_zero_hour_counterpart.tsv`, `link_ranked.tsv` or
`files.tsv` locally, pass the published inputs from `worklist/inputs/` on the
`progress` branch (`--unwritten`, `--ranked` and `--verdicts`), ideally into a
temporary directory with `--output`. For example, to check that a committed
function drops out of `unwritten_with_zero_hour_counterpart.tsv`:

```sh
out=$(mktemp -d)
python3 worklist/generate.py --unwritten /path/to/unwritten.tsv --output "$out"
```

See [Progress card and worklists](../ci/progress/README.md#worklists) for the
full recipe to regenerate all six tables from the published `worklist/inputs/`
files on `progress`.

The function lists depend on reference correspondence, current checker
results and a standalone link report. Refresh those measurements before
running the generator to reflect a new landing. The generator preserves
each source report's original date and revision independently. Missing
measurements leave the previous dated snapshot in place.

The generator checks every function row against the checkout and leaves out
rows that already have a body. Without `--output`, it rewrites the tracked
files in `worklist/`; restore them (`git checkout -- worklist/`) before you
commit.
