// AutoHealBehavior.h: the AutoHealBehavior update module. SIZE 0x38.
// Target facts (retail game.dat):
// - Constructor 0x00452592 calls the UpdateModule base first; it stores the module's vtables at
//   +0x0 (0x00C3FCDC), +0xC (0x00C495A0), +0x10 (0x00C3FCD0) and +0x28 (0x00BE2B78, 0x00C3FC78).
// - xfer 0x00452401 transfers the three members below after the UpdateModule xfer.
// - stopHealing 0x004523B6 clears the wake schedule: m_stopped = true, m_soonestHealFrame = the
//   sleep-forever value, then setWakeFrame(UPDATE_SLEEP_FOREVER).
// Bytes 0x20..0x2C hold the upgrade-mux base and the interface tables at +0x28; they are not
// established as members yet, so they stay one gap. Member names are role names.
// Not established: the base interface tables at +0x0C and +0x28, their overridden slots, and the
// members the constructor writes at +0x20..+0x2C.
#pragma once
#ifndef BFME2_AUTO_HEAL_BEHAVIOR_H
#define BFME2_AUTO_HEAL_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "GameLogic/Module/AutoHealBehaviorModuleData.h"
#include "GameLogic/Module/DamageModule.h"
#include "Common/BitFlags.h"
#include "Common/BitFlags11.h"

class Thing;
class Object;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char AutoHealBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

// The block the player scan callbacks (checkForAutoHeal 0x004527E5 and the helpers it calls) read: the module's KindOf
// mask copied by value, the healing object, the list the callback fills and three of the module data's flags (f03,
// from update 0x0045280B's two stack copies and ShouldHeal 0x004524E5).
struct AutoHealPlayerScanHelper
{
	KindOfMaskType m_kindOf;											// +0x00 the data's KindOf
	Object *m_healer;													// +0x1C the module's object
	void *m_targets;													// +0x20 a pointer to the _STL::list<Object *> the callback fills (the header cannot see STLport's list in every unit)
	Bool m_healOnlyIfNotUnderAttack;									// +0x24
	Bool m_healOnlyIfNotInCombat;										// +0x25
	Bool m_healOnlyOthers;												// +0x26
};

class AutoHealBehavior : public UpdateModule, public UpgradeMux, public DamageModuleInterface
{
public:
	virtual DamageModuleInterface *getDamage() { return this; }
	virtual UpgradeModuleInterface *getUpgrade() { return this; }
	AutoHealBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	void stopHealing();									// 0x004523B6
	void rva004523CC();									// 0x004523CC: clears m_stopped, gives the self upgrade, wakes the module now
	void pulseHealObject(Object *target, Bool doFX);				// 0x004526C6: one heal pulse onto a target
	virtual void onDamage(DamageInfo *damageInfo);					// 0x0045266B: DamageModuleInterface at +0x28 (table 0x00C3FC78)
	virtual void onHealing(DamageInfo *damageInfo) {}				// DamageModuleInterface slot 1 (table 0x0083FC78): the shared `ret 4` 0x0047A69C
	virtual void onBodyDamageStateChange(const DamageInfo *damageInfo, BodyDamageType oldState, BodyDamageType newState) {}	// slot 2: the shared `ret 0Ch` 0x000D1407
	virtual UpdateSleepTime update();								// 0x0045280B; update interface slot 0 at +0x10
	virtual DisabledMaskType getDisabledTypesToProcess() const { return DisabledMaskType(DisabledMaskType::kInit, 3); }	// UpdateModuleInterface slot 1 (table 0x0083FCD0): retail 0x004A2D67, the shared DISABLED_HELD body (h05 r13)

protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); upgradeMuxLoadPostProcess(); }	// retail slot 0x004B7BAA, the body AudioLoopUpgrade's shares (h05, gen3 vtable-content list)
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

	// UpgradeMux (table 0x00C3FC88 at +0x20)
	virtual void upgradeRemovalImplementation();						// slot 8, 0x004523E9
	virtual void upgradeImplementation();								// slot 10, the shared body 0x004A33E0
	virtual void getUpgradeActivationMasks(UpgradeMaskType &activation, UpgradeMaskType &conflicting) const;	// slot 11, 0x00452460
	virtual void performUpgradeFX();									// slot 12, 0x00452487
	virtual Bool requiresAllActivationUpgrades() const;					// slot 13, the shared 0x004827EF (data +0x114)
	virtual Bool requiresAllConflictingUpgrades() const;				// slot 14, the shared 0x004827F9 (data +0x115)
	virtual void processUpgradeRemoval() { ((const AutoHealBehaviorModuleData *)getModuleData())->m_upgradeMuxData.muxDataProcessUpgradeRemoval(getObject()); }	// slot 15: retail 0x00452487, ICF-folded with performUpgradeFX (both call the mux data's `ret 4` 0x0047A69C)

public:
	// +0x20..+0x27 is the UpgradeMux (xfer calls its upgradeMuxXfer 0x004CE397 on this+0x20); +0x28 is the DamageModuleInterface vptr.
	UnsignedInt m_soonestHealFrame;						// +0x2C; the next heal frame, sleep-forever once stopped
	Bool m_stopped;										// +0x30; xfer Bool
	UnsignedInt m_radiusParticleSystemID;				// +0x34; xfer UnsignedInt
};
// SIZE 0x38
typedef char AutoHealBehavior_size[(sizeof(AutoHealBehavior) == 0x38) ? 1 : -1];
typedef char AutoHealBehavior_off_m_soonestHealFrame[(offsetof(AutoHealBehavior, m_soonestHealFrame) == 0x2C) ? 1 : -1];
typedef char AutoHealBehavior_off_m_stopped[(offsetof(AutoHealBehavior, m_stopped) == 0x30) ? 1 : -1];
typedef char AutoHealBehavior_off_m_radiusParticleSystemID[(offsetof(AutoHealBehavior, m_radiusParticleSystemID) == 0x34) ? 1 : -1];

#endif
