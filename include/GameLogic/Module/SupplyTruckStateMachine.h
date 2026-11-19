// SupplyTruckStateMachine.h: the state machine a SupplyTruckAIUpdate keeps at +0x3E8 (f03, promoted from Draft).
// Target facts: its destructor 0x004A6C59 stores table 0x00C532A8 and tail-jumps to ~StateMachine 0x004D759C, so it derives
// from StateMachine directly (not AIStateMachine); slot 2 is 0x004A6C64 (the map's `name`, a GetSnapshotName body) and
// slot 3 is the shared xfer 0x0033FB0B. The transition tests below are the file statics the state table passes as
// StateTransFuncPtr (they read the owner's AI through the machine owner).
#pragma once
#ifndef BFME2_SUPPLY_TRUCK_STATE_MACHINE_H
#define BFME2_SUPPLY_TRUCK_STATE_MACHINE_H

#include "Common/StateMachine.h"
#include "Common/GameType.h"

class SupplyTruckStateMachine : public StateMachine
{
public:
	SupplyTruckStateMachine(Object *owner);				// 0x004A7010 (the map's COMDAT constructor; WorkerAIUpdate::createMachines 0x004A9ED5 allocates 0x3C bytes for it, i.e. StateMachine's size)
	virtual ~SupplyTruckStateMachine();					// 0x004A6C59

	static bool isForcedIntoWantingState(State *state, void *userData);		// 0x004A6C92
	static bool isForcedIntoBusyState(State *state, void *userData);		// 0x004A6CC2
	static bool ownerDocking(State *state, void *userData);					// 0x004A6CF2
	static bool ownerRva004A6D15(State *state, void *userData);				// 0x004A6D15: the owner is in state 0x2F
	static bool ownerIdle(State *state, void *userData);					// 0x004A6D38
	static bool ownerAvailableForSupplying(State *state, void *userData);	// 0x004A6D5D
	static bool ownerNotDockingOrIdle(State *state, void *userData);		// 0x004A6DA1

protected:
	virtual const char *GetSnapshotName() const;						// slot 2, 0x004A6C64 (table 0x00C532A8 holds it in StateMachine's GetSnapshotName slot)
	virtual void xfer(Xfer *xfer);										// slot 3, ICF fold 0x0033FB0B (StateMachine::xfer, then version 1)
};

// The supply truck's state names match the class CRCs stored by their constructors (the map still
// spells their constructors Rva004A6933, Rva004A6956, Rva004A6979, Rva004A699C, Rva004A6BCA and Rva004A6C13).
class SupplyTruckWantsToPickUpOrDeliverBoxesState : public State
{
public:
	SupplyTruckWantsToPickUpOrDeliverBoxesState(StateMachine *machine);	// 0x004A6933, State(machine, 0xAE77593F)
	virtual StateReturnType update();									// 0x004A7275
protected:
	virtual const char *GetSnapshotName() const;						// 0x004A6950
};

// PROVISIONAL: the supply truck's other states, declared for the members the rows below write (constructors
// 0x004A6956, 0x004A6979 and 0x004A699C each call State(machine, name hash); the three names are the strings
// their GetSnapshotName rows return). Zero Hour has RegroupingState and DockingState under these names;
// HarvestingState is BFME 2's own.
class RegroupingState : public State
{
public:
	virtual StateReturnType onEnter();									// 0x004A732B
protected:
	virtual const char *GetSnapshotName() const;						// 0x004A6973
};

class DockingState : public State
{
public:
	virtual StateReturnType onEnter();									// 0x004A6C6A
protected:
	virtual const char *GetSnapshotName() const;						// 0x004A6996
};

class HarvestingState : public State
{
protected:
	virtual const char *GetSnapshotName() const;						// 0x004A69B9
};

#endif
