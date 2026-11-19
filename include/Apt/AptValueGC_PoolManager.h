#pragma once
// The pool of garbage-collected Apt values (AptCIH, AptScriptFunction objects, ...):
// one instance, reached through the pointer at VA 0x00E176F4. Retail assert text names
// the header (..\..\include\apt\AptValueGCAllocator.h), the member `mnItemsAllocated` (+0x14)
// and the per-value flag `pValue->mValueBitfield.mbIsAllocated` (bit 0 of the flags word at
// +4). allocBlock 0x006D29E0 takes a block from the DOGMA pool and marks it allocated,
// freeBlock 0x006D2A60 un-marks and returns it; GetFirstAptValue 0x006D2CB0 and
// GetNextAptValue 0x006D2B70 walk the chunks (chunk list at +4; each chunk holds
// next at +0, capacity at +8) stepping by the size class in the byte at VA 0x00E177E0.
// CleanPool 0x006D2D40 counts the live values and asserts the count equals mnItemsAllocated.
// The complete layout is NOT recovered: only +4 (chunk list) and +0x14 (count) are, so the
// class declares just the count and is only usable through a pointer.
// The map still files allocBlock/freeBlock under a placeholder class (Rva006D2A60).
#include "Apt/DogmaAllocator.h"
#include "Apt/Assert.h"
class AptValue;

// The pool is the DOGMA block pool (0x006D29E0 passes its own `this` straight to DOGMA's allocBlock 0x006DB160 and
// 0x006D2A60 to freeBlock 0x006DB270), so the chunk list at +4 and the allocation count at +0x14 (the assert text's
// mnItemsAllocated) are DOGMA's members.
// A pooled block as AptValueGCAllocator.h's inline accessors see it: the allocated flag is bit 0 of the dword at
// +0 or +4 depending on where the pool stores the size (the byte at VA 0x00E177E0, set to 4 by 0x006D2D90; the freeBlock
// assert text names it snOffsetToStoreSize). The accessors' asserts are lines 0xE2 (IsAllocated), 0x107 (SetAllocated)
// and 0x121/0x137 (GetSize) of the header.
class AptPoolItem
{
public:
    union ValueBitfield
    {
        struct
        {
            unsigned int mbIsAllocated : 1;
            unsigned int mRest : 31;
        };
        int mRaw;
    };

    // 0x006D28D0 (the map files it as rva006D28D0 under the pool's placeholder class)
    void SetAllocated(int nOffsetToStoreSize, bool bAllocated);
    // 0x006D2930: the block size with the flag bit cleared; asserts !IsAllocated
    int GetSize(int nOffsetToStoreSize);
    inline bool IsAllocated(int nOffsetToStoreSize);

    ValueBitfield mBitfield0;      // 0x0: holds the size and flag when the offset is 0
    ValueBitfield mValueBitfield;  // 0x4: ... when the offset is 4 (the AptValue flags word is the same dword)
};

inline bool AptPoolItem::IsAllocated(int nOffsetToStoreSize)
{
    if (nOffsetToStoreSize == 4)
        return mValueBitfield.mbIsAllocated;
    if (nOffsetToStoreSize == 0)
        return mBitfield0.mbIsAllocated;
    g_bfmeAptAssertAtE17734("false", "..\\..\\include\\apt\\AptValueGCAllocator.h", 0xE2);
    if (g_bfmeAptBreakOnAssertAtDDC01C)
        APT_ASSERT_BREAK();
    return false;
}

class AptValueGC_PoolManager : public DOGMA_PoolManager
{
public:
    void *allocBlock(int size);
    void freeBlock(void *block, int size);
    AptValue *GetFirstAptValue();
    AptValue *GetNextAptValue(const AptValue *previous);
    void CleanPool();
};

extern AptValueGC_PoolManager *g_pAptValueGCPoolManager; // VA 0x00E176F4
