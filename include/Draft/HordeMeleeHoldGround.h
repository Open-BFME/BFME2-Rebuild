// HordeMeleeHoldGround.h: the hold-ground melee formation. Partial: its base is the placeholder Rva005D6FCC (the melee
// base shared with HordeMeleeAmoeba, HordeMeleeFormation and HordeMeleeSwarm), not named yet, so only what the three
// unit-state accessors show is here. They read a vector of 4-byte states at +0x08 (begin) and +0x0C (end): a state of 2
// is "rotating" and 3 is "arrived" (0x00583726 isUnitRotating, 0x00583750 setUnitRotating, 0x00583772 setUnitArrived).
#pragma once
#ifndef BFME2_HORDEMELEEHOLDGROUND_H
#define BFME2_HORDEMELEEHOLDGROUND_H

#include "Common/GameType.h"
#include "Common/Vector.h"

class Object;
class HordeContain;
class Xfer;

class HordeMeleeHoldGround
{
public:
	virtual Bool isUnitRotating(Int index) const;	// 0x00583726
	virtual void setUnitRotating(Int index);		// 0x00583750
	virtual void setUnitArrived(Int index);			// 0x00583772
	virtual void startMeleeAttack(Object *unit);	// 0x00583933 (table 0x00C6FC30 slot 3)
	virtual void updateMeleeAttack(Object *unit);	// 0x00583794 (slot 5)
	virtual void xfer(Xfer *xfer);					// 0x005837F0 (slot 15)

	HordeContain *m_horde;							// +0x04: the horde whose riders the rows below walk (0x00583794 and 0x00583933 read its ContainModuleInterface at +0x20 and call slot 70, getList)
	_STL::vector<Int> m_unitStates;					// +0x08 begin, +0x0C end
};

#endif
