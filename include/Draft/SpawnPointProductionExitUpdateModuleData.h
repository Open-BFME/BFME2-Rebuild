// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BF22D4).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// SpawnPointProductionExitUpdateModuleData.h: the INI data of SpawnPointProductionExitUpdate. SIZE 0xC.
// Target facts: ModuleFactory registers the module with operator new 0xC and
// constructor RVA ?; buildFieldParse RVA 0x0025436A adds VA 0x00BF22D4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SPAWN_POINT_PRODUCTION_EXIT_UPDATE_MODULE_DATA_H
#define BFME2_SPAWN_POINT_PRODUCTION_EXIT_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class SpawnPointProductionExitUpdateModuleData : public ModuleData
{
public:
	SpawnPointProductionExitUpdateModuleData() {}
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	AsciiString m_spawnPointBoneName;                      // +0x8 SpawnPointBoneName
};
// SIZE 0xC
typedef char SpawnPointProductionExitUpdateModuleData_size[(sizeof(SpawnPointProductionExitUpdateModuleData) == 0xC) ? 1 : -1];
typedef char SpawnPointProductionExitUpdateModuleData_off_m_spawnPointBoneName[(offsetof(SpawnPointProductionExitUpdateModuleData, m_spawnPointBoneName) == 0x8) ? 1 : -1];

#endif
