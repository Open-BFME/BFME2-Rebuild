// EntEnragedUpdate.h: the EntEnragedUpdate module. SIZE 0x34 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B2516 calls UpdateModule first; it stores the
// tables 0x00C56948 at +0x0, 0x00BEFF90 at +0xC, 0x00C5693C at +0x10. Members are what xfer (0x004B23C5) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004B2394 ?name@Rva004B2394Named@@QBEPBDXZ
//   +0x0 slot 3 -> 0x004B23C5 ?rva004B23C5@EntEnragedUpdate@@UAEXPAVXfer@@@Z
//   +0x10 slot 0 -> 0x004B2761 ?update@EntEnragedUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_ENT_ENRAGED_UPDATE_H
#define BFME2_ENT_ENRAGED_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char EntEnragedUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class EntEnragedUpdate : public UpdateModule
{
public:
	EntEnragedUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004B2761)

public:
	UnsignedInt m_unk0x20;                         // +0x20; xfer UnsignedInt
	UnsignedInt m_unk0x24;                         // +0x24; xfer UnsignedInt
	UnsignedInt m_unk0x28;                         // +0x28; xfer UnsignedInt
	Bool m_unk0x2C;                                // +0x2C; xfer Bool
	Bool m_unk0x2D;                                // +0x2D; xfer Bool
	Bool m_unk0x2E;                                // +0x2E; xfer Bool
	Bool m_unk0x2F;                                // +0x2F; xfer Bool
	Bool m_unk0x30;                                // +0x30; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x34
typedef char EntEnragedUpdate_size[(sizeof(EntEnragedUpdate) == 0x34) ? 1 : -1];
typedef char EntEnragedUpdate_off_m_unk0x20[(offsetof(EntEnragedUpdate, m_unk0x20) == 0x20) ? 1 : -1];
typedef char EntEnragedUpdate_off_m_unk0x24[(offsetof(EntEnragedUpdate, m_unk0x24) == 0x24) ? 1 : -1];
typedef char EntEnragedUpdate_off_m_unk0x28[(offsetof(EntEnragedUpdate, m_unk0x28) == 0x28) ? 1 : -1];
typedef char EntEnragedUpdate_off_m_unk0x2C[(offsetof(EntEnragedUpdate, m_unk0x2C) == 0x2C) ? 1 : -1];
typedef char EntEnragedUpdate_off_m_unk0x2D[(offsetof(EntEnragedUpdate, m_unk0x2D) == 0x2D) ? 1 : -1];
typedef char EntEnragedUpdate_off_m_unk0x2E[(offsetof(EntEnragedUpdate, m_unk0x2E) == 0x2E) ? 1 : -1];
typedef char EntEnragedUpdate_off_m_unk0x2F[(offsetof(EntEnragedUpdate, m_unk0x2F) == 0x2F) ? 1 : -1];
typedef char EntEnragedUpdate_off_m_unk0x30[(offsetof(EntEnragedUpdate, m_unk0x30) == 0x30) ? 1 : -1];

#endif
