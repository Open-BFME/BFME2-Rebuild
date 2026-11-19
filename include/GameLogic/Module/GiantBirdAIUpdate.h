// GiantBirdAIUpdate.h: the giant bird AI update module and its state machines.
// Target facts:
// - GiantBirdAIUpdate (SIZE 0x57C: friend_newModuleInstance 0x0024D2EF allocates
//   0x57C): ctor 0x0036B717 calls AIUpdateInterface's, stores tables 0x00C17A80
//   (+0), 0x00C17A70 (+0x10), 0x00C17A6C (+0x20), keeps 0x00BFA3C0 (+0xC) and
//   stores 0x00C17A54 (+0x24, its own onCollide 0x003698CD). xfer 0x0036AC73:
//   version, AIUpdateInterface::xfer, then (not light CRC) the 0xC4-byte member
//   at +0x3F0 (the same type as AIUpdateInterface +0x2EC: init 0x0026AFDA, xfer
//   0x003533BF), Coord3D +0x3E4, the list at +0x4B4, Int +0x4B8, Real +0x4BC,
//   ObjectIDs +0x4C0/+0x4C4, 4 raw bytes +0x528, Real +0x530, ObjectID +0x554,
//   Bool +0x558, 4 raw bytes +0x55C, Bools +0x534/+0x550, Coord3D +0x544, Reals
//   +0x52C/+0x538/+0x53C/+0x540, Coord3Ds +0x560/+0x56C, Bool +0x578. The
//   member at +0x4C8 (0x60 bytes) is built by ctor 0x00312C95.
// - GiantBirdGuardMachine: dtor 0x00367F8D stores table 0x00C17768 and jumps
//   to ~AIGuardMachine 0x00542C19; xfer 0x00367FDD: Version1, then
//   AIGuardMachine::xfer unless the Xfer is a light CRC.
// - AIAttackSwoopThenIdleStateMachine: dtor 0x003677B3 stores table 0x00C174E0
//   and jumps to ~StateMachine; ctor 0x003676A2 (Object *, UnsignedInt).
// - GiantBirdNormalFlightState (SIZE 0x24): ctor 0x00367539 (machine, Bool, Bool)
//   calls State(machine, 0x815D9F7C) and stores the Bools at +0x20/+0x21.
// - GiantBirdGuardReturnState (SIZE 0x34): ctor 0x00367EE1 calls
//   GiantBirdNormalFlightState(machine, true, true), zeroes +0x24 and the
//   Coord3D at +0x28. xfer 0x00368015: Version1, UnsignedInt +0x24, Coord3D +0x28.
// - GiantBirdGuardOuterState (SIZE 0x44): ctor 0x00367E92 calls
//   State(machine, 0xE3215944), stores the exit conditions (table 0x00C17640,
//   shouldExit 0x00367F19 in this unit, distinct from AIGuard's ExitConditions
//   0x00542F43) at +0x20 with Int +0x24, Coord3D +0x28, Real +0x34 and
//   UnsignedInt +0x38 (Zero Hour's ExitConditions members), then +0x3C = 0 and
//   the Bool +0x40. xfer 0x003680E9: Version1, +0x38, +0x28, +0x24, +0x34.
// - GiantBirdAttackMoveToState (SIZE 0x30, table 0x00C17548): ctor 0x003677BE
//   calls GiantBirdNormalFlightState(machine, false, true), stores 5 at +0x2C
//   and a new 0x3C-byte machine at +0x28. xfer 0x0036788E: Version1, the base
//   xfer (0x0055CA8A, shared), then Int +0x2C, the machine's snapshot and the
//   CommandSourceType +0x24 unless light CRC. onExit 0x0036964D resets the
//   machine to state 0 and runs the base onExit.
// - GiantBirdDieState (SIZE 0x20, table 0x00C175A8): ctor 0x003678F7 calls
//   State(machine, 0xA36413A3); onEnter 0x00367914 destroys the owner.
// - AIGiantBirdFollowThruState (table 0x00C17418): ctor 0x003675D6 (machine,
//   Bool) calls State(machine, 0xDE23E437), stores the Bool at +0x20 and zero
//   at +0x24. xfer 0x00367604: version 2, Bool +0x20, (v2) Int +0x24.
// - AIGiantBirdSwoopState (SIZE 0x3C, table 0x00C17360): ctor 0x0036756A
//   (machine, Int, Bool) calls State(machine, 0x826EAA22), stores the Int at
//   +0x20, the Bool at +0x30, zero at +0x34 and the constant 0x00BBB8D8 at
//   +0x38. xfer 0x0036929D: Version1, 4 raw bytes +0x20, Coord3D +0x24,
//   Bool +0x30, UnsignedInt +0x34, Real +0x38.
#pragma once
#ifndef BFME2_GIANTBIRDAIUPDATE_H
#define BFME2_GIANTBIRDAIUPDATE_H

