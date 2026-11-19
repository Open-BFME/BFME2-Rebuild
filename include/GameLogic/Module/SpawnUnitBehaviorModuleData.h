// SpawnUnitBehaviorModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x002559F1);
// name, size, bases and virtual order are retail facts given below.
// SpawnUnitBehaviorModuleData.h: the INI data of SpawnUnitBehavior. SIZE 0x20.
// Target facts: ModuleFactory registers the module with operator new 0x20 and
// constructor RVA 0x0025598C; buildFieldParse RVA 0x004B8840 adds VA 0x00C59238 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SPAWN_UNIT_BEHAVIOR_MODULE_DATA_H
#define BFME2_SPAWN_UNIT_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class SpawnUnitBehaviorModuleData : public ModuleData
{
public:
	static ModuleData *friend_newModuleData(INI *ini);
	SpawnUnitBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	AsciiString m_unitName;                                // +0x8 UnitName
	AsciiString m_unitCommand;                             // +0xC UnitCommand
	UnsignedByte m_unk0x10[0xC];
	Bool m_spawnOnce;                                      // +0x1C SpawnOnce
};
// SIZE 0x20
typedef char SpawnUnitBehaviorModuleData_size[(sizeof(SpawnUnitBehaviorModuleData) == 0x20) ? 1 : -1];
typedef char SpawnUnitBehaviorModuleData_off_m_unitName[(offsetof(SpawnUnitBehaviorModuleData, m_unitName) == 0x8) ? 1 : -1];
typedef char SpawnUnitBehaviorModuleData_off_m_unitCommand[(offsetof(SpawnUnitBehaviorModuleData, m_unitCommand) == 0xC) ? 1 : -1];
typedef char SpawnUnitBehaviorModuleData_off_m_spawnOnce[(offsetof(SpawnUnitBehaviorModuleData, m_spawnOnce) == 0x1C) ? 1 : -1];

#endif
