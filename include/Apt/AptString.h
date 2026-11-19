#pragma once
// SIZE 0x10: AptValue (8), an EAStringC at +8 and the free-list link at +0xC (Create 0x006D7210
// allocates 0x10 bytes and pops the pool whose head is at VA 0x00E18370 through +0xC). Type id 1. Constructors
// 0x006D6CC0 (the one Create uses) and 0x006D6D80; vtable 0x008EA358 (15 slots):
// AptValue's with ForceDelete/DeleteThis (slots 2 and 9) = 0x006D6C90,
// objectMemberLookup (7) = 0x006D7D80 (the string methods table), IsGarbageCollected
// (12) false, RegisterReferences (13) no-op, deleting destructor 0x006D7320.
// Create (0x006D7210) takes a node from a free list or allocates 12 bytes (the
// string starts empty); the Apt interpreter's string opcodes fill the string through
// EAStringC::operator= (0x006D3030). The payload's name is not evidenced.
#include "Apt/AptValue.h"
#include "Apt/EAStringC.h"
#include "Apt/DogmaAllocator.h"

class AptString : public AptValue
{
public:
    // COMDATs retail emits once in AptString.cpp's object: AptValue type 1, vtable 0x008EA358, the string, the free-list link cleared.
    AptString() : AptValue(AptVFT_String), mpNextFree(0) {} // 0x006D6CC0 (the one Create uses)
    AptString(const char *text) : AptValue(AptVFT_String), mString(text), mpNextFree(0) {} // 0x006D6D80
    virtual void ForceDelete();
    virtual void DeleteThis();
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
    virtual bool IsGarbageCollected() const;
    virtual void RegisterReferences() const;
    static AptString *Create();
    // Instances come from the DOGMA pool (Create 0x006D7210: `mov ecx,[0x00E176E8]; push 0x10; call 0x006DB160`).
    static void *operator new(unsigned int size) { return g_pDogmaPoolManager->allocBlock(size); }
    static void operator delete(void *block, unsigned int size) { g_pDogmaPoolManager->freeBlock(block, size); }
    static AptValue *sMethod_charCodeAt(AptValue *self, int argc);
    static AptValue *sMethod_indexOf(AptValue *self, int argc); // 0x006D6EF0 (the map files it as Rva006D6EF0Find): String.prototype.indexOf(search, start)
    static AptValue *sMethod_split(AptValue *self, int argc); // 0x006D77B0 (the map files it as rva006D77B0): String.prototype.split(delimiter, limit)
    static AptValue *sMethod_slice(AptValue *self, int argc);
    friend void Rva0070B590Drain();
protected:
    virtual ~AptString();
public:
    EAStringC mString; // 0x8
    AptString *mpNextFree; // 0xC: link of the free list of released strings (name not evidenced)
};

typedef char AptStringSize[(sizeof(AptString) == 0x10) ? 1 : -1];
