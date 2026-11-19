// AssistedTargetingUpdateModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x00486E7C);
// name, size, bases and virtual order are retail facts given below.
// AssistedTargetingUpdateModuleData.h: the INI data of AssistedTargetingUpdate. SIZE 0x18.
// Target facts: ModuleFactory registers the module with operator new 0x18 and
// constructor RVA ?; buildFieldParse RVA 0x00486E7C adds VA 0x00C4AFE8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_ASSISTED_TARGETING_UPDATE_MODULE_DATA_H
#define BFME2_ASSISTED_TARGETING_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;
class ThingTemplate;

class AssistedTargetingUpdateModuleData : public ModuleData
{
public:
	AssistedTargetingUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Int m_assistingClipSize;                               // +0x8 AssistingClipSize
	Int m_assistingWeaponSlot;                             // +0xC AssistingWeaponSlot; lookup-list value (enum)
	const ThingTemplate * m_laserFromAssisted;             // +0x10 LaserFromAssisted
	const ThingTemplate * m_laserToTarget;                 // +0x14 LaserToTarget
};
// SIZE 0x18
typedef char AssistedTargetingUpdateModuleData_size[(sizeof(AssistedTargetingUpdateModuleData) == 0x18) ? 1 : -1];
typedef char AssistedTargetingUpdateModuleData_off_m_assistingClipSize[(offsetof(AssistedTargetingUpdateModuleData, m_assistingClipSize) == 0x8) ? 1 : -1];
typedef char AssistedTargetingUpdateModuleData_off_m_assistingWeaponSlot[(offsetof(AssistedTargetingUpdateModuleData, m_assistingWeaponSlot) == 0xC) ? 1 : -1];
typedef char AssistedTargetingUpdateModuleData_off_m_laserFromAssisted[(offsetof(AssistedTargetingUpdateModuleData, m_laserFromAssisted) == 0x10) ? 1 : -1];
typedef char AssistedTargetingUpdateModuleData_off_m_laserToTarget[(offsetof(AssistedTargetingUpdateModuleData, m_laserToTarget) == 0x14) ? 1 : -1];

#endif
