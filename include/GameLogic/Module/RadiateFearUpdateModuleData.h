// RadiateFearUpdateModuleData.h: the INI data of RadiateFearUpdate. SIZE 0x130.
// Target facts: ModuleFactory registers the module with operator new 0x130 and
// constructor RVA 0x0049C06C; buildFieldParse RVA 0x0049C0E4 adds VA 0x00C510B8 at +0x0 and UpgradeMuxData at +0x20.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_RADIATE_FEAR_UPDATE_MODULE_DATA_H
#define BFME2_RADIATE_FEAR_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/ObjectFilter.h"
#include "GameLogic/Module/UpgradeMuxData.h"

class INI;
class MultiIniFieldParse;

class RadiateFearUpdateModuleData : public ModuleData
{
public:
	RadiateFearUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Bool m_initiallyActive;                                // +0x8 InitiallyActive
	Int m_whichSpecialPower;                               // +0xC WhichSpecialPower
	Bool m_generateTerror;                                 // +0x10 GenerateTerror
	Bool m_generateFear;                                   // +0x11 GenerateFear
	Bool m_generateUncontrollableFear;                     // +0x12 GenerateUncontrollableFear
	Real m_emotionPulseRadius;                             // +0x14 EmotionPulseRadius
	UnsignedInt m_emotionPulseInterval;                    // +0x18 EmotionPulseInterval; duration, frames
	ObjectFilter m_victimFilter;                           // +0x1C VictimFilter
	UpgradeMuxData m_upgradeMuxData;                       // +0x20; added by buildFieldParse at +0x20
};
// SIZE 0x130
typedef char RadiateFearUpdateModuleData_size[(sizeof(RadiateFearUpdateModuleData) == 0x130) ? 1 : -1];
typedef char RadiateFearUpdateModuleData_off_m_initiallyActive[(offsetof(RadiateFearUpdateModuleData, m_initiallyActive) == 0x8) ? 1 : -1];
typedef char RadiateFearUpdateModuleData_off_m_whichSpecialPower[(offsetof(RadiateFearUpdateModuleData, m_whichSpecialPower) == 0xC) ? 1 : -1];
typedef char RadiateFearUpdateModuleData_off_m_generateTerror[(offsetof(RadiateFearUpdateModuleData, m_generateTerror) == 0x10) ? 1 : -1];
typedef char RadiateFearUpdateModuleData_off_m_generateFear[(offsetof(RadiateFearUpdateModuleData, m_generateFear) == 0x11) ? 1 : -1];
typedef char RadiateFearUpdateModuleData_off_m_generateUncontrollableFear[(offsetof(RadiateFearUpdateModuleData, m_generateUncontrollableFear) == 0x12) ? 1 : -1];
typedef char RadiateFearUpdateModuleData_off_m_emotionPulseRadius[(offsetof(RadiateFearUpdateModuleData, m_emotionPulseRadius) == 0x14) ? 1 : -1];
typedef char RadiateFearUpdateModuleData_off_m_emotionPulseInterval[(offsetof(RadiateFearUpdateModuleData, m_emotionPulseInterval) == 0x18) ? 1 : -1];
typedef char RadiateFearUpdateModuleData_off_m_victimFilter[(offsetof(RadiateFearUpdateModuleData, m_victimFilter) == 0x1C) ? 1 : -1];
typedef char RadiateFearUpdateModuleData_off_m_upgradeMuxData[(offsetof(RadiateFearUpdateModuleData, m_upgradeMuxData) == 0x20) ? 1 : -1];

#endif
