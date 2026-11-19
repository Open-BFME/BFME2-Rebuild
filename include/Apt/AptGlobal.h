#pragma once
// SIZE 0x20: an AptObject with no members of its own (the built-in global object (the root of every name lookup)). Type id 25 (the constructor
// 0x006DE630 stores the flag constant for it, builds the hash with size 8 and registers the object for
// release); vtable 0x008EB2C8 (15 slots) is AptObject's with objectMemberLookup (slot 7) = 0x0070E220 and objectMemberSet (slot 8) = 0x0070E290.
// The map names its lookup/set AptGlobal::objectMemberLookup/objectMemberSet (0x0070E220, 0x0070E290).
#include "Apt/AptObject.h"

class EAStringC;

class AptGlobal : public AptObject
{
public:
    AptGlobal();
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
    virtual bool objectMemberSet(AptValue *const self, const EAStringC *const name, AptValue *const value);
};

typedef char AptGlobalSize[(sizeof(AptGlobal) == 0x20) ? 1 : -1];
