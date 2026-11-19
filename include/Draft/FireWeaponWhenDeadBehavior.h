// DRAFT (h05 modgen): generated from constructor RVA 0x00482D80 and xfer RVA 0x00482BDB; no function
// matches against it yet, so it stays in Draft until one does.
// FireWeaponWhenDeadBehavior.h: the FireWeaponWhenDeadBehavior module. SIZE 0xAC (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00482D80 calls UpdateModule first; it stores the
// tables 0x00C1C780 at +0x28, 0x00C497B4 at +0x0, 0x00C496F8 at +0xC, 0x00C496E8 at +0x10, 0x00C49698 at +0x28. Members are what xfer (0x00482BDB) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x28 slot 0 -> 0x0003B810 __purecall
//   +0x28 slot 2 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x28 slot 5 -> 0x0055CFF0 ?rva0055CFF0@Rva0055CFF0@@QAEXMMM@Z
//   +0x28 slot 6 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x28 slot 7 -> 0x003ABEFD ?rva003ABEFD@Rva003ABEFD@@QAEMXZ
//   +0x28 slot 8 -> 0x0055D0D5 ?rva0055D0D5@Rva0055D0D5@@QAE?AUCoord3D0055D0D5@@IIII@Z
//   +0x28 slot 9 -> 0x003ABFD9 ??_ERva003ABC58@@WBM@AEPAXI@Z
//   +0x28 slot 13 -> 0x0055E636 ?Rva0055E636Compute@@YGXPAVCoord3D@@0MM@Z
//   +0x28 slot 17 -> 0x0055D3F5 ?rva0055D3F5@Rva0055D3F5@@UAEXPAVXfer@@@Z
//   +0x28 slot 18 -> 0x0055D422 ?debugDraw@Rva0055D3F5@@QAEXUCoord3D@@@Z
//   +0x28 slot 21 -> 0x0055D48C ?getPosition@SphereEmissionVolumeModule@FXParticleSystem@@UAE?AUCoord3D@@MMMM@Z
//   +0x28 slot 25 -> 0x003AC0D2 ?rva0055D3F5@Rva0055D3F5@@WBM@AEXPAVXfer@@@Z
//   +0x28 slot 26 -> 0x0055D686 ?computeCylinderEmissionPosition@@YGXPAVCoord3D@@PAVCoord2D@@MM@Z
//   +0x28 slot 28 -> 0x0055D616 ?rva0055D616@Rva003AF7FE@@UAEXXZ
//   +0x28 slot 31 -> 0x0055D6D7 ?rva0055D6D7@Rva003AF7FE@@QAEXUCoord3D@@@Z
//   +0x28 slot 34 -> 0x0055D741 ?getPosition@CylinderEmissionVolumeModule@FXParticleSystem@@UAE?AUCoord3D@2@MMMM@Z
//   +0x28 slot 43 -> 0x000D1407 ?Set_Animation@RenderObjClass@@UAEXPAVHAnimClass@@MH@Z
//   +0x28 slot 45 -> 0x00098555 ?Get_ObjectScale@RenderObjClass@@QBE?BMXZ
//   +0x28 slot 46 -> 0x0055E697 ?rva0055E697@Rva0055E697@@QAEXHHHHH@Z
//   +0x28 slot 60 -> 0x0055E81F ?getVelocity@OrthoEmissionVelocityModule@FXParticleSystem@@UAE?AUCoord3D@2@HH@Z
//   +0x28 slot 61 -> 0x003AC1EF ??_ERva003ABA36@@WBI@AEPAXI@Z
//   +0x28 slot 69 -> 0x00564AB8 ?rva00564AB8@Rva00564AB8@@QAEXPAUCoord3D@@PBX1@Z
//   +0x28 slot 74 -> 0x003AC243 ??_ERva003AD19BModule@@WBI@AEPAXI@Z
//   +0x28 slot 82 -> 0x0055EB84 ?getVelocity@CylindricalEmissionVelocityModule@FXParticleSystem@@UAE?AUCoord3D@2@HH@Z
//   +0x28 slot 91 -> 0x0055ECF9 ?getVelocity@OutwardEmissionVelocityModule@FXParticleSystem@@UAE?AUCoord3D@2@PBU32@PAVEmissionVolumeModuleInterface@2@@Z
//   +0x28 slot 92 -> 0x003A5A48 ??_ERva003ADFB2@@WM@AEPAXI@Z
//   +0x28 slot 94 -> 0x003AC930 ?Rva003AC930Get@@YAHXZ
//   +0x28 slot 96 -> 0x003ACF2D ?getClass@?$ConcreteModuleTemplate@V?$DefaultModuleTag@$00@FXParticleSystem@@@FXParticleSystem@@UBEABV?$ConcreteModuleClass@V?$DefaultModuleTag@$00@FXParticleSystem@@@2@XZ
//   +0x28 slot 98 -> 0x0055B489 ?updateAlpha@Rva0055B39C@@QAEXXZ
//   +0x28 slot 99 -> 0x003ADC32 ?clone@Rva003ADC4F@@QBEPAV1@XZ
//   +0x28 slot 101 -> 0x0055B417 ?rva0055B417@Rva0055B417@@QBE_NH@Z
//   +0x28 slot 102 -> 0x0008BB58 ?get@Rva0008BB58FloatField@@QBEMXZ
//   +0x0 slot 1 -> 0x00482C39 ?loadPostProcess@FireWeaponWhenDeadBehavior@@UAEXXZ
//   +0x0 slot 13 -> 0x004830FB ?onDamage@PoisonedBehavior@@UAEXPAVDamageInfo@@@Z
//   +0x0 slot 14 -> 0x00483111 ?onHealing@PoisonedBehavior@@UAEXPAVDamageInfo@@@Z
//   +0x0 slot 16 -> 0x00483133 ?update@PoisonedBehavior@@UAE?AW4UpdateSleepTime@@XZ
//   +0x0 slot 17 -> 0x004DF396 ?getDisabledTypesToProcess@FireWeaponWhenDeadBehavior@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x0 slot 18 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0x0 slot 19 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 23 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0x0 slot 28 -> 0x002533D3 ?getDie@DieModule@@UAEPAVDieModuleInterface@@XZ
//   +0x0 slot 62 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 67 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 75 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x0 slot 78 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0xC slot 7 -> 0x004523A9 ?getDie@BridgeBehavior@@UAEPAVDieModuleInterface@@XZ
//   +0x10 slot 0 -> 0x00482BB9 ?update@FireWeaponWhenDeadBehavior@@UAE?AW4UpdateSleepTime@@XZ
//   +0x28 slot 0 -> 0x00482C4A ?onDie@FireWeaponWhenDeadBehavior@@UAEXPBVDamageInfo@@@Z
// Not established: interface tables at +0x28 (their vptrs sit in the gaps there).
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?, ?xfer@BehaviorModule@@MAEXPAVXfer@@@Z ?.
// Round 7 (h05, by hand): the constructor 0x00482D80 also builds an UpgradeMux at +0x20 (`lea edi,[esi+0x20]`,
// ??0UpgradeMux, table 0x00C496A0 stored through edi) and stores DieModuleInterface's own table 0x00C1C780
// then this class's 0x00C49698 at +0x28: UpdateModule + UpgradeMux + DieModuleInterface. The destructor
// 0x00482E96 restores all five tables and tail-jumps to ~UpdateModule. +0x2C holds a member with its own
// table (constructor 0x00263895, xfer through its slot 0) that is not established here.
#pragma once
#ifndef BFME2_FIRE_WEAPON_WHEN_DEAD_BEHAVIOR_H
#define BFME2_FIRE_WEAPON_WHEN_DEAD_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "Draft/FireWeaponWhenDeadBehaviorModuleData.h"
#include "GameLogic/Module/DieModuleInterface.h"
#include "GameLogic/Damage.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char FireWeaponWhenDeadBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class FireWeaponWhenDeadBehavior : public UpdateModule, public UpgradeMux, public DieModuleInterface
{
public:
	virtual DieModuleInterface *getDie() { return this; }
	virtual UpgradeModuleInterface *getUpgrade() { return this; }
	FireWeaponWhenDeadBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual ~FireWeaponWhenDeadBehavior();
	virtual void loadPostProcess();	// override: loadPostProcess slot 1 at +0x0 (0x00482C39)
	virtual DisabledMaskType getDisabledTypesToProcess() const;	// override: getDisabledTypesToProcess slot 64 at +0xC (0x004DF396)
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x00482BB9)
	virtual void onDie(const DamageInfo *damageInfo);	// DieModuleInterface slot 0 at +0x28 (0x00482C4A)

