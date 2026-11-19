// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5C8D0).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// LevelGrantSpecialPowerModuleData.h: the INI data of LevelGrantSpecialPower. SIZE 0xD8.
// Target facts: ModuleFactory registers the module with operator new 0xD8 and
// constructor RVA 0x004C2AA0; buildFieldParse RVA 0x004C2AF3 calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00C5C8D0 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_LEVEL_GRANT_SPECIAL_POWER_MODULE_DATA_H
#define BFME2_LEVEL_GRANT_SPECIAL_POWER_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"
#include "GameLogic/ObjectFilter.h"

class INI;
class MultiIniFieldParse;
class FXList;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char LevelGrantSpecialPowerModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class LevelGrantSpecialPowerModuleData : public SpecialAbilityUpdateModuleData
{
public:
	LevelGrantSpecialPowerModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	Int m_experience;                                      // +0xC8 Experience
	Real m_radiusEffect;                                   // +0xCC RadiusEffect
	ObjectFilter m_acceptanceFilter;                       // +0xD0 AcceptanceFilter
	const FXList * m_levelFX;                              // +0xD4 LevelFX
};
// SIZE 0xD8
typedef char LevelGrantSpecialPowerModuleData_size[(sizeof(LevelGrantSpecialPowerModuleData) == 0xD8) ? 1 : -1];
typedef char LevelGrantSpecialPowerModuleData_off_m_experience[(offsetof(LevelGrantSpecialPowerModuleData, m_experience) == 0xC8) ? 1 : -1];
typedef char LevelGrantSpecialPowerModuleData_off_m_radiusEffect[(offsetof(LevelGrantSpecialPowerModuleData, m_radiusEffect) == 0xCC) ? 1 : -1];
typedef char LevelGrantSpecialPowerModuleData_off_m_acceptanceFilter[(offsetof(LevelGrantSpecialPowerModuleData, m_acceptanceFilter) == 0xD0) ? 1 : -1];
typedef char LevelGrantSpecialPowerModuleData_off_m_levelFX[(offsetof(LevelGrantSpecialPowerModuleData, m_levelFX) == 0xD4) ? 1 : -1];

#endif
