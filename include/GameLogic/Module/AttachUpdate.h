// AttachUpdate.h: the AttachUpdate module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00491A0F calls UpdateModule first; it stores the
// tables 0x00C4DB20 at +0x0, 0x00BEFF90 at +0xC, 0x00C4DB14 at +0x10. Members are what xfer (0x004918AC) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004918A6 ?name@Rva004918A6Named@@QBEPBDXZ
//   +0x10 slot 0 -> 0x00491D2F ?update@AttachUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?, Bool ?.
#pragma once
#ifndef BFME2_ATTACH_UPDATE_H
#define BFME2_ATTACH_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char AttachUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class AttachUpdate : public UpdateModule
{
public:
	AttachUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x00491D2F)

protected:
	virtual void xfer(Xfer *xfer);

public:
	ObjectID m_unk0x20;                            // +0x20; xfer ObjectID
	UnsignedByte m_gap0x24[0x4];
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x28
typedef char AttachUpdate_size[(sizeof(AttachUpdate) == 0x28) ? 1 : -1];
typedef char AttachUpdate_off_m_unk0x20[(offsetof(AttachUpdate, m_unk0x20) == 0x20) ? 1 : -1];

#endif
