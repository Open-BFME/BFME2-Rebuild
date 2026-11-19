// OCLUpdate.h: promoted from include/Draft by h05 (round 10). Proven by 3 rows that MATCH against it on main (0x0049B2C1, 0x0049B303, 0x0049B310);
// name, size, bases and virtual order are retail facts given below.
// OCLUpdate.h: the OCLUpdate module. SIZE 0x2C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0049B381 calls UpdateModule first; it stores the
// tables 0x00C50B40 at +0x0, 0x00BEFF90 at +0xC, 0x00C50B34 at +0x10. Members are what xfer (0x0049B421) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x0049B2BB ?name@Rva0049B2BBNamed@@QBEPBDXZ
//   +0x10 slot 0 -> 0x0049B483 ?update@Rva0049B2A2@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_O_C_L_UPDATE_H
#define BFME2_O_C_L_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char OCLUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class OCLUpdate : public UpdateModule
{
public:
	OCLUpdate(Thing *thing, const ModuleData *moduleData);
	unsigned int getRemainingFrames() const;
	float getCountdownPercent() const;
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	UnsignedByte shouldCreate();		// 0x0049B3D2: the creation frame has come and the object is not under construction (class by the update 0x0049B483 calling it on its module)
	void setNextCreationFrame();	// 0x0049B3F3: restarts the timer with a random delay between MinDelay and MaxDelay

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedInt m_nextCreationFrame;                    // +0x20; xfer UnsignedInt
	UnsignedInt m_timerStartedFrame;                    // +0x24; xfer UnsignedInt
	Int m_currentCount;                            // +0x28; xfer Int (version 2 and later)
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x2C
typedef char OCLUpdate_size[(sizeof(OCLUpdate) == 0x2C) ? 1 : -1];
typedef char OCLUpdate_off_m_nextCreationFrame[(offsetof(OCLUpdate, m_nextCreationFrame) == 0x20) ? 1 : -1];
typedef char OCLUpdate_off_m_timerStartedFrame[(offsetof(OCLUpdate, m_timerStartedFrame) == 0x24) ? 1 : -1];
typedef char OCLUpdate_off_m_currentCount[(offsetof(OCLUpdate, m_currentCount) == 0x28) ? 1 : -1];

#endif
