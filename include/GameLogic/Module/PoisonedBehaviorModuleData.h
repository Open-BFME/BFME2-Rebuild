// PoisonedBehaviorModuleData.h: the INI data of PoisonedBehavior. SIZE 0x10.
// Target facts: ModuleFactory registers the module with operator new 0x10 and
// constructor RVA 0x00253CDD; buildFieldParse RVA 0x00482F6C adds VA 0x00C49918 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_POISONED_BEHAVIOR_MODULE_DATA_H
#define BFME2_POISONED_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class PoisonedBehaviorModuleData : public ModuleData
{
public:
	PoisonedBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_poisonDamageInterval;                    // +0x8 PoisonDamageInterval; duration, frames
	UnsignedInt m_poisonDuration;                          // +0xC PoisonDuration; duration, frames
};
// SIZE 0x10
typedef char PoisonedBehaviorModuleData_size[(sizeof(PoisonedBehaviorModuleData) == 0x10) ? 1 : -1];
typedef char PoisonedBehaviorModuleData_off_m_poisonDamageInterval[(offsetof(PoisonedBehaviorModuleData, m_poisonDamageInterval) == 0x8) ? 1 : -1];
typedef char PoisonedBehaviorModuleData_off_m_poisonDuration[(offsetof(PoisonedBehaviorModuleData, m_poisonDuration) == 0xC) ? 1 : -1];

#endif