#include "Common/StateMachine.h"
#include "Common/Xfer.h"
#include "Common/StlList.h"
#include "GameLogic/AIGuard.h"
#include "GameLogic/Module/AIUpdate.h"
#include "GameLogic/AIStateMachine.h"
#include "GameLogic/AIStoredCommand.h"

class AIAttackState;

// GiantBirdAIStateMachine (name: WorldBuilder string "GiantBirdAIStateMachine";
// map placeholder Rva00367E26). SIZE 0x68 (makeStateMachine 0x0036819D: `new 0x68`).
// Ctor 0x0036792F calls AIStateMachine(owner, name) and stores table 0x00C17600,
// which overrides the deleting dtor and the two factories: slot 9 0x00367F9E
// returns a new 0x3C-byte AIAttackSwoopThenIdleStateMachine(owner, 0x615E895F),
// slot 10 0x003697BB a new 0x74-byte GiantBirdGuardMachine(owner).
class GiantBirdAIStateMachine : public AIStateMachine
{
public:
	GiantBirdAIStateMachine(Object *owner, UnsignedInt name);	// 0x0036792F
	virtual StateMachine *makeSubMachine();						// slot 9, 0x00367F9E
	virtual StateMachine *makeGuardMachine();					// slot 10, 0x003697BB
};
// SIZE 0x68

class GiantBirdAIUpdate : public AIUpdateInterface
{
public:
	GiantBirdAIUpdate(Thing *thing, const ModuleData *moduleData);	// 0x0036B717

	// Primary table 0x00817A80 differs from AIUpdateInterface at these slots.
	virtual void vslot089();								// primary slot 89
	virtual Bool vslot090() { return true; }				// 90, 0x0050B5C6
	virtual GiantBirdAIUpdate *vslot097() { return this; }	// 97, 0x0047A6A9
	virtual AIUpdateInterface *vslot098() { return this; }	// 98, 0x0047A6A9
	virtual Bool vslot146() { return false; }			// 146, 0x0047A699

	virtual NameKeyType getModuleNameKey() const;				// 0x0036B89B
	const char *name() const;									// 0x0036B8E0, non-virtual, returns "GiantBirdAIUpdate"
	virtual UpdateSleepTime update();							// 0x0036B103
	virtual void onCollide(Object *other, const Coord3D *loc, const Coord3D *normal);	// 0x003698CD
	virtual Bool isIdle() const;								// slot 110, 0x003681DC: busy while flag bit 0 of +0x4B8 is set
	Bool isUnk0x558Clear() const;								// 0x00368594, returns !m_unk0x558 (role name)
	void clearUnk0x558();										// 0x0036859F (role name)
	Int getUnk0x4B8Bit6() const;								// 0x003685A7, flag bit 6 of +0x4B8 (role name)
	Int getUnk0x4B8Bit8() const;								// 0x003685B4, flag bit 8 of +0x4B8 (role name)
	void setUnk0x4B8Bit(Int bit, Bool value);					// 0x0036748E, sets or clears one flag bit of +0x4B8 (role name)
	void setUnk0x4B8Bit5(Bool value);							// 0x0036940A (role name)
	void setUnk0x4B8Bit6(Bool value);							// 0x003685C1 (role name)
	void setUnk0x4B8Bit7(Bool value);							// 0x00368654 (role name)

protected:
	virtual ~GiantBirdAIUpdate();								// 0x0036B8E6
	virtual void xfer(Xfer *xfer);								// 0x0036AC73
	virtual AIStateMachine *makeStateMachine();					// 0x0036819D
	virtual void privateFaceObject(Object *obj, CommandSourceType cmdSource);	// slot 69, 0x003698A5
	virtual void privateFacePosition(const Coord3D *pos, CommandSourceType cmdSource);	// slot 70, 0x003698BB
	void faceTowards(const Coord3D *pos, CommandSourceType cmdSource, Bool isObject);	// 0x00368E95 (role name)

