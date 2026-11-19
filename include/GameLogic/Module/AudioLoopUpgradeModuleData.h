// AudioLoopUpgradeModuleData.h: the INI data of AudioLoopUpgrade. SIZE 0x154.
// Target facts: ModuleFactory registers the module with operator new 0x154 and
// constructor RVA 0x004B7C00; buildFieldParse RVA 0x004B7C86 adds VA 0x00C58D98 at +0x0 and UpgradeMuxData at +0x14, DieMuxData at +0x124.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_soundToPlay +0x8: INI::parseAudioEventRTS 0x00339900
#pragma once
#ifndef BFME2_AUDIO_LOOP_UPGRADE_MODULE_DATA_H
#define BFME2_AUDIO_LOOP_UPGRADE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "GameLogic/Module/DieMuxData.h"

class INI;
class MultiIniFieldParse;

class AudioLoopUpgradeModuleData : public ModuleData
{
public:
	AudioLoopUpgradeModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedByte m_soundToPlay[0x4];                       // +0x8 SoundToPlay; unresolved: INI::parseAudioEventRTS
	UnsignedInt m_killAfterMS;                             // +0xC KillAfterMS; duration, frames
	Bool m_killOnDeath;                                    // +0x10 KillOnDeath
	UpgradeMuxData m_upgradeMuxData;                       // +0x14; added by buildFieldParse at +0x14
	DieMuxData m_dieMuxData;                               // +0x124; added by buildFieldParse at +0x124
};
// SIZE 0x154
typedef char AudioLoopUpgradeModuleData_size[(sizeof(AudioLoopUpgradeModuleData) == 0x154) ? 1 : -1];
typedef char AudioLoopUpgradeModuleData_off_m_soundToPlay[(offsetof(AudioLoopUpgradeModuleData, m_soundToPlay) == 0x8) ? 1 : -1];
typedef char AudioLoopUpgradeModuleData_off_m_killAfterMS[(offsetof(AudioLoopUpgradeModuleData, m_killAfterMS) == 0xC) ? 1 : -1];
typedef char AudioLoopUpgradeModuleData_off_m_killOnDeath[(offsetof(AudioLoopUpgradeModuleData, m_killOnDeath) == 0x10) ? 1 : -1];
typedef char AudioLoopUpgradeModuleData_off_m_upgradeMuxData[(offsetof(AudioLoopUpgradeModuleData, m_upgradeMuxData) == 0x14) ? 1 : -1];
typedef char AudioLoopUpgradeModuleData_off_m_dieMuxData[(offsetof(AudioLoopUpgradeModuleData, m_dieMuxData) == 0x124) ? 1 : -1];

#endif
