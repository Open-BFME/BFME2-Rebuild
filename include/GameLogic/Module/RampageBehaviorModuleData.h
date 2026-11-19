// RampageBehaviorModuleData.h: the INI data of RampageBehavior. SIZE 0x2C.
// Target facts: ModuleFactory registers the module with operator new 0x2C and
// constructor RVA 0x00458C0F; buildFieldParse RVA 0x00458B4F adds VA 0x00C40E78 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_RAMPAGE_BEHAVIOR_MODULE_DATA_H
#define BFME2_RAMPAGE_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/Vector.h"

class INI;
class MultiIniFieldParse;

class RampageBehaviorModuleData : public ModuleData
{
public:
	RampageBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	_STL::vector<AsciiString> m_requiredUpgrade;           // +0x8 RequiredUpgrade
	Real m_rampageHealthThreshold;                         // +0x14 RampageHealthThreshold
	Int m_rampageLifeTimer;                                // +0x18 RampageLifeTimer
	Int m_rampageAngryLifeTimer;                           // +0x1C RampageAngryLifeTimer
	Int m_rampageResetTimer;                               // +0x20 RampageResetTimer
	Real m_rampageEnemyCheckRange;                         // +0x24 RampageEnemyCheckRange
	Int m_rampageEnemyThreshold;                           // +0x28 RampageEnemyThreshold
};
// SIZE 0x2C
typedef char RampageBehaviorModuleData_size[(sizeof(RampageBehaviorModuleData) == 0x2C) ? 1 : -1];
typedef char RampageBehaviorModuleData_off_m_requiredUpgrade[(offsetof(RampageBehaviorModuleData, m_requiredUpgrade) == 0x8) ? 1 : -1];
typedef char RampageBehaviorModuleData_off_m_rampageHealthThreshold[(offsetof(RampageBehaviorModuleData, m_rampageHealthThreshold) == 0x14) ? 1 : -1];
typedef char RampageBehaviorModuleData_off_m_rampageLifeTimer[(offsetof(RampageBehaviorModuleData, m_rampageLifeTimer) == 0x18) ? 1 : -1];
typedef char RampageBehaviorModuleData_off_m_rampageAngryLifeTimer[(offsetof(RampageBehaviorModuleData, m_rampageAngryLifeTimer) == 0x1C) ? 1 : -1];
typedef char RampageBehaviorModuleData_off_m_rampageResetTimer[(offsetof(RampageBehaviorModuleData, m_rampageResetTimer) == 0x20) ? 1 : -1];
typedef char RampageBehaviorModuleData_off_m_rampageEnemyCheckRange[(offsetof(RampageBehaviorModuleData, m_rampageEnemyCheckRange) == 0x24) ? 1 : -1];
typedef char RampageBehaviorModuleData_off_m_rampageEnemyThreshold[(offsetof(RampageBehaviorModuleData, m_rampageEnemyThreshold) == 0x28) ? 1 : -1];

#endif
