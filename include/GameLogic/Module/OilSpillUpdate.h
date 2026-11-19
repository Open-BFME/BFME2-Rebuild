// OilSpillUpdate.h: the OilSpillUpdate module. SIZE 0x38 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0048C1F7 calls FireWeaponUpdate first; it stores the
// tables 0x00C4C380 at +0x0, 0x00BEFF90 at +0xC, 0x00C4C374 at +0x10. Members are what xfer (0x0048C415) transfers after FireWeaponUpdate::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 2 -> 0x0048C258 ?Rva0048C258Get@@YAHXZ
//   +0x10 slot 0 -> 0x0048C30E ?update@OilSpillUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: ?xfer@FireWeaponUpdate@@MAEXPAVXfer@@@Z ?, slot10 ?, Int ?, slot4 ?, Coord3D ?, slot4 ?.
#pragma once
#ifndef BFME2_OIL_SPILL_UPDATE_H
#define BFME2_OIL_SPILL_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/FireWeaponUpdate.h"

class Thing;
class ModuleData;
class Xfer;

// FireWeaponUpdate ends at 0x2C; this class's members follow it.
typedef char OilSpillUpdate_base_size[(sizeof(FireWeaponUpdate) == 0x2C) ? 1 : -1];

class OilSpillUpdate : public FireWeaponUpdate
{
public:
	OilSpillUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~OilSpillUpdate();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x0048C30E)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x2C[0xC];
};
// SIZE 0x38
typedef char OilSpillUpdate_size[(sizeof(OilSpillUpdate) == 0x38) ? 1 : -1];

#endif
