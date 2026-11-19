// UnleashSpecialPower.h: the UnleashSpecialPower module. SIZE 0x88 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004CE006 calls SpecialAbilityUpdate first; it stores the
// tables 0x00C5FDC0 at +0x0, 0x00C4D640 at +0xC, 0x00C4DEE8 at +0x10, 0x00C5FD98 at +0x20. Members are what xfer (0x004CE0BE) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x004CE037 ?Rva004CE037Get@@YAHXZ
//   +0x0 slot 17 -> 0x004CE18A ?triggerAbilityEffect@UnleashSpecialPower@@UAEXXZ
//   +0x0 slot 23 -> 0x0003B810 __purecall
//   +0x0 slot 30 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x20 slot 8 -> 0x004CE0D9 ?rva004CE0D9@UnleashSpecialPower@@UAE_NH@Z
#pragma once
#ifndef BFME2_UNLEASH_SPECIAL_POWER_H
#define BFME2_UNLEASH_SPECIAL_POWER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char UnleashSpecialPower_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class UnleashSpecialPower : public SpecialAbilityUpdate
{
public:
	UnleashSpecialPower(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~UnleashSpecialPower();
	virtual void triggerAbilityEffect();	// override: triggerAbilityEffect slot 17 at +0x0 (0x004CE18A)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x88
typedef char UnleashSpecialPower_size[(sizeof(UnleashSpecialPower) == 0x88) ? 1 : -1];

#endif
