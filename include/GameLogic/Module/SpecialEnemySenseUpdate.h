// SpecialEnemySenseUpdate.h: the SpecialEnemySenseUpdate module. SIZE 0x20 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00499FA9 calls UpdateModule first; it stores the
// tables 0x00C504A4 at +0x0, 0x00BEFF90 at +0xC, 0x00C50498 at +0x10. Members are what xfer (0x0049A016) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x00499F5E ?name@Rva00499F5ENamed@@QBEPBDXZ
//   +0x10 slot 0 -> 0x0049A037 ?update@SpecialEnemySenseUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_SPECIAL_ENEMY_SENSE_UPDATE_H
#define BFME2_SPECIAL_ENEMY_SENSE_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char SpecialEnemySenseUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class SpecialEnemySenseUpdate : public UpdateModule
{
public:
	SpecialEnemySenseUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x0049A037)

protected:
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x20
typedef char SpecialEnemySenseUpdate_size[(sizeof(SpecialEnemySenseUpdate) == 0x20) ? 1 : -1];

#endif
