// BroadcastStealthUpdate.h: promoted from include/Draft by h05 (round 10). Proven by 2 rows that MATCH against it on main (0x004A34C0, 0x004A34C6);
// name, size, bases and virtual order are retail facts given below.
// BroadcastStealthUpdate.h: the BroadcastStealthUpdate module. SIZE 0x2C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004A342E calls UpdateModule first; it stores the
// tables 0x00C52364 at +0x0, 0x00C49F78 at +0xC, 0x00C52358 at +0x10. Members are what xfer (0x004A3752) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0xC slot 10 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0xC slot 48 -> 0x004B7BAA ?loadPostProcess@AudioLoopUpgrade@@MAEXXZ
//   +0xC slot 49 -> 0x00484181 ?name@Rva00484181Named@@QBEPBDXZ
//   +0x10 slot 0 -> 0x004A35C9 ?update@BroadcastStealthUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?, slot8 ?.
#pragma once
#ifndef BFME2_BROADCAST_STEALTH_UPDATE_H
#define BFME2_BROADCAST_STEALTH_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char BroadcastStealthUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class BroadcastStealthUpdate : public UpdateModule
{
public:
	BroadcastStealthUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~BroadcastStealthUpdate();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004A35C9)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedInt m_unk0x20;                         // +0x20; xfer UnsignedInt
	UnsignedByte m_gap0x24[0x8];
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x2C
typedef char BroadcastStealthUpdate_size[(sizeof(BroadcastStealthUpdate) == 0x2C) ? 1 : -1];
typedef char BroadcastStealthUpdate_off_m_unk0x20[(offsetof(BroadcastStealthUpdate, m_unk0x20) == 0x20) ? 1 : -1];

#endif
