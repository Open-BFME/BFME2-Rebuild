// SalvageCrateCollideModuleData.h: the INI data of SalvageCrateCollide. SIZE 0x80.
// Target facts: ModuleFactory registers the module with operator new 0x80 and
// constructor RVA 0x00255B7E; buildFieldParse RVA 0x00251299 calls CrateCollideModuleData::buildFieldParse, then adds VA 0x00BEFC28 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SALVAGE_CRATE_COLLIDE_MODULE_DATA_H
#define BFME2_SALVAGE_CRATE_COLLIDE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/CrateCollideModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base CrateCollideModuleData: the constructor calls its constructor on this. It ends at 0x5C (its own operator-new size);
// this class's members follow it.
typedef char SalvageCrateCollideModuleData_base_size[(sizeof(CrateCollideModuleData) == 0x5C) ? 1 : -1];

class SalvageCrateCollideModuleData : public CrateCollideModuleData
{
public:
	SalvageCrateCollideModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Real m_porterChance;                                   // +0x5C PorterChance; percent
	Real m_bannerChance;                                   // +0x60 BannerChance; percent
	Real m_levelUpChance;                                  // +0x64 LevelUpChance; percent
	Real m_levelUpRadius;                                  // +0x68 LevelUpRadius; percent
	Real m_resourceChance;                                 // +0x6C ResourceChance; percent
	Int m_minResource;                                     // +0x70 MinResource
	Int m_maxResource;                                     // +0x74 MaxResource
	AsciiString m_upgrade;                                 // +0x78 Upgrade
	Bool m_allowAIPickup;                                  // +0x7C AllowAIPickup
};
// SIZE 0x80
typedef char SalvageCrateCollideModuleData_size[(sizeof(SalvageCrateCollideModuleData) == 0x80) ? 1 : -1];
typedef char SalvageCrateCollideModuleData_off_m_porterChance[(offsetof(SalvageCrateCollideModuleData, m_porterChance) == 0x5C) ? 1 : -1];
typedef char SalvageCrateCollideModuleData_off_m_bannerChance[(offsetof(SalvageCrateCollideModuleData, m_bannerChance) == 0x60) ? 1 : -1];
typedef char SalvageCrateCollideModuleData_off_m_levelUpChance[(offsetof(SalvageCrateCollideModuleData, m_levelUpChance) == 0x64) ? 1 : -1];
typedef char SalvageCrateCollideModuleData_off_m_levelUpRadius[(offsetof(SalvageCrateCollideModuleData, m_levelUpRadius) == 0x68) ? 1 : -1];
typedef char SalvageCrateCollideModuleData_off_m_resourceChance[(offsetof(SalvageCrateCollideModuleData, m_resourceChance) == 0x6C) ? 1 : -1];
typedef char SalvageCrateCollideModuleData_off_m_minResource[(offsetof(SalvageCrateCollideModuleData, m_minResource) == 0x70) ? 1 : -1];
typedef char SalvageCrateCollideModuleData_off_m_maxResource[(offsetof(SalvageCrateCollideModuleData, m_maxResource) == 0x74) ? 1 : -1];
typedef char SalvageCrateCollideModuleData_off_m_upgrade[(offsetof(SalvageCrateCollideModuleData, m_upgrade) == 0x78) ? 1 : -1];
typedef char SalvageCrateCollideModuleData_off_m_allowAIPickup[(offsetof(SalvageCrateCollideModuleData, m_allowAIPickup) == 0x7C) ? 1 : -1];

#endif
