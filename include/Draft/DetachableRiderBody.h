// DRAFT (h05 modgen): generated from constructor RVA 0x004C1C82 and xfer RVA 0x004C1D0B; no function
// matches against it yet, so it stays in Draft until one does.
// DetachableRiderBody.h: the DetachableRiderBody module. SIZE 0x108 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C1C82 calls ActiveBody first; it stores the
// tables 0x00C5C0B8 at +0x0, 0x00C5BFF8 at +0xC, 0x00C5BF48 at +0x10. Members are what xfer (0x004C1D0B) transfers after ActiveBody::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004C202F ?rva004C202F@Rva004C202F@@QAEXXZ
//   +0x0 slot 2 -> 0x004C1BCE ?name@Rva004C1BCENamed@@QBEPBDXZ
//   +0xC slot 10 -> 0x004C1BD4 ?rva004C1BD4@DetachableRiderBody@@UAEPAVUpgradeMux@@XZ
//   +0x10 slot 0 -> 0x004C1D49 ?attemptDamage@DetachableRiderBody@@UAEXPAVDamageInfo@@@Z
// Not established: transfers xfer makes that this view cannot place: ?xfer@ActiveBody@@MAEXPAVXfer@@@Z ?.
// Round 7 (h05, by hand): the constructor also builds an UpgradeMux at +0x100 (`lea edi,[esi+0x100]`,
// ??0UpgradeMux, then its table 0x00C5BF00 stored through edi), so the class is ActiveBody +
// UpgradeMux; xfer transfers the mux through UpgradeMux::upgradeMuxXfer when the version is 2.
#pragma once
#ifndef BFME2_DETACHABLE_RIDER_BODY_H
#define BFME2_DETACHABLE_RIDER_BODY_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Draft/ActiveBody.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "Draft/DetachableRiderBodyModuleData.h"

class Thing;
class ModuleData;
class Xfer;

// ActiveBody ends at 0x100; this class's members follow it.
typedef char DetachableRiderBody_base_size[(sizeof(ActiveBody) == 0x100) ? 1 : -1];

class DetachableRiderBody : public ActiveBody, public UpgradeMux
{
public:
	DetachableRiderBody(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual void attemptDamage(DamageInfo *);	// override: attemptDamage slot 0 at +0x10 (0x004C1D49)
	virtual UpgradeMux *getUpgrade();				// slot 10: 0x004C1BD4, the null-preserving upcast to the UpgradeMux base at +0x100

protected:
	virtual void loadPostProcess() { ActiveBody::loadPostProcess(); }	// slot 1: retail 0x004C202F, a jmp to ActiveBody's (0x004BF40F), ICF-folded across the ActiveBody subclasses
	virtual void xfer(Xfer *xfer);

	// UpgradeMux (table 0x00C5BF00 at +0x100)
	virtual void upgradeRemovalImplementation();						// slot 8
	virtual void upgradeImplementation();								// slot 10
	virtual void getUpgradeActivationMasks(UpgradeMaskType &activation, UpgradeMaskType &conflicting) const;	// slot 11, 0x004C1C29
	virtual void performUpgradeFX();									// slot 12, 0x004C1C53
	virtual Bool requiresAllActivationUpgrades() const;					// slot 13, 0x004C1C68
	virtual Bool requiresAllConflictingUpgrades() const;				// slot 14, 0x004C1C75
	virtual void processUpgradeRemoval() { ((const DetachableRiderBodyModuleData *)getModuleData())->m_upgradeMuxData.muxDataProcessUpgradeRemoval(getObject()); }	// slot 15: retail 0x004C1C53, ICF-folded with performUpgradeFX (both call the mux data's `ret 4` 0x0047A69C)
};
// SIZE 0x108
typedef char DetachableRiderBody_size[(sizeof(DetachableRiderBody) == 0x108) ? 1 : -1];

#endif
