#pragma once
#include "Common/SubsystemInterface.h"
#include "GameClient/GameWindow.h"
#include "Common/StlList.h"
class WindowLayoutInfo;
class GameWindowRenderer;
struct GameWindowCreationData;
struct _RadioButtonData;
struct _TabControlData;
struct _ListboxData;
struct _SliderData;
struct _TextData;
struct _EntryData;
struct _ComboBoxData;
enum GameWindowMessage;
enum WinInputReturnCode;
class Image;
class GameFont;
class ModalWindow;
// SIZE0x40: allocation0x0004C4C7, base ctor0x002C5334.
// Primary0..13 from SubsystemInterface, W3D table RVA0x007C7C90 gives0..77.
// Named calls agree across old canonical header and 32 synthesized views.
// Slots41/44/50/69 follow ZH names with target traversal/list/store/draw bodies.
// Slots74..76 are virtual in both retail tables despite old map QAE spellings.
// W3D targets8FCE3..8FEDE set factories then call the named base gadgets.
// Slots19/21..29/47 names: donor (ZH), matched against each body/call ABI.
// Slot14 creates the renderer,15 the window,16 returns W3DLeftHUDDraw.
// Slot20 name is role-inferred:8FFE0 sets WIN_STATUS_IMAGE and calls the
// push-button factory through2C19C9, with distinct renderer factory8FF84.
class GameWindowManager : public SubsystemInterface {
public:
    GameWindowManager();
    virtual ~GameWindowManager();
    virtual void init();
    virtual void reset();
    virtual void update();
    virtual GameWindowRenderer *createWindowRenderer(); // slot 14
    virtual GameWindow *allocateNewWindow(GameWindowCreationData *); // slot 15
    virtual GameWindowDrawFunc getLeftHUDDrawFunc(); // slot 16
    // MSVC reverses introducing overloads: bool form occupies17, short form18.
    virtual GameWindow *gogoMessageBox(int,int,int,int,unsigned short,UnicodeString,UnicodeString,void (*)(),void (*)(),void (*)(),void (*)());
    virtual GameWindow *gogoMessageBox(int,int,int,int,unsigned short,UnicodeString,UnicodeString,void (*)(),void (*)(),void (*)(),void (*)(),bool);
    virtual GameWindow *gogoGadgetPushButton(GameWindowCreationData *,GameFont *,bool); // slot 19
    virtual GameWindow *gogoGadgetImageButton(GameWindowCreationData *,GameFont *,bool); // slot20, inferred name
    virtual GameWindow *gogoGadgetCheckbox(GameWindowCreationData *,GameFont *,bool); // slot 21
    virtual GameWindow *gogoGadgetRadioButton(GameWindowCreationData *,_RadioButtonData *,GameFont *,bool); // slot 22
    virtual GameWindow *gogoGadgetTabControl(GameWindowCreationData *,_TabControlData *,GameFont *,bool); // slot 23
    virtual GameWindow *gogoGadgetListBox(GameWindowCreationData *,_ListboxData *,GameFont *,bool); // slot 24
    virtual GameWindow *gogoGadgetSlider(GameWindowCreationData *,_SliderData *,GameFont *,bool); // slot 25
    virtual GameWindow *gogoGadgetProgressBar(GameWindowCreationData *,GameFont *,bool); // slot 26
    virtual GameWindow *gogoGadgetStaticText(GameWindowCreationData *,_TextData *,GameFont *,bool); // slot 27
    virtual GameWindow *gogoGadgetTextEntry(GameWindowCreationData *,_EntryData *,GameFont *,bool); // slot 28
    virtual GameWindow *gogoGadgetComboBox(GameWindowCreationData *,_ComboBoxData *,GameFont *,bool); // slot 29
    virtual void assignDefaultGadgetLook(GameWindow *,GameFont *,bool); // slot 30
    virtual GameWindow *winCreateFromScript(AsciiString,WindowLayoutInfo *,GameWindow *); // slot 31
    virtual WindowLayout *winCreateLayout(AsciiString); // slot 32
    virtual void freeStaticStrings(); // slot 33
    virtual GameWindow *winCreate(GameWindowCreationData *); // slot 34
    virtual int winDestroy(GameWindow *); // slot 35
    virtual int winDestroyAll(); // slot 36
    virtual GameWindow *winGetWindowList(); // slot 37
    virtual void hideWindowsInRange(GameWindow *,int,int,bool); // slot 38
    virtual void enableWindowsInRange(GameWindow *,int,int,bool); // slot 39
    virtual void windowHiding(GameWindow *); // slot 40
    virtual void winRepaint(); // slot 41
    virtual void winNextTab(GameWindow *); // slot 42
    virtual void winPrevTab(GameWindow *); // slot 43
    virtual void registerTabList(_STL::list<GameWindow *>); // slot 44
    virtual void clearTabList(); // slot 45
    virtual WinInputReturnCode winProcessMouseEvent(GameWindowMessage,ICoord2D *,void *); // slot 46
    virtual WinInputReturnCode winProcessKey(unsigned char,unsigned char); // slot 47
    virtual GameWindow *winGetFocus(); // slot 48
    virtual int winSetFocus(GameWindow *); // slot 49
    virtual void winSetGrabWindow(GameWindow *); // slot 50
    virtual GameWindow *winGetGrabWindow(); // slot 51
    virtual void winSetLoneWindow(GameWindow *); // slot 52
    virtual GameWindow *winGetLoneWindow(); // slot 53
    virtual bool isEnabled(GameWindow *); // slot 54
    virtual bool isHidden(GameWindow *); // slot 55
    virtual void addWindowToParent(GameWindow *,GameWindow *); // slot 56
    virtual void addWindowToParentAtEnd(GameWindow *,GameWindow *); // slot 57
    virtual WindowMsgHandledType winSendSystemMsg(GameWindow *,unsigned int,unsigned int,unsigned int); // slot 58
    virtual WindowMsgHandledType winSendInputMsg(GameWindow *,unsigned int,unsigned int,unsigned int); // slot 59
    virtual GameWindow *winGetWindowFromId(GameWindow *,int); // slot 60
    virtual int winCapture(GameWindow *); // slot 61
    virtual int winRelease(GameWindow *); // slot 62
    virtual GameWindow *winGetCapture(); // slot 63
    virtual int winSetModal(GameWindow *); // slot 64
    virtual int winUnsetModal(GameWindow *); // slot 65
    virtual void winDrawImage(const Image *,int,int,int,int,int); // slot 66
    virtual void winFillRect(int,float,int,int,int,int); // slot 67
    virtual void winOpenRect(int,float,int,int,int,int); // slot 68
    virtual void winDrawLine(int,float,int,int,int,int); // slot 69
    virtual int winMakeColor(unsigned char,unsigned char,unsigned char,unsigned char); // slot 70
    virtual const Image *winFindImage(const char *); // slot 71
    virtual int winFontHeight(GameFont *); // slot 72
    virtual void winFormatText(GameFont *,UnicodeString,int,int,int,int,int); // slot 73
    virtual void winGetTextSize(GameFont *,UnicodeString,int *,int *,int); // slot 74
    virtual UnicodeString winTextLabelToText(AsciiString); // slot 75
    virtual GameFont *winFindFont(AsciiString,int,bool); // slot 76
    virtual GameWindow *getWindowUnderCursor(int,int,bool); // slot 77
    void linkWindow(GameWindow *);
    void unlinkWindow(GameWindow *);
    void unlinkChildWindow(GameWindow *);
    void insertWindowAheadOf(GameWindow *,GameWindow *);
protected:
    void processDestroyList();
    int drawWindow(GameWindow *); // 0x002C0FC9
    void dumpWindow(GameWindow *); // 2C1148, recursive child/sibling walk
    GameWindow *m_windowList,*m_windowTail,*m_destroyList; // 0C,10,14
    GameWindow *m_currMouseRgn,*m_mouseCaptor,*m_keyboardFocus; // 18,1C,20
    ModalWindow *m_modalHead; // 24
    GameWindow *m_grabWindow,*m_loneWindow; // 28,2C
    // ctor2C5334 constructs a four-byte STLport list at30; registerTabList
    // 2C55AC assigns list<GameWindow*> there. Donor names the following fields.
    _STL::list<GameWindow *> m_tabList; // 30
    const Image *m_cursorBitmap; //34
    unsigned int m_captureFlags; //38
    int m_unknown3C; // 3C
};
typedef char GameWindowManagerSizeCheck[sizeof(GameWindowManager)==0x40 ? 1 : -1];
extern GameWindowManager *TheWindowManager;
