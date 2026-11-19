// RunOffMapBehaviorModuleData.h: the INI data of RunOffMapBehavior. SIZE 0x1C.
// Target facts: ModuleFactory registers the module with operator new 0x1C and
// constructor RVA 0x00254ECE; buildFieldParse RVA 0x0046216A adds VA 0x00C42F70 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_RUN_OFF_MAP_BEHAVIOR_MODULE_DATA_H
#define BFME2_RUN_OFF_MAP_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/Coord2D.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class RunOffMapBehaviorModuleData : public ModuleData
{
public:
	RunOffMapBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Coord2D m_runToLocation;                               // +0x8 RunToLocation
	Bool m_requiresSpecificTrigger;                        // +0x10 RequiresSpecificTrigger
	AsciiString m_runOffMapWaypointName;                   // +0x14 RunOffMapWaypointName
	Bool m_dieOnMap;                                       // +0x18 DieOnMap
};
// SIZE 0x1C
typedef char RunOffMapBehaviorModuleData_size[(sizeof(RunOffMapBehaviorModuleData) == 0x1C) ? 1 : -1];
typedef char RunOffMapBehaviorModuleData_off_m_runToLocation[(offsetof(RunOffMapBehaviorModuleData, m_runToLocation) == 0x8) ? 1 : -1];
typedef char RunOffMapBehaviorModuleData_off_m_requiresSpecificTrigger[(offsetof(RunOffMapBehaviorModuleData, m_requiresSpecificTrigger) == 0x10) ? 1 : -1];
typedef char RunOffMapBehaviorModuleData_off_m_runOffMapWaypointName[(offsetof(RunOffMapBehaviorModuleData, m_runOffMapWaypointName) == 0x14) ? 1 : -1];
typedef char RunOffMapBehaviorModuleData_off_m_dieOnMap[(offsetof(RunOffMapBehaviorModuleData, m_dieOnMap) == 0x18) ? 1 : -1];

#endif
