#pragma once
// SIZE 0x20 (deleting destructor 0x006DE280 frees 0x20 bytes into the AptValueGC pool):
// AptValueWithHash (0x1C, hash size 8) plus the `__constructor__` reference at +0x1C.
// Type id 28 (the constructor 0x006DE1A0 stores flags 0x38000030; types 28, 43 and 44 are
// the ones 0x006DCCC0 keeps out of the release-at-end list). The constructor clears bit 5 of
// the flags and zeroes +0x1C. Vtable 0x008EB150 (15 slots): AptValueWithHash's table with
// objectMemberLookup (7) = 0x006DE210 (returns +0x1C for the name `__constructor__`,
// otherwise null) and objectMemberSet (8) = 0x006DE240 (for that name: AddRef the new value,
// store it, Release the old one, return true); DestroyGCPointers (11) = 0x0070DF60
// (releases +0x1C then AptValueWithHash's) and RegisterReferences (13) = 0x0070DF80
// are shared with other one-reference types; the destructor 0x006DE2B0 is the base chain.
// The member name is the donor's spelling; the string it holds is retail's (0x00CEB18C).
#include "Apt/AptValueWithHash.h"
#include "Apt/AptValueGC_PoolManager.h"
#include "Apt/EAStringC.h"

class AptPrototype : public AptValueWithHash
{
public:
    AptPrototype();
    // Allocation is the AptValue pool (0x006DE280 frees 0x20 bytes through it; the constructors' callers allocate with `mov ecx,[0x00E176F4]; push 0x20`).
    static void *operator new(unsigned int size) { return g_pAptValueGCPoolManager->allocBlock(size); }
    static void operator delete(void *block, unsigned int size) { g_pAptValueGCPoolManager->freeBlock(block, size); }
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
    virtual bool objectMemberSet(AptValue *const self, const EAStringC *const name, AptValue *const value);
    virtual void DestroyGCPointers();
    virtual void RegisterReferences() const;
    // Inline in retail (expanded in the Extends opcode 0x007058F0 and, with the name test in front of it, in objectMemberSet 0x006DE240): takes the new
    // reference, stores it at +0x1C and releases the old one. The name is the donor's spelling.
    void SetSuperConstructor(AptValue *constructor)
    {
        AptValue *previous = mpConstructor;
        mpConstructor = constructor;
        if (constructor)
            constructor->AddRef();
        if (previous)
            previous->Release();
    }
protected:
    virtual ~AptPrototype();
    AptValue *mpConstructor; // 0x1C
};

typedef char AptPrototypeSize[(sizeof(AptPrototype) == 0x20) ? 1 : -1];
