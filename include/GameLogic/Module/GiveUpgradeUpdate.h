// GiveUpgradeUpdate.h: the GiveUpgradeUpdate module. SIZE 0x90 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0049C3AC calls SpecialAbilityUpdate first; it stores the
// tables 0x00C51220 at +0x0, 0x00C4D640 at +0xC, 0x00C51214 at +0x10, 0x00C511F0 at +0x20. Members are what xfer (0x0049C41C) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x0049C3F6 ?Rva0049C3F6Get@@YAHXZ
//   +0x0 slot 13 -> 0x0049C5D9 ?onExit@Rva0049C5D9@@QAEX_N0@Z
//   +0x0 slot 14 -> 0x0049C8E9 ?rva0049C8E9@GiveUpgradeUpdate@@QAEEXZ
//   +0x0 slot 17 -> 0x0049CA2F ?triggerAbilityEffect@Rva0044E6AE@@QAEXXZ
//   +0x10 slot 0 -> 0x0049C93F ?update@Rva0049C93F@@QAEHXZ
//   +0x10 slot 8 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 12 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 13 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x10 slot 15 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0x10 slot 18 -> 0x00450D9A ?startPreparation@SpecialAbilityUpdate@@UAEXXZ
//   +0x10 slot 19 -> 0x0044FC52 ?continuePreparation@SpecialAbilityUpdate@@UAE_NXZ
//   +0x10 slot 21 -> 0x004C12CB ?vslot18@SpecialAbilityUpdate@@UAEXXZ
//   +0x10 slot 22 -> 0x00451B92 ?finishAbility@SpecialAbilityUpdate@@UAEXXZ
//   +0x10 slot 23 -> 0x0044F0B3 ?handlePackingProcessing@SpecialAbilityUpdate@@MAE_NXZ
//   +0x10 slot 24 -> 0x00450635 ?startPacking@SpecialAbilityUpdate@@UAEX_N@Z
//   +0x10 slot 25 -> 0x004508B7 ?startUnpacking@SpecialAbilityUpdate@@UAEXXZ
//   +0x20 slot 0 -> 0x0049C51A ?rva0049C51A@Rva0049C51A@@QAEXHHHHH@Z
//   +0x20 slot 8 -> 0x0049C553 ?vslot08@GiveUpgradeUpdate@@UAE_NH@Z
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_GIVE_UPGRADE_UPDATE_H
#define BFME2_GIVE_UPGRADE_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char GiveUpgradeUpdate_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class GiveUpgradeUpdate : public SpecialAbilityUpdate
{
public:
	GiveUpgradeUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual bool vslot08(int);	// override: vslot08 slot 8 at +0x20 (0x0049C553)
	virtual void initiateIntentToDoSpecialPower(const SpecialPowerTemplate *, const Object *, const Coord3D *, unsigned int, const Waypoint *);	// 0x0049C51A (SpecialPowerUpdateInterface slot 0 at +0x20)
	void releaseTarget();	// 0x0049C592
	void GetDeliveryTarget();	// 0x0049C73A
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x0049C93F, map: placeholder ?update@Rva0049C93F@@QAEHXZ)
	void beginDelivery(Object *target);	// 0x0049C67E (map: placeholder rva0049C67E): releases the old target, plays SpawnOutFX at the new one, flags it (status 3) and starts the fade
	bool rva0049C6E1(Object *target);	// 0x0049C6E1 (map: placeholder): whether the target can take the upgrade (not written yet)
	void cancelDelivery();	// 0x0049C8FC (map: placeholder ?rva0049C8FC@Rva0044E6AE@@QAEXXZ; SpecialPowerModule::initiateIntentToDoSpecialPower calls it on the sibling ability): idles the AI once a delivery target is set, then ends the ability
	virtual bool rva00450AE9();	// slot 14 override, 0x0049C8E9 (map: placeholder rva0049C8E9): finds the delivery target, runs the base, answers false

protected:
	virtual void onExit(bool, bool);	// 0x0049C5D9
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	Bool m_unk0x88;                                // +0x88; xfer Bool
	Bool m_unk0x89;                                // +0x89; xfer Bool
	Real m_unk0x8C;                                // +0x8C; xfer Real
protected:
	virtual void loadPostProcess() { SpecialAbilityUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialAbilityUpdate's (h05)
};
// SIZE 0x90
typedef char GiveUpgradeUpdate_size[(sizeof(GiveUpgradeUpdate) == 0x90) ? 1 : -1];
typedef char GiveUpgradeUpdate_off_m_unk0x88[(offsetof(GiveUpgradeUpdate, m_unk0x88) == 0x88) ? 1 : -1];
typedef char GiveUpgradeUpdate_off_m_unk0x89[(offsetof(GiveUpgradeUpdate, m_unk0x89) == 0x89) ? 1 : -1];
typedef char GiveUpgradeUpdate_off_m_unk0x8C[(offsetof(GiveUpgradeUpdate, m_unk0x8C) == 0x8C) ? 1 : -1];

#endif
