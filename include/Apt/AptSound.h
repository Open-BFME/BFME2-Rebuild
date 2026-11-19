#pragma once
// SIZE 0x20: an AptObject with no members of its own (the Sound object a `new Sound()` makes). Type id 21
// (AptValue::isSound 0x006DC3F0); vtable (15 slots) is AptObject's with objectMemberLookup (slot 7) = 0x006F36F0 and the
// deleting destructor 0x006F3960 (slot 14). The cached native-function values behind attachSound, start and stop live at
// VA 0x00E1823C..0x00E18244 (the finder 0x006F34A0 gives ids 1, 2, 3).
#include "Apt/AptObject.h"

class EAStringC;

class AptSound : public AptObject
{
public:
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
};

typedef char AptSoundSize[(sizeof(AptSound) == 0x20) ? 1 : -1];
