// AIStates.h: the AI state classes AIStates.cpp defines, BFME 2 layout.
//
// Evidence, per class below: the slot-3 xfer body gives member offsets and types
// (Xfer table: 0x28 Version, 0x30 Snapshot, 0x4C ICoord2D, 0x60 Coord3D, 0x78
// UnsignedInt, 0x7C Int, 0x80 UnsignedShort, 0x90 Bool); sizes come from the
// operator new calls in AIStateMachine's constructor 0x00351C48; the base class
// from the table (a slot shared with a base's table) and from which base xfer
// the body calls. Every class name here is the map's; the ones whose constructors
// are still map placeholders are named by the CRC-32 of the class name each
// constructor passes.
// Slot 2 (GetSnapshotName) bodies are map placeholders (Rva...Get) for most
// states, so it is declared and left to the constructors' landing.
#pragma once
#ifndef BFME2_AISTATES_H
#define BFME2_AISTATES_H

#include "GameLogic/AIStateMachine.h"
#include "Common/ICoord2D.h"
#include "Common/Coord2D.h"

class BackAwayAndCowerStateMachine;
class AIGuardRetaliateMachine;
class AITNGuardMachine;
class ContainModuleInterface;

// What an object asks a container for (ContainModuleInterface slot 17,
// onObjectWantsToEnterOrExit): AIExitState::onEnter 0x00341B11 and
// AIHordeExitState::onEnter 0x00341A1B pass 1 for an exit, AIHordeExitState::onExit
// 0x00341A61 passes 2 when it gives up. The contain header only forward-declares it.
enum ObjectEnterExitType
{
	WANTS_TO_ENTER,
	WANTS_TO_EXIT,
	WANTS_NEITHER
};

// A machine's "done" state: AIBackAwayAndCowerState::update 0x0034022F returns
// success when its sub-machine's current state ID is 999998 (0xF423E).
enum { MACHINE_DONE_STATE_ID = 999998 };

// Constructors are inline (retail emits them as COMDATs where AIStateMachine's
// constructor 0x00351C48 builds the states); each passes the CRC-32 of its class name.
// Small states whose constructors are map placeholders, named by CRC-32 of the
// name hash each constructor passes. Tables: AIBusyState 0x00810E98 (xfer, onEnter
// and update are shared empty bodies; slot 10 isBusy returns true), AIWaitState
// 0x008110D0, AIDeadState 0x00811318, AICowerState 0x00811958 (adds slot 17, a
// shared `return true`), AIQuarrelState 0x008119B0 (shares the cower xfer).
class AIBusyState : public State
{
public:
	AIBusyState(StateMachine *machine) : State(machine, 0x5FE77381) {}					// 0x0033F320 (CRC-32 0x5FE77381)
	virtual StateReturnType onEnter();					// slot 4 (shared 0x000D43D0)
	virtual StateReturnType update();					// slot 6 (shared 0x000D43D0)
	virtual Bool isBusy() const;						// slot 10 (shared 0x0050B5C6, true)

protected:
	virtual const char *GetSnapshotName() const;		// 0x0033F31A
	virtual void xfer(Xfer *xfer);						// slot 3 (shared empty 0x0047A69C)
};
// SIZE 0x20

class AIWaitState : public State
{
public:
	AIWaitState(StateMachine *machine) : State(machine, 0x3E719B30) {}					// 0x0033F41A (CRC-32 0x3E719B30)
	virtual StateReturnType onEnter();					// slot 4 (shared 0x000D43D0)
	virtual StateReturnType update();					// slot 6 (shared 0x000D43D0)

protected:
	virtual const char *GetSnapshotName() const;		// 0x0033F437
	virtual void xfer(Xfer *xfer);						// slot 3 (shared empty 0x0047A69C)
};
// SIZE 0x20

class AIDeadState : public State
{
public:
	AIDeadState(StateMachine *machine) : State(machine, 0x1F4FA782) {}					// 0x0033F51D (CRC-32 0x1F4FA782)
	virtual StateReturnType onEnter();					// 0x0034707A
	virtual void onExit(StateExitType status);			// 0x00347110
	virtual StateReturnType update();					// 0x0033FF05

protected:
	virtual const char *GetSnapshotName() const;		// 0x0033F53A
	virtual void xfer(Xfer *xfer);						// slot 3 (shared empty 0x0047A69C)
};
// SIZE 0x20

class AICowerState : public State
{
public:
	AICowerState(StateMachine *machine) : State(machine, 0x16C0B5AF) {}				// 0x0033F7C8 (CRC-32 0x16C0B5AF)
	virtual StateReturnType onEnter();					// 0x00344598
	virtual void onExit(StateExitType status);			// 0x003402FC
	virtual StateReturnType update();					// 0x00352BD0
	virtual Bool unknownQuery17() const;				// slot 17 (shared 0x0050B5C6, true)

protected:
	virtual const char *GetSnapshotName() const;		// 0x0033F7E5
	virtual void xfer(Xfer *xfer);						// slot 3 (shared 0x00306A9B)
};
// SIZE 0x20

class AIQuarrelState : public State
{
public:
	AIQuarrelState(StateMachine *machine) : State(machine, 0x1373A8A0) {}				// 0x0033F7EB (CRC-32 0x1373A8A0)
	virtual StateReturnType onEnter();					// 0x00347E4C
	virtual void onExit(StateExitType status);			// 0x00340342
	virtual StateReturnType update();					// 0x0034036F

protected:
	virtual const char *GetSnapshotName() const;		// 0x0033F808
	virtual void xfer(Xfer *xfer);						// slot 3 (shared 0x00306A9B)
};
// SIZE 0x20

// AIWaitUntilFinishedFiringState (ctor 0x0033F43D, CRC-32 0xE4F2D93D; table
// 0x00811120): update 0x0034133E waits out the current weapon's
// IdleAfterFiringDelay after its last shot (Weapon +0x2C); onExit 0x00341391
// releases the temporary weapon lock; slot 15 is the shared `return true`.
class AIWaitUntilFinishedFiringState : public State
{
public:
	AIWaitUntilFinishedFiringState(StateMachine *machine) : State(machine, 0xE4F2D93D) {}
	virtual StateReturnType onEnter();					// 0x00345F94
	virtual void onExit(StateExitType status);			// 0x00341391
	virtual StateReturnType update();					// 0x0034133E
	virtual Bool unknownQuery15() const;				// slot 15 (shared 0x0050B5C6)

protected:
	virtual const char *GetSnapshotName() const;		// 0x0033F45A
	virtual void xfer(Xfer *xfer);						// slot 3 (shared empty 0x0047A69C)
};
// SIZE 0x20

// AIFaceDirectionState (ctor 0x00342547, CRC-32 0x6649A02E; table 0x00812378):
// update 0x003425BF turns the owner toward the angle at +0x20 and succeeds once
// within PI/10 of it.
class AIFaceDirectionState : public State
{
public:
	AIFaceDirectionState(StateMachine *machine) : State(machine, 0x6649A02E), m_desiredAngle(0.0f) {}
	virtual StateReturnType onEnter();					// 0x00342572
	virtual void onExit(StateExitType status);			// 0x003425A0
	virtual StateReturnType update();					// 0x003425BF

protected:
	virtual const char *GetSnapshotName() const;		// 0x0034256C
	virtual void xfer(Xfer *xfer);						// slot 3 (shared 0x00306A9B)

	Real m_desiredAngle;								// +0x20
};

// AIIdleState (SIZE 0x28: AIStateMachine's ctor allocates 0x28 for the state
// constructed by 0x0033FE65, hash 0x8821F22E = CRC-32 "AIIdleState").
// Table 0x00811E08: 4 onEnter 0x00346FD0, 6 update 0x0035389E. xfer 0x0033FEA3:
// version 2; UnsignedShort +0x20, Bool +0x24, Bool +0x25, then (v2) UnsignedShort +0x22.
class AIIdleState : public State
{
public:
	AIIdleState(StateMachine *machine, Int shouldLookForTargets)
		: State(machine, 0x8821F22E), m_initialSleepOffset(0xFFFF), m_unk0x22(5), m_inited(shouldLookForTargets == 0), m_shouldLookForTargets(false) {}	// 0x0033FE65 (map placeholder Rva0033FE65; CRC-32 "AIIdleState" 0x8821F22E)
	virtual StateReturnType onEnter();					// 0x00346FD0
	virtual StateReturnType update();					// 0x0035389E
	virtual Bool isAttack() const;						// slot 8 (shared 0x0050B5C6, true)

protected:
	virtual const char *GetSnapshotName() const;		// 0x0033FE9D
	virtual void xfer(Xfer *xfer);						// 0x0033FEA3

