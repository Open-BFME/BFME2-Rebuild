// DRAFT (h05 modgen): generated from constructor RVA 0x004AE16E and xfer RVA 0x004AE25C; no function
// matches against it yet, so it stays in Draft until one does.
// ToggleHiddenSpecialAbilityUpdate.h: the ToggleHiddenSpecialAbilityUpdate module. SIZE 0x8C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004AE16E calls SpecialAbilityUpdate first; it stores the
// tables 0x00C552D8 at +0x0, 0x00C4D640 at +0xC, 0x00C552C8 at +0x10, 0x00C552A4 at +0x20. Members are what xfer (0x004AE25C) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x004AE1A6 ?Rva004AE1A6Get@@YAHXZ
//   +0x0 slot 16 -> 0x004B01E6 ?rva004B01E6@Rva004B01E6Owner@@QAE_NXZ
//   +0x0 slot 17 -> 0x004AE22D ?triggerAbilityEffect@ToggleHiddenSpecialAbilityUpdate@@UAEXXZ
//   +0x0 slot 23 -> 0x004AE2D0 ?turnOff@ToggleHiddenSpecialAbilityUpdate@@UAEXXZ
//   +0x0 slot 24 -> 0x004AE394 ?turnOn@ToggleHiddenSpecialAbilityUpdate@@UAEXXZ
//   +0x0 slot 25 -> 0x0021914D ?get@Rva0021914DDwordField@@QBEHXZ
//   +0x10 slot 0 -> 0x004AE286 ?update@ToggleHiddenSpecialAbilityUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@SpecialAbilityUpdate@@MAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_TOGGLE_HIDDEN_SPECIAL_ABILITY_UPDATE_H
#define BFME2_TOGGLE_HIDDEN_SPECIAL_ABILITY_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char ToggleHiddenSpecialAbilityUpdate_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class ToggleHiddenSpecialAbilityUpdate : public SpecialAbilityUpdate
{
public:
	ToggleHiddenSpecialAbilityUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void triggerAbilityEffect();	// override: triggerAbilityEffect slot 17 at +0x0 (0x004AE22D)
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004AE286)
	virtual void turnOff();	// slot 23 (0x004AE2D0); StealthUpdate rva00373D5E 0x00373D5E calls it through [vtable+0x5C] (f10)
	virtual void turnOn();	// slot 24 (0x004AE394)
	virtual Int rva0021914D() const;	// slot 25 (0x0021914D, map: ?get@Rva0021914DDwordField@@QBEHXZ); StealthUpdate 0x00373F59 compares the body's last damage frame with it (f10)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedInt m_unk0x88;                         // +0x88; xfer UnsignedInt
protected:
	virtual void loadPostProcess() { SpecialAbilityUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialAbilityUpdate's (h05)
};
// SIZE 0x8C
typedef char ToggleHiddenSpecialAbilityUpdate_size[(sizeof(ToggleHiddenSpecialAbilityUpdate) == 0x8C) ? 1 : -1];
typedef char ToggleHiddenSpecialAbilityUpdate_off_m_unk0x88[(offsetof(ToggleHiddenSpecialAbilityUpdate, m_unk0x88) == 0x88) ? 1 : -1];

#endif
