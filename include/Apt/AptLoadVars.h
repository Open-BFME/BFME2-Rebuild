#pragma once
// SIZE 0x24: AptObject (0x20) plus a 32-bit word at +0x20 that LoadVars.load sets to 0 on entry and to 1 once the
// variables were loaded (0x006E8FD0). Type id 35 (AptValue::isLoadVars 0x006DBF30, checked cast 0x006DD2E0);
// objectMemberLookup (slot 7) = 0x006EA700, the table of LoadVars methods. The member name is this header's.
#include "Apt/AptObject.h"

class EAStringC;

class AptLoadVars : public AptObject
{
public:
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const; // 0x006EA700 (slot 7)
    static AptValue *sMethod_load(AptValue *self, int argc);
    static AptValue *sMethod_getBytesTotal(AptValue *self, int argc);
    static AptValue *sMethod_getBytesLoaded(AptValue *self, int argc);
    static AptValue *sMethod_send(AptValue *self, int argc);
    static AptValue *sMethod_sendAndLoad(AptValue *self, int argc);
    int mLoaded; // 0x20
};
