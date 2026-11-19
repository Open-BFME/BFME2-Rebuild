// WeaponBonus.h: one set of weapon multipliers. SIZE 0x18.
// Target facts (retail game.dat):
// - The inline ctor (COMDAT 0x00235A0E) fills six floats with 1.0 (rep stosd, 6).
// - Field names: WeaponBonusAttributeType's table VA 0x00DBC71C (DAMAGE, RADIUS, RANGE,
//   RATE_OF_FIRE, PRE_ATTACK, FIRING), parsed by WeaponBonusSet::parseWeaponBonusSet.
// - setField 0x002C8E40 (`?setField@WeaponBonus@@QAEXW4Field@1@M@Z`, a COMDAT copy) stores
//   m_field[f]; appendBonuses 0x002C96F7 adds (this[i] - 1.0) into the argument's six fields.
#pragma once
#ifndef BFME2_WEAPONBONUS_H
#define BFME2_WEAPONBONUS_H

#include "Common/GameType.h"

class WeaponBonus
{
public:
	enum Field
	{
		DAMAGE = 0,
		RADIUS,
		RANGE,
		RATE_OF_FIRE,
		PRE_ATTACK,
		FIRING,

		FIELD_COUNT
	};

	WeaponBonus() { for (int i = 0; i < FIELD_COUNT; ++i) m_field[i] = 1.0f; }

	// Weapon::computeBonus 0x002C9A6F resets its argument in place (rep stosd of 1.0, 6).
	void clear() { for (int i = 0; i < FIELD_COUNT; ++i) m_field[i] = 1.0f; }
	Real getField(Field f) const { return m_field[f]; }
	void setField(Field f, Real v) { m_field[f] = v; }
	void appendBonuses(WeaponBonus &bonus) const;					// 0x002C96F7

private:
	Real m_field[FIELD_COUNT];	// 0x00
};
// SIZE 0x18

#endif
