// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5E23C).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_upgradeName.
// PlayerUpgradeSpecialPowerModuleData.h: the INI data of PlayerUpgradeSpecialPower. SIZE 0x88.
// Target facts: ModuleFactory registers the module with operator new 0x88 and
// constructor RVA 0x004C7D68; buildFieldParse RVA 0x004C7E48 calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5E23C at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_upgradeName +0x7C: Rva004C7E09_ParseUpgradeName 0x004C7E09
#pragma once
#ifndef BFME2_PLAYER_UPGRADE_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_PLAYER_UPGRADE_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"

class INI;
class MultiIniFieldParse;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char PlayerUpgradeSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class PlayerUpgradeSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	PlayerUpgradeSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedByte m_upgradeName[0xC];                       // +0x7C UpgradeName; unresolved: Rva004C7E09_ParseUpgradeName
};
// SIZE 0x88
typedef char PlayerUpgradeSpecialPowerModuleData_size[(sizeof(PlayerUpgradeSpecialPowerModuleData) == 0x88) ? 1 : -1];
typedef char PlayerUpgradeSpecialPowerModuleData_off_m_upgradeName[(offsetof(PlayerUpgradeSpecialPowerModuleData, m_upgradeName) == 0x7C) ? 1 : -1];

#endif
