// UpgradeModule.h: the upgrade mux, its INI data and the concrete UpgradeModule
// base every *Upgrade module derives from.
// Target facts (retail game.dat):
// - UpgradeModule is BehaviorModule (ctor 0x00253330) + UpgradeMux at +0x10
//   (ctor 0x004CE2A3: table 0x00C5FF10, Bool m_upgradeExecuted at +4) +
//   DamageModuleInterface at +0x18; size 0x1C (ArmorUpgrade, which adds
//   nothing, is allocated with `push 0x1c`). Ctor 0x00460AEC stores 0x00C42768
//   at +0, 0x00C57328 at +0xC, 0x00C42720 at +0x10 and 0x00C58790 at +0x18;
//   dtor 0x0046089D undoes them and jumps to ~Module.
// - The +0x18 base is DamageModuleInterface: its own table 0x00BE2B78 is three
//   __purecall slots, the folded table 0x00C58790 every upgrade and contain module
//   stores there holds empty bodies shaped onDamage(1 arg), onHealing(1 arg),
//   onBodyDamageStateChange(3 args) (ret 4, ret 4, ret 0xC), and
//   UpgradeModule::getDamage 0x0045875F returns it.
// - UpgradeMux's 18 slots (table 0x00C5FF10; UpgradeModule's 0x00C42720,
//   GeometryUpgrade's 0x00858978): names are the ledger's where it has one;
//   slots 8 and 10 are upgradeRemovalImplementation / upgradeImplementation
//   (GeometryUpgrade 0x004B6B29 / 0x004B6C8A). isAlreadyUpgraded (slot 0,
//   0x00098CBC) and isSubObjectsUpgrade (slot 4) are Zero Hour's names for
//   bodies that fit them. Slot 15 is processUpgradeRemoval: giveSelfUpgrade
//   0x0045230C calls slots 12, 15, 10 and 9 (+0x30, +0x3C, +0x28, +0x24), Zero
//   Hour's performUpgradeFX, processUpgradeRemoval, upgradeImplementation,
//   setUpgradeExecuted(true); UpgradeModule's body is the one performUpgradeFX
//   folds with (0x00460AD3: an empty mux-data call). Slots 6 and 17 are unnamed.
// - The UpgradeModule overrides run with `this` at the mux (+0x10) and reach
//   the module data at this-0xC: requiresAllActivationUpgrades 0x00460A98 reads
//   data+0x114, requiresAllConflictingUpgrades +0x115, vslot07 +0x116.
// - UpgradeModuleData is ModuleData + UpgradeMuxData at +8 (field table
//   0x0085FEA0, registered at offset 8: TriggeredBy 0, ConflictsWith 0x80,
//   CustomAnimAndDuration 0x100, RequiresAllTriggers 0x10C,
//   RequiresAllConflictingTriggers 0x10D, Permanent 0x10E), size 0x118: every
//   derived upgrade's own fields start at 280.
#pragma once
#ifndef BFME2_UPGRADEMODULE_H
#define BFME2_UPGRADEMODULE_H

#include "Common/GameType.h"
#include "GameLogic/Module/BehaviorModule.h"
#include "GameLogic/Module/UpgradeMuxData.h"
#include "Common/UpgradeMaskType.h"
#include "GameLogic/Module/DamageModule.h"
#include "GameLogic/Module/ModuleData.h"

template <int N> class BitFlags;
class MultiIniFieldParse;
class Object;
class Thing;
class Xfer;

// The two 0x80-byte masks getUpgradeActivationMasks 0x00460AAC copies out.

// SIZE 0x110

class UpgradeModuleData : public ModuleData
{
public:
	UpgradeMuxData m_upgradeMuxData;									// 0x008

	// Retail's table for the upgrade module datas (0x007F2558, stored by UpgradeModuleData's constructor 0x00253487 and
	// inherited by Attribute/CommandSet/Geometry/MaxHealth/ExperienceScalar/... upgrades) has the shared `mov al, 1; ret`
	// stub 0x0050B5C6 in slot 10; ModuleData's default is false (0x0047A699).
	virtual Bool slot10() const { return true; }
};
// SIZE 0x118

// The public upgrade surface BehaviorModuleInterface::getUpgrade hands out
// (slots 0-10 of the mux tables); UpgradeMux implements it and adds 11-17.
class UpgradeModuleInterface
{
public:
	virtual Bool isAlreadyUpgraded() const = 0;							// slot 0
	virtual Bool attemptUpgrade(const BitFlags<128> &keyMask) = 0;		// slot 1
	virtual Bool wouldUpgrade(const BitFlags<128> &keyMask) const = 0;	// slot 2
	virtual Bool resetUpgrade(const BitFlags<128> &keyMask) = 0;		// slot 3
	virtual Bool isSubObjectsUpgrade() = 0;								// slot 4
	virtual void forceRefreshUpgrade() = 0;								// slot 5
	virtual void vslot06() = 0;											// slot 6
	virtual Bool vslot07() const = 0;									// slot 7
	virtual void upgradeRemovalImplementation() = 0;					// slot 8
	virtual void setUpgradeExecuted(Bool executed) = 0;					// slot 9
	virtual void upgradeImplementation() = 0;							// slot 10
};

