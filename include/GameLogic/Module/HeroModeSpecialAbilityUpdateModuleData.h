// HeroModeSpecialAbilityUpdateModuleData.h: the INI data of HeroModeSpecialAbilityUpdate. SIZE 0xD4.
// Target facts: ModuleFactory registers the module with operator new 0xD4 and
// constructor RVA 0x0049227B; buildFieldParse RVA 0x0024D9AC calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00BEEF90 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_HERO_MODE_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H
#define BFME2_HERO_MODE_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char HeroModeSpecialAbilityUpdateModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class HeroModeSpecialAbilityUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	HeroModeSpecialAbilityUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	AsciiString m_heroAttributeModifier;                   // +0xC8 HeroAttributeModifier
	UnsignedInt m_heroEffectDuration;                      // +0xCC HeroEffectDuration; duration, frames
	Bool m_useUSERModelcondition;                          // +0xD0 UseUSERModelcondition
	Bool m_stopUnitBeforeActivating;                       // +0xD1 StopUnitBeforeActivating
};
// SIZE 0xD4
typedef char HeroModeSpecialAbilityUpdateModuleData_size[(sizeof(HeroModeSpecialAbilityUpdateModuleData) == 0xD4) ? 1 : -1];
typedef char HeroModeSpecialAbilityUpdateModuleData_off_m_heroAttributeModifier[(offsetof(HeroModeSpecialAbilityUpdateModuleData, m_heroAttributeModifier) == 0xC8) ? 1 : -1];
typedef char HeroModeSpecialAbilityUpdateModuleData_off_m_heroEffectDuration[(offsetof(HeroModeSpecialAbilityUpdateModuleData, m_heroEffectDuration) == 0xCC) ? 1 : -1];
typedef char HeroModeSpecialAbilityUpdateModuleData_off_m_useUSERModelcondition[(offsetof(HeroModeSpecialAbilityUpdateModuleData, m_useUSERModelcondition) == 0xD0) ? 1 : -1];
typedef char HeroModeSpecialAbilityUpdateModuleData_off_m_stopUnitBeforeActivating[(offsetof(HeroModeSpecialAbilityUpdateModuleData, m_stopUnitBeforeActivating) == 0xD1) ? 1 : -1];

#endif
