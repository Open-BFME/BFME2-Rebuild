// DozerAIUpdate.h: the worker AI that builds and repairs structures. SIZE 0x4A8.
//
// Target facts:
//  - The constructor 0x004894F5 calls AIUpdateInterface's 0x0026E9BD, then stores the primary
//    table 0x00C4B848 at +0x0, 0x00BFA3C0 at +0xC, 0x00C4B83C at +0x10, 0x00C4B838 at +0x20 and
//    0x00C52F88 at +0x24, and a second object's table 0x00C4BAB0 at +0x3E4 (it first stores that
//    interface's own table 0x00C4B788, which is all __purecall). The interface runs from +0x3E4
//    to +0x400; the class's own members follow it up to 0x4A8.
//  - The interface at +0x3E4 has 32 slots (the table 0x00C4B788 is 31 __purecall slots and one
//    empty body, slot 31). DozerAIUpdate's table 0x00C4BAB0 points slots 0 and 4 at thunks
//    (`sub ecx,0x3E4; jmp`) to onDelete 0x00489CA1 and construct 0x00488F52, so those two are
//    virtuals both bases share; every other slot's body takes the interface as `this` (it reads the
//    module data at this-0x3E0, the object at this-0x3DC and its own members from this+4).
//  - Task records: eight bytes each at +0x3E8, interface +4 (isTaskPending 0x00488B54, getTaskTarget
//    0x00488B83): the target object's ID, then the frame the task started (getMostRecentCommand
//    0x00488BBD picks the pending task with the largest second dword). Each task also owns three
//    dock points of sixteen bytes (a flag, then a Coord3D) at +0x410 (getDockPoint 0x00488BFA).
//  - xfer 0x00489671 (version 2) after AIUpdateInterface::xfer: three task records (ID, uint), the
//    state machine as a Snapshot, four raw bytes at +0x404, the nine dock points, four raw bytes at
//    +0xA0 of the interface, +0x408 through a global's slot 0x160, a bool at +0x40C, an ID at +0x4A4.
//  - Interface slots from the map's DozerAIUpdate rows: 5 getMostRecentCommand, 6 isTaskPending, 7
//    getTaskTarget, 8 isAnyTaskPending, 12 newTask, 13 cancelTask, 14 internalTaskComplete, 15
//    internalCancelTask, 16 internalTaskCompleteOrCancelled, 17 getDockPoint, 20 canAcceptNewRepair,
//    22 removeBridgeScaffolding, 25 createPhantomStructure, 26 makePhantomStructureReal, 27
//    destroyPhantomStructure, 28/29 0x00489DD1 (the phantom structure, looked up by ID), 31
//    notifyConstructionComplete. Slots 1-3 return the module data's three INI values (map rows
//    0x00488C34, 0x00488C3E, 0x00488C48 are still placeholders), 9/10 get and set the current
//    task (+0x404), 11 reads the byte at +0x40C, 18/19 set and get the word at +0x4A0.
// Donor facts: Zero Hour's DozerAIInterface supplies the interface name; its method order agrees with
// the first rows but not past them, so the unproven slots keep role names.
// PROVISIONAL: DozerAIInterface (the class AIUpdate.h forward-declares).
#pragma once
#ifndef BFME2_DOZERAIUPDATE_H
#define BFME2_DOZERAIUPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "GameLogic/Module/AIUpdate.h"
#include "Common/StateMachine.h"

class Thing;
class ModuleData;
class Object;
class Player;
class ThingTemplate;
class Xfer;
class AudioEventRTS;
class Snapshot;
class StateMachine;

// The three tasks, in the order the primary state machine tries them (its isBuildMostImportant,
// isRepairMostImportant and isFortifyMostImportant statics are the proof of the names).
enum DozerTask
{
	DOZER_TASK_INVALID = -1,									// the constructor's initial current task ('or [esi+0x404],-1')
	DOZER_TASK_BUILD = 0,
	DOZER_TASK_REPAIR = 1,
	DOZER_TASK_FORTIFY = 2
};

