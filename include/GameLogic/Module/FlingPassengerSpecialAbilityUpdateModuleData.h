// FlingPassengerSpecialAbilityUpdateModuleData.h: the INI data of FlingPassengerSpecialAbilityUpdate. SIZE 0xD8.
// Target facts: ModuleFactory registers the module with operator new 0xD8 and
// constructor RVA 0x00494F80; buildFieldParse RVA 0x0024DE1C calls SpecialAbilityUpdateModuleData::buildFieldParse, then adds VA 0x00BEF20C at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_FLING_PASSENGER_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H
#define BFME2_FLING_PASSENGER_SPECIAL_ABILITY_UPDATE_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdateModuleData.h"
#include "Common/Coord3D.h"

class INI;
class MultiIniFieldParse;
class WeaponTemplate;

// Base SpecialAbilityUpdateModuleData: the constructor calls its constructor on this. It ends at 0xC8 (its own operator-new size);
// this class's members follow it.
typedef char FlingPassengerSpecialAbilityUpdateModuleData_base_size[(sizeof(SpecialAbilityUpdateModuleData) == 0xC8) ? 1 : -1];

class FlingPassengerSpecialAbilityUpdateModuleData : public SpecialAbilityUpdateModuleData
{
public:
	FlingPassengerSpecialAbilityUpdateModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	Coord3D m_flingPassengerVelocity;                      // +0xC8 FlingPassengerVelocity
	const WeaponTemplate * m_flingPassengerLandingWarhead; // +0xD4 FlingPassengerLandingWarhead
};
// SIZE 0xD8
typedef char FlingPassengerSpecialAbilityUpdateModuleData_size[(sizeof(FlingPassengerSpecialAbilityUpdateModuleData) == 0xD8) ? 1 : -1];
typedef char FlingPassengerSpecialAbilityUpdateModuleData_off_m_flingPassengerVelocity[(offsetof(FlingPassengerSpecialAbilityUpdateModuleData, m_flingPassengerVelocity) == 0xC8) ? 1 : -1];
typedef char FlingPassengerSpecialAbilityUpdateModuleData_off_m_flingPassengerLandingWarhead[(offsetof(FlingPassengerSpecialAbilityUpdateModuleData, m_flingPassengerLandingWarhead) == 0xD4) ? 1 : -1];

#endif
