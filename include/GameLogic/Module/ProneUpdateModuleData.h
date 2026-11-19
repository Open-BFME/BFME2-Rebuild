// ProneUpdateModuleData.h: the INI data of ProneUpdate. SIZE 0xC.
// Target facts: ModuleFactory registers the module with operator new 0xC and
// constructor RVA 0x0049FEDF; buildFieldParse RVA 0x0049FEF5 adds VA 0x00C51964 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_PRONE_UPDATE_MODULE_DATA_H
#define BFME2_PRONE_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class ProneUpdateModuleData : public ModuleData
{
public:
	ProneUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Real m_damageToFramesRatio;                            // +0x8 DamageToFramesRatio
};
// SIZE 0xC
typedef char ProneUpdateModuleData_size[(sizeof(ProneUpdateModuleData) == 0xC) ? 1 : -1];
typedef char ProneUpdateModuleData_off_m_damageToFramesRatio[(offsetof(ProneUpdateModuleData, m_damageToFramesRatio) == 0x8) ? 1 : -1];

#endif
