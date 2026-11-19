// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C50DC8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// AttributeModifierAuraUpdateModuleData.h: the INI data of AttributeModifierAuraUpdate. SIZE 0x154.
// Target facts: ModuleFactory registers the module with operator new 0x154 and
// constructor RVA 0x0049BBD5; buildFieldParse RVA 0x0049BBB3 adds VA 0x00C50DC8 at +0x0 and UpgradeMuxData at +0x28.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_ATTRIBUTE_MODIFIER_AURA_UPDATE_MODULE_DATA_H
#define BFME2_ATTRIBUTE_MODIFIER_AURA_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"
#include "GameLogic/ObjectFilter.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;
class FXList;

class AttributeModifierAuraUpdateModuleData : public ModuleData
{
public:
	AttributeModifierAuraUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	AsciiString m_bonusName;                               // +0x8 BonusName
	UnsignedByte m_unk0xC[0xC];
	UnsignedInt m_refreshDelay;                            // +0x18 RefreshDelay; duration, frames
	Real m_range;                                          // +0x1C Range
	Bool m_allowPowerWhenAttacking;                        // +0x20 AllowPowerWhenAttacking
	Bool m_targetEnemy;                                    // +0x21 TargetEnemy
	ObjectFilter m_objectFilter;                           // +0x24 ObjectFilter
	UpgradeMuxData m_upgradeMuxData;                       // +0x28; added by buildFieldParse at +0x28
	Bool m_startsActive;                                   // +0x138 StartsActive
	UnsignedInt m_requiredConditions;                      // +0x13C RequiredConditions; bit string
	BitFlags<15> m_antiCategory;                           // +0x140 AntiCategory; BitFlags<15>
	const FXList * m_antiFX;                               // +0x144 AntiFX
	Bool m_affectGood;                                     // +0x148 AffectGood
	Bool m_affectEvil;                                     // +0x149 AffectEvil
	Bool m_runWhileDead;                                   // +0x14A RunWhileDead
	Bool m_allowSelf;                                      // +0x14B AllowSelf
	Bool m_affectContainedOnly;                            // +0x14C AffectContainedOnly
	Int m_maxActiveRank;                                   // +0x150 MaxActiveRank
};
// SIZE 0x154
typedef char AttributeModifierAuraUpdateModuleData_size[(sizeof(AttributeModifierAuraUpdateModuleData) == 0x154) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_bonusName[(offsetof(AttributeModifierAuraUpdateModuleData, m_bonusName) == 0x8) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_refreshDelay[(offsetof(AttributeModifierAuraUpdateModuleData, m_refreshDelay) == 0x18) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_range[(offsetof(AttributeModifierAuraUpdateModuleData, m_range) == 0x1C) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_allowPowerWhenAttacking[(offsetof(AttributeModifierAuraUpdateModuleData, m_allowPowerWhenAttacking) == 0x20) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_targetEnemy[(offsetof(AttributeModifierAuraUpdateModuleData, m_targetEnemy) == 0x21) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_objectFilter[(offsetof(AttributeModifierAuraUpdateModuleData, m_objectFilter) == 0x24) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_upgradeMuxData[(offsetof(AttributeModifierAuraUpdateModuleData, m_upgradeMuxData) == 0x28) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_startsActive[(offsetof(AttributeModifierAuraUpdateModuleData, m_startsActive) == 0x138) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_requiredConditions[(offsetof(AttributeModifierAuraUpdateModuleData, m_requiredConditions) == 0x13C) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_antiCategory[(offsetof(AttributeModifierAuraUpdateModuleData, m_antiCategory) == 0x140) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_antiFX[(offsetof(AttributeModifierAuraUpdateModuleData, m_antiFX) == 0x144) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_affectGood[(offsetof(AttributeModifierAuraUpdateModuleData, m_affectGood) == 0x148) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_affectEvil[(offsetof(AttributeModifierAuraUpdateModuleData, m_affectEvil) == 0x149) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_runWhileDead[(offsetof(AttributeModifierAuraUpdateModuleData, m_runWhileDead) == 0x14A) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_allowSelf[(offsetof(AttributeModifierAuraUpdateModuleData, m_allowSelf) == 0x14B) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_affectContainedOnly[(offsetof(AttributeModifierAuraUpdateModuleData, m_affectContainedOnly) == 0x14C) ? 1 : -1];
typedef char AttributeModifierAuraUpdateModuleData_off_m_maxActiveRank[(offsetof(AttributeModifierAuraUpdateModuleData, m_maxActiveRank) == 0x150) ? 1 : -1];

#endif
