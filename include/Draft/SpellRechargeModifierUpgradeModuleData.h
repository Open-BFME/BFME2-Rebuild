// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C58460).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_percentage.
// SpellRechargeModifierUpgradeModuleData.h: the INI data of SpellRechargeModifierUpgrade. SIZE 0x12C.
// Target facts: ModuleFactory registers the module with operator new 0x12C and
// constructor RVA 0x004B6011; buildFieldParse RVA 0x004B60FA adds VA 0x00C58460 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_percentage +0x118: INI::Rva004B60D5_ParsePercentage 0x004B60D5
#pragma once
#ifndef BFME2_SPELL_RECHARGE_MODIFIER_UPGRADE_MODULE_DATA_H
#define BFME2_SPELL_RECHARGE_MODIFIER_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "Common/AsciiString.h"
#include <vector>

class INI;
class MultiIniFieldParse;

class SpellRechargeModifierUpgradeModuleData : public ModuleData
{
public:
	SpellRechargeModifierUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	_STL::vector<Real> m_percentage;                       // +0x118 Percentage: the modifier table SpellRechargeModifierUpgrade indexes by the player's count (0x004B5F9F, clamped to its size)
	Bool m_startsActive;                                   // +0x124 StartsActive
	AsciiString m_labelForPalantirString;                  // +0x128 LabelForPalantirString
};
// SIZE 0x12C
typedef char SpellRechargeModifierUpgradeModuleData_size[(sizeof(SpellRechargeModifierUpgradeModuleData) == 0x12C) ? 1 : -1];
typedef char SpellRechargeModifierUpgradeModuleData_off_m_upgradeMuxData[(offsetof(SpellRechargeModifierUpgradeModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char SpellRechargeModifierUpgradeModuleData_off_m_percentage[(offsetof(SpellRechargeModifierUpgradeModuleData, m_percentage) == 0x118) ? 1 : -1];
typedef char SpellRechargeModifierUpgradeModuleData_off_m_startsActive[(offsetof(SpellRechargeModifierUpgradeModuleData, m_startsActive) == 0x124) ? 1 : -1];
typedef char SpellRechargeModifierUpgradeModuleData_off_m_labelForPalantirString[(offsetof(SpellRechargeModifierUpgradeModuleData, m_labelForPalantirString) == 0x128) ? 1 : -1];

#endif
