// OpenGateNugget.h: a weapon effect nugget. SIZE 0x12C.
// Target facts (retail game.dat):
// - parseOpenGateNugget 0x002CCAA1 (Weapon.cpp) allocates 0x12C bytes, runs the ctor 0x0050B374, stores the
//   template at +0x104 and reads the nugget through 0x0050B23B (base and class tables).
// - The ctor stores table VA 0x00C64C94 (14 slots). Slots whose target differs from the
//   WeaponEffectNugget table: 0 0x00508F8C, 1 0x00508925, 2 0x00507943, 5 0x0050B32D, 6 0x0050B391, 7 0x0047A699.
// - Members from the field table VA 0x00C64C74 (token = name, parser = type).
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_OPENGATENUGGET_H
#define BFME2_OPENGATENUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"

class INI;

class OpenGateNugget : public WeaponEffectNugget
{
public:
	OpenGateNugget();										// 0x0050B374
	virtual ~OpenGateNugget();								// 0, deleting dtor 0x00508F8C
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x00508925
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);	// 2, 0x00507943
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x0050B32D
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x0050B391
	virtual Bool slot07() const;	// 7, 0x0047A699

	// 0x0050B23B: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	Real m_radius;					// 0x128 Radius
};
// SIZE 0x12C
typedef char OpenGateNugget_size[(sizeof(OpenGateNugget) == 0x12C) ? 1 : -1];
typedef char OpenGateNugget_off_m_radius[(offsetof(OpenGateNugget, m_radius) == 0x128) ? 1 : -1];

#endif
