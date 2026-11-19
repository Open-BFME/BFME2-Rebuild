// UnitCrateCollide.h: the crate collide that gives the collector a free unit.
// Target facts: the constructor 0x004BCC13 and destructor 0x004BCC43 follow the
// CrateCollide base (no members of its own, size 0x18); getModuleNameKey 0x004BCC5C
// keys "UnitCrateCollide"; executeCrateBehavior 0x004BCCBD is the override of the
// base's pure slot.
#pragma once
#ifndef BFME2_UNITCRATECOLLIDE_H
#define BFME2_UNITCRATECOLLIDE_H

#include "GameLogic/Module/CrateCollide.h"

class Xfer;

class UnitCrateCollide : public CrateCollide
{
public:
	UnitCrateCollide(Thing *thing, const ModuleData *moduleData);	// 0x004BCC13
	virtual ~UnitCrateCollide();									// 0x004BCC43
	virtual NameKeyType getModuleNameKey() const;					// 0x004BCC5C

protected:
	virtual bool executeCrateBehavior(Object *other);				// 0x004BCCBD
protected:
	virtual void loadPostProcess() { CrateCollide::loadPostProcess(); }	// slot 1: retail 0x00483CEF, `jmp` to CrateCollide's (0x00497805) (h05 r9)
	virtual const char *GetSnapshotName() const;					// slot 2, 0x004BCC3D
	virtual void xfer(Xfer *xfer);									// slot 3: retail 0x004BD2F9, Version1 then CrateCollide::xfer (the fold SalvageCrateCollide shares) (h05 r9)
};
// SIZE 0x18

#endif
