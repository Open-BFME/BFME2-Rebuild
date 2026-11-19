// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C57BC4).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// DoCommandUpgradeModuleData.h: the INI data of DoCommandUpgrade. SIZE 0x120.
// Target facts: ModuleFactory registers the module with operator new 0x120 and
// constructor RVA 0x00255795; buildFieldParse RVA 0x004B4C0C adds VA 0x00C57BC4 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DO_COMMAND_UPGRADE_MODULE_DATA_H
#define BFME2_DO_COMMAND_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class DoCommandUpgradeModuleData : public ModuleData
{
public:
	DoCommandUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	AsciiString m_getUpgradeCommandButtonName;             // +0x118 GetUpgradeCommandButtonName
	AsciiString m_removeUpgradeCommandButtonName;          // +0x11C RemoveUpgradeCommandButtonName
};
// SIZE 0x120
typedef char DoCommandUpgradeModuleData_size[(sizeof(DoCommandUpgradeModuleData) == 0x120) ? 1 : -1];
typedef char DoCommandUpgradeModuleData_off_m_upgradeMuxData[(offsetof(DoCommandUpgradeModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char DoCommandUpgradeModuleData_off_m_getUpgradeCommandButtonName[(offsetof(DoCommandUpgradeModuleData, m_getUpgradeCommandButtonName) == 0x118) ? 1 : -1];
typedef char DoCommandUpgradeModuleData_off_m_removeUpgradeCommandButtonName[(offsetof(DoCommandUpgradeModuleData, m_removeUpgradeCommandButtonName) == 0x11C) ? 1 : -1];

#endif
