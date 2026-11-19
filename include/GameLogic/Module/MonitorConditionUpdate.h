// MonitorConditionUpdate.h: the MonitorConditionUpdate module. SIZE 0x24 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00491633 calls UpdateModule first; it stores the
// tables 0x00C4DAD8 at +0x0, 0x00BEFF90 at +0xC, 0x00C4DACC at +0x10. Members are what xfer (0x004915C6) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x00491661 ?Rva00491661Get@@YAHXZ
//   +0x10 slot 0 -> 0x0049176C ?update@MonitorConditionUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_MONITOR_CONDITION_UPDATE_H
#define BFME2_MONITOR_CONDITION_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "Common/AsciiString.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char MonitorConditionUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class MonitorConditionUpdate : public UpdateModule
{
public:
	MonitorConditionUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void xfer(Xfer *xfer);
	virtual ~MonitorConditionUpdate();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x0049176C)

protected:
	virtual const char *GetSnapshotName() const;

public:
	AsciiString m_unk0x20;                         // +0x20; xfer AsciiString
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x24
typedef char MonitorConditionUpdate_size[(sizeof(MonitorConditionUpdate) == 0x24) ? 1 : -1];
typedef char MonitorConditionUpdate_off_m_unk0x20[(offsetof(MonitorConditionUpdate, m_unk0x20) == 0x20) ? 1 : -1];

#endif
