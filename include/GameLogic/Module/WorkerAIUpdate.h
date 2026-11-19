// WorkerAIUpdate.h: the worker AI, a dozer that also harvests. SIZE 0x4D0.
//
// Target facts:
//  - The constructor 0x004AA3AB calls AIUpdateInterface's 0x0026E9BD, then stores three interface
//    tables: DozerAIInterface's own 0x00C4B788 (all __purecall) and then WorkerAIUpdate's 0x00C54100
//    at +0x3E4; SupplyTruckAIInterface's 0x00C52EE8 and then 0x00C540B0 at +0x3E8; a one-slot table
//    0x00C1C780 and then 0x00C540AC at +0x3EC. The primary table is 0x00C54190.
//  - The members start at +0x3F0: three task records (an ID, then a frame) as in DozerAIUpdate but
//    at interface +0xC (isTaskPending 0x004A97EE reads [this+task*8+0xC]), the current task at
//    +0x408 (-1 in the constructor, four raw bytes in xfer), and nine dock points of sixteen bytes
//    from +0x40C (a flag, then a Coord3D; getDockPoint 0x004A9875).
//  - xfer 0x004AA1DB (version 2) after AIUpdateInterface::xfer: the three records, a snapshot at
//    +0x4C4, the current task, the nine dock points, four raw bytes at +0x49C, an int at +0x4A0, an
//    ID at +0x4A4, a Coord3D at +0x4A8, bools at +0x4B4/+0x4BC/+0x4BE/+0x4BD, an unsigned at +0x4B8,
//    snapshots at +0x4C8 and +0x4C0 and +0x4CC through TheAudio's slot 88.
// Donor facts: Zero Hour's WorkerAIUpdate derives AIUpdateInterface, DozerAIInterface and
// SupplyTruckAIInterface (and WorkerAIInterface); only the layout above is proven here.
// The provisional WorkerAIInterface has one slot at +0x3EC (0x004AAD69). Its table
// at 0x00C540AC ends where the supply interface table begins at 0x00C540B0.
#pragma once
#ifndef BFME2_WORKERAIUPDATE_H
#define BFME2_WORKERAIUPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "GameLogic/Module/AIUpdate.h"
#include "GameLogic/Module/DozerAIUpdate.h"
#include "GameLogic/Module/SupplyTruckAIInterface.h"
#include "Draft/WorkerAIUpdateModuleData.h"

class Thing;
class ModuleData;
class Object;
class Xfer;
class Snapshot;
class StateMachine;

// PROVISIONAL name (see above).
class WorkerAIInterface
{
public:
	virtual void vslot0() = 0;													// 0, 0x004AAD69
};
typedef char WorkerAIInterface_size[(sizeof(WorkerAIInterface) == 0x4) ? 1 : -1];

class WorkerAIUpdate : public AIUpdateInterface, public DozerAIInterface, public SupplyTruckAIInterface, public WorkerAIInterface	// +0x3E4, +0x3E8, +0x3EC
{
public:
	WorkerAIUpdate(Thing *thing, const ModuleData *moduleData);					// 0x004AA3AB

	// The primary table 0x00854190 returns the dozer, supply and worker interfaces
	// at +0x3E4, +0x3E8 and +0x3EC (slots 92-95 and 99-100).
	virtual const DozerAIInterface *getDozerAIInterface() const { return this; }
	virtual DozerAIInterface *getDozerAIInterface() { return this; }
	virtual const SupplyTruckAIInterface *getSupplyTruckAIInterface() const { return this; }
	virtual SupplyTruckAIInterface *getSupplyTruckAIInterface() { return this; }
	virtual const WorkerAIInterface *getWorkerAIInterface() const { return this; }
	virtual WorkerAIInterface *getWorkerAIInterface() { return this; }

	virtual NameKeyType getModuleNameKey() const;								// 0x004A9B06
	virtual UpdateSleepTime update();											// 0x004AA92B
	virtual void onDelete();													// 0x004AAB0B (shared with DozerAIInterface slot 0)

	// DozerAIInterface
	virtual DozerTask getMostRecentCommand();									// 0x004A9838
	virtual Bool isTaskPending(DozerTask task);									// 0x004A97EE
	virtual ObjectID getTaskTarget(DozerTask task);								// 0x004A97FE
	virtual void newTask(DozerTask task, Object *target);						// 0x004AADB1
	virtual void internalTaskComplete(DozerTask task);							// 0x004A9809
	virtual void internalCancelTask(DozerTask task);							// 0x004AB050
	virtual void internalTaskCompleteOrCancelled(DozerTask task);				// 0x004AAA87
	virtual const Coord3D *getDockPoint(DozerTask task, DozerDockPoint point);	// 0x004A9875
	virtual Bool canAcceptNewRepair(Object *obj);								// 0x004AAA09
	virtual void aiDoCommand(const AICommandParms *parms);						// 0x004AB09D (AICommandInterface slot 0, at +0x20)

