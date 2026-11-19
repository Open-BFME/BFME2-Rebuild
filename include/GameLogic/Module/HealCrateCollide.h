// HealCrateCollide.h: the crate collide that heals the collector's player.
// Target facts: the constructor 0x004BC80F and destructor 0x004BC83F follow the
// CrateCollide base (no members of its own, size 0x18); getModuleNameKey 0x004BC858
// keys "HealCrateCollide"; executeCrateBehavior 0x004BC8B9 is the override of the
// base's pure slot.
#pragma once
#ifndef BFME2_HEALCRATECOLLIDE_H
#define BFME2_HEALCRATECOLLIDE_H

#include "GameLogic/Module/CrateCollide.h"

class Xfer;

class HealCrateCollide : public CrateCollide
{
public:
	HealCrateCollide(Thing *thing, const ModuleData *moduleData);	// 0x004BC80F
	virtual ~HealCrateCollide();									// 0x004BC83F
	virtual NameKeyType getModuleNameKey() const;					// 0x004BC858

protected:
	virtual bool executeCrateBehavior(Object *other);				// 0x004BC8B9
protected:
	virtual void loadPostProcess() { CrateCollide::loadPostProcess(); }	// slot 1: retail 0x00483CEF, `jmp` to CrateCollide's (0x00497805) (h05 r9)
	virtual const char *GetSnapshotName() const;					// slot 2, 0x004BC839
	virtual void xfer(Xfer *xfer);									// slot 3: retail 0x004BD2F9, Version1 then CrateCollide::xfer (the fold SalvageCrateCollide shares) (h05 r9)
};
// SIZE 0x18

#endif