class UpgradeMux : public UpgradeModuleInterface
{
public:
	UpgradeMux();												// 0x004CE2A3 (out of line)

	virtual Bool isAlreadyUpgraded() const { return m_upgradeExecuted; }	// slot 0, 0x00098CBC
	virtual Bool attemptUpgrade(const BitFlags<128> &keyMask);			// slot 1, 0x004CE27C
	virtual Bool wouldUpgrade(const BitFlags<128> &keyMask) const;		// slot 2, 0x004CE2B0
	virtual Bool resetUpgrade(const BitFlags<128> &keyMask);			// slot 3, 0x004CE342
	virtual Bool isSubObjectsUpgrade() { return false; }				// slot 4
	virtual void forceRefreshUpgrade();									// slot 5, 0x004CE270
	virtual void vslot06() {}											// slot 6
	virtual Bool vslot07() const { return false; }						// slot 7
	virtual void setUpgradeExecuted(Bool executed);						// slot 9, 0x00452354
	void giveSelfUpgrade();																// 0x0045230C, public QAE in the map

protected:
	virtual void getUpgradeActivationMasks(UpgradeMaskType &activation, UpgradeMaskType &conflicting) const = 0;	// slot 11
	virtual void performUpgradeFX() = 0;								// slot 12
	virtual Bool requiresAllActivationUpgrades() const = 0;				// slot 13
	virtual Bool requiresAllConflictingUpgrades() const = 0;			// slot 14
	virtual void processUpgradeRemoval() = 0;								// slot 15, 0x0045230C calls it at +0x3C
	virtual void upgradeMuxXfer(Xfer *xfer);							// slot 16, 0x004CE397
	virtual void vslot17() {}											// slot 17


	Bool m_upgradeExecuted;												// 0x04
public:
	// The mux's own load pass: an empty, out-of-line body (the shared `ret` 0x000B3FD0) that
	// UpdateModule + UpgradeMux classes call on their +0x20 sub-object after the base's pass
	// (AudioLoopUpgrade 0x004B7BAA, FireWeaponWhenDeadBehavior 0x00482C39) (h05, gen3 vtable-content list).
	void upgradeMuxLoadPostProcess();
};
// SIZE 0x08

class UpgradeModule : public BehaviorModule, public UpgradeMux, public DamageModuleInterface
{
public:
	UpgradeModule(Thing *thing, const ModuleData *moduleData);			// 0x00460AEC

	virtual DamageModuleInterface *getDamage() { return this; }			// 0x0045875F
	virtual UpgradeModuleInterface *getUpgrade() { return this; }

	// COMDAT 0x004CE4B0: the mux's reset, then the CustomAnimAndDuration condition off.
	virtual Bool resetUpgrade(const BitFlags<128> &keyMask)
	{
		if (UpgradeMux::resetUpgrade(keyMask))
		{
			rva004CE41E(false);
			return true;
		}
		return false;
	}
	virtual Bool vslot07() const { return getUpgradeModuleData()->m_upgradeMuxData.m_permanent; }	// 0x00460AE2: data +0x116, Permanent

	virtual void onDamage(DamageInfo *) {}
	virtual void onHealing(DamageInfo *) {}
	virtual void onBodyDamageStateChange(const DamageInfo *, BodyDamageType, BodyDamageType) {}

	const UpgradeModuleData *getUpgradeModuleData() const { return (const UpgradeModuleData *)getModuleData(); }

	// The CustomAnimAndDuration condition (module data +0x108): set (timed when it
	// has frames) or cleared on the object. 0x004CE4A0/0x004CE4A8 are the
	// apply/remove wrappers the upgrade implementations call (h05, from retail).
	void rva004CE41E(Bool apply);										// 0x004CE41E
	void rva004CE4A0();													// 0x004CE4A0
	void rva004CE4A8();													// 0x004CE4A8

protected:
	virtual ~UpgradeModule() {}											// COMDAT 0x0046089D
	virtual const char *GetSnapshotName() const;						// 0x00460B4A
	virtual void xfer(Xfer *xfer);										// 0x004CE3F9

	virtual void getUpgradeActivationMasks(UpgradeMaskType &activation, UpgradeMaskType &conflicting) const;	// 0x00460AAC
	virtual void performUpgradeFX() { getUpgradeModuleData()->m_upgradeMuxData.performUpgradeFX(getObject()); }	// fold 0x00460AD3
	virtual Bool requiresAllActivationUpgrades() const;					// 0x00460A98
	virtual Bool requiresAllConflictingUpgrades() const;				// 0x00460AA2
	virtual void processUpgradeRemoval() { getUpgradeModuleData()->m_upgradeMuxData.muxDataProcessUpgradeRemoval(getObject()); }	// fold 0x00460AD3: performUpgradeFX's bytes (both call the mux data's `ret 4` 0x0047A69C); slot 15 of every UpgradeModule table
protected:
	virtual void loadPostProcess();	// out of line (a jmp to BehaviorModule's), ICF-folded with UpdateModule's at 0x0058B03E; inline, MSVC folds it into each subclass's forwarder and loses retail's second level (0x00497805) (h05)
};
// SIZE 0x1C

#endif
