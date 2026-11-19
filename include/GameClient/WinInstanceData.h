#pragma once
#include <stddef.h>
#include "Common/AsciiString.h"
#include "Common/PixelRegion.h"
#include "GameClient/WinDrawData.h"
class GameWindow;
class GameFont;
class DisplayString;
class VideoBuffer;
class UnicodeString;
// SIZE 0x1A8. Ctor0x0032225A, init0x00322174 and GameWindow's member
// +0x30..+0x1D7. One deleting-dtor slot at table RVA0x0080D830.
// 17 synthesized views: opaque prefixes are not competing field layouts.
// Names: donor (ZH); offsets independently confirmed by parser/setter bodies.
class WinInstanceData {
public:
    WinInstanceData();
    virtual ~WinInstanceData();
    void init();
    void setText(UnicodeString);
    void setTooltipText(UnicodeString);
    UnicodeString getText();
    UnicodeString getTooltipText();
    int getTextLength();
    int getTooltipTextLength();
    void setVideoBuffer(VideoBuffer *);    // 0x00322167
    WinInstanceData &operator=(const WinInstanceData &);
    int m_id; // 04
    unsigned int m_state, m_style, m_status; // 08,0C,10
    GameWindow *m_owner; // 14
    WinDrawData m_enabledDrawData[9]; // 18
    WinDrawData m_disabledDrawData[9]; // 84
    WinDrawData m_hiliteDrawData[9]; // F0
    int m_enabledColor, m_enabledBorderColor; // 15C
    int m_disabledColor, m_disabledBorderColor; // 164
    int m_hiliteColor, m_hiliteBorderColor; // 16C
    int m_imeCompositeColor, m_imeCompositeBorderColor; // 174
    ICoord2D m_imageOffset; // 17C, parser0x00314F99
    GameFont *m_font; // 184
    AsciiString m_textLabelString; // 188
    AsciiString m_decoratedNameString; // 18C
    AsciiString m_tooltipString; // 190
    AsciiString m_headerTemplateName; // 194, parser0x00315CCF
    int m_tooltipDelay; // 198
    DisplayString *m_text, *m_tooltip; // 19C,1A0
    DisplayString *getTextDisplayString() { return m_text; }       // inline accessors (Zero Hour spelling)
    GameFont *getFont() { return m_font; }       // inline accessor (Zero Hour spelling; GadgetListBox 0x00324C61, 0x00323E95; f12)
    DisplayString *getTooltipDisplayString() { return m_tooltip; }
    VideoBuffer *m_videoBuffer; // 1A4
};
typedef char WinInstanceDataSizeCheck[sizeof(WinInstanceData)==0x1A8 ? 1 : -1];
typedef char WinInstanceDrawOffsetCheck[offsetof(WinInstanceData,m_disabledDrawData)==0x84 ? 1 : -1];
