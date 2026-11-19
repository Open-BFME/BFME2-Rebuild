// HitReactionBehavior.h: the HitReactionBehavior module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00459228 calls UpdateModule first; it stores the
// tables 0x00C41004 at +0x0, 0x00BEFF90 at +0xC, 0x00C40FF8 at +0x10. Members are what xfer (0x00459173) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x0045916D ?name@HitReactionBehavior@@QBEPBDXZ
//   +0x10 slot 0 -> 0x004592A1 ?update@HitReactionBehavior@@UAE?AW4UpdateSleepTime@@XZ
#pragma once
#ifndef BFME2_HIT_REACTION_BEHAVIOR_H
#define BFME2_HIT_REACTION_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char HitReactionBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class HitReactionBehavior : public UpdateModule
{
public:
	HitReactionBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	const char *name() const;						// 0x0045916D (non-virtual, the literal)
	virtual ~HitReactionBehavior();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004592A1)

protected:
	virtual void xfer(Xfer *xfer);

public:
	Int m_unk0x20;                                 // +0x20; xfer Int
	Real m_unk0x24;                                // +0x24; xfer Real
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x28
typedef char HitReactionBehavior_size[(sizeof(HitReactionBehavior) == 0x28) ? 1 : -1];
typedef char HitReactionBehavior_off_m_unk0x20[(offsetof(HitReactionBehavior, m_unk0x20) == 0x20) ? 1 : -1];
typedef char HitReactionBehavior_off_m_unk0x24[(offsetof(HitReactionBehavior, m_unk0x24) == 0x24) ? 1 : -1];

#endif
