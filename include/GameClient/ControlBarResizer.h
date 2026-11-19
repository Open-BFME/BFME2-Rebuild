#pragma once
#include "Common/AsciiString.h"
#include "Common/ICoord2D.h"
#include "Common/StlList.h"
struct FieldParse;
// Allocation1DB9A2 requests24; ctor1DB6FD initializes eight coordinates.
// Native sizers1DB7C9/1DB82D independently access each pair in this order.
class ResizerWindow {
public:
    ResizerWindow() throw();
    AsciiString m_name;
    ICoord2D m_defaultSize,m_defaultPos,m_altSize,m_altPos;
};
typedef char ResizerWindowSizeCheck[sizeof(ResizerWindow)==0x24?1:-1];
// Native methods1DB71C..1DB9A2 access a single STLport list at+0;
// no virtual dispatch/base. Names: donor(ZH), matching native row names.
class ControlBarResizer {
public:
    ControlBarResizer();
    ~ControlBarResizer();
    void init();
    ResizerWindow *findResizerWindow(AsciiString);
    ResizerWindow *newResizerWindow(AsciiString);
    void sizeWindowsDefault();
    void sizeWindowsAlt();
    static const FieldParse m_controlBarResizerParseTable[];
    const FieldParse *getFieldParse() const { return m_controlBarResizerParseTable; }
    typedef _STL::list<ResizerWindow *> ResizerWindowList;
    ResizerWindowList m_resizerWindowsList;
};
typedef char ControlBarResizerSizeCheck[sizeof(ControlBarResizer)==4?1:-1];
