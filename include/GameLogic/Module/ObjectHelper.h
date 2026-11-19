// ObjectHelper.h: the object-helper update base (ObjectDefectionHelper and its siblings).
// SIZE 0x20: an UpdateModule with no members of its own.
// Target facts (retail game.dat 1.06):
// - Layout: UpdateModule (0x20). The helper's own derived fields start at +0x20
//   (ObjectDefectionHelper's four members, ObjectDefectionHelper.h).
// - Primary vtable: slot 0 is the scalar deleting destructor, slot 3 is the xfer
//   that ObjectDefectionHelper::xfer calls first (retail 0x004DF81B; the map folds
//   it with other classes' identical base xfers, so its name is not proven here).
// - The base destructor is out of line and the derived destructors tail-jump to it.
// This header declares only what the rowed helper bodies use. The constructor and
// the destructor are defined by the helper's own translation unit when it lands.
#pragma once
#ifndef BFME2_OBJECTHELPER_H
#define BFME2_OBJECTHELPER_H

#include <stddef.h>
#include "GameLogic/Module/UpdateModule.h"

class Xfer;

class ObjectHelper : public UpdateModule
{
public:
	ObjectHelper(Thing *thing, const ModuleData *moduleData);	// 0x0028C8DF (out of line: the helpers' ctors call it)
	virtual ~ObjectHelper();
	// UpdateModuleInterface slot 1: every helper runs whatever is disabled. Out of line
	// (0x004DF396, a copy of DISABLEDMASK_ALL, ICF-folded with FireWeaponWhenDeadBehavior's);
	// ObjectDefectionHelper's +0x10 table 0x007FBD34 carries it.
	virtual DisabledMaskType getDisabledTypesToProcess() const;

protected:
	virtual void xfer(Xfer *xfer);								// slot 3, retail 0x004DF81B
	// Slot 1: out of line; every helper's own loadPostProcess tail-jumps to it
	// (ObjectDefectionHelper's slot 1 is 0x00483CEF, `jmp 0x00497805`).
	virtual void loadPostProcess();
};
// SIZE 0x20
typedef char ObjectHelper_size[(sizeof(ObjectHelper) == 0x20) ? 1 : -1];

#endif
