// AudioLoopUpgrade.h: promoted from include/Draft by h05 (round 10). Proven by 2 rows that MATCH against it on main (0x004B7B81, 0x004B7BBB);
// name, size, bases and virtual order are retail facts given below.
// AudioLoopUpgrade.h: the AudioLoopUpgrade module. SIZE 0x30 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B7CBC calls UpdateModule first; it stores the
// tables 0x00C1C780 at +0x28, 0x00C58CC4 at +0x0, 0x00C496F8 at +0xC, 0x00C58CB8 at +0x10, 0x00C58C68 at +0x28. Members are what xfer (0x004B7E75) transfers after UpdateModule::xfer,
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
//   +0x0 slot 1 -> 0x004B7BAA ?loadPostProcess@AudioLoopUpgrade@@MAEXXZ
//   +0x0 slot 2 -> 0x004B7B81 ?name@AudioLoopUpgrade@@QBEPBDXZ
//   +0x0 slot 16 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 27 -> 0x000B69A1 ?slot14@ModuleData@@UBE?AVAsciiString@@H@Z
//   +0x0 slot 29 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 43 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0xC slot 7 -> 0x004523A9 ?getDie@BridgeBehavior@@UAEPAVDieModuleInterface@@XZ
//   +0xC slot 10 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0xC slot 48 -> 0x00482C39 ?loadPostProcess@FireWeaponWhenDeadBehavior@@UAEXXZ
//   +0xC slot 60 -> 0x004830FB ?onDamage@PoisonedBehavior@@UAEXPAVDamageInfo@@@Z
//   +0xC slot 61 -> 0x00483111 ?onHealing@PoisonedBehavior@@UAEXPAVDamageInfo@@@Z
//   +0xC slot 63 -> 0x00483133 ?update@PoisonedBehavior@@UAE?AW4UpdateSleepTime@@XZ
//   +0xC slot 64 -> 0x004DF396 ?getDisabledTypesToProcess@FireWeaponWhenDeadBehavior@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0xC slot 65 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
//   +0xC slot 75 -> 0x002533D3 ?getDie@DieModule@@UAEPAVDieModuleInterface@@XZ
//   +0xC slot 114 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x10 slot 0 -> 0x004B7B87 ?update@AudioLoopUpgrade@@QAEHXZ
//   +0x10 slot 1 -> 0x004B7ED9 ?getDisabledTypesToProcess@AudioLoopUpgrade@@QAEPAVBfmeFixedStorage002CF0F0@@PAV2@@Z
//   +0x28 slot 0 -> 0x004B7E3C ?rva004B7E3C@AudioLoopUpgrade@@UAEXH@Z
// Not established: interface tables at +0x28 (their vptrs sit in the gaps there).
// Not established: transfers xfer makes that this view cannot place: slot160 ?, ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_AUDIO_LOOP_UPGRADE_H
#define BFME2_AUDIO_LOOP_UPGRADE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char AudioLoopUpgrade_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class AudioLoopUpgrade : public UpdateModule
{
public:
	AudioLoopUpgrade(Thing *thing, const ModuleData *moduleData);
const char *name() const;											// 0x004B7B81
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x20[0x10];
};
// SIZE 0x30
typedef char AudioLoopUpgrade_size[(sizeof(AudioLoopUpgrade) == 0x30) ? 1 : -1];

#endif
