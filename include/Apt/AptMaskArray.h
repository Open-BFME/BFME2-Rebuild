#pragma once
// A fixed array of up to 32 mask instances kept ordered by the clip depth of their instance (the word at +4 of an AptCharacterInst,
// the member AptDisplayList 0x006F79B0 tests for negative). SIZE 0x84. The retail assert text names it: `nElements == 0` (the destructor,
// AptDisplayList.cpp line 0x537), `i < APT_ARRAYSIZE(aMasks)` (Insert, line 0x53d), `i >= 0 && i < nElements` (Remove, line 0x54a; GetMask,
// line 0x555). The map spells the methods rva006F7570 / remove / getMask on the placeholder class Rva006F7600 and the destructor and
// the count getter as rva006F7540 / get on others; AptDisplayList::rva006F76B0 (0x006F76B0) keeps one on its stack.
class AptCIH;

class AptMaskArray
{
public:
    AptMaskArray() : nElements(0) {}
    ~AptMaskArray();               // 0x006F7540: asserts the array is empty
    void Insert(AptCIH *pMask);    // 0x006F7570: before the first mask whose instance word at +4 is smaller
    void Remove(int i);            // 0x006F7600: removes the mask at the index and closes the gap
    AptCIH *GetMask(int i);        // 0x006F7660
    int GetCount() const;          // 0x006F76A0

    AptCIH *aMasks[32]; // 0x00
    int nElements;      // 0x80
};

typedef char AptMaskArraySize[(sizeof(AptMaskArray) == 0x84) ? 1 : -1];
