#pragma once
#include <vector>
#include "Common/SubsystemInterface.h"
#include "Common/Snapshot.h"
#include "Common/StlList.h"
#include "Common/StlHashMap.h"
#include "GameClient/EvaEvent.h"
// Retail constructor0x1DF91B/destructor0x1DF057 establish SubsystemInterface+0,
// Snapshot+C, allocation size98. The data at80..94 is the MiscEvaData table.
// Container evidence (retail game.dat): +0x10 is an RB tree of frame-keyed delayed reports
// (update 0x001DE156 compares node+0x10 with the logic frame and passes node+0x14 to
// internalReportEvaEvent), 0xC bytes with its node count at +0x14. +0x1C and +0x28 are
// vectors of 0x30-byte EvaEvent (update divides by 0x30; resize 0x001DF88C builds a 0x30
// default on the stack). +0x34 and +0x48 are 0x14-byte hash_maps from AsciiString to an
// event id (messageToName 0x001DEB8A reads node+4 as the string and node+8 as the id;
// iniScanEvaEventID 0x001DE9C4 and rva001DE78E look names up in +0x48 and +0x34).
// +0x5C is a vector of 0x34-byte EvaEventStatus, +0x68 a list of frames with its
// current-position iterator copied to +0x6C by the constructor. Under `// stlport46`
// these are the real STLport types; elsewhere they are byte storage of the same size.
class INI;
class AsciiString;
#if defined(_STLPORT_VERSION)
#include <set>
#include "Common/STLTypedefs.h"
#include "Common/RTSEqualTo.h"
// Role-inferred: one delayed report is its due frame followed by the report record.
struct EvaDelayedReport {
    unsigned int m_frame; // +0x00 (node+0x10)
    EvaEventReport m_report; // +0x04 (node+0x14)
};
struct EvaDelayedReportLess {
    bool operator()(const EvaDelayedReport &a, const EvaDelayedReport &b) const { return a.m_frame < b.m_frame; }
};
typedef _STL::hash_map<AsciiString, int, rts::hash<AsciiString>, rts::equal_to<AsciiString> > EvaNameMap;
#endif
class Eva : public SubsystemInterface, public Snapshot {
public:
    Eva();
    bool internalReportEvaEvent(const EvaEventReport *);
    bool reportEvaEvent(EvaEventID, const Coord3D *, const Coord3D *); // 0x001DE2DA; EvaEventFXNugget::doFXObj 0x001E0028 calls it
    void countEvaEventAsPlayed(EvaEventID);
    int iniScanEvaEventID(const char *);
    int rva001DE78E(const AsciiString *);
    void rva001DDAE1(Coord3D *);	// 0x001DDAE1: the map spells it on Eva (ScriptActions::doObjectRadarCreateEvent 0x003BB79D and doTeamRadarCreateEvent 0x003C00CE call it with a position after the radar event)
    AsciiString messageToName(int);
    void xferEvaEventID(Xfer *, int *);
    static void parseOtherEvaEventsToBlock(INI *, void *, void *, const void *);
    static void parseEvaEventID(INI *, void *, void *, const void *);
    void setEvaEnabled(bool); // donor spelling; target clears pending timers on disable
    bool getLastPlayedLocationForEvaEvent(EvaEventID, Coord3D *) const; // WB name lead1DD670
    bool isEventBlockedByTimeout(EvaEventID) const;
    bool isEventAboutToPlay(EvaEventID) const;
    bool getLastReallyPlayedFrameForEvaEvent(EvaEventID, unsigned int *) const;
    virtual ~Eva();
    virtual void init();
    virtual void reset();
    virtual void update();
    virtual const char *GetSnapshotName() const;
protected:
    virtual void loadPostProcess();
    virtual void xfer(Xfer *);
public:
#if defined(_STLPORT_VERSION)
    _STL::set<EvaDelayedReport, EvaDelayedReportLess> m_delayedReports; // +0x10
    _STL::vector<EvaEvent> m_allEventInfos; // +0x1C
    _STL::vector<EvaEvent> m_iniEventInfos; // +0x28
    EvaNameMap m_eventNameMap; // +0x34
    EvaNameMap m_iniEventNameMap; // +0x48
    _STL::vector<EvaEventStatus> m_eventStatus; // +0x5C
    _STL::list<unsigned int> m_playedFrames; // +0x68
    _STL::list<unsigned int>::iterator m_playedFramesPosition; // +0x6C
#else
    unsigned char m_storage0x10[0xC];
    unsigned char m_storage0x1C[0xC];
    unsigned char m_storage0x28[0xC];
    unsigned char m_storage0x34[0x14];
    unsigned char m_storage0x48[0x14];
    unsigned char m_storage0x5C[0xC];
    unsigned char m_storage0x68[4];
    unsigned int m_storage0x6C;
#endif
    int m_unk0x70;
    unsigned int m_unk0x74;
    int m_unk0x78;
    bool m_enabled; // +7C; xfer's secondary receiver accesses +70
    unsigned char m_unk0x7d[3];
    float m_enemySightedMaxVoicePositionScanRange;
    unsigned int m_enemyCampDestroyedDamageTimeout;
    unsigned int m_friendlyCampDestroyedDamageTimeout;
    unsigned int m_maxMillisecondsToKeepJumpToEvents;
    unsigned int m_maxMillisecondsBeforeResettingLastJumpTo;
    float m_minDistanceBetweenJumpToEvents;
};
// SIZE 0x98
typedef char EvaSizeCheck[sizeof(Eva)==0x98?1:-1];
extern Eva *TheEva;	// VA 0x00DFDC30
