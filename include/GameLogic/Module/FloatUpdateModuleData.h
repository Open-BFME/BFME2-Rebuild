// FloatUpdateModuleData.h: the INI data of FloatUpdate. SIZE 0xC.
// Target facts: ModuleFactory registers the module with operator new 0xC and
// constructor RVA 0x0048D7A6; buildFieldParse RVA 0x0048D7B3 adds VA 0x00C4C7A4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_FLOAT_UPDATE_MODULE_DATA_H
#define BFME2_FLOAT_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class FloatUpdateModuleData : public ModuleData
{
public:
	FloatUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Bool m_enabled;                                        // +0x8 Enabled
};
// SIZE 0xC
typedef char FloatUpdateModuleData_size[(sizeof(FloatUpdateModuleData) == 0xC) ? 1 : -1];
typedef char FloatUpdateModuleData_off_m_enabled[(offsetof(FloatUpdateModuleData, m_enabled) == 0x8) ? 1 : -1];

#endif
