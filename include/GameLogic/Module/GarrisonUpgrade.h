// GarrisonUpgrade.h: the GarrisonUpgrade module. SIZE 0x1C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B5839 calls UpgradeModule first; it stores the
// tables 0x00C58088 at +0x0, 0x00C57328 at +0xC, 0x00C58040 at +0x10, 0x00C58790 at +0x18. Members are what xfer (0x004B8710) transfers after UpgradeModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 12 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 13 -> 0x004B5B82 ?rva004B5B82@Rva004B5B82@@UAEXXZ
//   +0x0 slot 15 -> 0x00098CBC ?Is_Valid@RegistryClass@@QAE_NXZ
//   +0x0 slot 17 -> 0x004CE27C ?attemptUpgrade@UpgradeMux@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x0 slot 18 -> 0x004CE2B0 ?wouldUpgrade@UpgradeMux@@UBE_NABV?$BitFlags@$0IA@@@@Z
//   +0x0 slot 19 -> 0x004CE4B0 ?resetUpgrade@UpgradeModule@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x0 slot 20 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 21 -> 0x004CE270 ?forceRefreshUpgrade@UpgradeMux@@UAEXXZ
//   +0x0 slot 23 -> 0x00460AE2 ?vslot07@UpgradeModule@@UBE_NXZ
//   +0x0 slot 24 -> 0x004B5A65 ?upgradeRemovalImplementation@Rva004B5A65_Secondary@@QAEXXZ
//   +0x0 slot 25 -> 0x00452354 ?setUpgradeExecuted@UpgradeMux@@UAEX_N@Z
//   +0x0 slot 26 -> 0x004B5B4C ?upgradeImplementation@Rva004B5A65_Secondary@@QAEXXZ
//   +0x0 slot 27 -> 0x00460AAC ?getUpgradeActivationMasks@UpgradeModule@@MBEXAAUUpgradeMaskType@@0@Z
//   +0x0 slot 28 -> 0x00460AD3 ?performUpgradeFX@UpgradeModule@@MAEXXZ
//   +0x0 slot 29 -> 0x00460A98 ?requiresAllActivationUpgrades@UpgradeModule@@MBE_NXZ
//   +0x0 slot 30 -> 0x00460AA2 ?requiresAllConflictingUpgrades@UpgradeModule@@MBE_NXZ
//   +0x0 slot 32 -> 0x004CE397 ?upgradeMuxXfer@UpgradeMux@@MAEXPAVXfer@@@Z
//   +0x0 slot 34 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 37 -> 0x004B4A8C ?getCreate@ReplaceSelfUpgrade@@UAEPAVCreateModuleInterface@@XZ
//   +0x0 slot 38 -> 0x0045875F ?getDamage@UpgradeModule@@UAEPAVDamageModuleInterface@@XZ
//   +0x0 slot 44 -> 0x002533D3 ?getDie@DieModule@@UAEPAVDieModuleInterface@@XZ
//   +0x0 slot 77 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 83 -> 0x004B59A9 ?name@Rva004B59A9Named@@QBEPBDXZ
//   +0x0 slot 84 -> 0x004B5E06 ?rva004B5E06@Rva004B5E06@@QAEXPAVXfer@@@Z
//   +0x0 slot 89 -> 0x004B59AF ?rva004B59AF@Rva004B59AF@@QAEXXZ
//   +0x0 slot 90 -> 0x004B5AC0 ?onCapture@Rva004B5AC0_Primary@@QAIXHPAVPlayer@@0@Z
//   +0x10 slot 8 -> 0x004B58FF ?upgradeRemovalImplementation@GarrisonUpgrade@@MAEXXZ
//   +0x10 slot 10 -> 0x004B5886 ?upgradeImplementation@GarrisonUpgrade@@MAEXXZ
//   +0x10 slot 27 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
#pragma once
#ifndef BFME2_GARRISON_UPGRADE_H
#define BFME2_GARRISON_UPGRADE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpgradeModule ends at 0x1C; this class's members follow it.
typedef char GarrisonUpgrade_base_size[(sizeof(UpgradeModule) == 0x1C) ? 1 : -1];

class GarrisonUpgrade : public UpgradeModule
{
public:
	GarrisonUpgrade(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~GarrisonUpgrade();

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void upgradeRemovalImplementation();	// override: upgradeRemovalImplementation slot 8 at +0x10 (0x004B58FF)
	virtual void upgradeImplementation();	// override: upgradeImplementation slot 10 at +0x10 (0x004B5886)

public:
protected:
	virtual void loadPostProcess() { UpgradeModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpgradeModule's, ICF-folded across classes (h05)
};
// SIZE 0x1C
typedef char GarrisonUpgrade_size[(sizeof(GarrisonUpgrade) == 0x1C) ? 1 : -1];

#endif
