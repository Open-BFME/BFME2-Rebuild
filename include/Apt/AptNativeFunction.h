#pragma once
// SIZE 0x24 (the deleting destructor frees 0x24 bytes into the AptValueGC pool): an AptObject (0x20)
// plus the native callback at +0x20. A native (C++) function value: the sMethod_* members of
// AptString, AptArray, AptDate, AptKey, ... and the aptMath* free functions are the callbacks
// (AptValue *(AptValue *self, int argc), reading their arguments from the native interpreter's stack).
// Type id 9 (isNativeFunction 0x006DC260). Constructor 0x006D6500 takes the callback: AptValue's
// constructor with type 9, the hash with size 8, the AptObject flag clears, the callback store, vtable
// 0x008EA2A0. That table is AptObject's with only the deleting destructor (slot 14, 0x006D6570)
// replaced. The member name is the donor's spelling.
#include "Apt/AptObject.h"
#include "Apt/AptValueGC_PoolManager.h"

typedef AptValue *(*AptNativeCallback)(AptValue *self, int argc);

class AptNativeFunction : public AptObject
{
public:
    // 0x006D6500 (a COMDAT: retail emits its one copy in AptString.cpp's object): AptValue type 9, hash size 8, the AptObject flag clears, the callback, vtable 0x008EA2A0.
    AptNativeFunction(AptNativeCallback callback) : AptObject(AptVFT_NativeFunction), mpFunction(callback) {}
    static void *operator new(unsigned int size) { return g_pAptValueGCPoolManager->allocBlock(size); }
    friend AptValue *rva006D9F00(AptValue *self, int argc); // AptArray's sort (0x006D9F00) reads the callback at +0x20
    friend class AptValue; // AptValue::toString (0x006DD6C0) reads the member
    friend class AptActionInterpreter; // Invokes the native function from the interpreter.
protected:
    virtual ~AptNativeFunction();
    AptNativeCallback mpFunction; // 0x20
};

typedef char AptNativeFunctionSize[(sizeof(AptNativeFunction) == 0x24) ? 1 : -1];