	UnsignedShort m_initialSleepOffset;					// +0x20
	UnsignedShort m_unk0x22;							// +0x22, version 2
	Bool m_inited;										// +0x24
	Bool m_shouldLookForTargets;						// +0x25

private:
	void doInitIdleState();								// 0x0034405D
};
// SIZE 0x28

// AIFaceState (SIZE 0x30, ctor 0x00342FCD). xfer 0x0034236F calls AIIdleState's
// xfer first, then its own version 1 and the Bool +0x2C, so it derives AIIdleState.
// Table 0x00812FA0: 5 onExit 0x003423A6, 6 update 0x0035487E.
class AIFaceState : public AIIdleState
{
public:
	AIFaceState(StateMachine *machine, Int faceType);	// 0x00342FCD (calls AIIdleState's; states 36, 37, 59 pass 1, 0, 2)
	virtual void onExit(StateExitType status);			// 0x003423A6
	virtual StateReturnType update();					// 0x0035487E

protected:
	virtual const char *GetSnapshotName() const;		// 0x00342FF2
	virtual void xfer(Xfer *xfer);						// 0x0034236F

	UnsignedInt m_unk0x28;								// +0x28
	Bool m_canTurnInPlace;								// +0x2C
};
// SIZE 0x30

// AIRampageState (ctor 0x0033F80E, hash CRC-32 "AIRampageState"). Table 0x00811A08:
// 4 onEnter 0x0034BD1B, 5 onExit 0x0034BDF8, 6 update 0x0034FE10. xfer 0x00342469:
// version 1; Bool +0x20, UnsignedInt +0x24, UnsignedInt +0x28.
class AIRampageState : public State
{
public:
	AIRampageState(StateMachine *machine) : State(machine, 0x0CAC7338), m_unk0x20(false), m_unk0x24(0), m_unk0x28((UnsignedInt)-1) {}			// 0x0033F80E (map placeholder; CRC-32 0x0CAC7338)
	virtual StateReturnType onEnter();					// 0x0034BD1B
	virtual void onExit(StateExitType status);			// 0x0034BDF8
	virtual StateReturnType update();					// 0x0034FE10
	void fireRampageWeapon(Object *obj);				// 0x0034BE57 (the map files it as rva0034BE57): with the rampage flag set it fires the current weapon at the object's own position when ready; otherwise it sets the flag and object status 0xD when the weapon is ready

protected:
	virtual const char *GetSnapshotName() const;		// 0x0033F837
	virtual void xfer(Xfer *xfer);						// 0x00342469

	Bool m_unk0x20;										// +0x20
	UnsignedInt m_unk0x24;								// +0x24
	UnsignedInt m_unk0x28;								// +0x28
};

// AIExitState (SIZE 0x28, ctor 0x0033F64E (machine, Bool), hash CRC-32
// "AIExitState"). Table 0x008115E8: 4 onEnter 0x00341B11, 5 onExit 0x0035118A,
// 6 update 0x00341B4E. xfer 0x00341ACD: version 2; ObjectID +0x20, (v2) Bool +0x24.
class AIExitState : public State
{
public:
	AIExitState(StateMachine *machine, Bool unk) : State(machine, 0xDFB3DBC7), m_entryToClear(INVALID_ID), m_unk0x24(unk) {}		// 0x0033F64E (map placeholder; CRC-32 0xDFB3DBC7)
	virtual StateReturnType onEnter();					// 0x00341B11
	virtual void onExit(StateExitType status);			// 0x0035118A
	virtual StateReturnType update();					// 0x00341B4E

protected:
	virtual const char *GetSnapshotName() const;		// 0x0033F676
	virtual void xfer(Xfer *xfer);						// 0x00341ACD

	ObjectID m_entryToClear;							// +0x20
	Bool m_unk0x24;										// +0x24, version 2
};
// SIZE 0x28

// AIHordeExitState (SIZE 0x24, ctor 0x0033F5B9 (machine, Bool), hash CRC-32
// "AIHordeExitState"). xfer 0x00341A95: version 2; (v2) Bool +0x20.
class AIHordeExitState : public State
{
public:
	AIHordeExitState(StateMachine *machine, Bool unk) : State(machine, 0x55A8130B), m_unk0x20(unk) {}	// 0x0033F5B9 (map placeholder; CRC-32 0x55A8130B)
	virtual StateReturnType onEnter();					// 0x00341A1B
	virtual void onExit(StateExitType status);			// 0x00341A61
	virtual StateReturnType update();					// 0x003462E1

	virtual Bool unknownQuery12() const;				// slot 12 (shared 0x0050B5C6, true)
protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x00341A95

	Bool m_unk0x20;										// +0x20, version 2
};
// SIZE 0x24

// AIAttackAimAtTargetState (ctor 0x0033F3B6 (machine, Bool, Bool), hash CRC-32
// "AIAttackAimAtTargetState"). xfer 0x00341237: version 1; Bools +0x21, +0x22, +0x23.
class AIAttackAimAtTargetState : public State
{
public:
	AIAttackAimAtTargetState(StateMachine *machine, Bool unk20, Bool unk23)
		: State(machine, 0x5C57BD74), m_unk0x20(unk20), m_canTurnInPlace(false), m_setLocomotor(false), m_unk0x23(unk23) {}	// 0x0033F3B6 (map placeholder; CRC-32 0x5C57BD74)
	virtual StateReturnType update();
	virtual void onExit(StateExitType status);

protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x00341237

	Bool m_unk0x20;										// +0x20
	Bool m_canTurnInPlace;								// +0x21
	Bool m_setLocomotor;								// +0x22
	Bool m_unk0x23;										// +0x23
};

// AIAttackPositionAimAtTargetState: xfer 0x00341275: version 1; Bools +0x20, +0x21.
class AIAttackPositionAimAtTargetState : public State
{
public:
	StateReturnType updateInternal(Bool unk);			// 0x0034A570; update 0x0034A871 passes false
	virtual StateReturnType update();					// 0x0034A871
	virtual void onExit(StateExitType status);

protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x00341275

	Bool m_unk0x20;										// +0x20
	Bool m_unk0x21;										// +0x21
};

// AIUncontrollableCower (SIZE 0x24): ctor 0x00340391 calls AICowerState's
// 0x0033F7C8 and stores table 0x00C12048 (slot 6 is AICowerState's update
// 0x00352BD0; GetSnapshotName returns "AIUncontrollableCower"). xfer 0x003403B3:
// version 1, Bool +0x20, version 1 again.
class AIUncontrollableCower : public AICowerState
{
public:
	AIUncontrollableCower(StateMachine *machine);		// 0x00340391
	virtual StateReturnType onEnter();					// 0x00347EA8
	virtual void onExit(StateExitType status);			// 0x00347EFA

protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x003403B3

	Bool m_unk0x20;										// +0x20
};

// AIGoingIdleState (SIZE 0x20, ctor 0x0033F701). Table 0x00811798: 0 deleting dtor
// 0x00342F41 (it has its own dtor 0x00341E22), 3 xfer 0x00341E2D (nothing but the
// version, skipped for a light CRC), 4 onEnter 0x00341E48.
class AIGoingIdleState : public State
{
public:
	AIGoingIdleState(StateMachine *machine);			// 0x0033F701
	virtual ~AIGoingIdleState();						// 0x00341E22
	virtual StateReturnType onEnter();					// 0x00341E48
	virtual StateReturnType update();					// 0x00488988 (the map spells it non-virtual)

protected:
	virtual const char *GetSnapshotName() const;		// 0x0033F71E
	virtual void xfer(Xfer *xfer);						// 0x00341E2D
};
// SIZE 0x20

// The three states that run a sub-machine at +0x20 save it the same way (xfer
// 0x00341D9B, 0x003414F6, 0x00342283): nothing for a light CRC; version 1; a Bool
// "has a machine"; on load a missing machine is made through the owning machine's
// factory slot (guard: makeGuardMachine, the others makeSubMachine); then the
// machine's snapshot.
class AIGuardState : public State
{
public:
	AIGuardState(StateMachine *machine);				// 0x0033F6DA
	virtual StateReturnType onEnter();
	virtual void onExit(StateExitType status);
	virtual StateReturnType update();					// 0x00341DFC
	virtual Bool isAttack() const;						// 0x00341D81

	virtual Bool isGuardIdle() const;					// slot 9 (fold 0x0050B5C6, returns true)
protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x00341D9B

