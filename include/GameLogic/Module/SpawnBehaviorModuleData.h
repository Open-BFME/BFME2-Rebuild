// SpawnBehaviorModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x0025769F);
// name, size, bases and virtual order are retail facts given below.
// SpawnBehaviorModuleData.h: the INI data of SpawnBehavior. SIZE 0x174.
// Target facts: ModuleFactory registers the module with operator new 0x174 and
// constructor RVA 0x002575C2; buildFieldParse RVA 0x00253542 adds VA 0x00BF0200 at +0x0 and DieMuxData at +0x2C, UpgradeMuxData at +0x5C.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// m_spawnTemplateName +0x20 is a vector<AsciiString> (s02: the ctor 0x002575C2 builds a _Vector_base
// and clears it through vector<AsciiString>::erase 0x0002CCFC; the dtor frees it through ~vector 0x0002CC70).
#pragma once
#ifndef BFME2_SPAWN_BEHAVIOR_MODULE_DATA_H
#define BFME2_SPAWN_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include <vector>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Damage.h"
#include "GameLogic/Module/DieMuxData.h"
#include "GameLogic/Module/UpgradeMuxData.h"

class INI;
class MultiIniFieldParse;

class SpawnBehaviorModuleData : public ModuleData
{
public:
	SpawnBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);	// 0x0025769F

	Int m_spawnNumber;                                     // +0x8 SpawnNumber
	UnsignedInt m_spawnReplaceDelay;                       // +0xC SpawnReplaceDelay; duration, frames
	Int m_initialBurst;                                    // +0x10 InitialBurst
	Bool m_oneShot;                                        // +0x14 OneShot
	Bool m_canReclaimOrphans;                              // +0x15 CanReclaimOrphans
	Bool m_aggregateHealth;                                // +0x16 AggregateHealth
	Bool m_exitByBudding;                                  // +0x17 ExitByBudding
	Bool m_spawnedRequireSpawner;                          // +0x18 SpawnedRequireSpawner
	Bool m_respectCommandLimit;                            // +0x19 RespectCommandLimit
	DamageTypeFlags m_propagateDamageTypesToSlavesWhenExisting;// +0x1C PropagateDamageTypesToSlavesWhenExisting
	_STL::vector<AsciiString> m_spawnTemplateName;         // +0x20 SpawnTemplateName (INI::parseAsciiStringVectorAppend; the ctor clears it, the implicit dtor 0x0025766F frees it)
	DieMuxData m_dieMuxData;                               // +0x2C; added by buildFieldParse at +0x2C
	UpgradeMuxData m_upgradeMuxData;                       // +0x5C; added by buildFieldParse at +0x5C
	UnsignedInt m_fadeInTime;                              // +0x16C FadeInTime
	Bool m_killSpawnsBasedOnModelConditionState;           // +0x170 KillSpawnsBasedOnModelConditionState
	Bool m_shareUpgrades;                                  // +0x171 ShareUpgrades
	Bool m_spawnInsideBuilding;                            // +0x172 SpawnInsideBuilding
};
// SIZE 0x174
typedef char SpawnBehaviorModuleData_size[(sizeof(SpawnBehaviorModuleData) == 0x174) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_spawnNumber[(offsetof(SpawnBehaviorModuleData, m_spawnNumber) == 0x8) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_spawnReplaceDelay[(offsetof(SpawnBehaviorModuleData, m_spawnReplaceDelay) == 0xC) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_initialBurst[(offsetof(SpawnBehaviorModuleData, m_initialBurst) == 0x10) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_oneShot[(offsetof(SpawnBehaviorModuleData, m_oneShot) == 0x14) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_canReclaimOrphans[(offsetof(SpawnBehaviorModuleData, m_canReclaimOrphans) == 0x15) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_aggregateHealth[(offsetof(SpawnBehaviorModuleData, m_aggregateHealth) == 0x16) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_exitByBudding[(offsetof(SpawnBehaviorModuleData, m_exitByBudding) == 0x17) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_spawnedRequireSpawner[(offsetof(SpawnBehaviorModuleData, m_spawnedRequireSpawner) == 0x18) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_respectCommandLimit[(offsetof(SpawnBehaviorModuleData, m_respectCommandLimit) == 0x19) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_propagateDamageTypesToSlavesWhenExisting[(offsetof(SpawnBehaviorModuleData, m_propagateDamageTypesToSlavesWhenExisting) == 0x1C) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_spawnTemplateName[(offsetof(SpawnBehaviorModuleData, m_spawnTemplateName) == 0x20) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_dieMuxData[(offsetof(SpawnBehaviorModuleData, m_dieMuxData) == 0x2C) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_upgradeMuxData[(offsetof(SpawnBehaviorModuleData, m_upgradeMuxData) == 0x5C) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_fadeInTime[(offsetof(SpawnBehaviorModuleData, m_fadeInTime) == 0x16C) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_killSpawnsBasedOnModelConditionState[(offsetof(SpawnBehaviorModuleData, m_killSpawnsBasedOnModelConditionState) == 0x170) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_shareUpgrades[(offsetof(SpawnBehaviorModuleData, m_shareUpgrades) == 0x171) ? 1 : -1];
typedef char SpawnBehaviorModuleData_off_m_spawnInsideBuilding[(offsetof(SpawnBehaviorModuleData, m_spawnInsideBuilding) == 0x172) ? 1 : -1];

#endif
