#pragma once
#include "GameLogic/Module/ModuleData.h"
#include "Common/OpaqueRefElement4.h"
class OneRingPenaltyUpdateModuleData : public ModuleData
{
public:
 OneRingPenaltyUpdateModuleData();
 virtual ~OneRingPenaltyUpdateModuleData();
 static void buildFieldParse(MultiIniFieldParse &);
 AsciiString m_specialObjectName; // 0x08
 unsigned m_ringTimeBeforeSpawning; // 0x0C
 unsigned m_timeSpentRoamingAround; // 0x10
 unsigned m_timeRingPowerSuppressed; // 0x14
 float m_startingDistanceFromMe; // 0x18
 unsigned m_timeFrozenFromPenalty; // 0x1C
 OpaqueRefElement4 m_discoveredSound; // 0x20
};
// SIZE 0x24
