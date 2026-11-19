// DRAFT (h05 modgen): generated from constructor RVA 0x00460B6C and xfer RVA 0x004609BC; no function
// matches against it yet, so it stays in Draft until one does.
// DynamicPortalBehaviour.h: the DynamicPortalBehaviour module. SIZE 0x40 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00460B6C calls UpgradeModule first; it stores the
// tables 0x00C1C780 at +0x1C, 0x00C4EF80 at +0x20, 0x00C428C4 at +0x0, 0x00C42808 at +0xC, 0x00C427C0 at +0x10, 0x00C58790 at +0x18, 0x00C427B8 at +0x1C, 0x00C427A8 at +0x20. Members are what xfer (0x004609BC) transfers after UpgradeModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x1C slot 0 -> 0x0003B810 __purecall
//   +0x1C slot 2 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x1C slot 5 -> 0x0055CFF0 ?rva0055CFF0@Rva0055CFF0@@QAEXMMM@Z
//   +0x1C slot 6 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x1C slot 7 -> 0x003ABEFD ?rva003ABEFD@Rva003ABEFD@@QAEMXZ
//   +0x1C slot 8 -> 0x0055D0D5 ?rva0055D0D5@Rva0055D0D5@@QAE?AUCoord3D0055D0D5@@IIII@Z
//   +0x1C slot 9 -> 0x003ABFD9 ??_ERva003ABC58@@WBM@AEPAXI@Z
//   +0x1C slot 13 -> 0x0055E636 ?Rva0055E636Compute@@YGXPAVCoord3D@@0MM@Z
//   +0x1C slot 17 -> 0x0055D3F5 ?rva0055D3F5@Rva0055D3F5@@UAEXPAVXfer@@@Z
//   +0x1C slot 18 -> 0x0055D422 ?debugDraw@Rva0055D3F5@@QAEXUCoord3D@@@Z
//   +0x1C slot 21 -> 0x0055D48C ?getPosition@SphereEmissionVolumeModule@FXParticleSystem@@UAE?AUCoord3D@@MMMM@Z
//   +0x1C slot 25 -> 0x003AC0D2 ?rva0055D3F5@Rva0055D3F5@@WBM@AEXPAVXfer@@@Z
//   +0x1C slot 26 -> 0x0055D686 ?computeCylinderEmissionPosition@@YGXPAVCoord3D@@PAVCoord2D@@MM@Z
//   +0x1C slot 28 -> 0x0055D616 ?rva0055D616@Rva003AF7FE@@UAEXXZ
//   +0x1C slot 31 -> 0x0055D6D7 ?rva0055D6D7@Rva003AF7FE@@QAEXUCoord3D@@@Z
//   +0x1C slot 34 -> 0x0055D741 ?getPosition@CylinderEmissionVolumeModule@FXParticleSystem@@UAE?AUCoord3D@2@MMMM@Z
//   +0x1C slot 43 -> 0x000D1407 ?Set_Animation@RenderObjClass@@UAEXPAVHAnimClass@@MH@Z
//   +0x1C slot 45 -> 0x00098555 ?Get_ObjectScale@RenderObjClass@@QBE?BMXZ
//   +0x1C slot 46 -> 0x0055E697 ?rva0055E697@Rva0055E697@@QAEXHHHHH@Z
//   +0x1C slot 60 -> 0x0055E81F ?getVelocity@OrthoEmissionVelocityModule@FXParticleSystem@@UAE?AUCoord3D@2@HH@Z
//   +0x1C slot 61 -> 0x003AC1EF ??_ERva003ABA36@@WBI@AEPAXI@Z
//   +0x1C slot 69 -> 0x00564AB8 ?rva00564AB8@Rva00564AB8@@QAEXPAUCoord3D@@PBX1@Z
//   +0x1C slot 74 -> 0x003AC243 ??_ERva003AD19BModule@@WBI@AEPAXI@Z
//   +0x1C slot 82 -> 0x0055EB84 ?getVelocity@CylindricalEmissionVelocityModule@FXParticleSystem@@UAE?AUCoord3D@2@HH@Z
//   +0x1C slot 91 -> 0x0055ECF9 ?getVelocity@OutwardEmissionVelocityModule@FXParticleSystem@@UAE?AUCoord3D@2@PBU32@PAVEmissionVolumeModuleInterface@2@@Z
//   +0x1C slot 92 -> 0x003A5A48 ??_ERva003ADFB2@@WM@AEPAXI@Z
//   +0x1C slot 94 -> 0x003AC930 ?Rva003AC930Get@@YAHXZ
//   +0x1C slot 96 -> 0x003ACF2D ?getClass@?$ConcreteModuleTemplate@V?$DefaultModuleTag@$00@FXParticleSystem@@@FXParticleSystem@@UBEABV?$ConcreteModuleClass@V?$DefaultModuleTag@$00@FXParticleSystem@@@2@XZ
//   +0x1C slot 98 -> 0x0055B489 ?updateAlpha@Rva0055B39C@@QAEXXZ
//   +0x1C slot 99 -> 0x003ADC32 ?clone@Rva003ADC4F@@QBEPAV1@XZ
//   +0x1C slot 101 -> 0x0055B417 ?rva0055B417@Rva0055B417@@QBE_NH@Z
//   +0x1C slot 102 -> 0x0008BB58 ?get@Rva0008BB58FloatField@@QBEMXZ
//   +0x20 slot 4 -> 0x00495DC6 ?get@Rva00495DC6PtrChaseField@@QBEHXZ
//   +0x20 slot 5 -> 0x00495DCD ?get@Rva00495DCDByteChaseField@@QBEEXZ
//   +0x20 slot 6 -> 0x00495C6E ?rva00495C6E@AutoPickUpUpdate@@UAEXXZ
//   +0x20 slot 7 -> 0x0058B826 ?getIsQuitting@FrameDataManager@@QAE_NXZ
//   +0x20 slot 8 -> 0x00495E01 ?update@AutoPickUpUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 9 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 10 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x0 slot 1 -> 0x00461274 ?loadPostProcess@DynamicPortalBehaviour@@MAEXXZ
//   +0x0 slot 2 -> 0x00460BCD ?Rva00460BCDGet@@YAHXZ
//   +0xC slot 3 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0xC slot 7 -> 0x004B4A8C ?getCreate@ReplaceSelfUpgrade@@UAEPAVCreateModuleInterface@@XZ
//   +0xC slot 45 -> 0x00461284 ?mapLoadPostProcess@DynamicPortalBehaviour@@UAEXPBVDict@@@Z
//   +0x10 slot 0 -> 0x00098CBC ?Is_Valid@RegistryClass@@QAE_NXZ
//   +0x10 slot 1 -> 0x004CE27C ?attemptUpgrade@UpgradeMux@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 2 -> 0x004CE2B0 ?wouldUpgrade@UpgradeMux@@UBE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 3 -> 0x004CE4B0 ?resetUpgrade@UpgradeModule@@UAE_NABV?$BitFlags@$0IA@@@@Z
//   +0x10 slot 5 -> 0x004CE270 ?forceRefreshUpgrade@UpgradeMux@@UAEXXZ
//   +0x10 slot 7 -> 0x00460AE2 ?vslot07@UpgradeModule@@UBE_NXZ
//   +0x10 slot 8 -> 0x00460C7D ?upgradeRemovalImplementation@DynamicPortalBehaviour@@MAEXXZ
//   +0x10 slot 9 -> 0x00452354 ?setUpgradeExecuted@UpgradeMux@@UAEX_N@Z
//   +0x10 slot 10 -> 0x0046129A ?upgradeImplementation@DynamicPortalBehaviour@@UAEXXZ
//   +0x10 slot 11 -> 0x00460AAC ?getUpgradeActivationMasks@UpgradeModule@@MBEXAAUUpgradeMaskType@@0@Z
//   +0x10 slot 12 -> 0x00460AD3 ?performUpgradeFX@UpgradeModule@@MAEXXZ
//   +0x10 slot 13 -> 0x00460A98 ?requiresAllActivationUpgrades@UpgradeModule@@MBE_NXZ
//   +0x10 slot 14 -> 0x00460AA2 ?requiresAllConflictingUpgrades@UpgradeModule@@MBE_NXZ
//   +0x10 slot 16 -> 0x004CE397 ?upgradeMuxXfer@UpgradeMux@@MAEXPAVXfer@@@Z
//   +0x10 slot 18 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x10 slot 22 -> 0x0045875F ?getDamage@UpgradeModule@@UAEPAVDamageModuleInterface@@XZ
//   +0x10 slot 28 -> 0x002533D3 ?getDie@DieModule@@UAEPAVDieModuleInterface@@XZ
//   +0x10 slot 61 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x10 slot 74 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x1C slot 0 -> 0x00460A85 ?bfmeGoEBC@BfmeThingEBC@@QAEXPAX@Z
//   +0x20 slot 1 -> 0x0046132F ?onBuildComplete@DynamicPortalBehaviour@@UAEXXZ
//   +0x20 slot 3 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
// Not established: interface tables at +0x1C, +0x20 (their vptrs sit in the gaps there).
// Not established: transfers xfer makes that this view cannot place: slot10 ?, slot4 ?, Char ?.
#pragma once
#ifndef BFME2_DYNAMIC_PORTAL_BEHAVIOUR_H
#define BFME2_DYNAMIC_PORTAL_BEHAVIOUR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpgradeModule.h"

