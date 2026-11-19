// DozerAIUpdateModuleData.h: the INI data of DozerAIUpdate. SIZE 0x70.
// Target facts: ModuleFactory registers the module with operator new 0x70 and
// constructor RVA 0x00488A81; buildFieldParse RVA 0x00488AA5 calls AIUpdateModuleData::buildFieldParse, then adds VA 0x00C4B748 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DOZER_A_I_UPDATE_MODULE_DATA_H
#define BFME2_DOZER_A_I_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/AIUpdate.h"

class INI;
class MultiIniFieldParse;

// Base AIUpdateModuleData: the constructor calls its constructor on this. It ends at 0x64 (its own operator-new size);
// this class's members follow it.
typedef char DozerAIUpdateModuleData_base_size[(sizeof(AIUpdateModuleData) == 0x64) ? 1 : -1];

class DozerAIUpdateModuleData : public AIUpdateModuleData
{
public:
	DozerAIUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Real m_repairHealthPercentPerSecond;                   // +0x64 RepairHealthPercentPerSecond; percent
	Real m_boredTime;                                      // +0x68 BoredTime; duration
	Real m_boredRange;                                     // +0x6C BoredRange
};
// SIZE 0x70
typedef char DozerAIUpdateModuleData_size[(sizeof(DozerAIUpdateModuleData) == 0x70) ? 1 : -1];
typedef char DozerAIUpdateModuleData_off_m_repairHealthPercentPerSecond[(offsetof(DozerAIUpdateModuleData, m_repairHealthPercentPerSecond) == 0x64) ? 1 : -1];
typedef char DozerAIUpdateModuleData_off_m_boredTime[(offsetof(DozerAIUpdateModuleData, m_boredTime) == 0x68) ? 1 : -1];
typedef char DozerAIUpdateModuleData_off_m_boredRange[(offsetof(DozerAIUpdateModuleData, m_boredRange) == 0x6C) ? 1 : -1];

#endif