	StateMachine *m_guardMachine;						// +0x20
};

class AIAttackSquadState : public State
{
public:
	AIAttackSquadState(StateMachine *machine, Int unk);	// 0x0033F4DF
	virtual ~AIAttackSquadState();						// 0x0034149B (halts and deletes the machine)
	virtual StateReturnType onEnter();
	virtual StateReturnType update();

	virtual void onExit(StateExitType status);			// slot 5, 0x00341557
	virtual Bool isAttack() const;						// slot 8 (the area state's body 0x003422EF)
	virtual Bool isGuardIdle() const;					// slot 9 (AIAttackState's body 0x0034B167)
	virtual Bool unknownQuery15() const;				// slot 15, 0x0033F504
protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x003414F6

	StateMachine *m_attackSquadMachine;					// +0x20
	Bool m_unk0x24;										// +0x24: the ctor 0x0033F4DF clears it (SIZE 0x28 from AIStateMachine's new)
};

// AIAttackAreaState also saves an UnsignedInt +0x24 after the machine.
class AIAttackAreaState : public State
{
public:
	AIAttackAreaState(StateMachine *machine);			// 0x0033F79D
	virtual ~AIAttackAreaState();						// 0x00342228
	virtual StateReturnType onEnter();
	virtual void onExit(StateExitType status);			// 0x0034234C
	virtual StateReturnType update();					// 0x0034680D
	virtual Bool isAttack() const;						// 0x003422EF

	virtual Bool isGuardIdle() const;					// slot 9 (AIAttackState's body 0x0034B167)
	virtual Bool unknownQuery14() const;				// slot 14 (fold 0x0050B5C6)
protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x00342283

	StateMachine *m_attackMachine;						// +0x20
	UnsignedInt m_nextEnemyScanTime;					// +0x24
};

// The guard-retaliate, tunnel-network-guard and hunt states own a sub-machine at
// +0x20 the same way; their dtors (0x00341E70, 0x00341F99, 0x00342157) halt and
// delete it. Retaliate's xfer 0x00341ECB recreates an AIGuardRetaliateMachine
// (0x4C bytes) on load, the tunnel guard's 0x00341FF4 an AITNGuardMachine (0x50).
class AIGuardRetaliateState : public State
{
public:
	AIGuardRetaliateState(StateMachine *machine);		// 0x0033F724
	virtual ~AIGuardRetaliateState();					// 0x00341E70
	virtual StateReturnType onEnter();					// 0x003463DA

	virtual void onExit(StateExitType status);			// slot 5 (0x00342120, shared with the tunnel guard)
	virtual StateReturnType update();					// slot 6, 0x00341F5D
	virtual Bool isGuardIdle() const;					// slot 9 (fold 0x0050B5C6)
protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x00341ECB

	AIGuardRetaliateMachine *m_retaliateMachine;		// +0x20
};

class AITunnelNetworkGuardState : public State
{
public:
	AITunnelNetworkGuardState(StateMachine *machine);	// 0x0033F74B
	virtual ~AITunnelNetworkGuardState();				// 0x00341F99
	virtual StateReturnType onEnter();					// 0x00342086
	virtual StateReturnType update();					// 0x00346487

	virtual void onExit(StateExitType status);			// slot 5, 0x00342120
	virtual Bool isGuardIdle() const;					// slot 9 (fold 0x0050B5C6)
protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x00341FF4

	AITNGuardMachine *m_guardMachine;					// +0x20
};

// AIHuntState: onEnter 0x003421B2 makes the sub-machine through makeSubMachine and
// sets the next scan frame +0x24; onExit 0x003421F4 deletes it and releases the
// owner's temporary weapon lock.
class AIHuntState : public State
{
public:
	AIHuntState(StateMachine *machine);				// 0x0033F772
	virtual ~AIHuntState();								// 0x00342157
	virtual void onExit(StateExitType status);			// 0x003421F4
	virtual StateReturnType update();					// 0x003464D1

	virtual StateReturnType onEnter();					// slot 4, 0x003421B2
	virtual Bool isAttack() const;						// slot 8 (the area state's body 0x003422EF)
	virtual Bool isGuardIdle() const;					// slot 9 (AIAttackState's body 0x0034B167)
	virtual Bool unknownQuery14() const;				// slot 14 (fold 0x0050B5C6)
protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// slot 3 (the area state's body 0x00342283)

	StateMachine *m_huntMachine;						// +0x20
	UnsignedInt m_nextEnemyScanTime;					// +0x24
};

// AIAttackMeleeHordeWaitPathState (ctor 0x0033F38B, hash CRC-32
// "AIAttackMeleeHordeWaitPathState"). xfer 0x0034074C: version 2; unless a light
// CRC: UnsignedInt +0x20, (v2) Int +0x24.
class AIAttackMeleeHordeWaitPathState : public State
{
public:
	AIAttackMeleeHordeWaitPathState(StateMachine *machine) : State(machine, 0xF17C0982), m_unk0x20(0), m_unk0x24(0) {}	// 0x0033F38B (map placeholder; CRC-32 0xF17C0982)
	virtual StateReturnType onEnter();					// 0x00345076
	virtual StateReturnType update();					// 0x003450DB

protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x0034074C

	UnsignedInt m_unk0x20;								// +0x20
	Int m_unk0x24;										// +0x24, version 2
};

// AIMoveToStateSA (SIZE 0x54, ctor 0x003400CE, hash CRC-32 "AIMoveToStateSA").
// The ctor calls AIInternalMoveToState's directly and zeroes the UnsignedInt
// +0x4C (AIMoveToState's Bool +0x4C would sit there), so it derives
// AIInternalMoveToState; table 0x00811F00's onExit is the folded forwarder
// 0x0034BA5F. xfer 0x0034007E: version 1; the move state's xfer; unless a
// light CRC: UnsignedInt +0x4C, Bool +0x50.
class AIMoveToStateSA : public AIInternalMoveToState
{
public:
	AIMoveToStateSA(StateMachine *machine);				// 0x003400CE
	virtual StateReturnType onEnter();					// 0x0034C8C5
	virtual void onExit(StateExitType status);			// slot 5, 0x0034BA5F (folded forwarder)
	virtual StateReturnType update();					// 0x00347A51

protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x0034007E
	virtual Bool computePath();							// 0x00347959

	UnsignedInt m_unk0x4C;								// +0x4C
	Bool m_unk0x50;										// +0x50
};
// SIZE 0x54

// The attack approach states have the folded onExit forwarder 0x0034BA5F (tables
// 0x00812798, 0x00812800) and an UnsignedInt at +0x4C, so like AIMoveToStateSA
// they derive AIInternalMoveToState. xfer: version 1; the move state's xfer;
// unless a light CRC: Coord3D +0x50, ICoord2D +0x5C, UnsignedInt +0x4C, then the
// class's own tail.
class AIAttackFireDuringApproachState : public AIInternalMoveToState
{
public:
	virtual StateReturnType onEnter();					// 0x0034D4A2
	virtual void onExit(StateExitType status);			// slot 5, 0x0034BA5F (folded forwarder)
	virtual StateReturnType update();					// slot 6, 0x003508C0

protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x0034081A
	virtual Bool computePath();							// 0x00350497

	UnsignedInt m_unk0x4C;								// +0x4C
	Coord3D m_unk0x50;									// +0x50
	ICoord2D m_unk0x5C;									// +0x5C
	ObjectID m_unk0x64;									// +0x64
	Bool m_unk0x68;										// +0x68
};

class AIAttackMeleeApproachState : public AIInternalMoveToState
{
public:
	virtual StateReturnType onEnter();					// 0x0034D5B7
	virtual void onExit(StateExitType status);			// slot 5, 0x0034BA5F (folded forwarder)
	virtual StateReturnType update();					// slot 6, 0x003497C2

protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x0034087C
	virtual Bool computePath();							// 0x00345393

	UnsignedInt m_unk0x4C;								// +0x4C
	Coord3D m_unk0x50;									// +0x50
	ICoord2D m_unk0x5C;									// +0x5C
};

// AIAttackMeleeSquishState: table 0x00812868 has its own onExit 0x003497C7; xfer
// 0x00340A92 adds a Bool +0x64 to the approach layout.
class AIAttackMeleeSquishState : public AIInternalMoveToState
{
public:
	virtual StateReturnType onEnter();					// 0x0034D886
	virtual void onExit(StateExitType status);			// 0x003497C7
	virtual StateReturnType update();					// slot 6, 0x0034DC00

protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x00340A92
	virtual Bool computePath();							// 0x003408C4

