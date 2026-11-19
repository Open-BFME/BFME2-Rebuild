// DozerStates.h: the dozer's state classes, BFME 2 layout (DozerAIUpdate.cpp defines them).
//
// Evidence, per class: each constructor stores the CRC-32 of its class name through State's
// 0x004D73FC (the same method used in AIStates.h),
// the table pointer, then the members; sizes come from the operator new calls in
// DozerActionStateMachine's constructor 0x004885DE and DozerPrimaryStateMachine's 0x00488DAB;
// member offsets and types from each state's xfer (Xfer table: 0x24 raw bytes, 0x78 UnsignedInt,
// 0x7C Int, 0x90 Bool, 0x30 Snapshot). Zero Hour's DozerAIUpdate.cpp is the name donor.
// Table slot 2 (GetSnapshotName) of every state returns the string stored right after the table.
#pragma once
#ifndef BFME2_DOZERSTATES_H
#define BFME2_DOZERSTATES_H

#include "Common/StateMachine.h"
#include "GameLogic/Module/DozerAIUpdate.h"
#include "Common/Xfer.h"

class Object;
class Xfer;

// Dozer's primary machine (DozerPrimaryStateMachine's constructor 0x00488DAB defines them in this
// order; the idle state's transitions come from the table at VA 0x00C4B808).
enum DozerPrimaryStateID
{
	DOZER_PRIMARY_IDLE,
	DOZER_PRIMARY_BUILD,
	DOZER_PRIMARY_REPAIR,
	DOZER_PRIMARY_FORTIFY,
	DOZER_PRIMARY_GO_HOME
};

// Dozer's action sub-machine (DozerActionStateMachine's constructor 0x004885DE).
enum DozerActionType
{
	DOZER_ACTION_PICK_ACTION_POS,
	DOZER_ACTION_MOVE_TO_ACTION_POS,
	DOZER_ACTION_DO_ACTION
};

// Dozer picks a position around the target to do the action. SIZE 0x28.
class DozerActionPickActionPosState : public State
{
public:
	DozerActionPickActionPosState(StateMachine *machine, Int task) : State(machine, 0xAA55FE2F)	// 0x004884ED (CRC-32 0xAA55FE2F)
	{
		m_failedAttempts = 0;
		m_task = task;
	}
	virtual StateReturnType update();										// 0x0048A4DD

protected:
	virtual const char *GetSnapshotName() const;							// 0x00488515
	virtual void xfer(Xfer *xfer);											// 0x004892B5

	Int m_task;																// +0x20
	Int m_failedAttempts;													// +0x24
};
// SIZE 0x28

// Dozer moves to the action position. SIZE 0x24.
class DozerActionMoveToActionPosState : public State
{
public:
	DozerActionMoveToActionPosState(StateMachine *machine, Int task) : State(machine, 0xF0395B0E)	// 0x0048851B (CRC-32 0xF0395B0E)
	{
		m_task = task;
	}
	virtual StateReturnType update();										// 0x004892E1

protected:
	virtual const char *GetSnapshotName() const;							// 0x0048853F
	virtual void xfer(Xfer *xfer);											// shared with the other task-only xfers

	Int m_task;																// +0x20
};
// SIZE 0x24

// Dozer does the action. SIZE 0x28.
class DozerActionDoActionState : public State
{
public:
	DozerActionDoActionState(StateMachine *machine, Int task) : State(machine, 0xBCA9F972)	// 0x00488545 (CRC-32 0xBCA9F972)
	{
		m_enterFrame = 0;
		m_task = task;
	}
	virtual StateReturnType onEnter();										// 0x00488573
	virtual void onExit(StateExitType status);								// 0x004885B8
	virtual StateReturnType update();										// 0x0048A69E

protected:
	virtual const char *GetSnapshotName() const;							// 0x0048856D
	virtual void xfer(Xfer *xfer);											// 0x0048947D

	Int m_task;																// +0x20
	UnsignedInt m_enterFrame;												// +0x24
};
// SIZE 0x28

