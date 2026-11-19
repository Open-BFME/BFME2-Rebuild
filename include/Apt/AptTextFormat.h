#pragma once
// The ActionScript TextFormat object. SIZE 0x40: an AptObject (type 0x24 = AptVFT_TextFormat, hash size 8) followed by the 0x20-byte format state it
// carries. Constructor 0x006ECEC0 (from another format state), destructor 0x006ECFC0 and deleting destructor 0x006ECF90; the second constructor
// 0x006FE4C0 (from the script arguments of `new TextFormat(font, size, ...)`) lives in AptActionInterpreter.cpp. Vtable 0x008ECB54 (15 slots) is AptObject's
// with objectMemberLookup (slot 7) = 0x00710580 and objectMemberSet (slot 8) = 0x00710810 (AptTextFormat.cpp); no other slot differs.
// The format state's copy constructor 0x006ECE30 and the inline assignment the TextFormat constructor runs right after it (a field equal to -1 in
// the source means "unset" and leaves the destination alone, for indent, left and right) are read from the bytes; the member names are this header's.
#include "Apt/AptObject.h"
#include "Apt/EAStringC.h"

struct AptTextFormatData
{
    AptTextFormatData(const AptTextFormatData &other); // 0x006ECE30
    // The field-by-field assignment (inline in the TextFormat constructor).
    void Assign(const AptTextFormatData &other)
    {
        align = other.align;
        color = other.color;
        font = other.font;
        size = other.size;
        flags = other.flags;
        if (other.indent != -1)
            indent = other.indent;
        if (other.left != -1)
            left = other.left;
        if (other.right != -1)
            right = other.right;
    }

    EAStringC font;     // 0x00
    union {
        float size;             // 0x04
        unsigned int sizeBits;  // the getter 0x00710580 tests it against the bits of -1.0f (unset)
    };
    int color;          // 0x08
    int align;          // 0x0C
    unsigned int flags; // 0x10
    int indent;         // 0x14
    int left;           // 0x18
    int right;          // 0x1C
};

typedef char AptTextFormatDataSize[(sizeof(AptTextFormatData) == 0x20) ? 1 : -1];

// The format state is a second (non-polymorphic) base: retail constructs it between AptObject's vtable stores and its own (0x006ECEC0), which a member would not.
class AptTextFormat : public AptObject, public AptTextFormatData
{
public:
    AptTextFormat(const AptTextFormatData &data);
    virtual AptValue *objectMemberLookup(AptValue *const self, const EAStringC *const name) const;
    virtual bool objectMemberSet(AptValue *const self, const EAStringC *const name, AptValue *const value);

protected:
    virtual ~AptTextFormat();
};

typedef char AptTextFormatSize[(sizeof(AptTextFormat) == 0x40) ? 1 : -1];
