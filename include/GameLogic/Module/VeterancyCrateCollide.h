// VeterancyCrateCollide.h: the crate collide that grants veterancy to the collector.
// Target facts: constructor 0x004BCE32 forwards to CrateCollide's; the destructor
// 0x004BCE62 is empty and tail-jumps to CrateCollide's; getModuleNameKey
// 0x004BCE7B keys "VeterancyCrateCollide"; getLevelsToGain 0x004BCEDC is 0 when the
// module data's AddsOwnerVeterancy is set and 1 otherwise. The CrateCollide base is
// 0x18 bytes and this class adds no members.
// Not declared yet (rows still open): executeCrateBehavior 0x004BCFF0 and
// isValidToExecute 0x004BCEF0, the two pure slots CrateCollide leaves to its derivatives.
#pragma once
#ifndef BFME2_VETERANCYCRATECOLLIDE_H
#define BFME2_VETERANCYCRATECOLLIDE_H

#include "GameLogic/Module/CrateCollide.h"
#include "GameLogic/Module/VeterancyCrateCollideModuleData.h"

class Xfer;

class VeterancyCrateCollide : public CrateCollide
{
public:
	VeterancyCrateCollide(Thing *thing, const ModuleData *moduleData);	// 0x004BCE32
	virtual ~VeterancyCrateCollide();									// 0x004BCE62
	virtual NameKeyType getModuleNameKey() const;						// 0x004BCE7B


private:
	int getLevelsToGain() const;										// 0x004BCEDC, private (ledger access A)

public:
	const VeterancyCrateCollideModuleData *getVeterancyCrateCollideModuleData() const
	{
		return static_cast<const VeterancyCrateCollideModuleData *>(getCrateCollideModuleData());
	}
protected:
	virtual void loadPostProcess() { CrateCollide::loadPostProcess(); }	// slot 1: retail 0x00483CEF, `jmp` to CrateCollide's (0x00497805) (h05 r9)
	virtual const char *GetSnapshotName() const;					// slot 2, 0x004BCE5C
	virtual void xfer(Xfer *xfer);									// slot 3: retail 0x004BD2F9, Version1 then CrateCollide::xfer (the fold SalvageCrateCollide shares) (h05 r9)
	virtual bool executeCrateBehavior(Object *other);				// slot 12, 0x004BCFF0
	virtual bool isValidToExecute(const Object *other) const;		// slot 13, 0x004BCEF0
};
// SIZE 0x18

#endif
