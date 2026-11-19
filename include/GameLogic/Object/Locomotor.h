// Locomotor.h: Locomotor, a unit's instance of a LocomotorTemplate.
// WorldBuilder names the header "GameLogic/Object/Locomotor.h" and the unit Locomotor.cpp.
//
// Target facts (retail game.dat):
// - SIZE 0xB0: LocomotorStore::newLocomotor 0x001E4A81 allocates 0xB0.
// - Base Snapshot only: the table 0x00BDE888 has Snapshot's four slots (deleting
//   dtor 0x001E4625, the shared empty loadPostProcess, GetSnapshotName 0x001E3E56,
//   xfer 0x001E525A); the dtor 0x001E3E43 restores Snapshot's table 0x00BBB554.
// - Ctor 0x001E449A (const LocomotorTemplate *, Bool): +4 the template's final
//   override; two Coord3Ds +0x08/+0x14 zeroed; Reals +0x20 = 1.0, +0x24/+0x28 and
//   +0x30..+0x38 = 99999.0, +0x2C = -1.0, +0x3C = the template's CloseEnoughDist
//   (+0xC4), +0x40 = 0; +0x44 flag word = 0; the template's PreferredHeight,
//   PreferredAttackHeight and PreferredHeightDamping (+0x58/+0x5C/+0x60) copied to
//   +0x48/+0x4C/+0x50; +0x54 = 0, +0x58 = PI/40, +0x5C = 0; +0x60/+0x64 = 0; a
//   Matrix3D +0x68 built as identity; Bools +0x98..+0x9A; +0x9C = 0; Reals +0xA0/+0xA4;
//   +0xA8 = 0; +0xAC = 100; then the instance count (VA 0x00DFDC64) goes up and
//   setFlag(10, template's CloseEnoughDist3D).
// - Copy ctor 0x001E4641: the Matrix3D's rows, count up, then operator= 0x001E3CF4.
// - xfer 0x001E525A gives the member types (version 3, minimum 1; nothing under
//   light CRC): Coord3D +0x08; Reals +0x20..+0x40; UnsignedInt +0x44; Reals
//   +0x48..+0x58; Bool +0x98; Reals +0xA0/+0xA4; Coord3D +0x14; Real +0x5C;
//   UnsignedInts +0x60/+0x64; Bool +0x99; 4 raw bytes +0x9C (WorldBuilder:
//   "XferRawBytesHelper<enum Locomotor::AirborneState,1>"); Bool +0x9A from version 2;
//   a discarded UnsignedInt from version 3.
// - setFlag 0x001E3459 sets or clears bit n of +0x44.
// - WorldBuilder: "Locomotor::GetInstanceCount()==0" (the count's accessor).
// Names: m_template, m_flags and the four template copies are named for what they
// hold; m_instanceCount is provisional (retail's symbol is unknown). Other members
// keep their offsets.
#pragma once
#ifndef BFME2_LOCOMOTOR_H
#define BFME2_LOCOMOTOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "Common/Snapshot.h"
#include "WWMath/Matrix3D.h"

class AsciiString;
class LocomotorTemplate;
class Object;

class Locomotor : public Snapshot
{
public:
	enum LocoFlag
	{
		PRECISE_Z_POS = 3,						// setUsePreciseZPos 0x002638E4 (Zero Hour's order: the flag before NO_SLOW_DOWN_AS_APPROACHING_DEST)
		NO_SLOW_DOWN_AS_APPROACHING_DEST = 4,	// maintainCurrentPositionWings 0x001E7CDF sets it (Zero Hour's order)
		CLOSE_ENOUGH_DIST_3D = 10	// the ctor sets it from the template's CloseEnoughDist3D
	};
	enum AirborneState {};

	Locomotor(const LocomotorTemplate *tmpl, Bool flag);	// 0x001E449A
	Locomotor(const Locomotor &that);						// 0x001E4641
	Locomotor &operator=(const Locomotor &that);			// 0x001E3CF4
	virtual ~Locomotor();									// 0x001E3E43

	static Int GetInstanceCount() { return m_instanceCount; }

