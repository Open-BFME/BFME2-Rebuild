// RepairDockUpdate.h: the RepairDockUpdate module. SIZE 0x90 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004A118B calls DockUpdate first; it stores the
// tables 0x00C51C80 at +0x0, 0x00C51B20 at +0xC, 0x00C51D68 at +0x10, 0x00C51C30 at +0x20. Members are what xfer (0x004A1255) transfers after DockUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004A0E47 ?loadPostProcess@MonsterDockUpdate@@MAEXXZ
//   +0x0 slot 2 -> 0x004A11CE ?Rva004A11CEGet@@YAHXZ
//   +0xC slot 22 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0xC slot 49 -> 0x004A0E06 ?Rva004A0E06Get@@YAHXZ
//   +0x20 slot 12 -> 0x004A12B6 ?action@RepairDockUpdate@@UAE_NPAVObject@@0@Z
//   +0x20 slot 19 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_REPAIR_DOCK_UPDATE_H
#define BFME2_REPAIR_DOCK_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DockUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// DockUpdate ends at 0x88; this class's members follow it.
typedef char RepairDockUpdate_base_size[(sizeof(DockUpdate) == 0x88) ? 1 : -1];

class RepairDockUpdate : public DockUpdate
{
public:
	virtual DockUpdateInterface *getDockUpdateInterface() { return this; }
	RepairDockUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual Bool action(Object *docker, Object *drone);	// override: action slot 12 at +0x20 (0x004A12B6)

	virtual Bool isRallyPointAfterDockType() { return true; }	// DockUpdateInterface slot 19: the shared `mov al,1; ret` fold 0x0050B5C6 (h05 r13)
protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	ObjectID m_unk0x88;                            // +0x88; xfer ObjectID
	Real m_unk0x8C;                                // +0x8C; xfer Real
protected:
	virtual void loadPostProcess() { DockUpdate::loadPostProcess(); }	// retail slot 0x004A0E47: a jmp to DockUpdate's, ICF-folded across classes (h05)
};
// SIZE 0x90
typedef char RepairDockUpdate_size[(sizeof(RepairDockUpdate) == 0x90) ? 1 : -1];
typedef char RepairDockUpdate_off_m_unk0x88[(offsetof(RepairDockUpdate, m_unk0x88) == 0x88) ? 1 : -1];
typedef char RepairDockUpdate_off_m_unk0x8C[(offsetof(RepairDockUpdate, m_unk0x8C) == 0x8C) ? 1 : -1];

#endif
