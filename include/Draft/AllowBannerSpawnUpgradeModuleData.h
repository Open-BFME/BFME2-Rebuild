// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C6BB18).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// AllowBannerSpawnUpgradeModuleData.h: the INI data of AllowBannerSpawnUpgrade. SIZE 0x118.
// Target facts: ModuleFactory registers the module with operator new 0x118 and
// constructor RVA 0x00253487; buildFieldParse RVA 0x004B8565 adds VA 0x00C6BB18 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_ALLOW_BANNER_SPAWN_UPGRADE_MODULE_DATA_H
#define BFME2_ALLOW_BANNER_SPAWN_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"

class INI;
class MultiIniFieldParse;

class AllowBannerSpawnUpgradeModuleData : public ModuleData
{
public:
	AllowBannerSpawnUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
};
// SIZE 0x118
typedef char AllowBannerSpawnUpgradeModuleData_size[(sizeof(AllowBannerSpawnUpgradeModuleData) == 0x118) ? 1 : -1];
typedef char AllowBannerSpawnUpgradeModuleData_off_m_upgradeMuxData[(offsetof(AllowBannerSpawnUpgradeModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];

#endif
