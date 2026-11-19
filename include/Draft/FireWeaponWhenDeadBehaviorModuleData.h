// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BF0508).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// FireWeaponWhenDeadBehaviorModuleData.h: the INI data of FireWeaponWhenDeadBehavior. SIZE 0x160.
// Target facts: ModuleFactory registers the module with operator new 0x160 and
// constructor RVA 0x00253737; buildFieldParse RVA 0x002537A6 adds VA 0x00BF0508 at +0x0 and UpgradeMuxData at +0x8, DieMuxData at +0x12C.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_FIRE_WEAPON_WHEN_DEAD_BEHAVIOR_MODULE_DATA_H
#define BFME2_FIRE_WEAPON_WHEN_DEAD_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "Common/Coord3D.h"
#include "GameLogic/Module/DieMuxData.h"

class INI;
class MultiIniFieldParse;
class WeaponTemplate;

class FireWeaponWhenDeadBehaviorModuleData : public ModuleData
{
public:
	FireWeaponWhenDeadBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	Bool m_startsActive;                                   // +0x118 StartsActive
	Bool m_activeDuringConstruction;                       // +0x119 ActiveDuringConstruction
	UnsignedInt m_delayTime;                               // +0x11C DelayTime; duration, frames
	Coord3D m_weaponOffset;                                // +0x120 WeaponOffset
	DieMuxData m_dieMuxData;                               // +0x12C; added by buildFieldParse at +0x12C
	const WeaponTemplate * m_deathWeapon;                  // +0x15C DeathWeapon
};
// SIZE 0x160
typedef char FireWeaponWhenDeadBehaviorModuleData_size[(sizeof(FireWeaponWhenDeadBehaviorModuleData) == 0x160) ? 1 : -1];
typedef char FireWeaponWhenDeadBehaviorModuleData_off_m_upgradeMuxData[(offsetof(FireWeaponWhenDeadBehaviorModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char FireWeaponWhenDeadBehaviorModuleData_off_m_startsActive[(offsetof(FireWeaponWhenDeadBehaviorModuleData, m_startsActive) == 0x118) ? 1 : -1];
typedef char FireWeaponWhenDeadBehaviorModuleData_off_m_activeDuringConstruction[(offsetof(FireWeaponWhenDeadBehaviorModuleData, m_activeDuringConstruction) == 0x119) ? 1 : -1];
typedef char FireWeaponWhenDeadBehaviorModuleData_off_m_delayTime[(offsetof(FireWeaponWhenDeadBehaviorModuleData, m_delayTime) == 0x11C) ? 1 : -1];
typedef char FireWeaponWhenDeadBehaviorModuleData_off_m_weaponOffset[(offsetof(FireWeaponWhenDeadBehaviorModuleData, m_weaponOffset) == 0x120) ? 1 : -1];
typedef char FireWeaponWhenDeadBehaviorModuleData_off_m_dieMuxData[(offsetof(FireWeaponWhenDeadBehaviorModuleData, m_dieMuxData) == 0x12C) ? 1 : -1];
typedef char FireWeaponWhenDeadBehaviorModuleData_off_m_deathWeapon[(offsetof(FireWeaponWhenDeadBehaviorModuleData, m_deathWeapon) == 0x15C) ? 1 : -1];

#endif
