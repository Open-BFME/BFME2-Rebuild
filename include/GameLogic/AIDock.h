// AIDock.h: the docking state machine.
// Names: every State and StateMachine ctor passes CRC-32 of its class name as the
// name hash, so retail's own hashes confirm AIDockMachine (0x2B638EB7) and the dock
// states below, each beside its hash; AIDockAdvancePositionState is the exception
// (its ctor 0x00544810 passes the approach state's hash, so its name stays a
// provisional role label for state 2).
// AIDockMachine (SIZE 0x40). Target facts: ctor 0x005448A1 calls
// StateMachine(owner, 0x2B638EB7, false) and defines its states; xfer
// 0x00544A8E: StateMachine::xfer, Version1, Int +0x3C. Dtor 0x005440BC stores
// table 0x00C699A0 and jumps to ~StateMachine. ableToAdvance 0x005440F6 is a
// static transition test (StateTransFuncPtr).
// AIDockWaitForClearanceState (SIZE 0x24, `new 0x24` in the machine ctor):
// onEnter 0x0054420A stores TheGameLogic's frame at +0x20.
// AIDockProcessDockState: onExit 0x00544670 clears the machine's lock byte +0x38.
#pragma once
#ifndef BFME2_AIDOCK_H
#define BFME2_AIDOCK_H

#include "Common/StateMachine.h"
#include "GameLogic/AIStateMachine.h"

// State IDs in the machine ctor 0x005448A1, by the order it defines them (role labels; the
// classes it constructs for each id are named by their CRC hashes).
enum
{
	AI_DOCK_APPROACH = 0,
	AI_DOCK_WAIT_FOR_CLEARANCE,
	AI_DOCK_ADVANCE_POSITION,
	AI_DOCK_MOVE_TO_ENTRY,
	AI_DOCK_MOVE_TO_DOCK,
	AI_DOCK_PROCESS_DOCK,
	AI_DOCK_MOVE_TO_EXIT,
	AI_DOCK_MOVE_TO_RALLY
};

class AIDockMachine : public StateMachine
{
public:
	AIDockMachine(Object *owner);				// 0x005448A1
	virtual ~AIDockMachine();					// 0x005440BC

	static Bool ableToAdvance(State *thisState, void *userData);	// 0x005440F6
	virtual void halt();						// slot 15, 0x005440CD: cancels the dock, then StateMachine::halt

	Int m_approachPosition;						// +0x3C; the move-to-entry state resets it to -1

protected:
	virtual const char *GetSnapshotName() const;	// slot 2, 0x005440C7
	virtual void xfer(Xfer *xfer);				// slot 3, 0x00544A8E
};
// SIZE 0x40

class AIDockWaitForClearanceState : public State
{
public:
	AIDockWaitForClearanceState(StateMachine *machine);	// 0x00544095 (name hash 0xD2719020)
	virtual StateReturnType onEnter();			// 0x0054420A
	virtual StateReturnType update();			// 0x00544218
	virtual void onExit(StateExitType status);	// 0x0054428B

protected:
	virtual const char *GetSnapshotName() const;	// 0x005440B6
	virtual void xfer(Xfer *xfer);				// table slot 3 = 0x00544AB3 (Version1, UnsignedInt +0x20), a body the map files under AIHarvestPrepareSiteState (identical code folded)
	UnsignedInt m_enterFrame;					// +0x20
};
// SIZE 0x24

// Two pointers, owner then drone, as the retail callback and findMyDrone lay them out.
struct DroneInfo
{
	Object *owner;
	Object *drone;
};

// The approach leg of a dock: a MoveTo state that fails when the machine has no goal (retail 0x00544348).
class AIDockApproachState : public AIInternalMoveToState
{
public:
	AIDockApproachState(StateMachine *machine) : AIInternalMoveToState(machine, 0x26F20F3B) {}
	virtual StateReturnType onEnter();			// 0x00544133 (map placeholder Rva005447ED: the ctor 0x005447ED stores table 0x00C69AB0)
	virtual void onExit(StateExitType status);	// 0x005441AF
	virtual StateReturnType update();			// 0x00544348

protected:
	virtual void xfer(Xfer *xfer);				// 0x00341979 (AIStates.cpp): version 1, then the move state's

protected:
	virtual const char *GetSnapshotName() const;	// 0x0054480A
};

