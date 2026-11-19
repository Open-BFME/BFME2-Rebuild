// SubObjectsUpgrade.h: the SubObjectsUpgrade module. SIZE 0x24 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B4D63 calls UpgradeModule first; it stores the
// tables 0x00C57C40 at +0x0, 0x00C57328 at +0xC, 0x00C57BF8 at +0x10, 0x00C58790 at +0x18. Members are what xfer (0x004B4F67) transfers after UpgradeModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004B4D05 ?loadPostProcess@Rva004B4CDF@@MAEXXZ
//   +0x0 slot 2 -> 0x004B4CFF ?name@Rva004B4CFFNamed@@QBEPBDXZ
//   +0x10 slot 0 -> 0x00098CBC ?Is_Valid@RegistryClass@@QAE_NXZ
//   +0x10 slot 1 -> 0x004CE27C ?attemptUpgrade@UpgradeMux@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 2 -> 0x004CE2B0 ?wouldUpgrade@UpgradeMux@@UBE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 3 -> 0x004CE4B0 ?resetUpgrade@UpgradeModule@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 4 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x10 slot 5 -> 0x004CE270 ?forceRefreshUpgrade@UpgradeMux@@UAEXXZ
//   +0x10 slot 6 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 7 -> 0x00460AE2 ?vslot07@UpgradeModule@@UBE_NXZ
//   +0x10 slot 8 -> 0x004B5101 ?upgradeRemovalImplementation@SubObjectsUpgrade@@MAEXXZ
//   +0x10 slot 9 -> 0x00452354 ?setUpgradeExecuted@UpgradeMux@@UAEX_N@Z
//   +0x10 slot 10 -> 0x004B5020 ?upgradeImplementation@SubObjectsUpgrade@@MAEXXZ
//   +0x10 slot 11 -> 0x00460AAC ?getUpgradeActivationMasks@UpgradeModule@@MBEXAAUUpgradeMaskType@@0@Z
//   +0x10 slot 12 -> 0x00460AD3 ?performUpgradeFX@UpgradeModule@@MAEXXZ
//   +0x10 slot 13 -> 0x00460A98 ?requiresAllActivationUpgrades@UpgradeModule@@MBE_NXZ
//   +0x10 slot 14 -> 0x00460AA2 ?requiresAllConflictingUpgrades@UpgradeModule@@MBE_NXZ
//   +0x10 slot 16 -> 0x004CE397 ?upgradeMuxXfer@UpgradeMux@@MAEXPAVXfer@@@Z
//   +0x10 slot 27 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 28 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpgradeModule@@MAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_SUB_OBJECTS_UPGRADE_H
#define BFME2_SUB_OBJECTS_UPGRADE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"

class Thing;
class ModuleData;
class Xfer;
class Drawable;

// UpgradeModule ends at 0x1C; this class's members follow it.
typedef char SubObjectsUpgrade_base_size[(sizeof(UpgradeModule) == 0x1C) ? 1 : -1];

class SubObjectsUpgrade : public UpgradeModule
{
public:
	SubObjectsUpgrade(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;

	// The fade rates the module data's FadeTimeInSeconds and WaitBeforeFadeInSeconds give (1 / seconds / client fps); both
	// stay untouched while the object was produced within three frames of the module's creation and has no producer.
	void rva004B4DBF(Real *fadeRate, Real *waitRate);					// 0x004B4DBF
	// True when neither the object nor its player already holds one of the conflicting upgrades.
	UnsignedByte rva004B4E56();											// 0x004B4E56
	// Show (0x004B4EC3) or hide (0x004B4F11) each named sub-object with the fade rates; the list is a vector<AsciiString>.
	Bool rva004B4EC3(Drawable *draw, void *names, Real fadeRate, Real waitRate);	// 0x004B4EC3
	Bool rva004B4F11(Drawable *draw, void *names, Real fadeRate, Real waitRate);	// 0x004B4F11
	Bool swapTextures(Drawable *draw, Int restore);						// 0x004B4F91
	virtual Bool isSubObjectsUpgrade() { return true; }				// UpgradeMux slot 4: retail 0x0050B5C6 (`mov al,1`), as in Zero Hour (h05 r9)

protected:
	virtual void loadPostProcess();										// 0x004B4D05 (the map's owner is the address-named Rva004B4CDF): re-applies the upgrade when it was applied
	virtual void xfer(Xfer *xfer);
	virtual void upgradeRemovalImplementation();	// override: upgradeRemovalImplementation slot 8 at +0x10 (0x004B5101)
	virtual void upgradeImplementation();	// override: upgradeImplementation slot 10 at +0x10 (0x004B5020)

public:
	UnsignedInt m_creationFrame;                   // +0x1C; TheGameLogic's frame when the module was built (constructor 0x004B4D63)
	Bool m_applied;                                // +0x20; xfer Bool; set by upgradeImplementation
};
// SIZE 0x24
typedef char SubObjectsUpgrade_size[(sizeof(SubObjectsUpgrade) == 0x24) ? 1 : -1];
typedef char SubObjectsUpgrade_off_m_creationFrame[(offsetof(SubObjectsUpgrade, m_creationFrame) == 0x1C) ? 1 : -1];
typedef char SubObjectsUpgrade_off_m_applied[(offsetof(SubObjectsUpgrade, m_applied) == 0x20) ? 1 : -1];

#endif
