#pragma once
#include "Common/Coord3D.h"
#include <vector>

enum EvaEventID { EVA_INVALID = -1 };
// Name lead: WB EvaEventStatus::getPlayPositionForEvent at retail1DCE6B.
// Target query bodies1DD53D/1DD5A0/1DD604 and clearer1DCD3C establish the
// 52-byte element, float timers, two positions, flags and last-played fields.
// Names of individual fields describe those observed uses.
struct EvaEvent;
struct EvaEventStatus {
    void clear(); // 0x001DCD3C: the default-element builder resize calls and the per-element clear in reset/init
    void rva001DD7C1(EvaEvent *, const Coord3D *); // target records a played event and blocked peers
    void rva001DD240(const EvaEvent *); // target played-event timer reset; original name unknown
    bool rva001DCDAF(const EvaEvent *, const Coord3D *, const Coord3D *); // target report-state update
    float m_blockedTime;
    float m_aboutToPlayTime;
    Coord3D m_position;
    Coord3D m_secondPosition;
    bool m_hasPosition;
    bool m_hasSecondPosition;
    bool m_hasPlayed;
    unsigned char m_unk0x23;
    Coord3D m_lastPlayedPosition;
    unsigned int m_lastReallyPlayedFrame;
};
// EvaEvent is the structural name from map/types table007DC078 (OpenSage
// field-set match); original target C++ spelling remains unproven. Native
// field table proves0..13, flags2C/2D; vector indexing proves stride30.
// SideSound's container at14 is unresolved and is not accessed here.
struct EvaEvent {
    unsigned int m_timeBetweenEvents;
    unsigned int m_expirationTime;
    unsigned int m_quietTime;
    unsigned int m_millisecondsToWaitBeforePlaying;
    unsigned int m_priority;
    unsigned char m_sideSoundsUnrecovered[12];
    std::vector<EvaEventID> m_otherEvaEventsToBlock;
    bool m_alwaysPlayFromHomeBase;
    bool m_countAsJumpToLocation;
    bool m_unk0x2e;
    unsigned char m_unk0x2f;
};
typedef char EvaEventStatusSize[(sizeof(EvaEventStatus)==0x34)?1:-1];
typedef char EvaEventSize[(sizeof(EvaEvent)==0x30)?1:-1];

// Target1DD468 reads id0, presence flags4/5 and positions8/14; size20.
// EvaEventReport is the named argument type in the retail-row identity.
struct EvaEventReport {
    EvaEventID m_eventID;
    bool m_hasPosition;
    bool m_hasSecondPosition;
    unsigned char m_pad06[2];
    Coord3D m_position;
    Coord3D m_secondPosition;
};
typedef char EvaEventReportSize[sizeof(EvaEventReport)==0x20 ? 1 : -1];

