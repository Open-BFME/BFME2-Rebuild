// SalvageCrateCollide.h: the crate collide that hands the collector salvage.
// Target facts: the constructor 0x004BD1D7 and destructor 0x004BD207 follow the
// CrateCollide base (no members of its own, size 0x18); getModuleNameKey 0x004BD220
// keys "SalvageCrateCollide"; isValidToExecute 0x004BD314 overrides the base's
// virtual test; executeCrateBehavior 0x004BD442 is the override of the pure slot.
// Two private helpers (0x004BD281, 0x004BD2D0) and doMoney 0x004BD342 are not
// declared yet: their identities are open.
#pragma once
#ifndef BFME2_SALVAGECRATECOLLIDE_H
#define BFME2_SALVAGECRATECOLLIDE_H

#include "GameLogic/Module/CrateCollide.h"

class ObjectIterator;

class SalvageCrateCollide : public CrateCollide
{
public:
	SalvageCrateCollide(Thing *thing, const ModuleData *moduleData);	// 0x004BD1D7
	virtual ~SalvageCrateCollide();										// 0x004BD207
	virtual NameKeyType getModuleNameKey() const;						// 0x004BD220
	virtual bool isSalvageCrateCollide() const { return true; }			// slot 5: the shared `mov al,1; ret` stub 0x0050B5C6

private:
	Int rollCrateOutcome();												// 0x004BD281: 2 level, 4 upgrade, else 3
	void doLevelGainNearby(ObjectIterator *units);						// 0x004BD2D0 (placeholder row): one level for every unit the scan found
	void doMoney(Object *other);										// 0x004BD342
protected:
	virtual void xfer(Xfer *xfer);										// slot 3, 0x004BD2F9: Version1, then CrateCollide's (f08)
	virtual bool executeCrateBehavior(Object *other);					// 0x004BD442
	virtual bool isValidToExecute(const Object *other) const;			// 0x004BD314
protected:
	virtual const char *GetSnapshotName() const;	// 0x004BD201, primary slot 2; retail's table has this class's own (h05, gen3 vtable-content list)
protected:
	virtual void loadPostProcess() { CrateCollide::loadPostProcess(); }	// retail slot 1 0x00483CEF: a jmp to CrateCollide's (h05)
};
// SIZE 0x18

#endif
