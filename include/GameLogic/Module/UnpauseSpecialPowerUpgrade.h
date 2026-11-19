// UnpauseSpecialPowerUpgrade.h: the UnpauseSpecialPowerUpgrade module. SIZE 0x1C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B548C calls UpgradeModule first; it stores the
// tables 0x00C57EA8 at +0x0, 0x00C57328 at +0xC, 0x00C57E60 at +0x10, 0x00C58790 at +0x18. Members are what xfer (0x004B8710) transfers after UpgradeModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004B53FF ?name@Rva004B53FFNamed@@QBEPBDXZ
//   +0x0 slot 5 -> 0x004B54D9 ?onCreate@UnpauseSpecialPowerUpgrade@@MAEXXZ
//   +0x10 slot 0 -> 0x00098CBC ?Is_Valid@RegistryClass@@QAE_NXZ
//   +0x10 slot 1 -> 0x004CE27C ?attemptUpgrade@UpgradeMux@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 2 -> 0x004CE2B0 ?wouldUpgrade@UpgradeMux@@UBE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 3 -> 0x004CE4B0 ?resetUpgrade@UpgradeModule@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 4 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x10 slot 5 -> 0x004CE270 ?forceRefreshUpgrade@UpgradeMux@@UAEXXZ
//   +0x10 slot 6 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 7 -> 0x00460AE2 ?vslot07@UpgradeModule@@UBE_NXZ
//   +0x10 slot 8 -> 0x004B5574 ?upgradeRemovalImplementation@UnpauseSpecialPowerUpgrade@@MAEXXZ
//   +0x10 slot 9 -> 0x00452354 ?setUpgradeExecuted@UpgradeMux@@UAEX_N@Z
//   +0x10 slot 10 -> 0x004B54E9 ?upgradeImplementation@UnpauseSpecialPowerUpgrade@@MAEXXZ
//   +0x10 slot 11 -> 0x00460AAC ?getUpgradeActivationMasks@UpgradeModule@@MBEXAAUUpgradeMaskType@@0@Z
//   +0x10 slot 12 -> 0x00460AD3 ?performUpgradeFX@UpgradeModule@@MAEXXZ
//   +0x10 slot 13 -> 0x00460A98 ?requiresAllActivationUpgrades@UpgradeModule@@MBE_NXZ
//   +0x10 slot 14 -> 0x00460AA2 ?requiresAllConflictingUpgrades@UpgradeModule@@MBE_NXZ
//   +0x10 slot 16 -> 0x004CE397 ?upgradeMuxXfer@UpgradeMux@@MAEXPAVXfer@@@Z
//   +0x10 slot 27 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 28 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
#pragma once
#ifndef BFME2_UNPAUSE_SPECIAL_POWER_UPGRADE_H
#define BFME2_UNPAUSE_SPECIAL_POWER_UPGRADE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"

class Thing;
class ModuleData;
class Xfer;
class SpecialPowerTemplate;
class AsciiString;

// UpgradeModule ends at 0x1C; this class's members follow it.
typedef char UnpauseSpecialPowerUpgrade_base_size[(sizeof(UpgradeModule) == 0x1C) ? 1 : -1];

class UnpauseSpecialPowerUpgrade : public UpgradeModule
{
public:
	UnpauseSpecialPowerUpgrade(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual ~UnpauseSpecialPowerUpgrade();
	const SpecialPowerTemplate *getSpecialPowerTemplate() const;	// 0x004B554F: the module data's SpecialPowerTemplate, NULL without data
	const AsciiString &getFirstTriggerUpgradeName() const;		// 0x004B555F: the first upgrade of the data's TriggeredBy mask (Object::AddCreateAHeroSpecialPowerUpgrade 0x002972DE asks it)

protected:
	virtual void xfer(Xfer *xfer);
	virtual void upgradeRemovalImplementation();	// override: upgradeRemovalImplementation slot 8 at +0x10 (0x004B5574)
	virtual void upgradeImplementation();	// override: upgradeImplementation slot 10 at +0x10 (0x004B54E9)

public:
public:
	virtual void onObjectCreated();	// 0x004B54D9, Module slot 5; retail's table has this class's own (h05, gen3 vtable-content list)
protected:
	virtual void loadPostProcess() { UpgradeModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpgradeModule's, ICF-folded across classes (h05)
};
// SIZE 0x1C
typedef char UnpauseSpecialPowerUpgrade_size[(sizeof(UnpauseSpecialPowerUpgrade) == 0x1C) ? 1 : -1];

#endif
