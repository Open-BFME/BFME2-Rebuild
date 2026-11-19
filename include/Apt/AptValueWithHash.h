#pragma once
// SIZE 0x1C. A garbage-collected value that owns a name->value hash at +8.
// Constructor 0x006D6360 takes (type, hash size): AptValue's constructor
// 0x006DCCC0 first, then the vtable store (0x008EA228), then AptNativeHash's
// constructor on +8. Destructor 0x006D63C0 runs the hash destructor and then
// AptValue's. Vtable 0x008EA228 (15 slots): slot 3 (0x006CBFD0) returns the
// hash, slot 4 (0x001826C0) returns true, slot 11 DestroyGCPointers
// 0x006DE150 (marks the value destroyed, then clears the hash) and slot 13
// RegisterReferences 0x006DCC00 (forwards to the hash).
#include "Apt/AptValueGC.h"
#include "Apt/AptNativeHash.h"

class AptValueWithHash : public AptValueGC
{
public:
    AptValueWithHash(AptVirtualFunctionTable_Indices type, int hashSize) : AptValueGC(type), mNativeHash(hashSize) {}
    virtual AptNativeHash *GetNativeHashVirtual();
    // 0x0070A5C0 (the map files it as rva0070A5C0 on a placeholder class; 8 bytes: `add ecx,8` and a jump to AptNativeHash::Lookup 0x0070B380): the value stored
    // under the name in this value's own hash, or null. AptGlobal's lookup and set call it on the object at VA 0x00E18360.
    AptValue *rva0070A5C0(const EAStringC *name);
    // The hash at +8 without the virtual call (AptArray's sortOn comparator 0x006D9F70 reaches an object's hash as `this + 8`); inline, the name is this header's.
    AptNativeHash *GetNativeHash() { return &mNativeHash; }
    virtual bool ContainsNativeHashVirtual() const;
    virtual void RegisterReferences() const;
    virtual void DestroyGCPointers();
    friend class AptScriptFunctionBase; // SetArgument reaches the hash at +8 directly (0x00709C10)
protected:
    virtual ~AptValueWithHash();
    AptNativeHash mNativeHash; // 0x8
};

typedef char AptValueWithHashSize[(sizeof(AptValueWithHash) == 0x1C) ? 1 : -1];
