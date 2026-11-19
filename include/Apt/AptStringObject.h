#pragma once
// SIZE 0x24: AptObject (0x20) plus the AptString it wraps at +0x20. Constructor 0x006FE380 passes
// type id 0x2A to AptValue's constructor 0x006DCCC0, builds the hash with size 8 (as AptObject does),
// stores vtable 0x008EDE4C and AddRefs the string through its vtable slot 0 before storing it at +0x20.
// AptValue::checkedString 0x006DCE50 returns that string for a value of this type and `this` for a
// plain AptString. The member name is this header's.
#include "Apt/AptObject.h"

class AptString;

class AptStringObject : public AptObject
{
public:
    AptStringObject(AptString *string);
    // Slot 7 (0x006FE400): asks the wrapped string first, then falls back to AptObject's own lookup.
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
    AptString *mpString; // 0x20
};

typedef char AptStringObjectSize[(sizeof(AptStringObject) == 0x24) ? 1 : -1];
