// ToggleDeploySpecialAbilityUpdate.h: the ToggleDeploySpecialAbilityUpdate module. SIZE 0x88 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004AE4AB calls SpecialAbilityUpdate first; it stores the
// tables 0x00C55370 at +0x0, 0x00C4D640 at +0xC, 0x00C4DEE8 at +0x10, 0x00C55348 at +0x20. Members are what xfer (0x004CE0BE) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x004AE4DC ?Rva004AE4DCGet@@YAHXZ
//   +0x0 slot 16 -> 0x004B01E6 ?rva004B01E6@Rva004B01E6Owner@@QAE_NXZ
//   +0x0 slot 17 -> 0x004AE634 ?triggerAbilityEffect@ToggleDeploySpecialAbilityUpdate@@UAEXXZ
//   +0x0 slot 23 -> 0x004AE5B8 ?rva004AE5B8@ToggleDeploySpecialAbilityUpdate@@QAEXPAX@Z
//   +0x0 slot 24 -> 0x004AE6D7 ?rva004AE6D7@ToggleDeploySpecialAbilityUpdate@@QAEXPAX@Z
//   +0x20 slot 8 -> 0x004AE583 ?vslot08@ToggleDeploySpecialAbilityUpdate@@UAE_NH@Z
#pragma once
#ifndef BFME2_TOGGLE_DEPLOY_SPECIAL_ABILITY_UPDATE_H
#define BFME2_TOGGLE_DEPLOY_SPECIAL_ABILITY_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char ToggleDeploySpecialAbilityUpdate_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class ToggleDeploySpecialAbilityUpdate : public SpecialAbilityUpdate
{
public:
	ToggleDeploySpecialAbilityUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void triggerAbilityEffect();	// override: triggerAbilityEffect slot 17 at +0x0 (0x004AE634)
	virtual bool vslot08(int);	// override: vslot08 slot 8 at +0x20 (0x004AE583)
	virtual void turnOff(Object *object);	// slot 23, 0x004AE5B8 (the same slot ToggleHiddenSpecialAbilityUpdate calls turnOff)
	virtual void turnOn(Object *object);	// slot 24, 0x004AE6D7

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { SpecialAbilityUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialAbilityUpdate's (h05)
};
// SIZE 0x88
typedef char ToggleDeploySpecialAbilityUpdate_size[(sizeof(ToggleDeploySpecialAbilityUpdate) == 0x88) ? 1 : -1];

#endif
