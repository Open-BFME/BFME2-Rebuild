// StealthDetectorUpdate.h: the stealth detector update module. SIZE 0x20.
// Target facts: the class adds no members to UpdateModule. Its table 0x00C51FC0
// carries the deleting destructor 0x004A2D2D, loadPostProcess (inherited),
// xfer 0x004A2C30, getModuleNameKey 0x004A2C74 and update 0x004A2D93; the
// non-virtual name getter 0x004A2BEA returns the class-name literal. The
// constructor 0x004A2CB9 parks the module with a random wake over the module
// data's update rate unless the data is initially disabled.
#pragma once
#ifndef BFME2_STEALTHDETECTORUPDATE_H
#define BFME2_STEALTHDETECTORUPDATE_H

#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "Common/BitFlags11.h"

class ModuleData;
class Thing;
class Xfer;

class StealthDetectorUpdate : public UpdateModule
{
public:
	StealthDetectorUpdate(Thing *thing, const ModuleData *moduleData);
	virtual ~StealthDetectorUpdate();

	virtual NameKeyType getModuleNameKey() const;		// 0x004A2C74
	virtual UpdateSleepTime update();					// 0x004A2D93
	virtual DisabledMaskType getDisabledTypesToProcess() const;	// slot 1, the shared body 0x004A2D67: one bit (3) set in an empty mask

protected:
	// Own override: the table slot is the fold 0x00497805 of the per-module
	// `{ UpdateModule::loadPostProcess(); }` bodies, not UpdateModule's 0x0058B03E.
	virtual void loadPostProcess();
	virtual void xfer(Xfer *xfer);						// 0x004A2C30
protected:
	virtual const char *GetSnapshotName() const;	// 0x004A2BEA (retail slot 2 of the class table)
};
// SIZE 0x20

#endif
