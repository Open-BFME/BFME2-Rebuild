// RadarUpdate.h: promoted from include/Draft by h05 (round 10). Proven by 3 rows that MATCH against it on main (0x004A0C2D, 0x004A0D0F, 0x004A0D5F);
// name, size, bases and virtual order are retail facts given below.
// RadarUpdate.h: the RadarUpdate module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004A0C72 calls UpdateModule first; it stores the
// tables 0x00C51A0C at +0x0, 0x00BEFF90 at +0xC, 0x00C51A00 at +0x10. Members are what xfer (0x004A0CC3) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004A0C27 ?name@Rva004A0C27Named@@QBEPBDXZ
//   +0x10 slot 0 -> 0x004A0D5F ?update@RadarUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_RADAR_UPDATE_H
#define BFME2_RADAR_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char RadarUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class RadarUpdate : public UpdateModule
{
public:
	RadarUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual UpdateSleepTime update();
	void extendRadar();					// 0x004A0D0F

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedInt m_unk0x20;                         // +0x20; xfer UnsignedInt
	Bool m_unk0x24;                                // +0x24; xfer Bool
	Bool m_unk0x25;                                // +0x25; xfer Bool
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x28
typedef char RadarUpdate_size[(sizeof(RadarUpdate) == 0x28) ? 1 : -1];
typedef char RadarUpdate_off_m_unk0x20[(offsetof(RadarUpdate, m_unk0x20) == 0x20) ? 1 : -1];
typedef char RadarUpdate_off_m_unk0x24[(offsetof(RadarUpdate, m_unk0x24) == 0x24) ? 1 : -1];
typedef char RadarUpdate_off_m_unk0x25[(offsetof(RadarUpdate, m_unk0x25) == 0x25) ? 1 : -1];

#endif
