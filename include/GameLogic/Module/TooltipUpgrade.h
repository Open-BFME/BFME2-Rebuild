// TooltipUpgrade.h: the TooltipUpgrade module. SIZE 0x1C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B7A50 calls UpgradeModule first; it stores the
// tables 0x00C58C08 at +0x0, 0x00C57328 at +0xC, 0x00C58BC0 at +0x10, 0x00C58790 at +0x18. Members are what xfer (0x004B8710) transfers after UpgradeModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x10 slot 0 -> 0x00098CBC ?Is_Valid@RegistryClass@@QAE_NXZ
//   +0x10 slot 1 -> 0x004CE27C ?attemptUpgrade@UpgradeMux@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 2 -> 0x004CE2B0 ?wouldUpgrade@UpgradeMux@@UBE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 3 -> 0x004CE4B0 ?resetUpgrade@UpgradeModule@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 4 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x10 slot 5 -> 0x004CE270 ?forceRefreshUpgrade@UpgradeMux@@UAEXXZ
//   +0x10 slot 6 -> 0x004B3B42 ?postUpgradeCheck@CommandSetUpgrade@@UAEXXZ
//   +0x10 slot 7 -> 0x00460AE2 ?vslot07@UpgradeModule@@UBE_NXZ
//   +0x10 slot 8 -> 0x004B7ADB ?upgradeRemovalImplementation@TooltipUpgrade@@MAEXXZ
//   +0x10 slot 9 -> 0x00452354 ?setUpgradeExecuted@UpgradeMux@@UAEX_N@Z
//   +0x10 slot 10 -> 0x004B7A9D ?upgradeImplementation@TooltipUpgrade@@MAEXXZ
//   +0x10 slot 11 -> 0x00460AAC ?getUpgradeActivationMasks@UpgradeModule@@MBEXAAUUpgradeMaskType@@0@Z
//   +0x10 slot 12 -> 0x00460AD3 ?performUpgradeFX@UpgradeModule@@MAEXXZ
//   +0x10 slot 13 -> 0x00460A98 ?requiresAllActivationUpgrades@UpgradeModule@@MBE_NXZ
//   +0x10 slot 14 -> 0x00460AA2 ?requiresAllConflictingUpgrades@UpgradeModule@@MBE_NXZ
//   +0x10 slot 16 -> 0x004CE397 ?upgradeMuxXfer@UpgradeMux@@MAEXPAVXfer@@@Z
//   +0x10 slot 17 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 27 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 28 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
#pragma once
#ifndef BFME2_TOOLTIP_UPGRADE_H
#define BFME2_TOOLTIP_UPGRADE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpgradeModule ends at 0x1C; this class's members follow it.
typedef char TooltipUpgrade_base_size[(sizeof(UpgradeModule) == 0x1C) ? 1 : -1];

class TooltipUpgrade : public UpgradeModule
{
public:
	TooltipUpgrade(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~TooltipUpgrade();

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void upgradeRemovalImplementation();	// override: upgradeRemovalImplementation slot 8 at +0x10 (0x004B7ADB)
	virtual void upgradeImplementation();	// override: upgradeImplementation slot 10 at +0x10 (0x004B7A9D)

public:
public:
	virtual void vslot06();	// UpgradeMux slot 6: folded with CommandSetUpgrade's 0x004B3B42; retail's table has this class's own (h05, gen3 vtable-content list)
protected:
	virtual void loadPostProcess() { UpgradeModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpgradeModule's, ICF-folded across classes (h05)
};
// SIZE 0x1C
typedef char TooltipUpgrade_size[(sizeof(TooltipUpgrade) == 0x1C) ? 1 : -1];

#endif