	AsciiString getTemplateName() const;					// 0x001E4A63
	void rva001E4147(Object *obj);							// 0x001E4147: copies the object's position (+0x38) into +0x14 (AIUpdateInterface::bfmePrivateCommand48 0x00264558 calls it; the map files it under placeholder class Rva001E4147)
	Real getMaxSpeed(Object *obj) const;					// 0x001E46E1
	Bool isChargeSpeedActive(Object *obj);					// 0x001E468F (map placeholder class Rva001E468F): the template can charge and the object has status 0x16 or 0x1E, or its owner-flag byte +0x249 is set and it has kind 0x84; getMaxSpeed 0x001E46E1 asks it
	Real getMaxTurnRate(Object *obj) const;					// 0x001E3E9F
	// Provisional names (no strings); the map files these under placeholder class Rva001E46E1.
	Real getMaxAcceleration(Object *obj);					// 0x001E4845: getMaxSpeed / template Acceleration, capped by +0x30
	Real getMaxLift(Object *obj, Int damageState);			// 0x001E3F4D: Lift/LiftDamaged per frame squared, capped by +0x24 (map placeholder class Rva001E46E1)
	Real getMinSpeed(Object *obj);							// 0x001E3F08: MinSpeed per frame (map placeholder class Rva001E3F08)
	Real calcMinTurnRadius(Object *obj, Real *timeToTravelThatDist);	// 0x001E4073 (map placeholder class Rva001E4073)
	void moveTowardsPositionWings(Object *obj, const Coord3D &goalPos, Real onPathDistToGoal, Real desiredSpeed);	// 0x001E7C2B (map placeholder class Rva001E7C2B)
	void moveTowardsPositionOther(Object *obj, const Coord3D &goalPos, Real onPathDistToGoal, Real desiredSpeed);	// 0x001E7053 (map placeholder class Rva001E7053)
	void rotateObjTowardsPosition(Object *obj, const Coord3D *pos, Real *out);	// 0x001E702E (map placeholder class Rva001E702E)
	void rotateTowardsPosition(Object *obj, const Coord3D *pos, Real *out);	// 0x001E685F: copies the object's transform into +0x68, then 0x001E56DC
	void moveTowardsPositionStep(Object *obj, const Coord3D *pos, Real onPathDistToGoal, Real desiredSpeed);	// 0x001E6007 (map placeholder class Rva001E46E1)
	void moveTowardsPositionSteered(Object *obj, const Coord3D &goalPos, Real onPathDistToGoal, Real desiredSpeed);	// 0x001E9045 (map placeholder class Rva001E9045)
	void steerTowardsPosition(Object *obj, const Coord3D &goalPos, Real onPathDistToGoal, Real desiredSpeed);	// 0x001E8C1B (map placeholder class Rva001E8C1B)
	void clearMovingCondition(Object *obj);							// 0x001E6516: clears MODELCONDITION 61 (MOVING) (map placeholder class Rva001E6516)
	Bool updateWadeConditions(Object *obj, const Coord3D *pos, Real *ground);	// 0x001E5F1D (map placeholder class Rva001E46E1; provisional name)
	Real getMaxBraking(Object *obj);						// 0x001E488A: getMaxSpeed / template Braking, capped by +0x34
	Bool isShockStunned(Object *obj);							// 0x001E4194: the object is shock-wave stunned (PhysicsBehavior +0x5C, set by 0x003906BF)
	Real getCurrentMaxSpeed(Object *obj);					// 0x001E48CF: +0x5C until frame +0x60, else getMaxSpeed
	Int isAboveQuarterSpeed(Object *obj);					// 0x001E543F: +0x40 > getMaxSpeed * 0.25 (returns a 32-bit 1/0)
	void updateCurrentSpeed(Real targetSpeed, Object *obj);	// 0x001E53D8: +0x40 steps toward the target by getMaxAcceleration, then 0..getMaxSpeed

protected:
	virtual void loadPostProcess() {}						// shared empty body
	virtual const char *GetSnapshotName() const;			// 0x001E3E56
	virtual void xfer(Xfer *xfer);							// 0x001E525A

protected:
	void maintainCurrentPositionWings(Object *obj);			// 0x001E7CDF

private:
	void setFlag(LocoFlag f, Bool set);						// 0x001E3459
	void setUsePreciseZPos(Bool u);							// 0x002638E4: setFlag(PRECISE_Z_POS, u), out of line (no caller in retail)
	void rva002638CC(Bool b);								// 0x002638CC (the map's name): setFlag(6, b), out of line (no caller in retail)

	static Int m_instanceCount;								// VA 0x00DFDC64

public:
	const LocomotorTemplate *m_template;		// +0x04, final override
	Coord3D m_unk0x08;							// +0x08
	Coord3D m_unk0x14;							// +0x14
	Real m_unk0x20;								// +0x20, 1.0
	Real m_unk0x24;								// +0x24, 99999.0
	Real m_unk0x28;								// +0x28, 99999.0
	Real m_unk0x2C;								// +0x2C, -1.0
	Real m_unk0x30;								// +0x30, 99999.0
	Real m_unk0x34;								// +0x34, 99999.0
	Real m_unk0x38;								// +0x38, 99999.0
	Real m_closeEnoughDist;						// +0x3C, template +0xC4
	Real m_unk0x40;								// +0x40
	UnsignedInt m_flags;						// +0x44, LocoFlag bits (setFlag)
	Real m_preferredHeight;						// +0x48, template +0x58
	Real m_preferredAttackHeight;				// +0x4C, template +0x5C
	Real m_preferredHeightDamping;				// +0x50, template +0x60
	Real m_unk0x54;								// +0x54
	Real m_unk0x58;								// +0x58, PI/40
	Real m_unk0x5C;								// +0x5C
	UnsignedInt m_unk0x60;						// +0x60
	UnsignedInt m_unk0x64;						// +0x64
	Matrix3D m_unk0x68;							// +0x68, identity
	Bool m_unk0x98;								// +0x98
	Bool m_unk0x99;								// +0x99
	Bool m_unk0x9A;								// +0x9A, xfer version 2
	AirborneState m_airborneState;				// +0x9C, xfer'd as 4 raw bytes
	Real m_unk0xA0;								// +0xA0
	Real m_unk0xA4;								// +0xA4
	UnsignedInt m_unk0xA8;						// +0xA8
	Int m_unk0xAC;								// +0xAC, 100
};

typedef char Locomotor_off_m_flags[(offsetof(Locomotor, m_flags) == 0x44) ? 1 : -1];
typedef char Locomotor_off_m_unk0x68[(offsetof(Locomotor, m_unk0x68) == 0x68) ? 1 : -1];
typedef char Locomotor_off_m_airborneState[(offsetof(Locomotor, m_airborneState) == 0x9C) ? 1 : -1];
typedef char Locomotor_off_m_unk0xAC[(offsetof(Locomotor, m_unk0xAC) == 0xAC) ? 1 : -1];
typedef char LocomotorSize[(sizeof(Locomotor) == 0xB0) ? 1 : -1];

#endif
