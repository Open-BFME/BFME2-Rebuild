// AIGuard.h: the guard state machine (Zero Hour AIGuard.h names).
// AIGuardMachine (SIZE 0x74: AIStateMachine's factory slot 0x0034145C allocates
// 0x74 and calls the ctor 0x00543163 with (owner, 0x0F9E47F7)).
// Target facts: ctor 0x00543163 zeroes +0x3C/+0x40/+0x44, the two Coord3Ds at
// +0x48/+0x54, the Bool +0x60, +0x64 and +0x68..+0x70, then defines its states.
// xfer 0x00542FC5: return on a light CRC, StateMachine::xfer, version 3,
// XferObjectID +0x3C, UnsignedInt +0x40, XferObjectID +0x68, Coord3D +0x48 and
// +0x54, Bool +0x60, (v2) 4 raw bytes +0x6C and UnsignedInt +0x70, the trigger
// area at +0x44 by name (its AsciiString at trigger +0x40), (v3) Real +0x64.
// Dtor 0x00542C19 stores table 0x00C69680 and jumps to ~StateMachine.
#pragma once
#ifndef BFME2_AIGUARD_H
#define BFME2_AIGUARD_H

#include "Common/StateMachine.h"
#include "GameLogic/AICommandInterface.h"
#include "GameLogic/AIStateMachine.h"
#include "Common/Coord3D.h"

class Object;
class PolygonTrigger;

class AIGuardMachine : public StateMachine
{
public:
	AIGuardMachine(Object *owner, UnsignedInt name);	// 0x00543163
	virtual ~AIGuardMachine();					// 0x00542C19

	static Real getStdGuardRange(const Object *obj);	// 0x00542C2A
	Bool lookForInnerTarget();							// 0x005433FA
	Coord3D findPositionToGuard() const;				// 0x00543326 (the map row is a placeholder): the guarded object's, team's or fixed position, the area's centre when an area is set; the owner's position when that is the origin
	void setTargetPositionToGuard(const Coord3D *pos, Real speed);	// 0x0033F867
	void setBfmeAreaCenter(const Coord3D *pos);			// 0x0033F883
	GuardMode getGuardMode() const { return m_guardMode; }
	const PolygonTrigger *getAreaToGuard() const { return m_areaToGuard; }
	const Coord3D *getPositionToGuard() const { return &m_positionToGuard; }
	Bool hasBfmeAreaCenter() const { return m_unk0x60; }
	const Coord3D *getBfmeAreaCenter() const { return &m_unk0x54; }
	Real getGuardRadius() const { return m_unk0x64; }
	ObjectID getNemesisID() const { return m_nemesisToAttack; }
	void setNemesisID(ObjectID id) { m_nemesisToAttack = id; }
	ObjectID getTargetToGuardID() const { return m_targetToGuard; }		// the states look the object up through GameLogic::findObjectByID in place
	UnsignedInt getTeamToGuardID() const { return m_teamToGuard; }			// +0x40: the states look the team up through TeamFactory::findTeamByID in place

protected:
	virtual const char *GetSnapshotName() const;	// slot 2, 0x00542C24
	virtual void xfer(Xfer *xfer);				// slot 3, 0x00542FC5

	ObjectID m_targetToGuard;					// +0x3C
	UnsignedInt m_teamToGuard;					// +0x40 (a team ID: findPositionToGuard 0x00543326 looks it up with TeamFactory::findTeamByID)
	const PolygonTrigger *m_areaToGuard;		// +0x44
	Coord3D m_positionToGuard;					// +0x48
	Coord3D m_unk0x54;							// +0x54
	Bool m_unk0x60;								// +0x60
	Real m_unk0x64;								// +0x64, version 3
	ObjectID m_nemesisToAttack;					// +0x68
	GuardMode m_guardMode;						// +0x6C, version 2, 4 raw bytes
	UnsignedInt m_unk0x70;						// +0x70, version 2
};
// SIZE 0x74


