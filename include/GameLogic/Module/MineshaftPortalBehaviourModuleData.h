// MineshaftPortalBehaviourModuleData.h: the INI data of MineshaftPortalBehaviour. SIZE 0x11C.
// Target facts: ModuleFactory registers the module with operator new 0x11C and
// constructor RVA 0x00372DB8; buildFieldParse RVA 0x00372DD8 adds VA 0x00C42B10 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_MINESHAFT_PORTAL_BEHAVIOUR_MODULE_DATA_H
#define BFME2_MINESHAFT_PORTAL_BEHAVIOUR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"

class INI;
class MultiIniFieldParse;

class MineshaftPortalBehaviourModuleData : public ModuleData
{
public:
	MineshaftPortalBehaviourModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	Bool m_allowEnemies;                                   // +0x118 AllowEnemies
	Bool m_allowNonSkirmishAIUnits;                        // +0x119 AllowNonSkirmishAIUnits
};
// SIZE 0x11C
typedef char MineshaftPortalBehaviourModuleData_size[(sizeof(MineshaftPortalBehaviourModuleData) == 0x11C) ? 1 : -1];
typedef char MineshaftPortalBehaviourModuleData_off_m_upgradeMuxData[(offsetof(MineshaftPortalBehaviourModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char MineshaftPortalBehaviourModuleData_off_m_allowEnemies[(offsetof(MineshaftPortalBehaviourModuleData, m_allowEnemies) == 0x118) ? 1 : -1];
typedef char MineshaftPortalBehaviourModuleData_off_m_allowNonSkirmishAIUnits[(offsetof(MineshaftPortalBehaviourModuleData, m_allowNonSkirmishAIUnits) == 0x119) ? 1 : -1];

#endif
