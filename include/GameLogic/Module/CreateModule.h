// CreateModule.h: the create-module interface and the concrete CreateModule base.
// Target facts (retail game.dat):
// - CreateModuleInterface's own table 0x00C4EF80 is four __purecall slots.
//   CreateModule's 0x00C59358 fills them as: 0 onCreate (still pure),
//   1 onBuildComplete 0x00250A49, 2 an empty body (the shared `ret`
//   0x000B3FD0; unnamed), 3 0x00098CBC `mov al,[ecx+4]; ret`, which returns
//   the Bool at +0x14 (Zero Hour's shouldDoOnBuildComplete).
//   ReplaceSelfUpgrade carries the same interface at +0x1C (table 0x00858AB8).
// - CreateModule is BehaviorModule (ctor 0x00253330) plus the interface at
//   +0x10 and m_needToRunOnBuildComplete at +0x14, set to true; ctor 0x004B8CF7
//   stores 0x00C59368 at +0, 0x00BEF9C0 at +0xC and 0x00C59358 at +0x10.
#pragma once
#ifndef BFME2_CREATEMODULE_H
#define BFME2_CREATEMODULE_H

#include "Common/GameType.h"
#include "GameLogic/Module/BehaviorModule.h"

class Thing;
class Xfer;

class CreateModuleInterface
{
public:
	virtual void onCreate() = 0;										// slot 0
	virtual void onBuildComplete() = 0;									// slot 1
	virtual void vslot02() = 0;											// slot 2
	virtual Bool shouldDoOnBuildComplete() const = 0;					// slot 3
};

class CreateModule : public BehaviorModule, public CreateModuleInterface
{
public:
	CreateModule(Thing *thing, const ModuleData *moduleData);			// 0x004B8CF7

	virtual CreateModuleInterface *getCreate() { return this; }

	virtual void onBuildComplete() { m_needToRunOnBuildComplete = false; }	// fold 0x00250A49: `mov byte [ecx+4],0` on the +0x10 interface
	virtual void vslot02() {}
	virtual Bool shouldDoOnBuildComplete() const { return m_needToRunOnBuildComplete; }

protected:
	virtual ~CreateModule();											// 0x004B8CDE (??1CreateModule@@MAE@XZ)
	virtual const char *GetSnapshotName() const;						// 0x004B8D2C
	virtual void xfer(Xfer *xfer);										// 0x004B8D4E

	Bool m_needToRunOnBuildComplete;									// 0x14
protected:
	virtual void loadPostProcess();	// out of line (a jmp to BehaviorModule's), ICF-folded with UpdateModule's at 0x0058B03E; inline, MSVC folds it into each subclass's forwarder and loses retail's second level (0x00497805) (h05)
};
// SIZE 0x18

#endif
