// RunOffMapBehavior's single callback starts the flee action. The constructor
// installs its interface at +0x10, and the behavior selector returns that subobject.
#pragma once
#ifndef BFME2_RUN_OFF_MAP_BEHAVIOR_H
#define BFME2_RUN_OFF_MAP_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/BehaviorModule.h"

class Thing;
class ModuleData;
class Xfer;

// BehaviorModule ends at 0x10; this class's members follow it.
typedef char RunOffMapBehavior_base_size[(sizeof(BehaviorModule) == 0x10) ? 1 : -1];

class RunOffMapBehaviorInterface
{
public:
	virtual void triggerFlee() = 0;
};

class RunOffMapBehavior : public BehaviorModule, public RunOffMapBehaviorInterface
{
public:
	RunOffMapBehavior(Thing *thing, const ModuleData *moduleData);
	virtual RunOffMapBehaviorInterface *getRunOffMapBehaviorInterface() { return this; }
	virtual void triggerFlee();
	virtual NameKeyType getModuleNameKey() const;
	virtual void onObjectCreated();	// override: onObjectCreated slot 5 at +0x0 (0x00462213)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { BehaviorModule::loadPostProcess(); }	// retail slot 0x0058B03E: a jmp to BehaviorModule's, ICF-folded across classes (h05)
};
// SIZE 0x14
typedef char RunOffMapBehavior_size[(sizeof(RunOffMapBehavior) == 0x14) ? 1 : -1];

#endif
