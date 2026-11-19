// TerrainResourceBehaviorModuleData.h: the INI data of TerrainResourceBehavior. SIZE 0x24.
// Target facts: ModuleFactory registers the module with operator new 0x24 and
// constructor RVA 0x0048202F; buildFieldParse RVA 0x0048208A adds VA 0x00C494F0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_TERRAIN_RESOURCE_BEHAVIOR_MODULE_DATA_H
#define BFME2_TERRAIN_RESOURCE_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;
class UpgradeTemplate;

class TerrainResourceBehaviorModuleData : public ModuleData
{
public:
	TerrainResourceBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);
	virtual const void *slot24() const { return this; }	// slot 24: retail table 0x00C49440 has 0x0047A6A9 (`mov eax,ecx`), not ModuleData's 0x000D43D0

	Real m_radius;                                         // +0x8 Radius
	Int m_maxIncome;                                       // +0xC MaxIncome
	UnsignedInt m_incomeInterval;                          // +0x10 IncomeInterval; duration, frames
	Bool m_highPriority;                                   // +0x14 HighPriority
	Bool m_visible;                                        // +0x15 Visible
	ObjectFilter m_upgradeMustBePresent;                   // +0x18 UpgradeMustBePresent
	const UpgradeTemplate * m_upgrade;                     // +0x1C Upgrade
	Real m_upgradeBonusPercent;                            // +0x20 UpgradeBonusPercent; percent
};
// SIZE 0x24
typedef char TerrainResourceBehaviorModuleData_size[(sizeof(TerrainResourceBehaviorModuleData) == 0x24) ? 1 : -1];
typedef char TerrainResourceBehaviorModuleData_off_m_radius[(offsetof(TerrainResourceBehaviorModuleData, m_radius) == 0x8) ? 1 : -1];
typedef char TerrainResourceBehaviorModuleData_off_m_maxIncome[(offsetof(TerrainResourceBehaviorModuleData, m_maxIncome) == 0xC) ? 1 : -1];
typedef char TerrainResourceBehaviorModuleData_off_m_incomeInterval[(offsetof(TerrainResourceBehaviorModuleData, m_incomeInterval) == 0x10) ? 1 : -1];
typedef char TerrainResourceBehaviorModuleData_off_m_highPriority[(offsetof(TerrainResourceBehaviorModuleData, m_highPriority) == 0x14) ? 1 : -1];
typedef char TerrainResourceBehaviorModuleData_off_m_visible[(offsetof(TerrainResourceBehaviorModuleData, m_visible) == 0x15) ? 1 : -1];
typedef char TerrainResourceBehaviorModuleData_off_m_upgradeMustBePresent[(offsetof(TerrainResourceBehaviorModuleData, m_upgradeMustBePresent) == 0x18) ? 1 : -1];
typedef char TerrainResourceBehaviorModuleData_off_m_upgrade[(offsetof(TerrainResourceBehaviorModuleData, m_upgrade) == 0x1C) ? 1 : -1];
typedef char TerrainResourceBehaviorModuleData_off_m_upgradeBonusPercent[(offsetof(TerrainResourceBehaviorModuleData, m_upgradeBonusPercent) == 0x20) ? 1 : -1];

#endif
