// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C574AC).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// LevelUpUpgradeModuleData.h: the INI data of LevelUpUpgrade. SIZE 0x120.
// Target facts: ModuleFactory registers the module with operator new 0x120 and
// constructor RVA 0x0025467E; buildFieldParse RVA 0x004B3DA6 adds VA 0x00C574AC at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_LEVEL_UP_UPGRADE_MODULE_DATA_H
#define BFME2_LEVEL_UP_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"

class INI;
class MultiIniFieldParse;

class LevelUpUpgradeModuleData : public ModuleData
{
public:
	LevelUpUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	Int m_levelsToGain;                                    // +0x118 LevelsToGain
	Int m_levelCap;                                        // +0x11C LevelCap
};
// SIZE 0x120
typedef char LevelUpUpgradeModuleData_size[(sizeof(LevelUpUpgradeModuleData) == 0x120) ? 1 : -1];
typedef char LevelUpUpgradeModuleData_off_m_upgradeMuxData[(offsetof(LevelUpUpgradeModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char LevelUpUpgradeModuleData_off_m_levelsToGain[(offsetof(LevelUpUpgradeModuleData, m_levelsToGain) == 0x118) ? 1 : -1];
typedef char LevelUpUpgradeModuleData_off_m_levelCap[(offsetof(LevelUpUpgradeModuleData, m_levelCap) == 0x11C) ? 1 : -1];

#endif
