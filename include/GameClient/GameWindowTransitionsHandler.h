#pragma once
#include "Common/SubsystemInterface.h"
#include "Common/StlHashMap.h"
#include "Common/StlList.h"
#include "Common/STLTypedefs.h"
#include "Common/RTSEqualTo.h"
#include "Common/ICoord2D.h"
#include "Common/AsciiString.h"
#include "Common/NameKeyGenerator.h"
class Transition;
class TransitionGroup;
class GameWindow;
typedef Transition *(*WindowTransitionFactory)();
typedef _STL::hash_map<NameKeyType,WindowTransitionFactory,rts::hash<NameKeyType>,rts::equal_to<NameKeyType> > WindowTransitionMap;
// One window of a transition group (retail allocation 0x18: parseWindow 0x001DCB2F news 0x18 and runs 0x001DBC61).
// Members from init 0x001DBD24 (name key at +8, window at +0xC, the frame delay copied to +0x14) and update 0x001DBACA
// (transition at +0x10). Zero Hour's TransitionWindow keeps the style in the object and builds the transition in init;
// BFME 2 builds the transition while parsing (the Style field's factory stores it through 0x005F69CE), so there is no style member.
class TransitionWindow {
public:
    bool init();                    // 0x001DBD24
    void update(int frame);         // 0x001DBACA
    bool isFinished() const;        // 0x001DBAF6
    void reverse(int totalFrames);  // 0x001DBB04
    void retire();                  // 0x001DBB15: the transition's slot 5
    void skip();                    // 0x001DBB24
    void draw();                    // 0x001DBB33
    int getTotalFrames() const;     // 0x001DBB42
    AsciiString m_winName;          // +0
    int m_frameDelay;               // +4
    NameKeyType m_winID;            // +8
    GameWindow *m_win;              // +0xC
    Transition *m_transition;       // +0x10
    int m_currentFrameDelay;        // +0x14
};
typedef char TransitionWindowSizeCheck[sizeof(TransitionWindow)==0x18 ? 1 : -1];
typedef _STL::list<TransitionWindow *> TransitionWindowList;
// A named group of window transitions (retail allocation 0x14: getNewGroup 0x001DC4D2). Zero Hour's TransitionGroup with the
// list first; name at +0xC (findGroup 0x001DC01C compares it), fire-once byte at +0x10 (setGroup 0x001DC252 and update 0x001DBEB6).
class TransitionGroup {
public:
    void init();                    // 0x001DC164: also tells the handler the group's length plus three frames
    void update();                  // 0x001DBD5D
    bool isFinished() const;        // 0x001DBD83
    int getTotalFrames() const;     // 0x001DBDA4 (the longest window)
    void reverse();                 // 0x001DBDC6
    void retire();                  // 0x001DBE17
    void skip();                    // 0x001DBE34
    void draw();                    // 0x001DBE51
    bool isReversed() const;        // 0x001DBB54
    void addWindow(TransitionWindow *window);   // 0x001DC19F
    bool isFireOnce() const { return m_fireOnce; }
    const AsciiString &getName() const { return m_name; }
    TransitionWindowList m_transitionWindowList;    // +0
    int m_directionMultiplier;      // +4
    int m_currentFrame;             // +8
    AsciiString m_name;             // +0xC
    bool m_fireOnce;                // +0x10
};
typedef char TransitionGroupSizeCheck[sizeof(TransitionGroup)==0x14 ? 1 : -1];
// SIZE6C: ctor1DCBC3, dtor1DC6E4 and allocation2C0A0F. Fourteen slots at
// table7DBC30; SubsystemInterface baseC. init1DBB5C and lock1DBB82 operate
// on this object's critical section. Rva001DBAA4 is the separate Transition
// base (size10/table7DBC10), despite old typed-receiver casts to that name.
// MapC/type from ctor1DCBA4; list20; group pointers24..30. Donor(ZH) supplies
// group roles; native update1DBEB6/reset1DBE6E corroborate them.
class GameWindowTransitionsHandler : public SubsystemInterface {
public:
    GameWindowTransitionsHandler();
    virtual ~GameWindowTransitionsHandler();
    virtual void init();
    virtual void reset();
    virtual void update();
    virtual void draw();
    void load(); //1DC7E1 reads Data/INI/WindowTransitions.ini
    bool isFinished();
    void remove(AsciiString,bool);
    void reverse(AsciiString);
    void setGroup(AsciiString,bool);
    void lock();
    void unlock();
    void regainFocus();
    void waitUntilFinished();           // 0x001DC07E: pumps the window manager and display until the current group is finished
    void finishTransitions();           // 0x001DC5EC: under the lock, runs waitUntilFinished once (not re-entrant)
    void holdGroups();                  // 0x001DBB82: update() stops promoting and retiring groups until releaseGroups
    void releaseGroups();               // 0x001DBB87
    int bfmeGetGroupTotalFrames(AsciiString groupName);   // 0x001DC1FD: the named group's length, 0 when there is none
    void setCoords(const ICoord2D *,const ICoord2D *);
    friend class TransitionGroup;
private:
    TransitionGroup *findGroup(AsciiString groupName);   // 0x001DC01C
    WindowTransitionMap m_transitionMap; //C,14 bytes
    _STL::list<TransitionGroup *> m_transitionGroupList; //20
    TransitionGroup *m_currentGroup,*m_pendingGroup,*m_drawGroup,*m_secondaryDrawGroup;
    int m_lockCount; //34
    // Native Win32 CRITICAL_SECTION at38,24 bytes; preserved opaque storage
    // until the shared platform header is available (no fabricated class).
    unsigned int m_criticalSectionStorage[6];
    bool m_lockInitialized; //50
    bool m_unknown51;
    ICoord2D m_coord1,m_coord2; //54/5C, setter1DBBB7
    unsigned int m_unknown64; //ctor sets33
    bool m_waiting;                     // +0x68: set while waitUntilFinished runs inside finishTransitions
    bool m_groupsHeld;                  // +0x69: holdGroups/releaseGroups
};
typedef char GameWindowTransitionsSizeCheck[sizeof(GameWindowTransitionsHandler)==0x6C ? 1 : -1];
extern GameWindowTransitionsHandler *TheTransitionHandler;