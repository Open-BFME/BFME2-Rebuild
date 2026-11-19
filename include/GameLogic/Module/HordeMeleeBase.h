// HordeMeleeBase.h: the melee formation interface a HordeContain keeps at +0x2C8 (the constructor 0x0046F543 builds a
// HordeMeleeSwarm through the ThingTemplate's factory or `new`, 0x005843DA, and stores it there). Retail's base is the
// map's placeholder Rva005D6FCC; HordeMeleeAmoeba, HordeMeleeFormation, HordeMeleeHoldGround and HordeMeleeSwarm share its
// 16-slot table (HordeMeleeSwarm's, Draft/HordeMeleeSwarm.h: slot 3 startMeleeAttack, 5 updateMeleeAttack, 10
// setUnitRotating, 11 setUnitArrived, 15 xfer). Only the slots HordeContain calls are given a signature, in table order:
// slot 14 (offset 0x38, 0x005D7011 in the swarm) takes the unit and answers a Bool (HordeContainInterface slots 85 and 89,
// 0x00468DCD / 0x00468D7D, call it with the object in hand), slot 15 (0x3C) is the xfer (0x004751F3).
#pragma once
#ifndef BFME2_HORDEMELEEBASE_H
#define BFME2_HORDEMELEEBASE_H

#include "Common/GameType.h"

class Object;
class Xfer;
class Coord3D;

class HordeMeleeBase
{
public:
	virtual ~HordeMeleeBase();							// 0
	virtual void meleeSlot01(UnsignedInt) = 0;			// 1, 0x00584A75 in the swarm (map: UAEXI)
	virtual void meleeSlot02(UnsignedInt) = 0;			// 2, 0x00584A7D (map: QAEXI)
	virtual void startMeleeAttack(Object *unit) = 0;	// 3, 0x0058453D in the swarm
	virtual void meleeSlot04() = 0;						// 4, 0x00583B6F (map: QAEXXZ): HordeContainInterface slot 0 (0x00468FDC) calls it
	virtual void updateMeleeAttack(Object *unit) = 0;	// 5, 0x00584644 in the swarm
	virtual void meleeSlot06(Int) = 0;					// 6, 0x005E2171 (map: QAEXH)
	virtual Bool isUnitMoving(Int index) = 0;			// 7, 0x00583BE6
	virtual Bool isUnitRotating(Int index) = 0;			// 8, 0x00583C1A
	virtual Bool canUnitRotate(Int index) = 0;			// 9, 0x00583C4E
	virtual void setUnitRotating(Int index) = 0;		// 10, 0x00583C88
	virtual void setUnitArrived(Int index) = 0;			// 11, 0x00583CB4
	virtual void setUnitNeedsNewAttackPos(Int index, Int arg) = 0;	// 12, 0x00583CE0
	virtual Bool getUnitAttackPosition(Int index, Coord3D *pos) = 0;	// 13, 0x00583D0A
	virtual Bool meleeSlot14(Object *unit) = 0;			// 14, 0x005D7011 in the swarm
	virtual void xfer(Xfer *xfer) = 0;					// 15, the xfer
};

#endif
