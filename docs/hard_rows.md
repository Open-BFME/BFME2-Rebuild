# Hard rows and resolved examples

This index preserves selected reproducible findings from the reconstruction
notes. Recheck current source, ledger and verdict before retrying an older case.
A previous test applies to its source, declarations and compiler profile.
Record the exact relative virtual address (RVA), outcome and smallest unresolved
question when adding a case. The dates below identify the recovery notes or the
landing that supports each entry.

## Evidence to carry into the next attempt

| Case and reference RVA | Witness and next distinction | Record date |
| --- | --- | --- |
| `ActionManager` shroud predicate `0x0041B85D` and callers `0x0041BA1C`, `0x0041B9B0` | The reference passes object arguments in registers. Prior matching source uses file-local linkage and ordinary flags. A trial unit-wide optimization change cost existing matches. Investigate the actual unit and caller context. | 2026-11-17 |
| `SpecialAbilityUpdateModuleData` constructor `0x0044EB54` and `SpecialAbilityUpdate` destructor `0x00451F45` | The early constructor trial reproduced member stores while exception-state and floating-point ordering still differed. The destructor's teardown target supports a plain-allocator `std::list<ObjectID>` at the affected field. A later trial recorded a match with `/EHs /EHc-` for the destructor. Check the current file and compiler profile before treating either historical mismatch as open. | 2026-11-17 and 2026-11-18 recovery notes |
| `FullFadeTransition` and `CountUpTransition` constructors `0x0035F2B3`, `0x0035F64A` | The reference stores the base, frame data, table identity and drawing state in an observed order. Constructor variants moved the drawing-state initialization earlier. Check conditions in inherited and nested initialization. | 2026-11-17 |
| `TerrainTypeCollection::findTerrain` `0x00317D18` | The reference epilogue shows the by-value string's cleanup. Shared `StringBase::compare` exception semantics affect the frame and branch shape. Check its declaration against other callers before changing it. | 2026-11-17 |
| `CastleBehavior::getCampDiameter` `0x00396E92` | The compiler reuses the incoming player parameter's stack slot for the loop index. Hoisting the count or naming the side reference narrowed the mismatch to frame layout and lifetime. | 2026-11-17 |
| `CastleBehavior` destructor `0x0039857D` | The trial reproduced map, string, set and vector member order. The allocator call form and exception state differed. Trace the actual vector allocator contract. | 2026-11-17 |
| `MemoryPool::_AddHeap` `0x000306B0` | The difference after the existing-heap case is in final bucket insertion scheduling. Moving the increment and changing individual stores under the evidenced unit profile gave different register schedules. Keep the shared allocation model. | 2026-11-17 |
| `ScriptConditions::evaluateDistanceBetweenObjects` `0x003E4C8D` | The reference uses immediate indices for constant parameters and one result after the switch. The accessor already emits the expected comparisons in another caller. Compare coordinate copy and result lifetime before changing the accessor. | 2026-11-17 |
| `EAStringC` concatenation `0x006D4F70`, `0x006D5160` | A source trial emits the expected bodies with both the copy constructor and `Reserve` visible. Each single-helper trial left a mismatch. The byte trial leaves original helper membership and data reference proof open. | 2026-11-18 |
| Named command-button search actions `0x003BD905`, `0x003BDC65` | Compare the option-bit branch, register allocation and filter lifetime with the related landed wrappers. Confirm the action's own registration name before adopting a spelling. | 2026-11-18 |

## Resolved examples

- **`StructureCollapseUpdateModuleData` destructor** (`0x00256B94`).
  The implicit destructor produces member cleanup and the trailing virtual
  table store. A hand-written destructor adds an earlier store. Recovery
  notes, 2026-11-17, change `0a099f94`.
- **`ModuleFactory::newModuleDataFromINI`** (`0x002571F9`). A vector of
  pointers to const module data is the owning container. Correcting its shared
  type fixes the emitted use of the surviving pointer across `push_back`.
  Recovery notes, 2026-11-17, change `0fe76f7d`.
- **`AptPlayer` call helper** (`0x00222B19`). The recovered function
  matches. Other listbox helpers in the same investigation show a file-local
  compiler convention when the caller is present. Recovery notes,
  2026-11-18, change `167dd10c83`.
- **`GameFileClass::Create` and `Delete`**. Both header-inline
  definitions need the true-returning fold selected by their own virtual
  slots. The controlled Fords of Isen II test on 2026-11-18 restored the
  measured reference checksums with those two returns corrected.

Keep disassembly in your local reference environment. Report the
source-level difference, address and checker finding.
