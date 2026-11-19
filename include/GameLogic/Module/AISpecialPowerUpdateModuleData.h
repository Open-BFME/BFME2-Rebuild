// AISpecialPowerUpdateModuleData.h: the INI data of AISpecialPowerUpdate. SIZE 0x1C.
// Target facts: ModuleFactory registers the module with operator new 0x1C and
// constructor RVA 0x004B2FCD; buildFieldParse RVA 0x004B2F19 adds VA 0x00C56ED8 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
// Unresolved (byte storage up to the next member, named after the token):
//   m_specialPowerAIType +0xC: parseSpecialPowerAIType 0x0058A617
#pragma once
#ifndef BFME2_A_I_SPECIAL_POWER_UPDATE_MODULE_DATA_H
#define BFME2_A_I_SPECIAL_POWER_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"

class INI;
class MultiIniFieldParse;

class AISpecialPowerUpdateModuleData : public ModuleData
{
public:
	AISpecialPowerUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	AsciiString m_commandButtonName;                       // +0x8 CommandButtonName
	UnsignedByte m_specialPowerAIType[0x4];                // +0xC SpecialPowerAIType; unresolved: parseSpecialPowerAIType
	Real m_specialPowerRadius;                             // +0x10 SpecialPowerRadius
	Real m_specialPowerRange;                              // +0x14 SpecialPowerRange
	Bool m_randomizeTargetLocation;                        // +0x18 RandomizeTargetLocation
	Bool m_spellMakesAStructure;                           // +0x19 SpellMakesAStructure
};
// SIZE 0x1C
typedef char AISpecialPowerUpdateModuleData_size[(sizeof(AISpecialPowerUpdateModuleData) == 0x1C) ? 1 : -1];
typedef char AISpecialPowerUpdateModuleData_off_m_commandButtonName[(offsetof(AISpecialPowerUpdateModuleData, m_commandButtonName) == 0x8) ? 1 : -1];
typedef char AISpecialPowerUpdateModuleData_off_m_specialPowerAIType[(offsetof(AISpecialPowerUpdateModuleData, m_specialPowerAIType) == 0xC) ? 1 : -1];
typedef char AISpecialPowerUpdateModuleData_off_m_specialPowerRadius[(offsetof(AISpecialPowerUpdateModuleData, m_specialPowerRadius) == 0x10) ? 1 : -1];
typedef char AISpecialPowerUpdateModuleData_off_m_specialPowerRange[(offsetof(AISpecialPowerUpdateModuleData, m_specialPowerRange) == 0x14) ? 1 : -1];
typedef char AISpecialPowerUpdateModuleData_off_m_randomizeTargetLocation[(offsetof(AISpecialPowerUpdateModuleData, m_randomizeTargetLocation) == 0x18) ? 1 : -1];
typedef char AISpecialPowerUpdateModuleData_off_m_spellMakesAStructure[(offsetof(AISpecialPowerUpdateModuleData, m_spellMakesAStructure) == 0x19) ? 1 : -1];

#endif
