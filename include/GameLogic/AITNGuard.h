// AITNGuard.h: the tunnel-network guard state machine (Zero Hour AITNGuard.h names).
//
// AITNGuardMachine (SIZE 0x50). Target facts: ctor 0x00546001 calls
// StateMachine(owner, 0x91632A45, false), stores table 0x00C6A090, zeroes
// +0x3C..+0x44 (Coord3D), +0x48 and +0x4C, then defines states 5000-5005.
// xfer 0x00545EB2: StateMachine::xfer, version 2, XferObjectID +0x48,
// Coord3D +0x3C, then (version >= 2) 4 raw bytes +0x4C. Dtor 0x00545B84 stores
// the table and jumps to ~StateMachine. Table 0x00C6A090 overrides only slots
// 0, 2 (GetSnapshotName 0x00545B8F) and 3 (xfer).
//
// AITNGuardInnerState (SIZE 0x30, `new 0x30` in the machine ctor). Target facts:
// ctor 0x00545D50 calls State(machine, 0xD405C261), stores table 0x00C6A140,
// then the exit conditions (table 0x00C6A13C at +0x20, +0x24 = 0). xfer
// 0x00545B95: Version1, Bool +0x28. onExit 0x00545BB7 exits and frees the
// attack state at +0x2C. Table 0x00C6A140 overrides slots 0-6;
// loadPostProcess 0x005CB26A tail-calls onEnter. onExit 0x00545BB7 exits the
// attack state and `::delete`s it.
//
// AITNGuardOuterState (SIZE 0x2C, table 0x00C6A198) and
// AITNGuardAttackAggressorState (SIZE 0x2C, table 0x00C6A298): ctors 0x00545D7E
// and 0x00545E80 call State(machine, 0xCDECC1F9 / 0x33E96013) and store the
// tunnel exit conditions at +0x20 and a null attack state at +0x28.
// AITNGuardIdleState (SIZE 0x30: table 0x00C6A038, ctor 0x00545B4F only calls
// State(machine, 0xED015B70), `new 0x30`). onEnter 0x00545CAF sets the next
// enemy scan frame at +0x20 from TheAI's GuardEnemyScanRate.
#pragma once
#ifndef BFME2_AITNGUARD_H
#define BFME2_AITNGUARD_H

#include "Common/StateMachine.h"
#include "GameLogic/AIStateMachine.h"
#include "GameLogic/AICommandInterface.h"

class AIAttackState;
class Player;

class TunnelNetworkExitConditions : public AttackExitConditionsInterface
{
public:
	TunnelNetworkExitConditions() : m_attackGiveUpFrame(0) {}
	virtual Bool shouldExit(const StateMachine *machine) const;	// slot 0, 0x00545B72

	UnsignedInt m_attackGiveUpFrame;	// +0x04
};
// SIZE 0x8

class AITNGuardInnerState;

class AITNGuardMachine : public StateMachine
{
public:
	AITNGuardMachine(Object *owner);			// 0x00546001
	friend class AITNGuardInnerState;
	friend class AITNGuardOuterState;
	virtual ~AITNGuardMachine();				// 0x00545B84

	Bool lookForInnerTarget();					// 0x005461A2
	void setNemesisID(ObjectID id) { m_nemesisToAttack = id; }
	ObjectID getNemesisID() const { return m_nemesisToAttack; }
	void setTargetPositionToGuard(const Coord3D *pos) { m_positionToGuard = *pos; }	// inline in AITunnelNetworkGuardState::onEnter 0x00342086
	void setGuardMode(GuardMode mode) { m_guardMode = mode; }

protected:
	virtual const char *GetSnapshotName() const;	// slot 2, 0x00545B8F
	virtual void xfer(Xfer *xfer);				// slot 3, 0x00545EB2

	Coord3D m_positionToGuard;					// +0x3C
	ObjectID m_nemesisToAttack;					// +0x48
	GuardMode m_guardMode;						// +0x4C, version 2
};
// SIZE 0x50

