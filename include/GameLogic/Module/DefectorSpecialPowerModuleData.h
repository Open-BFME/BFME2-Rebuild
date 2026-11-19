// DefectorSpecialPowerModuleData.h: the INI data of DefectorSpecialPower. SIZE 0x80.
// Target facts: ModuleFactory registers the module with operator new 0x80 and
// constructor RVA 0x004C2A6A; buildFieldParse RVA 0x004C2970 calls SpecialPowerModuleData::buildFieldParse, then adds VA 0x00C5C764 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_DEFECTOR_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_DEFECTOR_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialPowerModule.h"

class INI;
class MultiIniFieldParse;

// Base SpecialPowerModuleData: the constructor calls its constructor on this. It ends at 0x7C (its own operator-new size);
// this class's members follow it.
typedef char DefectorSpecialPowerModuleData_base_size[(sizeof(SpecialPowerModuleData) == 0x7C) ? 1 : -1];

class DefectorSpecialPowerModuleData : public SpecialPowerModuleData
{
public:
	DefectorSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Real m_fatCursorRadius;                                // +0x7C FatCursorRadius
};
// SIZE 0x80
typedef char DefectorSpecialPowerModuleData_size[(sizeof(DefectorSpecialPowerModuleData) == 0x80) ? 1 : -1];
typedef char DefectorSpecialPowerModuleData_off_m_fatCursorRadius[(offsetof(DefectorSpecialPowerModuleData, m_fatCursorRadius) == 0x7C) ? 1 : -1];

#endif
