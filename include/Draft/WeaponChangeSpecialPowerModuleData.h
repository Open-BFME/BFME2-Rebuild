// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5D0F0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// WeaponChangeSpecialPowerModuleData.h: the INI data of WeaponChangeSpecialPowerModule. SIZE 0x9C.
// Target facts: ModuleFactory registers the module with operator new 0x9C and
// constructor RVA 0x004C4257; buildFieldParse RVA 0x004C3FDD calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5D0F0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_WEAPON_CHANGE_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_WEAPON_CHANGE_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModuleData.h"
#include "Common/BitFlags.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char WeaponChangeSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class WeaponChangeSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	WeaponChangeSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	WeaponSetFlags m_flagsUsedForToggle;                   // +0x7C FlagsUsedForToggle; BitFlags<104>
	Int m_toggleOnSleepFrames;                             // +0x8C ToggleOnSleepFrames
	Int m_toggleOffSleepFrames;                            // +0x90 ToggleOffSleepFrames
	AsciiString m_toggleOnAttributeModifier;               // +0x94 ToggleOnAttributeModifier
	AsciiString m_toggleOffAttributeModifier;              // +0x98 ToggleOffAttributeModifier
};
// SIZE 0x9C
typedef char WeaponChangeSpecialPowerModuleData_size[(sizeof(WeaponChangeSpecialPowerModuleData) == 0x9C) ? 1 : -1];
typedef char WeaponChangeSpecialPowerModuleData_off_m_flagsUsedForToggle[(offsetof(WeaponChangeSpecialPowerModuleData, m_flagsUsedForToggle) == 0x7C) ? 1 : -1];
typedef char WeaponChangeSpecialPowerModuleData_off_m_toggleOnSleepFrames[(offsetof(WeaponChangeSpecialPowerModuleData, m_toggleOnSleepFrames) == 0x8C) ? 1 : -1];
typedef char WeaponChangeSpecialPowerModuleData_off_m_toggleOffSleepFrames[(offsetof(WeaponChangeSpecialPowerModuleData, m_toggleOffSleepFrames) == 0x90) ? 1 : -1];
typedef char WeaponChangeSpecialPowerModuleData_off_m_toggleOnAttributeModifier[(offsetof(WeaponChangeSpecialPowerModuleData, m_toggleOnAttributeModifier) == 0x94) ? 1 : -1];
typedef char WeaponChangeSpecialPowerModuleData_off_m_toggleOffAttributeModifier[(offsetof(WeaponChangeSpecialPowerModuleData, m_toggleOffAttributeModifier) == 0x98) ? 1 : -1];

#endif
