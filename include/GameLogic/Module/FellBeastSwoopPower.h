// FellBeastSwoopPower.h: the FellBeastSwoopPower module. SIZE 0x8C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C6C1E calls SpecialAbilityUpdate first; it stores the
// tables 0x00C5DF80 at +0x0, 0x00C4D640 at +0xC, 0x00C5DF74 at +0x10, 0x00C552A4 at +0x20. Members are what xfer (0x004C6CE2) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x004C6C56 ?Rva004C6C56Get@@YAHXZ
//   +0x0 slot 13 -> 0x004C6D0F ?onExit@FellBeastSwoopPower@@MAEX_N0@Z
//   +0x0 slot 17 -> 0x004C6E06 ?triggerAbilityEffect@FellBeastSwoopPower@@UAEXXZ
//   +0x10 slot 0 -> 0x004C6E49 ?update@FellBeastSwoopPower@@UAE?AW4UpdateSleepTime@@XZ
//   +0x10 slot 8 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 12 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 13 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x10 slot 15 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0x10 slot 17 -> 0x00450AE9 ?rva00450AE9@SpecialAbilityUpdate@@UAE_NXZ
//   +0x10 slot 18 -> 0x00450D9A ?startPreparation@SpecialAbilityUpdate@@UAEXXZ
//   +0x10 slot 19 -> 0x0044FC52 ?continuePreparation@SpecialAbilityUpdate@@UAE_NXZ
//   +0x10 slot 21 -> 0x004C12CB ?vslot18@SpecialAbilityUpdate@@UAEXXZ
//   +0x10 slot 22 -> 0x00451B92 ?finishAbility@SpecialAbilityUpdate@@UAEXXZ
//   +0x10 slot 23 -> 0x0044F0B3 ?handlePackingProcessing@SpecialAbilityUpdate@@MAE_NXZ
//   +0x10 slot 24 -> 0x00450635 ?startPacking@SpecialAbilityUpdate@@UAEX_N@Z
//   +0x10 slot 25 -> 0x004508B7 ?startUnpacking@SpecialAbilityUpdate@@UAEXXZ
#pragma once
#ifndef BFME2_FELL_BEAST_SWOOP_POWER_H
#define BFME2_FELL_BEAST_SWOOP_POWER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char FellBeastSwoopPower_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class FellBeastSwoopPower : public SpecialAbilityUpdate
{
public:
	FellBeastSwoopPower(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void triggerAbilityEffect();	// override: triggerAbilityEffect slot 17 at +0x0 (0x004C6E06)
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004C6E49)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void onExit(bool, bool);	// override: onExit slot 13 at +0x0 (0x004C6D0F)

public:
	Bool m_unk0x88;                                // +0x88; xfer Bool
protected:
	virtual void loadPostProcess() { SpecialAbilityUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialAbilityUpdate's (h05)
};
// SIZE 0x8C
typedef char FellBeastSwoopPower_size[(sizeof(FellBeastSwoopPower) == 0x8C) ? 1 : -1];
typedef char FellBeastSwoopPower_off_m_unk0x88[(offsetof(FellBeastSwoopPower, m_unk0x88) == 0x88) ? 1 : -1];

#endif
