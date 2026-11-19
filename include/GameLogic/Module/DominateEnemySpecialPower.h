// DominateEnemySpecialPower.h: the DominateEnemySpecialPower module. SIZE 0x88 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004CC988 calls SpecialAbilityUpdate first; it stores the
// tables 0x00C5F698 at +0x0, 0x00C4D640 at +0xC, 0x00C4DEE8 at +0x10, 0x00C552A4 at +0x20. Members are what xfer (0x004CE0BE) transfers after SpecialAbilityUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x004CC9B9 ?Rva004CC9B9Get@@YAHXZ
//   +0x0 slot 17 -> 0x004CCBDF ?triggerAbilityEffect@DominateEnemySpecialPower@@UAEXXZ
#pragma once
#ifndef BFME2_DOMINATE_ENEMY_SPECIAL_POWER_H
#define BFME2_DOMINATE_ENEMY_SPECIAL_POWER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/SpecialAbilityUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// SpecialAbilityUpdate ends at 0x88; this class's members follow it.
typedef char DominateEnemySpecialPower_base_size[(sizeof(SpecialAbilityUpdate) == 0x88) ? 1 : -1];

class DominateEnemySpecialPower : public SpecialAbilityUpdate
{
public:
	DominateEnemySpecialPower(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void triggerAbilityEffect();	// override: triggerAbilityEffect slot 17 at +0x0 (0x004CCBDF)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { SpecialAbilityUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to SpecialAbilityUpdate's (h05)
};
// SIZE 0x88
typedef char DominateEnemySpecialPower_size[(sizeof(DominateEnemySpecialPower) == 0x88) ? 1 : -1];

#endif
