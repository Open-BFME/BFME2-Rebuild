// CommandButtonHuntUpdateModuleData.h: the INI data of CommandButtonHuntUpdate. SIZE 0x10.
// Target facts: ModuleFactory registers the module with operator new 0x10 and
// constructor RVA 0x0049529F; buildFieldParse RVA 0x004952BE adds VA 0x00C4ECC0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_COMMAND_BUTTON_HUNT_UPDATE_MODULE_DATA_H
#define BFME2_COMMAND_BUTTON_HUNT_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class CommandButtonHuntUpdateModuleData : public ModuleData
{
public:
	CommandButtonHuntUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_scanRate;                                // +0x8 ScanRate; duration, frames
	Real m_scanRange;                                      // +0xC ScanRange
};
// SIZE 0x10
typedef char CommandButtonHuntUpdateModuleData_size[(sizeof(CommandButtonHuntUpdateModuleData) == 0x10) ? 1 : -1];
typedef char CommandButtonHuntUpdateModuleData_off_m_scanRate[(offsetof(CommandButtonHuntUpdateModuleData, m_scanRate) == 0x8) ? 1 : -1];
typedef char CommandButtonHuntUpdateModuleData_off_m_scanRange[(offsetof(CommandButtonHuntUpdateModuleData, m_scanRange) == 0xC) ? 1 : -1];

#endif