// The argument of slot 18 (the map's WorkerAIUpdate row 0x002B21F6 is `setBuildSubTask(DozerBuildSubTask)`);
// its enumerators are not recovered, the interface stores it in the word at +0x4A0 / +0x4BC.
enum DozerBuildSubTask
{
	DOZER_BUILD_SUBTASK_UNKNOWN = 0
};

// The second index of getDockPoint 0x00488BFA (both indices are range-checked against 3).
enum DozerDockPoint
{
	DOZER_DOCK_POINT_START = 0,
	DOZER_DOCK_POINT_ACTION = 1,
	DOZER_DOCK_POINT_END = 2
};

struct DozerTaskRecord
{
	ObjectID m_targetObjectID;		// +0x0: nonzero while the task is pending
	UnsignedInt m_startFrame;		// +0x4: getMostRecentCommand compares it
};

struct DozerDockPointRecord
{
	Bool m_valid;					// +0x0
	Coord3D m_position;				// +0x4
};
typedef char DozerDockPointRecordSize[(sizeof(DozerDockPointRecord) == 0x10) ? 1 : -1];

// PROVISIONAL name (see above). Size 4: the table pointer.
class DozerAIInterface
{
public:
	virtual void onDelete() = 0;												// 0, thunk 0x00489EAC to 0x00489CA1
	virtual Real getRepairHealthPercentPerSecond() const = 0;					// 1, 0x00488C34 (module data +0x64)
	virtual Real getBoredTime() const = 0;										// 2, 0x00488C3E (module data +0x68)
	virtual Real getBoredRange() const = 0;										// 3, 0x00488C48 (module data +0x6C, scaled when the owner qualifies)
	virtual Object *construct(const ThingTemplate *what, const Coord3D *pos, Real angle,
		Player *owningPlayer, Bool isRebuild, Int unknown) = 0;					// 4, thunk 0x0048963C to 0x00488F52
	virtual DozerTask getMostRecentCommand() = 0;								// 5, 0x00488BBD
	virtual Bool isTaskPending(DozerTask task) = 0;								// 6, 0x00488B54
	virtual ObjectID getTaskTarget(DozerTask task) = 0;							// 7, 0x00488B83
	virtual Bool isAnyTaskPending() = 0;										// 8, 0x00488B64
	virtual DozerTask getCurrentTask() const = 0;								// 9, 0x0030D377 (role name; word at +0x404)
	virtual void setCurrentTask(DozerTask task) = 0;							// 10, 0x00317B9B (role name)
	virtual Bool getUnknownFlag() const = 0;									// 11, 0x003FE57D (role name; byte at +0x40C)
	virtual void newTask(DozerTask task, Object *target) = 0;					// 12, 0x00489A07
	virtual void cancelTask(DozerTask task) = 0;								// 13, 0x0048AE8A
	virtual void internalTaskComplete(DozerTask task) = 0;						// 14, 0x00488B8E
	virtual void internalCancelTask(DozerTask task) = 0;						// 15, 0x0048AEC6
	virtual void internalTaskCompleteOrCancelled(DozerTask task) = 0;			// 16, 0x00489647
	virtual const Coord3D *getDockPoint(DozerTask task, DozerDockPoint point) = 0;	// 17, 0x00488BFA
	virtual void setBuildSubTask(DozerBuildSubTask subTask) = 0;				// 18, 0x0048962F (word at +0x4A0)
	virtual DozerBuildSubTask getBuildSubTask() = 0;							// 19, 0x0052AF70
	virtual Bool canAcceptNewRepair(Object *obj) = 0;							// 20, 0x00489895
	virtual void createBridgeScaffolding(Object *bridgeTower) = 0;				// 21, 0x004AAB73 (shared by DozerAIUpdate's and WorkerAIUpdate's tables; the map spells it DozerAIUpdate's)
	virtual void removeBridgeScaffolding(Object *bridgeTower) = 0;				// 22, 0x004897BF
	virtual void startBuildingSound(const AudioEventRTS *sound, Int constructionSiteID) = 0;	// 23, 0x004891DE (Worker's 0x004A9E3E takes (const AudioEventRTS *, Int))
	virtual void finishBuildingSound() = 0;										// 24, 0x004550DE
	virtual void createPhantomStructure(const ThingTemplate *what, Player *owningPlayer,
		const Coord3D *pos, Real angle) = 0;									// 25, 0x00489F7D
	virtual void makePhantomStructureReal() = 0;								// 26, 0x0048A15A
	virtual void destroyPhantomStructure() = 0;									// 27, 0x00489D09
	virtual Object *getPhantomStructure() = 0;									// 28, 0x00489DD1 (role name)
	virtual Object *getPhantomStructure2() = 0;									// 29, the same body (role name)
	virtual Int vslot30() = 0;													// 30, 0x00488C90 (map: Rva00488C90; the phantom's float at +0x324 as an int)
	virtual void notifyConstructionComplete() {}								// 31, 0x000B3FD0 here; DozerAIUpdate's is 0x00488CC4
};
// SIZE 0x4 (the table pointer only: WorkerAIUpdate's interface at the same offset has its task records at
// +0xC, so the records are the implementing class's members, not the interface's)
typedef char DozerAIInterface_size[(sizeof(DozerAIInterface) == 0x4) ? 1 : -1];

