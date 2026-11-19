#pragma once
#include "Common/SubsystemInterface.h"
#include "Common/AsciiString.h"
class WindowLayout;
class GameWindow;
class AnimateWindowManager;
class ShellMenuSchemeManager;
enum AnimTypes;
// SIZE0x78: allocation239891 and ctor35BFBC; SubsystemInterface0xC,
// fourteen primary slots at816208 (ctor35BFDA/dtor35C099 stores).
// StackC/count4C proved by top35BD7E/link35BD8B/unlink35BDA5; ctor
// stores bytes50..54/5C/5D/6C/6D, string58, managers60/64, layouts70/74.
// Names: donor(ZH), corroborated by native menu/animation/scheme callers.
class Shell : public SubsystemInterface {
public:
    Shell();
    virtual ~Shell();
    virtual void init();
    virtual void reset();
    virtual void update();
    bool showShellMap(bool useShellMap,bool restartShellGame);
    void hide(bool hide);                // 0x0035BD0D: hides or shows every layout on the stack, then detaches the IME (Zero Hour Shell::hide)
    void updateSchemeManager();          // 0x0035BD00: m_schemeManager->update() when there is a manager
    void stopMusic();                    // 0x0035BD3F: removes the audio event in m_audioHandle (AudioManager slot 27) and resets the handle to 1
    void push(AsciiString filename,bool shutdownImmediate=false);
    void pop();
    void popImmediate();
    void showShell(bool runInit=true);
    WindowLayout *top();
    void shutdownComplete(WindowLayout *,bool impendingPush=false);
    WindowLayout *findScreenByFilename(AsciiString);
    bool isShellActive() const { return m_isShellActive; }
    int getScreenCount() const { return m_screenCount; }
    void registerWithAnimateManager(GameWindow *,AnimTypes,bool,unsigned int delayMS=0);
    bool isAnimFinished();
    void reverseAnimatewindow();
    bool isAnimReversed();
    void loadScheme(AsciiString);
    ShellMenuSchemeManager *getShellMenuSchemeManager() { return m_schemeManager; }
    WindowLayout *getSaveLoadMenuLayout();
    WindowLayout *getPopupReplayLayout();
    bool rva0035BD5D(); // unresolved native method name; checks playing audio68
    void rva0035C2B9();
    void rva0035BF4C(bool);
    // The Apt screens' close helpers (StrategicPlayerStatus 0x0052340D, LanLobby 0x00444040, SaveLoad 0x00433D27)
    // store the byte at +0x54 inline; the setter is inline here, not a retail function.
    void setClearBackground(bool value) { m_clearBackground = value; }
    // AptCampaignReview::Continue 0x00512823 stores Shell+0x5D inline.
    void setShellMapOn(bool value) { m_shellMapOn = value; }
protected:
    void linkScreen(WindowLayout *);
    void unlinkScreen(WindowLayout *);
    void doPush(AsciiString);
    void doPop(bool impendingPush);
    WindowLayout *m_screenStack[16]; //C
    int m_screenCount; //4C
    bool m_pendingPush,m_pendingPop,m_unknown52,m_unknown53,m_clearBackground;
    AsciiString m_pendingPushName; //58
    bool m_isShellActive,m_shellMapOn; //5C
    AnimateWindowManager *m_animateWindowManager; //60
    ShellMenuSchemeManager *m_schemeManager; //64
    unsigned int m_audioHandle; //68: isCurrentlyPlaying caller35BD5D
    bool m_unknown6C,m_unknown6D;
    WindowLayout *m_saveLoadMenuLayout,*m_popupReplayLayout; //70/74
};
typedef char ShellSizeCheck[sizeof(Shell)==0x78?1:-1];
extern Shell *TheShell;