	// SupplyTruckAIInterface (+0x3E8; bodies take the interface as this)
	virtual Bool loseOneBox();													// 0x004A98AC
	virtual Bool isForcedIntoWantingState() const;								// 0x004A9BB0
	virtual Bool getHarvestTrees() const;										// 0x004A973B
	virtual Real getHarvestActivationRange() const;								// 0x004A9748
	virtual UnsignedInt getHarvestPreparationTime() const;						// 0x004A9755
	virtual UnsignedInt getHarvestActionTime() const;							// 0x004A9762
	virtual Real getWarehouseScanDistance() const;								// 0x004A9715
	Bool isSupplyTruckBrainActiveAndBusy();										// 0x004A98D6

	virtual void cancelTask(DozerTask task);									// slot 13, 0x004A97D3 (placeholder row, body below)
	virtual Real getBoredTime() const;											// slot 2, 0x004A964C
	virtual Real getBoredRange() const;											// slot 3, 0x004A9656
	virtual Bool getUnknownFlag() const;										// slot 11, 0x004A9B7E
	virtual Bool isCurrentlyFerryingSupplies() const;							// SupplyTruckAIInterface slot 5, 0x004A9617
	virtual void setForceWantingState(Bool settingVal);							// SupplyTruckAIInterface slot 11, 0x004A9BA3
	virtual void setForceBusyState(Bool settingVal);							// SupplyTruckAIInterface slot 13, 0x004A9B8F
	virtual Bool isForcedIntoBusyState() const;									// SupplyTruckAIInterface slot 14, 0x004A9B9C
	virtual void unknownNotify17(Int arg);										// SupplyTruckAIInterface slot 17, 0x004A9BD1 (stores +0x4B8)
	virtual UnsignedInt getActionDelayForDock(Object *dock);					// SupplyTruckAIInterface slot 19, 0x004A9660
	virtual void finishBuildingSound();											// DozerAIInterface slot 24, 0x004A99F6

protected:
	virtual void privateDock(Object *obj, CommandSourceType cmdSource);				// 51, 0x004A9774
	virtual void privateHarvest(const Coord3D *pos, CommandSourceType cmdSource);	// 52, 0x004A979F
	virtual void privateRepair(Object *obj, CommandSourceType cmdSource);			// 44, 0x004A9BDE
	virtual void privateResumeConstruction(Object *obj, CommandSourceType cmdSource);	// 45, the same body as DozerAIUpdate's 0x00488B1A (folded)
	Bool findGoodBuildOrRepairPosition(const Object *me, const Object *target, Coord3D &pos);	// 0x004AA4FE
	Object *findGoodBuildOrRepairPositionAndTarget(Object *me, Object *target, Coord3D &pos);	// 0x004AABB4

	void resetSupplyTruckMachine();												// 0x004A9914 (role name): the +0x4C8 machine back to its default state
	void resetDozerMachine();													// 0x004A991F (role name): the +0x4C4 machine back to its default state
	const WorkerAIUpdateModuleData *getWorkerAIUpdateModuleData() const { return static_cast<const WorkerAIUpdateModuleData *>(getModuleData()); }	// inline

private:
	void createMachines();														// 0x004A9ED5 (the map spells it private: AAE)

protected:
	virtual const char *GetSnapshotName() const;								// 0x004A9B4B
	virtual void xfer(Xfer *xfer);												// 0x004AA1DB

public:
	DozerTaskRecord m_tasks[3];													// +0x3F0, +0x3F8, +0x400 (interface +0xC)
	DozerTask m_currentTask;													// +0x408, -1 in the constructor
	DozerDockPointRecord m_dockPoints[3][3];									// +0x40C, [task][point]
	Int m_unk0x49C;																// +0x49C, four raw bytes in xfer
	Int m_unk0x4A0;																// +0x4A0, xfer'd as an int
	ObjectID m_unk0x4A4;														// +0x4A4, xfer'd through XferObjectID
	Coord3D m_unk0x4A8;															// +0x4A8, xfer'd through the Coord3DBase slot
	Bool m_unk0x4B4;															// +0x4B4
	UnsignedInt m_unk0x4B8;														// +0x4B8, the last transfer (unsigned)
	Bool m_unk0x4BC;															// +0x4BC
	Bool m_unk0x4BD;															// +0x4BD
	Bool m_unk0x4BE;															// +0x4BE
	StateMachine *m_unk0x4C0;														// +0x4C0, xfer'd as a Snapshot
	StateMachine *m_unk0x4C4;														// +0x4C4, the first Snapshot xfer'd
	StateMachine *m_unk0x4C8;														// +0x4C8, xfer'd after the dock points
	Int m_buildingSoundHandle;													// +0x4CC, 1 in the constructor; TheAudio's slot 88
};
// SIZE 0x4D0
typedef char WorkerAIUpdate_off_m_tasks[(offsetof(WorkerAIUpdate, m_tasks) == 0x3F0) ? 1 : -1];
typedef char WorkerAIUpdate_off_m_dockPoints[(offsetof(WorkerAIUpdate, m_dockPoints) == 0x40C) ? 1 : -1];
typedef char WorkerAIUpdate_off_m_unk0x4C0[(offsetof(WorkerAIUpdate, m_unk0x4C0) == 0x4C0) ? 1 : -1];
typedef char WorkerAIUpdate_size[(sizeof(WorkerAIUpdate) == 0x4D0) ? 1 : -1];

#endif
