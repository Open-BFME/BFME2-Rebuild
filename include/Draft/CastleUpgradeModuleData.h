// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C588C4).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// CastleUpgradeModuleData.h: the INI data of CastleUpgrade. SIZE 0x120.
// Target facts: ModuleFactory registers the module with operator new 0x120 and
// constructor RVA 0x00255836; buildFieldParse RVA 0x004B6847 adds VA 0x00C588C4 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_CASTLE_UPGRADE_MODULE_DATA_H
#define BFME2_CASTLE_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class CastleUpgradeModuleData : public ModuleData
{
public:
	CastleUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	AsciiString m_upgrade;                                 // +0x118 Upgrade
	Real m_wallUpgradeRadius;                              // +0x11C WallUpgradeRadius
};
// SIZE 0x120
typedef char CastleUpgradeModuleData_size[(sizeof(CastleUpgradeModuleData) == 0x120) ? 1 : -1];
typedef char CastleUpgradeModuleData_off_m_upgradeMuxData[(offsetof(CastleUpgradeModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char CastleUpgradeModuleData_off_m_upgrade[(offsetof(CastleUpgradeModuleData, m_upgrade) == 0x118) ? 1 : -1];
typedef char CastleUpgradeModuleData_off_m_wallUpgradeRadius[(offsetof(CastleUpgradeModuleData, m_wallUpgradeRadius) == 0x11C) ? 1 : -1];

#endif
