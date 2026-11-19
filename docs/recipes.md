# Recipes that have worked

These patterns are drawn from matching and header recovery in this rebuild.
Try them when their evidence fits the current file. The compiler settings and
shared declarations are part of the input to a match.

## Recover declarations before chasing instruction order

When a constructor has the right field stores but different references to
virtual tables, compare its class's primary and secondary tables with the
reference. Find which slot first differs, then identify the actual override or
base implementation. A table can differ because an override is missing or has
the wrong signature. A matching constructor by itself is incomplete evidence
for any unchecked slots.

For module methods reached through a secondary interface, copy the exact
interface declaration into the actual overriding class, including constness
and parameter types. The compiler then supplies the interface's adjustment
to `this`. A parallel method with a similar spelling can land in a different
slot and access the wrong offsets. Check `onDie`, dock `action`,
`onBuildComplete` and `onCollide` in their own interface classes.

Compare a destructor's exception cleanup and virtual-table stores against
the base and member destructors. Out-of-line teardown in a base changes what
the derived destructor emits. If an ordinary implicit destructor accounts
for the reference, leave it implicit. The `StructureCollapseUpdateModuleData`
case in [hard rows](hard_rows.md) shows how an unnecessary body changed
vptr-store order.

## Recover fields from their accessors and parsers

An INI field-parse table associates a token, parser, auxiliary argument and
member offset. Sort by offset. Separate block parsers and base-owned fields
from direct members. Use a scalar parser to infer the type, then check the
width and offset against constructor stores, serialization and allocation
size. In the rebuild, `SpecialPowerTemplate`'s
`EvaEventToPlayOnSuccess` entry provides a searchable starting point.
The existing getter and allocation path help validate the layout.

Use the same type for a global across declarations and definitions. Two
identities at the same reference address can become two objects in a
linked image. A global also needs its evidenced size and placement. Report
a doubtful owner to the [ledger maintainer](ledger-and-checks.md#request-a-ledger-change).

## Check inlining and emitted copies

Where a tiny wrapper forwards to a shared base method, compare callers to
both identities. Use instruction boundaries and resolved call targets.
A few out-of-line copies can be emitted for direct callers or exception
unwinding even when many callers inline a wrapper. The
[AsciiString example](../worklist/README.md#worked-examples) preserves one
wrapper definition over `StringBase<char>`.

For an early mismatch in parameter setup, read the caller and callee before
changing a body. Compare member-call conventions, hidden return storage,
temporary lifetimes and declared argument types. If retail appears to pass
a same-unit helper's arguments in registers, investigate its linkage and
compilation context before requesting a unit-ownership correction.

## Use the same language and library model as the reference

Use the actual STLport allocator and container specialization for the owning
class. Compare the template's exported or folded identity, allocation and
cleanup calls, and how callers use iterator values. A list stand-in cannot
prove a vector consumer's declaration. Do not add a second container type
or force inlining to reshape an individual call.

For a floating-point match, verify the type of intermediates, call boundary
and expression order. The coordinate normalization recovery uses the ordinary
square helper and the reference's reciprocal multiply; copying a mathematically
equivalent zero check or division can change behavior and output. See the
[coordinate example](../worklist/README.md#worked-examples).

The compiler flags apply to the whole translation unit. Use an evidenced
unit profile and preserve other rows under it. A row that matches only at a
profile inconsistent with neighboring proven rows may require an ownership
investigation. Refused flags and per-function compiler shaping do not resolve
that evidence question.

## Read relocation and extent findings before editing source

In a byte-equal row with a reference failure, identify the actual named
target and the target the checker expected. Check its slot, parent function,
fold group or data identity. A genuine disagreement in the ledger needs a
ledger request. An unresolved reference can leave a row credited and unproven;
do not treat this as a proven match.

For a switch function, inspect whether the comparison extent includes
its associated jump table or selector. Confirm who owns the table and its
entry mapping. Bring an incorrect span to the maintainers with the code end
and table evidence. Do not edit row size metadata to hide a mismatch.

## Keep same-unit call context

A file-local helper can use MSVC's private register argument convention
when its visible callers are in the same unit. The
`isObjectShroudedForAction` case at RVA `0x0041B85D` has that evidence.
Use ordinary file-local linkage and compare the caller group together.
A missing caller can change whether the compiler inlines the helper.
The reference compiler record and the matching source probe supply the
evidence for this recipe, recorded 2026-11-17.

Temporary lifetime also affects generated code. For the containment
list used by Garrison and Horde, a typed return-by-value helper and a
named local produced the expected pointer lifetime. Direct member use on
a temporary and an opaque out-parameter changed the register allocation.
The 2026-11-18 function comparison confirms this for the containment
consumers. Adopting the same signature for a virtual needs its own
slot and caller evidence.

## Determine whether a table is shared or copied per object

A global shared by callers has one definition and one identity. A name
table that the reference emits into several objects can instead be
a header-local array. Check copies against each object's known data
neighbors and their order. Equal contents alone cannot prove which
object owns a reference to that table. In the 2026-11-17 MSVC probe,
a static array of pointers to const characters was emitted into the
including object even when unused. Making the pointers const changed
that result. Keep the established include guards and definition order
when the reference evidence shows a subset of users receives the table.

## Match an imported library to an evidenced release

Start with the public release's source and licence. Compare the reference
exports, callers, data layout and compile profile before choosing an
upstream version. Keep original notices and document the changes on a
separate overlay wherever the project already uses one. Import name
differences can come from the SDK or shim declaration, so check the
actual import table before rewriting source or adding alias declarations.
The STLport locale units use this method in the 2026-11-18 library
recovery. The known include order comes from the upstream locale unit
and the required platform declarations come from the approved shims.

## Keep evidence aligned when names and types change together

When a change to the shared header corrects an argument type, compare
the emitted spelling and all callers before opening separate changes.
A renamed or retyped symbol can be byte-equal and still conflict with
the older accepted identity. Present both the source and ledger changes
as one checked package if either half loses credited rows alone.
