// AIGuardRetaliate.h: the guard-retaliate state machine (Zero Hour AIGuardRetaliate.h names).
// AIGuardRetaliateMachine (SIZE 0x4C). Target facts: ctor 0x0054551F calls
// StateMachine(owner, 0xD5DFA9B7, false), zeroes the Coord3D at +0x3C and the
// ObjectID at +0x48, then defines its states. xfer 0x0054528D: return on a
// light CRC (Xfer slot 4), Version1, StateMachine::xfer, XferObjectID +0x48,
// Coord3D +0x3C. Dtor 0x00545211 stores table 0x00C69E78 and jumps to
// ~StateMachine. Table 0x00C69E78 overrides slots 0, 2 and 3.
// AIGuardRetaliateReturnState (SIZE 0x50, `new 0x50` in the machine ctor):
// ctor 0x005453BB calls AIInternalMoveToState(machine, 0xC784A170), zeroes
// +0x4C and stores table 0x00C69F38 (onEnter 0x005452CB, update 0x0054574D;
// onExit and xfer are shared bodies 0x0034BA5F and 0x00542D06).
#pragma once
#ifndef BFME2_AIGUARDRETALIATE_H
#define BFME2_AIGUARDRETALIATE_H

#include "Common/StateMachine.h"
#include "GameLogic/AIStateMachine.h"

// State IDs (the guard machines share them; AITNGuard's ctor uses 5000..5005).
enum
{
	AI_GUARD_RETALIATE_RETURN = 5003,
	AI_GUARD_RETALIATE_ATTACK_AGGRESSOR = 5005
};

// GuardRetaliateExitConditions (Zero Hour AIGuardRetaliate.h): the attack-exit test of the retaliate states,
// the same layout as AIGuard.h's ExitConditions. Target facts: AIGuardRetaliateAttackAggressorState's ctor
// 0x005454D0 stores its one-slot table 0x00C69F34 at +0x20 and zeroes the conditions (+0x24), the centre
// (+0x28..+0x30), the radius squared (+0x34) and the give-up frame (+0x38); shouldExit 0x005453E2 tests the
// give-up frame (bit 2), then the goal's distance to the centre against the radius squared and the owner's
// against the standard guard range (bit 1).
class GuardRetaliateExitConditions : public AttackExitConditionsInterface
{
public:
	enum ExitConditionsEnum
	{
		ATTACK_ExitIfOutsideRadius = 0x01,
		ATTACK_ExitIfExpiredDuration = 0x02,
		ATTACK_ExitIfNoUnitFound = 0x04
	};

	GuardRetaliateExitConditions() : m_attackGiveUpFrame(0), m_conditionsToConsider(0), m_radiusSqr(0.0f) { m_center.zero(); }
	virtual Bool shouldExit(const StateMachine *machine) const;	// slot 0, 0x005453E2

	Int m_conditionsToConsider;			// +0x04
	Coord3D m_center;					// +0x08, can be updated at any time by the owner
	Real m_radiusSqr;					// +0x14
	UnsignedInt m_attackGiveUpFrame;	// +0x18, the frame at which the attack gives up
};
// SIZE 0x1C

class AIGuardRetaliateMachine : public StateMachine
{
public:
	const char *name() const;				// 0x0054521C
	AIGuardRetaliateMachine(Object *owner);		// 0x0054551F
	virtual ~AIGuardRetaliateMachine();			// 0x00545211
	// Slot 16 (table 0x00C69E78), the one virtual this machine adds: true when the
	// current state's ID is 0 (0x00545222). Provisional name; no string names it.
	virtual Bool isCurrentStateZero() const;	// 0x00545222

	// The return state's condition: retail's test is a folded `return false` body (0x0047A699).
	static Bool shouldAttackAggressor(State *thisState, void *userData);	// provisional name