	UnsignedInt m_unk0x4C;								// +0x4C
	Coord3D m_unk0x50;									// +0x50
	ICoord2D m_unk0x5C;									// +0x5C
	Bool m_unk0x64;										// +0x64
};

// AIMoveAndEvacuateState (SIZE 0x58, ctor 0x00342D76; table 0x00812C28).
// xfer 0x00340E70: version 1; the move state's xfer; Coord3D +0x4C.
class AIMoveAndEvacuateState : public AIInternalMoveToState
{
public:
	AIMoveAndEvacuateState(StateMachine *machine);		// 0x00342D76
	virtual StateReturnType onEnter();					// 0x0034EB7E
	virtual void onExit(StateExitType status);			// 0x00349FD0
	virtual StateReturnType update();					// 0x00353C27

protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x00340E70

	Coord3D m_origin;									// +0x4C
};
// SIZE 0x58

// AIMoveAndDeleteState (SIZE 0x50, ctor 0x00342DAB; table 0x00812C88).
// xfer 0x00340F42: version 1 (explicit pair); the move state's xfer; Bool +0x4C.
class AIMoveAndDeleteState : public AIInternalMoveToState
{
public:
	AIMoveAndDeleteState(StateMachine *machine);		// 0x00342DAB
	virtual StateReturnType onEnter();					// 0x0034ECDB
	virtual void onExit(StateExitType status);			// 0x0034A0CB
	virtual StateReturnType update();					// 0x0034A02D

protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x00340F42

	Bool m_appendGoalPosition;							// +0x4C
};
// SIZE 0x50

// AIWanderInPlaceState (SIZE 0x60, ctor 0x00342CBF; table 0x00812AC8).
// xfer 0x003411BD: version 1; the move state's xfer; unless a light CRC:
// Coord3D +0x4C, Int +0x58, Int +0x5C.
class AIWanderInPlaceState : public AIInternalMoveToState
{
public:
	AIWanderInPlaceState(StateMachine *machine);		// 0x00342CBF
	virtual StateReturnType onEnter();					// 0x0034F2F7
	virtual void onExit(StateExitType status);			// 0x0034A364
	virtual StateReturnType update();					// 0x0034F3EB

protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x003411BD

	Coord3D m_origin;									// +0x4C
	Int m_unk0x58;										// +0x58
	Int m_unk0x5C;										// +0x5C
};
// SIZE 0x60

// AIEnterAndAttackState (ctor 0x00342EFA; table 0x00812F40). xfer 0x00341994:
// version 1; the move state's xfer; ObjectIDs +0x4C and +0x50.
class AIEnterAndAttackState : public AIInternalMoveToState
{
public:
	AIEnterAndAttackState(StateMachine *machine);		// 0x00342EFA
	virtual StateReturnType onEnter();					// 0x0034FD0D
	virtual StateReturnType update();					// 0x003546FD

protected:
	virtual const char *GetSnapshotName() const;		// slot 2
	virtual void xfer(Xfer *xfer);						// 0x00341994

	ObjectID m_unk0x4C;									// +0x4C
	ObjectID m_unk0x50;									// +0x50
};

// AIDockState (SIZE 0x28: AIStateMachine's ctor allocates 0x28). ctor 0x0033F540
// passes 0xE84618B6 (CRC-32 "AIDockState"), zeroes +0x20 and the Bool +0x24 and
// stores table 0x00C11368. onEnter 0x00341695 builds an AIDockMachine (0x40 bytes)
// on the owner aimed at the goal object; update 0x00341770 runs it; onExit
// 0x00341723 and the dtor 0x0034159A halt and delete it. xfer 0x003415F5: the
// sub-machine as in AIGuardState, then the Bool +0x24.
class AIDockState : public State
{
public:
	AIDockState(StateMachine *machine);					// 0x0033F540
	virtual ~AIDockState();								// 0x0034159A
	virtual StateReturnType onEnter();					// 0x00341695
	virtual void onExit(StateExitType status);			// 0x00341723
	virtual StateReturnType update();					// 0x00341770

protected:
	virtual const char *GetSnapshotName() const;		// 0x0033F565
	virtual void xfer(Xfer *xfer);						// 0x003415F5

	StateMachine *m_dockMachine;						// +0x20
	Bool m_usingPrecisionMovement;						// +0x24
};
// SIZE 0x28

// AIBackAwayAndCowerState (SIZE 0x24: AIStateMachine's ctor allocates 0x24).
// Table 0x00C10EE0. onEnter 0x00347C38 builds a BackAwayAndCowerStateMachine
// (0x3C bytes, name hash 0xD944B508 = CRC-32 "BackAwayAndCowerStateMachine") on
// the owner and the goal object; update 0x0034022F succeeds once it reaches
// MACHINE_DONE_STATE_ID; onExit 0x0034025E and the dtor 0x003401E4 delete it.
// xfer 0x0034027D: version 1 and the sub-machine's snapshot.
class AIBackAwayAndCowerState : public State
{
public:
	AIBackAwayAndCowerState(StateMachine *machine);	// 0x0033F33D
	virtual ~AIBackAwayAndCowerState();					// 0x003401E4
	virtual StateReturnType onEnter();					// 0x00347C38
	virtual void onExit(StateExitType status);			// 0x0034025E
	virtual StateReturnType update();					// 0x0034022F

protected:
	virtual const char *GetSnapshotName() const;		// 0x0033F35E
	virtual void xfer(Xfer *xfer);						// 0x0034027D

	BackAwayAndCowerStateMachine *m_cowerMachine;		// +0x20
};
// SIZE 0x24

// AIAttackMeleeHordeWaitState: ctor 0x0033F364 passes 0xEFC3D47F (CRC-32 of the
// name), zeroes +0x20 and stores table 0x00C10F40. Table slot 16 is its own
// 0x00340744 (a map placeholder). xfer 0x0034071A: version 1; unless a light CRC,
// UnsignedInt +0x20. The map files slot 2's body as a non-virtual name().
class AIAttackMeleeHordeWaitState : public State
{
public:
	AIAttackMeleeHordeWaitState(StateMachine *machine);	// 0x0033F364
	const char *name() const;							// 0x0033F385
	virtual StateReturnType onEnter();					// 0x00344F57
	virtual void onExit(StateExitType status);			// 0x00345207
	virtual StateReturnType update();					// 0x003492BD
	virtual void unknownNotify16();						// slot 16, 0x00340744

protected:
	virtual const char *GetSnapshotName() const;		// slot 2, 0x0033F385
	virtual void xfer(Xfer *xfer);						// 0x0034071A

	UnsignedInt m_unk0x20;								// +0x20
};

// ---------------------------------------------------------------------------
// The remaining states AIStateMachine's constructor 0x00351C48 builds. Each name
// is proven by the image: the CRC-32 of the name its constructor passes (h07's
// h07_state_name_crc.tsv, verified by n01) or, for a class whose constructor
// passes its base's hash, the string its own slot-2 GetSnapshotName returns.
// Sizes are the constructor's operator new argument; bases are the constructor
// it calls first. Members not yet read from an xfer are left as the gap
// (UnsignedInt m_unk0x..) that keeps the size; each constructor is its own
// function in retail (the machine's constructor calls it).

// AIMoveOutOfTheWayState (SIZE 0x4C, ctor 0x00342826, CRC-32 0x0DA1899B). Table
// 0x00C12490: 4 onEnter 0x0034C994, 5 onExit 0x00347AAC, 6 update 0x00349D8D,
// 17 computePath 0x003400F9; name and xfer are AIInternalMoveToState's.
class AIMoveOutOfTheWayState : public AIInternalMoveToState
{
public:
	AIMoveOutOfTheWayState(StateMachine *machine);			// 0x00342826
	virtual StateReturnType onEnter();						// 0x0034C994
	virtual void onExit(StateExitType status);				// 0x00347AAC
	virtual StateReturnType update();						// 0x00349D8D

protected:
	virtual Bool computePath();								// 0x003400F9
};
// SIZE 0x4C

// AIMoveAndTightenState (SIZE 0x54, ctor 0x00342843, CRC-32 0x5106D8AC; a second
// ctor 0x0034286E takes the name hash and is what the repulsor, panic, fear and
// cower-away states call). Table 0x00C124D8: 2 name 0x00342868, 3 xfer 0x00340123,
// 4 onEnter 0x0034C9ED, 6 update 0x00347AE8, 17 computePath 0x00340158.
class AIMoveAndTightenState : public AIInternalMoveToState
{
public:
	AIMoveAndTightenState(StateMachine *machine);						// 0x00342843
	AIMoveAndTightenState(StateMachine *machine, UnsignedInt name);	// 0x0034286E
	virtual StateReturnType onEnter();						// 0x0034C9ED
	virtual StateReturnType update();						// 0x00347AE8

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342868
	virtual void xfer(Xfer *xfer);							// 0x00340123
	virtual Bool computePath();								// 0x00340158

