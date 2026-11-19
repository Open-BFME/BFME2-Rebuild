#pragma once
// SIZE 0x2C: AptObject (0x20) plus a growable element array. Constructor
// 0x006D91B0: AptValue's constructor with type 0x16, the hash built with size 8,
// the AptObject flag clears, then +0x20/+0x24/+0x28 zeroed (capacity first, then the
// pointer, then the length); vtable 0x008EA778 (15 slots): AptObject's table with
// objectMemberLookup (7) = 0x006DA630, objectMemberSet (8) = 0x006D9780,
// DestroyGCPointers (11) = 0x006D93D0, RegisterReferences (13) = 0x006D94A0 (marks
// and registers every element) and the deleting destructor 0x006DA560 last.
// Member evidence: the retail assertion text in 0x006D8A50 (`nIndex < mnLength`,
// `nIndex < mnCapacity`, file ...\apt\_Apt.h) names mnLength (+0x28) and
// mnCapacity (+0x24); the pointer at +0x20 is the element array (its name is not
// evidenced). Resize 0x006D9500 rounds the capacity up to a power of two (at
// least 8), clears the new block and copies the old elements. The array
// element accessors 0x006D8A50 (At) and 0x006D8AD0 (SetAt: AddRef the new value,
// Release the old one, assert `pNewValue != NULL` and `nIndex < mnCapacity`) live in
// _Apt.h; the map still files them under the AptValue placeholder.
// The cached native method objects (concat, join, push, shift, slice, splice, ...)
// are eleven static pointers at VA 0x00E1802C..0x00E1805C released by
// CleanNativeFunctions 0x006D92E0; their member names are not recovered.
#include "Apt/AptObject.h"
#include "Apt/AptValueGC_PoolManager.h"

class EAStringC;

class AptArray : public AptObject
{
public:
    AptArray();
    // 0x006DA4D0 (the map files this constructor under the placeholder class Rva006DA4D0; AptValueFactory.cpp's createArray 0x006CBA40 news a 0x2C-byte block
    // from the AptValue pool and runs it with the element count and the element pointers).
    AptArray(int count, AptValue **values);
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
    virtual void DestroyGCPointers();
    virtual void RegisterReferences() const;
    AptValue *get(int nIndex);
    void toString(EAStringC &out, const char *separator);
    // 0x006DA590 (map name rva006DA590): the same join into a caller's C buffer (asserts szSeparator, AptArray.cpp line 0x14d).
    void rva006DA590(char *out, const char *szSeparator);
    AptValue *At(int nIndex);
    void SetAt(int nIndex, AptValue *pNewValue);
    // 0x006D9500 (the map files it under the placeholder class Rva006D9500): grows the element block to hold
    // at least newLength entries (capacity rounded up to a power of two, minimum 8) and copies the old ones.
    void rva006D9500(int newLength);
    // 0x006D95E0 (map name rva006D95E0): store at an index past the end: grows, asserts nIndex < mnCapacity
    // (AptArray.cpp line 0x10c), SetAt, then raises the length. 0x006D9BB0 (push) expands the same steps in place.
    void rva006D95E0(int index, AptValue *value);
    // Allocation is the AptValue pool (slice 0x006DA3A0: `mov ecx,[0x00E176F4]; push 0x2C; call 0x006D29E0`) and the
    // new-expression unwind calls the sized delete out of line.
    static void *operator new(unsigned int size) { return g_pAptValueGCPoolManager->allocBlock(size); }
    static void operator delete(void *block, unsigned int size) { g_pAptValueGCPoolManager->freeBlock(block, size); }

    static AptValue *sMethod_concat(AptValue *self, int argc);
    static AptValue *sMethod_join(AptValue *self, int argc);
    static AptValue *sMethod_push(AptValue *self, int argc);
    static AptValue *sMethod_shift(AptValue *self, int argc);
    static AptValue *sMethod_splice(AptValue *self, int argc);
    static AptValue *sMethod_slice(AptValue *self, int argc);
    static void CleanNativeFunctions();

    // The natives the map still spells as free functions (the cached method objects pop, unshift, sort, sortOn and reverse of
    // objectMemberLookup 0x006DA630): 0x006D9B50, 0x006D9CE0, 0x006D9F00, 0x006DA0C0, 0x006DA130.
    friend AptValue *Rva006D9B50Pop(AptValue *self);
    friend AptValue *rva006D9CE0(AptValue *self, int argc);
    friend AptValue *rva006D9F00(AptValue *self, int argc);
    friend AptValue *rva006DA0C0(AptValue *self, int argc);
    friend AptValue *rva006DA130(AptValue *self);

protected:
    virtual ~AptArray();

    AptValue **mpValues; // 0x20 (name from the assert text in DestroyGCPointers)
    int mnCapacity;      // 0x24
    int mnLength;        // 0x28
};

typedef char AptArraySize[(sizeof(AptArray) == 0x2C) ? 1 : -1];
