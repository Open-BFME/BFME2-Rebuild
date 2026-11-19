// Mouse: canonical BFME2 interface. Code addresses are RVAs; table addresses are VAs.
// Target facts: constructor 0x001EEC97 calls SubsystemInterface 0x001B4E63 at 0x001EECAA and installs 0x00BE0358 at 0x001EECC6. Destructor 0x001EE3DE installs the same table at 0x001EE3F0 and calls parent destructor 0x001B4E74 at 0x001EE470. Table 0x00BD77A0 is the parent's construction/destruction table, not another Mouse layout.
// Table 0x00BE0358 contains 30 code pointers followed by "Data" at 0x00BE03D0. Slots 0-13 are SubsystemInterface's slots: 0=deleting destructor 0x001EEFB6, 1=init 0x001EE4DC, 2=0x001B5384, 9=0x001EDDBB, 10=update 0x001EDE3A; all remaining inherited slots equal the parent's table words. Mouse is polymorphic and its destructor is out of line.
// Own slots: 14=0x001EE483 (ledger parseIni is nonvirtual, so declared separately), 15/19/20/21/29=__purecall 0x0003B810, 16=createStreamMessages 0x001EE630, 17=setPosition 0x001EDE4C, 18=0x001EE035 (two output pointers, ret 8), 22=setMouseLimits 0x001EDE63, 23=0x0050B238 (ret 8), 24=0x001F01E8 (ret 16), 25=0x0047A69C (ret 4), 26=0x000419CF, 27=0x000419DC, 28=0x001EE5EE (byte and byte pointer, ret 8). Zero Hour names initCursorResources, setCursor, capture, releaseCapture, setRedrawMode, getRedrawMode and getMouseEvent; other slots retain neutral names.
// Constructor 0x001EECB9..0x001EECCC constructs 56 records of 0x54 bytes at +0xC; 0x001EECF0..0x001EED02 constructs 256 records of 0x3C bytes at +0x130C. It clears 0x3C-byte current/previous records at +0x4F0C/+0x4F48 (0x001EED7C..0x001EED95), and writes through dword +0x500C at 0x001EEE72. This proves extent >=0x5010; Win32Mouse's constructor 0x00041976 calls Mouse at 0x0004197B, then clears its own first member at +0x5010 (0x00041987..0x00041995). The constructor sets bytes +0x5008/+0x5009 at 0x001EEFA2/0x001EEF71. Unexposed member storage stays opaque.
// Redraw mode is the dword at +0x12DC (constructor 0x001EEDE8/0x001EEDF0, slot 26 setter 0x000419CF, slot 27 getter 0x000419DC); Zero Hour supplies its name and enum. The constructor writes the ortho flag +0x12E3 and zoom +0x12E4; Zero Hour supplies m_orthoCamera/m_orthoZoom. Cursor index +0x4FA4 is read and bounds-checked against 0x38 by setCursor 0x001EEFD2; Zero Hour supplies m_currentCursor and MouseCursor names.
// Out-of-line rows: parseIni 0x001EE483; getMouseStatus 0x000425A2 (returns this+0x4F0C); getCursorIndex 0x001EF291; setMouseText 0x001EEBD5; drawCursorText 0x001EDED7; drawTooltip 0x001EF06F; visibility setters 0x001EE5BE/0x001EE5D6; private commitPendingCursor 0x001EDDFF, which AptTimeLine::rva0051ED7E 0x0051ED7E calls directly, so AptTimeLine is a friend. No inline lifetime or accessor bodies are supplied for these rows.
// Other ledger spellings: rva001EEA6D 0x001EEA6D; rva001EDD80 0x001EDD80; rva001EDFE9 0x001EDFE9 (cursor-record hotspot at +0x40); rva001EDFF7 0x001EDFF7; rva001EDE26 0x001EDE26; rva001EE5EE 0x001EE5EE; rva000B3FD0 0x000B3FD0; rva001EDE9C 0x001EDE9C; protected moveMouse 0x001EDC41, updateMouseData 0x001EDCB1, checkForDrag 0x001EDD1C; private processMouseEvent 0x001EE069. Nonvirtual ledger identities are kept separately from neutral table slots.
// setPosition 0x001EDE4C stores the two position components at +0x4F0C/+0x4F10; Zero Hour's MouseIO::pos supplies the ICoord2D type. The rest of that record stays opaque. getCursorIndex 0x001EF291 indexes the name-pointer table at VA 0x00DB9058: None/Normal/Arrow/Scroll/Target/Move/AttackMove are indices 0-6, Select is 13. Zero Hour supplies the matching enum spellings and the inline getMouseCursor getter; there is no ledger row for that getter.
#ifndef BFME2_MOUSE_H
#define BFME2_MOUSE_H