// The dozer's state classes (DozerActionState and the machines) are in GameLogic/DozerStates.h.

class DozerAIUpdate : public AIUpdateInterface, public DozerAIInterface		// +0x3E4
{
public:
	DozerAIUpdate(Thing *thing, const ModuleData *moduleData);					// 0x004894F5

	// AIUpdateInterface slots 92 and 93 return the dozer interface at +0x3E4.
	virtual const DozerAIInterface *getDozerAIInterface() const { return this; }
	virtual DozerAIInterface *getDozerAIInterface() { return this; }

	virtual NameKeyType getModuleNameKey() const;								// 0x004895E4
	virtual UpdateSleepTime update();											// 0x00489800
	virtual void onDelete();													// 0x00489CA1 (shared with DozerAIInterface slot 0)
	virtual Object *construct(const ThingTemplate *what, const Coord3D *pos, Real angle,
		Player *owningPlayer, Bool isRebuild, Int unknown);						// 0x00488F52 (AIUpdateInterface slot 126 and interface slot 4)

	// DozerAIInterface
	virtual DozerTask getMostRecentCommand();									// 0x00488BBD
	virtual Bool isTaskPending(DozerTask task);									// 0x00488B54
	virtual ObjectID getTaskTarget(DozerTask task);								// 0x00488B83
	virtual Bool isAnyTaskPending();											// 0x00488B64
	virtual void newTask(DozerTask task, Object *target);						// 0x00489A07
	virtual void cancelTask(DozerTask task);									// 0x0048AE8A
	virtual void internalTaskComplete(DozerTask task);							// 0x00488B8E
	virtual void internalCancelTask(DozerTask task);							// 0x0048AEC6
	virtual void internalTaskCompleteOrCancelled(DozerTask task);				// 0x00489647
	virtual const Coord3D *getDockPoint(DozerTask task, DozerDockPoint point);	// 0x00488BFA
	virtual Bool canAcceptNewRepair(Object *obj);								// 0x00489895
	virtual void removeBridgeScaffolding(Object *bridgeTower);					// 0x004897BF
	virtual void createPhantomStructure(const ThingTemplate *what, Player *owningPlayer,
		const Coord3D *pos, Real angle);										// 0x00489F7D
	virtual void makePhantomStructureReal();									// 0x0048A15A
	virtual void vslot147();												// AIUpdateInterface slot 147, 0x00488CF0
	virtual void destroyPhantomStructure();										// 0x00489D09
	virtual void notifyConstructionComplete();									// 0x00488CC4
	virtual Real getRepairHealthPercentPerSecond() const;						// 0x00488C34
	virtual Real getBoredTime() const;											// 0x00488C3E
	virtual Real getBoredRange() const;											// 0x00488C48
	virtual DozerTask getCurrentTask() const;									// 0x0030D377 (shared body)
	virtual void setCurrentTask(DozerTask task);								// 0x00317B9B (shared body)
	virtual Bool getUnknownFlag() const;										// 0x003FE57D (shared body)
	virtual void setBuildSubTask(DozerBuildSubTask subTask);					// 0x0048962F
	virtual DozerBuildSubTask getBuildSubTask();								// 0x0052AF70 (shared body)
	virtual void createBridgeScaffolding(Object *bridgeTower);					// 0x004AAB73
	virtual void startBuildingSound(const AudioEventRTS *sound, Int constructionSiteID);	// 0x004891DE
	virtual void finishBuildingSound();											// 0x004550DE (shared body)
	virtual Object *getPhantomStructure();										// 0x00489DD1
	virtual Object *getPhantomStructure2();										// 0x00489DD1 (folded with the above)
	virtual Int vslot30();														// 0x00488C90

