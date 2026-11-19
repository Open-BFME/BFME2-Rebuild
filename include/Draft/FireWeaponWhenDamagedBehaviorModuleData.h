// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00BF0400).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// FireWeaponWhenDamagedBehaviorModuleData.h: the INI data of FireWeaponWhenDamagedBehavior. SIZE 0x144.
// Target facts: ModuleFactory registers the module with operator new 0x144 and
// constructor RVA 0x00253682; buildFieldParse RVA 0x002536E1 adds VA 0x00BF0400 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_FIRE_WEAPON_WHEN_DAMAGED_BEHAVIOR_MODULE_DATA_H
#define BFME2_FIRE_WEAPON_WHEN_DAMAGED_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "GameLogic/Damage.h"

class INI;
class MultiIniFieldParse;
class WeaponTemplate;

class FireWeaponWhenDamagedBehaviorModuleData : public ModuleData
{
public:
	FireWeaponWhenDamagedBehaviorModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	Bool m_startsActive;                                   // +0x118 StartsActive
	DamageTypeFlags m_damageTypes;                         // +0x11C DamageTypes
	Real m_damageAmount;                                   // +0x120 DamageAmount
	const WeaponTemplate * m_reactionWeaponPristine;       // +0x124 ReactionWeaponPristine
	const WeaponTemplate * m_reactionWeaponDamaged;        // +0x128 ReactionWeaponDamaged
	const WeaponTemplate * m_reactionWeaponReallyDamaged;  // +0x12C ReactionWeaponReallyDamaged
	const WeaponTemplate * m_reactionWeaponRubble;         // +0x130 ReactionWeaponRubble
	const WeaponTemplate * m_continuousWeaponPristine;     // +0x134 ContinuousWeaponPristine
	const WeaponTemplate * m_continuousWeaponDamaged;      // +0x138 ContinuousWeaponDamaged
	const WeaponTemplate * m_continuousWeaponReallyDamaged;// +0x13C ContinuousWeaponReallyDamaged
	const WeaponTemplate * m_continuousWeaponRubble;       // +0x140 ContinuousWeaponRubble
};
// SIZE 0x144
typedef char FireWeaponWhenDamagedBehaviorModuleData_size[(sizeof(FireWeaponWhenDamagedBehaviorModuleData) == 0x144) ? 1 : -1];
typedef char FireWeaponWhenDamagedBehaviorModuleData_off_m_upgradeMuxData[(offsetof(FireWeaponWhenDamagedBehaviorModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char FireWeaponWhenDamagedBehaviorModuleData_off_m_startsActive[(offsetof(FireWeaponWhenDamagedBehaviorModuleData, m_startsActive) == 0x118) ? 1 : -1];
typedef char FireWeaponWhenDamagedBehaviorModuleData_off_m_damageTypes[(offsetof(FireWeaponWhenDamagedBehaviorModuleData, m_damageTypes) == 0x11C) ? 1 : -1];
typedef char FireWeaponWhenDamagedBehaviorModuleData_off_m_damageAmount[(offsetof(FireWeaponWhenDamagedBehaviorModuleData, m_damageAmount) == 0x120) ? 1 : -1];
typedef char FireWeaponWhenDamagedBehaviorModuleData_off_m_reactionWeaponPristine[(offsetof(FireWeaponWhenDamagedBehaviorModuleData, m_reactionWeaponPristine) == 0x124) ? 1 : -1];
typedef char FireWeaponWhenDamagedBehaviorModuleData_off_m_reactionWeaponDamaged[(offsetof(FireWeaponWhenDamagedBehaviorModuleData, m_reactionWeaponDamaged) == 0x128) ? 1 : -1];
typedef char FireWeaponWhenDamagedBehaviorModuleData_off_m_reactionWeaponReallyDamaged[(offsetof(FireWeaponWhenDamagedBehaviorModuleData, m_reactionWeaponReallyDamaged) == 0x12C) ? 1 : -1];
typedef char FireWeaponWhenDamagedBehaviorModuleData_off_m_reactionWeaponRubble[(offsetof(FireWeaponWhenDamagedBehaviorModuleData, m_reactionWeaponRubble) == 0x130) ? 1 : -1];
typedef char FireWeaponWhenDamagedBehaviorModuleData_off_m_continuousWeaponPristine[(offsetof(FireWeaponWhenDamagedBehaviorModuleData, m_continuousWeaponPristine) == 0x134) ? 1 : -1];
typedef char FireWeaponWhenDamagedBehaviorModuleData_off_m_continuousWeaponDamaged[(offsetof(FireWeaponWhenDamagedBehaviorModuleData, m_continuousWeaponDamaged) == 0x138) ? 1 : -1];
typedef char FireWeaponWhenDamagedBehaviorModuleData_off_m_continuousWeaponReallyDamaged[(offsetof(FireWeaponWhenDamagedBehaviorModuleData, m_continuousWeaponReallyDamaged) == 0x13C) ? 1 : -1];
typedef char FireWeaponWhenDamagedBehaviorModuleData_off_m_continuousWeaponRubble[(offsetof(FireWeaponWhenDamagedBehaviorModuleData, m_continuousWeaponRubble) == 0x140) ? 1 : -1];

#endif
