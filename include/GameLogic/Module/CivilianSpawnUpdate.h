// CivilianSpawnUpdate.h: the CivilianSpawnUpdate module. SIZE 0x24 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0047F9D4 calls UpdateModule first; it stores the
// tables 0x00C48418 at +0x0, 0x00BEFF90 at +0xC, 0x00C4840C at +0x10. Members are what xfer (0x0047F94C) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x0047F946 ?name@Rva0047F946Named@@QBEPBDXZ
//   +0x10 slot 0 -> 0x0047FAFE ?update@CivilianSpawnUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@UpdateModule@@UAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_CIVILIAN_SPAWN_UPDATE_H
#define BFME2_CIVILIAN_SPAWN_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char CivilianSpawnUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class CivilianSpawnUpdate : public UpdateModule
{
public:
	CivilianSpawnUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x0047FAFE)

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x20[0x4];
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x24
typedef char CivilianSpawnUpdate_size[(sizeof(CivilianSpawnUpdate) == 0x24) ? 1 : -1];

#endif
