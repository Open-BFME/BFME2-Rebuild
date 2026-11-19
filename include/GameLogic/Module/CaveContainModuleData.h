// CaveContainModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x00257714);
// name, size, bases and virtual order are retail facts given below.
// CaveContainModuleData.h: the INI data of CaveContain. SIZE 0x9C.
// Target facts: ModuleFactory registers the module with operator new 0x9C and
// constructor RVA 0x00466D37; buildFieldParse RVA 0x0024B77C calls OpenContainModuleData::buildFieldParse, then adds VA 0x00BEEAC8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_CAVE_CONTAIN_MODULE_DATA_H
#define BFME2_CAVE_CONTAIN_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/OpenContainModuleData.h"

class INI;
class MultiIniFieldParse;

// Base OpenContainModuleData: the constructor calls its constructor on this. It ends at 0x98 (its own operator-new size);
// this class's members follow it.
typedef char CaveContainModuleData_base_size[(sizeof(OpenContainModuleData) == 0x98) ? 1 : -1];

class CaveContainModuleData : public OpenContainModuleData
{
public:
	CaveContainModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);	// 0x00257714

	Int m_caveIndex;                                       // +0x98 CaveIndex
};
// SIZE 0x9C
typedef char CaveContainModuleData_size[(sizeof(CaveContainModuleData) == 0x9C) ? 1 : -1];
typedef char CaveContainModuleData_off_m_caveIndex[(offsetof(CaveContainModuleData, m_caveIndex) == 0x98) ? 1 : -1];

#endif
