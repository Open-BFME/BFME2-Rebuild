// WeaponBonusSet.h: one WeaponBonus per weapon-bonus condition. SIZE 0x210.
// Target facts (retail game.dat): appendBonuses 0x002C9778 walks 22 (0x16) entries of
// stride 0x18 from +0 and appends each whose condition bit is set; the condition names are
// WeaponBonusType's table VA 0x00DBC6C0 (22: GARRISONED .. SOLO_AI_HARD). The parsers are
// Weapon.cpp rows (parseWeaponBonusSet 0x002C971E and the two INI wrappers 0x002CA80E,
// 0x002CA81C).
#pragma once
#ifndef BFME2_WEAPONBONUSSET_H
#define BFME2_WEAPONBONUSSET_H

#include "GameLogic/WeaponBonus.h"

class INI;

class WeaponBonusSet
{
public:
	enum { WEAPONBONUSCONDITION_COUNT = 22 };

	void appendBonuses(UnsignedInt flags, WeaponBonus &bonus) const;					// 0x002C9778
	void parseWeaponBonusSet(INI *ini);													// 0x002C971E
	static void parseWeaponBonusSet(INI *ini, void *instance, void *store, const void *userData);	// 0x002CA80E
	static void parseWeaponBonusSetPtr(INI *ini, void *instance, void *store, const void *userData);	// 0x002CA81C

	WeaponBonus m_bonus[WEAPONBONUSCONDITION_COUNT];	// 0x000
};
// SIZE 0x210
typedef char WeaponBonusSet_size[(sizeof(WeaponBonusSet) == 0x210) ? 1 : -1];

#endif
