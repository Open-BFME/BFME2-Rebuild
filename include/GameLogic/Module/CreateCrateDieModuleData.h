// CreateCrateDieModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 2 rows that MATCH against it on main (0x004858F4, 0x0025739D);
// name, size, bases and virtual order are retail facts given below.
// CreateCrateDieModuleData.h: the INI data of CreateCrateDie. SIZE 0x3C.
// Target facts: ModuleFactory registers the module with operator new 0x3C and
// constructor RVA 0x002572EE; buildFieldParse RVA 0x00253991 adds VA 0x00BF0740 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_CREATE_CRATE_DIE_MODULE_DATA_H
#define BFME2_CREATE_CRATE_DIE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DestroyDieModuleData.h"
#include "Common/AsciiString.h"
#include <list>

class INI;
class MultiIniFieldParse;

// Base DestroyDieModuleData: the constructor calls its constructor on this. It ends at 0x38 (its own operator-new size);
// this class's members follow it.
typedef char CreateCrateDieModuleData_base_size[(sizeof(DestroyDieModuleData) == 0x38) ? 1 : -1];

class CreateCrateDieModuleData : public DestroyDieModuleData
{
public:
	CreateCrateDieModuleData();
	virtual ~CreateCrateDieModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);	// 0x0025739D
	static void parseCrateData(INI *ini, void *instance, void *store, const void *userData);

	std::list<AsciiString> m_crateNameList;                // +0x38 CrateNameList
};
// SIZE 0x3C
typedef char CreateCrateDieModuleData_size[(sizeof(CreateCrateDieModuleData) == 0x3C) ? 1 : -1];

#endif