	Int m_okToRepathTimes;									// +0x4C (Zero Hour's name): the derived computePaths count it down
	Bool m_checkForPath;									// +0x50 (Zero Hour's name)
};
// SIZE 0x54

// The four states built on AIMoveAndTightenState's named ctor 0x0034286E
// (tables 0x00C12538, 0x00C12580, 0x00C125C8, 0x00C12FF0).
// The map spells its members AIMoveAwayFromRepulsorsState (computePath 0x003401AF; Zero Hour's
// name); the ctor's name hash is CRC-32 "AIMoveAwayFromRepulsors".
class AIMoveAwayFromRepulsorsState : public AIMoveAndTightenState
{
public:
	AIMoveAwayFromRepulsorsState(StateMachine *machine);			// 0x00342892 (CRC-32 0xD5F5D4FA)
	virtual StateReturnType onEnter();						// 0x0034CAF6
	virtual void onExit(StateExitType status);				// 0x00347BA3
	virtual StateReturnType update();						// 0x00347BD2

protected:
	virtual Bool computePath();								// 0x003401AF
};
// SIZE 0x54

class AIMoveAwayPanicState : public AIMoveAndTightenState
{
public:
	AIMoveAwayPanicState(StateMachine *machine);			// 0x003428AF (CRC-32 0xE3F474A1)
	virtual StateReturnType onEnter();						// 0x0034CA53
	virtual void onExit(StateExitType status);				// 0x00347BA3 (shared)
	virtual StateReturnType update();						// 0x00347B3D

protected:
	virtual Bool computePath();								// 0x0034017A
};
// SIZE 0x54

class AIFearState : public AIMoveAndTightenState
{
public:
	AIFearState(StateMachine *machine);						// 0x003428CC (CRC-32 0xE4366E82)
	virtual StateReturnType onEnter();						// 0x00354D0D
	virtual void onExit(StateExitType status);				// 0x00347E0A
	virtual StateReturnType update();						// 0x00347CA3

protected:
	virtual Bool computePath();								// 0x0034029B

	UnsignedInt m_unk0x54;									// +0x54
	UnsignedInt m_unk0x58;									// +0x58
};
// SIZE 0x5C

class AIMoveAwayAndCowerState : public AIMoveAndTightenState
{
public:
	AIMoveAwayAndCowerState(StateMachine *machine);			// 0x00342FF8 (CRC-32 0x0C947CC9)
	virtual StateReturnType onEnter();						// 0x0034CBAB
	virtual void onExit(StateExitType status);				// 0x00347F48
	virtual StateReturnType update();						// 0x00347F76

protected:
	virtual const char *GetSnapshotName() const;			// 0x00343015
	virtual void xfer(Xfer *xfer);							// 0x00340411
	virtual Bool computePath();								// 0x003403DC
};
// SIZE 0x54

// AIBurningDeathState (SIZE 0x5C, ctor 0x00342CFC passes 0x334BBBD8, CRC-32
// "AIBurningDeathState"; table 0x00C12B28: 3 xfer 0x003427A9, 4 onEnter
// 0x0035017B, 5 onExit 0x00346B6D, 6 update 0x0035025B. Its slot 2 is
// AIWanderInPlaceState's getter 0x00342CF6: the same string, folded.)
class AIBurningDeathState : public AIInternalMoveToState
{
public:
	AIBurningDeathState(StateMachine *machine);				// 0x00342CFC
	virtual StateReturnType onEnter();						// 0x0035017B
	virtual void onExit(StateExitType status);				// 0x00346B6D
	virtual StateReturnType update();						// 0x0035025B

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342CF6 (folded)
	virtual void xfer(Xfer *xfer);							// 0x003427A9: version 1, UnsignedInt +0x4C, Coord3D +0x50 (not the base's)

	UnsignedInt m_unk0x4C;									// +0x4C
	Coord3D m_unk0x50;										// +0x50
};
// SIZE 0x5C

// AIFollowWaypointPathState (SIZE 0x68, ctors 0x00342BAE (machine, Bool) and
// 0x00342BFC (machine, Bool, Bool), CRC-32 0x6CE5EF54; table 0x00C12930).
class AIFollowWaypointPathState : public AIInternalMoveToState
{
public:
	AIFollowWaypointPathState(StateMachine *machine, Bool asGroup);				// 0x00342BAE
	AIFollowWaypointPathState(StateMachine *machine, Bool asGroup, Bool unk);	// 0x00342BFC
	virtual StateReturnType onEnter();						// 0x0034ED7B
	virtual void onExit(StateExitType status);				// 0x0034A0D7
	const Waypoint *getNextWaypoint();						// 0x00340F7C
	Real calcExtraPathDistance();							// 0x00340FCD
	void computeGoal(Bool useGroupOffsets);					// 0x00345D00

protected:
	virtual void xfer(Xfer *xfer);							// 0x0034103B (tables 0x00C12930 and 0x00C12998)
public:

	// Zero Hour's members, in the order the xfer 0x0034103B transfers them.
	Coord2D m_groupOffset;									// +0x4C
	Real m_angle;											// +0x54
	Int m_framesSleeping;									// +0x58
	const Waypoint *m_currentWaypoint;						// +0x5C (by ID through TerrainLogic slot 35)
	const Waypoint *m_priorWaypoint;						// +0x60
	Bool m_appendGoalPosition;								// +0x64
	UnsignedByte m_unk0x65[3];								// +0x65

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342BF6
};
// SIZE 0x68

// Derived without a name hash of their own (each ctor calls 0x00342BAE); the
// names are their GetSnapshotName strings. AIFollowWaypointPathStateAndEvacuate
// (ctor 0x00342C47, table 0x00C12998: 4 onEnter 0x0034F004, 5 onExit 0x0034A105,
// 6 update 0x003541F4); AIWanderState (0x00342C97, table 0x00C12A70: 3 xfer
// 0x00341205, 4 0x0034F1E1, 5 0x0034A323, 6 0x0034A1BB); AIPanicState (0x00342D19,
// table 0x00C12B70: 3 xfer 0x00341205, 4 0x0034F505, 5 0x0034A4DB, 6 0x0034A38E).
class AIFollowWaypointPathStateAndEvacuate : public AIFollowWaypointPathState
{
public:
	AIFollowWaypointPathStateAndEvacuate(StateMachine *machine, Bool asGroup);	// 0x00342C47
	virtual StateReturnType onEnter();						// 0x0034F004
	virtual void onExit(StateExitType status);				// 0x0034A105
	virtual StateReturnType update();						// 0x003541F4

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342C63
};
// SIZE 0x68

class AIWanderState : public AIFollowWaypointPathState
{
public:
	AIWanderState(StateMachine *machine);					// 0x00342C97
	virtual StateReturnType onEnter();						// 0x0034F1E1
	virtual void onExit(StateExitType status);				// 0x0034A323
	virtual StateReturnType update();						// 0x0034A1BB

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342CB9
	virtual void xfer(Xfer *xfer);							// 0x00341205: version 1, the waypoint state's xfer, Int +0x68, Int +0x6C

	Int m_unk0x68;											// +0x68
	Int m_unk0x6C;											// +0x6C
};
// SIZE 0x70

class AIPanicState : public AIFollowWaypointPathState
{
public:
	AIPanicState(StateMachine *machine);					// 0x00342D19
	virtual StateReturnType onEnter();						// 0x0034F505
	virtual void onExit(StateExitType status);				// 0x0034A4DB
	virtual StateReturnType update();						// 0x0034A38E

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342D3B
	virtual void xfer(Xfer *xfer);							// 0x00341205 (shared with AIWanderState)

	Int m_unk0x68;											// +0x68
	Int m_unk0x6C;											// +0x6C
};
// SIZE 0x70

// AIFollowWaypointPathExactState (SIZE 0x54, ctor 0x00342C69, CRC-32 0xE1CB82BF).
class AIFollowWaypointPathExactState : public AIInternalMoveToState
{
public:
	AIFollowWaypointPathExactState(StateMachine *machine, Bool asGroup);	// 0x00342C69
	virtual StateReturnType update();						// 0x0034A167
	virtual void onExit(StateExitType status);				// 0x0034A121 (slot 5: base onExit, then the AI's completed waypoint, path-through-units off, locomotor flag bit 1 cleared)
	virtual void xfer(Xfer *xfer);							// 0x00341114

