// ShroudCrateCollide.h: the crate collide that shrouds the map around the collector.
// Target facts: the constructor 0x004BCADD follows the CrateCollide base (no members
// of its own, size 0x18); getModuleNameKey 0x004BCBB2 keys "ShroudCrateCollide";
// executeCrateBehavior 0x004BCB26 is the override of the base's pure slot. The map
// has no destructor row for this class.
#pragma once
#ifndef BFME2_SHROUDCRATECOLLIDE_H
#define BFME2_SHROUDCRATECOLLIDE_H

#include "GameLogic/Module/CrateCollide.h"

class Xfer;

class ShroudCrateCollide : public CrateCollide
{
public:
	ShroudCrateCollide(Thing *thing, const ModuleData *moduleData);	// 0x004BCADD
	virtual NameKeyType getModuleNameKey() const;					// 0x004BCBB2

protected:
	virtual bool executeCrateBehavior(Object *other);				// 0x004BCB26
protected:
	virtual void loadPostProcess() { CrateCollide::loadPostProcess(); }	// slot 1: retail 0x00483CEF, `jmp` to CrateCollide's (0x00497805) (h05 r9)
	virtual const char *GetSnapshotName() const;					// slot 2, 0x004BCB07
	virtual void xfer(Xfer *xfer);									// slot 3: retail 0x004BD2F9, Version1 then CrateCollide::xfer (the fold SalvageCrateCollide shares) (h05 r9)
};
// SIZE 0x18

#endif
