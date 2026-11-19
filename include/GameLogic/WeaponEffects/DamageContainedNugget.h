// DamageContainedNugget.h: a weapon effect nugget. SIZE 0x168.
// Target facts (retail game.dat):
// - parseDamageContainedNugget 0x002CC9D4 (Weapon.cpp) allocates 0x168 bytes, runs the ctor 0x0050AEAB, stores the
//   template at +0x104 and reads the nugget through 0x0050ADB9 (base and class tables).
// - The ctor stores table VA 0x00C64BB0 (14 slots). Slots whose target differs from the
//   WeaponEffectNugget table: 0 0x00508F8C, 1 0x0050ADEE, 2 0x0050B5F1, 5 0x0050AEE4, 6 0x0050B238, 7 0x0050B5C6.
// - No class field table found by its helper; members past the base are a gap.
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_DAMAGECONTAINEDNUGGET_H
#define BFME2_DAMAGECONTAINEDNUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"

class INI;

class DamageContainedNugget : public WeaponEffectNugget
{
public:
	DamageContainedNugget();										// 0x0050AEAB
	virtual ~DamageContainedNugget();								// 0, deleting dtor 0x00508F8C
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x0050ADEE
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);	// 2, 0x0050B5F1
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x0050AEE4
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x0050B238
	virtual Bool slot07() const;	// 7, 0x0050B5C6

	// 0x0050ADB9: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	UnsignedByte m_unk0x128[0x40];					// 0x128
};
// SIZE 0x168
typedef char DamageContainedNugget_size[(sizeof(DamageContainedNugget) == 0x168) ? 1 : -1];

#endif
