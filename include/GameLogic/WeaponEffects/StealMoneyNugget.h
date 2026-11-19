// StealMoneyNugget.h: a weapon effect nugget. SIZE 0x12C.
// Target facts (retail game.dat):
// - parseStealMoneyNugget 0x002CCB67 (Weapon.cpp) allocates 0x12C bytes, runs the ctor 0x0050B6B4, stores the
//   template at +0x104 and reads the nugget through 0x0050B5F6 (base and class tables).
// - The ctor stores table VA 0x00C64D80 (14 slots). Slots whose target differs from the
//   WeaponEffectNugget table: 0 0x0050B6D1, 2 0x0050B5F1, 5 0x0050B62B, 6 0x0050B238, 7 0x0047A699.
// - No class field table found by its helper; members past the base are a gap.
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_STEALMONEYNUGGET_H
#define BFME2_STEALMONEYNUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"

class INI;

class StealMoneyNugget : public WeaponEffectNugget
{
public:
	StealMoneyNugget();										// 0x0050B6B4
	virtual ~StealMoneyNugget();								// 0, deleting dtor 0x0050B6D1
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);	// 2, 0x0050B5F1
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x0050B62B
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x0050B238
	virtual Bool slot07() const;	// 7, 0x0047A699

	// 0x0050B5F6: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	UnsignedByte m_unk0x128[0x4];					// 0x128
};
// SIZE 0x12C
typedef char StealMoneyNugget_size[(sizeof(StealMoneyNugget) == 0x12C) ? 1 : -1];

#endif
