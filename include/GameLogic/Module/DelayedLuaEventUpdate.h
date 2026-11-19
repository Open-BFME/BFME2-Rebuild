// DelayedLuaEventUpdate.h: the DelayedLuaEventUpdate module. SIZE 0x78 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004A8E7B calls UpdateModule first; it stores the
// tables 0x00C53A5C at +0x0, 0x00BEFF90 at +0xC, 0x00C53A50 at +0x10. Members are what xfer (0x004A8E0A) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x10 slot 0 -> 0x004A8F52 ?update@DelayedLuaEventUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?, XferRawBytes +0x20, slotC ?.
#pragma once
#ifndef BFME2_DELAYED_LUA_EVENT_UPDATE_H
#define BFME2_DELAYED_LUA_EVENT_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char DelayedLuaEventUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class DelayedLuaEventUpdate : public UpdateModule
{
public:
	DelayedLuaEventUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~DelayedLuaEventUpdate();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004A8F52)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x20[0x50];
	Real m_unk0x70;                                // +0x70; xfer Real
	Bool m_unk0x74;                                // +0x74; xfer Bool
	Bool m_unk0x75;                                // +0x75; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x78
typedef char DelayedLuaEventUpdate_size[(sizeof(DelayedLuaEventUpdate) == 0x78) ? 1 : -1];
typedef char DelayedLuaEventUpdate_off_m_unk0x70[(offsetof(DelayedLuaEventUpdate, m_unk0x70) == 0x70) ? 1 : -1];
typedef char DelayedLuaEventUpdate_off_m_unk0x74[(offsetof(DelayedLuaEventUpdate, m_unk0x74) == 0x74) ? 1 : -1];
typedef char DelayedLuaEventUpdate_off_m_unk0x75[(offsetof(DelayedLuaEventUpdate, m_unk0x75) == 0x75) ? 1 : -1];

#endif