// The sub-machine one DozerActionState owns. SIZE 0x40.
class DozerActionStateMachine : public StateMachine
{
public:
	DozerActionStateMachine(Object *owner, Int task);						// 0x004885DE (CRC-32 0x253E8923)
	virtual ~DozerActionStateMachine();										// 0x00488D1F (deleting)

protected:
	virtual const char *GetSnapshotName() const;							// 0x004886C1
	virtual void xfer(Xfer *xfer);											// 0x004894A9

	Int m_task;																// +0x3C
};
// SIZE 0x40

// Dozer idles; after a while it looks for something to repair. SIZE 0x2C.
class DozerPrimaryIdleState : public State
{
public:
	DozerPrimaryIdleState(StateMachine *machine) : State(machine, 0xC618E42C)	// 0x004886C7 (CRC-32 0xC618E42C)
	{
		m_idleTooLongTimestamp = 0;
		m_idlePlayerNumber = 0;
		m_isMarkedAsIdle = false;
	}
	virtual StateReturnType onEnter();										// 0x0048872D
	virtual void onExit(StateExitType status);								// 0x0048873F
	virtual StateReturnType update();										// 0x0048876B

protected:
	virtual const char *GetSnapshotName() const;							// 0x004886EF
	virtual void xfer(Xfer *xfer);											// 0x004886F5

	UnsignedInt m_idleTooLongTimestamp;										// +0x20
	Int m_idlePlayerNumber;													// +0x24
	Bool m_isMarkedAsIdle;													// +0x28
};
// SIZE 0x2C

// The state the primary machine enters for one task: it owns a DozerActionStateMachine. SIZE 0x28.
// Constructor 0x00488883 (CRC-32 0x473A607F, table 0x00C4B5C8), update 0x004888F8 forwards to the
// sub-machine's updateStateMachine, onEnter 0x00488906, onExit 0x0048893C, xfer 0x004894CA.
class DozerActionState : public State
{
public:
	DozerActionState(StateMachine *machine, Int task);						// 0x00488883
	virtual ~DozerActionState();											// 0x00488D40 (deleting), body 0x00488D5C
	virtual StateReturnType onEnter();										// 0x00488906
	virtual void onExit(StateExitType status);								// 0x0048893C
	virtual StateReturnType update();										// 0x004888F8
	virtual const char *GetSnapshotName() const;							// 0x00488900 (public: the map spells it UBE)

protected:
	virtual void xfer(Xfer *xfer);											// 0x004894CA

public:
	Int m_task;																// +0x20
	DozerActionStateMachine *m_actionMachine;								// +0x24
};
// SIZE 0x28
typedef char DozerActionState_off_m_task[(offsetof(DozerActionState, m_task) == 0x20) ? 1 : -1];
typedef char DozerActionState_size[(sizeof(DozerActionState) == 0x28) ? 1 : -1];

// The state the dozer goes home in: nothing to transfer, always fails. SIZE 0x20.
class DozerPrimaryGoingHomeState : public State
{
public:
	DozerPrimaryGoingHomeState(StateMachine *machine) : State(machine, 0x69EAAB49) {}	// 0x0048896B (CRC-32 0x69EAAB49)
	virtual StateReturnType update() { return STATE_FAILURE; }			// 0x00488988 (the map calls the shared body AIGoingIdleState::update)

protected:
	virtual const char *GetSnapshotName() const { return 0; }				// slot 2: the shared `xor eax,eax` 0x000D43D0
	virtual void xfer(Xfer *xfer) {}										// slot 3: the shared empty body 0x0047A69C
};
// SIZE 0x20

// The dozer's primary machine. SIZE 0x3C.
class DozerPrimaryStateMachine : public StateMachine
{
public:
	DozerPrimaryStateMachine(Object *owner);								// 0x00488DAB (CRC-32 0x3EA7DE5F)
	virtual ~DozerPrimaryStateMachine();									// 0x0048898C

	static bool isBuildMostImportant(State *state, void *userData);			// 0x0048899D
	static bool isRepairMostImportant(State *state, void *userData);		// 0x004889E8
	static bool isFortifyMostImportant(State *state, void *userData);		// 0x00488A34

protected:
	virtual const char *GetSnapshotName() const;							// 0x00488997
	virtual void xfer(Xfer *xfer) { StateMachine::xfer(xfer); xfer->Version1(); }	// slot 3: the shared version-only override 0x0033FB0B
};
// SIZE 0x3C

#endif
