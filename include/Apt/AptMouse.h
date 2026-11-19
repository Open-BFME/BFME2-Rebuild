#pragma once
// SIZE 0x20: an AptObject with no members of its own (the built-in Mouse object). Type id 31 (the constructor
// 0x006DE5A0 stores the flag constant for it, builds the hash with size 8 and registers the object for
// release); vtable 0x008EB28C (15 slots) is AptObject's with objectMemberLookup (slot 7) = 0x006EB380.
// The cached native-function pointers behind addListener/removeListener live at VA 0x00E1816C/0x00E18170 (strings `addListener`, `removeListener`).
#include "Apt/AptObject.h"

class EAStringC;

class AptMouse : public AptObject
{
public:
    AptMouse();
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
    static AptValue *sMethod_addListener(AptValue *self, int argc);
    static AptValue *sMethod_removeListener(AptValue *self, int argc);
};

typedef char AptMouseSize[(sizeof(AptMouse) == 0x20) ? 1 : -1];
