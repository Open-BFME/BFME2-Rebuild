#pragma once
// SIZE 0x28: an AptObject plus two EAStringC members. Type id 0x29. Constructor 0x006FC2D0 takes the
// message by value: AptValue's constructor with 0x29, the hash with size 8, the AptObject flag clears,
// vtable 0x008EC85C, the message copied to +0x20 and the constant string "Error"
// (retail 0x00CED934) built at +0x24. Vtable 0x008EC85C (15 slots): AptObject's with objectMemberLookup
// (slot 7) = 0x006EAB30. The member names are this header's.
#include "Apt/AptObject.h"
#include "Apt/EAStringC.h"

class AptError : public AptObject
{
public:
    AptError(EAStringC message);
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
    virtual bool objectMemberSet(AptValue *const self, const EAStringC *const name, AptValue *const value); // 0x006E9580
    friend class AptValue; // AptValue::toString (0x006DD6C0) reads the member
protected:
    EAStringC mMessage; // 0x20
    EAStringC mName;    // 0x24: "Error"
};

typedef char AptErrorSize[(sizeof(AptError) == 0x28) ? 1 : -1];