	const Waypoint *m_lastWaypoint;							// +0x4C (Zero Hour name; the xfer transfers its ID)
	UnsignedInt m_unk0x50;									// +0x50

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342C91
};
// SIZE 0x54

// AIFollowPathState (SIZE 0x58, ctor 0x00342D41 (machine, UnsignedInt name hash):
// AIStateMachine passes 0x8A091FF3 for states 6 and 7); AIFollowPathAndEvacuateState
// (ctor 0x00342E35) derives from it.
class AIFollowPathState : public AIInternalMoveToState
{
public:
	virtual StateReturnType onEnter();						// 0x0034DEF8 (AIFollowPathAndEvacuateState::onEnter 0x0035014D tail-calls it)
	AIFollowPathState(StateMachine *machine, UnsignedInt name);	// 0x00342D41
	virtual void onExit(StateExitType status);				// 0x00349D92

protected:
	virtual Bool computePath();								// 0x00345B89
	virtual void xfer(Xfer *xfer);							// 0x00340CB0: version 2; unless a light CRC Int +0x4C, Bools +0x50/+0x51, (v2) Int +0x54
public:

	Int m_unk0x4C;											// +0x4C
	Bool m_unk0x50;											// +0x50
	Bool m_unk0x51;											// +0x51
	Int m_unk0x54;											// +0x54

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342D70
};
// SIZE 0x58

class AIFollowPathAndEvacuateState : public AIFollowPathState
{
public:
	AIFollowPathAndEvacuateState(StateMachine *machine);	// 0x00342E35
	virtual StateReturnType onEnter();						// 0x0035014D: chooses the evacuate locomotor set, then the follow-path state's onEnter
	virtual void onExit(StateExitType status);				// 0x0034C0B4

protected:
	virtual void xfer(Xfer *xfer);							// 0x0034252C: version 1 and the follow-path state's xfer
public:

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342E52
};
// SIZE 0x58

// AIFollowPathAsTeamState (SIZE 0x68, ctor 0x0034E307 (machine, Bool, name hash
// 0x757AFBB5): AIInternalMoveToState's ctor, table 0x00C121E8, then an
// AIAttackMoveStateMachine (name 0xC586154E)).
class AIFollowPathAsTeamState : public AIInternalMoveToState
{
public:
	AIFollowPathAsTeamState(StateMachine *machine, Bool unk, UnsignedInt name);	// 0x0034E307
	virtual void onExit(StateExitType status);				// 0x00349E15
	virtual Bool isGuardIdle() const;						// slot 9, 0x00340E39

	Int m_unk0x4C;											// +0x4C (the constructor 0x0034E307 zeroes it)
	Int m_unk0x50;											// +0x50 (the constructor stores 5)
	Bool m_unk0x54;											// +0x54 (the constructor stores true)
	Bool m_unk0x55;											// +0x55
	Bool m_unk0x56;											// +0x56
	Bool m_unk0x57;											// +0x57 (version 3)
	Int m_commandSource;									// +0x58 (CommandSourceType, transferred by XferCommandSourceType 0x00305C02)
	StateMachine *m_subMachine;								// +0x5C: isGuardIdle 0x00340E39 asks it for its attack state; the constructor 0x0034E307 makes an AIAttackMoveStateMachine when its Bool argument is set
	Real m_unk0x60;											// +0x60
	Bool m_unk0x64;											// +0x64

protected:
	virtual const char *GetSnapshotName() const;			// 0x00340D6A
	virtual void xfer(Xfer *xfer);							// 0x00340D70: version 3; Int +0x4C, Bool +0x54/+0x55/+0x56, (v2) Int +0x50, the command source +0x58, the sub-machine +0x5C, Real +0x60, Bool +0x64, (v3) Bool +0x57
};
// SIZE 0x68

// AIAttackMoveToState (SIZE 0x70, ctor 0x0034EBE8: AIMoveToState's ctor, table
// 0x00C13548, then an AIAttackMoveStateMachine (name 0xC586154E)).
class AIAttackMoveToState : public AIMoveToState
{
public:
	AIAttackMoveToState(StateMachine *machine);				// 0x0034EBE8
	virtual StateReturnType onEnter();						// 0x0034EC7B
	virtual void onExit(StateExitType status);				// 0x0034A011
	virtual Bool isGuardIdle() const;						// slot 9, 0x00340F25 (true while the attack-move machine is not in an attack state)

	Int m_unk0x50;											// +0x50 (the command source: onEnter stores AIUpdateInterface slot 143's value; version 2 transfers it with XferCommandSourceType 0x00305C02)
	StateMachine *m_attackMoveMachine;						// +0x54: onExit 0x0034A011 resets it to state 0 (setState, slot 8)
	UnsignedInt m_unk0x58;									// +0x58, onEnter 0x0034EC7B zeroes it
	Int m_unk0x5C;											// +0x5C, onEnter sets it to 5
	Coord3D m_unk0x60;										// +0x60, the machine's goal position when it has no goal object
	ObjectID m_unk0x6C;										// +0x6C, the machine's goal object id

protected:
	virtual const char *GetSnapshotName() const;			// 0x00345CFA
	virtual void xfer(Xfer *xfer);							// 0x00340E97: version 2; UnsignedInt +0x58, Int +0x5C, the machine +0x54, Coord3D +0x60, ObjectID +0x6C, Bool +0x4A, (v2) command source +0x50
};
// SIZE 0x70

// AIAttackFollowWaypointPathState (SIZE 0x6C, ctor 0x0034F153: the (machine, Bool,
// Bool) ctor 0x00342BFC, table 0x00C13638, then an AIAttackMoveStateMachine
// (name 0x1B42B558) at +0x68).
class AIAttackFollowWaypointPathState : public AIFollowWaypointPathState
{
public:
	AIAttackFollowWaypointPathState(StateMachine *machine, Bool asGroup);	// 0x0034F153
	virtual StateReturnType onEnter();						// 0x0034F1C4
	virtual void onExit(StateExitType status);				// 0x0034A19F

protected:
	virtual void xfer(Xfer *xfer);							// 0x0034117C: version 1, the waypoint state's xfer, the machine's snapshot
public:

	StateMachine *m_attackFollowMachine;					// +0x68

protected:
	virtual const char *GetSnapshotName() const;			// 0x00345F70
};
// SIZE 0x6C

// AIStartAttackObjState (SIZE 0x20, ctor 0x0033F2FD, CRC-32 0xED821984; table
// 0x00C10E48, whose slot-2 getter is AIBusyState's, folded).
class AIStartAttackObjState : public State
{
public:
	AIStartAttackObjState(StateMachine *machine);			// 0x0033F2FD
	virtual StateReturnType onEnter();						// 0x00346017
	virtual StateReturnType update();						// slot 6 (shared 0x000D43D0)
	virtual Bool isBusy() const;							// slot 10 (shared 0x0050B5C6, true)

protected:
	virtual const char *GetSnapshotName() const;			// slot 2 (AIBusyState's 0x0033F31A, folded)
	virtual void xfer(Xfer *xfer);							// slot 3 (shared empty 0x0047A69C)
};
// SIZE 0x20

// AIMoveOntoWallState (SIZE 0x24, ctor 0x0033FC1B, CRC-32 0x7E4CE41F; table 0x00C11DB0).
class AIMoveOntoWallState : public State
{
public:
	AIMoveOntoWallState(StateMachine *machine);				// 0x0033FC1B
	virtual ~AIMoveOntoWallState();							// deleting 0x00343CBA
	virtual StateReturnType onEnter();						// 0x00343D63
	virtual StateReturnType update();						// 0x0033FC9D

protected:
	virtual const char *GetSnapshotName() const;			// 0x0033FC3C
	virtual void xfer(Xfer *xfer);							// 0x00343CD6

	UnsignedInt m_unk0x20;									// +0x20
};
// SIZE 0x24

// AIHarvestState (SIZE 0x28, ctor 0x0033F56B; table 0x00C113B8).
class AIHarvestState : public State
{
public:
	AIHarvestState(StateMachine *machine);					// 0x0033F56B
	virtual ~AIHarvestState();								// deleting 0x00342E90
	virtual StateReturnType onEnter();						// 0x00341891
	virtual void onExit(StateExitType status);				// 0x003418EA
	virtual StateReturnType update();						// 0x00341770 (the body AIDockState's table shares)

protected:
	virtual const char *GetSnapshotName() const;			// 0x0033F590
	virtual void xfer(Xfer *xfer);							// 0x003417F1

