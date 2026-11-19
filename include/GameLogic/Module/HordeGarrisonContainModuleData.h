// HordeGarrisonContainModuleData.h: the INI data of HordeGarrisonContain. SIZE 0xD4.
// Target facts: ModuleFactory registers the module with operator new 0xD4 and
// constructor RVA 0x0047A251; buildFieldParse RVA 0x0047A025 calls GarrisonContainModuleData::buildFieldParse, then adds VA 0x00C46388 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_HORDE_GARRISON_CONTAIN_MODULE_DATA_H
#define BFME2_HORDE_GARRISON_CONTAIN_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/GarrisonContainModuleData.h"
#include "Common/Coord3D.h"

class INI;
class MultiIniFieldParse;

// Base GarrisonContainModuleData: the constructor calls its constructor on this. It ends at 0xAC (its own operator-new size);
// this class's members follow it.
typedef char HordeGarrisonContainModuleData_base_size[(sizeof(GarrisonContainModuleData) == 0xAC) ? 1 : -1];

class HordeGarrisonContainModuleData : public GarrisonContainModuleData
{
public:
	HordeGarrisonContainModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);
	virtual const HordeGarrisonContainModuleData *slot29() const { return this; }

	UnsignedInt m_exitDelay;                               // +0xAC ExitDelay; duration, frames
	Coord3D m_entryOffset;                                 // +0xB0 EntryOffset
	Coord3D m_entryPosition;                               // +0xBC EntryPosition
	Coord3D m_exitOffset;                                  // +0xC8 ExitOffset
};
// SIZE 0xD4
typedef char HordeGarrisonContainModuleData_size[(sizeof(HordeGarrisonContainModuleData) == 0xD4) ? 1 : -1];
typedef char HordeGarrisonContainModuleData_off_m_exitDelay[(offsetof(HordeGarrisonContainModuleData, m_exitDelay) == 0xAC) ? 1 : -1];
typedef char HordeGarrisonContainModuleData_off_m_entryOffset[(offsetof(HordeGarrisonContainModuleData, m_entryOffset) == 0xB0) ? 1 : -1];
typedef char HordeGarrisonContainModuleData_off_m_entryPosition[(offsetof(HordeGarrisonContainModuleData, m_entryPosition) == 0xBC) ? 1 : -1];
typedef char HordeGarrisonContainModuleData_off_m_exitOffset[(offsetof(HordeGarrisonContainModuleData, m_exitOffset) == 0xC8) ? 1 : -1];

#endif
