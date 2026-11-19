// AttributeModifierNugget.h: a weapon effect nugget. SIZE 0x140.
// Target facts (retail game.dat):
// - parseAttributeModifierNugget 0x002CC64B (Weapon.cpp) allocates 0x140 bytes, runs the ctor 0x00508BE5, stores the
//   template at +0x104 and reads the nugget through 0x0050893A (base and class tables).
// - The ctor stores table VA 0x00C64370 (14 slots). Slots whose target differs from the
//   WeaponEffectNugget table: 0 0x00508CDB, 1 0x00508925, 2 0x00507943, 5 0x00508B9E, 6 0x00508C46, 7 0x0050B5C6.
// - Members from the field table VA 0x00C64300 (token = name, parser = type).
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_ATTRIBUTEMODIFIERNUGGET_H
#define BFME2_ATTRIBUTEMODIFIERNUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"
#include "Common/AsciiString.h"
#include "Common/BitFlags.h"

class INI;
class FXList;

class AttributeModifierNugget : public WeaponEffectNugget
{
public:
	AttributeModifierNugget();										// 0x00508BE5
	virtual ~AttributeModifierNugget();								// 0, deleting dtor 0x00508CDB
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x00508925
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);	// 2, 0x00507943
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x00508B9E
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x00508C46
	virtual Bool slot07() const;	// 7, 0x0050B5C6

	// 0x0050893A: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	AsciiString m_attributeModifier;					// 0x128 AttributeModifier
	Int m_damageFXType;					// 0x12C DamageFXType
	Real m_radius;					// 0x130 Radius
	Real m_damageArc;					// 0x134 DamageArc (angle)
	BitFlags<15> m_antiCategories;					// 0x138 AntiCategories (AntiCategory set)
	const FXList * m_antiFX;					// 0x13C AntiFX
};
// SIZE 0x140
typedef char AttributeModifierNugget_size[(sizeof(AttributeModifierNugget) == 0x140) ? 1 : -1];
typedef char AttributeModifierNugget_off_m_attributeModifier[(offsetof(AttributeModifierNugget, m_attributeModifier) == 0x128) ? 1 : -1];
typedef char AttributeModifierNugget_off_m_damageFXType[(offsetof(AttributeModifierNugget, m_damageFXType) == 0x12C) ? 1 : -1];
typedef char AttributeModifierNugget_off_m_radius[(offsetof(AttributeModifierNugget, m_radius) == 0x130) ? 1 : -1];
typedef char AttributeModifierNugget_off_m_damageArc[(offsetof(AttributeModifierNugget, m_damageArc) == 0x134) ? 1 : -1];
typedef char AttributeModifierNugget_off_m_antiCategories[(offsetof(AttributeModifierNugget, m_antiCategories) == 0x138) ? 1 : -1];
typedef char AttributeModifierNugget_off_m_antiFX[(offsetof(AttributeModifierNugget, m_antiFX) == 0x13C) ? 1 : -1];

#endif
