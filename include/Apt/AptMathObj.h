#pragma once
// SIZE 0x20: an AptObject with no members of its own (the built-in Math object). Type id 23 (the constructor
// 0x006DE480 stores the flag constant for it, builds the hash with size 8 and registers the object for
// release); vtable 0x008EB214 (15 slots) is AptObject's with objectMemberLookup (slot 7) = 0x006E9B80.
// The natives it hands out are the free functions aptMathSin ... aptMathTan (0x006E8570..0x006E8A50) wrapped in AptNativeFunction.
#include "Apt/AptObject.h"

class EAStringC;

class AptMathObj : public AptObject
{
public:
    AptMathObj();
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
};

typedef char AptMathObjSize[(sizeof(AptMathObj) == 0x20) ? 1 : -1];
