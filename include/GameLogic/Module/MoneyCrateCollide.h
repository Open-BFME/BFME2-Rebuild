// MoneyCrateCollide.h: the crate collide that pays the collector's player.
// Target facts: the constructor 0x004BC93F and destructor 0x004BC96F follow the
// CrateCollide base (no members of its own, size 0x18); getModuleNameKey 0x004BC988
// keys "MoneyCrateCollide"; executeCrateBehavior 0x004BC9E9 is the override of the
// base's pure slot.
#pragma once
#ifndef BFME2_MONEYCRATECOLLIDE_H
#define BFME2_MONEYCRATECOLLIDE_H

#include "GameLogic/Module/CrateCollide.h"

class Xfer;

class MoneyCrateCollide : public CrateCollide
{
public:
	MoneyCrateCollide(Thing *thing, const ModuleData *moduleData);	// 0x004BC93F
	virtual ~MoneyCrateCollide();									// 0x004BC96F
	virtual NameKeyType getModuleNameKey() const;					// 0x004BC988

protected:
	virtual bool executeCrateBehavior(Object *other);				// 0x004BC9E9
protected:
	virtual void loadPostProcess() { CrateCollide::loadPostProcess(); }	// slot 1: retail 0x00483CEF, `jmp` to CrateCollide's (0x00497805) (h05 r9)
	virtual const char *GetSnapshotName() const;					// slot 2, 0x004BC969
	virtual void xfer(Xfer *xfer);									// slot 3: retail 0x004BD2F9, Version1 then CrateCollide::xfer (the fold SalvageCrateCollide shares) (h05 r9)
};
// SIZE 0x18

#endif