	StateMachine *m_harvestMachine;							// +0x20
	UnsignedInt m_unk0x24;									// +0x24
};
// SIZE 0x28

// AICombineState (SIZE 0x4C, ctor 0x00342ED7; table 0x00C12EE8).
class AICombineState : public AIInternalMoveToState
{
public:
	AICombineState(StateMachine *machine);					// 0x00342ED7

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342EF4
};
// SIZE 0x4C

// The exit and horde-exit variants: their ctors call the base's and store their
// own tables (0x00C11638, 0x00C11698, 0x00C114C0, 0x00C11520), which differ from the
// base's only in the slot-2 getter; the names are those getters' strings.
class AIExitAndMoveToState : public AIExitState
{
public:
	AIExitAndMoveToState(StateMachine *machine, Bool unk);	// 0x0033F67C

protected:
	virtual const char *GetSnapshotName() const;			// 0x0033F698
};
// SIZE 0x28

class AIExitAndFollowPathState : public AIExitState
{
public:
	AIExitAndFollowPathState(StateMachine *machine, Bool unk);	// 0x0033F69E

protected:
	virtual const char *GetSnapshotName() const;			// 0x0033F6BA
};
// SIZE 0x28

class AIHordeExitAndMoveToState : public AIHordeExitState
{
public:
	AIHordeExitAndMoveToState(StateMachine *machine, Bool unk);	// 0x0033F5E3

protected:
	virtual const char *GetSnapshotName() const;			// 0x0033F5FF
};
// SIZE 0x24

class AIHordeExitAndFollowPathState : public AIHordeExitState
{
public:
	AIHordeExitAndFollowPathState(StateMachine *machine, Bool unk);	// 0x0033F605

protected:
	virtual const char *GetSnapshotName() const;			// 0x0033F621
};
// SIZE 0x24

// AIHordeEnterState (SIZE 0x20, ctor 0x0033F596, CRC-32 0x1072431B; table 0x00C11410).
class AIHordeEnterState : public State
{
public:
	AIHordeEnterState(StateMachine *machine);				// 0x0033F596
	virtual StateReturnType onEnter();						// 0x00346214
	virtual void onExit(StateExitType status);				// 0x003462AE
	virtual StateReturnType update();						// 0x003419C7
	virtual Bool unknownQuery11() const;					// slot 11 (shared 0x0050B5C6, true)

protected:
	virtual const char *GetSnapshotName() const;			// 0x0033F5B3
	virtual void xfer(Xfer *xfer);							// slot 3 (shared 0x00306A9B)
};
// SIZE 0x20

// AIPickUpCrateState (SIZE 0x50, ctor 0x00342B87; table 0x00C128D0, whose getter
// returns "AIPickUpCrateState"; the ctor passes 0x67979DC4, CRC-32
// "AIAttackPickUpCrateState").
class AIPickUpCrateState : public AIInternalMoveToState
{
public:
	AIPickUpCrateState(StateMachine *machine);				// 0x00342B87
	virtual StateReturnType update();						// 0x0034DEDE

protected:
	virtual Bool computePath();								// 0x00345B5F

	virtual void xfer(Xfer *xfer);							// 0x00340C2D: version 1, the move state's xfer, Int +0x4C, Coord3D +0x20

	Int m_delayCounter;										// +0x4C (Zero Hour's name): update 0x0034DEDE counts it down

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342BA8
};
// SIZE 0x50

// AIMoveToPositionAndDieState (SIZE 0x50, ctor 0x00342DD2; table 0x00C12CE8).
class AIMoveToPositionAndDieState : public AIInternalMoveToState
{
public:
	AIMoveToPositionAndDieState(StateMachine *machine);		// 0x00342DD2
	virtual StateReturnType onEnter();						// 0x0034FFBE
	virtual StateReturnType update();						// 0x003513C9

protected:
	virtual void xfer(Xfer *xfer);							// 0x00342502: version 1, the move state's xfer, Bool +0x4C
public:
	Bool m_appendGoalPosition;								// +0x4C, onEnter 0x0034FFBE sets it

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342DF3
};
// SIZE 0x50

// Built on AIMoveToState's ctor 0x0034005D: AIMoveToPositionAndEnterState (ctor
// 0x00346CDD, table 0x00C136A0: 4 onEnter 0x00350020, 5 onExit 0x0034C035, 6 update
// 0x00354945), AIMoveToAndEvacuateState (0x00342DF9, table 0x00C12D50; derived by
// AIAttackMoveToAndEvacuateState 0x00342E17, table 0x00C12DB8), AIMoveForBoarding
// (0x00343077, table 0x00C13108).
class AIMoveToPositionAndEnterState : public AIMoveToState
{
public:
	AIMoveToPositionAndEnterState(StateMachine *machine);	// 0x00346CDD
	virtual StateReturnType onEnter();						// 0x00350020
	virtual void onExit(StateExitType status);				// 0x0034C035
	virtual StateReturnType update();						// 0x00354945

protected:
	virtual const char *GetSnapshotName() const;			// 0x00346CF5
};
// SIZE 0x50

class AIMoveToAndEvacuateState : public AIMoveToState
{
public:
	AIMoveToAndEvacuateState(StateMachine *machine);		// 0x00342DF9
	virtual void onExit(StateExitType status);				// 0x0034C098

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342E11
};
// SIZE 0x50

class AIAttackMoveToAndEvacuateState : public AIMoveToAndEvacuateState
{
public:
	AIAttackMoveToAndEvacuateState(StateMachine *machine);	// 0x00342E17

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342E2F
};
// SIZE 0x50

class AIMoveForBoarding : public AIMoveToState
{
public:
	AIMoveForBoarding(StateMachine *machine);				// 0x00343077
	virtual StateReturnType onEnter();						// 0x00350164: chooses the evacuate locomotor set, then AIMoveToState::onEnter

protected:
	virtual const char *GetSnapshotName() const;			// 0x0034308F
};
// SIZE 0x50

// AIPrepareForBoarding (SIZE 0x20, ctor 0x00342619, CRC-32 0x9B3AE6B6; table
// 0x00C123D8) and AIRotateFiringarc (SIZE 0x20, ctor 0x003426AB, CRC-32 0x9AA7C3F8;
// table 0x00C12438).
class AIPrepareForBoarding : public State
{
public:
	AIPrepareForBoarding(StateMachine *machine);			// 0x00342619
	virtual StateReturnType onEnter();						// 0x0034263C
	virtual StateReturnType update();						// 0x00354C6B

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342636
	virtual void xfer(Xfer *xfer);							// slot 3 (shared 0x00306A9B)
};
// SIZE 0x20

class AIRotateFiringarc : public State
{
public:
	AIRotateFiringarc(StateMachine *machine);				// 0x003426AB
	virtual StateReturnType onEnter();						// 0x003426CE
	virtual void onExit(StateExitType status);				// 0x0034270C
	virtual StateReturnType update();						// 0x0034274A

protected:
	virtual const char *GetSnapshotName() const;			// 0x003426C8
	virtual void xfer(Xfer *xfer);							// slot 3 (shared 0x00306A9B)
};
// SIZE 0x20

// ---------------------------------------------------------------------------
// States other machines build (attack, horde, cower and wait-for-members machines).
// Names: each class's slot-2 getter returns its name (`mov eax, offset str; ret`);
// bases: the constructor each one calls first. Only the constructor and the
// getter are declared so far; the remaining slots are still to be read from
// each table.
class AIWaitUntilMembersFinishedBeforeExitState : public State
{
public:
	AIWaitUntilMembersFinishedBeforeExitState(StateMachine *machine);	// 0x0033F460 (CRC-32 0xFD797D2A)

protected:
	virtual const char *GetSnapshotName() const;			// 0x0033F47D
};

class AIAttackFireWeaponState : public State
{
public:
	AIAttackFireWeaponState(StateMachine *machine, Int unk);	// 0x0033F483 (CRC-32 0x7252E66D)
	virtual StateReturnType onEnter();				// slot 4, 0x0034A879 (decl_queue f01 17:58, ACCEPTED k11)
	virtual void onExit(StateExitType status);				// 0x0034B08A
	virtual StateReturnType update();				// slot 6, 0x0034AAC8 (decl_queue f01 17:58, ACCEPTED k11)

protected:
	virtual const char *GetSnapshotName() const;			// 0x0033F4AB
	virtual void xfer(Xfer *xfer);				// slot 3 (shared empty 0x0047A69C; decl_queue f01 18:08, ACCEPTED k11)

public:
	Int m_unk0x20;				// +0x20: the constructor 0x0033F483 stores its second argument here (decl_queue f01 18:42, ACCEPTED k11)
	Bool m_unk0x24;				// +0x24: the constructor 0x0033F483 clears it
};
// SIZE 0x28

