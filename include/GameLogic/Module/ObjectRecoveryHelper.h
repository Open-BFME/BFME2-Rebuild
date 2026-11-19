// ObjectRecoveryHelper.h: one of the object helpers (Object.cpp builds them). SIZE 0x20.
// Target facts (retail game.dat 1.06):
// - Layout: ObjectHelper (0x20); no members of its own (xfer 0x004DF87C is Version1, then
//   ObjectHelper's xfer 0x004DF81B, and is shared by the Recovery, Repulsor and WeaponStatus helpers).
// - Tables: 0x007FBD98 at +0 (13 slots: own 0 deleting destructor, 1 loadPostProcess = the helpers'
//   fold 0x00483CEF `jmp 0x00497805`, 2 GetSnapshotName 0x0028CA92, 3 xfer 0x004DF87C, 4 the
//   module key 0x0028CA4D), 0x007EFF90 at +0xC, 0x007FBD8C at +0x10 (update 0x004DF84F).
// - Constructor 0x0028C9F9: ObjectHelper's (0x0028C8DF), the three table stores, then
//   setWakeFrame(getObject(), UPDATE_SLEEP_FOREVER).
// - update 0x004DF84F (UpdateModuleInterface slot 0, this at +0x10) calls slot 3 of the object's
//   body module (+0x254) and sleeps forever.
// Donor: Zero Hour has the same class (GameLogic/Module/ObjectRecoveryHelper.h) on ObjectHelper.
#pragma once
#ifndef BFME2_OBJECTRECOVERYHELPER_H
#define BFME2_OBJECTRECOVERYHELPER_H

#include "GameLogic/Module/ObjectHelper.h"

class Xfer;

class ObjectRecoveryHelper : public ObjectHelper
{
public:
	ObjectRecoveryHelper(Thing *thing, const ModuleData *moduleData);	// 0x0028C9F9
	virtual NameKeyType getModuleNameKey() const;				// slot 4, 0x0028CA4D
	virtual UpdateSleepTime update();							// 0x004DF84F

protected:
	virtual void loadPostProcess();								// slot 1 (0x00483CEF: tail-jumps to ObjectHelper's)
	virtual const char *GetSnapshotName() const;				// slot 2, 0x0028CA92
	virtual void xfer(Xfer *xfer);								// slot 3, 0x004DF87C
};
// SIZE 0x20
typedef char ObjectRecoveryHelper_size[(sizeof(ObjectRecoveryHelper) == 0x20) ? 1 : -1];

#endif
