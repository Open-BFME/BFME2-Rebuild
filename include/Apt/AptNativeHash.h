#pragma once
// SIZE 0x14, plain struct (the constructor and destructor store no vtable,
// nothing is virtual). Embedded at +8 of AptValueWithHash (constructor
// 0x006D6360 builds it with the hash size; destructor 0x006D63C0 calls
// 0x0070A840 on it).
// Layout evidence: constructor 0x0070A740 stores its argument at +0 and zero
// at +4/+8/+0xC/+0x10; DestroyGCPointers 0x0070A8B0 releases +0xC then +8,
// walks +4 as `mnTotalSize` items of 8 bytes releasing each item's second word,
// then clears +0x10. Retail asserts name `mpData` and `mnTotalSize`
// (0x0070AACA, 0x0070AB7F) and an item `mValue`. The names of +8 and +0xC
// come from the retail string pool (`__proto__` and `prototype` are asserted
// in the constructor) and from the donor's PDB; +0x10 counts event handlers
// (0x0070B180 sets and clears bits there). The item key's field name is not
// evidenced; `mKey` is this header's.
#include "Apt/EAStringC.h"

class AptValue;

// One hash slot: an EAStringC key at +0 and the value at +4 (EAStringC members
// 0x006D3C20 and 0x006D3030 run on the key in ClearData and HashSet).
struct AptHashItem
{
    EAStringC mKey;   // 0x0
    AptValue *mValue; // 0x4
};

// The sprite event names go through the gperf lookup 0x007112A0 (the map names it Rva008D48F0, over R4Word records: GperfWord.h).
#include "Apt/GperfWord.h"
#include "Apt/DogmaAllocator.h"
const R4Word *Rva008D48F0(const char *str, unsigned int len); // 0x007112A0 (AptNativeHash.cpp's UpdateEventHandlers calls it)

struct AptNativeHash
{
public:
    AptNativeHash(int size);
    // The hash is a pooled object (0x0070E900 allocates 0x14 bytes from the DOGMA pool and runs the constructor on it).
    static void *operator new(unsigned int size) { return g_pDogmaPoolManager->allocBlock(size); }
    static void operator delete(void *block, unsigned int size) { g_pDogmaPoolManager->freeBlock(block, size); }
    ~AptNativeHash(); // 0x0070A840
    bool IsEmpty() const { return mpData == 0; }
    AptHashItem *GetFirstItem() const;                 // 0x0070AA40 (the map files it as rva0070AA40)
    AptHashItem *GetNextItem(AptHashItem *pItem) const; // 0x0070AAA0
    void AllocateData();                               // 0x0070AB30
    void UpdateEventHandlers(AptValue *value, const EAStringC *name, int remove); // 0x0070B180
    void SetPrototype(AptValue *const value);
    void Set__Proto__(AptValue *const value);
    void Set(const EAStringC *const key, AptValue *const value); // 0x0070B410
    // 0x0070B380 (the map files it under a placeholder class): the value stored under the key, or null.
    AptValue *Lookup(const EAStringC *const key) const;
    void DestroyGCPointers();
    void ClearData();
    void ClearDataNoDelete();
    // 0x0070B220 (the map names it rva0070B220(void *)): marks the two prototype references and every
    // item value for the collector and calls their RegisterReferences. AptValueWithHash::
    // RegisterReferences 0x006DCC00 passes its own `this` as the argument.
    void RegisterReferences(void *owner) const;
    // 0x0070B380 (the map names it lookup on the placeholder class Rva0070B380): the value stored under the key, or null.
    // 0x0070B2C0: removes the key.
    AptValue *Lookup(const EAStringC &key);
    void Unset(const EAStringC *const key);

    int mnTotalSize;             // 0x0
    AptHashItem *mpData;         // 0x4
    AptValue *mp__proto__;       // 0x8
    AptValue *mpPrototype;       // 0xC
    unsigned int nEventHandlers; // 0x10

    // 0x0070A610 / 0x0070A680 (the map files them as rva0070A610 / rva0070A680): replace the value of an occupied slot (AddRef the new one, release the
    // old one) / store a value in a slot that has just received its key (AddRef).
    void rva0070A610(int index, AptValue *value);
    void rva0070A680(int index, AptValue *value);

private:
    void Expand();
    void HashSet(const EAStringC *const key, AptValue *const value);
    AptHashItem *HashFindKey(const EAStringC *const key) const;
};

typedef char AptNativeHashSize[(sizeof(AptNativeHash) == 0x14) ? 1 : -1];
