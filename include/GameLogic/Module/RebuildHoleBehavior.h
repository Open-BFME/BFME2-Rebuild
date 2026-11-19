// RebuildHoleBehavior.h: the rebuild-hole module: it keeps a worker, respawns him and rebuilds the
// structure that stood here. SIZE 0x48 (ModuleFactory operator new).
// Target facts: the constructor 0x00483271 calls UpdateModule (0x00253390) first, stores the
// folded all-pure tables of the two interfaces it inherits at +0x20 (VA 0x00C1C780 = DieModuleInterface)
// and +0x24 (VA 0x00C078DC = RebuildHoleBehaviorInterface), zeroes +0x28..+0x3C, then stores its
// own tables: primary VA 0x00C49A74 (+0), 0x00C49188 (+0xC), update interface 0x00C49A68 (+0x10),
// die interface 0x00C49A64 (+0x20), rebuild interface 0x00C49A50 (+0x24); +0x40 = 0.0f, +0x44 = 1.
// Primary table: UpdateModule's 13 slots, the class's loadPostProcess forward at slot 1 (the folded
// 0x00497805, as SlowDeathBehavior's) and its own slot 13 getRebuildHoleBehaviorInterface (0x00483322).
// Members are what xfer (0x004833EC) transfers after UpdateModule::xfer, typed by the Xfer slot each
// goes through, in that order; the templates are transferred as their names (ThingTemplate +0x64).
#pragma once
#ifndef BFME2_REBUILD_HOLE_BEHAVIOR_H
#define BFME2_REBUILD_HOLE_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/DieModuleInterface.h"
#include "GameLogic/Module/RebuildHoleBehaviorInterface.h"

class Object;
class Thing;
class ThingTemplate;
class DamageInfo;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow the two interface bases.
typedef char RebuildHoleBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class RebuildHoleBehavior : public UpdateModule, public DieModuleInterface, public RebuildHoleBehaviorInterface
{
public:
	RebuildHoleBehavior(Thing *thing, const ModuleData *moduleData);					// 0x00483271
	virtual NameKeyType getModuleNameKey() const;										// 0x004832D7
	virtual ~RebuildHoleBehavior();														// 0x0048353E
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x00483645)
	virtual void onDie(const DamageInfo *);	// override: onDie slot 0 at +0x20 (0x004835FB)
	virtual DieModuleInterface *getDie() { return this; }	// override: BehaviorModuleInterface slot 7 (retail 0x004543A9: the null-checked interface pointer)
	virtual void startRebuildProcess(const ThingTemplate *rebuildTemplate, ObjectID spawnerID);	// override: slot 0 at +0x24 (0x004835B4)
	virtual ObjectID getSpawnerID() const;												// override: slot 1 at +0x24 (folded body 0x001DB0A8)
	virtual ObjectID getReconstructingID() const;										// override: slot 2 at +0x24 (folded body 0x0030F45F)
	virtual const ThingTemplate *getRebuildTemplate() const;							// override: slot 3 at +0x24 (0x002A79A1)
	virtual void setSlot4Flag(bool flag);												// override: slot 4 at +0x24 (0x00210CD3)
	virtual RebuildHoleBehaviorInterface *getRebuildHoleBehaviorInterface();			// primary slot 13, 0x00483322 (own new slot)

	static RebuildHoleBehaviorInterface *getRebuildHoleBehaviorInterfaceFromObject(Object *obj);	// 0x004831CB
	void newWorkerRespawnProcess(Object *oldWorker);									// 0x0048337A

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);
	virtual void loadPostProcess();	// retail slot 0x00497805: the UpdateModule forward, folded with SlowDeathBehavior's (h05)

public:
	ObjectID m_workerID;                           // +0x28; the constructor zeroes it; xfer ObjectID (update looks it up and re-spawns the worker when it is gone)
	ObjectID m_reconstructingID;                   // +0x2C; xfer ObjectID
	ObjectID m_spawnerObjectID;                    // +0x30; xfer ObjectID (getSpawnerID's value)
	UnsignedInt m_workerWaitCounter;               // +0x34; xfer UnsignedInt; newWorkerRespawnProcess sets it from the module data's respawn delay
	const ThingTemplate *m_workerTemplate;         // +0x38; xfer as its name, looked up through the thing factory when loading
	const ThingTemplate *m_rebuildTemplate;        // +0x3C; xfer as its name
	Real m_unk0x40;                                // +0x40; xfer Real (version 3 or later); startRebuildProcess copies Object +0x324 here
	Bool m_slot4Flag;                              // +0x44; xfer Bool; setSlot4Flag's byte
};
// SIZE 0x48
typedef char RebuildHoleBehavior_size[(sizeof(RebuildHoleBehavior) == 0x48) ? 1 : -1];
typedef char RebuildHoleBehavior_off_m_workerID[(offsetof(RebuildHoleBehavior, m_workerID) == 0x28) ? 1 : -1];
typedef char RebuildHoleBehavior_off_m_reconstructingID[(offsetof(RebuildHoleBehavior, m_reconstructingID) == 0x2C) ? 1 : -1];
typedef char RebuildHoleBehavior_off_m_spawnerObjectID[(offsetof(RebuildHoleBehavior, m_spawnerObjectID) == 0x30) ? 1 : -1];
typedef char RebuildHoleBehavior_off_m_workerWaitCounter[(offsetof(RebuildHoleBehavior, m_workerWaitCounter) == 0x34) ? 1 : -1];
typedef char RebuildHoleBehavior_off_m_workerTemplate[(offsetof(RebuildHoleBehavior, m_workerTemplate) == 0x38) ? 1 : -1];
typedef char RebuildHoleBehavior_off_m_rebuildTemplate[(offsetof(RebuildHoleBehavior, m_rebuildTemplate) == 0x3C) ? 1 : -1];
typedef char RebuildHoleBehavior_off_m_unk0x40[(offsetof(RebuildHoleBehavior, m_unk0x40) == 0x40) ? 1 : -1];
typedef char RebuildHoleBehavior_off_m_slot4Flag[(offsetof(RebuildHoleBehavior, m_slot4Flag) == 0x44) ? 1 : -1];

#endif
