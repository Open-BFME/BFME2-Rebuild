// HealContainModuleData.h: the INI data of HealContain. SIZE 0x9C.
// Target facts: ModuleFactory registers the module with operator new 0x9C and
// constructor RVA 0x00466D37; buildFieldParse RVA 0x00466AD0 calls OpenContainModuleData::buildFieldParse, then adds VA 0x00C43D24 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_HEAL_CONTAIN_MODULE_DATA_H
#define BFME2_HEAL_CONTAIN_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/OpenContainModuleData.h"

class INI;
class MultiIniFieldParse;

// Base OpenContainModuleData: the constructor calls its constructor on this. It ends at 0x98 (its own operator-new size);
// this class's members follow it.
typedef char HealContainModuleData_base_size[(sizeof(OpenContainModuleData) == 0x98) ? 1 : -1];

class HealContainModuleData : public OpenContainModuleData
{
public:
	HealContainModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_timeForFullHeal;                         // +0x98 TimeForFullHeal; duration, frames
};
// SIZE 0x9C
typedef char HealContainModuleData_size[(sizeof(HealContainModuleData) == 0x9C) ? 1 : -1];
typedef char HealContainModuleData_off_m_timeForFullHeal[(offsetof(HealContainModuleData, m_timeForFullHeal) == 0x98) ? 1 : -1];

#endif
