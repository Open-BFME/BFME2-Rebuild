// DRAFT (h05 fpgen): generated from retail's field-parse table (VA 0x00C5B228).
// Offsets and size are compile-checked below; no function matches against it yet,
// so it stays in Draft until one does. Unresolved members: none.
// ActiveBodyModuleData.h: the INI data of OathbreakerBody. SIZE 0x64.
// Target facts: ModuleFactory registers the module with operator new 0x64 and
// constructor RVA 0x004BF59F; buildFieldParse RVA 0x004BFDF6 adds VA 0x00C5B228 at +0x0.
// Offsets are the table entries'; each type is what the entry's parse function stores
// (h05 fpgen: INI::parseReal -> Real, parseDurationUnsignedInt -> UnsignedInt, ...);
// names are the INI tokens. Byte gaps are bytes no table entry writes.
#pragma once
#ifndef BFME2_ACTIVE_BODY_MODULE_DATA_H
#define BFME2_ACTIVE_BODY_MODULE_DATA_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/ModuleData.h"
#include "Common/AsciiString.h"
#include "Common/Coord2D.h"
#include <vector>

class INI;
class MultiIniFieldParse;
class FXList;
class ObjectCreationList;

// One entry of the damage-effect list (+0x58): doDamageFX walks the copy ActiveBody keeps at +0xE0 and creates
// the entry's ObjectCreationList on the object when its trigger equals the damage's FX trigger (+0x2C of the
// DamageInfo) and its third word is 0 (12 bytes).
struct ActiveBodyDamageEffect
{
	const ObjectCreationList *m_ocl;	// +0x0 (ObjectCreationList::create 0x001F08D3)
	Int m_trigger;						// +0x4
	Int m_side;							// +0x8 BodySideDestroyedType (names at VA 0x00DCD7FC); doDamageFX needs it NONE (0)
};
typedef char ActiveBodyDamageEffect_size[(sizeof(ActiveBodyDamageEffect) == 0xC) ? 1 : -1];

class ActiveBodyModuleData : public ModuleData
{
public:
	ActiveBodyModuleData();
	static void buildFieldParse(MultiIniFieldParse &parse);
	virtual Bool slot07() const { return true; }	// retail table 0x0085AFB8 slot 7 is the shared `mov al,1; ret` stub

	Real m_maxHealth;                                      // +0x8 MaxHealth
	Real m_initialHealth;                                  // +0xC InitialHealth
	Real m_maxHealthDamaged;                               // +0x10 MaxHealthDamaged
	Real m_maxHealthReallyDamaged;                         // +0x14 MaxHealthReallyDamaged
	Real m_dodgePercent;                                   // +0x18 DodgePercent; percent
	UnsignedInt m_enteringDamagedTransitionTime;           // +0x1C EnteringDamagedTransitionTime; duration, frames
	UnsignedInt m_enteringReallyDamagedTransitionTime;     // +0x20 EnteringReallyDamagedTransitionTime; duration, frames
	UnsignedInt m_recoveryTime;                            // +0x24 RecoveryTime; duration, frames
	Bool m_useDefaultDamageSettings;                       // +0x28 UseDefaultDamageSettings
	AsciiString m_grabObject;                              // +0x2C GrabObject
	AsciiString m_damagedAttributeModifier;                // +0x30 DamagedAttributeModifier
	AsciiString m_reallyDamagedAttributeModifier;          // +0x34 ReallyDamagedAttributeModifier
	const FXList * m_grabFX;                               // +0x38 GrabFX
	Real m_grabDamage;                                     // +0x3C GrabDamage
	Coord2D m_grabOffset;                                  // +0x40 GrabOffset
	const FXList * m_healingBuffFx;                        // +0x48 HealingBuffFx
	Real m_cheerRadius;                                    // +0x4C CheerRadius
	Bool m_removeUpgradesOnDeath;                          // +0x50 RemoveUpgradesOnDeath
	Bool m_burningDeathBehavior;                           // +0x51 BurningDeathBehavior
	const FXList * m_burningDeathFX;                       // +0x54 BurningDeathFX
	std::vector<ActiveBodyDamageEffect> m_damageEffects;	// +0x58 (12 bytes); ActiveBody's constructor copies it to +0xE0
};
// SIZE 0x64
typedef char ActiveBodyModuleData_size[(sizeof(ActiveBodyModuleData) == 0x64) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_maxHealth[(offsetof(ActiveBodyModuleData, m_maxHealth) == 0x8) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_initialHealth[(offsetof(ActiveBodyModuleData, m_initialHealth) == 0xC) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_maxHealthDamaged[(offsetof(ActiveBodyModuleData, m_maxHealthDamaged) == 0x10) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_maxHealthReallyDamaged[(offsetof(ActiveBodyModuleData, m_maxHealthReallyDamaged) == 0x14) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_dodgePercent[(offsetof(ActiveBodyModuleData, m_dodgePercent) == 0x18) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_enteringDamagedTransitionTime[(offsetof(ActiveBodyModuleData, m_enteringDamagedTransitionTime) == 0x1C) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_enteringReallyDamagedTransitionTime[(offsetof(ActiveBodyModuleData, m_enteringReallyDamagedTransitionTime) == 0x20) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_recoveryTime[(offsetof(ActiveBodyModuleData, m_recoveryTime) == 0x24) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_useDefaultDamageSettings[(offsetof(ActiveBodyModuleData, m_useDefaultDamageSettings) == 0x28) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_grabObject[(offsetof(ActiveBodyModuleData, m_grabObject) == 0x2C) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_damagedAttributeModifier[(offsetof(ActiveBodyModuleData, m_damagedAttributeModifier) == 0x30) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_reallyDamagedAttributeModifier[(offsetof(ActiveBodyModuleData, m_reallyDamagedAttributeModifier) == 0x34) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_grabFX[(offsetof(ActiveBodyModuleData, m_grabFX) == 0x38) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_grabDamage[(offsetof(ActiveBodyModuleData, m_grabDamage) == 0x3C) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_grabOffset[(offsetof(ActiveBodyModuleData, m_grabOffset) == 0x40) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_healingBuffFx[(offsetof(ActiveBodyModuleData, m_healingBuffFx) == 0x48) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_cheerRadius[(offsetof(ActiveBodyModuleData, m_cheerRadius) == 0x4C) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_removeUpgradesOnDeath[(offsetof(ActiveBodyModuleData, m_removeUpgradesOnDeath) == 0x50) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_burningDeathBehavior[(offsetof(ActiveBodyModuleData, m_burningDeathBehavior) == 0x51) ? 1 : -1];
typedef char ActiveBodyModuleData_off_m_burningDeathFX[(offsetof(ActiveBodyModuleData, m_burningDeathFX) == 0x54) ? 1 : -1];

#endif
