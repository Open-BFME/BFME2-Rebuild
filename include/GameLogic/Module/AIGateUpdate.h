// AIGateUpdate.h: the AIGateUpdate module. SIZE 0x34 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B08E8 calls UpdateModule first; it stores the
// tables 0x00C56480 at +0x0, 0x00BEFF90 at +0xC, 0x00C56474 at +0x10. Members are what xfer (0x004B095C) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x10 slot 0 -> 0x004B0C27 ?update@AIGateUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?, slot4 +0x24, Int ?.
#pragma once
#ifndef BFME2_A_I_GATE_UPDATE_H
#define BFME2_A_I_GATE_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char AIGateUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class AIGateUpdate : public UpdateModule
{
public:
	AIGateUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004B0C27)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	Int m_unk0x20;                                 // +0x20; constructor store
	Int m_unk0x24;                                 // +0x24; xfer Int
	Int m_unk0x28;                                 // +0x28; xfer Int
	Int m_unk0x2C;                                 // +0x2C; xfer Int
	Bool m_unk0x30;                                // +0x30; xfer Bool
	Bool m_unk0x31;                                // +0x31; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x34
typedef char AIGateUpdate_size[(sizeof(AIGateUpdate) == 0x34) ? 1 : -1];
typedef char AIGateUpdate_off_m_unk0x20[(offsetof(AIGateUpdate, m_unk0x20) == 0x20) ? 1 : -1];
typedef char AIGateUpdate_off_m_unk0x24[(offsetof(AIGateUpdate, m_unk0x24) == 0x24) ? 1 : -1];
typedef char AIGateUpdate_off_m_unk0x28[(offsetof(AIGateUpdate, m_unk0x28) == 0x28) ? 1 : -1];
typedef char AIGateUpdate_off_m_unk0x2C[(offsetof(AIGateUpdate, m_unk0x2C) == 0x2C) ? 1 : -1];
typedef char AIGateUpdate_off_m_unk0x30[(offsetof(AIGateUpdate, m_unk0x30) == 0x30) ? 1 : -1];
typedef char AIGateUpdate_off_m_unk0x31[(offsetof(AIGateUpdate, m_unk0x31) == 0x31) ? 1 : -1];

#endif
