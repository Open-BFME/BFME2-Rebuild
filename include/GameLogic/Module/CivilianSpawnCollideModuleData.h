// CivilianSpawnCollideModuleData.h: the INI data of CivilianSpawnCollide. SIZE 0xC.
// Target facts: ModuleFactory registers the module with operator new 0xC and
// constructor RVA 0x004BD651; buildFieldParse RVA 0x004BD683 adds VA 0x00C5AB48 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_CIVILIAN_SPAWN_COLLIDE_MODULE_DATA_H
#define BFME2_CIVILIAN_SPAWN_COLLIDE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

class CivilianSpawnCollideModuleData : public ModuleData
{
public:
	CivilianSpawnCollideModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	ObjectFilter m_deleteObjectFilter;                     // +0x8 DeleteObjectFilter
};
// SIZE 0xC
typedef char CivilianSpawnCollideModuleData_size[(sizeof(CivilianSpawnCollideModuleData) == 0xC) ? 1 : -1];
typedef char CivilianSpawnCollideModuleData_off_m_deleteObjectFilter[(offsetof(CivilianSpawnCollideModuleData, m_deleteObjectFilter) == 0x8) ? 1 : -1];

#endif
