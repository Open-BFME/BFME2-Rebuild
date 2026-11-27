// The amoeba policy for a horde's melee unit positions.
#pragma once

#include "Common/Coord3D.h"
#include "Common/GameType.h"
#include "Common/Vector.h"
#include "GameLogic/Module/HordeMeleeBase.h"

class HordeMeleeAmoebaConfig;

// One unit of the amoeba: 0x3C bytes. setUnitNeedsNewAttackPos (0x00587548) sets the flag at +0x0C and clears the word at +0x10.
struct HordeMeleeAmoebaUnit
{
	Coord3D m_attackPosition;  // +0x00: getUnitAttackPosition (0x0058776E) copies it out
	Bool m_needsNewAttackPos;
	unsigned char m_unk0x0D[0x3];
	Int m_unk0x10;
	ObjectID m_unk0x14;  // +0x14: xfer'd through XferObjectID
	unsigned char m_unk0x18[0x21];
	Bool m_attackPositionValid;  // +0x39: getUnitAttackPosition requires it
	unsigned char m_unk0x3A[0x2];
};

class HordeMeleeAmoeba : public HordeMeleeBase
{
public:
	HordeMeleeAmoeba(HordeContain *horde, HordeMeleeAmoebaConfig *config);
	virtual ~HordeMeleeAmoeba();

	virtual void meleeSlot01(UnsignedInt count);
	virtual void meleeSlot02(UnsignedInt count);
	virtual void startMeleeAttack(Object *unit);
	virtual void meleeSlot04() {}
	virtual void updateMeleeAttack(Object *unit);
	virtual void notifyOfMemberDeath(Object *unit);
	virtual Bool isUnitMoving(Int) { return true; }
	virtual Bool isUnitRotating(Int) const { return false; }
	virtual Bool canUnitRotate(Int) { return true; }
	virtual void setUnitRotating(Int) {}
	virtual void setUnitArrived(Int) {}
	virtual void setUnitNeedsNewAttackPos(Object *unit, Int index);
	virtual Bool getUnitAttackPosition(Int index, Coord3D *position);
	virtual Bool meleeSlot14(Object *unit);
	virtual void xfer(Xfer *xfer);

private:
	_STL::vector<HordeMeleeAmoebaUnit> m_units;
	Bool m_unk0x14;
	HordeMeleeAmoebaConfig *m_config;
};

typedef char HordeMeleeAmoebaSizeCheck[sizeof(HordeMeleeAmoeba) == 0x1C ? 1 : -1];
typedef char HordeMeleeAmoebaUnitSizeCheck[sizeof(HordeMeleeAmoebaUnit) == 0x3C ? 1 : -1];
