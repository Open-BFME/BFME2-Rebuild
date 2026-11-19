// ObjectCreationUpgrade.h: the ObjectCreationUpgrade module. SIZE 0x30 (ModuleFactory operator new).
// Target facts (retail game.dat, f03):
// - The class is UpgradeMux (first base, +0x0, 8 bytes) and UpdateModule (+0x8, 0x20 bytes), in that order:
//   constructor 0x004B40A6 calls ??0UpgradeMux on this, then ??0UpdateModule on this+8, and stores the tables
//   0x00C57660 at +0x0 (the UpgradeMux table, 18 slots), 0x00C5762C at +0x8 (the module's), 0x00C57570 at +0x14
//   and 0x00C57560 at +0x18. Every function reached through the module or update interface sees `this` at +0x8
//   or +0x18 and reads the module data at this+0x4 / this-0xC.
// - The members are at +0x28 and +0x2C (constructor: 0 and false). upgradeImplementation 0x004B4106 stores
//   TheGameLogic's frame plus the data's Delay at +0x28 and sets the flag at +0x2C; update 0x004B42DB waits for
//   the frame to pass, creates the object and clears the flag; xfer 0x004B421D transfers the Bool (slot 36)
//   then the UnsignedInt (slot 30) after Version1, then UpdateModule::xfer, then the mux.
// - Overrides of the UpgradeMux table (0x00C57660): slot 2 0x004B4130 and slot 6 0x004B41A9 (bodies the map names
//   under the address-named class Rva004B4130), 8 0x004B463C, 10 upgradeImplementation, 11
//   getUpgradeActivationMasks, 12 and 15 performUpgradeFX, 13 0x004B4061 and 14 0x004B406B (the module data's
//   +0x11C and +0x11D bytes, the RequiresAll flags of its UpgradeMuxData).
#pragma once
#ifndef BFME2_OBJECT_CREATION_UPGRADE_H
#define BFME2_OBJECT_CREATION_UPGRADE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "GameLogic/Module/ObjectCreationUpgradeModuleData.h"

class Thing;
class ModuleData;
class Xfer;
class ObjectCreationUpgradeModuleData;

typedef char ObjectCreationUpgrade_base_size[(sizeof(UpgradeMux) == 0x8 && sizeof(UpdateModule) == 0x20) ? 1 : -1];

class ObjectCreationUpgrade : public UpgradeMux, public UpdateModule
{
public:
	ObjectCreationUpgrade(Thing *thing, const ModuleData *moduleData);		// 0x004B40A6
	virtual NameKeyType getModuleNameKey() const;							// 0x004B3FF5
protected:
	virtual UpdateSleepTime update();										// 0x004B42DB; update interface slot 0 at +0x18 (protected: the map spells ?update@ObjectCreationUpgrade@@MAE?AW4UpdateSleepTime@@XZ)
public:
	virtual UpdateModuleInterface *getUpdate() { return this; }				// 0x004B3FBA: retail's own copy adjusts from the +0x14 interface back to the object and forward to +0x18
	virtual UpgradeModuleInterface *getUpgrade() { return this; }			// 0x004B3FC7: the UpgradeMux at +0

protected:
	virtual ~ObjectCreationUpgrade();										// 0x004B3F90
	virtual void loadPostProcess();											// 0x004B3FCB
	virtual const char *GetSnapshotName() const;							// 0x004B3FB4
	virtual void xfer(Xfer *xfer);											// 0x004B421D

	// UpgradeMux (table 0x00C57660 at +0x0)
	virtual Bool wouldUpgrade(const BitFlags<128> &keyMask) const;			// slot 2, 0x004B4130
	virtual void vslot06();													// slot 6, 0x004B41A9
	virtual void upgradeRemovalImplementation();							// slot 8, whose retail body is rva004B463C below
	virtual void upgradeImplementation();									// slot 10, 0x004B4106
	virtual void getUpgradeActivationMasks(UpgradeMaskType &activation, UpgradeMaskType &conflicting) const;	// slot 11, 0x004B403A
	virtual void performUpgradeFX();										// slot 12, 0x004B4075
	virtual Bool requiresAllActivationUpgrades() const;						// slot 13, 0x004B4061
	virtual Bool requiresAllConflictingUpgrades() const;					// slot 14, 0x004B406B
	virtual void processUpgradeRemoval() { ((const ObjectCreationUpgradeModuleData *)getModuleData())->m_upgradeMuxData.muxDataProcessUpgradeRemoval(getObject()); }	// slot 15: retail 0x004B4075, ICF-folded with performUpgradeFX (both call the mux data's `ret 4` 0x0047A69C)

public:
	// 0x004B463C: slot 8's body (the map spells it a non-virtual member of this class, `this` is the UpgradeMux at +0): clears the
	// pending creation, and when the data says DestroyWhenSold kills the object the SlaveWatcherBehavior module watches, then
	// plays the data's death animation as a special model condition on it.
	void rva004B463C();

	UnsignedInt m_createFrame;												// +0x28; the frame the object is created on
	Bool m_createPending;													// +0x2C
};
// SIZE 0x30
typedef char ObjectCreationUpgrade_size[(sizeof(ObjectCreationUpgrade) == 0x30) ? 1 : -1];
typedef char ObjectCreationUpgrade_off_m_createFrame[(offsetof(ObjectCreationUpgrade, m_createFrame) == 0x28) ? 1 : -1];
typedef char ObjectCreationUpgrade_off_m_createPending[(offsetof(ObjectCreationUpgrade, m_createPending) == 0x2C) ? 1 : -1];

#endif
