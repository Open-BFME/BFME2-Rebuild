// TurretAI.h: one turret's controller (an AIUpdateInterface owns up to one), its parsed data and its state machine.
// Target facts (retail game.dat 1.06). Zero Hour's TurretAI.h is the name donor; every layout fact is read from the binary:
//  - TurretAI (constructor 0x004D85B6 (owner, data, whichTurret), SIZE 0x40; the owner allocates 0x40 bytes): Snapshot at +0x00
//    (table 0x00C60D78: 0 the deleting destructor 0x004D8974, 1 loadPostProcess 0x004D80D4, 2 GetSnapshotName 0x004D86BB,
//    3 xfer 0x004D7FAD) and NotifyWeaponFiredInterface at +0x04 (table 0x00C60D64: 0 notifyFired 0x004D81D2, 1
//    notifyNewVictimChosen 0x004D8B3F, 2 isWeaponSlotOkToFire 0x004D8717 (forwards to isWeaponSlotOnTurret with this - 4),
//    3 isAttackingObject 0x004D86B1 (target == TARGET_OBJECT), 4 getOriginalVictimPos = the shared "return 0").
//    Members from the constructor's stores, in Zero Hour's order: +0x08 data, +0x0C whichTurret, +0x10 owner, +0x14 state
//    machine (a new 0x40-byte TurretStateMachine), +0x18/+0x1C angle and pitch (the data's natural angle and pitch),
//    +0x20 a word set to 1 (the sound slot of Zero Hour's m_turretRotOrPitchSound), +0x24 enableSweepUntil (isSweepEnabled
//    0x004D8365), +0x28 victim initial team (loadPostProcess 0x004D80D4 copies the goal object's +0x304), +0x2C target
//    (TurretTargetType), +0x30 continuousFireExpirationFrame (-1), +0x34 sleepUntil (setTurretEnabled 0x004D82D6 stamps
//    the frame), then one byte per Zero Hour bit-field: +0x38 playRotSound, +0x39 playPitchSound, +0x3A positiveSweep (true),
//    +0x3B didFire (notifyFired stores it through the +4 base), +0x3C enabled (= !data->initiallyDisabled), +0x3D
//    firesWhileTurning, +0x3E isForceAttacking, +0x3F targetWasSetByIdleMood.
//  - TurretAIData (constructor 0x004D7E6C, SIZE 0x70): the INI block. +0x00 turn rate, +0x04 pitch rate, +0x08 natural angle,
//    +0x0C natural pitch, +0x10 six fire-angle sweeps, +0x28 six sweep-speed modifiers (1.0), +0x40 fire pitch, +0x44 min
//    pitch, +0x48 ground-unit pitch, +0x4C controlled weapon-slot mask, +0x50/+0x54 idle scan angles, +0x58/+0x5C idle scan
//    intervals (0x98967F), +0x60 recenter time (twice the logic rate), +0x64..+0x66 InitiallyDisabled, FiresWhileTurning and
//    AllowsPitch, +0x68/+0x6C the two Real deflection limits the draft module data names TurretMaxDeflectionCW/CCW.
#pragma once
#ifndef BFME2_TURRETAI_H
#define BFME2_TURRETAI_H

#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "Common/Snapshot.h"
#include "Common/StateMachine.h"
#include "Common/Xfer.h"
#include "GameLogic/AIStateMachine.h"

class Object;
class Team;
class INI;
class MultiIniFieldParse;
enum WeaponSlotType;

// What a turret is aimed at (Zero Hour's TurretTargetType; AIUpdateInterface::getTurretTargetObject 0x00262605 compares with 1).
enum TurretTargetType
{
	TARGET_NONE,
	TARGET_OBJECT,
	TARGET_POSITION
};

// The turret state machine's states (Zero Hour's order; setTurretTargetObject 0x004D8791 switches between AIM, FIRE and HOLD).
enum TurretStateType
{
	TURRETAI_IDLE = 0,
	TURRETAI_IDLESCAN = 1,
	TURRETAI_AIM = 2,
	TURRETAI_FIRE = 3,
	TURRETAI_RECENTER = 4,
	TURRETAI_HOLD = 5
};

class TurretAI;

