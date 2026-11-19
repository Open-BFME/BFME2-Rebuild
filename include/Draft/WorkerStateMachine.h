// WorkerStateMachine: the worker's own state machine (WorkerAIUpdate::createMachines 0x004A9ED5 allocates 0x3C bytes).
// Target facts: its vtable (slot 0 deleting dtor 0x004A9E22, slot 1 the empty 0x000B3FD0, slot 2 GetSnapshotName 0x004A996F,
// slot 3 the shared version-only xfer 0x0033FB0B, slots 4..15 StateMachine's own) is StateMachine's with those four overridden;
// the destructor 0x004A9964 stores the table and jumps to ~StateMachine 0x004D759C (11 bytes, as ~AIAttackSwoopThenIdleStateMachine).
#pragma once
#include "Common/StateMachine.h"
#include "Common/Xfer.h"
class Object;
class State;

class WorkerStateMachine : public StateMachine
{
public:
	// The transition tests the machine's states use to decide when the supply-truck sub-machine runs.
	static bool supplyTruckSubMachineWantsToEnter(State *state, void *userData); // 0x004A9975
	static bool supplyTruckSubMachineReadyToLeave(State *state, void *userData); // 0x004A99BB
	WorkerStateMachine(Object *owner); // 0x004A9D79 (the map's COMDAT constructor)
	virtual ~WorkerStateMachine(); // 0x004A9964 (deleting 0x004A9E22)
protected:
	virtual const char *GetSnapshotName() const; // 0x004A996F
	virtual void xfer(Xfer *xfer) { StateMachine::xfer(xfer); xfer->Version1(); }	// slot 3: the shared version-only override 0x0033FB0B
};
// SIZE 0x3C
typedef char WorkerStateMachine_size[(sizeof(WorkerStateMachine) == 0x3C) ? 1 : -1];
