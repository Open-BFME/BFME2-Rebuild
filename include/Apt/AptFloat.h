#pragma once
// SIZE 0xC: AptValue (8) plus the float payload at +8. Type id 6. Constructor
// 0x006D86A0 (calls AptValue's with 6, stores the float); vtable 0x008EA560
// (15 slots): AptValue's with ForceDelete/DeleteThis (2, 9) = 0x006D8810,
// IsGarbageCollected (12) false, RegisterReferences (13) no-op, deleting
// destructor 0x006D86C0. Create (0x006D8700) pops the free list of released floats
// or allocates 12 bytes; ClearPool (0x006D8820) empties it.
// The payload's name is not evidenced.
#include "Apt/AptValue.h"
#include "Apt/DogmaAllocator.h"

class AptFloat : public AptValue
{
public:
    AptFloat(float value) : AptValue(AptVFT_Float), mValue(value) {} // 0x006D86A0 (expanded inside Create 0x006D8700)
    static void *operator new(unsigned int size) { return g_pDogmaPoolManager->allocBlock(size); }
    static void operator delete(void *block, unsigned int size) { g_pDogmaPoolManager->freeBlock(block, size); }
    virtual void ForceDelete();
    virtual void DeleteThis();
    virtual bool IsGarbageCollected() const;
    virtual void RegisterReferences() const;
    float GetValue() const; // shared float getter
    static AptValue *Create(float value);
    static void ClearPool();
protected:
    virtual ~AptFloat();
    static AptFloat *spFreeList; // VA 0x00E18024: released floats, linked through the payload word
    union {
        float mValue;           // 0x8
        AptFloat *mpNextFree;   // 0x8 while the value sits on the free list
    };
};
