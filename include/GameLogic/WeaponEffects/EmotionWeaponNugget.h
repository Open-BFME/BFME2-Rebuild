// EmotionWeaponNugget.h: a weapon effect nugget. SIZE 0x134.
// Target facts (retail game.dat):
// - parseEmotionWeaponNugget 0x002CCB04 (Weapon.cpp) allocates 0x134 bytes, runs the ctor 0x0050B59B, stores the
//   template at +0x104 and reads the nugget through 0x0050B42C (base and class tables).
// - The ctor stores table VA 0x00C64D10 (14 slots). Slots whose target differs from the
//   WeaponEffectNugget table: 0 0x00508F8C, 5 0x0050B4E2, 6 0x0050B519, 7 0x0050B5C6.
// - Members from the field table VA 0x00C64CD0 (token = name, parser = type).
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_EMOTIONWEAPONNUGGET_H
#define BFME2_EMOTIONWEAPONNUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"

class INI;

class EmotionWeaponNugget : public WeaponEffectNugget
{
public:
	EmotionWeaponNugget();										// 0x0050B59B
	virtual ~EmotionWeaponNugget();								// 0, deleting dtor 0x00508F8C
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x0050B4E2
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x0050B519
	virtual Bool slot07() const;	// 7, 0x0050B5C6

	// 0x0050B42C: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	Int m_emotionType;					// 0x128 EmotionType (an emotion type)
	Real m_radius;					// 0x12C Radius
	UnsignedInt m_duration;					// 0x130 Duration
};
// SIZE 0x134
typedef char EmotionWeaponNugget_size[(sizeof(EmotionWeaponNugget) == 0x134) ? 1 : -1];
typedef char EmotionWeaponNugget_off_m_emotionType[(offsetof(EmotionWeaponNugget, m_emotionType) == 0x128) ? 1 : -1];
typedef char EmotionWeaponNugget_off_m_radius[(offsetof(EmotionWeaponNugget, m_radius) == 0x12C) ? 1 : -1];
typedef char EmotionWeaponNugget_off_m_duration[(offsetof(EmotionWeaponNugget, m_duration) == 0x130) ? 1 : -1];

#endif
