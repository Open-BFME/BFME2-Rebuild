// DamageNugget.h: a weapon effect nugget. SIZE 0x1A4.
// Target facts (retail game.dat):
// - parseDamageNugget 0x002CC5E1 (Weapon.cpp) allocates 0x1A4 bytes, runs the ctor 0x00507C2D, stores the
//   template at +0x104 and reads the nugget through 0x005088F0 (base and class tables).
// - The ctor stores table VA 0x00C64048 (15 slots). Slots whose target differs from the
//   WeaponEffectNugget table: 0 0x00508668, 1 0x005086CE, 2 0x00507943, 3 0x0050797D, 4 0x00507A04, 5 0x00507991, 6 0x00507D84, 7 0x0050B5C6, 10 0x0047A6A9, 14 0x005084FC.
// - No class field table found by its helper; members past the base are a gap.
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_DAMAGENUGGET_H
#define BFME2_DAMAGENUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"

class INI;

class DamageNugget : public WeaponEffectNugget
{
public:
	DamageNugget();										// 0x00507C2D
	virtual ~DamageNugget();								// 0, deleting dtor 0x00508668
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x005086CE
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);	// 2, 0x00507943
	virtual void slot03(void *a, void *b);	// 3, 0x0050797D
	virtual void slot04(void *a, void *b);	// 4, 0x00507A04
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x00507991
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x00507D84
	virtual Bool slot07() const;	// 7, 0x0050B5C6
	virtual Int slot10() const;	// 10, 0x0047A6A9
	virtual Bool slot14(const ProjectileNuggetFireInfo *info, Object *victim, const Coord3D *pos);	// 14, 0x005084FC

	// 0x005088F0: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	UnsignedByte m_unk0x128[0x7C];					// 0x128
};
// SIZE 0x1A4
typedef char DamageNugget_size[(sizeof(DamageNugget) == 0x1A4) ? 1 : -1];

#endif
