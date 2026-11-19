// WeaponSetSpecialAbilityUpdateModuleData.h: the INI data of WeaponSetSpecialAbilityUpdate. SIZE 0xD0.
// Target facts: ModuleFactory registers the module with operator new 0xD0 and
// constructor RVA 0x004923E2; buildFieldParse RVA 0x0024DA56 calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00BEF008 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_WEAPON_SET_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H
#define BFME2_WEAPON_SET_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"

class INI;
class MultiIniFieldParse;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char WeaponSetSpecialAbilityUpdateModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class WeaponSetSpecialAbilityUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	WeaponSetSpecialAbilityUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UnsignedInt m_weaponsetEffectDuration;                 // +0xC8 WeaponsetEffectDuration; duration, frames
	Int m_whichWeaponSet;                                  // +0xCC WhichWeaponSet
};
// SIZE 0xD0
typedef char WeaponSetSpecialAbilityUpdateModuleData_size[(sizeof(WeaponSetSpecialAbilityUpdateModuleData) == 0xD0) ? 1 : -1];
typedef char WeaponSetSpecialAbilityUpdateModuleData_off_m_weaponsetEffectDuration[(offsetof(WeaponSetSpecialAbilityUpdateModuleData, m_weaponsetEffectDuration) == 0xC8) ? 1 : -1];
typedef char WeaponSetSpecialAbilityUpdateModuleData_off_m_whichWeaponSet[(offsetof(WeaponSetSpecialAbilityUpdateModuleData, m_whichWeaponSet) == 0xCC) ? 1 : -1];

#endif
