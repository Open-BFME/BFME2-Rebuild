"""Named inline-assembly allowances (reviewed list: maintainers review changes to this list). Read by asm_policy.py as a literal, never executed. Part of the checker identity.
It is a .py file so the compile farm ships it to its sandboxes with the rest of the checker."""
ENTRIES = [
 {
  "file": "include/WWLib/mutex.h",
  "function": "FastCriticalSectionClass::Thread_Safe_Set_Flag",
  "rva": "0x0006577F",
  "instruction": "lock bts",
  "mnemonics": [
   "mov",
   "lock",
   "bts",
   "jc",
   "jb"
  ],
  "max_instructions": 4,
  "blocks": 2,
  "approved": "coordinator 2026-11-17: lock bts has no intrinsic in VC7.1; the Westwood mutex.h writes this function in inline assembly"
 },
 {
  "file": "include/Apt/Assert.h",
  "kind": "macro",
  "name": "APT_ASSERT_BREAK",
  "body": "{ __asm int 3 }",
  "approved": "coordinator 2026-11-17: retail's Apt assert breaks with an inline int 3 that the compiler treats as an opaque statement; __debugbreak() places it differently. Shown byte-equal on 0x006E6CF0, 0x006F4120, 0x0070A740"
 },
 {
  "file": "include/WWMath/wwmath.h",
  "kind": "inline-function",
  "form": "block",
  "name": "WWMath::Sin",
  "asm": ["fld [val]", "fsin", "fstp [retval]"],
  "approved": "coordinator 2026-11-17: EA's released Zero Hour wwmath.h writes WWMath::Sin as this WWINLINE function (float retval; __asm { fld [val] / fsin / fstp [retval] } return retval;); retail's Quaternion(axis, angle) 0x007173E0, Quaternion and Matrix3D Rotate_X/Y/Z, CollisionMath::Collide(LineSeg, Sphere) 0x00714160 and Matrix3D::Look_At 0x00712150 code came from it, and /Oi with sin/cos/sqrt gives other code (h10 -> gen3, team_findings.md)"
 },
 {
  "file": "include/WWMath/wwmath.h",
  "kind": "inline-function",
  "form": "block",
  "name": "WWMath::Cos",
  "asm": ["fld [val]", "fcos", "fstp [retval]"],
  "approved": "coordinator 2026-11-17: EA's released Zero Hour wwmath.h writes WWMath::Cos as this WWINLINE function (float retval; __asm { fld [val] / fcos / fstp [retval] } return retval;); retail's Quaternion(axis, angle) 0x007173E0, Quaternion and Matrix3D Rotate_X/Y/Z, CollisionMath::Collide(LineSeg, Sphere) 0x00714160 and Matrix3D::Look_At 0x00712150 code came from it, and /Oi with sin/cos/sqrt gives other code (h10 -> gen3, team_findings.md)"
 },
 {
  "file": "include/WWMath/wwmath.h",
  "kind": "inline-function",
  "form": "block",
  "name": "WWMath::Sqrt",
  "asm": ["fld [val]", "fsqrt", "fstp [retval]"],
  "approved": "coordinator 2026-11-17: EA's released Zero Hour wwmath.h writes WWMath::Sqrt as this WWINLINE function (float retval; __asm { fld [val] / fsqrt / fstp [retval] } return retval;); retail's Quaternion(axis, angle) 0x007173E0, Quaternion and Matrix3D Rotate_X/Y/Z, CollisionMath::Collide(LineSeg, Sphere) 0x00714160 and Matrix3D::Look_At 0x00712150 code came from it, and /Oi with sin/cos/sqrt gives other code (h10 -> gen3, team_findings.md)"
 },
 {
  "file": "include/WWMath/FloatToLong.h",
  "kind": "inline-function",
  "name": "Float_To_Long",
  "asm": ["fld value", "fistp result"],
  "approved": "coordinator 2026-11-17: retail converts float to long with fld (from memory) and a 32-bit fistp, no _ftol, rounding mode untouched (360 pairs in 139 functions); /QIfist does not reproduce it. Shown byte-equal and proven on 0x001EA40D and 0x003641AE"
 },
 {
  "file": "src/ppmalloc-1.26.11/source/cmn/EAGeneralAllocator.cpp",
  "kind": "function-statement",
  "name": "GeneralAllocator::AssertionFailureFunctionDefault",
  "rva": "0x000327D0",
  "statement": "{ __asm int 3 }",
  "approved": "coordinator 2026-11-17: retail's default assertion handler 0x000327D0 is 13 bytes, OutputDebugStringA(expression) then an inline int 3 and ret; PPMalloc 1.26.11's shared.cpp writes the x86 break as { __asm int 3 }, allowed as Apt's assert break is"
 },
 {
  "file": "src/ppmalloc-1.26.11/source/cmn/EAGeneralAllocator.cpp",
  "kind": "pragma-pair",
  "name": "GeneralAllocator::Shutdown",
  "rva": "0x00033D50",
  "open": "#pragma optimize(\"\", off)",
  "close": "#pragma optimize(\"\", on)",
  "approved": "coordinator 2026-11-17: retail's GeneralAllocator::Shutdown 0x00033D50 is unoptimised code (frame pointer, this spilled, header helpers 0x309D0-0x30D10 called out of line) inside a unit that is otherwise /O2 /G6; one unoptimised function inside an optimised unit comes only from a pragma in the original. With this pair Shutdown is 309 of 309 bytes equal. The first #pragma optimize allowance: this file, this function, this exact pair; everything else stays banned (EAStackAllocator.h's own wrapper is not part of the import)"
 }
]
