// StateMachine.h: SAGE State and StateMachine, BFME 2 layout.
//
// State (SIZE 0x20). Target facts: constructor 0x004D73FC (out of line, ret 8)
// stores INVALID_STATE_ID (999999) at +4/+8/+0xC, the machine at +0x18, zero
// at +0x10/+0x14 and the byte +0x1C, and table 0x00C605D8. Its second argument
// is a name hash (callers push e.g. 0xD405C261) that release builds ignore.
// Table 0x00C605D8, 17 slots: 0 deleting dtor (fold 0x004A10FD), 1 empty
// (0x000B3FD0), 2/3 __purecall (Snapshot), 4 `xor eax,eax` (onEnter returns
// STATE_CONTINUE), 5 `ret 4` (onExit), 6 __purecall (update), 7 empty,
// 8-15 `xor al,al` (false), 16 empty.
// Slot names: 8 isAttack (StateMachine::isInAttackState 0x002621B8 calls +0x20;
// AIGuardState::isAttack 0x00341D81 overrides it), 9 isGuardIdle
// (isInGuardIdleState 0x002621C9 calls +0x24), 10 isBusy (isInBusyState
// 0x004D7309 calls +0x28). Slot 7 (role-inferred): its 54 overrides are all
// 0x0033F2D7, which clears the move states' waiting-for-path byte +0x49.
// Slots 11-15 are bool queries whose names are not recovered: only
// 0x002621DA/0x002621EB/0x002621FC/0x0026220D (StateMachine wrappers, no
// recovered name either) ask slots 11-14. Slot 16 is asked by
// StateMachine 0x004D7395 (machine slot 13).
// Transitions: rva004D7127 (setTransitions) stores a StateConditionInfo array
// at +0x10 and its count at +0x14 (12-byte records, test function first).
//
// StateMachine (SIZE 0x3C). Target facts: constructor 0x004D79E1
// (Object *owner, UnsignedInt name hash, Bool) stores table 0x00C60740,
// current state +4 = 0, the STLport map at +8 (0x0033C432), owner +0x14,
// goal object +0x20 = 0, default state +0x1C = 999999, goal position
// +0x24..+0x2C = 0, goal range +0x30 = the constant 0x00BBB8E0, +0x34 = 0,
// lock +0x38 and +0x39 = false and the Bool argument at +0x3A. Destructor
// 0x004D759C (out of line): current state onExit(EXIT_RESET), delete every
// state in the map, clear and destroy the map, restore Snapshot.
// Table 0x00C60740, 16 slots: 0 dtor, 1 empty loadPostProcess, 2
// GetSnapshotName 0x00341456, 3 xfer 0x004D7744, 4 updateStateMachine, 5 clear,
// 6 resetToDefaultState, 7 initDefaultState, 8 setState, 9/10 return 0 (both
// are factories: AIStateMachine's slot 9 0x0034BA20 returns a new 0x3C-byte
// machine, slot 10 0x0034145C a new AIGuardMachine; names role-inferred),
// 11 isInAttackState, 12 isInBusyState, 13 0x004D7395, 14 setGoalObject, 15 halt.
// Members: sleep frame +0x18 (internalSetState 0x004D766B), goal position and
// range (internalSetGoalPosition 0x004D73C4), goal object ID from Object +0x74
// (internalSetGoalObject 0x004D7435), a second object ID at +0x34
// (0x004D73AD/0x004D7427, the same locked/internal pair shape), lock +0x38
// (lock 0x0026221D). Zero Hour's StateMachine.h is the name donor.
#pragma once
#ifndef BFME2_STATEMACHINE_H
#define BFME2_STATEMACHINE_H

#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "Common/Snapshot.h"
#include "Common/StlMap.h"

class Object;
class State;
class StateMachine;
class Xfer;

typedef UnsignedInt StateID;