class TurretAIData
{
public:
	TurretAIData();																// 0x004D7E6C
	static void buildFieldParse(MultiIniFieldParse &p);							// 0x004D7F9C
	static void parseTurretSweep(INI *ini, void *instance, void *store, const void *userData);			// 0x004D7F36
	static void parseTurretSweepSpeed(INI *ini, void *instance, void *store, const void *userData);	// 0x004D7F69

	Real m_turnRate;															// +0x00
	Real m_pitchRate;															// +0x04
	Real m_naturalTurretAngle;													// +0x08
	Real m_naturalTurretPitch;													// +0x0C
	Real m_turretFireAngleSweep[6];												// +0x10
	Real m_turretSweepSpeedModifier[6];											// +0x28
	Real m_firePitch;															// +0x40
	Real m_minPitch;															// +0x44
	Real m_groundUnitPitch;														// +0x48
	UnsignedInt m_turretWeaponSlots;											// +0x4C
	Real m_minIdleScanAngle;													// +0x50
	Real m_maxIdleScanAngle;													// +0x54
	UnsignedInt m_minIdleScanInterval;											// +0x58
	UnsignedInt m_maxIdleScanInterval;											// +0x5C
	UnsignedInt m_recenterTime;													// +0x60
	Bool m_initiallyDisabled;													// +0x64
	Bool m_firesWhileTurning;													// +0x65
	Bool m_isAllowsPitch;														// +0x66
	Real m_maxDeflectionCW;														// +0x68 (role name: the draft module data's TurretMaxDeflectionCW)
	Real m_maxDeflectionCCW;													// +0x6C
};
// SIZE 0x70

// The turret's own state machine (SIZE 0x40): StateMachine plus the owning TurretAI at +0x3C.
class TurretStateMachine : public StateMachine
{
public:
	TurretStateMachine(TurretAI *tai, Object *owner, UnsignedInt name);		// 0x004D7CE6
	virtual ~TurretStateMachine();											// 0x004D7E61
	TurretAI *getTurretAI() const { return m_turretAI; }

	virtual void clear();													// slot 5, 0x004D853F
	virtual StateReturnType resetToDefaultState();							// slot 6, 0x004D855B
	virtual StateReturnType setState(StateID newStateID);					// slot 8, 0x004D8578

	virtual const char *GetSnapshotName() const;							// 0x004D7E5B (public in the map: vfunc_0x08)
protected:
	virtual void xfer(Xfer *xfer);											// slot 3: retail table 0x00C609C8 points at the version-only fold 0x00306A9B (Version1 only, no StateMachine::xfer)

private:
	TurretAI *m_turretAI;													// +0x3C
};


// The turret states (Zero Hour's TurretAI.h). Every concrete state's xfer is the shared version-only body 0x00544AB3 except
// IdleScan's. State name hashes are the constructors' pushes (0x004D7B98 and its neighbours).
class TurretState : public State
{
protected:
	TurretState(TurretStateMachine *machine, UnsignedInt name) : State(machine, name) {}	// 0x004D7B70
	TurretAI *getTurretAI() { return ((TurretStateMachine *)getMachine())->getTurretAI(); }
};

class TurretAIIdleState : public TurretState
{
public:
	TurretAIIdleState(TurretStateMachine *machine) : TurretState(machine, 0x2722C825), m_nextIdleScan(0) {}	// 0x004D7B98
	virtual StateReturnType onEnter();						// 0x004D83D1
	virtual StateReturnType update();						// 0x004D88F6
	virtual const char *GetSnapshotName() const;			// 0x004D7BB9
protected:
	virtual void xfer(Xfer *xfer) { xfer->Version1(); }		// the shared 0x00544AB3
private:
	void resetIdleScan();									// 0x004D839A
	UnsignedInt m_nextIdleScan;								// +0x20
};
// SIZE 0x24

class TurretAIIdleScanState : public TurretState
{
public:
	TurretAIIdleScanState(TurretStateMachine *machine) : TurretState(machine, 0x0A3D5FB3), m_desiredAngle(0.0f) {}	// 0x004D7BBF
	virtual StateReturnType onEnter();						// 0x004D8448
	virtual StateReturnType update();						// 0x004D8D7F
	virtual const char *GetSnapshotName() const;			// 0x004D7BE4
protected:
	virtual void xfer(Xfer *xfer);							// 0x004D8429
private:
	Real m_desiredAngle;									// +0x20
};
// SIZE 0x24

