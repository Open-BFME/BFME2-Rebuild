#pragma once
#include "Common/GameType.h"
#include "Common/SubsystemInterface.h"
#include "GameClient/GameWindow.h"
class WindowLayout;
typedef void (*WindowLayoutInitFunc)(WindowLayout *, void *);
typedef void (*WindowLayoutUpdateFunc)(WindowLayout *, void *);
typedef void (*WindowLayoutShutdownFunc)(WindowLayout *, void *);
class WindowLayoutInfo;
typedef WindowLayout *(__stdcall *WindowLayoutFactory)(WindowLayoutInfo *);
// Target: 0x002D2235/225F/23A1 read twelve table pointers at +0x0C;
// entries are key/name/function, 12 bytes. Table indices 7/8 and count12
// come from the native loops. Base identity and API spellings follow the
// FunctionLexicon donor, not a claim that the adjacent Rva002D22CA owner
// has been identified. The existing shared base retains unresolved slots.
class FunctionLexicon : public SubsystemInterface {
public:
    struct TableEntry { NameKeyType key; const char *name; void *func; };
    enum TableIndex {
        TABLE_ANY = -1,
        TABLE_WIN_SYSTEM = 0,
        TABLE_WIN_INPUT = 1,
        TABLE_WIN_TOOLTIP = 2,
        TABLE_WIN_DEVICE_DRAW = 3,
        TABLE_WIN_DRAW = 4,
        TABLE_WIN_DEVICE_FACTORY = 5,
        TABLE_WIN_FACTORY = 6,
        TABLE_WIN_LAYOUT_INIT = 7,
        TABLE_WIN_LAYOUT_DEVICEINIT = 8,
        TABLE_WIN_LAYOUT_UPDATE = 9,
        TABLE_WIN_LAYOUT_SHUTDOWN = 10,
        TABLE_WIN_LAYOUT_CLASS = 11,
        MAX_FUNCTION_TABLES = 12
    };
    FunctionLexicon();
    virtual ~FunctionLexicon();
    virtual void init();
    virtual void reset();
    virtual void update();
    bool validate();	// 0x002D22D5: no two table entries share a function (Zero Hour's debug check)
    WindowLayoutInitFunc winLayoutInitFunc(NameKeyType, TableIndex = TABLE_ANY);
    GameWindowInputFunc gameWinSystemFunc(NameKeyType key, TableIndex index=TABLE_WIN_SYSTEM)
        { return (GameWindowInputFunc)findFunction(key,index); }
    GameWindowInputFunc gameWinInputFunc(NameKeyType key, TableIndex index=TABLE_WIN_INPUT)
        { return (GameWindowInputFunc)findFunction(key,index); }
    GameWindowTooltipFunc gameWinTooltipFunc(NameKeyType key, TableIndex index=TABLE_WIN_TOOLTIP)
        { return (GameWindowTooltipFunc)findFunction(key,index); }
    GameWindowDrawFunc gameWinDrawFunc(NameKeyType key, TableIndex index=TABLE_ANY);		// 0x002D2341
    GameWindowFactory gameWinFactoryFunc(NameKeyType key, TableIndex index=TABLE_ANY);	// 0x002D2371
    // Inline donor wrappers; native parsers317261/A5/E9 pass indices9/10/11
    // directly to findFunction225F, with no intervening call.
    WindowLayoutUpdateFunc winLayoutUpdateFunc(NameKeyType key, TableIndex index=TABLE_WIN_LAYOUT_UPDATE)
        { return (WindowLayoutUpdateFunc)findFunction(key,index); }
    WindowLayoutShutdownFunc winLayoutShutdownFunc(NameKeyType key, TableIndex index=TABLE_WIN_LAYOUT_SHUTDOWN)
        { return (WindowLayoutShutdownFunc)findFunction(key,index); }
    // Structural name: LAYOUTCLASS entry passes table11; factory ABI is
    // proved by winCreateLayout3176B9's stdcall invocation of info+10.
    WindowLayoutFactory winLayoutClassFunc(NameKeyType key)
        { return (WindowLayoutFactory)findFunction(key,TABLE_WIN_LAYOUT_CLASS); }
protected:
    void *keyToFunc(NameKeyType, TableEntry *);
    void *findFunction(NameKeyType, TableIndex);
    TableEntry *m_tables[MAX_FUNCTION_TABLES];
};
typedef char FunctionLexiconMeasuredFootprint[(sizeof(FunctionLexicon)==0x3C)?1:-1];
extern FunctionLexicon *TheFunctionLexicon;
