// ActivateModuleSpecialPower.h: the ActivateModuleSpecialPower module. SIZE 0x88 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004CDD02 calls SpecialAbilityUpdate first; it stores the
// tables 0x00C5FC88 at +0x0, 0x00C4D640 at +0xC, 0x00C4DEE8 at +0x10, 0x00C552A4 at +0x20. Members are what xfer (0x004CE0BE) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x004CDD33 ?Rva004CDD33Get@@YAHXZ
//   +0x0 slot 5 -> 0x004CDF1D ?onObjectCreated@ActivateModuleSpecialPower@@QAEXXZ
//   +0x0 slot 13 -> 0x004CDF00 ?onExit@ActivateModuleSpecialPower@@QAEX_N0@Z
//   +0x0 slot 17 -> 0x004CDEED ?triggerAbilityEffect@ActivateModuleSpecialPower@@UAEXXZ
#pragma once
#ifndef BFME2_ACTIVATE_MODULE_SPECIAL_POWER_H
#define BFME2_ACTIVATE_MODULE_SPECIAL_POWER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char ActivateModuleSpecialPower_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class ActivateModuleSpecialPower : public SpecialAbilityUpdate
{
public:
	ActivateModuleSpecialPower(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~ActivateModuleSpecialPower();

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	virtual void onObjectCreated();					// 0x004CDF1D, primary table 0x0085FC88 slot 5: wakes the module when it has an object
	virtual void onExit(bool a, bool b);				// 0x004CDF00, slot 13 (public in the map: UAE): the base's exit, then wakes the module
	virtual void triggerAbilityEffect();			// 0x004CDEED, slot 17
	void SetWakeState(int state);				// 0x004CDDBA
protected:
	virtual void loadPostProcess() { SpecialAbilityUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF (table 0x0085FC88): a jmp to SpecialAbilityUpdate's 0x00497805 (h05 r13)
};
// SIZE 0x88
typedef char ActivateModuleSpecialPower_size[(sizeof(ActivateModuleSpecialPower) == 0x88) ? 1 : -1];

#endif
