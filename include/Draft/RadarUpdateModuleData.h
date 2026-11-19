// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BF1EF8).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// RadarUpdateModuleData.h: the INI data of RadarUpdate. SIZE 0xC.
// Target facts: ModuleFactory registers the module with operator new 0xC and
// constructor RVA 0x003ECC80; buildFieldParse RVA 0x00254280 adds VA 0x00BF1EF8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_RADAR_UPDATE_MODULE_DATA_H
#define BFME2_RADAR_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class RadarUpdateModuleData : public ModuleData
{
public:
	RadarUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_radarExtendTime;                                // +0x8 RadarExtendTime; duration
};
// SIZE 0xC
typedef char RadarUpdateModuleData_size[(sizeof(RadarUpdateModuleData) == 0xC) ? 1 : -1];
typedef char RadarUpdateModuleData_off_m_radarExtendTime[(offsetof(RadarUpdateModuleData, m_radarExtendTime) == 0x8) ? 1 : -1];

#endif
