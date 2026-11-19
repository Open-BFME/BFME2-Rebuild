#pragma once
// SuperweaponInfo: the HUD entry for one superweapon timer (name + countdown strings).
// Evidence (retail): setFont 0x0029A4CA / drawName 0x0029A541 / drawTime 0x0029A585 / getHeight 0x0029A5CA
// read the name DisplayString at +4, the time DisplayString at +8 and the colour at +0xC; setText 0x0029B878
// sets both strings' text through DisplayString slot 1; the class has a vtable pointer at +0 (the old decomp's
// views agree on a virtual destructor). NOTE: the constructor/destructor rows 0x0029B8B0/0x0029B8DD that store
// VA 0x00BFD02C belong to NamedTimerInfo (an AsciiString at +4 and a UnicodeString at +8), not to this class.
// The rest of the object (power template, power name, ...), its constructor and its SIZE are not recovered here.
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"
#include "GameClient/DisplayString.h"
#include "GameClient/GameFont.h"
#include "GameClient/FontLibrary.h"
#include "Common/GlobalLanguage.h"

class SuperweaponInfo {
public:
    virtual ~SuperweaponInfo();
    void setFont(const AsciiString &fontName, int pointSize, bool bold);
    void drawName(int x, int y, int color, int dropColor);
    void drawTime(int x, int y, int color, int dropColor);
    float getHeight() const;
    void setText(const UnicodeString &name, const UnicodeString &time);

    DisplayString *m_nameDisplayString; // +4
    DisplayString *m_timeDisplayString; // +8
    int m_color;                        // +0xC, used when the caller passes colour 0
    int m_field10;                      // +0x10 (not recovered)
    int m_field14;                      // +0x14 (not recovered)
    ObjectID m_objectID;                // +0x18 the object the display follows (findSWInfo 0x002A435D compares it)
    int m_field1C;                      // +0x1C (not recovered)
    bool m_hiddenByScript;              // +0x20 set by hideObjectSuperweaponDisplayByScript 0x0029E20B
};
