// FireSpreadUpdateModuleData.h: the INI data of FireSpreadUpdate. SIZE 0x18.
// Target facts: ModuleFactory registers the module with operator new 0x18 and
// constructor RVA 0x0048B73C; buildFieldParse RVA 0x0048B758 adds VA 0x00C4C000 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_FIRE_SPREAD_UPDATE_MODULE_DATA_H
#define BFME2_FIRE_SPREAD_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;
class ObjectCreationList;

class FireSpreadUpdateModuleData : public ModuleData
{
public:
	FireSpreadUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	const ObjectCreationList * m_oclEmbers;                // +0x8 OCLEmbers
	UnsignedInt m_minSpreadDelay;                          // +0xC MinSpreadDelay; duration, frames
	UnsignedInt m_maxSpreadDelay;                          // +0x10 MaxSpreadDelay; duration, frames
	Real m_spreadTryRange;                                 // +0x14 SpreadTryRange
};
// SIZE 0x18
typedef char FireSpreadUpdateModuleData_size[(sizeof(FireSpreadUpdateModuleData) == 0x18) ? 1 : -1];
typedef char FireSpreadUpdateModuleData_off_m_oclEmbers[(offsetof(FireSpreadUpdateModuleData, m_oclEmbers) == 0x8) ? 1 : -1];
typedef char FireSpreadUpdateModuleData_off_m_minSpreadDelay[(offsetof(FireSpreadUpdateModuleData, m_minSpreadDelay) == 0xC) ? 1 : -1];
typedef char FireSpreadUpdateModuleData_off_m_maxSpreadDelay[(offsetof(FireSpreadUpdateModuleData, m_maxSpreadDelay) == 0x10) ? 1 : -1];
typedef char FireSpreadUpdateModuleData_off_m_spreadTryRange[(offsetof(FireSpreadUpdateModuleData, m_spreadTryRange) == 0x14) ? 1 : -1];

#endif
