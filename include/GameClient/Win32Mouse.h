#pragma once
#include "GameClient/Mouse.h"
// Native ctor41976/dtor419E3, table7C2478: Mouse base5010 followed by
// 256 four-word Win32 events. Own final flag6020; W3DMouse starts at6024.
class Win32Mouse : public Mouse
{
public:
    Win32Mouse();
    virtual ~Win32Mouse();
    virtual void init();
    virtual void reset() { Mouse::reset(); }   // slot 9: retail 0x00041A06, a tail jump to Mouse::reset
    virtual void update();
    virtual void initCursorResources();
    virtual void setPosition(int x, int y);
    virtual void setCursor(MouseCursor cursor);
    virtual void capture() {}      // slot 20: empty, folded with the one-byte ret at 0x000B3FD0
    virtual void releaseCapture() {} // slot 21: the same stub
    virtual void vslot28(unsigned char value, unsigned char *store);   // slot 28, 0x00041B85
    void addWin32Event(unsigned int, unsigned int, long, unsigned long);
protected:
    virtual unsigned char getMouseEvent(MouseIO *, bool);
    void translateEvent(unsigned int, MouseIO *);
    struct Win32MouseEvent
    {
        unsigned int msg, wParam;
        long lParam;
        unsigned long time;
    };
    Win32MouseEvent m_eventBuffer[NUM_MOUSE_EVENTS];
    unsigned int m_nextFreeIndex, m_nextGetIndex;
    MouseCursor m_currentWin32Cursor;
    int m_directionFrame;
    bool m_lostFocus;
};
typedef char Win32MouseSize[(sizeof(Win32Mouse) == 0x6024) ? 1 : -1];
extern Win32Mouse *TheWin32Mouse;
