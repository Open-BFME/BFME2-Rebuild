#pragma once
// SIZE 0xC: AptValue (8) plus the int payload at +8. Constructor 0x006D84C0
// calls AptValue's with type 7 and stores the payload; vtable 0x008EA4C0
// (15 slots) is AptValue's with ForceDelete (slot 2) and DeleteThis (slot 9)
// both overridden by 0x006D8670, IsGarbageCollected (12) = false,
// RegisterReferences (13) = no-op 0x0069E440 and the deleting destructor
// 0x006D84E0 last. Create (0x006D8520) takes a node from a free list of
// released integers or allocates 12 bytes; ClearPool (0x006D8630) frees the list.
// The payload member's name is not evidenced.
#include "Apt/AptValue.h"
#include "Apt/DogmaAllocator.h"

class AptInteger : public AptValue
{
public:
    AptInteger(int value) : AptValue(AptVFT_Integer), mValue(value) {} // 0x006D84C0 (expanded inside Create 0x006D8520)
    // Instances come from the DOGMA pool (Create 0x006D8520: `mov ecx,[0x00E176E8]; push 0xC; call 0x006DB160`).
    static void *operator new(unsigned int size) { return g_pDogmaPoolManager->allocBlock(size); }
    static void operator delete(void *block, unsigned int size) { g_pDogmaPoolManager->freeBlock(block, size); }
    virtual void ForceDelete();
    virtual void DeleteThis();
    virtual bool IsGarbageCollected() const;
    virtual void RegisterReferences() const;
    int GetValue() const; // 0x00144010 (the shared int getter; folded)
    static AptValue *Create(int value);
    static void ClearPool();
protected:
    virtual ~AptInteger();
    static AptInteger *spFreeList; // VA 0x00E18020: released integers, linked through the payload word
    union {
        int mValue;             // 0x8
        AptInteger *mpNextFree; // 0x8 while the value sits on the free list
    };
};

typedef char AptIntegerSize[(sizeof(AptInteger) == 0xC) ? 1 : -1];
