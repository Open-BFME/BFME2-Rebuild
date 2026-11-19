// TunnelContainModuleData.h: promoted from include/Draft by h05 (round 10). Proven by 1 row that MATCH against it on main (0x00257A0A);
// name, size, bases and virtual order are retail facts given below.
// TunnelContainModuleData.h: the INI data of TunnelContain. SIZE 0xD8.
// Target facts: ModuleFactory registers the module with operator new 0xD8 and
// constructor RVA 0x0025797E; buildFieldParse RVA 0x0024BCD2 calls HordeGarrisonContainModuleData::buildFieldParse, then adds VA 0x00BEEAF8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_TUNNEL_CONTAIN_MODULE_DATA_H
#define BFME2_TUNNEL_CONTAIN_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/HordeGarrisonContainModuleData.h"

class INI;
class MultiIniFieldParse;

// Base HordeGarrisonContainModuleData: the constructor calls its constructor on this. It ends at 0xD4 (its own operator-new size);
// this class's members follow it.
typedef char TunnelContainModuleData_base_size[(sizeof(HordeGarrisonContainModuleData) == 0xD4) ? 1 : -1];

class TunnelContainModuleData : public HordeGarrisonContainModuleData
{
public:
	TunnelContainModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);	// 0x00257A0A

	Real m_timeForFullHeal;                                // +0xD4 TimeForFullHeal; duration
};
// SIZE 0xD8
typedef char TunnelContainModuleData_size[(sizeof(TunnelContainModuleData) == 0xD8) ? 1 : -1];
typedef char TunnelContainModuleData_off_m_timeForFullHeal[(offsetof(TunnelContainModuleData, m_timeForFullHeal) == 0xD4) ? 1 : -1];

#endif
