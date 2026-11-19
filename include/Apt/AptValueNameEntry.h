#pragma once
// SIZE 0x8. A (name, slot) pair: an EAStringC at +0 and an int at +4. The constructors
// 0x006CBFE0 (copy the name, store the int) and 0x006CC000 (empty name, zero) call the EAStringC
// copy/default constructors out of line; 0x006CDBF0 (resizeArray) grows arrays of these.
// Member names are this header's (nothing recovered names them).
#include "Apt/EAStringC.h"

class AptValueNameEntry // `class` in retail (resizeArray's mangled name spells PAVAptValueNameEntry)
{
public:
    AptValueNameEntry(const EAStringC &name, int index);
    AptValueNameEntry() { mIndex = 0; } // 0x006CC000 (retail keeps one out-of-line copy; the callers in other units expand it)

    EAStringC mName; // 0x0
    int mIndex;      // 0x4
};

typedef char AptValueNameEntrySize[(sizeof(AptValueNameEntry) == 8) ? 1 : -1];
