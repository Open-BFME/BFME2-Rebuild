// GettingBuiltBehaviorModuleData.h: the INI data of GettingBuiltBehavior. SIZE 0x4C.
// Target facts: ModuleFactory registers the module with operator new 0x4C and
// constructor RVA 0x0045324E; buildFieldParse RVA 0x00453396 adds VA 0x00C402A0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_selfBuildingLoop +0x8: INI::parseAudioEventRTS 0x00339900
//   m_selfRepairFromDamageLoop +0xC: INI::parseAudioEventRTS 0x00339900
//   m_selfRepairFromRubbleLoop +0x10: INI::parseAudioEventRTS 0x00339900
#pragma once
#ifndef BFME2_GETTING_BUILT_BEHAVIOR_MODULE_DATA_H
#define BFME2_GETTING_BUILT_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;
class WeaponTemplate;

class GettingBuiltBehaviorModuleData : public ModuleData
{
public:
	GettingBuiltBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedByte m_selfBuildingLoop[0x4];                  // +0x8 SelfBuildingLoop; unresolved: INI::parseAudioEventRTS
	UnsignedByte m_selfRepairFromDamageLoop[0x4];          // +0xC SelfRepairFromDamageLoop; unresolved: INI::parseAudioEventRTS
	UnsignedByte m_selfRepairFromRubbleLoop[0x4];          // +0x10 SelfRepairFromRubbleLoop; unresolved: INI::parseAudioEventRTS
	AsciiString m_workerName;                              // +0x14 WorkerName
	AsciiString m_evilWorkerName;                          // +0x18 EvilWorkerName
	Bool m_testFaction;                                    // +0x1C TestFaction
	Real m_spawnTimer;                                     // +0x20 SpawnTimer
	Real m_rebuildTimeSeconds;                             // +0x24 RebuildTimeSeconds
	const WeaponTemplate * m_healWeapon;                   // +0x28 HealWeapon
	Bool m_rebuildWhenDead;                                // +0x2C RebuildWhenDead
	Real m_percentOfBuildCostToRebuildPristine;            // +0x30 PercentOfBuildCostToRebuildPristine; percent
	Real m_percentOfBuildCostToRebuildDamaged;             // +0x34 PercentOfBuildCostToRebuildDamaged; percent
	Real m_percentOfBuildCostToRebuildReallyDamaged;       // +0x38 PercentOfBuildCostToRebuildReallyDamaged; percent
	Real m_percentOfBuildCostToRebuildRubble;              // +0x3C PercentOfBuildCostToRebuildRubble; percent
	ObjectFilter m_disallowRebuildFilter;                  // +0x40 DisallowRebuildFilter
	Real m_disallowRebuildRange;                           // +0x44 DisallowRebuildRange
	Bool m_useSpawnTimerWithoutWorker;                     // +0x48 UseSpawnTimerWithoutWorker
};
// SIZE 0x4C
typedef char GettingBuiltBehaviorModuleData_size[(sizeof(GettingBuiltBehaviorModuleData) == 0x4C) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_selfBuildingLoop[(offsetof(GettingBuiltBehaviorModuleData, m_selfBuildingLoop) == 0x8) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_selfRepairFromDamageLoop[(offsetof(GettingBuiltBehaviorModuleData, m_selfRepairFromDamageLoop) == 0xC) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_selfRepairFromRubbleLoop[(offsetof(GettingBuiltBehaviorModuleData, m_selfRepairFromRubbleLoop) == 0x10) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_workerName[(offsetof(GettingBuiltBehaviorModuleData, m_workerName) == 0x14) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_evilWorkerName[(offsetof(GettingBuiltBehaviorModuleData, m_evilWorkerName) == 0x18) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_testFaction[(offsetof(GettingBuiltBehaviorModuleData, m_testFaction) == 0x1C) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_spawnTimer[(offsetof(GettingBuiltBehaviorModuleData, m_spawnTimer) == 0x20) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_rebuildTimeSeconds[(offsetof(GettingBuiltBehaviorModuleData, m_rebuildTimeSeconds) == 0x24) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_healWeapon[(offsetof(GettingBuiltBehaviorModuleData, m_healWeapon) == 0x28) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_rebuildWhenDead[(offsetof(GettingBuiltBehaviorModuleData, m_rebuildWhenDead) == 0x2C) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_percentOfBuildCostToRebuildPristine[(offsetof(GettingBuiltBehaviorModuleData, m_percentOfBuildCostToRebuildPristine) == 0x30) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_percentOfBuildCostToRebuildDamaged[(offsetof(GettingBuiltBehaviorModuleData, m_percentOfBuildCostToRebuildDamaged) == 0x34) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_percentOfBuildCostToRebuildReallyDamaged[(offsetof(GettingBuiltBehaviorModuleData, m_percentOfBuildCostToRebuildReallyDamaged) == 0x38) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_percentOfBuildCostToRebuildRubble[(offsetof(GettingBuiltBehaviorModuleData, m_percentOfBuildCostToRebuildRubble) == 0x3C) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_disallowRebuildFilter[(offsetof(GettingBuiltBehaviorModuleData, m_disallowRebuildFilter) == 0x40) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_disallowRebuildRange[(offsetof(GettingBuiltBehaviorModuleData, m_disallowRebuildRange) == 0x44) ? 1 : -1];
typedef char GettingBuiltBehaviorModuleData_off_m_useSpawnTimerWithoutWorker[(offsetof(GettingBuiltBehaviorModuleData, m_useSpawnTimerWithoutWorker) == 0x48) ? 1 : -1];

#endif