	void makePhantomStructureInert();											// 0x00489DE3
	void makePhantomStructureNotInert();										// 0x00489E2E

protected:
	virtual void privateRepair(Object *obj, CommandSourceType cmdSource);			// 44, 0x00488AC9
	virtual void privateResumeConstruction(Object *obj, CommandSourceType cmdSource);	// 45, 0x00488B1A (also WorkerAIUpdate's slot 45)
	virtual ~DozerAIUpdate();													// 0x00489EB7 (deleting 0x0048A39D)
	virtual void loadPostProcess();												// 0x00489E91
	virtual const char *GetSnapshotName() const;								// 0x00489629
	virtual void xfer(Xfer *xfer);												// 0x00489671

protected:
	Bool findGoodBuildOrRepairPosition(const Object *me, const Object *target, Coord3D &pos);	// 0x00489039
	Object *findGoodBuildOrRepairPositionAndTarget(Object *me, Object *target, Coord3D &pos);	// 0x00489913

private:
	void createMachines();														// 0x00488EFC

public:

public:
	DozerTaskRecord m_tasks[3];													// +0x3E8, +0x3F0, +0x3F8 (interface +4)
	StateMachine *m_dozerMachine;												// +0x400, the primary state machine, xfer'd as a Snapshot
	DozerTask m_currentTask;													// +0x404, -1 in the constructor; four raw bytes in xfer
	Int m_buildingSoundHandle;													// +0x408, 1 in the constructor; xfer passes it to TheAudio's slot 88 (xferAudioHandle)
	Bool m_unk0x40C;															// +0x40C, 0 in the constructor
	DozerDockPointRecord m_dockPoints[3][3];									// +0x410, [task][point]
	Int m_buildSubTask;															// +0x4A0 (setBuildSubTask / getBuildSubTask), four raw bytes in xfer
	ObjectID m_phantomStructureID;												// +0x4A4, the ID xfer'd last
};
// SIZE 0x4A8
typedef char DozerAIUpdate_off_m_tasks[(offsetof(DozerAIUpdate, m_tasks) == 0x3E8) ? 1 : -1];
typedef char DozerAIUpdate_off_m_dozerMachine[(offsetof(DozerAIUpdate, m_dozerMachine) == 0x400) ? 1 : -1];
typedef char DozerAIUpdate_off_m_dockPoints[(offsetof(DozerAIUpdate, m_dockPoints) == 0x410) ? 1 : -1];
typedef char DozerAIUpdate_off_m_phantomStructureID[(offsetof(DozerAIUpdate, m_phantomStructureID) == 0x4A4) ? 1 : -1];
typedef char DozerAIUpdate_size[(sizeof(DozerAIUpdate) == 0x4A8) ? 1 : -1];

#endif
