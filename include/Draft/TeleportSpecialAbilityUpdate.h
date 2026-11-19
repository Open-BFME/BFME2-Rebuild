// DRAFT (h05 modgen): generated from constructor RVA 0x00492C59 and xfer RVA 0x00492E10; no function
// matches against it yet, so it stays in Draft until one does.
// TeleportSpecialAbilityUpdate.h: the TeleportSpecialAbilityUpdate module. SIZE 0x88 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00492C59 calls SpecialAbilityUpdate first; it stores the
// tables 0x00C4E1A8 at +0x0, 0x00C4D640 at +0xC, 0x00C4EB30 at +0x10, 0x00C4E184 at +0x20. Members are what xfer (0x00492E10) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x00492C8A ?Rva00492C8AGet@@YAHXZ
//   +0x0 slot 13 -> 0x00494E4D ?rva00494E4D@Rva00494E4D@@QAEX_N0@Z
//   +0x0 slot 16 -> 0x00494E21 ?rva00494E21@Rva00494E21@@QAE_NXZ
//   +0x0 slot 17 -> 0x00492D11 ?triggerAbilityEffect@Rva00492C59@@UAEXXZ
//   +0x0 slot 21 -> 0x004925DB ?startPacking@WeaponFireSpecialAbilityUpdate@@UAEX_N@Z
//   +0x0 slot 22 -> 0x00492DAB ?startUnpacking@Rva00492C59@@UAEXXZ
//   +0x10 slot 0 -> 0x00494E52 ?rva00494E52@Rva00494E52@@QAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 8 -> 0x00492E79 ?vslot08@Rva00492C59@@UAE_NH@Z
//   +0x20 slot 14 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x20 slot 18 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x20 slot 19 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x20 slot 21 -> 0x0022C4CB ?getUpdatePhase@UpdateModule@@MBE?AW4SleepyUpdatePhase@@XZ
//   +0x20 slot 23 -> 0x00450AE9 ?rva00450AE9@SpecialAbilityUpdate@@UAE_NXZ
//   +0x20 slot 24 -> 0x00450D9A ?startPreparation@SpecialAbilityUpdate@@UAEXXZ
//   +0x20 slot 27 -> 0x004C12CB ?vslot18@SpecialAbilityUpdate@@UAEXXZ
//   +0x20 slot 28 -> 0x00451B92 ?finishAbility@SpecialAbilityUpdate@@UAEXXZ
//   +0x20 slot 29 -> 0x0044F0B3 ?handlePackingProcessing@SpecialAbilityUpdate@@MAE_NXZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@SpecialAbilityUpdate@@MAEXPAVXfer@@@Z ?, slotC ?.
#pragma once
#ifndef BFME2_TELEPORT_SPECIAL_ABILITY_UPDATE_H
#define BFME2_TELEPORT_SPECIAL_ABILITY_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char TeleportSpecialAbilityUpdate_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class TeleportSpecialAbilityUpdate : public SpecialAbilityUpdate
{
public:
	static Module *friend_newModuleInstance(Thing *thing, const ModuleData *moduleData);
	TeleportSpecialAbilityUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void triggerAbilityEffect();	// 0x00492D11
	virtual void startUnpacking();	// 0x00492DAB

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
};
// SIZE 0x88
typedef char TeleportSpecialAbilityUpdate_size[(sizeof(TeleportSpecialAbilityUpdate) == 0x88) ? 1 : -1];

#endif
