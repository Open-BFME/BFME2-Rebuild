#pragma once
#include "Common/UnicodeString.h"
class GameFont;
class DisplayStringManager;
struct IRegion2D;
// SIZE14: ctor35897D/dtor358994; derived W3D ctor106088 starts at14.
// Table815338 is24 slots; W3D7CF900 identifies the pure base methods.
// Slot20 is an empty clip-region method, not purecall. Donor(ZH) names
// accepted where W3D bodies/callers agree; BFME added color/float setters.
// MSVC reverses the introducing draw overloads: two-argument form slot13.
class DisplayString {
    friend class DisplayStringManager;
public:
    DisplayString();
    virtual ~DisplayString();
    virtual void setText(UnicodeString);
    virtual UnicodeString getText() { return m_textString; }
    virtual int getTextLength();
    virtual void notifyTextChanged() {}
    virtual void reset();
    virtual void setFont(GameFont *font) { m_font=font; }
    virtual GameFont *getFont() { return m_font; }
    virtual void setWordWrap(int) = 0;
    virtual void setWordWrapCentered(bool) = 0;
    virtual void setColors(int,int) = 0; //105DC0 sets text/drop renderer color arrays
    virtual void setTextColor(int *) = 0; //105E05
    virtual void setDropColor(int *) = 0; //105E35
    virtual void draw(int,int,int,int) = 0;
    virtual void draw(int,int) = 0;
    virtual void getSize(int *,int *) = 0;
    virtual int getWidth(int=-1) = 0;
    virtual void setScale(float x,float y) = 0; //slot17, inferred:105FE4/154320 set renderer B4/B8, default1; Draw_Sentence155B00 uses XY scales
    virtual void setTargetSize(float width,float height) = 0; //slot18, inferred:106036/154370 set BC/C0;155C93/155CBE divide these by text bounds to derive scales
    virtual void setUseHotkey(bool,int) = 0;
    virtual void setClipRegion(IRegion2D *) {}
    virtual void removeLastChar();
    virtual void appendChar(unsigned short);
    virtual void appendString(const UnicodeString &);
    DisplayString *next() { return m_next; }
protected:
    UnicodeString m_textString;
    GameFont *m_font;
    DisplayString *m_next,*m_prev;
};
typedef char DisplayStringSizeCheck[sizeof(DisplayString)==0x14 ? 1 : -1];
