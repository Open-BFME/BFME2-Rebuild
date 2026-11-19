// SpecialPowerCompletionDie.h: the SpecialPowerCompletionDie module. SIZE 0x1C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00486A74 calls DieModule first; it stores the
// tables 0x00C4AE88 at +0x0, 0x00C4A650 at +0xC, 0x00C4AE84 at +0x10. Members are what xfer (0x00486AC2) transfers after DieModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x00486A15 ?name@Rva00486A15Named@@QBEPBDXZ
//   +0x0 slot 12 -> 0x00486C00 ?onDie@UpgradeDie@@UAEXPBVDamageInfo@@@Z
//   +0x0 slot 15 -> 0x00486B6F ?name@Rva00486B6FNamed@@QBEPBDXZ
//   +0x0 slot 18 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 22 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x0 slot 23 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 25 -> 0x00486CF6 ?onDie@KeepObjectDie@@UAEXPBVDamageInfo@@@Z
//   +0x0 slot 28 -> 0x00486C8F ?name@Rva00486C8FNamed@@QBEPBDXZ
//   +0x10 slot 0 -> 0x00486B38 ?onDie@SpecialPowerCompletionDie@@UAEXPBVDamageInfo@@@Z
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_SPECIAL_POWER_COMPLETION_DIE_H
#define BFME2_SPECIAL_POWER_COMPLETION_DIE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DieModule.h"

class Thing;
class ModuleData;
class Xfer;

// DieModule ends at 0x14; this class's members follow it.
typedef char SpecialPowerCompletionDie_base_size[(sizeof(DieModule) == 0x14) ? 1 : -1];

class SpecialPowerCompletionDie : public DieModule
{
public:
	SpecialPowerCompletionDie(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual void onDie(const DamageInfo *);	// override: onDie slot 0 at +0x10 (0x00486B38)
	void setCreator(ObjectID creator);		// 0x00486A1B: records the first creator only (m_creatorSet is the set flag)
	void notifyScriptEngine();			// 0x00486B01

protected:
	virtual ~SpecialPowerCompletionDie() {}		// 0x004869FC; the map spells it MAE (protected)
	virtual void xfer(Xfer *xfer);

public:
	ObjectID m_creatorID;                          // +0x14; xfer ObjectID (Zero Hour name)
	Bool m_creatorSet;                             // +0x18; xfer Bool (Zero Hour name)
protected:
	virtual void loadPostProcess() { DieModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to DieModule's (h05)
};
// SIZE 0x1C
typedef char SpecialPowerCompletionDie_size[(sizeof(SpecialPowerCompletionDie) == 0x1C) ? 1 : -1];
typedef char SpecialPowerCompletionDie_off_m_creatorID[(offsetof(SpecialPowerCompletionDie, m_creatorID) == 0x14) ? 1 : -1];
typedef char SpecialPowerCompletionDie_off_m_creatorSet[(offsetof(SpecialPowerCompletionDie, m_creatorSet) == 0x18) ? 1 : -1];

#endif
