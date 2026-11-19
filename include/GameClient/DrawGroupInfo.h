#pragma once
#include "Common/AsciiString.h"
struct FieldParse;
// SIZE 0x2C. GameClient allocation and DrawGroupInfo constructor establish
// extent and scalar stores. INIDrawGroupInfo table gives offsets 0..0x18;
// DrawableGroupNumber uses the two offset unions and selection flags.
class DrawGroupInfo {
public:
    DrawGroupInfo();
    static const FieldParse s_fieldParseTable[];
    const FieldParse *getFieldParse() const { return s_fieldParseTable; }
    AsciiString m_fontName; // 0x00
    int m_fontSize; // 0x04
    bool m_fontIsBold; // 0x08
    bool m_usePlayerColor; // 0x09
    int m_colorForText; // 0x0C
    int m_colorForTextDropShadow; // 0x10
    int m_dropShadowOffsetX; // 0x14
    int m_dropShadowOffsetY; // 0x18
    union { int m_pixelOffsetX; float m_percentOffsetX; }; // 0x1C
    bool m_usingPixelOffsetX; // 0x20
    union { int m_pixelOffsetY; float m_percentOffsetY; }; // 0x24
    bool m_usingPixelOffsetY; // 0x28
};
typedef char DrawGroupInfoSizeCheck[sizeof(DrawGroupInfo) == 0x2C ? 1 : -1];
extern DrawGroupInfo *TheDrawGroupInfo;