class Thing;
class Module;
class Object;
class ModuleData;
class Xfer;

// UpgradeModule ends at 0x1C; this class's members follow it.
typedef char DynamicPortalBehaviour_base_size[(sizeof(UpgradeModule) == 0x1C) ? 1 : -1];

class DynamicPortalBehaviour : public UpgradeModule
{
public:
	DynamicPortalBehaviour(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void upgradeImplementation();	// override: upgradeImplementation slot 10 at +0x10 (0x0046129A)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual ~DynamicPortalBehaviour();
	virtual void loadPostProcess();	// override: loadPostProcess slot 1 at +0x0 (0x00461274)
	virtual void upgradeRemovalImplementation();	// override: upgradeRemovalImplementation slot 8 at +0x10 (0x00460C7D)

public:
	static Module *rva004608E0(Object *obj);	// 0x004608E0: the object's DynamicPortalBehaviour module (SiegeDeploySpecialPower::rva004C573C)
	void rva0046094C();							// 0x0046094C (SiegeDeploySpecialPower::rva004C573C tail-calls it)

	UnsignedByte m_gap0x1C[0x20];
	Bool m_unk0x3C;                                // +0x3C; xfer Bool
	Bool m_unk0x3D;                                // +0x3D; xfer Bool
};
// SIZE 0x40
typedef char DynamicPortalBehaviour_size[(sizeof(DynamicPortalBehaviour) == 0x40) ? 1 : -1];
typedef char DynamicPortalBehaviour_off_m_unk0x3C[(offsetof(DynamicPortalBehaviour, m_unk0x3C) == 0x3C) ? 1 : -1];
typedef char DynamicPortalBehaviour_off_m_unk0x3D[(offsetof(DynamicPortalBehaviour, m_unk0x3D) == 0x3D) ? 1 : -1];

#endif
