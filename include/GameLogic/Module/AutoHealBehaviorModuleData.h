// Promoted from Draft by f03 (AutoHealBehavior's helpers read these fields).
// AutoHealBehaviorModuleData.h: the INI data of AutoHealBehavior. SIZE 0x160.
// Target facts: ModuleFactory registers the module with operator new 0x160 and
// constructor RVA 0x00254DE7; buildFieldParse RVA 0x00256929 adds VA 0x00BF3CE0 at +0x0 and UpgradeMuxData at +0x8.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_AUTO_HEAL_BEHAVIOR_MODULE_DATA_H
#define BFME2_AUTO_HEAL_BEHAVIOR_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "Common/BitFlags.h"

class INI;
class MultiIniFieldParse;
class FXList;

class AutoHealBehaviorModuleData : public ModuleData
{
public:
	AutoHealBehaviorModuleData() throw();
	static void buildFieldParse(MultiIniFieldParse &parse);
	static ModuleData *friend_newModuleData(INI *ini);

	UpgradeMuxData m_upgradeMuxData;                       // +0x8; added by buildFieldParse at +0x8
	Bool m_startsActive;                                   // +0x118 StartsActive
	Bool m_buttonTriggered;                                // +0x119 ButtonTriggered
	Bool m_singleBurst;                                    // +0x11A SingleBurst
	Int m_healingAmount;                                   // +0x11C HealingAmount
	UnsignedInt m_healingDelay;                            // +0x120 HealingDelay; duration, frames
	UnsignedInt m_startHealingDelay;                       // +0x124 StartHealingDelay; duration, frames
	Int m_radius;                                          // +0x128 Radius
	Bool m_affectsWholePlayer;                             // +0x12C AffectsWholePlayer
	Bool m_affectsContained;                               // +0x12D AffectsContained
	Bool m_healOnlyIfNotUnderAttack;                       // +0x12E HealOnlyIfNotUnderAttack
	Bool m_healOnlyIfNotInCombat;                          // +0x12F HealOnlyIfNotInCombat
	KindOfMaskType m_kindOf;                               // +0x130 KindOf; BitFlags<218>
	Bool m_healOnlyOthers;                                 // +0x14C HealOnlyOthers
	const FXList * m_unitHealPulseFX;                      // +0x150 UnitHealPulseFX
	Bool m_nonStackable;                                   // +0x154 NonStackable
	Bool m_respawnNearbyHordeMembers;                      // +0x155 RespawnNearbyHordeMembers
	const FXList * m_respawnFXList;                        // +0x158 RespawnFXList
	UnsignedInt m_respawnMinimumDelay;                     // +0x15C RespawnMinimumDelay
};
// SIZE 0x160
typedef char AutoHealBehaviorModuleData_size[(sizeof(AutoHealBehaviorModuleData) == 0x160) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_upgradeMuxData[(offsetof(AutoHealBehaviorModuleData, m_upgradeMuxData) == 0x8) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_startsActive[(offsetof(AutoHealBehaviorModuleData, m_startsActive) == 0x118) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_buttonTriggered[(offsetof(AutoHealBehaviorModuleData, m_buttonTriggered) == 0x119) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_singleBurst[(offsetof(AutoHealBehaviorModuleData, m_singleBurst) == 0x11A) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_healingAmount[(offsetof(AutoHealBehaviorModuleData, m_healingAmount) == 0x11C) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_healingDelay[(offsetof(AutoHealBehaviorModuleData, m_healingDelay) == 0x120) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_startHealingDelay[(offsetof(AutoHealBehaviorModuleData, m_startHealingDelay) == 0x124) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_radius[(offsetof(AutoHealBehaviorModuleData, m_radius) == 0x128) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_affectsWholePlayer[(offsetof(AutoHealBehaviorModuleData, m_affectsWholePlayer) == 0x12C) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_affectsContained[(offsetof(AutoHealBehaviorModuleData, m_affectsContained) == 0x12D) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_healOnlyIfNotUnderAttack[(offsetof(AutoHealBehaviorModuleData, m_healOnlyIfNotUnderAttack) == 0x12E) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_healOnlyIfNotInCombat[(offsetof(AutoHealBehaviorModuleData, m_healOnlyIfNotInCombat) == 0x12F) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_kindOf[(offsetof(AutoHealBehaviorModuleData, m_kindOf) == 0x130) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_healOnlyOthers[(offsetof(AutoHealBehaviorModuleData, m_healOnlyOthers) == 0x14C) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_unitHealPulseFX[(offsetof(AutoHealBehaviorModuleData, m_unitHealPulseFX) == 0x150) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_nonStackable[(offsetof(AutoHealBehaviorModuleData, m_nonStackable) == 0x154) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_respawnNearbyHordeMembers[(offsetof(AutoHealBehaviorModuleData, m_respawnNearbyHordeMembers) == 0x155) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_respawnFXList[(offsetof(AutoHealBehaviorModuleData, m_respawnFXList) == 0x158) ? 1 : -1];
typedef char AutoHealBehaviorModuleData_off_m_respawnMinimumDelay[(offsetof(AutoHealBehaviorModuleData, m_respawnMinimumDelay) == 0x15C) ? 1 : -1];

#endif
