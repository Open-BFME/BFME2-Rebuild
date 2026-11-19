// CommandSetUpgradeModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x004B3A90);
// name, size, bases and virtual order are retail facts given below.
// CommandSetUpgradeModuleData.h: the INI data of CommandSetUpgrade. SIZE 0x11C.
// Target facts: ModuleFactory registers the module with operator new 0x11C and
// constructor RVA 0x00255652; buildFieldParse RVA 0x004B3A90 adds VA 0x00C572C0 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_COMMAND_SET_UPGRADE_MODULE_DATA_H
#define BFME2_COMMAND_SET_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Derives from UpgradeModuleData (0x118: ModuleData plus UpgradeMuxData at +0x8), as every upgrade
// module data does; holding UpgradeMuxData as a member here gave a wrong constructor.
class CommandSetUpgradeModuleData : public UpgradeModuleData
{
public:
	CommandSetUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);				// 0x004B3A90

	AsciiString m_commandSet;                              // +0x118 CommandSet
};
// SIZE 0x11C
typedef char CommandSetUpgradeModuleData_size[(sizeof(CommandSetUpgradeModuleData) == 0x11C) ? 1 : -1];
typedef char CommandSetUpgradeModuleData_off_m_commandSet[(offsetof(CommandSetUpgradeModuleData, m_commandSet) == 0x118) ? 1 : -1];

#endif
