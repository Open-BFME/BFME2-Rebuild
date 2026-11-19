// HordeAttackNugget.h: a weapon effect nugget. SIZE 0x130.
// Target facts (retail game.dat):
// - parseHordeAttackNugget 0x002CCBCA (Weapon.cpp) allocates 0x130 bytes, runs the ctor 0x0050B8B3, stores the
//   template at +0x104 and reads the nugget through 0x0050B6F8 (base and class tables).
// - The ctor stores table VA 0x00C64DFC (14 slots). Slots whose target differs from the
//   WeaponEffectNugget table: 0 0x00508F8C, 1 0x0050B745, 2 0x00507943, 5 0x0050B7D2, 6 0x0050B843, 7 0x0047A699, 8 0x0050B928.
// - Members from the field table VA 0x00C64DCC (token = name, parser = type).
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_HORDEATTACKNUGGET_H
#define BFME2_HORDEATTACKNUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"

class INI;

class HordeAttackNugget : public WeaponEffectNugget
{
public:
	HordeAttackNugget();										// 0x0050B8B3
	virtual ~HordeAttackNugget();								// 0, deleting dtor 0x00508F8C
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x0050B745
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);	// 2, 0x00507943
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x0050B7D2
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x0050B843
	virtual Bool slot07() const;	// 7, 0x0047A699
	virtual void friend_postProcessLoad();	// 8, 0x0050B928

	// 0x0050B6F8: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	Bool m_closestMemberOnly;					// 0x128 ClosestMemberOnly
	Int m_lockWeaponSlot;					// 0x12C LockWeaponSlot
};
// SIZE 0x130
typedef char HordeAttackNugget_size[(sizeof(HordeAttackNugget) == 0x130) ? 1 : -1];
typedef char HordeAttackNugget_off_m_closestMemberOnly[(offsetof(HordeAttackNugget, m_closestMemberOnly) == 0x128) ? 1 : -1];
typedef char HordeAttackNugget_off_m_lockWeaponSlot[(offsetof(HordeAttackNugget, m_lockWeaponSlot) == 0x12C) ? 1 : -1];

#endif
