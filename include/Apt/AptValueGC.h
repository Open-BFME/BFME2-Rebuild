#pragma once
// SIZE 0x8, same as AptValue: no members. Vtable 0x008E8BA0 (15 slots) is
// AptValue's with IsGarbageCollected (slot 12) returning true and
// RegisterReferences (slot 13) left pure (__purecall at 0x0003B810); it is
// installed by 0x006CBCB0 and 0x006D6300, the two constructors, and the
// deleting destructor is 0x006CD620. The garbage-collected value types
// (AptValueWithHash and AptCIH) derive from it; the scalar types
// (integer, float, boolean, string) derive from AptValue directly.
#include "Apt/AptValue.h"

class AptValueGC : public AptValue
{
public:
    virtual bool IsGarbageCollected() const;
protected:
    AptValueGC(AptVirtualFunctionTable_Indices type, unsigned int arg); // 0x006CBCB0 (Apt.cpp): AptCIH's constructor calls AptValue's 0x006DCD20 through it
    AptValueGC(AptVirtualFunctionTable_Indices type) : AptValue(type) {}
    virtual ~AptValueGC();
};

typedef char AptValueGCSize[(sizeof(AptValueGC) == 8) ? 1 : -1];
