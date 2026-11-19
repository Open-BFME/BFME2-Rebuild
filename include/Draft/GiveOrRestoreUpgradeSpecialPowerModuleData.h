// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5F8C0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// GiveOrRestoreUpgradeSpecialPowerModuleData.h: the INI data of GiveOrRestoreUpgradeSpecialPower. SIZE 0xE0.
// Target facts: ModuleFactory registers the module with operator new 0xE0 and
// constructor RVA 0x004CD0B1; buildFieldParse RVA 0x004CD061 calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00C5F8C0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_GIVE_OR_RESTORE_UPGRADE_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_GIVE_OR_RESTORE_UPGRADE_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"
#include "Common/AsciiString.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char GiveOrRestoreUpgradeSpecialPowerModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class GiveOrRestoreUpgradeSpecialPowerModuleData : public SpecialAbilityUpdateModuleData
{
public:
	GiveOrRestoreUpgradeSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	AsciiString m_commandButton;                           // +0xC8 CommandButton
	AsciiString m_upgradeToGive;                           // +0xCC UpgradeToGive
	WeaponSetFlags m_flagsUsedForToggle;                   // +0xD0 FlagsUsedForToggle; BitFlags<104>
};
// SIZE 0xE0
typedef char GiveOrRestoreUpgradeSpecialPowerModuleData_size[(sizeof(GiveOrRestoreUpgradeSpecialPowerModuleData) == 0xE0) ? 1 : -1];
typedef char GiveOrRestoreUpgradeSpecialPowerModuleData_off_m_commandButton[(offsetof(GiveOrRestoreUpgradeSpecialPowerModuleData, m_commandButton) == 0xC8) ? 1 : -1];
typedef char GiveOrRestoreUpgradeSpecialPowerModuleData_off_m_upgradeToGive[(offsetof(GiveOrRestoreUpgradeSpecialPowerModuleData, m_upgradeToGive) == 0xCC) ? 1 : -1];
typedef char GiveOrRestoreUpgradeSpecialPowerModuleData_off_m_flagsUsedForToggle[(offsetof(GiveOrRestoreUpgradeSpecialPowerModuleData, m_flagsUsedForToggle) == 0xD0) ? 1 : -1];

#endif
