// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C58818).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// AttributeModifierUpgradeModuleData.h: the INI data of AttributeModifierUpgrade. SIZE 0x11C.
// Target facts: ModuleFactory registers the module with operator new 0x11C and
// constructor RVA 0x002557E9; buildFieldParse RVA 0x004B671E adds VA 0x00C58818 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_ATTRIBUTE_MODIFIER_UPGRADE_MODULE_DATA_H
#define BFME2_ATTRIBUTE_MODIFIER_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class AttributeModifierUpgradeModuleData : public ModuleData
{
public:
	AttributeModifierUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	AsciiString m_attributeModifier;                       // +0x118 AttributeModifier
};
// SIZE 0x11C
typedef char AttributeModifierUpgradeModuleData_size[(sizeof(AttributeModifierUpgradeModuleData) == 0x11C) ? 1 : -1];
typedef char AttributeModifierUpgradeModuleData_off_m_upgradeMuxData[(offsetof(AttributeModifierUpgradeModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char AttributeModifierUpgradeModuleData_off_m_attributeModifier[(offsetof(AttributeModifierUpgradeModuleData, m_attributeModifier) == 0x118) ? 1 : -1];

#endif
