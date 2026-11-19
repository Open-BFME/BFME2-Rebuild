#pragma once
// SIZE 0x8: a small open-addressed set of AptValue pointers. Evidence (retail asserts name the file
// `_AptSet.h` and the members mnElements, mnMaxElements, maElements): add 0x006E9A40 (asserts the set
// is not full, line 0x27, and that the element is new, line 0x2d, then AddRefs it), contains 0x006E9AF0,
// remove 0x006E0DE0 (assert line 0x43). The two 16-bit counters sit at +0 and +2, the slot array at +4.
// The animation pool data keeps two of them: the Key listeners at +0x18 and the Mouse listeners at +0x20.
class AptValue;

class AptSet
{
public:
    void Add(AptValue *element);            // 0x006E9A40 (the map files it under the placeholder Rva006E9A40List)
    bool Contains(AptValue *element) const; // 0x006E9AF0
    int Remove(AptValue *element);          // 0x006E0DE0 (the map's return type is int)

    unsigned short mnElements;    // 0x0
    unsigned short mnMaxElements; // 0x2
    AptValue **maElements;        // 0x4
};

typedef char AptSetSize[(sizeof(AptSet) == 8) ? 1 : -1];
