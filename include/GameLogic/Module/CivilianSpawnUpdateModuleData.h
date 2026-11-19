// CivilianSpawnUpdateModuleData.h: the INI data of CivilianSpawnUpdate. SIZE 0x20.
// Target facts: ModuleFactory registers the module with operator new 0x20 and
// constructor RVA 0x0047FA55; buildFieldParse RVA 0x0047F9C3 adds VA 0x00C48478 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_CIVILIAN_SPAWN_UPDATE_MODULE_DATA_H
#define BFME2_CIVILIAN_SPAWN_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"
#include "Common/Vector.h"

class INI;
class MultiIniFieldParse;

class CivilianSpawnUpdateModuleData : public ModuleData
{
public:
	CivilianSpawnUpdateModuleData();
	virtual ~CivilianSpawnUpdateModuleData();						// 0x0047FABF
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_spawnDelayTime;                          // +0x8 SpawnDelayTime; duration, frames
	ObjectFilter m_runToFilter;                            // +0xC RunToFilter
	Int m_maximumDistance;                                 // +0x10 MaximumDistance
	_STL::vector<AsciiString> m_civilian;                  // +0x14 Civilian
};
// SIZE 0x20
typedef char CivilianSpawnUpdateModuleData_size[(sizeof(CivilianSpawnUpdateModuleData) == 0x20) ? 1 : -1];
typedef char CivilianSpawnUpdateModuleData_off_m_spawnDelayTime[(offsetof(CivilianSpawnUpdateModuleData, m_spawnDelayTime) == 0x8) ? 1 : -1];
typedef char CivilianSpawnUpdateModuleData_off_m_runToFilter[(offsetof(CivilianSpawnUpdateModuleData, m_runToFilter) == 0xC) ? 1 : -1];
typedef char CivilianSpawnUpdateModuleData_off_m_maximumDistance[(offsetof(CivilianSpawnUpdateModuleData, m_maximumDistance) == 0x10) ? 1 : -1];
typedef char CivilianSpawnUpdateModuleData_off_m_civilian[(offsetof(CivilianSpawnUpdateModuleData, m_civilian) == 0x14) ? 1 : -1];

#endif
