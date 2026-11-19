// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C590E8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// CommandPointsUpgradeModuleData.h: the INI data of CommandPointsUpgrade. SIZE 0x120.
// Target facts: ModuleFactory registers the module with operator new 0x120 and
// constructor RVA 0x00254891; buildFieldParse RVA 0x004B863F adds VA 0x00C590E8 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_COMMAND_POINTS_UPGRADE_MODULE_DATA_H
#define BFME2_COMMAND_POINTS_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

class CommandPointsUpgradeModuleData : public ModuleData
{
public:
	CommandPointsUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	Int m_commandPoints;                                   // +0x118 CommandPoints
	ObjectFilter m_requiredObject;                         // +0x11C RequiredObject
};
// SIZE 0x120
typedef char CommandPointsUpgradeModuleData_size[(sizeof(CommandPointsUpgradeModuleData) == 0x120) ? 1 : -1];
typedef char CommandPointsUpgradeModuleData_off_m_upgradeMuxData[(offsetof(CommandPointsUpgradeModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char CommandPointsUpgradeModuleData_off_m_commandPoints[(offsetof(CommandPointsUpgradeModuleData, m_commandPoints) == 0x118) ? 1 : -1];
typedef char CommandPointsUpgradeModuleData_off_m_requiredObject[(offsetof(CommandPointsUpgradeModuleData, m_requiredObject) == 0x11C) ? 1 : -1];

#endif
