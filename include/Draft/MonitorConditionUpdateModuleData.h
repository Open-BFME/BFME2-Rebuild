// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C4DA00).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// MonitorConditionUpdateModuleData.h: the INI data of MonitorConditionUpdate. SIZE 0x6C.
// Target facts: ModuleFactory registers the module with operator new 0x6C and
// constructor RVA 0x004915FD; buildFieldParse RVA 0x004915B5 adds VA 0x00C4DA00 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_MONITOR_CONDITION_UPDATE_MODULE_DATA_H
#define BFME2_MONITOR_CONDITION_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/BitFlags.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class MonitorConditionUpdateModuleData : public ModuleData
{
public:
	MonitorConditionUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	BitFlags<591> m_modelConditionFlags;                   // +0x8 ModelConditionFlags; BitFlags<591>
	AsciiString m_modelConditionCommandSet;                // +0x54 ModelConditionCommandSet
	WeaponSetFlags m_weaponSetFlags;                       // +0x58 WeaponSetFlags; BitFlags<104>
	AsciiString m_weaponToggleCommandSet;                  // +0x68 WeaponToggleCommandSet
};
// SIZE 0x6C
typedef char MonitorConditionUpdateModuleData_size[(sizeof(MonitorConditionUpdateModuleData) == 0x6C) ? 1 : -1];
typedef char MonitorConditionUpdateModuleData_off_m_modelConditionFlags[(offsetof(MonitorConditionUpdateModuleData, m_modelConditionFlags) == 0x8) ? 1 : -1];
typedef char MonitorConditionUpdateModuleData_off_m_modelConditionCommandSet[(offsetof(MonitorConditionUpdateModuleData, m_modelConditionCommandSet) == 0x54) ? 1 : -1];
typedef char MonitorConditionUpdateModuleData_off_m_weaponSetFlags[(offsetof(MonitorConditionUpdateModuleData, m_weaponSetFlags) == 0x58) ? 1 : -1];
typedef char MonitorConditionUpdateModuleData_off_m_weaponToggleCommandSet[(offsetof(MonitorConditionUpdateModuleData, m_weaponToggleCommandSet) == 0x68) ? 1 : -1];

#endif
