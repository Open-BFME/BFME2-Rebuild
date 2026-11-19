// PartTheHeavensUpdate.h: promoted from include/Draft by h05 (round 10). Proven by 5 rows that MATCH against it on main (0x004AC93C, 0x004AC942, 0x004ACAAA, 0x004ACBB3, 0x004ACBE8);
// name, size, bases and virtual order are retail facts given below.
// PartTheHeavensUpdate.h: the PartTheHeavensUpdate module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004AC998 calls UpdateModule first; it stores the
// tables 0x00C54DA4 at +0x0, 0x00BEFF90 at +0xC, 0x00C54D98 at +0x10. Members are what xfer (0x004ACAAA) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004ACBE8 ?loadPostProcess@PartTheHeavensUpdate@@MAEXXZ
//   +0x10 slot 0 -> 0x004ACBB3 ?update@PartTheHeavensUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_PART_THE_HEAVENS_UPDATE_H
#define BFME2_PART_THE_HEAVENS_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char PartTheHeavensUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class PartTheHeavensUpdate : public UpdateModule
{
public:
	PartTheHeavensUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x004ACBB3)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void loadPostProcess();	// override: loadPostProcess slot 1 at +0x0 (0x004ACBE8)

private:
	void rva004AC9FA(Int elapsed);	// 0x004AC9FA (update: frames since +0x20)
	void rva004ACADA();	// 0x004ACADA (update on the first frame, loadPostProcess)

public:
	UnsignedInt m_unk0x20;                         // +0x20; xfer UnsignedInt
	UnsignedInt m_unk0x24;							// +0x24: the constructor 0x004AC998 zeroes it (a dword store)
};
// SIZE 0x28
typedef char PartTheHeavensUpdate_size[(sizeof(PartTheHeavensUpdate) == 0x28) ? 1 : -1];
typedef char PartTheHeavensUpdate_off_m_unk0x20[(offsetof(PartTheHeavensUpdate, m_unk0x20) == 0x20) ? 1 : -1];

#endif
