// BeaconClientUpdateModuleData.h: the INI data of BeaconClientUpdate. SIZE 0x10.
// Target facts: ModuleFactory registers the module with operator new 0x10 and
// constructor RVA 0x004C964C; buildFieldParse RVA 0x004C9663 adds VA 0x00C5EB94 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_BEACON_CLIENT_UPDATE_MODULE_DATA_H
#define BFME2_BEACON_CLIENT_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class BeaconClientUpdateModuleData : public ModuleData
{
public:
	BeaconClientUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_radarPulseFrequency;                     // +0x8 RadarPulseFrequency; duration, frames
	UnsignedInt m_radarPulseDuration;                      // +0xC RadarPulseDuration; duration, frames
};
// SIZE 0x10
typedef char BeaconClientUpdateModuleData_size[(sizeof(BeaconClientUpdateModuleData) == 0x10) ? 1 : -1];
typedef char BeaconClientUpdateModuleData_off_m_radarPulseFrequency[(offsetof(BeaconClientUpdateModuleData, m_radarPulseFrequency) == 0x8) ? 1 : -1];
typedef char BeaconClientUpdateModuleData_off_m_radarPulseDuration[(offsetof(BeaconClientUpdateModuleData, m_radarPulseDuration) == 0xC) ? 1 : -1];

#endif
