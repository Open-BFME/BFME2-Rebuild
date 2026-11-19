// ModelConditionSpecialAbilityUpdateModuleData.h: the INI data of ModelConditionSpecialAbilityUpdate. SIZE 0xD8.
// Target facts: ModuleFactory registers the module with operator new 0xD8 and
// constructor RVA 0x00490DDB; buildFieldParse RVA 0x0024D655 calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00BEEED0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_MODEL_CONDITION_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H
#define BFME2_MODEL_CONDITION_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char ModelConditionSpecialAbilityUpdateModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class ModelConditionSpecialAbilityUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	ModelConditionSpecialAbilityUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Int m_whichSpecialPower;                               // +0xC8 WhichSpecialPower
	Bool m_generateTerror;                                 // +0xCC GenerateTerror
	Bool m_generateUncontrollableFear;                     // +0xCD GenerateUncontrollableFear
	Real m_emotionPulseRadius;                             // +0xD0 EmotionPulseRadius
	ObjectFilter m_objectFilter;                           // +0xD4 ObjectFilter
};
// SIZE 0xD8
typedef char ModelConditionSpecialAbilityUpdateModuleData_size[(sizeof(ModelConditionSpecialAbilityUpdateModuleData) == 0xD8) ? 1 : -1];
typedef char ModelConditionSpecialAbilityUpdateModuleData_off_m_whichSpecialPower[(offsetof(ModelConditionSpecialAbilityUpdateModuleData, m_whichSpecialPower) == 0xC8) ? 1 : -1];
typedef char ModelConditionSpecialAbilityUpdateModuleData_off_m_generateTerror[(offsetof(ModelConditionSpecialAbilityUpdateModuleData, m_generateTerror) == 0xCC) ? 1 : -1];
typedef char ModelConditionSpecialAbilityUpdateModuleData_off_m_generateUncontrollableFear[(offsetof(ModelConditionSpecialAbilityUpdateModuleData, m_generateUncontrollableFear) == 0xCD) ? 1 : -1];
typedef char ModelConditionSpecialAbilityUpdateModuleData_off_m_emotionPulseRadius[(offsetof(ModelConditionSpecialAbilityUpdateModuleData, m_emotionPulseRadius) == 0xD0) ? 1 : -1];
typedef char ModelConditionSpecialAbilityUpdateModuleData_off_m_objectFilter[(offsetof(ModelConditionSpecialAbilityUpdateModuleData, m_objectFilter) == 0xD4) ? 1 : -1];

#endif
