// SpecialModelConditionNugget.h: a weapon effect nugget. SIZE 0x138.
// Target facts (retail game.dat):
// - parseSpecialModelConditionNugget 0x002CC6AE (Weapon.cpp) allocates 0x138 bytes, runs the ctor 0x00508E11, stores the
//   template at +0x104 and reads the nugget through 0x00508D2F (base and class tables).
// - The ctor stores table VA 0x00C64404 (14 slots). Slots whose target differs from the
//   WeaponEffectNugget table: 0 0x00508E6B, 1 0x00508925, 2 0x0050B5F1, 5 0x00508D7C, 6 0x0050B238, 7 0x0047A699, 8 0x0050B928.
// - Members from the field table VA 0x00C643D4 (token = name, parser = type).
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_SPECIALMODELCONDITIONNUGGET_H
#define BFME2_SPECIALMODELCONDITIONNUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"

class INI;

class SpecialModelConditionNugget : public WeaponEffectNugget
{
public:
	SpecialModelConditionNugget();										// 0x00508E11
	virtual ~SpecialModelConditionNugget();								// 0, deleting dtor 0x00508E6B
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x00508925
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);	// 2, 0x0050B5F1
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x00508D7C
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x0050B238
	virtual Bool slot07() const;	// 7, 0x0047A699
	virtual void friend_postProcessLoad();	// 8, 0x0050B928

	// 0x00508D2F: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	void *m_modelConditionNames[3];					// 0x128 ModelConditionNames (STLport vector<AsciiString>)
	UnsignedInt m_modelConditionDuration;					// 0x134 ModelConditionDuration (duration)
};
// SIZE 0x138
typedef char SpecialModelConditionNugget_size[(sizeof(SpecialModelConditionNugget) == 0x138) ? 1 : -1];
typedef char SpecialModelConditionNugget_off_m_modelConditionNames[(offsetof(SpecialModelConditionNugget, m_modelConditionNames) == 0x128) ? 1 : -1];
typedef char SpecialModelConditionNugget_off_m_modelConditionDuration[(offsetof(SpecialModelConditionNugget, m_modelConditionDuration) == 0x134) ? 1 : -1];

#endif
