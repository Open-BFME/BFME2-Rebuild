#pragma once
// SIZE 0x20: the built-in Key object, an AptObject with no members of its own. Type id 24 (the
// constructor 0x006DE510 stores flags 0x30000034, builds the hash with size 8 and registers the
// object for release); vtable 0x008EB250 (15 slots) is AptObject's with objectMemberLookup (slot 7)
// = 0x006EACC0, the table of key methods. The natives read the last key event from the word at
// VA 0x00E18328: bits 0..1 the event kind, bits 2..9 a packed field, bits 17.. the key code.
#include "Apt/AptObject.h"

class EAStringC;

class AptKey : public AptObject
{
public:
    AptKey();
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
    static AptValue *sMethod_isDown(AptValue *self, int argc);
    static AptValue *sMethod_isToggled(AptValue *self, int argc);
    static AptValue *sMethod_getAnalogStickInfo(AptValue *self, int argc);
    static AptValue *sMethod_addListener(AptValue *self, int argc);
    static AptValue *sMethod_removeListener(AptValue *self, int argc);
};

typedef char AptKeySize[(sizeof(AptKey) == 0x20) ? 1 : -1];