// Its ctor passes the same name hash as AIAttackFireWeaponState; the getter names it.
class AIAttackPositionFireWeaponState : public State
{
public:
	AIAttackPositionFireWeaponState(StateMachine *machine, Int unk);	// 0x0033F4B1

protected:
	virtual const char *GetSnapshotName() const;			// 0x0033F4D9
};

class AIMeleeReAcquireState : public State
{
public:
	AIMeleeReAcquireState(StateMachine *machine);			// 0x0033F627 (CRC-32 0x16D9EC7E)

protected:
	virtual const char *GetSnapshotName() const;			// 0x0033F648
};

// AIAttackApproachTargetState (SIZE 0x74). Target facts: ctor 0x0034290D passes
// CRC-32 0xCC44C7B1 to AIInternalMoveToState's, stores table 0x00C12610 (slots: 2
// GetSnapshotName 0x00342972, 3 xfer 0x0034048B, 4 onEnter 0x0034CD3A, 5 onExit
// 0x0034894C, 6 update 0x003488AF, 8 the shared Bool getter 0x002259EF that
// returns the byte at +0x71, 17 computePath 0x0034804E), zeroes the Coord3Ds at
// +0x4C and +0x58 and the UnsignedInts at +0x64 and +0x68 and stores its three Bool
// parameters at +0x6C, +0x6D and +0x70 with +0x6E and +0x71 false and +0x6F true.
// xfer 0x0034048B lists the members in a different order (see it). Zero Hour's
// AIStateMachine.h is the name donor for the members it has.
class AIAttackApproachTargetState : public AIInternalMoveToState
{
public:
	AIAttackApproachTargetState(StateMachine *machine, Bool follow, Bool attackingObject, Bool forceAttacking)	// 0x0034290D
		: AIInternalMoveToState(machine, 0xCC44C7B1), m_follow(follow), m_isAttackingObject(attackingObject), m_stopIfInRange(false), m_isInitialApproach(true), m_isForceAttacking(forceAttacking), m_unk0x71(false)
	{
		m_prevVictimPos.x = 0;
		m_prevVictimPos.y = 0;
		m_prevVictimPos.z = 0;
		m_unk0x58.x = 0;
		m_unk0x58.y = 0;
		m_unk0x58.z = 0;
		m_approachTimestamp = 0;
		m_unk0x68 = 0;
	}
	virtual StateReturnType onEnter();						// 0x0034CD3A
	virtual void onExit(StateExitType status);				// 0x0034894C
	virtual StateReturnType update();						// 0x003488AF
	virtual Bool isAttack() const { return m_unk0x71; }		// slot 8, the shared getter 0x002259EF

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342972
	virtual void xfer(Xfer *xfer);							// 0x0034048B
	virtual Bool computePath();								// 0x0034804E
	StateReturnType updateInternal();						// 0x00348356

	Coord3D m_prevVictimPos;								// +0x4C
	Coord3D m_unk0x58;										// +0x58
	UnsignedInt m_approachTimestamp;						// +0x64
	UnsignedInt m_unk0x68;									// +0x68
	Bool m_follow;											// +0x6C
	Bool m_isAttackingObject;								// +0x6D
	Bool m_stopIfInRange;									// +0x6E
	Bool m_isInitialApproach;								// +0x6F
	Bool m_isForceAttacking;								// +0x70
	Bool m_unk0x71;											// +0x71
};
// SIZE 0x74

class AIAttackMeleeHordeApproachTargetState : public AIInternalMoveToState
{
public:
	AIAttackMeleeHordeApproachTargetState(StateMachine *machine);	// 0x003429B9 (CRC-32 0xFA014ECE)
	virtual void onExit(StateExitType status);				// 0x0034921C

protected:
	virtual const char *GetSnapshotName() const;			// 0x003429F4
	virtual void xfer(Xfer *xfer);							// 0x003406A5: version 3; Coord3D +0x4C, UnsignedInt +0x58, (v2) Bool +0x5C, (v3) Bool +0x5D

	Coord3D m_unk0x4C;										// +0x4C
	UnsignedInt m_unk0x58;									// +0x58
	Bool m_unk0x5C;											// +0x5C (version 2)
	Bool m_unk0x5D;											// +0x5D (version 3)
};

// AIAttackPursueTargetState (SIZE 0x64). Target facts: ctor 0x003429FA passes
// CRC-32 0xD69031BF to AIInternalMoveToState's, stores table 0x00C12730 (slots:
// 2 GetSnapshotName 0x00342A4A, 3 xfer 0x00340797, 4 onEnter 0x0034D345, 5 onExit
// 0x003495FD, 6 update 0x003495A1, 17 computePath 0x00345246), zeroes the Coord3D
// +0x4C and the UnsignedInt +0x58 and stores its three Bool parameters at +0x5C,
// +0x5D and +0x60 with +0x5E false and +0x5F true. xfer 0x00340797 lists them in
// that order. Zero Hour's AIStateMachine.h is the name donor.
class AIAttackPursueTargetState : public AIInternalMoveToState
{
public:
	AIAttackPursueTargetState(StateMachine *machine, Bool follow, Bool attackingObject, Bool forceAttacking)	// 0x003429FA
		: AIInternalMoveToState(machine, 0xD69031BF), m_follow(follow), m_isAttackingObject(attackingObject), m_stopIfInRange(false), m_isInitialApproach(true), m_isForceAttacking(forceAttacking)
	{
		m_prevVictimPos.x = 0;
		m_prevVictimPos.y = 0;
		m_prevVictimPos.z = 0;
		m_approachTimestamp = 0;
	}
	virtual StateReturnType onEnter();						// 0x0034D345
	virtual void onExit(StateExitType status);				// 0x003495FD
	virtual StateReturnType update();						// 0x003495A1

protected:
	virtual const char *GetSnapshotName() const;			// 0x00342A4A
	virtual void xfer(Xfer *xfer);							// 0x00340797
	virtual Bool computePath();								// 0x00345246
	StateReturnType updateInternal();						// 0x0034939E

	Coord3D m_prevVictimPos;								// +0x4C
	UnsignedInt m_approachTimestamp;						// +0x58
	Bool m_follow;											// +0x5C
	Bool m_isAttackingObject;								// +0x5D
	Bool m_stopIfInRange;									// +0x5E
	Bool m_isInitialApproach;								// +0x5F
	Bool m_isForceAttacking;								// +0x60
};
// SIZE 0x64

class AIAttackMeleeEngageState : public AIInternalMoveToState
{
public:
	AIAttackMeleeEngageState(StateMachine *machine, Int unk);	// 0x00342B1A (CRC-32 0x26AB28F6)
	virtual void onExit(StateExitType status);				// 0x00349D26

protected:
	virtual const char *GetSnapshotName() const;			// 0x00340C27
};

// Built on AIMoveAndTightenState's named ctor 0x0034286E, like the move-away states.
class AIBackAwayState : public AIMoveAndTightenState
{
public:
	AIBackAwayState(StateMachine *machine);					// 0x0034301B
	virtual void onExit(StateExitType status);				// 0x00347FFE

protected:
	virtual const char *GetSnapshotName() const;			// 0x0034303C
	virtual void xfer(Xfer *xfer);							// 0x00340461: version 1; Bool +0x54, then AIMoveAndTightenState's
	virtual Bool computePath();								// 0x0034042C (AIMoveAwayPanicState::computePath's twin, "ComputePath9")

	Bool m_unk0x54;											// +0x54
};

class AIChargeTargetState : public AIInternalMoveToState
{
public:
	AIChargeTargetState(StateMachine *machine);				// 0x00343042
	virtual void onExit(StateExitType status);				// 0x0034BEC3

protected:
	virtual const char *GetSnapshotName() const;			// 0x00343071
	virtual void xfer(Xfer *xfer);							// 0x003424B1: version 1; Bool +0x55, Bool +0x54, Bool +0x4C, Int +0x50, then AIInternalMoveToState's

	Bool m_unk0x4C;											// +0x4C
	Int m_unk0x50;											// +0x50
	Bool m_unk0x54;											// +0x54
	Bool m_unk0x55;											// +0x55
};

#endif
