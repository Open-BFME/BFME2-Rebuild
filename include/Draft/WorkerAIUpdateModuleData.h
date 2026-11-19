// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BEF620).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_suppliesDepletedVoice.
// WorkerAIUpdateModuleData.h: the INI data of WorkerAIUpdate. SIZE 0x94.
// Target facts: ModuleFactory registers the module with operator new 0x94 and
// constructor RVA 0x00254434; buildFieldParse RVA 0x0024F299 calls AIUpdateModuleData::buildFieldParse, then adds VA 0x00BEF620 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_suppliesDepletedVoice +0x90: INI::parseAudioEventRTS 0x00339900
#pragma once
#ifndef BFME2_WORKER_A_I_UPDATE_MODULE_DATA_H
#define BFME2_WORKER_A_I_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/AIUpdate.h"

class INI;
class MultiIniFieldParse;

// Base AIUpdateModuleData: the constructor calls its constructor on this. It ends at 0x64 (its own operator-new size);
// this class's members follow it.
typedef char WorkerAIUpdateModuleData_base_size[(sizeof(AIUpdateModuleData) == 0x64) ? 1 : -1];

class WorkerAIUpdateModuleData : public AIUpdateModuleData
{
public:
	WorkerAIUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Int m_maxBoxes;                                        // +0x64 MaxBoxes
	Real m_repairHealthPercentPerSecond;                   // +0x68 RepairHealthPercentPerSecond; percent
	Real m_boredTime;                                      // +0x6C BoredTime; duration
	Real m_boredRange;                                     // +0x70 BoredRange
	UnsignedInt m_supplyCenterActionDelay;                 // +0x74 SupplyCenterActionDelay; duration, frames
	UnsignedInt m_supplyWarehouseActionDelay;              // +0x78 SupplyWarehouseActionDelay; duration, frames
	Real m_supplyWarehouseScanDistance;                    // +0x7C SupplyWarehouseScanDistance
	Bool m_harvestTrees;                                   // +0x80 HarvestTrees
	Real m_harvestActivationRange;                         // +0x84 HarvestActivationRange
	UnsignedInt m_harvestPreparationTime;                  // +0x88 HarvestPreparationTime; duration, frames
	UnsignedInt m_harvestActionTime;                       // +0x8C HarvestActionTime; duration, frames
	UnsignedByte m_suppliesDepletedVoice[0x4];             // +0x90 SuppliesDepletedVoice; unresolved: INI::parseAudioEventRTS
};
// SIZE 0x94
typedef char WorkerAIUpdateModuleData_size[(sizeof(WorkerAIUpdateModuleData) == 0x94) ? 1 : -1];
typedef char WorkerAIUpdateModuleData_off_m_maxBoxes[(offsetof(WorkerAIUpdateModuleData, m_maxBoxes) == 0x64) ? 1 : -1];
typedef char WorkerAIUpdateModuleData_off_m_repairHealthPercentPerSecond[(offsetof(WorkerAIUpdateModuleData, m_repairHealthPercentPerSecond) == 0x68) ? 1 : -1];
typedef char WorkerAIUpdateModuleData_off_m_boredTime[(offsetof(WorkerAIUpdateModuleData, m_boredTime) == 0x6C) ? 1 : -1];
typedef char WorkerAIUpdateModuleData_off_m_boredRange[(offsetof(WorkerAIUpdateModuleData, m_boredRange) == 0x70) ? 1 : -1];
typedef char WorkerAIUpdateModuleData_off_m_supplyCenterActionDelay[(offsetof(WorkerAIUpdateModuleData, m_supplyCenterActionDelay) == 0x74) ? 1 : -1];
typedef char WorkerAIUpdateModuleData_off_m_supplyWarehouseActionDelay[(offsetof(WorkerAIUpdateModuleData, m_supplyWarehouseActionDelay) == 0x78) ? 1 : -1];
typedef char WorkerAIUpdateModuleData_off_m_supplyWarehouseScanDistance[(offsetof(WorkerAIUpdateModuleData, m_supplyWarehouseScanDistance) == 0x7C) ? 1 : -1];
typedef char WorkerAIUpdateModuleData_off_m_harvestTrees[(offsetof(WorkerAIUpdateModuleData, m_harvestTrees) == 0x80) ? 1 : -1];
typedef char WorkerAIUpdateModuleData_off_m_harvestActivationRange[(offsetof(WorkerAIUpdateModuleData, m_harvestActivationRange) == 0x84) ? 1 : -1];
typedef char WorkerAIUpdateModuleData_off_m_harvestPreparationTime[(offsetof(WorkerAIUpdateModuleData, m_harvestPreparationTime) == 0x88) ? 1 : -1];
typedef char WorkerAIUpdateModuleData_off_m_harvestActionTime[(offsetof(WorkerAIUpdateModuleData, m_harvestActionTime) == 0x8C) ? 1 : -1];
typedef char WorkerAIUpdateModuleData_off_m_suppliesDepletedVoice[(offsetof(WorkerAIUpdateModuleData, m_suppliesDepletedVoice) == 0x90) ? 1 : -1];

#endif