	Bool lookForInnerTarget();					// 0x005455E3
	static Real getStdGuardRange(const Object *obj);	// 0x00545239 (the map files it as a free function): the AI data's retaliate distance, 1.15 times it for a melee weapon
	void setNemesisID(ObjectID id) { m_nemesisToAttack = id; }
	ObjectID getNemesisID() const { return m_nemesisToAttack; }
	void setTargetPositionToGuard(const Coord3D *pos) { m_positionToGuard = *pos; }	// inline in AIGuardRetaliateState::onEnter 0x003463DA
	const Coord3D *getPositionToGuard() const { return &m_positionToGuard; }	// inline in AIGuardRetaliateReturnState::onEnter 0x005452CB

protected:
	virtual const char *GetSnapshotName() const;	// slot 2, 0x0054521C (the map files it as a non-virtual name())
	virtual void xfer(Xfer *xfer);				// slot 3, 0x0054528D

	Coord3D m_positionToGuard;					// +0x3C
	ObjectID m_nemesisToAttack;					// +0x48
};
// SIZE 0x4C

// AIGuardReturnState (SIZE 0x50; +0x4C is the next scan frame its xfer 0x00542D06 and update 0x00543DF1 use): retail's table 0x00869830 overrides xfer
// 0x00542D06, onEnter 0x00543CC4, update 0x00543DF1 and GetSnapshotName 0x00542ED2, and its onExit is
// the folded forwarder 0x0034BA5F (a jump to AIInternalMoveToState::onExit, the map's
// AIMoveToState::onExit). AIMoveToState itself is 0x50 bytes (ctor 0x0034005D), so this 0x4C state
// derives AIInternalMoveToState. AIGuardRetaliateReturnState derives from it (table 0x00869F38 keeps
// 0x00542D06 and 0x0034BA5F).
class AIGuardReturnState : public AIInternalMoveToState
{
public:
	AIGuardReturnState(StateMachine *machine, UnsignedInt name) : AIInternalMoveToState(machine, name) {}
	virtual StateReturnType onEnter();					// 0x00543CC4
	virtual void onExit(StateExitType status);			// slot 5, 0x0034BA5F (folded forwarder)
	virtual StateReturnType update();					// 0x00543DF1

protected:
	virtual const char *GetSnapshotName() const;		// 0x00542ED2
	virtual void xfer(Xfer *xfer);						// 0x00542D06

	UnsignedInt m_nextReturnScanTime;					// +0x4C (Zero Hour's name): xfer 0x00542D06 transfers it, update 0x00543DF1 reschedules it
};
// SIZE 0x50

class AIGuardRetaliateReturnState : public AIGuardReturnState
{
public:
	AIGuardRetaliateReturnState(StateMachine *machine);	// 0x005453BB
	virtual StateReturnType onEnter();					// 0x005452CB
	virtual StateReturnType update();					// 0x0054574D

protected:
	virtual const char *GetSnapshotName() const;	// 0x005453DC
};
// SIZE 0x50

// AIGuardRetaliateAttackAggressorState (SIZE 0x44, `new 0x44`): ctor 0x005454D0 calls
// State(machine, 0x9CB27A45), stores table 0x00C69FA0, an exit-conditions object at +0x20
// (table 0x00C69F34, shouldExit 0x005453E2) and zeroes +0x24..+0x40. onExit is
// AIGuardAttackAggressorState's body (0x00542C87). Members are not recovered yet.
class AIGuardRetaliateAttackAggressorState : public State
{
public:
	AIGuardRetaliateAttackAggressorState(StateMachine *machine);	// 0x005454D0
	virtual StateReturnType onEnter();			// 0x0054587A
	virtual void onExit(StateExitType status);	// 0x00542C87 (shared)
	virtual StateReturnType update();			// 0x00545A46

	Object *findHordeTarget();					// 0x0054582A (the map files it as rva0054582A): the horde interface of the guard-kind nemesis chooses a unit near the owner

protected:
	virtual const char *GetSnapshotName() const;	// 0x00545519
	virtual void xfer(Xfer *xfer);				// table slot 3 = 0x00306A9B (shared)

	AIGuardRetaliateMachine *getGuardMachine() { return (AIGuardRetaliateMachine *)getMachine(); }

	GuardRetaliateExitConditions m_exitConditions;	// +0x20
	AIAttackState *m_attackState;				// +0x3C
	ObjectID m_unk0x40;							// +0x40, set when the nemesis is a guard-kind unit (onEnter 0x0054587A, rva0054582A)
};
// SIZE 0x44

#endif
