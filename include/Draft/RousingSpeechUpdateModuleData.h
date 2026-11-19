// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C54F28).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: m_modifierName.
// RousingSpeechUpdateModuleData.h: the INI data of RousingSpeechUpdate. SIZE 0xF8.
// Target facts: ModuleFactory registers the module with operator new 0xF8 and
// constructor RVA 0x004AD0EE; buildFieldParse RVA 0x004ACD73 calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00C54F28 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_modifierName +0xE4: INI::parseAsciiStringVectorAppend 0x0002E896
#pragma once
#ifndef BFME2_ROUSING_SPEECH_UPDATE_MODULE_DATA_H
#define BFME2_ROUSING_SPEECH_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;
class FXList;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char RousingSpeechUpdateModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class RousingSpeechUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	RousingSpeechUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Real m_bonusRadius;                                    // +0xC8 BonusRadius
	UnsignedInt m_speechDuration;                          // +0xCC SpeechDuration; duration, frames
	UnsignedInt m_updateInterval;                          // +0xD0 UpdateInterval; duration, frames
	const FXList * m_leaderFX;                             // +0xD4 LeaderFX
	const FXList * m_followerFX;                           // +0xD8 FollowerFX
	Bool m_createWave;                                     // +0xDC CreateWave
	Real m_waveWidth;                                      // +0xE0 WaveWidth
	UnsignedByte m_modifierName[0xC];                      // +0xE4 ModifierName; unresolved: INI::parseAsciiStringVectorAppend
	Bool m_levelUp;                                        // +0xF0 LevelUp
	ObjectFilter m_objectFilter;                           // +0xF4 ObjectFilter
};
// SIZE 0xF8
typedef char RousingSpeechUpdateModuleData_size[(sizeof(RousingSpeechUpdateModuleData) == 0xF8) ? 1 : -1];
typedef char RousingSpeechUpdateModuleData_off_m_bonusRadius[(offsetof(RousingSpeechUpdateModuleData, m_bonusRadius) == 0xC8) ? 1 : -1];
typedef char RousingSpeechUpdateModuleData_off_m_speechDuration[(offsetof(RousingSpeechUpdateModuleData, m_speechDuration) == 0xCC) ? 1 : -1];
typedef char RousingSpeechUpdateModuleData_off_m_updateInterval[(offsetof(RousingSpeechUpdateModuleData, m_updateInterval) == 0xD0) ? 1 : -1];
typedef char RousingSpeechUpdateModuleData_off_m_leaderFX[(offsetof(RousingSpeechUpdateModuleData, m_leaderFX) == 0xD4) ? 1 : -1];
typedef char RousingSpeechUpdateModuleData_off_m_followerFX[(offsetof(RousingSpeechUpdateModuleData, m_followerFX) == 0xD8) ? 1 : -1];
typedef char RousingSpeechUpdateModuleData_off_m_createWave[(offsetof(RousingSpeechUpdateModuleData, m_createWave) == 0xDC) ? 1 : -1];
typedef char RousingSpeechUpdateModuleData_off_m_waveWidth[(offsetof(RousingSpeechUpdateModuleData, m_waveWidth) == 0xE0) ? 1 : -1];
typedef char RousingSpeechUpdateModuleData_off_m_modifierName[(offsetof(RousingSpeechUpdateModuleData, m_modifierName) == 0xE4) ? 1 : -1];
typedef char RousingSpeechUpdateModuleData_off_m_levelUp[(offsetof(RousingSpeechUpdateModuleData, m_levelUp) == 0xF0) ? 1 : -1];
typedef char RousingSpeechUpdateModuleData_off_m_objectFilter[(offsetof(RousingSpeechUpdateModuleData, m_objectFilter) == 0xF4) ? 1 : -1];

#endif