// The guard machine's states (each name is its slot-2 getter's string; each ctor calls
// State's). Only the ctor and the getter are declared so far.
// ExitConditions (Zero Hour AIGuard.h): the guard states' attack-exit test, a
// member of AIGuardInnerState at +0x20 (its ctor 0x00542E13 stores the table
// 0x00C69778 there). Offsets from that ctor and from shouldExit 0x00542F43
// (conditions +0x04, centre +0x08, radius squared +0x14, give-up frame +0x18).
class ExitConditions : public AttackExitConditionsInterface
{
public:
	enum ExitConditionsEnum
	{
		ATTACK_ExitIfOutsideRadius = 0x01,
		ATTACK_ExitIfExpiredDuration = 0x02,
		ATTACK_ExitIfNoUnitFound = 0x04
	};

	ExitConditions() : m_attackGiveUpFrame(0), m_conditionsToConsider(0), m_radiusSqr(0.0f) { m_center.zero(); }
	virtual Bool shouldExit(const StateMachine *machine) const;	// slot 0, 0x00542F43

	Int m_conditionsToConsider;			// +0x04
	Coord3D m_center;					// +0x08
	Real m_radiusSqr;					// +0x14
	UnsignedInt m_attackGiveUpFrame;	// +0x18
};
// SIZE 0x1C

class AIGuardIdleState : public State
{
public:
	AIGuardIdleState(StateMachine *machine);		// 0x00542D25 (CRC-32 0x08F11452)
	virtual StateReturnType onEnter();			// 0x00542D88
	virtual StateReturnType update();			// 0x00543E5D

protected:
	virtual const char *GetSnapshotName() const;	// 0x00542D58
	virtual void xfer(Xfer *xfer);				// 0x00542D5E (AITNGuardIdleState's slot 3 folds to it)

	UnsignedInt m_nextEnemyScanTime;			// +0x20 (Zero Hour name; onEnter sets it from the AI data's guard scan rate)
	Coord3D m_guardeePos;						// +0x24 (Zero Hour name; the ctor zeroes it, xfer transfers it)
};
// SIZE 0x30

class AIAttackState;
class AIGuardInnerState : public State
{
public:
	AIGuardInnerState(StateMachine *machine);		// 0x00542E13
	virtual StateReturnType onEnter();			// 0x0054379B
	virtual StateReturnType update();			// 0x005438D6

protected:
	virtual const char *GetSnapshotName() const;	// 0x00542E5C
	virtual void xfer(Xfer *xfer);				// 0x00542C38

	ExitConditions m_exitConditions;			// +0x20
	AIAttackState *m_attackState;				// +0x3C (xfer transfers the state when there is one)
	Bool m_scanForEnemy;						// +0x40 (Zero Hour name; the ctor clears it)
	UnsignedInt m_unk0x44;						// +0x44, version 2
};

class AIGuardOuterState : public State
{
public:
	AIGuardOuterState(StateMachine *machine);		// 0x00542E62
	virtual StateReturnType onEnter();			// 0x005439F2
	virtual StateReturnType update();			// 0x00543B5A

protected:
	virtual const char *GetSnapshotName() const;	// 0x00542EAB

	ExitConditions m_exitConditions;			// +0x20
	AIAttackState *m_attackState;				// +0x3C
	Bool m_scanForEnemy;						// +0x40 (restart flag: update clears it and re-enters)
};
// SIZE 0x44

// AIGuardRetaliateAttackAggressorState reuses its onExit 0x00542C87.
class AIGuardAttackAggressorState : public State
{
public:
	virtual StateReturnType onEnter();			// 0x00543F58

protected:
	virtual const char *GetSnapshotName() const;	// 0x00542F3D

	ExitConditions m_exitConditions;			// +0x20
	AIAttackState *m_attackState;				// +0x3C
	Bool m_scanForEnemy;						// +0x40 (restart flag: onEnter clears it and re-enters)
};
// SIZE 0x44

#endif
