// ObjectWeaponStatusHelper.h: one of the object helpers (Object.cpp builds them). SIZE 0x20.
// Target facts (retail game.dat 1.06):
// - Layout: ObjectHelper (0x20); no members of its own (xfer 0x004DF87C is Version1, then
//   ObjectHelper's xfer 0x004DF81B, and is shared by the Recovery, Repulsor and WeaponStatus helpers).
// - Tables: 0x007FBE48 at +0 (13 slots: own 0 deleting destructor, 1 loadPostProcess = the helpers'
//   fold 0x00483CEF `jmp 0x00497805`, 2 GetSnapshotName 0x0028CBDB, 3 xfer 0x004DF87C, 4 the
//   module key 0x0028CB96), 0x007EFF90 at +0xC, 0x007FBE3C at +0x10 (update 0x0028F976).
// - Constructor 0x0028CB45: ObjectHelper's (0x0028C8DF), the three table stores, then
//   setWakeFrame(getObject(), UPDATE_SLEEP_NONE).
// - update 0x0028F976 calls Object 0x0028DB3C (Zero Hour: adjustModelConditionForWeaponStatus) and
//   returns UPDATE_SLEEP_NONE.
// - Primary slot 12 (getUpdatePhase) is 0x0044827C `push 3; pop eax`: PHASE_FINAL, as in Zero Hour.
// - +0x10 slot 1 (getDisabledTypesToProcess) is its own 0x004DF8C2: a mask of bits 3 and 8.
// Donor: Zero Hour has the same class (GameLogic/Module/ObjectWeaponStatusHelper.h) on ObjectHelper.
#pragma once
#ifndef BFME2_OBJECTWEAPONSTATUSHELPER_H
#define BFME2_OBJECTWEAPONSTATUSHELPER_H

#include "Common/BitFlags11.h"
#include "GameLogic/Module/ObjectHelper.h"

class Xfer;

class ObjectWeaponStatusHelper : public ObjectHelper
{
public:
	ObjectWeaponStatusHelper(Thing *thing, const ModuleData *moduleData);	// 0x0028CB45
	virtual NameKeyType getModuleNameKey() const;				// slot 4, 0x0028CB96
	virtual UpdateSleepTime update();							// 0x0028F976
	virtual DisabledMaskType getDisabledTypesToProcess() const { return DisabledMaskType(DisabledMaskType::kInit, 3, 8); }	// +0x10 slot 1, 0x004DF8C2
	virtual SleepyUpdatePhase getUpdatePhase() const { return PHASE_FINAL; }	// slot 12, the fold 0x0044827C

protected:
	virtual void loadPostProcess();								// slot 1 (0x00483CEF: tail-jumps to ObjectHelper's)
	virtual const char *GetSnapshotName() const;				// slot 2, 0x0028CBDB
	virtual void xfer(Xfer *xfer);								// slot 3, 0x004DF87C
};
// SIZE 0x20
typedef char ObjectWeaponStatusHelper_size[(sizeof(ObjectWeaponStatusHelper) == 0x20) ? 1 : -1];

#endif
