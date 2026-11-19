// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C57208).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// BaseUpgradeModuleData.h: the INI data of BaseUpgrade. SIZE 0x124.
// Target facts: ModuleFactory registers the module with operator new 0x124 and
// constructor RVA 0x004B3698; buildFieldParse RVA 0x004B36FA adds VA 0x00C57208 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_BASE_UPGRADE_MODULE_DATA_H
#define BFME2_BASE_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class BaseUpgradeModuleData : public ModuleData
{
public:
	BaseUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	AsciiString m_buildingTemplateName;                    // +0x118 BuildingTemplateName
	AsciiString m_placementPrefix;                         // +0x11C PlacementPrefix
	Int m_placementIndex;                                  // +0x120 PlacementIndex
};
// SIZE 0x124
typedef char BaseUpgradeModuleData_size[(sizeof(BaseUpgradeModuleData) == 0x124) ? 1 : -1];
typedef char BaseUpgradeModuleData_off_m_upgradeMuxData[(offsetof(BaseUpgradeModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char BaseUpgradeModuleData_off_m_buildingTemplateName[(offsetof(BaseUpgradeModuleData, m_buildingTemplateName) == 0x118) ? 1 : -1];
typedef char BaseUpgradeModuleData_off_m_placementPrefix[(offsetof(BaseUpgradeModuleData, m_placementPrefix) == 0x11C) ? 1 : -1];
typedef char BaseUpgradeModuleData_off_m_placementIndex[(offsetof(BaseUpgradeModuleData, m_placementIndex) == 0x120) ? 1 : -1];

#endif