	Coord3D m_unk0x3E4;									// +0x3E4
	AIStoredCommand m_unk0x3F0;							// +0x3F0, the stored command (init 0x0026AFDA, xfer 0x003533BF: xfer 0x0036AC73 calls doXfer)
	_STL::list<Int> m_unk0x4B4;							// +0x4B4, element type folded with int
	Int m_unk0x4B8;										// +0x4B8, flag bits (0: busy, 6, 8)
	Real m_unk0x4BC;									// +0x4BC
	ObjectID m_unk0x4C0;								// +0x4C0
	ObjectID m_unk0x4C4;								// +0x4C4
	UnsignedByte m_unk0x4C8[0x60];						// +0x4C8, ctor 0x00312C95
	UnsignedInt m_unk0x528;								// +0x528, xfer'd as 4 raw bytes
	Real m_unk0x52C;									// +0x52C
	Real m_unk0x530;									// +0x530
	Bool m_unk0x534;									// +0x534
	Real m_unk0x538;									// +0x538
	Real m_unk0x53C;									// +0x53C
	Real m_unk0x540;									// +0x540
	Coord3D m_unk0x544;									// +0x544
	Bool m_unk0x550;									// +0x550
	ObjectID m_unk0x554;								// +0x554
	Bool m_unk0x558;									// +0x558
	UnsignedInt m_unk0x55C;								// +0x55C, xfer'd as 4 raw bytes
	Coord3D m_unk0x560;									// +0x560
	Coord3D m_unk0x56C;									// +0x56C
	Bool m_unk0x578;									// +0x578
};
// SIZE 0x57C

class GiantBirdGuardMachine : public AIGuardMachine
{
public:
	GiantBirdGuardMachine(Object *owner);			// 0x00368A13 (`new 0x74`)
	virtual ~GiantBirdGuardMachine();			// 0x00367F8D
	const char *name() const;					// 0x00367F98, non-virtual, returns "GiantBirdGuardMachine"
	virtual StateMachine *makeSubMachine();		// slot 9: the same body as GiantBirdAIStateMachine's, folded by /OPT:ICF into 0x00367F9E

protected:
	virtual void xfer(Xfer *xfer);				// slot 3, 0x00367FDD
};

class AIAttackSwoopThenIdleStateMachine : public StateMachine
{
public:
	AIAttackSwoopThenIdleStateMachine(Object *owner, UnsignedInt name);	// 0x003676A2
	virtual ~AIAttackSwoopThenIdleStateMachine();	// 0x003677B3

	virtual Bool isInAttackState() const;		// slot 11, 0x0036890E

protected:
	virtual const char *GetSnapshotName() const;	// 0x003677AD
	virtual void xfer(Xfer *xfer) { StateMachine::xfer(xfer); xfer->Version1(); }	// slot 3: the shared version-only override 0x0033FB0B
};

// PROVISIONAL: these state classes are declared here for their name getters only, so
// that every six-byte getter in this unit is compiled in retail order (the checker pairs
// identical getters by position). Bases and layouts are not recovered yet.
class AIGiantBirdAttackState : public State
{
public:
	AIGiantBirdAttackState(StateMachine *machine, Int unk) : State(machine, 0xFF874822), m_unk0x20(unk) {}	// 0x003675AC (COMDAT; the map's table is 0x00C173C0)
	virtual StateReturnType onEnter();				// 0x003699CA
	virtual void onExit(StateExitType status);		// 0x0036939B
	virtual StateReturnType update();				// 0x00369DED
protected:
	virtual void xfer(Xfer *xfer);					// 0x0036937A
	virtual const char *GetSnapshotName() const;	// 0x003675D0, returns "AIGiantBirdAttack"
	Int m_unk0x20;									// +0x20
};
// SIZE 0x24

class GiantBirdFollowWaypointPathState : public State
{
protected:
	virtual const char *GetSnapshotName() const;	// 0x0036766F
	virtual void xfer(Xfer *xfer);					// 0x00367675

	Int m_unk0x20;									// +0x20
	Bool m_unk0x24;									// +0x24
};

class GiantBirdGuardIdleState : public State
{
protected:
	virtual const char *GetSnapshotName() const;	// 0x00367E8C
};

class GiantBirdGuardInnerState : public State
{
protected:
	virtual const char *GetSnapshotName() const;	// 0x00368A0D
};

class GiantBirdGuardAttackAggressorState : public State
{
protected:
	virtual const char *GetSnapshotName() const;	// 0x003680E3
};

class GiantBirdDieState : public State
{
public:
	GiantBirdDieState(StateMachine *machine);	// 0x003678F7
	virtual StateReturnType onEnter();			// 0x00367914
	virtual StateReturnType update();			// 0x0036CC7A (shared body)

protected:
	virtual const char *GetSnapshotName() const;	// 0x00367929
};
// SIZE 0x20

class AIGiantBirdFollowThruState : public State
{
public:
	AIGiantBirdFollowThruState(StateMachine *machine, Bool flag) : State(machine, 0xDE23E437), m_flag20(flag), m_unk0x24(0) {}	// 0x003675D6 (COMDAT; CRC-32 "AIGiantBirdFollowThruState")
	virtual StateReturnType onEnter();			// 0x0036C6AF
	virtual void onExit(StateExitType status);	// 0x00369418
	virtual StateReturnType update();			// 0x00369EA3
	virtual Bool unknownQuery13() const { return true; }	// slot 13 (the shared `return true` 0x0050B5C6)

protected:
	virtual const char *GetSnapshotName() const;	// 0x003675FE
	virtual void xfer(Xfer *xfer);				// 0x00367604

