// ObjectRepulsorHelper.h: one of the object helpers (Object.cpp builds them). SIZE 0x20.
// Target facts (retail game.dat 1.06):
// - Layout: ObjectHelper (0x20); no members of its own (xfer 0x004DF87C is Version1, then
//   ObjectHelper's xfer 0x004DF81B, and is shared by the Recovery, Repulsor and WeaponStatus helpers).
// - Tables: 0x007FBDF0 at +0 (13 slots: own 0 deleting destructor, 1 loadPostProcess = the helpers'
//   fold 0x00483CEF `jmp 0x00497805`, 2 GetSnapshotName 0x0028CB23, 3 xfer 0x004DF87C, 4 the
//   module key 0x0028CADE), 0x007EFF90 at +0xC, 0x007FBDE4 at +0x10 (update 0x004DF897).
// - Constructor 0x0028CAB4: ObjectHelper's (0x0028C8DF) and the three table stores; nothing else.
// - update 0x004DF897 clears object status 8 (setStatus(8, false)) and sleeps forever (Zero Hour:
//   clearStatus(OBJECT_STATUS_REPULSOR)).
// Donor: Zero Hour has the same class (GameLogic/Module/ObjectRepulsorHelper.h) on ObjectHelper.
#pragma once
#ifndef BFME2_OBJECTREPULSORHELPER_H
#define BFME2_OBJECTREPULSORHELPER_H

#include "GameLogic/Module/ObjectHelper.h"

class Xfer;

class ObjectRepulsorHelper : public ObjectHelper
{
public:
	ObjectRepulsorHelper(Thing *thing, const ModuleData *moduleData);	// 0x0028CAB4
	virtual NameKeyType getModuleNameKey() const;				// slot 4, 0x0028CADE
	virtual UpdateSleepTime update();							// 0x004DF897

protected:
	virtual void loadPostProcess();								// slot 1 (0x00483CEF: tail-jumps to ObjectHelper's)
	virtual const char *GetSnapshotName() const;				// slot 2, 0x0028CB23
	virtual void xfer(Xfer *xfer);								// slot 3, 0x004DF87C
};
// SIZE 0x20
typedef char ObjectRepulsorHelper_size[(sizeof(ObjectRepulsorHelper) == 0x20) ? 1 : -1];

#endif
