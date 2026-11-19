// SpecialPowerCompletionDieModuleData.h: the INI data of SpecialPowerCompletionDie. SIZE 0x3C.
// Target facts: ModuleFactory registers the module with operator new 0x3C and
// constructor RVA 0x004C230E; buildFieldParse RVA 0x00254AFC adds VA 0x00BF0928 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SPECIAL_POWER_COMPLETION_DIE_MODULE_DATA_H
#define BFME2_SPECIAL_POWER_COMPLETION_DIE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DestroyDieModuleData.h"

class INI;
class MultiIniFieldParse;
class SpecialPowerTemplate;

// Base DestroyDieModuleData: the constructor calls its constructor on this. It ends at 0x38 (its own operator-new size);
// this class's members follow it.
typedef char SpecialPowerCompletionDieModuleData_base_size[(sizeof(DestroyDieModuleData) == 0x38) ? 1 : -1];

class SpecialPowerCompletionDieModuleData : public DestroyDieModuleData
{
public:
	SpecialPowerCompletionDieModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	const SpecialPowerTemplate * m_specialPowerTemplate;   // +0x38 SpecialPowerTemplate
};
// SIZE 0x3C
typedef char SpecialPowerCompletionDieModuleData_size[(sizeof(SpecialPowerCompletionDieModuleData) == 0x3C) ? 1 : -1];
typedef char SpecialPowerCompletionDieModuleData_off_m_specialPowerTemplate[(offsetof(SpecialPowerCompletionDieModuleData, m_specialPowerTemplate) == 0x38) ? 1 : -1];

#endif
