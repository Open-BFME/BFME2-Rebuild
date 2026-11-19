// Promoted from Draft by f03: the class derives from UpgradeModuleData (its constructor calls 0x00253487 first, and
// the destructor 0x004B5C80 is implicit: one trailing ModuleData vptr store); Percentage (0x004B60D5) parses a percent to a Real and push_backs it into a
// vector<Real> at +0x11C (the destructor 0x004B5C80 frees its buffer inline; the constructor builds it as an empty
// vector). The constructor 0x004B5BAC initialises the ObjectFilter from two empty KindOf masks, sets the label to
// ">UNSPECIFIED<" and clears the three Bools.
// CostModifierUpgradeModuleData.h: the INI data of CostModifierUpgrade. SIZE 0x13C.
// Target facts: ModuleFactory registers the module with operator new 0x13C and
// constructor RVA 0x004B5BAC; buildFieldParse RVA 0x004B5CE9 adds VA 0x00C582D8 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_COST_MODIFIER_UPGRADE_MODULE_DATA_H
#define BFME2_COST_MODIFIER_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "GameLogic/ObjectFilter.h"
#include "Common/AsciiString.h"
#include "Common/Vector.h"

class INI;
class MultiIniFieldParse;

class CostModifierUpgradeModuleData : public UpgradeModuleData
{
public:
	CostModifierUpgradeModuleData();										// 0x004B5BAC
	static void buildFieldParse(MultiIniFieldParse &parse);

	ObjectFilter m_objectFilter;                           // +0x118 ObjectFilter
	_STL::vector<Real> m_percentage;                       // +0x11C Percentage; INI::Rva004B60D5_ParsePercentage
	Bool m_upgradeDiscount;                                // +0x128 UpgradeDiscount
	Bool m_startsActive;                                   // +0x129 StartsActive
	Bool m_slaughter;                                      // +0x12A Slaughter
	AsciiString m_labelForPalantirString;                  // +0x12C LabelForPalantirString
	_STL::vector<AsciiString> m_applyToTheseUpgrades;      // +0x130 ApplyToTheseUpgrades
};
// SIZE 0x13C
typedef char CostModifierUpgradeModuleData_size[(sizeof(CostModifierUpgradeModuleData) == 0x13C) ? 1 : -1];
typedef char CostModifierUpgradeModuleData_off_m_objectFilter[(offsetof(CostModifierUpgradeModuleData, m_objectFilter) == 0x118) ? 1 : -1];
typedef char CostModifierUpgradeModuleData_off_m_percentage[(offsetof(CostModifierUpgradeModuleData, m_percentage) == 0x11C) ? 1 : -1];
typedef char CostModifierUpgradeModuleData_off_m_upgradeDiscount[(offsetof(CostModifierUpgradeModuleData, m_upgradeDiscount) == 0x128) ? 1 : -1];
typedef char CostModifierUpgradeModuleData_off_m_startsActive[(offsetof(CostModifierUpgradeModuleData, m_startsActive) == 0x129) ? 1 : -1];
typedef char CostModifierUpgradeModuleData_off_m_slaughter[(offsetof(CostModifierUpgradeModuleData, m_slaughter) == 0x12A) ? 1 : -1];
typedef char CostModifierUpgradeModuleData_off_m_labelForPalantirString[(offsetof(CostModifierUpgradeModuleData, m_labelForPalantirString) == 0x12C) ? 1 : -1];
typedef char CostModifierUpgradeModuleData_off_m_applyToTheseUpgrades[(offsetof(CostModifierUpgradeModuleData, m_applyToTheseUpgrades) == 0x130) ? 1 : -1];

#endif