protected:
	virtual void xfer(Xfer *xfer);

	// UpgradeMux (table 0x00C496A0 at +0x20)
	virtual void upgradeRemovalImplementation();
	virtual void upgradeImplementation();
	virtual void getUpgradeActivationMasks(UpgradeMaskType &activation, UpgradeMaskType &conflicting) const;
	virtual void performUpgradeFX();
	virtual Bool requiresAllActivationUpgrades() const;
	virtual Bool requiresAllConflictingUpgrades() const;
	virtual void processUpgradeRemoval() { ((const FireWeaponWhenDeadBehaviorModuleData *)getModuleData())->m_upgradeMuxData.muxDataProcessUpgradeRemoval(getObject()); }	// slot 15: retail 0x00452487, ICF-folded with performUpgradeFX (both call the mux data's `ret 4` 0x0047A69C)

public:
	DamageInfo m_deathDamage;									// +0x2C: built by the constructor (0x00263895), transferred through its slot 0 by xfer 0x00482BDB
	Int m_delayFrames;                             // +0xA8; xfer Int, the constructor sets it from the module data's DelayTime
};
// SIZE 0xAC
typedef char FireWeaponWhenDeadBehavior_size[(sizeof(FireWeaponWhenDeadBehavior) == 0xAC) ? 1 : -1];
typedef char FireWeaponWhenDeadBehavior_off_m_delayFrames[(offsetof(FireWeaponWhenDeadBehavior, m_delayFrames) == 0xA8) ? 1 : -1];

#endif
