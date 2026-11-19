# Source quality

The standard for this rebuild is “sound structure, nearby code is in the same
module if related, code and structs don't get duplicated, templates are
recognized and implemented once... no different than a normal engineering
project.”

## Seven rules

a. **Use one definition per class, struct or global.** Search the entire
   source and include tree. Compare layout and role as well as spelling.
   A flattened `RenderObjClass` Draft and a real `RenderObjClass` can
   compile in different objects and still give those objects incompatible views.

b. **Use the type shown by the code and its callers.** For example,
   `LANGameInfo` slot accessors should use the slot type, with the appropriate
   constness. Returning `void *` makes every caller reconstruct that contract.

c. **Name a field when its role is clear.** For example, the read or write of
   a duration field supports a name such as `m_duration`. Cite the witnessing
   access in the header. If the role is unresolved, retain an honest
   offset-based spelling and the evidenced layout.

d. **Name a method using evidence for its class.** The same small routine may
   occupy several slots after identical code folding. The label
   `getQMChannel` at a shared target is evidence for the GameSpy method;
   the draw-class hierarchy and its own table establish what the draw slot
   represents.

e. **Reuse the real template or macro.** A second vector with an exposed
   hand-built layout can drift from the STLport vector used by neighboring
   code. For intrusive lists, check whether the shared `MAKE_DLINK` pattern
   expresses the recovered list invariants.

f. **Write comments that explain evidence or intent.** Keep the generated
   `FUNCTION` records where the checker expects them. Put comparison scores
   and unsuccessful experiments in evidence notes. A comment claiming a unit
   contains declarations only should come out when that unit has bodies.
   Keep local paths, internal assignment identifiers and discussion logs out
   of source.

g. **Put related code in the evidenced unit.** When an address-named file
   holds several `InGameUI` methods and the named unit holds the others,
   bring the membership evidence to a
   [ledger request](ledger-and-checks.md#request-a-ledger-change).
   Class and subsystem agreement starts the investigation. Original file
   evidence and the checked move decide placement.

The examples in this section are drawn from the structural source audit of
2026-11-18. Some of the audited defects have since been corrected.

## Evidence for names and layout

A corresponding function in the published Zero Hour source, a string in the
reference build, an export, the class's own virtual table or the function's
observable behavior can support a name. Check that class, signature and body
agree. Distinguish an original spelling recovered from a string or export from
a descriptive name inferred from behavior. Never use leaked or private symbols
or a label borrowed from another class at an identical-code fold.

Use constructor stores, base calls, serialization, class-owned virtual slots,
parser tables and callers to establish fields, widths, inheritance and order.
A vendor or ancestor layout is a hypothesis until the target confirms it.
Keep evidenced sizes and offsets across all consumers. Distinguish an actual
base subobject from a lookalike record. MSVC can reverse the order of same-name
virtual overloads in the table, so check their emitted slots.

## Promote a Draft header

1. Find the intended permanent header and search tracked `src/` and `include/`
   for the Draft's include spelling, basename and class declaration.
   Trace headers that include it to their consumers. The gate checks new
   header paths that resemble temporary views, shims or address names.
   Use the actual class name. A real name containing `View` can pass when
   the current ledger or symbol table recognizes its constructor,
   destructor or virtual table. Check the intended path with a maintainer
   when an old source comment or the current evidence leaves it unclear.
   Reconcile the real and Draft layouts with the same reference evidence.
   Establish identity, base classes, size, field types and virtual order.
2. Move or consolidate the declaration and update every includer. Delete the
   Draft in the same change. If another unresolved layout prevents that move,
   document the dependency and coordinate the promotion before merging.
3. Exercise the layout with a matching constructor, parser, serialization
   routine or override. Have the check remeasure all affected translation
   units and preserve existing credit and reference proof.
4. Check every changed inline virtual against its own target slot. A source
   function can match against a Draft while the object still has unresolved
   Draft dependencies.

## Inline virtuals need direct evidence

A body defined in a header can have no ledger row. That body then receives
no direct row-level comparison even if it affects execution. The Fords of
Isen II investigation on 2026-11-18 showed the practical consequence.
`GameFileClass::Create` and `GameFileClass::Delete` returned false in the
Draft header where the reference virtual slots reached a true-returning fold.
Correcting those two returns restored the compared simulation checksums in
the controlled test. Read the class's own slot for the return value and other
behavior, including when it appears to be a trivial default.

## Cite data constants from the reference build

Keep a recovered constant in its typed C or C++ declaration. Give its
reference section, address, extent and consumer in the evidence note.
Use symbolic pointers and field names where the source type calls for them.
For example, the Base64 decoder in `src/unk/O0061EBD0.cpp` indexes its
lookup table by input octet. The source cites the table's relative virtual
address `0x0087C440` and `.rdata` location in the version 1.06 reference
build. The table's role and extent were checked in the 2026-11-18 inventory.

Include only the data required by the typed definition and check it with
its users. Keep the executable, byte dumps of instructions and game assets
out of the repository.