class AITNGuardInnerState : public State
{
public:
	AITNGuardInnerState(StateMachine *machine);	// 0x00545D50 (out of line: the machine ctor calls it)
	virtual StateReturnType onEnter();			// slot 4, 0x0054632D
	virtual void onExit(StateExitType status);	// slot 5, 0x00545BB7
	virtual StateReturnType update();			// slot 6, 0x005463C9
	AITNGuardMachine *getGuardMachine() { return (AITNGuardMachine *)getMachine(); }

protected:
	virtual void loadPostProcess();				// slot 1, 0x005CB26A
	virtual const char *GetSnapshotName() const;	// slot 2, 0x00545D78
	virtual void xfer(Xfer *xfer);				// slot 3, 0x00545B95

	TunnelNetworkExitConditions m_exitConditions;	// +0x20
	Bool m_scanForEnemy;						// +0x28
	AIAttackState *m_attackState;				// +0x2C
};
// SIZE 0x30

// AITNGuardReturnState (SIZE 0x58, `new 0x58` in the machine ctor): ctor 0x00545DB0
// calls AIEnterState's, zeroes +0x54 and stores table 0x00C6A1F0. update 0x00545C4D
// hands the team's target or the tunnel system's nemesis to the machine and fails
// (so the machine attacks); otherwise it runs AIEnterState::update and succeeds when
// that stops continuing. xfer 0x00545C1D: AIEnterState::xfer, then (not light CRC)
// Version1 and UnsignedInt +0x54.
class AITNGuardReturnState : public AIEnterState
{
public:
	AITNGuardReturnState(StateMachine *machine);	// 0x00545DB0
	virtual StateReturnType onEnter();				// 0x005465E5
	virtual StateReturnType update();				// 0x00545C4D

	AITNGuardMachine *getGuardMachine() { return (AITNGuardMachine *)getMachine(); }

protected:
	virtual void xfer(Xfer *xfer);					// 0x00545C1D

	UnsignedInt m_nextReturnScanTime;				// +0x54

protected:
	virtual const char *GetSnapshotName() const;	// 0x00545DCC
};
// SIZE 0x58

class AITNGuardIdleState : public State
{
public:
	AITNGuardIdleState(StateMachine *machine);	// 0x00545B4F
	virtual StateReturnType onEnter();			// 0x00545CAF
	virtual StateReturnType update();			// 0x0054665B

	AITNGuardMachine *getGuardMachine() { return (AITNGuardMachine *)getMachine(); }

protected:
	virtual const char *GetSnapshotName() const;	// 0x00545B6C
	virtual void xfer(Xfer *xfer);				// 0x00542D5E (shared)

	UnsignedInt m_nextEnemyScanTime;			// +0x20
	UnsignedByte m_unk0x24[0x30 - 0x24];		// +0x24, not touched by the constructor
};
// SIZE 0x30

class AITNGuardOuterState : public State
{
public:
	AITNGuardOuterState(StateMachine *machine);	// 0x00545D7E
	virtual StateReturnType onEnter();			// 0x005464CE
	virtual void onExit(StateExitType status);	// 0x00545BEA
	virtual StateReturnType update();			// 0x00546577

protected:
	virtual void loadPostProcess();				// 0x005CB26A (shared)
	virtual const char *GetSnapshotName() const;	// 0x00545DAA
	virtual void xfer(Xfer *xfer);				// 0x00306A9B (shared)

	TunnelNetworkExitConditions m_exitConditions;	// +0x20
	AIAttackState *m_attackState;				// +0x28
};
// SIZE 0x2C

class AITNGuardAttackAggressorState : public State
{
public:
	AITNGuardAttackAggressorState(StateMachine *machine);	// 0x00545E80
	virtual StateReturnType onEnter();			// 0x0054675D
	virtual void onExit(StateExitType status);	// 0x00545CF7
	virtual StateReturnType update();			// 0x0054684E

	AITNGuardMachine *getGuardMachine() { return (AITNGuardMachine *)getMachine(); }

protected:
	virtual const char *GetSnapshotName() const;	// 0x00545EAC
	TunnelNetworkExitConditions m_exitConditions;	// +0x20
	AIAttackState *m_attackState;				// +0x28
};
// SIZE 0x2C

#endif
