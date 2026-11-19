// the +0x1C table is CreateModuleInterface (identified from its
// four slots: empty, onBuildComplete, empty, shouldDoOnBuildComplete), so name, size, bases and
// virtual order are established; the constructor 0x004B7045 matches against it.
// ReplaceSelfUpgrade.h: the ReplaceSelfUpgrade module. SIZE 0x20 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B7045 calls UpgradeModule first; it stores the
// tables 0x00C4EF80 at +0x1C, 0x00C58B10 at +0x0, 0x00C58110 at +0xC, 0x00C58AC8 at +0x10, 0x00C58790 at +0x18, 0x00C58AB8 at +0x1C. Members are what xfer (0x004B6FF7) transfers after UpgradeModule::xfer,
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
//   +0x0 slot 13 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 15 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 16 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 22 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x0 slot 26 -> 0x000B69A1 ?slot14@ModuleData@@UBE?AVAsciiString@@H@Z
//   +0x0 slot 28 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0xC slot 3 -> 0x004B4A8C ?getCreate@ReplaceSelfUpgrade@@UAEPAVCreateModuleInterface@@XZ
//   +0xC slot 49 -> 0x004B59A9 ?name@Rva004B59A9Named@@QBEPBDXZ
//   +0xC slot 50 -> 0x004B5E06 ?rva004B5E06@Rva004B5E06@@QAEXPAVXfer@@@Z
//   +0xC slot 55 -> 0x004B59AF ?rva004B59AF@Rva004B59AF@@QAEXXZ
//   +0xC slot 56 -> 0x004B5AC0 ?onCapture@Rva004B5AC0_Primary@@QAIXHPAVPlayer@@0@Z
//   +0x10 slot 0 -> 0x00098CBC ?Is_Valid@RegistryClass@@QAE_NXZ
//   +0x10 slot 1 -> 0x004CE27C ?attemptUpgrade@UpgradeMux@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 2 -> 0x004B7110 ?wouldUpgrade@ReplaceSelfUpgrade@@UBE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 3 -> 0x004CE4B0 ?resetUpgrade@UpgradeModule@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 5 -> 0x004CE270 ?forceRefreshUpgrade@UpgradeMux@@UAEXXZ
//   +0x10 slot 7 -> 0x00460AE2 ?vslot07@UpgradeModule@@UBE_NXZ
//   +0x10 slot 9 -> 0x00452354 ?setUpgradeExecuted@UpgradeMux@@UAEX_N@Z
//   +0x10 slot 10 -> 0x004B74CB ?upgradeImplementation@ReplaceSelfUpgrade@@UAEXXZ
//   +0x10 slot 11 -> 0x00460AAC ?getUpgradeActivationMasks@UpgradeModule@@MBEXAAUUpgradeMaskType@@0@Z
//   +0x10 slot 12 -> 0x00460AD3 ?performUpgradeFX@UpgradeModule@@MAEXXZ
//   +0x10 slot 13 -> 0x00460A98 ?requiresAllActivationUpgrades@UpgradeModule@@MBE_NXZ
//   +0x10 slot 14 -> 0x00460AA2 ?requiresAllConflictingUpgrades@UpgradeModule@@MBE_NXZ
//   +0x10 slot 16 -> 0x004CE397 ?upgradeMuxXfer@UpgradeMux@@MAEXPAVXfer@@@Z
//   +0x10 slot 27 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x1C slot 1 -> 0x004B70F0 ?rva004B70F0@ReplaceSelfUpgrade@@UAEXXZ
// Not established: interface tables at +0x1C (their vptrs sit in the gaps there).
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpgradeModule@@MAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_REPLACE_SELF_UPGRADE_H
#define BFME2_REPLACE_SELF_UPGRADE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "GameLogic/Module/CreateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpgradeModule ends at 0x1C; this class's members follow it.
typedef char ReplaceSelfUpgrade_base_size[(sizeof(UpgradeModule) == 0x1C) ? 1 : -1];

class ReplaceSelfUpgrade : public UpgradeModule, public CreateModuleInterface
{
public:
	ReplaceSelfUpgrade(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual CreateModuleInterface *getCreate();	// override: getCreate slot 3 at +0xC (0x004B4A8C)
	virtual Bool wouldUpgrade(const BitFlags<128> &keyMask) const;	// override: wouldUpgrade slot 2 at +0x10 (0x004B7110)
	virtual void upgradeImplementation();	// override: upgradeImplementation slot 10 at +0x10 (0x004B74CB)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);


public:
	// CreateModuleInterface (+0x1C, the folded four-slot pure table 0x00C4EF80; OpenContain's +0x34
	// and DynamicPortalBehaviour's +0x20, whose slot 1 the map names onBuildComplete, have the same shape)
	virtual void onCreate() {}											// slot 0: the shared empty body 0x000B3FD0
	virtual void onBuildComplete();										// slot 1: 0x004B70F0 (map: rva004B70F0)
	virtual void vslot02() {}											// slot 2: the shared empty body
	virtual Bool shouldDoOnBuildComplete() const { return true; }			// slot 3: 0x0050B5C6, `mov al,1`
protected:
	virtual ~ReplaceSelfUpgrade() {}
	virtual void upgradeRemovalImplementation() {}	// UpgradeMux slot 8: the shared empty body 0x000B3FD0	// the map spells it protected (??1ReplaceSelfUpgrade@@MAE@XZ, 0x004B70EB)
	virtual void loadPostProcess() { UpgradeModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpgradeModule's (h05)
};
// SIZE 0x20
typedef char ReplaceSelfUpgrade_size[(sizeof(ReplaceSelfUpgrade) == 0x20) ? 1 : -1];

#endif
