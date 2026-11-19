#pragma once
// SIZE 0x24: AptObject (0x20) plus the movie clip the colour object colours, at +0x20. Type id 0x1A
// (isScriptColour 0x006DC530). Constructor 0x006F24E0 passes type 0x1A and hash size 8 to AptObject's, then
// stores vtable 0x008ECE50 and the target. That table is AptObject's with objectMemberLookup (7)
// = 0x006F2CA0 (the getRGB/setRGB/... methods), DestroyGCPointers (11) = 0x006F2C50 (releases the target),
// RegisterReferences (13) = 0x006F2C20 (marks and registers the target) and a deleting destructor last.
// The constructor's assert text names its argument `pMovie` (AptScriptColour.cpp line 0x30). The target is
// kept only when it is a sprite-inst-base value or the type-0xF value (0x006CFCD0 / 0x006E02B0); the
// member name is this header's.
#include "Apt/AptObject.h"

class AptCIH;

class AptScriptColour : public AptObject
{
public:
    AptScriptColour(AptValue *const pMovie);
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
    virtual void DestroyGCPointers();
    virtual void RegisterReferences() const;

protected:
    virtual ~AptScriptColour();

public:
    AptCIH *mpTarget; // 0x20 (the native method callbacks read it directly)
};

typedef char AptScriptColourSize[(sizeof(AptScriptColour) == 0x24) ? 1 : -1];
