// HordeAIUpdate.h: the horde AI update module and its worker variant.
// Target facts:
// - HordeAIUpdate (SIZE 0x3E8): ctor 0x0049A7F3 calls AIUpdateInterface's,
//   stores the tables 0x00C505F8 (+0), 0x00C50538 (+0xC), 0x00C5052C (+0x10),
//   0x00C50528 (+0x20), keeps 0x00C52F88 (+0x24), and stores DockUpdateInterface's
//   pure table 0x00C52EE8 then 0x00C50868 at +0x3E4, then setWakeFrame(obj,
//   UPDATE_SLEEP_NONE). Table 0x00C50868 is DockUpdateInterface's 20 slots:
//   isClearToApproach 0x0049A880, reserveApproachPosition 0x0049A8A1,
//   getEnterPosition 0x0049A8FA and slot 12 0x0049A973 are real bodies; the rest
//   are shared defaults (slot 2 `return true` 0x0049A6D7; slots 6/7 forward to
//   getEnterPosition through the vcall thunk 0x005CB26F; 8-11, 13, 15, 17 `ret 4`;
//   3, 14 and 18 `return true`; 4, 16 and 19 `return false`).
//   HordeWorkerAIUpdate's members start at +0x3E8.
// - HordeWorkerAIUpdate (SIZE 0x3F8): ctor 0x0049AD15 calls HordeAIUpdate's,
//   zeroes ObjectIDs +0x3E8/+0x3EC/+0x3F0 and the Bool +0x3F4.
#pragma once
#ifndef BFME2_HORDEAIUPDATE_H
#define BFME2_HORDEAIUPDATE_H

#include "GameLogic/Module/AIUpdate.h"
#include "GameLogic/Module/DockUpdate.h"

class HordeAIUpdate : public AIUpdateInterface, public DockUpdateInterface
{
public:
	HordeAIUpdate(Thing *thing, const ModuleData *moduleData);	// 0x0049A7F3

	virtual NameKeyType getModuleNameKey() const;				// 0x0049A67C
	virtual void aiDoCommand(const AICommandParms *parms);		// 0x0049A9A9
	virtual UpdateSleepTime update();							// 0x0049AB10
	virtual DockUpdateInterface *getDockUpdateInterface() { return this; }	// behavior slot 22, 0x0049A6C7

	Bool commandCancelsPorcupineFormation(const AICommandParms *parms);	// 0x0049A6DC
	Bool porcupineFormationIgnoresCommand(const AICommandParms *parms);	// 0x0049A76C

	// DockUpdateInterface (+0x3E4).
	virtual Bool isClearToApproach(const Object *docker) const;					// 0x0049A880
	virtual Bool reserveApproachPosition(Object *docker, Coord3D *position, Int *index);	// 0x0049A8A1
	virtual Bool advanceApproachPosition(Object *docker, Coord3D *position, Int *index) { return true; }	// 0x0049A6D7
	virtual Bool isClearToEnter(const Object *docker) const { return true; }	// Dock slot 3, 0x005CB9FA
	virtual Bool isClearToAdvance(const Object *docker, Int index) const { return false; }
	virtual void getEnterPosition(Object *docker, Coord3D *position);			// 0x0049A8FA
	virtual void getDockPosition(Object *docker, Coord3D *position) { getEnterPosition(docker, position); }
	virtual void getExitPosition(Object *docker, Coord3D *position) { getEnterPosition(docker, position); }
	virtual void onApproachReached(Object *docker) {}
	virtual void onEnterReached(Object *docker) {}
	virtual void onDockReached(Object *docker) {}
	virtual void onExitReached(Object *docker) {}
	virtual Bool action(Object *docker, Object *drone);						// 0x0049A973
	virtual void cancelDock(Object *docker) {}
	virtual Bool isDockOpen() { return true; }
	virtual void setDockOpen(Bool open) {}
	virtual Bool dockSlot16(Object *docker) { return false; }
	virtual void setDockCrippled(Bool setting) {}
	virtual Bool isAllowPassthroughType() { return true; }
	virtual Bool isRallyPointAfterDockType() { return false; }

protected:
	virtual void privateMoveToPosition(const Coord3D *, Real, CommandSourceType);	// 0x0049A93B
	virtual void bfmePrivateCommand42(Object *, CommandSourceType);				// 0x0049AC5F
protected:
	virtual void loadPostProcess() { AIUpdateInterface::loadPostProcess(); }	// retail slot 0x0049A7EE: a jmp to AIUpdateInterface's, ICF-folded across classes (h05)
protected:
	virtual const char *GetSnapshotName() const;	// slot 2 of the class table, 0x0049A6C1 (h05 r9)
};
// SIZE 0x3E8

class HordeWorkerAIUpdate : public HordeAIUpdate
{
public:
	HordeWorkerAIUpdate(Thing *thing, const ModuleData *moduleData);	// 0x0049AD15

	virtual NameKeyType getModuleNameKey() const;				// 0x0049ACD0
	virtual void aiDoCommand(const AICommandParms *parms);		// 0x0049B231
	virtual UpdateSleepTime update();							// 0x0049AEA4

protected:
	virtual void xfer(Xfer *xfer);								// 0x0049AE07

	ObjectID m_unk0x3E8;										// +0x3E8
	ObjectID m_unk0x3EC;										// +0x3EC
	ObjectID m_unk0x3F0;										// +0x3F0
	Bool m_unk0x3F4;											// +0x3F4
protected:
	virtual const char *GetSnapshotName() const;	// slot 2 of the class table, 0x0049ACC5 (h05 r9)
};
// SIZE 0x3F8

#endif