class TurretAIAimTurretState : public TurretState
{
public:
	TurretAIAimTurretState(TurretStateMachine *machine) : TurretState(machine, 0xF5FC67E3) {}	// 0x004D7BEA
	virtual StateReturnType update();						// 0x004D8EAC
	void SetModelAngle(Object *obj, Real angle);			// 0x004D8C23 (sets one of four model-condition bits, 306..309, by the quadrant of the turret angle)
	virtual const char *GetSnapshotName() const;			// 0x004D7C07
protected:
	virtual void xfer(Xfer *xfer) { xfer->Version1(); }
};
// SIZE 0x20

class TurretAIRecenterTurretState : public TurretState
{
public:
	TurretAIRecenterTurretState(TurretStateMachine *machine) : TurretState(machine, 0x5309DBFB) {}	// 0x004D7C0D
	virtual StateReturnType update();						// 0x004D8D25
	virtual const char *GetSnapshotName() const;			// 0x004D7C2A
protected:
	virtual void xfer(Xfer *xfer) { xfer->Version1(); }
};
// SIZE 0x20

class TurretAIHoldTurretState : public TurretState
{
public:
	TurretAIHoldTurretState(TurretStateMachine *machine) : TurretState(machine, 0x8546A46A), m_timestamp(0) {}	// 0x004D7C30
	virtual StateReturnType onEnter();						// 0x004D84D2
	virtual StateReturnType update();						// 0x004D8935
	virtual const char *GetSnapshotName() const;			// 0x004D7C51
protected:
	virtual void xfer(Xfer *xfer) { xfer->Version1(); }
private:
	UnsignedInt m_timestamp;								// +0x20
};
// SIZE 0x24

class TurretAI : public Snapshot, public NotifyWeaponFiredInterface
{
public:
	TurretAI(Object *owner, const TurretAIData *data, WhichTurretType tur);	// 0x004D85B6

	Real getNaturalTurretAngle() const { return m_data->m_naturalTurretAngle; }	// inline in retail: the constructor copies it through an xmm register (0x004D85B6)
	Real getNaturalTurretPitch() const { return m_data->m_naturalTurretPitch; }
	// Zero Hour's TurretAI accessors of the module data, out of line in BFME 2 (retail emits them in TurretAI.cpp with no caller; the deflection pair is BFME 2's, role names).
	Real getMinPitch() const;	// 0x004D7C57
	Bool isAllowsPitch() const;	// 0x004D7C5E
	Real getPitchRate() const;	// 0x004D7C6C
	Real getFirePitch() const;	// 0x004D7C73
	Real getGroundUnitPitch() const;	// 0x004D7C7A
	Real getMinIdleScanAngle() const;	// 0x004D7C88
	Real getMaxIdleScanAngle() const;	// 0x004D7C8F
	UnsignedInt getMaxIdleScanInterval() const;	// 0x004D7C9D
	UnsignedInt getRecenterTime() const;	// 0x004D7CA4
	Real getMaxDeflectionCW() const;	// 0x004D7CAB
	Real getMaxDeflectionCCW() const;	// 0x004D7CB2
	Real getTurretAngle() const { return m_angle; }							// inline in retail (AIUpdateInterface::getTurretRotAndPitch 0x002626FA)
	Real getTurretPitch() const { return m_pitch; }
	const TurretAIData *getData() const { return m_data; }
	Object *getOwner() const { return m_owner; }
	Bool isTurretEnabled() const { return m_enabled; }						// AIUpdateInterface::isTurretEnabled 0x00262683 returns the byte
	Real getTurretFireAngleSweepForWeaponSlot(WeaponSlotType slot) const;	// 0x004D81B6
	Real getTurretSweepSpeedModifierForWeaponSlot(WeaponSlotType slot) const;	// 0x004D81C4
	Bool isOwnersCurWeaponOnTurret() const;									// 0x004D8762
	Bool isWeaponSlotOnTurret(WeaponSlotType wslot) const;					// 0x004D81D7
	void setTurretTargetObject(Object *victim, Bool forceAttacking);		// 0x004D8791
	void setTurretTargetPosition(const Coord3D *pos);						// 0x004D8828
	void recenterTurret();													// 0x004D8298
	Bool isTurretInNaturalPosition() const;									// 0x004D82A3
	void setTurretEnabled(Bool enabled);									// 0x004D82D6
	Bool isTryingToAimAtTarget(const Object *victim) const;					// 0x004D871F
	Bool friend_turnTowardsAngle(Real desiredAngle, Real rateModifier, Real relThresh);	// 0x004D8990
	Bool friend_turnTowardsPitch(Real pitch, Real rateModifier);			// 0x004D80EE
	WhichTurretType friend_getWhichTurret() const { return m_whichTurret; }
	UnsignedInt friend_getNextIdleMoodTargetFrame() const;					// 0x004D837D
	Bool friend_isSweepEnabled() const;										// 0x004D8365
	void wakeNow();															// 0x004D82CA: the sleep-until frame becomes the current frame
	Bool friend_isAnyWeaponInRangeOf(const Object *o) const;				// 0x004D8310
	TurretTargetType friend_getTurretTarget(Object *&obj, Coord3D &pos) const;	// 0x004D81F1
	void friend_checkForIdleMoodTarget();									// 0x004D88AC
	UpdateSleepTime updateTurretAI();										// 0x004D8DEA