	Bool m_flag20;								// +0x20
	Int m_unk0x24;								// +0x24, version 2
};

class AIGiantBirdSwoopState : public State
{
public:
	AIGiantBirdSwoopState(StateMachine *machine, Int mode, Bool flag)
		: State(machine, 0x826EAA22), m_mode(mode), m_flag30(flag), m_unk0x34(0), m_unk0x38(1.0f) {}	// 0x0036756A (COMDAT; the Real is VA 0x00BBB8D8)
	virtual StateReturnType onEnter();			// 0x0036BDF2
	virtual void onExit(StateExitType status);	// 0x003692ED
	virtual StateReturnType update();			// 0x0036C201

protected:
	virtual const char *GetSnapshotName() const;	// 0x003675A6
	virtual void xfer(Xfer *xfer);				// 0x0036929D

	Int m_mode;									// +0x20, xfer'd as 4 raw bytes
	Coord3D m_swoopPosition;					// +0x24
	Bool m_flag30;								// +0x30
	UnsignedInt m_unk0x34;						// +0x34
	Real m_unk0x38;								// +0x38
};
// SIZE 0x3C

// Role name: the giant bird's own exit conditions (retail spelling not recovered).
class GiantBirdExitConditions : public AttackExitConditionsInterface
{
public:
	virtual Bool shouldExit(const StateMachine *machine) const;	// 0x00367F19

	Int m_conditionsToConsider;					// +0x04
	Coord3D m_center;							// +0x08
	Real m_radiusSqr;							// +0x14
	UnsignedInt m_attackGiveUpFrame;			// +0x18
};
// SIZE 0x1C

class GiantBirdNormalFlightState : public State
{
public:
	GiantBirdNormalFlightState(StateMachine *machine, Bool flag20, Bool flag21);	// 0x00367539
	virtual StateReturnType onEnter();			// 0x00369064
	virtual void onExit(StateExitType status);	// 0x00369217
	virtual StateReturnType update();			// 0x0036914D

protected:
	virtual const char *GetSnapshotName() const;	// 0x00367564
	virtual void xfer(Xfer *xfer);				// slot 3, 0x0055CA8A (shared body: Version1 and the Bool +0x20; ICF-folded with several other classes)
	Bool m_flag20;								// +0x20
	Bool m_flag21;								// +0x21
};
// SIZE 0x24

class GiantBirdAttackMoveToState : public GiantBirdNormalFlightState
{
public:
	GiantBirdAttackMoveToState(StateMachine *machine);	// 0x003677BE
	virtual ~GiantBirdAttackMoveToState();		// 0x0036783F
	virtual Bool isGuardIdle() const;			// slot 9, 0x003678D6: true unless the attack-move machine's state is attacking
	virtual StateReturnType onEnter();			// 0x0036960E
	virtual void onExit(StateExitType status);	// 0x0036964D
	virtual StateReturnType update();			// 0x00369669

protected:
	virtual const char *GetSnapshotName() const;	// 0x00367839
	virtual void xfer(Xfer *xfer);				// 0x0036788E

	CommandSourceType m_commandSource;			// +0x24
	StateMachine *m_attackMoveMachine;			// +0x28
	Int m_unk0x2C;								// +0x2C, 5
};
// SIZE 0x30

class GiantBirdGuardReturnState : public GiantBirdNormalFlightState
{
public:
	GiantBirdGuardReturnState(StateMachine *machine);	// 0x00367EE1
	virtual StateReturnType onEnter();			// 0x0036A7DB

protected:
	virtual const char *GetSnapshotName() const;	// 0x00367F13
	virtual void xfer(Xfer *xfer);				// 0x00368015

	UnsignedInt m_nextReturnScanTime;			// +0x24
	Coord3D m_returnPosition;					// +0x28
};
// SIZE 0x34

class GiantBirdGuardOuterState : public State
{
public:
	GiantBirdGuardOuterState(StateMachine *machine);	// 0x00367E92
	virtual StateReturnType onEnter();			// 0x0036A5BB
	void enable();								// 0x00368129 (role name): sets m_unk0x40
	Bool attackStateQuery();					// 0x00368004 (role name): false without an attack state, else the attack state's slot 18 (AIAttackState::rva003413A1)
	virtual StateReturnType update();			// 0x0036A75C

protected:
	virtual const char *GetSnapshotName() const;	// 0x00367EDB
	virtual void xfer(Xfer *xfer);				// 0x003680E9

	GiantBirdExitConditions m_exitConditions;	// +0x20
	AIAttackState *m_attackState;				// +0x3C
	Bool m_unk0x40;								// +0x40
};
// SIZE 0x44

#endif
