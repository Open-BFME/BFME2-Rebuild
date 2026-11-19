// DOTNugget.h: a weapon effect nugget. SIZE 0x1AC.
// Target facts (retail game.dat):
// - parseDOTNugget 0x002CCA37 (Weapon.cpp) allocates 0x1AC bytes, runs the ctor 0x0050B1FC, stores the
//   template at +0x104 and reads the nugget through 0x0050B13A (base and class tables).
// - The ctor stores table VA 0x00C64C38 (15 slots). Slots whose target differs from the
//   DamageNugget table: 0 0x0050B21C, 14 0x0050B16F.
// - No class field table found by its helper; members past the base are a gap.
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_DOTNUGGET_H
#define BFME2_DOTNUGGET_H

#include "GameLogic/WeaponEffects/DamageNugget.h"

class INI;

class DOTNugget : public DamageNugget
{
public:
	DOTNugget();										// 0x0050B1FC
	virtual ~DOTNugget();								// 0, deleting dtor 0x0050B21C
	virtual Bool slot14(const ProjectileNuggetFireInfo *info, Object *victim, const Coord3D *pos);	// 14, 0x0050B16F

	// 0x0050B13A: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	UnsignedByte m_unk0x1A4[0x8];					// 0x1A4
};
// SIZE 0x1AC
typedef char DOTNugget_size[(sizeof(DOTNugget) == 0x1AC) ? 1 : -1];

#endif
