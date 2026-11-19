// OathbreakersFadeAwayBehaviorModuleData.h: the INI data of OathbreakersFadeAwayBehavior. SIZE 0xC.
// Target facts: ModuleFactory registers the module with operator new 0xC and
// constructor RVA ?; buildFieldParse RVA 0x004B8A79 adds VA 0x00C592CC at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_OATHBREAKERS_FADE_AWAY_BEHAVIOR_MODULE_DATA_H
#define BFME2_OATHBREAKERS_FADE_AWAY_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"

class INI;
class MultiIniFieldParse;

class OathbreakersFadeAwayBehaviorModuleData : public ModuleData
{
public:
	OathbreakersFadeAwayBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_fadeOutTime;                             // +0x8 FadeOutTime; duration, frames
};
// SIZE 0xC
typedef char OathbreakersFadeAwayBehaviorModuleData_size[(sizeof(OathbreakersFadeAwayBehaviorModuleData) == 0xC) ? 1 : -1];
typedef char OathbreakersFadeAwayBehaviorModuleData_off_m_fadeOutTime[(offsetof(OathbreakersFadeAwayBehaviorModuleData, m_fadeOutTime) == 0x8) ? 1 : -1];

#endif