// Each state's inline ctor passes its name hash (the advance state reuses the approach
// state's, 0x26F20F3B). The machine ctor 0x005448A1 defines states 0..7 in this order:
// 0 approach (0x005447ED), 1 wait for clearance, 2 advance (0x00544810), 3 move to
// entry (0x0054482D, success 4, failure 6), 4 move to dock (0x0054484A, 5/6), 5 process
// dock, 6 move to exit (0x00544884, 7), 7 move to rally (0x00544867, an AIMoveToState).
// The move-to states keep AIInternalMoveToState's name getter, so no string names them;
// their names come from the CRC-32 name hashes their ctors pass (shown on each ctor).
// Each one drives the goal object's DockUpdateInterface.
// State 2: table 0x00C69B10; its onExit/update slots are the approach state's
// bodies (0x005441AF, 0x00544348), identical code folded together, and its ctor passes
// the approach state's hash 0x26F20F3B. Name provisional (role label for state 2; CRC-32
// of "AIDockAdvancePositionState" would be 0xF52811C5).
class AIDockAdvancePositionState : public AIInternalMoveToState
{
public:
	AIDockAdvancePositionState(StateMachine *machine) : AIInternalMoveToState(machine, 0x26F20F3B) {}
	virtual StateReturnType onEnter();			// 0x005442CC
};

class AIDockMoveToEntryState : public AIInternalMoveToState		// table 0x00C69B58
{
public:
	AIDockMoveToEntryState(StateMachine *machine) : AIInternalMoveToState(machine, 0x58821EDA) {}
	virtual StateReturnType onEnter();			// 0x00544364
	virtual StateReturnType update();			// 0x005443ED
	virtual void onExit(StateExitType status);	// 0x00544425
};

class AIDockMoveToDockState : public AIInternalMoveToState		// table 0x00C69BA0
{
public:
	AIDockMoveToDockState(StateMachine *machine) : AIInternalMoveToState(machine, 0x493559EF) {}
	virtual StateReturnType onEnter();			// 0x00544480
	virtual StateReturnType update();			// 0x0054452A
	virtual void onExit(StateExitType status);	// 0x00544572
};

class AIDockMoveToExitState : public AIInternalMoveToState		// table 0x00C69C30
{
public:
	AIDockMoveToExitState(StateMachine *machine) : AIInternalMoveToState(machine, 0x7EC09A9E) {}
	virtual StateReturnType onEnter();			// 0x0054467A
	virtual StateReturnType update();			// 0x00544707
	virtual void onExit(StateExitType status);	// 0x0054473F
};

// SIZE 0x4C: it derives AIInternalMoveToState; its table's onExit is the folded
// forwarder 0x0034BA5F and its update 0x00349D8D is shared with
// AIMoveOutOfTheWayState's table 0x00C12490.
class AIDockMoveToRallyState : public AIInternalMoveToState		// table 0x00C69BE8
{
public:
	AIDockMoveToRallyState(StateMachine *machine) : AIInternalMoveToState(machine, 0xDAF68697) {}
	virtual StateReturnType onEnter();			// 0x0054477C
	virtual void onExit(StateExitType status);	// slot 5, 0x0034BA5F (folded forwarder)
	virtual StateReturnType update();			// slot 6, 0x00349D8D
};

class AIDockProcessDockState : public State
{
public:
	AIDockProcessDockState(StateMachine *machine) : State(machine, 0x90B65C1D), m_nextDockActionFrame(0), m_droneID(INVALID_ID) {}
	virtual StateReturnType onEnter();			// 0x00544646
	virtual StateReturnType update();			// 0x00544B96
	virtual void onExit(StateExitType status);	// 0x00544670

	void setNextDockActionFrame();				// 0x005445FF
	Object *findMyDrone();						// 0x00544B41

protected:
	virtual const char *GetSnapshotName() const;	// 0x005445F9
	virtual void xfer(Xfer *xfer);				// 0x00544AD2: version 2; the drone ID, then (v2) the next action frame
	UnsignedInt m_nextDockActionFrame;			// +0x20, set by setNextDockActionFrame
	ObjectID m_droneID;							// +0x24, cached by findMyDrone
};
// SIZE 0x28

#endif
