// ScaleWallSpecialAbilityUpdateModuleData.h: the INI data of ScaleWallSpecialAbilityUpdate. SIZE 0xCC.
// Target facts: ModuleFactory registers the module with operator new 0xCC and
// constructor RVA 0x00494DB1; buildFieldParse RVA 0x0024DD72 calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00BEF1B4 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_SCALE_WALL_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H
#define BFME2_SCALE_WALL_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"

class INI;
class MultiIniFieldParse;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char ScaleWallSpecialAbilityUpdateModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class ScaleWallSpecialAbilityUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	ScaleWallSpecialAbilityUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_delayAtFootOfWall;                       // +0xC8 DelayAtFootOfWall; duration, frames
};
// SIZE 0xCC
typedef char ScaleWallSpecialAbilityUpdateModuleData_size[(sizeof(ScaleWallSpecialAbilityUpdateModuleData) == 0xCC) ? 1 : -1];
typedef char ScaleWallSpecialAbilityUpdateModuleData_off_m_delayAtFootOfWall[(offsetof(ScaleWallSpecialAbilityUpdateModuleData, m_delayAtFootOfWall) == 0xC8) ? 1 : -1];

#endif
