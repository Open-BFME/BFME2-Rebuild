// SlaveAttackNugget.h: a weapon effect nugget. SIZE 0x128.
// Target facts (retail game.dat):
// - parseSlaveAttackNugget 0x002CC971 (Weapon.cpp) allocates 0x128 bytes, runs the ctor 0x0050ABF7, stores the
//   template at +0x104 and reads the nugget through 0x0050AAEA (base and class tables).
// - The ctor stores table VA 0x00C64B00 (14 slots). Slots whose target differs from the
//   WeaponEffectNugget table: 0 0x00508F8C, 1 0x0050AB37, 2 0x00507943, 5 0x0050AC09, 6 0x0050ACE0, 7 0x0047A699, 8 0x0050B928, 13 0x0050B5C6.
// - No class field table found by its helper; members past the base are a gap.
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_SLAVEATTACKNUGGET_H
#define BFME2_SLAVEATTACKNUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"

class INI;

class SlaveAttackNugget : public WeaponEffectNugget
{
public:
	SlaveAttackNugget();										// 0x0050ABF7
	virtual ~SlaveAttackNugget();								// 0, deleting dtor 0x00508F8C
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x0050AB37
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);	// 2, 0x00507943
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x0050AC09
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x0050ACE0
	virtual Bool slot07() const;	// 7, 0x0047A699
	virtual void friend_postProcessLoad();	// 8, 0x0050B928
	virtual Bool slot13() const;	// 13, 0x0050B5C6

	// 0x0050AAEA: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

};
// SIZE 0x128
typedef char SlaveAttackNugget_size[(sizeof(SlaveAttackNugget) == 0x128) ? 1 : -1];

#endif
