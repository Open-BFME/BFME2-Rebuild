// PickupStuffUpdateModuleData.h: the INI data of PickupStuffUpdate. SIZE 0x18.
// Target facts: ModuleFactory registers the module with operator new 0x18 and
// constructor RVA 0x00491E37; buildFieldParse RVA 0x00491E87 adds VA 0x00C4DE90 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_PICKUP_STUFF_UPDATE_MODULE_DATA_H
#define BFME2_PICKUP_STUFF_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

class PickupStuffUpdateModuleData : public ModuleData
{
public:
	PickupStuffUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Bool m_skirmishAIOnly;                                 // +0x8 SkirmishAIOnly
	Real m_scanRange;                                      // +0xC ScanRange
	ObjectFilter m_stuffToPickUp;                          // +0x10 StuffToPickUp
	Real m_scanIntervalSeconds;                            // +0x14 ScanIntervalSeconds
};
// SIZE 0x18
typedef char PickupStuffUpdateModuleData_size[(sizeof(PickupStuffUpdateModuleData) == 0x18) ? 1 : -1];
typedef char PickupStuffUpdateModuleData_off_m_skirmishAIOnly[(offsetof(PickupStuffUpdateModuleData, m_skirmishAIOnly) == 0x8) ? 1 : -1];
typedef char PickupStuffUpdateModuleData_off_m_scanRange[(offsetof(PickupStuffUpdateModuleData, m_scanRange) == 0xC) ? 1 : -1];
typedef char PickupStuffUpdateModuleData_off_m_stuffToPickUp[(offsetof(PickupStuffUpdateModuleData, m_stuffToPickUp) == 0x10) ? 1 : -1];
typedef char PickupStuffUpdateModuleData_off_m_scanIntervalSeconds[(offsetof(PickupStuffUpdateModuleData, m_scanIntervalSeconds) == 0x14) ? 1 : -1];

#endif