#include "Common/SubsystemInterface.h"
#include "Common/ICoord2D.h"
#include "Common/UnicodeString.h"
#include "Common/RGBAColorInt.h"

class UnicodeString;
struct RGBColor;
struct RGBAColorInt;
struct MouseIO;
struct MouseIO;

// Records: native Mouse ctor 0x001EEC97 walks 56 CursorInfo at stride54,
// then 256 MouseIO at stride3C; Win32Mouse::translateEvent 0x00041839 and
// cursor-resource setup 0x00041BA6 independently establish their typed fields.
struct CursorInfo
{
    CursorInfo();
    ~CursorInfo();
    AsciiString cursorName, cursorText;
    RGBAColorInt cursorTextColor, cursorTextDropColor;
    AsciiString textureName, imageName, W3DModelName, W3DAnimName;
    float W3DScale;
    bool loop;
    ICoord2D hotSpotPosition;
    int numFrames;
    float fps;
    int numDirections;
};
enum MouseButtonState { MBS_Up, MBS_Down, MBS_DoubleClick };
struct MouseIO
{
    MouseIO() {}
    ICoord2D pos;
    unsigned int time;
    int wheelPos;
    ICoord2D deltaPos;
    MouseButtonState leftState;
    int leftEvent, leftFrame;
    MouseButtonState rightState;
    int rightEvent, rightFrame;
    MouseButtonState middleState;
    int middleEvent, middleFrame;
};
class DisplayString;

class Mouse : public SubsystemInterface {
public:
	bool isBeyondDragTolerance(const ICoord2D *anchor, const ICoord2D *dest);	// 0x001EDD80: either axis of anchor-dest exceeds m_dragTolerance (placeholder row, role-named)
	void resetTooltipDelay();	// 0x001EEC4B: restarts the still-time, hides the displayed tooltip and clears its strings (Zero Hour Mouse::resetTooltipDelay)
    enum MouseCursor {
        INVALID_MOUSE_CURSOR = -1, NONE = 0, FIRST_CURSOR = 1, NORMAL = 1,
        ARROW = 2, SCROLL = 3, CROSS = 4, MOVETO = 5, ATTACKMOVETO = 6,
        SELECTING = 13, NUM_MOUSE_CURSORS = 0x38
    };
    enum RedrawMode { RM_WINDOWS, RM_W3D, RM_POLYGON, RM_DX8, RM_MAX };
    enum { NUM_MOUSE_EVENTS = 256 };

    Mouse();
    virtual ~Mouse();
    virtual void init();
    virtual void reset();
    virtual void update();
    virtual void parseIni(); // slot14, native 0x001EE483; old QAE map identity is nonvirtual
    virtual void initCursorResources() = 0;
    virtual void createStreamMessages();
    virtual void setPosition(int x, int y);
    virtual void vslot18(unsigned int *x, unsigned int *y) const;
    virtual void setCursor(MouseCursor cursor) = 0;
    virtual void capture() = 0;
    virtual void releaseCapture() = 0;
    virtual void setMouseLimits();
    virtual void vslot23(int, int);
    virtual void vslot24(int, int, int, int);
    virtual void vslot25(int);
    virtual void setRedrawMode(RedrawMode mode);
    virtual RedrawMode getRedrawMode();
    virtual void vslot28(unsigned char value, unsigned char *store);
    virtual unsigned char getMouseEvent(MouseIO *result, bool flush) = 0;

