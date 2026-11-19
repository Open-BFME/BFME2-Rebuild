// FireWeaponWhenDamagedBehavior.h: the behaviour module that fires reaction and
// continuous weapons when its object takes damage. SIZE 0x4C.
// Target facts (retail game.dat):
// - Layout: UpdateModule (0x20, its update interface at +0x10), then UpgradeMux
//   (8 bytes at +0x20: vptr and m_upgradeExecuted), then DamageModuleInterface (vptr
//   at +0x28), then eight Weapon pointers at +0x2C..+0x48. The UpgradeModule
//   precedent (UpgradeMux and DamageModuleInterface after a 0x10 base) has the same shape.
// - Destructor 0x00482551 (234 bytes): restores the vptrs, then deletes the eight
//   weapons in member order when each is non-null, then the UpdateModule base destructor.
// - getModuleNameKey 0x004827AA: a cached pool key for "FireWeaponWhenDamagedBehavior".
// - name 0x0048263B: the literal class name.
// - onDamage 0x00482A69 is DamageModuleInterface slot 0; update 0x00482B24 is
//   UpdateModuleInterface slot 0 (the +0x10 table).
// Not established yet: the constructor's member stores beyond the eight Weapon slots, the
// xfer body (its owner row is address-named), the remaining vtable slots, and the module
// data class that the constructor reads.
#pragma once
#ifndef BFME2_FIREWEAPONWHENDAMAGEDBEHAVIOR_H
#define BFME2_FIREWEAPONWHENDAMAGEDBEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "GameLogic/Module/DamageModule.h"

class Thing;
class ModuleData;
class Xfer;
class Weapon;
class DamageInfo;

// UpdateModule ends at 0x20; this class's bases and members follow it.
typedef char FireWeaponWhenDamagedBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class FireWeaponWhenDamagedBehavior : public UpdateModule, public UpgradeMux, public DamageModuleInterface
{
public:
	virtual DamageModuleInterface *getDamage() { return this; }
	virtual UpgradeModuleInterface *getUpgrade() { return this; }
	FireWeaponWhenDamagedBehavior(Thing *thing, const ModuleData *moduleData);	// 0x00482803
	virtual void upgradeRemovalImplementation();
	virtual void upgradeImplementation();
	virtual ~FireWeaponWhenDamagedBehavior();						// 0x00482551
	virtual NameKeyType getModuleNameKey() const;					// 0x004827AA

	virtual void onDamage(DamageInfo *damageInfo);					// 0x00482A69
	virtual void onHealing(DamageInfo *) {}
	virtual void onBodyDamageStateChange(const DamageInfo *, BodyDamageType, BodyDamageType) {}
	virtual UpdateSleepTime update();								// 0x00482B24

public:
	Weapon *m_reactionWeaponPristine;								// +0x2C
	Weapon *m_reactionWeaponDamaged;								// +0x30
	Weapon *m_reactionWeaponReallyDamaged;							// +0x34
	Weapon *m_reactionWeaponRubble;									// +0x38
	Weapon *m_continuousWeaponPristine;								// +0x3C
	Weapon *m_continuousWeaponDamaged;								// +0x40
	Weapon *m_continuousWeaponReallyDamaged;						// +0x44
	Weapon *m_continuousWeaponRubble;								// +0x48
protected:
	virtual void getUpgradeActivationMasks(UpgradeMaskType &activation, UpgradeMaskType &conflicting) const;
	virtual void performUpgradeFX();
	virtual void processUpgradeRemoval();
	virtual const char *GetSnapshotName() const;					// 0x0048263B (slot 2 of table 0x0084965C; the map names it, the old spelling was a non-virtual name())
	virtual void xfer(Xfer *xfer);									// 0x00482641 (slot 3)
	virtual Bool requiresAllActivationUpgrades() const;				// 0x004827EF (UpgradeMux slot 13)
	virtual Bool requiresAllConflictingUpgrades() const;			// 0x004827F9 (UpgradeMux slot 14)
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); upgradeMuxLoadPostProcess(); }	// retail slot 0x004B7BAA, the body AudioLoopUpgrade's shares (h05, gen3 vtable-content list)
};
// SIZE 0x4C
typedef char FireWeaponWhenDamagedBehavior_size[(sizeof(FireWeaponWhenDamagedBehavior) == 0x4C) ? 1 : -1];
typedef char FireWeaponWhenDamagedBehavior_off_m_reactionWeaponPristine[(offsetof(FireWeaponWhenDamagedBehavior, m_reactionWeaponPristine) == 0x2C) ? 1 : -1];
typedef char FireWeaponWhenDamagedBehavior_off_m_continuousWeaponRubble[(offsetof(FireWeaponWhenDamagedBehavior, m_continuousWeaponRubble) == 0x48) ? 1 : -1];

#endif
