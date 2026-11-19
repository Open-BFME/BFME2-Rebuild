// SupplyCenterDockUpdate.h: the SupplyCenterDockUpdate module. SIZE 0x88 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004A0DC7 calls DockUpdate first; it stores the
// tables 0x00C51BDC at +0x0, 0x00C51B20 at +0xC, 0x00C51B10 at +0x10, 0x00C51AC0 at +0x20. Members are what xfer (0x004A0E2C) transfers after DockUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004A0E47 ?loadPostProcess@MonsterDockUpdate@@MAEXXZ
//   +0x0 slot 2 -> 0x004A0E06 ?Rva004A0E06Get@@YAHXZ
//   +0xC slot 22 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0x10 slot 0 -> 0x004A1119 ?update@SupplyCenterDockUpdate@@UAE?AW4UpdateSleepTime@@XZ
//   +0x20 slot 12 -> 0x004A0EAD ?action@SupplyCenterDockUpdate@@UAE_NPAVObject@@0@Z
//   +0x20 slot 21 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 22 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
#pragma once
#ifndef BFME2_SUPPLY_CENTER_DOCK_UPDATE_H
#define BFME2_SUPPLY_CENTER_DOCK_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DockUpdate.h"
#include "GameLogic/Module/SupplyCenterDockUpdateModuleData.h"

class Thing;
class ModuleData;
class Xfer;

// DockUpdate ends at 0x88; this class's members follow it.
typedef char SupplyCenterDockUpdate_base_size[(sizeof(DockUpdate) == 0x88) ? 1 : -1];

class SupplyCenterDockUpdate : public DockUpdate
{
public:
	SupplyCenterDockUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	// Slot 12 of the +0x20 dock table per the header comment above. Its bytes are open: the
	// interface slot is action(Object *, Object *).
	virtual Bool action(Object *owner, Object *drone);					// 0x004A0EAD
	virtual UpdateSleepTime update();									// 0x004A1119

	const SupplyCenterDockUpdateModuleData *getSupplyCenterDockUpdateModuleData() const
	{
		return (const SupplyCenterDockUpdateModuleData *)m_moduleData;
	}

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
};
// SIZE 0x88
typedef char SupplyCenterDockUpdate_size[(sizeof(SupplyCenterDockUpdate) == 0x88) ? 1 : -1];

#endif
