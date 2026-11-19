// DamageFieldUpdate.h: the DamageFieldUpdate module. SIZE 0x30 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004912A9 calls FireWeaponUpdate first; it stores the
// tables 0x00C4D970 at +0x0, 0x00BEFF90 at +0xC, 0x00C4D964 at +0x10. Members are what xfer (0x0049157D) transfers after FireWeaponUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00483CEF ?loadPostProcess@ObjectDefectionHelper@@MAEXXZ
//   +0x0 slot 2 -> 0x0049125E ?name@Rva0049125ENamed@@QBEPBDXZ
//   +0x10 slot 0 -> 0x00491316 ?update@DamageFieldUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@FireWeaponUpdate@@MAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_DAMAGE_FIELD_UPDATE_H
#define BFME2_DAMAGE_FIELD_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/FireWeaponUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// FireWeaponUpdate ends at 0x2C; this class's members follow it.
typedef char DamageFieldUpdate_base_size[(sizeof(FireWeaponUpdate) == 0x2C) ? 1 : -1];

class DamageFieldUpdate : public FireWeaponUpdate
{
public:
	DamageFieldUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x00491316)

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x2C[0x4];
protected:
	virtual void loadPostProcess() { FireWeaponUpdate::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to FireWeaponUpdate's (h05)
};
// SIZE 0x30
typedef char DamageFieldUpdate_size[(sizeof(DamageFieldUpdate) == 0x30) ? 1 : -1];

#endif
