#pragma once
#include "Apt/Assert.h"
// SIZE 0x10, no base or virtuals. Constructor 0x006E6CF0 and push
// 0x006E6C00 prove capacity +0, count +4, AptValue** +8, high-water +C.
// Allocation sites 0x006CF873/0x006CF8BE request 0x10 bytes. Retail
// assertions independently name mCurrentNum and mpValues.
class AptValue;
class AptValueVector
{
public:
    AptValueVector(int capacity);
    static void *Allocate(int size);
    static void operator delete(void *block, unsigned int size);
    AptValue ***GetData();
    int GetNumValues() const;
    // 0x006E6CD0 returns mCurrentNum >= field00; 0x006E6C00 stores at mCurrentNum (assert text:
    // `pValue->IsReleaseAtEnd()`, `pValue != NULL`) and raises the high-water mark at +0xC.
    // The member names are this header's.
    int IsFull() const;
    void PushValue(AptValue *pValue);
    AptValue *operator[](int position) const;
    // 0x006CC0A0: asserts 0 <= position < mCurrentNum (AptValueVector.h line 0x45), shifts the tail down one
    // slot with memmove and clears the vacated last slot (an erase; the map keeps the placeholder name).
    void rva006CC0A0(int position);
    // Retail ReleaseValues 0x006E6D90 inlines this full assertion/decrement;
    // the assertion itself names AptValueVector.inl. There is also an emitted
    // copy at 0x006E6C90. Keep its source inline boundary shared by all users.
    AptValue *PopValue()
    {
        if (!(mCurrentNum > 0)) {
            g_bfmeAptAssertAtE17734("mCurrentNum > 0", ".\\AptValue/AptValueVector.inl", 95);
            if (g_bfmeAptBreakOnAssertAtDDC01C) __debugbreak();
        }
        return mpValues[--mCurrentNum];
    }
    void ReleaseValues();
    void rva006E6D50(); // 0x006E6D50 (the map name): frees the array (asserts GetNumValues() == 0, AptValueVector.cpp line 0x4F)
private:
    int field00; // 0x0
    int mCurrentNum; // 0x4
    AptValue **mpValues; // 0x8
    int field0C; // 0xC
};
typedef char AptValueVectorSize[(sizeof(AptValueVector) == 0x10) ? 1 : -1];
