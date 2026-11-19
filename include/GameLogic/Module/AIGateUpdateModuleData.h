// AIGateUpdateModuleData.h: the INI data of AIGateUpdate. SIZE 0x10.
// Target facts: ModuleFactory registers the module with operator new 0x10 and
// constructor RVA 0x004B08C1; buildFieldParse RVA 0x004B08D7 adds VA 0x00C564D4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_A_I_GATE_UPDATE_MODULE_DATA_H
#define BFME2_A_I_GATE_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class AIGateUpdateModuleData : public ModuleData
{
public:
	AIGateUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Real m_triggerWidthX;                                  // +0x8 TriggerWidthX
	Real m_triggerWidthY;                                  // +0xC TriggerWidthY
};
// SIZE 0x10
typedef char AIGateUpdateModuleData_size[(sizeof(AIGateUpdateModuleData) == 0x10) ? 1 : -1];
typedef char AIGateUpdateModuleData_off_m_triggerWidthX[(offsetof(AIGateUpdateModuleData, m_triggerWidthX) == 0x8) ? 1 : -1];
typedef char AIGateUpdateModuleData_off_m_triggerWidthY[(offsetof(AIGateUpdateModuleData, m_triggerWidthY) == 0xC) ? 1 : -1];

#endif
