// UnitCrateCollideModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x00255AB8);
// name, size, bases and virtual order are retail facts given below.
// UnitCrateCollideModuleData.h: the INI data of UnitCrateCollide. SIZE 0x64.
// Target facts: ModuleFactory registers the module with operator new 0x64 and
// constructor RVA 0x00255A72; buildFieldParse RVA 0x002511F3 calls CrateCollideModuleData::buildFieldParse, then adds VA 0x00BEFAF4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_UNIT_CRATE_COLLIDE_MODULE_DATA_H
#define BFME2_UNIT_CRATE_COLLIDE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/CrateCollideModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base CrateCollideModuleData: the constructor calls its constructor on this. It ends at 0x5C (its own operator-new size);
// this class's members follow it.
typedef char UnitCrateCollideModuleData_base_size[(sizeof(CrateCollideModuleData) == 0x5C) ? 1 : -1];

class UnitCrateCollideModuleData : public CrateCollideModuleData
{
public:
	static ModuleData *friend_newModuleData(INI *ini);
	UnitCrateCollideModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UnsignedInt m_unitCount;                               // +0x5C UnitCount
	AsciiString m_unitName;                                // +0x60 UnitName
};
// SIZE 0x64
typedef char UnitCrateCollideModuleData_size[(sizeof(UnitCrateCollideModuleData) == 0x64) ? 1 : -1];
typedef char UnitCrateCollideModuleData_off_m_unitCount[(offsetof(UnitCrateCollideModuleData, m_unitCount) == 0x5C) ? 1 : -1];
typedef char UnitCrateCollideModuleData_off_m_unitName[(offsetof(UnitCrateCollideModuleData, m_unitName) == 0x60) ? 1 : -1];

#endif
