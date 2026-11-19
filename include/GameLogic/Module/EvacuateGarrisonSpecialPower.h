// EvacuateGarrisonSpecialPower.h: the EvacuateGarrisonSpecialPower module. SIZE 0x88 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004CDBFC calls SpecialAbilityUpdate first; it stores the
// tables 0x00C5FC28 at +0x0, 0x00C4D640 at +0xC, 0x00C4DEE8 at +0x10, 0x00C552A4 at +0x20. Members are what xfer (0x004CE0BE) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x004CDC2D ?Rva004CDC2DGet@@YAHXZ
//   +0x0 slot 19 -> 0x004CDCB4 ?finishAbility@EvacuateGarrisonSpecialPower@@UAEXXZ
#pragma once
#ifndef BFME2_EVACUATE_GARRISON_SPECIAL_POWER_H
#define BFME2_EVACUATE_GARRISON_SPECIAL_POWER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char EvacuateGarrisonSpecialPower_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class EvacuateGarrisonSpecialPower : public SpecialAbilityUpdate
{
public:
	EvacuateGarrisonSpecialPower(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~EvacuateGarrisonSpecialPower();
	virtual void finishAbility();	// override: finishAbility slot 19 at +0x0 (0x004CDCB4)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x88
typedef char EvacuateGarrisonSpecialPower_size[(sizeof(EvacuateGarrisonSpecialPower) == 0x88) ? 1 : -1];

#endif