enum
{
	EXIT_MACHINE_WITH_SUCCESS = 9998,	// 0x270E, AIDockMachine 0x005448A1 passes it as the last state's success
	EXIT_MACHINE_WITH_FAILURE = 9999,	// 0x270F, the usual failure target
	INVALID_STATE_ID = 999999	// 0xF423F, State/StateMachine constructors
};

enum StateReturnType
{
	STATE_CONTINUE = 0,
	STATE_SUCCESS = -1,
	STATE_FAILURE = -2,
	STATE_SLEEP_FOREVER = 0x3FFFFFFF	// SleepState::onEnter/update 0x004A6563 return it
};

enum StateExitType
{
	EXIT_NORMAL,
	EXIT_RESET		// ~StateMachine 0x004D759C passes 1
};

// A transition test (AIDockMachine::ableToAdvance 0x005440F6 is one).
typedef Bool (*StateTransFuncPtr)(State *state, void *userData);

// SIZE 0xC: rva004D7127 walks the array in 12-byte steps until test is null.
struct StateConditionInfo
{
	StateTransFuncPtr test;		// +0x0
	StateID toStateID;			// +0x4
	void *userData;				// +0x8
};

class State : public Snapshot
{
public:
	State(StateMachine *machine, UnsignedInt name);		// 0x004D73FC
	virtual ~State();											// slot 0; out of line: the deleting dtor fold 0x004A10FD and
														// ~GiantBirdAttackMoveToState 0x0036783F call it, and its
														// body folds into the 7-byte Snapshot restore 0x0049B47C

	virtual StateReturnType onEnter() { return STATE_CONTINUE; }	// slot 4
	virtual void onExit(StateExitType status);				// slot 5, 0x0047A69C (the shared `ret 4`; the map accepts ?onExit@State@@UAEXW4StateExitType@@@Z there since publish 5)
	virtual StateReturnType update() = 0;					// slot 6
	virtual void clearWaitingForPath() {}					// slot 7
	virtual Bool isAttack() const { return false; }			// slot 8
	virtual Bool isGuardIdle() const { return false; }		// slot 9
	virtual Bool isBusy() const { return false; }			// slot 10
	virtual Bool unknownQuery11() const { return false; }	// slot 11
	virtual Bool unknownQuery12() const { return false; }	// slot 12
	virtual Bool unknownQuery13() const { return false; }	// slot 13
	virtual Bool unknownQuery14() const { return false; }	// slot 14
	virtual Bool unknownQuery15() const { return false; }	// slot 15
	virtual void unknownNotify16() {}						// slot 16

	StateID getID() const { return m_ID; }
	StateMachine *getMachine() const { return m_machine; }
	Object *getMachineOwner() const;
	Object *getMachineGoalObject();							// 0x0033F263

	void rva004D7127(const StateConditionInfo *conditions);	// 0x004D7127
	StateReturnType friend_checkForTransitions(StateReturnType status);		// 0x004D7148
	StateReturnType friend_checkForSleepTransitions(StateReturnType status);	// 0x004D722A

protected:
	virtual void loadPostProcess() {}						// slot 1

	friend class StateMachine;

	StateID m_ID;									// +0x04
	StateID m_successStateID;						// +0x08
	StateID m_failureStateID;						// +0x0C
	const StateConditionInfo *m_transitions;		// +0x10
	Int m_transitionCount;							// +0x14
	StateMachine *m_machine;						// +0x18
	Bool m_unk0x1C;									// +0x1C, constructor stores false
};
// SIZE 0x20

class StateMachine : public Snapshot
{
public:
	StateMachine(Object *owner, UnsignedInt name, Bool flag);	// 0x004D79E1
	virtual ~StateMachine();									// 0x004D759C

