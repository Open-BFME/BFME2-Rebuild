// the +0x1C table is CreateModuleInterface (identified from its
// four slots: empty, onBuildComplete, empty, shouldDoOnBuildComplete), so name, size, bases and
// virtual order are established; the constructor 0x004B5D7D matches against it.
// SpellRechargeModifierUpgrade.h: the SpellRechargeModifierUpgrade module. SIZE 0x24 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B5D7D calls UpgradeModule first; it stores the
// tables 0x00C4EF80 at +0x1C, 0x00C583B0 at +0x0, 0x00C58110 at +0xC, 0x00C58368 at +0x10, 0x00C58790 at +0x18, 0x00C58358 at +0x1C. Members are what xfer (0x004B5E06) transfers after UpgradeModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x1C slot 0 -> 0x0003B810 __purecall
//   +0x1C slot 4 -> 0x00495DC6 ?get@Rva00495DC6PtrChaseField@@QBEHXZ
//   +0x1C slot 5 -> 0x00495DCD ?get@Rva00495DCDByteChaseField@@QBEEXZ
//   +0x1C slot 6 -> 0x00495C6E ?rva00495C6E@AutoPickUpUpdate@@UAEXXZ
//   +0x1C slot 7 -> 0x0058B826 ?getIsQuitting@FrameDataManager@@QAE_NXZ
//   +0x1C slot 8 -> 0x00495E01 ?update@AutoPickUpUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x1C slot 9 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x1C slot 10 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004B5D32 ?name@Rva004B5D32Named@@QBEPBDXZ
//   +0x0 slot 3 -> 0x004B5E06 ?rva004B5E06@Rva004B5E06@@QAEXPAVXfer@@@Z
//   +0x0 slot 8 -> 0x004B59AF ?rva004B59AF@Rva004B59AF@@QAEXXZ
//   +0x0 slot 9 -> 0x004B5EC4 ?onCapture@SpellRechargeModifierUpgrade@@UAEXPAVPlayer@@0@Z
//   +0x0 slot 13 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 15 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 16 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 22 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x0 slot 26 -> 0x000B69A1 ?slot14@ModuleData@@UBE?AVAsciiString@@H@Z
//   +0x0 slot 28 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0xC slot 3 -> 0x004B4A8C ?getCreate@ReplaceSelfUpgrade@@UAEPAVCreateModuleInterface@@XZ
//   +0xC slot 49 -> 0x004B59A9 ?name@Rva004B59A9Named@@QBEPBDXZ
//   +0xC slot 56 -> 0x004B5AC0 ?onCapture@Rva004B5AC0_Primary@@QAIXHPAVPlayer@@0@Z
//   +0x10 slot 0 -> 0x00098CBC ?Is_Valid@RegistryClass@@QAE_NXZ
//   +0x10 slot 1 -> 0x004CE27C ?attemptUpgrade@UpgradeMux@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 2 -> 0x004CE2B0 ?wouldUpgrade@UpgradeMux@@UBE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 3 -> 0x004CE4B0 ?resetUpgrade@UpgradeModule@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 5 -> 0x004CE270 ?forceRefreshUpgrade@UpgradeMux@@UAEXXZ
//   +0x10 slot 7 -> 0x00460AE2 ?vslot07@UpgradeModule@@UBE_NXZ
//   +0x10 slot 8 -> 0x004B5E46 ?upgradeRemovalImplementation@SpellRechargeModifierUpgrade@@UAEXXZ
//   +0x10 slot 9 -> 0x00452354 ?setUpgradeExecuted@UpgradeMux@@UAEX_N@Z
//   +0x10 slot 10 -> 0x004B5F9F ?upgradeImplementation@SpellRechargeModifierUpgrade@@UAEXXZ
//   +0x10 slot 11 -> 0x00460AAC ?getUpgradeActivationMasks@UpgradeModule@@MBEXAAUUpgradeMaskType@@0@Z
//   +0x10 slot 12 -> 0x00460AD3 ?performUpgradeFX@UpgradeModule@@MAEXXZ
//   +0x10 slot 13 -> 0x00460A98 ?requiresAllActivationUpgrades@UpgradeModule@@MBE_NXZ
//   +0x10 slot 14 -> 0x00460AA2 ?requiresAllConflictingUpgrades@UpgradeModule@@MBE_NXZ
//   +0x10 slot 16 -> 0x004CE397 ?upgradeMuxXfer@UpgradeMux@@MAEXPAVXfer@@@Z
//   +0x1C slot 1 -> 0x004B5DDC ?rva004B5DDC@SpellRechargeModifierUpgrade@@UAEXXZ
// Not established: interface tables at +0x1C (their vptrs sit in the gaps there).
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpgradeModule@@MAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_SPELL_RECHARGE_MODIFIER_UPGRADE_H
#define BFME2_SPELL_RECHARGE_MODIFIER_UPGRADE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "GameLogic/Module/CreateModule.h"

class Thing;
class Player;
class ModuleData;
class Xfer;

// UpgradeModule ends at 0x1C; this class's members follow it.
typedef char SpellRechargeModifierUpgrade_base_size[(sizeof(UpgradeModule) == 0x1C) ? 1 : -1];

class SpellRechargeModifierUpgrade : public UpgradeModule, public CreateModuleInterface
{
public:
	SpellRechargeModifierUpgrade(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void onDelete();											// +0x0 slot 8: 0x004B59AF (shared with CostModifierUpgrade)
	virtual CreateModuleInterface *getCreate() { return this; }			// +0xC slot 3: the shared 0x004B4A8C
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void onCapture(Player *oldOwner, Player *newOwner);	// override: onCapture slot 9 at +0x0 (0x004B5EC4)
	virtual void upgradeRemovalImplementation();	// override: upgradeRemovalImplementation slot 8 at +0x10 (0x004B5E46)
	virtual void upgradeImplementation();	// override: upgradeImplementation slot 10 at +0x10 (0x004B5F9F)


public:
	// CreateModuleInterface (+0x1C, the folded four-slot pure table 0x00C4EF80; OpenContain's +0x34
	// and DynamicPortalBehaviour's +0x20, whose slot 1 the map names onBuildComplete, have the same shape)
	virtual void onCreate() {}											// slot 0: the shared empty body 0x000B3FD0
	virtual void onBuildComplete();										// slot 1: 0x004B5DDC (map: rva004B5DDC)
	virtual void vslot02() {}											// slot 2: the shared empty body
	virtual Bool shouldDoOnBuildComplete() const { return m_unk0x20; }			// slot 3: 0x00098CBC, `mov al,[ecx+4]` (the Bool at +0x20)

	Bool m_unk0x20;										// +0x20; xfer Bool; the constructor sets it, onBuildComplete sets it again, slot 3 returns it
protected:
	virtual void loadPostProcess() { UpgradeModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpgradeModule's, ICF-folded across classes (h05)
};
// SIZE 0x24
typedef char SpellRechargeModifierUpgrade_size[(sizeof(SpellRechargeModifierUpgrade) == 0x24) ? 1 : -1];
typedef char SpellRechargeModifierUpgrade_off_m_unk0x20[(offsetof(SpellRechargeModifierUpgrade, m_unk0x20) == 0x20) ? 1 : -1];

#endif
