// RebuildHoleBehaviorModuleData.h: the INI data of RebuildHoleBehavior. SIZE 0x14.
// Target facts: ModuleFactory registers the module with operator new 0x14 and
// constructor RVA 0x0048323E; buildFieldParse RVA 0x00483260 adds VA 0x00C49A10 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_REBUILD_HOLE_BEHAVIOR_MODULE_DATA_H
#define BFME2_REBUILD_HOLE_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class RebuildHoleBehaviorModuleData : public ModuleData
{
public:
	RebuildHoleBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_workerRespawnDelay;                             // +0x8 WorkerRespawnDelay; duration
	Real m_holeHealthRegen;                                // +0xC HoleHealthRegen%PerSecond; ctor 0x0048323E stores 0.1f
	AsciiString m_workerObjectName;                        // +0x10 WorkerObjectName
};
// SIZE 0x14
typedef char RebuildHoleBehaviorModuleData_size[(sizeof(RebuildHoleBehaviorModuleData) == 0x14) ? 1 : -1];
typedef char RebuildHoleBehaviorModuleData_off_m_workerRespawnDelay[(offsetof(RebuildHoleBehaviorModuleData, m_workerRespawnDelay) == 0x8) ? 1 : -1];
typedef char RebuildHoleBehaviorModuleData_off_m_workerObjectName[(offsetof(RebuildHoleBehaviorModuleData, m_workerObjectName) == 0x10) ? 1 : -1];

#endif