	virtual StateReturnType updateStateMachine();				// slot 4, 0x004D7321
	virtual void clear();										// slot 5, 0x004D72C5
	virtual StateReturnType resetToDefaultState();				// slot 6, 0x004D7A75
	virtual StateReturnType initDefaultState();					// slot 7, 0x004D770F
	virtual StateReturnType setState(StateID newStateID);		// slot 8, 0x004D7ACD
	virtual StateMachine *makeSubMachine() { return 0; }		// slot 9
	virtual StateMachine *makeGuardMachine() { return 0; }		// slot 10
	virtual Bool isInAttackState() const { if (m_currentState) return m_currentState->isAttack(); return true; }	// slot 11, 0x002621B8: in the class; callers that qualify the call carry the body in place, the vtable slot is the COMDAT copy
	virtual Bool isInBusyState() const;							// slot 12, 0x004D7309
	virtual void rva004D7395();									// slot 13, 0x004D7395
	virtual void setGoalObject(const Object *obj);				// slot 14, 0x004D750F
	virtual void halt();										// slot 15, 0x004D73A4

	void defineState(StateID id, State *state, StateID successID, StateID failureID, const StateConditionInfo *conditions = 0);	// 0x004D7B0F
	State *internalGetState(StateID id);						// 0x004D764C
	StateReturnType internalSetState(StateID newStateID);		// 0x004D766B
	Bool hasState(StateID id);									// 0x004D76F2
	void rva004D7627(StateID id);								// 0x004D7627
	void setGoalPosition(const Coord3D *pos);					// 0x00262224
	void setGoalPosition(const Coord3D *pos, Real range);		// 0x004D745C
	void internalSetGoalPosition(const Coord3D *pos, Real range);	// 0x004D73C4
	void internalSetGoalObject(const Object *obj);				// 0x004D7435
	void clearGoal();											// 0x004D729D (the goal object, second goal object, goal position and range back to their cleared values)
	void rva004D73AD(const void *obj);							// 0x004D73AD
	void rva004D7427(const void *obj);							// 0x004D7427
	Bool isGoalObjectDestroyed() const;							// 0x004D7ADD
	Bool isInIdleState() const { if (m_currentState) return m_currentState->isAttack(); return true; }	// Zero Hour's isInIdleState: inline in retail (AIUpdateInterface::isIdle 0x002645FF reads +4 twice and tail-calls slot 8); no out-of-line copy
	Bool isInGuardIdleState() const { if (m_currentState) return m_currentState->isGuardIdle(); return true; }	// 0x002621C9: inline in retail (AIUpdateInterface slot 111 0x00262D8F); the out-of-line copy is the COMDAT
	Bool rva002621DA() const { if (m_currentState) return m_currentState->unknownQuery11(); return true; }	// 0x002621DA: inline (AIUpdateInterface slot 114 0x00262DAB)
	Bool rva002621EB() const { if (m_currentState) return m_currentState->unknownQuery12(); return true; }	// 0x002621EB: inline (AIUpdateInterface slot 115 0x00262DBF)
	Bool rva002621FC() const { if (m_currentState) return m_currentState->unknownQuery13(); return true; }	// 0x002621FC: inline (AIUpdateInterface slot 116 0x00262D7B)
	Bool rva0026220D() const { if (m_currentState) return m_currentState->unknownQuery14(); return false; }	// 0x0026220D: inline (AIUpdateInterface::isBusy 0x00262B39)
	void lock(const char *msg) { m_locked = true; }			// inline: AIStates.cpp stores the byte in place (AITunnelNetworkGuardState::update 0x00346487); 0x0026221D is an out-of-line copy
	State *getCurrentState() const { return m_currentState; }	// AIGuardState::isAttack 0x00341D81 reads +4 in place
	void unlock() { m_locked = false; }
	void setFlag(Bool flag) { m_flag = flag; }		// +0x3A (the constructor's Bool argument): AIChargeTargetState::onExit 0x0034BEC3 clears it in place
	Bool isLocked() const { return m_locked; }
	Object *getGoalObject();
	Object *getSecondGoalObject();						// 0x004D7735: the object whose ID is at +0x34 (set by rva004D73AD/rva004D7427); the map files it under the placeholder class TurretStateMachine
	Object *getOwner() const { return m_owner; }
	StateID getCurrentStateID() const { return m_currentState ? m_currentState->getID() : INVALID_STATE_ID; }
	const Coord3D *getGoalPosition() const { return &m_goalPosition; }
	ObjectID getGoalObjectID() const { return m_goalObjectID; }	// +0x20; AIAttackMoveToState::onEnter 0x0034EC7B tests it in place

protected:
	virtual void loadPostProcess() {}							// slot 1
	virtual const char *GetSnapshotName() const;				// slot 2, 0x00341456
	virtual void xfer(Xfer *xfer);								// slot 3, 0x004D7744

