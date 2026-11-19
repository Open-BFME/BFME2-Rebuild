// WeaponBonusUpgrade.h: the WeaponBonusUpgrade module. SIZE 0x1C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B5633 calls UpgradeModule first; it stores the
// tables 0x00C57F68 at +0x0, 0x00C57328 at +0xC, 0x00C57F20 at +0x10, 0x00C58790 at +0x18. Members are what xfer (0x004B8710) transfers after UpgradeModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 12 -> 0x00098CBC ?Is_Valid@RegistryClass@@QAE_NXZ
//   +0x0 slot 13 -> 0x004CE27C ?attemptUpgrade@UpgradeMux@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x0 slot 14 -> 0x004CE2B0 ?wouldUpgrade@UpgradeMux@@UBE_NABV?$BitFlags@$0IA@@@@Z
//   +0x0 slot 15 -> 0x004CE4B0 ?resetUpgrade@UpgradeModule@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x0 slot 16 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 17 -> 0x004CE270 ?forceRefreshUpgrade@UpgradeMux@@UAEXXZ
//   +0x0 slot 18 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 19 -> 0x00460AE2 ?vslot07@UpgradeModule@@UBE_NXZ
//   +0x0 slot 20 -> 0x004B5783 ?upgradeRemovalImplementation@WeaponSetUpgradeSecondary@@QAEXXZ
//   +0x0 slot 21 -> 0x00452354 ?setUpgradeExecuted@UpgradeMux@@UAEX_N@Z
//   +0x0 slot 22 -> 0x004B5765 ?upgradeImplementation@WeaponSetUpgradeSecondary@@QAEXXZ
//   +0x0 slot 23 -> 0x00460AAC ?getUpgradeActivationMasks@UpgradeModule@@MBEXAAUUpgradeMaskType@@0@Z
//   +0x0 slot 24 -> 0x00460AD3 ?performUpgradeFX@UpgradeModule@@MAEXXZ
//   +0x0 slot 25 -> 0x00460A98 ?requiresAllActivationUpgrades@UpgradeModule@@MBE_NXZ
//   +0x0 slot 26 -> 0x00460AA2 ?requiresAllConflictingUpgrades@UpgradeModule@@MBE_NXZ
//   +0x0 slot 28 -> 0x004CE397 ?upgradeMuxXfer@UpgradeMux@@MAEXPAVXfer@@@Z
//   +0x0 slot 39 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x0 slot 40 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x10 slot 10 -> 0x004B5680 ?upgradeImplementation@WeaponBonusUpgrade@@MAEXXZ
#pragma once
#ifndef BFME2_WEAPON_BONUS_UPGRADE_H
#define BFME2_WEAPON_BONUS_UPGRADE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpgradeModule ends at 0x1C; this class's members follow it.
typedef char WeaponBonusUpgrade_base_size[(sizeof(UpgradeModule) == 0x1C) ? 1 : -1];

class WeaponBonusUpgrade : public UpgradeModule
{
public:
	WeaponBonusUpgrade(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~WeaponBonusUpgrade();

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void upgradeImplementation();

public:
protected:
	virtual void upgradeRemovalImplementation() {}	// UpgradeMux slot 8: retail's is the shared empty body 0x000B3FD0 (h05, gen3 vtable-content list)
protected:
	virtual void loadPostProcess() { UpgradeModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpgradeModule's, ICF-folded across classes (h05)
};
// SIZE 0x1C
typedef char WeaponBonusUpgrade_size[(sizeof(WeaponBonusUpgrade) == 0x1C) ? 1 : -1];

#endif
