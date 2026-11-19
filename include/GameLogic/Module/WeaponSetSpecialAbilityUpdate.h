// WeaponSetSpecialAbilityUpdate.h: the WeaponSetSpecialAbilityUpdate module. SIZE 0x88 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00492402 calls SpecialAbilityUpdate first; it stores the
// tables 0x00C4DFF8 at +0x0, 0x00C4D640 at +0xC, 0x00C4DEE8 at +0x10, 0x00C4DFD4 at +0x20. Members are what xfer (0x004CE0BE) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x00492433 ?Rva00492433Get@@YAHXZ
//   +0x0 slot 17 -> 0x004924BA ?triggerAbilityEffect@Rva00492402@@UAEXXZ
//   +0x0 slot 18 -> 0x0049251D ?rva0049251D@Rva00492402@@UAEXXZ
//   +0x0 slot 22 -> 0x004924D9 ?startUnpacking@Rva00492402@@UAEXXZ
//   +0x20 slot 8 -> 0x00492543 ?bfmeGoFDH@BfmeThingFDH@@QAEDPAX@Z
//   +0x20 slot 14 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x20 slot 18 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x20 slot 19 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x20 slot 21 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0x20 slot 22 -> 0x004502CE ?onExit@SpecialAbilityUpdate@@MAEX_N0@Z
//   +0x20 slot 23 -> 0x00450AE9 ?rva00450AE9@SpecialAbilityUpdate@@UAE_NXZ
//   +0x20 slot 24 -> 0x00450D9A ?startPreparation@SpecialAbilityUpdate@@UAEXXZ
//   +0x20 slot 25 -> 0x0044FC52 ?continuePreparation@SpecialAbilityUpdate@@UAE_NXZ
//   +0x20 slot 28 -> 0x00451B92 ?finishAbility@SpecialAbilityUpdate@@UAEXXZ
//   +0x20 slot 29 -> 0x0044F0B3 ?handlePackingProcessing@SpecialAbilityUpdate@@MAE_NXZ
//   +0x20 slot 30 -> 0x00450635 ?startPacking@SpecialAbilityUpdate@@UAEX_N@Z
#pragma once
#ifndef BFME2_WEAPON_SET_SPECIAL_ABILITY_UPDATE_H
#define BFME2_WEAPON_SET_SPECIAL_ABILITY_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char WeaponSetSpecialAbilityUpdate_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class WeaponSetSpecialAbilityUpdate : public SpecialAbilityUpdate
{
public:
	static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
	WeaponSetSpecialAbilityUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void triggerAbilityEffect();	// 0x004924BA
	virtual void startUnpacking();	// 0x004924D9
	virtual void vslot18();	// 0x0049251D: the effect ends

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
};
// SIZE 0x88
typedef char WeaponSetSpecialAbilityUpdate_size[(sizeof(WeaponSetSpecialAbilityUpdate) == 0x88) ? 1 : -1];

#endif
