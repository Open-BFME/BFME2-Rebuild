#pragma once
#include <list>
#include "Common/AsciiString.h"
#include "GameClient/WindowLayout.h"
class GameWindow;
class WindowLayoutInfo;
typedef WindowLayout *(__stdcall *WindowLayoutFactory)(WindowLayoutInfo *);
// Native constructor31763A/assignment317779 establish this 2C-byte record.
// Layout parsers31721D..3172E9 name callbacks and strings at04..20;
// load31742C stores filename24. winCreateFromScript appends GameWindow* at28.
// The old address-named constructor owner denotes this same record; its map
// identity remains unresolved, so no constructor body is supplied here.
class WindowLayoutInfo {
public:
    WindowLayoutInfo();
    ~WindowLayoutInfo();
    bool load(AsciiString filename);
    unsigned int m_version;
    WindowLayoutFunc m_init, m_update, m_shutdown;
    WindowLayoutFactory m_classCallback;
    AsciiString m_initName, m_updateName, m_shutdownName, m_className, m_filename;
    _STL::list<GameWindow *> m_windowList;
};
typedef char WindowLayoutInfoSizeCheck[sizeof(WindowLayoutInfo)==0x2C ? 1 : -1];

struct LayoutScriptParse {
    char *name;
    bool (*parse)(char *,char *,unsigned int,WindowLayoutInfo *);
};
