// FloatUpdate.h: the FloatUpdate module. SIZE 0x24 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0048D7C4 calls UpdateModule first; it stores the
// tables 0x00C4C770 at +0x0, 0x00BEFF90 at +0xC, 0x00C4C764 at +0x10. Members are what xfer (0x0048DBC4) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x10 slot 0 -> 0x0048D813 ?update@FloatUpdate@@UAE?AW4UpdateSleepTime@@XZ
#pragma once
#ifndef BFME2_FLOAT_UPDATE_H
#define BFME2_FLOAT_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char FloatUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class FloatUpdate : public UpdateModule
{
public:
	FloatUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x0048D813)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	Bool m_unk0x20;                                // +0x20; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x24
typedef char FloatUpdate_size[(sizeof(FloatUpdate) == 0x24) ? 1 : -1];
typedef char FloatUpdate_off_m_unk0x20[(offsetof(FloatUpdate, m_unk0x20) == 0x20) ? 1 : -1];

#endif
