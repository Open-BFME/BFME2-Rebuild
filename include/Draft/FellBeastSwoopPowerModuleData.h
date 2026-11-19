// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BEFE14).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// FellBeastSwoopPowerModuleData.h: the INI data of FellBeastSwoopPower. SIZE 0xD0.
// Target facts: ModuleFactory registers the module with operator new 0xD0 and
// constructor RVA 0x004923E2; buildFieldParse RVA 0x00252398 calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00BEFE14 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_FELL_BEAST_SWOOP_POWER_MODULE_DATA_H
#define BFME2_FELL_BEAST_SWOOP_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"

class INI;
class MultiIniFieldParse;
class WeaponTemplate;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char FellBeastSwoopPowerModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class FellBeastSwoopPowerModuleData : public SpecialAbilityUpdateModuleData
{
public:
	FellBeastSwoopPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	const WeaponTemplate * m_specialWeapon;                // +0xC8 SpecialWeapon
	Int m_whichSpecialWeapon;                              // +0xCC WhichSpecialWeapon
};
// SIZE 0xD0
typedef char FellBeastSwoopPowerModuleData_size[(sizeof(FellBeastSwoopPowerModuleData) == 0xD0) ? 1 : -1];
typedef char FellBeastSwoopPowerModuleData_off_m_specialWeapon[(offsetof(FellBeastSwoopPowerModuleData, m_specialWeapon) == 0xC8) ? 1 : -1];
typedef char FellBeastSwoopPowerModuleData_off_m_whichSpecialWeapon[(offsetof(FellBeastSwoopPowerModuleData, m_whichSpecialWeapon) == 0xCC) ? 1 : -1];

#endif