	State *m_currentState;							// +0x04
	_STL::map<StateID, State *> m_stateMap;			// +0x08
	Object *m_owner;								// +0x14
	UnsignedInt m_sleepTill;						// +0x18
	StateID m_defaultStateID;						// +0x1C
	ObjectID m_goalObjectID;						// +0x20
	Coord3D m_goalPosition;							// +0x24
	Real m_goalRange;								// +0x30
	ObjectID m_secondGoalObjectID;					// +0x34, 0x004D73AD stores Object +0x74
	Bool m_locked;									// +0x38
	Bool m_defaultStateInited;						// +0x39
	Bool m_flag;									// +0x3A, constructor's Bool argument
};
// SIZE 0x3C

// The four built-in states every machine can use. Constructors 0x004D7476, 0x004D7491,
// 0x004D74AC and 0x004D74C7 (each pushes the CRC-32 of the class name, tables 0x00C60620,
// 0x00C60668, 0x00C606B0, 0x00C606F8); slot 2 returns the class name (0x004D73E4, 0x004D73EA,
// 0x004D73F0, 0x004D73F6), slot 3 is the empty xfer (fold 0x0047A69C), slots 4 and 6 return
// the constant named in each class.
class SuccessState : public State
{
public:
	SuccessState(StateMachine *machine) : State(machine, 0x8C36A3D4) {}
	virtual StateReturnType onEnter() { return STATE_SUCCESS; }
	virtual StateReturnType update() { return STATE_SUCCESS; }
protected:
	virtual const char *GetSnapshotName() const;			// 0x004D73E4
	virtual void xfer(Xfer *xfer) {}
};

class FailureState : public State
{
public:
	FailureState(StateMachine *machine) : State(machine, 0xC17A0A70) {}
	virtual StateReturnType onEnter() { return STATE_FAILURE; }
	virtual StateReturnType update() { return STATE_FAILURE; }
protected:
	virtual const char *GetSnapshotName() const;			// 0x004D73EA
	virtual void xfer(Xfer *xfer) {}
};

class ContinueState : public State
{
public:
	ContinueState(StateMachine *machine) : State(machine, 0xDF4D3EB3) {}
	virtual StateReturnType onEnter() { return STATE_CONTINUE; }
	virtual StateReturnType update() { return STATE_CONTINUE; }
protected:
	virtual const char *GetSnapshotName() const;			// 0x004D73F0
	virtual void xfer(Xfer *xfer) {}
};

class SleepState : public State
{
public:
	SleepState(StateMachine *machine) : State(machine, 0xE529ED33) {}
	virtual StateReturnType onEnter() { return STATE_SLEEP_FOREVER; }
	virtual StateReturnType update() { return STATE_SLEEP_FOREVER; }
protected:
	virtual const char *GetSnapshotName() const;			// 0x004D73F6
	virtual void xfer(Xfer *xfer) {}
};

inline Object *State::getMachineOwner() const { return m_machine->getOwner(); }
inline Object *State::getMachineGoalObject() { return m_machine->getGoalObject(); }	// 0x0033F263: the COMDAT copy

#endif
