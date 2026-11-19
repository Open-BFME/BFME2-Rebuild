// StancesBehavior.h: the StancesBehavior module. SIZE 0x34 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0045EF80 calls UpdateModule first; it stores the
// tables 0x00C424DC at +0x0, 0x00BEFF90 at +0xC, 0x00C424D0 at +0x10. Members are what xfer (0x0045ED9D) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x10 slot 0 -> 0x0045F290 ?update@StancesBehavior@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_STANCES_BEHAVIOR_H
#define BFME2_STANCES_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char StancesBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class StancesBehavior : public UpdateModule
{
public:
	StancesBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x0045F290)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	void rva0045F235();	// 0x0045F235: AIGoingIdleState::onEnter 0x00341E48 calls it on the owner's module
	Bool changeStance(Int stance);	// 0x0045F084 (the map's ?changeStance@StancesBehavior@@QAE_NH@Z; AIGroup::groupChangeStance 0x0036E269 calls it)
	Int rva0045ED4B() const;	// 0x0045ED4B: the stance (the member at +0x30); ScriptConditions 0x003E5267 calls it out of line
	UnsignedByte m_gap0x20[0x10];
	Int m_unk0x30;                                 // +0x30; constructor store
};
// SIZE 0x34
typedef char StancesBehavior_size[(sizeof(StancesBehavior) == 0x34) ? 1 : -1];
typedef char StancesBehavior_off_m_unk0x30[(offsetof(StancesBehavior, m_unk0x30) == 0x30) ? 1 : -1];

#endif
