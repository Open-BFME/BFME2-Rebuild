// ExperienceLevelCreateModuleData.h: the INI data of ExperienceLevelCreate. SIZE 0x10.
// Target facts: ModuleFactory registers the module with operator new 0x10 and
// constructor RVA 0x004B9220; buildFieldParse RVA 0x004B91F0 adds VA 0x00C595B4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_EXPERIENCE_LEVEL_CREATE_MODULE_DATA_H
#define BFME2_EXPERIENCE_LEVEL_CREATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class ExperienceLevelCreateModuleData : public ModuleData
{
public:
	ExperienceLevelCreateModuleData();
	virtual Bool slot09() const { return true; }
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Int m_levelToGrant;                                    // +0x8 LevelToGrant
	Bool m_mpOnly;                                         // +0xC MPOnly
};
// SIZE 0x10
typedef char ExperienceLevelCreateModuleData_size[(sizeof(ExperienceLevelCreateModuleData) == 0x10) ? 1 : -1];
typedef char ExperienceLevelCreateModuleData_off_m_levelToGrant[(offsetof(ExperienceLevelCreateModuleData, m_levelToGrant) == 0x8) ? 1 : -1];
typedef char ExperienceLevelCreateModuleData_off_m_mpOnly[(offsetof(ExperienceLevelCreateModuleData, m_mpOnly) == 0xC) ? 1 : -1];

#endif
