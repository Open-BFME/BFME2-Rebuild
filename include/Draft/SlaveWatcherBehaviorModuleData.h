// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C4A208).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// SlaveWatcherBehaviorModuleData.h: the INI data of SlaveWatcherBehavior. SIZE 0x14.
// Target facts: ModuleFactory registers the module with operator new 0x14 and
// constructor RVA 0x004846C7; buildFieldParse RVA 0x004845BA adds VA 0x00C4A208 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SLAVE_WATCHER_BEHAVIOR_MODULE_DATA_H
#define BFME2_SLAVE_WATCHER_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class SlaveWatcherBehaviorModuleData : public ModuleData
{
public:
	SlaveWatcherBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	AsciiString m_grantUpgrade;                            // +0x8 GrantUpgrade
	AsciiString m_removeUpgrade;                           // +0xC RemoveUpgrade
	Bool m_shareUpgrades;                                  // +0x10 ShareUpgrades
	Bool m_letSlaveLive;                                   // +0x11 LetSlaveLive
};
// SIZE 0x14
typedef char SlaveWatcherBehaviorModuleData_size[(sizeof(SlaveWatcherBehaviorModuleData) == 0x14) ? 1 : -1];
typedef char SlaveWatcherBehaviorModuleData_off_m_grantUpgrade[(offsetof(SlaveWatcherBehaviorModuleData, m_grantUpgrade) == 0x8) ? 1 : -1];
typedef char SlaveWatcherBehaviorModuleData_off_m_removeUpgrade[(offsetof(SlaveWatcherBehaviorModuleData, m_removeUpgrade) == 0xC) ? 1 : -1];
typedef char SlaveWatcherBehaviorModuleData_off_m_shareUpgrades[(offsetof(SlaveWatcherBehaviorModuleData, m_shareUpgrades) == 0x10) ? 1 : -1];
typedef char SlaveWatcherBehaviorModuleData_off_m_letSlaveLive[(offsetof(SlaveWatcherBehaviorModuleData, m_letSlaveLive) == 0x11) ? 1 : -1];

#endif