	virtual void notifyFired();												// slot 0 of the +4 base, 0x004D81D2
	virtual void notifyNewVictimChosen(Object *victim);						// slot 1, 0x004D8B3F
	virtual Bool isWeaponSlotOkToFire(Int wslot) const;						// slot 2, 0x004D8717 (forwarder to isWeaponSlotOnTurret)
	virtual Bool isAttackingObject() const;									// slot 3, 0x004D86B1
	virtual const Coord3D *getOriginalVictimPos() const { return 0; }		// slot 4, the shared "return 0"
	virtual void loadPostProcess();											// 0x004D80D4 (public U in the map)

protected:
	virtual ~TurretAI();													// 0x004D86C1; deleting 0x004D8974
	virtual const char *GetSnapshotName() const;							// 0x004D86BB
	virtual void xfer(Xfer *xfer);											// 0x004D7FAD

	friend class TurretStateMachine;	// clear/resetToDefaultState/setState stamp m_sleepUntil (0x004D853F, 0x004D855B, 0x004D8578)
	friend class AIUpdateInterface;	// ~AIUpdateInterface 0x0026E836 deletes its turret through the deleting destructor (Zero Hour's deleteInstance, `::delete` here)
	void stopRotOrPitchSound();												// 0x004D82F6 (the map spells it public)
private:
	void startRotOrPitchSound();											// 0x004D8B50 (no body yet; ledger AAE: private)
	void removeSelfAsTargeter();											// 0x004D825F

	const TurretAIData *m_data;												// +0x08
	WhichTurretType m_whichTurret;											// +0x0C
	Object *m_owner;														// +0x10
	TurretStateMachine *m_turretStateMachine;								// +0x14
	Real m_angle;															// +0x18
	Real m_pitch;															// +0x1C
	Int m_unk0x20;															// +0x20 (1 after construction; Zero Hour's rotation/pitch sound slot)
	UnsignedInt m_enableSweepUntil;											// +0x24
	Team *m_victimInitialTeam;												// +0x28
	mutable TurretTargetType m_target;										// +0x2C
	UnsignedInt m_continuousFireExpirationFrame;							// +0x30
	UnsignedInt m_sleepUntil;												// +0x34
	Bool m_playRotSound;													// +0x38
	Bool m_playPitchSound;													// +0x39
	Bool m_positiveSweep;													// +0x3A
	Bool m_didFire;															// +0x3B
	Bool m_enabled;															// +0x3C
	Bool m_firesWhileTurning;												// +0x3D
	Bool m_isForceAttacking;												// +0x3E
	mutable Bool m_targetWasSetByIdleMood;									// +0x3F
};
// SIZE 0x40

#endif
