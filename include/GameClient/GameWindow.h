#pragma once
#include <stddef.h>
#include "GameClient/WinInstanceData.h"
enum WindowMsgHandledType { MSG_IGNORED, MSG_HANDLED };
// Window status bits (Zero Hour's WIN_STATUS values; findPrevLeaf 0x003139B3 tests 0x100 and winRepaint 0x003141AA tests 0x10).
enum { WIN_STATUS_HIDDEN = 0x00000010, WIN_STATUS_TAB_STOP = 0x00000100 };
// Window messages (values proven by Mouse::processMouseEvent 0x001EE069 and the gadget system procedures; names are Zero Hour's).
enum GameWindowMessage {
	GWM_CREATE = 1,
	GWM_LEFT_DOWN = 5, GWM_LEFT_UP = 6, GWM_LEFT_DOUBLE_CLICK = 7, GWM_LEFT_DRAG = 8,
	GWM_MIDDLE_DOWN = 9, GWM_MIDDLE_UP = 10, GWM_MIDDLE_DOUBLE_CLICK = 11, GWM_MIDDLE_DRAG = 12,
	GWM_RIGHT_DOWN = 13, GWM_RIGHT_UP = 14, GWM_RIGHT_DOUBLE_CLICK = 15, GWM_RIGHT_DRAG = 16
};
class GameWindow;
class GameFont;
class WindowLayout;
class GameWindowRenderer;
struct GameWindowCreationData;
struct GameWindowEditData;
struct IRegion2D;
typedef GameWindow *(__stdcall *GameWindowFactory)(GameWindowCreationData *);
class UnicodeString;
typedef WindowMsgHandledType (*GameWindowInputFunc)(GameWindow *,unsigned int,unsigned int,unsigned int);
typedef void (*GameWindowDrawFunc)(GameWindow *,WinInstanceData *);
typedef void (*GameWindowTooltipFunc)(GameWindow *,WinInstanceData *,unsigned int);
typedef void (*GameWindowCallback)(GameWindow *);
// SIZE0x218: ctor0x003145F0/dtor0x00314A0C and Apt secondary base at+0x218.
// Table RVA0x0080BBCC has11 slots. The table is not a SubsystemInterface.
// Callback names reflect called fields; slots7/10 roles come from creation consumers.
// The handler object a window can carry at +0x1DC: winSendInputMsg 0x00314511 calls its slot 3 and winSendSystemMsg 0x0031452F its slot 4
// when it is set, otherwise the window's own input (slot 1) / system (slot 2) virtual. Slots 0-2 are not exercised by a recovered function.
class GameWindowMessageHandler {
public:
    virtual void unknownHandlerSlot0();
    virtual void unknownHandlerSlot1();
    virtual void unknownHandlerSlot2();
    virtual WindowMsgHandledType handleInput(unsigned int msg,unsigned int data1,unsigned int data2);
    virtual WindowMsgHandledType handleSystem(unsigned int msg,unsigned int data1,unsigned int data2);
};
class GameWindow {
public:
    GameWindow();
    virtual ~GameWindow();
    virtual WindowMsgHandledType input(unsigned int,unsigned int,unsigned int);
    virtual WindowMsgHandledType system(unsigned int,unsigned int,unsigned int);
    virtual int draw(WinInstanceData *);
    virtual int tooltip(WinInstanceData *,unsigned int);
    virtual int invokeCallback(); // role name:9FD78 invokes callback+1F0
    virtual bool isLeftHUD() const; // role name:313913 compares input to LeftHUDInput
    virtual GameWindowFactory getWindowFactory() = 0; // role:3146DC stores slot7 in creation.allocator; A2197 returns8FA81
    virtual bool isPushButton() const; // role name:31393A compares GadgetPushButtonInput
    virtual bool hasTooltip() const; // role name:8FF78 tests tooltip callback
    virtual bool supportsWindowRenderer(); // role:2C2533 gates allocation of renderer at+4
    GameWindow *winGetParent();
    GameWindow *winGetNext();
    GameWindow *winGetChild();
    int winGetWindowId();
    unsigned int winGetStatus();
    void *winGetUserData();
    WinInstanceData *winGetInstanceData();
    void playComboBoxClickSound();	// 0x00323E1C: plays the combo box click sound when the list data asks for audio feedback
    WindowMsgHandledType winSendSystemMsg(unsigned int,unsigned int,unsigned int);
    WindowMsgHandledType winSendInputMsg(unsigned int,unsigned int,unsigned int);
    int winGetScreenPosition(int *,int *);
    int winGetSize(int *,int *);
    int winSetSize(int,int);
    int winHide(bool);
    int winEnable(bool);
    void winSetFont(GameFont *);
    int winSetOwner(GameWindow *); //3140CF stores instance.owner, null selects self
    // Nonvirtual surface: native matched GameWindow.cpp/Bfme2GameWindow.h views.
    // Declaration-only APIs preserve retail's out-of-line call boundaries.
    GameWindow *winGetPrev();
    GameWindow *winGetOwner();
    GameWindow *winPointInChild(int,int,bool,bool = false);
    GameWindow *winPointInAnyChild(int,int,bool,bool);
    int winGetPosition(int *,int *);
    int winSetPosition(int,int);
    int winGetCursorPosition(int *,int *);
    void set(int,int);                         // 0x00313D28 (the ledger's name; stores m_instData.m_imageOffset)
    void get(int *,int *);                     // 0x00313D3F (the ledger's name; reads it back unless either pointer is null)
    int winBringToTop();
    int winActivate();
    bool winIsHidden();
    unsigned int winGetStyle();
    unsigned int winSetStatus(unsigned int);
    unsigned int winClearStatus(unsigned int);
    int winSetWindowId(int);
    int winSetInstanceData(WinInstanceData *);
    GameFont *winGetFont();
    void winSetUserData(void *);
    int winSetInputFunc(GameWindowInputFunc);
    int winSetDrawFunc(GameWindowDrawFunc);
    int winSetTooltipFunc(GameWindowTooltipFunc);
    int winSetCallbacks(GameWindowInputFunc,GameWindowDrawFunc,GameWindowTooltipFunc);
    int winSetParent(GameWindow *);
    int winSetCursorPosition(int x,int y); // 0x00313B0A
    int winSetSystemFunc(GameWindowInputFunc system); // 0x00314134
    void winSetCallback(GameWindowCallback callback); // 0x0031417C
    bool winIsChild(GameWindow *);              // 0x003140AB (Zero Hour's name: the argument or one of its parents is this window)
    void winSetNext(GameWindow *);              // 0x003140E4 (role name: stores m_next; the map spells it as a dword slot)
    void winSetPrev(GameWindow *);              // 0x003140F8 (role name: stores m_prev)
    WindowLayout *winGetLayout();               // 0x00314126 (returns m_layout)
    int winRepaint();                           // 0x003141AA (role name: draws the window unless it is hidden)
    void setRva003144FD(GameWindowEditData *);  // 0x003144FD (the ledger's name; stores m_editData)
    void snapshotCreateView(GameWindowCreationData *,bool);
    void orderPairs();
    void winDrawBorder();
    int winNextTab();
    // Window-layout links (role names; the ledger spells these as address-named dword-field accessors).
    // WindowLayout::addWindow 0x00538B48 / removeWindow 0x00538B95 / bringForward 0x00538B07 prove the roles:
    // a layout keeps its windows in a doubly linked list through these links, newest at the head.
    GameWindow *winGetPrevInLayout();           // 0x0031412D
    GameWindow *winGetNextInLayout();           // 0x0052A28E
    void winSetPrevInLayout(GameWindow *);      // 0x00314119
    void winSetNextInLayout(GameWindow *);      // 0x0031410C
    void winSetLayout(WindowLayout *);          // 0x0023C4EB
    int winPrevTab();
    int winSetEnabledImage(int,const Image *);
    int winSetEnabledColor(int,int);
    int winSetEnabledBorderColor(int,int);
    int winSetDisabledImage(int,const Image *);
    int winSetDisabledColor(int,int);
    int winSetDisabledBorderColor(int,int);
    int winSetHiliteImage(int,const Image *);
    int winSetHiliteColor(int,int);
    int winSetHiliteBorderColor(int,int);
    const Image *winGetEnabledImage(int);
    int winGetEnabledColor(int);
    int winGetEnabledBorderColor(int);
    const Image *winGetDisabledImage(int);
    int winGetDisabledColor(int);
    int winGetDisabledBorderColor(int);
    const Image *winGetHiliteImage(int);
    int winGetHiliteColor(int);
    int winGetHiliteBorderColor(int);
    void winSetEnabledTextColors(int,int);
    void winSetDisabledTextColors(int,int);
    void winSetHiliteTextColors(int,int);
    void winSetIMECompositeTextColors(int,int);
    int winGetEnabledTextColor();
    int winGetEnabledTextBorderColor();
    int winGetDisabledTextColor();
    int winGetDisabledTextBorderColor();
    int winGetHiliteTextColor();
    int winGetHiliteTextBorderColor();
    int winGetIMECompositeTextColor();
    int winGetIMECompositeBorderColor();
    int winGetTextLength();
    UnicodeString winGetText();
    // 0x0031484A: sets the instance data's text (WinInstanceData::setText 0x003223B3), then tells the
    // renderer at +0x4 (its slot 3, passing the string by address) and returns 0. It is NOT in the
    // 11-slot table at 0x0080BBCC, so it is non-virtual; the map spells it `UAEH` (virtual), which
    // disagrees with the retail table: the right spelling is `QAEH`.
    int winSetText(UnicodeString);
    void winSetTooltip(UnicodeString);
    unsigned char winPointInWindow(int,int);
    int winGetRegion(IRegion2D *region); // Zero Hour's winGetRegion: copies the region at +0x14 into the buffer when one is given
    GameWindowEditData *winGetEditData();
protected:
    GameWindow *findFirstLeaf();
    GameWindow *findLastLeaf();
    GameWindow *findPrevLeaf();
    GameWindow *findNextLeaf();
public:
    GameWindowRenderer *m_positionSink; //04; class role inferred from creation/drawing
    unsigned int m_status; // 08
    int m_sizeX,m_sizeY; // 0C,10
    int m_regionLoX,m_regionLoY,m_regionHiX,m_regionHiY; // 14..20
    int m_cursorX,m_cursorY; // 24,28
    void *m_userData; // 2C
    WinInstanceData m_instData; // 30
    void *m_inputData; // 1D8
    GameWindowMessageHandler *m_inputExtra; // 1DC
    GameWindowInputFunc m_inputFunc,m_systemFunc; // 1E0,1E4
    GameWindowDrawFunc m_drawFunc; // 1E8
    GameWindowTooltipFunc m_tooltipFunc; // 1EC
    GameWindowCallback m_callback; // 1F0
    int m_callbackExtra; // 1F4
    GameWindow *m_next,*m_prev,*m_parent,*m_child; // 1F8..204
    GameWindow *m_nextLayout,*m_prevLayout; // 208,20C
    WindowLayout *m_layout; // 210, winDestroy0x002C1173 removes from layout
    GameWindowEditData *m_editData; //214; ctor/dtor own four strings, identity unresolved
};
typedef char GameWindowSizeCheck[sizeof(GameWindow)==0x218 ? 1 : -1];
typedef char GameWindowInstanceOffsetCheck[offsetof(GameWindow,m_instData)==0x30 ? 1 : -1];
typedef char GameWindowLinksOffsetCheck[offsetof(GameWindow,m_next)==0x1F8 ? 1 : -1];
