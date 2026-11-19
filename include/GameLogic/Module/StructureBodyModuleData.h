// StructureBodyModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x0025708C);
// name, size, bases and virtual order are retail facts given below.
// StructureBodyModuleData.h: the INI data of StructureBody. SIZE 0x64.
// Target facts: ModuleFactory registers the module with operator new 0x64 and
// constructor RVA 0x00257006; buildFieldParse RVA 0x002514AF calls ActiveBodyModuleData::buildFieldParse, then adds VA 0x00C6BB18 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_STRUCTURE_BODY_MODULE_DATA_H
#define BFME2_STRUCTURE_BODY_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/ActiveBodyModuleData.h"

class INI;
class MultiIniFieldParse;

// Base ActiveBodyModuleData: the constructor calls its constructor on this. It ends at 0x64 (its own operator-new size);
// this class's members follow it.
typedef char StructureBodyModuleData_base_size[(sizeof(ActiveBodyModuleData) == 0x64) ? 1 : -1];

class StructureBodyModuleData : public ActiveBodyModuleData
{
public:
	StructureBodyModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);	// 0x0025708C

};
// SIZE 0x64
typedef char StructureBodyModuleData_size[(sizeof(StructureBodyModuleData) == 0x64) ? 1 : -1];

#endif
