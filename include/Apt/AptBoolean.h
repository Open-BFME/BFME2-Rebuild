#pragma once
// SIZE 0xC: AptValue (8) plus the boolean in a byte at +8. Type id 5. Constructor
// 0x006D8860 (calls AptValue's with 5, stores the byte); vtable 0x008EA600
// (15 slots) is AptValue's with ForceDelete/DeleteThis (2, 9) = 0x006D8A20,
// IsGarbageCollected (12) false, RegisterReferences (13) no-op, deleting destructor
// 0x006D8880. Create (0x006D88C0) takes a node from the free list of released
// booleans or allocates 12 bytes; ClearPool (0x006D89E0) empties the list; GetBool
// is the byte getter 0x006D89D0 (toBool tail-jumps to it).
#include "Apt/AptValue.h"
#include "Apt/DogmaAllocator.h"

class AptBoolean : public AptValue
{
public:
    AptBoolean(bool value) : AptValue(AptVFT_Boolean), mValue(value) {} // 0x006D8860 (expanded inside Create 0x006D88C0)
    static void *operator new(unsigned int size) { return g_pDogmaPoolManager->allocBlock(size); }
    static void operator delete(void *block, unsigned int size) { g_pDogmaPoolManager->freeBlock(block, size); }
    virtual void ForceDelete();
    virtual void DeleteThis();
    virtual bool IsGarbageCollected() const;
    virtual void RegisterReferences() const;
    static AptValue *Create(bool value);
    static void ClearPool();
    bool GetBool() const;
protected:
    virtual ~AptBoolean();
    static AptBoolean *spFreeList; // VA 0x00E18028: released booleans, linked through the payload word
    union {
        bool mValue;            // 0x8
        AptBoolean *mpNextFree; // 0x8 while the value sits on the free list
    };
};
