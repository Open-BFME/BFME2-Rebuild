// TemporarilyDefectUpdate.h: the TemporarilyDefectUpdate module. SIZE 0x30 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004CC6AF calls UpdateModule first; it stores the
// tables 0x00C5F5A0 at +0x0, 0x00BEFF90 at +0xC, 0x00C5F594 at +0x10. Members are what xfer (0x004CC72C) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004CC653 ?name@Rva004CC653Named@@QBEPBDXZ
//   +0x10 slot 0 -> 0x004CC882 ?update@TemporarilyDefectUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x10 slot 1 -> 0x004A2D67 ?getDisabledTypesToProcess@AIUpdateInterface@@UBE?AV?$BitFlags@$0L@@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?, slot4 +0x24.
#pragma once
#ifndef BFME2_TEMPORARILY_DEFECT_UPDATE_H
#define BFME2_TEMPORARILY_DEFECT_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char TemporarilyDefectUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class TemporarilyDefectUpdate : public UpdateModule
{
public:
	TemporarilyDefectUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004CC882)

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedInt m_unk0x20;                         // +0x20; xfer UnsignedInt
	UnsignedInt m_unk0x24;                         // +0x24; xfer UnsignedInt
	ObjectID m_unk0x28;                            // +0x28; xfer ObjectID
	UnsignedByte m_gap0x2C[0x4];
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x30
typedef char TemporarilyDefectUpdate_size[(sizeof(TemporarilyDefectUpdate) == 0x30) ? 1 : -1];
typedef char TemporarilyDefectUpdate_off_m_unk0x20[(offsetof(TemporarilyDefectUpdate, m_unk0x20) == 0x20) ? 1 : -1];
typedef char TemporarilyDefectUpdate_off_m_unk0x24[(offsetof(TemporarilyDefectUpdate, m_unk0x24) == 0x24) ? 1 : -1];
typedef char TemporarilyDefectUpdate_off_m_unk0x28[(offsetof(TemporarilyDefectUpdate, m_unk0x28) == 0x28) ? 1 : -1];

#endif
