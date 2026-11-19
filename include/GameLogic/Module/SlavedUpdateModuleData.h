// SlavedUpdateModuleData.h: the INI data of SlavedUpdate. SIZE 0x70.
// Target facts: ModuleFactory registers the module with operator new 0x70 and constructor
// RVA 0x002552D7; buildFieldParse RVA 0x002542E2 adds the table at VA 0x00BF2110 at +0x0.
// Every name, offset and parse function below is one entry of that table (read from the image,
// 25 entries); the constructor zeroes the block and sets MarkUnselectable (+0x6C) to true.
// The two AsciiString entries are parseAsciiString (0x0002F11E) at +0x4C and +0x50.
// An earlier generated draft stopped at +0x34 and treated the rest as a byte gap; SlavedUpdate's
// rows read +0x54, +0x55, +0x6C and +0x6D by name below (stopSlavedEffects 0x004A1B17, update
// 0x004A265E, startSlavedEffects 0x004A1A69, the interface getters 0x004A1942 and 0x004A1949).
#pragma once
#ifndef BFME2_SLAVED_UPDATE_MODULE_DATA_H
#define BFME2_SLAVED_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class SlavedUpdateModuleData : public ModuleData
{
public:
	SlavedUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Int m_leashRange;                                      // +0x8 LeashRange
	Int m_guardMaxRange;                                   // +0xC GuardMaxRange
	Int m_guardWanderRange;                                // +0x10 GuardWanderRange
	Int m_attackRange;                                     // +0x14 AttackRange
	Int m_attackWanderRange;                               // +0x18 AttackWanderRange
	Int m_scoutRange;                                      // +0x1C ScoutRange
	Int m_scoutWanderRange;                                // +0x20 ScoutWanderRange
	Int m_distToTargetToGrantRangeBonus;                   // +0x24 DistToTargetToGrantRangeBonus
	Int m_repairRange;                                     // +0x28 RepairRange
	Real m_repairMinAltitude;                              // +0x2C RepairMinAltitude
	Real m_repairMaxAltitude;                              // +0x30 RepairMaxAltitude
	Real m_repairRatePerSecond;                            // +0x34 RepairRatePerSecond
	Int m_repairWhenBelowHealthPercent;                    // +0x38 RepairWhenBelowHealth%
	UnsignedInt m_repairMinReadyTime;                      // +0x3C RepairMinReadyTime; duration, frames
	UnsignedInt m_repairMaxReadyTime;                      // +0x40 RepairMaxReadyTime
	UnsignedInt m_repairMinWeldTime;                       // +0x44 RepairMinWeldTime
	UnsignedInt m_repairMaxWeldTime;                       // +0x48 RepairMaxWeldTime
	AsciiString m_repairWeldingSys;                        // +0x4C RepairWeldingSys
	AsciiString m_repairWeldingFXBone;                     // +0x50 RepairWeldingFXBone
	Bool m_stayOnSameLayerAsMaster;                        // +0x54 StayOnSameLayerAsMaster
	Bool m_dieOnMastersDeath;                              // +0x55 DieOnMastersDeath
	Coord3D m_guardPositionOffset;                         // +0x58 GuardPositionOffset
	Int m_fadeOutRange;                                    // +0x64 FadeOutRange
	UnsignedInt m_fadeTime;                                // +0x68 FadeTime; duration, frames
	Bool m_markUnselectable;                               // +0x6C MarkUnselectable
	Bool m_useSlaverAsControlForEvaObjectSightedEvents;    // +0x6D UseSlaverAsControlForEvaObjectSightedEvents
};
// SIZE 0x70
typedef char SlavedUpdateModuleData_size[(sizeof(SlavedUpdateModuleData) == 0x70) ? 1 : -1];
typedef char SlavedUpdateModuleData_off_m_leashRange[(offsetof(SlavedUpdateModuleData, m_leashRange) == 0x8) ? 1 : -1];
typedef char SlavedUpdateModuleData_off_m_repairRatePerSecond[(offsetof(SlavedUpdateModuleData, m_repairRatePerSecond) == 0x34) ? 1 : -1];
typedef char SlavedUpdateModuleData_off_m_repairWeldingSys[(offsetof(SlavedUpdateModuleData, m_repairWeldingSys) == 0x4C) ? 1 : -1];
typedef char SlavedUpdateModuleData_off_m_stayOnSameLayerAsMaster[(offsetof(SlavedUpdateModuleData, m_stayOnSameLayerAsMaster) == 0x54) ? 1 : -1];
typedef char SlavedUpdateModuleData_off_m_guardPositionOffset[(offsetof(SlavedUpdateModuleData, m_guardPositionOffset) == 0x58) ? 1 : -1];
typedef char SlavedUpdateModuleData_off_m_fadeOutRange[(offsetof(SlavedUpdateModuleData, m_fadeOutRange) == 0x64) ? 1 : -1];
typedef char SlavedUpdateModuleData_off_m_markUnselectable[(offsetof(SlavedUpdateModuleData, m_markUnselectable) == 0x6C) ? 1 : -1];

#endif
