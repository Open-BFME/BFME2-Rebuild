#pragma once
#include "Common/AsciiString.h"
#include "Common/AudioEventRef.h"
class GameWindow;
class GameInfo;
// Native ctor355C60 registers this at VA E01E1C using next+4; table814E4C
// has deleting dtor and pure update(int). WB names ProgressIndicator's dtor.
class ProgressIndicator
{
public:
    ProgressIndicator();
    virtual ~ProgressIndicator();
    virtual void update(int percent) = 0;
    ProgressIndicator *m_next;
};
extern ProgressIndicator *TheProgressIndicator;
// Rva00355D66 is LoadScreen: its ctor355D4E calls ProgressIndicator and
// table814E5C slot1 is the named LoadScreen::update355FF9. Six slots.
class LoadScreen : public ProgressIndicator
{
public:
    LoadScreen();
    virtual ~LoadScreen();
    virtual void update(int percent);
    virtual void init(GameInfo *) = 0;       // slot2
    virtual void reset() = 0;               // slot3
    virtual void processProgress(int, int) = 0; // slot4
    virtual bool isReadyToFinish();         // slot5, role inferred; base returns true
    GameWindow *m_loadScreen;               // 08, destroyed by dtor355D66
    bool m_ready;                          // 0C, frame-update guard355FF9
};
// Rva00355DC5: table814E74, init3556541 has the native ShellGameLoadScreen
// window-name strings, and slot1 is ShellGameLoadScreen::update356082.
class ShellGameLoadScreen : public LoadScreen
{
public:
    ShellGameLoadScreen();
    virtual ~ShellGameLoadScreen();
    virtual void update(int percent);
    virtual void init(GameInfo *);
    virtual void reset();
    virtual void processProgress(int, int);
    GameWindow *m_progressBar;              // 10, ctor355DAF/dtor355DC5
};
// Rva00355F3E: table814E8C slot2=MapTransferLoadScreen::init356B78.
// ctor355EFC and reset355F75 establish five 8-word arrays and three counters.
class MapTransferLoadScreen : public LoadScreen
{
public:
    MapTransferLoadScreen();
    virtual ~MapTransferLoadScreen();
    virtual void update(int percent);
    virtual void init(GameInfo *);
    virtual void reset();
    virtual void processProgress(int, int);
    void processProgress(int player, int percent, AsciiString fileName);
    void processTimeout(int player);
    void setCurrentFilename(AsciiString fileName);
    GameWindow *m_playerWindows[8];          // 10
    GameWindow *m_progressBars[8];           // 30
    GameWindow *m_fileWindows[8];            // 50
    int m_progress[8];                      // 70
    int m_timeout[8];                       // 90
    GameWindow *m_fileNameText;             // B0: setCurrentFilename 0x00356E8E sets its text
    GameWindow *m_timeoutText;              // B4: processTimeout 0x003564B6 sets its text
    int m_oldTimeout;                       // B8: the seconds processTimeout last showed
};
// Rva0035615D: table814EBC slot2=356889, paired with the WB diagnostic
// "StillImageLoadScreen::init". ctor35615D, GameLogic allocation23DDFA=28.
class StillImageLoadScreen : public LoadScreen
{
public:
    StillImageLoadScreen(const AsciiString &, const AudioEventInfoRef &,
        const AudioEventInfoRef &, bool);
    virtual ~StillImageLoadScreen();
    virtual void update(int percent);
    virtual void init(GameInfo *);
    virtual void reset();
    virtual void processProgress(int, int);
    virtual bool isReadyToFinish(); //355E4C: update audio; wait until voice handle stops
    void startAudio();             //356284, role inferred
    AsciiString m_imageName;        //10
    AudioEventInfoRef m_voice, m_music; //14,18
    unsigned int m_voiceHandle, m_musicHandle; //1C,20, invalid=1
    bool m_unknown24;
};
typedef char ProgressIndicatorSize[sizeof(ProgressIndicator)==8?1:-1];
typedef char LoadScreenSize[sizeof(LoadScreen)==0x10?1:-1];
typedef char ShellGameLoadScreenSize[sizeof(ShellGameLoadScreen)==0x14?1:-1];
typedef char MapTransferLoadScreenSize[sizeof(MapTransferLoadScreen)==0xBC?1:-1];
typedef char StillImageLoadScreenSize[sizeof(StillImageLoadScreen)==0x28?1:-1];
