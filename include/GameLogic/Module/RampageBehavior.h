// RampageBehavior.h: the RampageBehavior module. SIZE 0x2C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00458B60 calls UpdateModule first; it stores the
// tables 0x00C40DA8 at +0x0, 0x00BEFF90 at +0xC, 0x00C40D9C at +0x10. Members are what xfer (0x00458ACD) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x00458AC7 ?name@RampageBehavior@@QBEPBDXZ
//   +0x10 slot 0 -> 0x00458CCB ?update@RampageBehavior@@UAE?AW4UpdateSleepTime@@XZ
#pragma once
#ifndef BFME2_RAMPAGE_BEHAVIOR_H
#define BFME2_RAMPAGE_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char RampageBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class RampageBehavior : public UpdateModule
{
public:
	RampageBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	const char *name() const;						// 0x00458AC7 (non-virtual, the literal)
	virtual ~RampageBehavior();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x00458CCB)

protected:
	virtual void xfer(Xfer *xfer);

public:
	Real m_unk0x20;                                // +0x20; xfer Real
	Int m_unk0x24;                                 // +0x24; xfer Int
	Int m_unk0x28;                                 // +0x28; xfer Int
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x2C
typedef char RampageBehavior_size[(sizeof(RampageBehavior) == 0x2C) ? 1 : -1];
typedef char RampageBehavior_off_m_unk0x20[(offsetof(RampageBehavior, m_unk0x20) == 0x20) ? 1 : -1];
typedef char RampageBehavior_off_m_unk0x24[(offsetof(RampageBehavior, m_unk0x24) == 0x24) ? 1 : -1];
typedef char RampageBehavior_off_m_unk0x28[(offsetof(RampageBehavior, m_unk0x28) == 0x28) ? 1 : -1];

#endif