    const MouseIO *getMouseStatus();
    int getCursorIndex(const AsciiString &name);
    MouseCursor getMouseCursor() { return m_currentCursor; }
    void setMouseText(UnicodeString text, const RGBAColorInt *color, const RGBAColorInt *dropColor);
    void rva001EEA6D(UnicodeString tooltip, int delay = -1, const RGBColor *color = 0, float width = 1.0f);
    void drawCursorText();
    void drawTooltip();
    void _bfme_setEngineVisibility(bool visible);
    void setVisibility(bool visible);
    bool rva001EDD80(const ICoord2D *lhs, const ICoord2D *rhs);
    void *getCurrentCursorHotSpot() const;	// 0x001EDFE9: &m_cursorInfo[m_currentCursor].hotSpotPosition (role-named; the map spells it rva001EDFE9)
    void getMouseLimits(float *out) const;	// 0x001EDFF7: minX, minY, maxX, maxY as floats into out[0..3] (role-named; the map spells it rva001EDFF7)
    bool rva001EDE26() const;
    void latchVisibility(unsigned char visible);   // 0x001EDDC6: once per cleared +0x4FA1: shifts the visibility pair down one step, stores the new value in both, sets the latch
    void rva000B3FD0();
    void rva001EDE9C();

protected:
    void moveMouse(int x, int y, int relOrAbs);
    void updateMouseData();
    void checkForDrag();

public:
	CursorInfo m_cursorInfo[0x38];
	AsciiString m_tooltipFontName;			// 0x126C
	int m_tooltipFontSize;					// 0x1270
	bool m_tooltipFontIsBold;				// 0x1274
	bool m_tooltipAnimateBackground;		// 0x1275
	int m_tooltipFillTime;					// 0x1278
	int m_tooltipDelayTime;					// 0x127C
	float m_tooltipWidth;					// 0x1280
	float m_lastTooltipWidth;				// 0x1284
	bool m_buttonActivity;					// 0x1288
	RGBAColorInt m_tooltipColorText;		// 0x128C
	RGBAColorInt m_tooltipColorHighlight;	// 0x129C
	RGBAColorInt m_tooltipColorShadow;		// 0x12AC
	RGBAColorInt m_tooltipColorBackground;	// 0x12BC
	RGBAColorInt m_tooltipColorBorder;		// 0x12CC
	RedrawMode m_currentRedrawMode;				// 0x12DC
	bool m_useTooltipAltTextColor;			// 0x12E0
	bool m_useTooltipAltBackColor;			// 0x12E1
	bool m_adjustTooltipAltColor;			// 0x12E2
	bool m_orthoCamera;						// 0x12E3
	float m_orthoZoom;						// 0x12E4
	unsigned int m_dragTolerance;			// 0x12E8
	unsigned int m_dragTolerance3D;			// 0x12EC
	unsigned int m_dragToleranceMS;			// 0x12F0
	unsigned char m_numButtons;				// 0x12F4
	unsigned char m_numAxes;				// 0x12F5
	bool m_forceFeedback;					// 0x12F6
	UnicodeString m_str12F8;
	UnicodeString m_tooltipString;			// 0x12FC
	UnicodeString m_str1300;
	DisplayString *m_tooltipDisplayString;			// 0x1304
	bool m_displayTooltip;					// 0x1308
	bool m_isTooltipEmpty;					// 0x1309
	MouseIO m_mouseEvents[256];				// 0x130C
	MouseIO m_currMouse;					// 0x4F0C
	MouseIO m_prevMouse;					// 0x4F48
	int m_minX;								// 0x4F84
	int m_maxX;
	int m_minY;
	int m_maxY;
	unsigned int m_inputFrame;				// 0x4F94
	unsigned int m_deadInputFrame;			// 0x4F98
	bool m_inputMovesAbsolute;				// 0x4F9C
	unsigned char m_visible;					// 0x4F9D (byte: latchVisibility 0x001EDDC6 copies its byte argument unnormalized)
	unsigned char m_flag4F9E;
	unsigned char m_flag4F9F;
	unsigned char m_flag4FA0;
	bool m_flag4FA1;
	MouseCursor m_currentCursor;					// 0x4FA4
	DisplayString *m_cursorTextDisplayString;		// 0x4FA8
	RGBAColorInt m_cursorTextColor;			// 0x4FAC
	RGBAColorInt m_cursorTextDropColor;		// 0x4FBC
	int m_tooltipDelay;						// 0x4FCC
	int m_highlightPos;						// 0x4FD0
	unsigned int m_highlightUpdateStart;	// 0x4FD4
	unsigned int m_stillTime;				// 0x4FD8
	RGBAColorInt m_tooltipTextColor;		// 0x4FDC
	RGBAColorInt m_tooltipBackColor;		// 0x4FEC
	int m_eventsThisFrame;					// 0x4FFC
	int m_leftClickAge;						// 0x5000
	int m_rightClickAge;					// 0x5004
	bool m_flag5008;
	bool m_flag5009;
	int m_value500C;						// 0x500C

private:
    friend class AptTimeLine;
    void commitPendingCursor();
    void processMouseEvent(int eventToProcess);
};

typedef char MouseSize[(sizeof(Mouse) == 0x5010) ? 1 : -1];
typedef char CursorInfoSize[(sizeof(CursorInfo) == 0x54) ? 1 : -1];
typedef char MouseIOSize[(sizeof(MouseIO) == 0x3C) ? 1 : -1];
extern Mouse *TheMouse;

#endif
