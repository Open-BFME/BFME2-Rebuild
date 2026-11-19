// WeaponEffectNugget.h: the base of the weapon nuggets a WeaponTemplate owns (DamageNugget,
// MetaImpactNugget and the rest). SIZE 0x128.
// Target facts (retail game.dat):
// - Ctor COMDAT 0x0050775B: table VA 0x00C64010 at +0, two 0x80-byte sets at +0x04 and
//   +0x84 built through 0x001EAE6F, zero at +0x104, STLport vectors at +0x108/+0x114, the
//   object filter at +0x120 (0x003623E5, then initFromStorages(KINDOFMASK_NONE x2)), and
//   +0x124 = 0. Dtor COMDAT 0x00507823 tears down +0x120, +0x114, +0x108 in that order.
// - Field table VA 0x00C63FD0: RequiredUpgradeNames +0x108 and ForbiddenUpgradeNames +0x114
//   (parseAsciiStringVector), SpecialObjectFilter +0x120 (iniParseObjectFilter).
// - Every parse<X>Nugget in Weapon.cpp (e.g. parseMetaImpactNugget 0x002CC8A4) stores the
//   owning WeaponTemplate at +0x104 before reading the nugget's table.
// - The table has 14 slots (VA 0x00C64010 and every nugget's copy). Names where a rowed
//   override carries one: 1 canAffectObject (ProjectileNugget 0x00509720; the base
//   0x005075D6 checks the source's upgrades and the filter), 5 doEffectObject
//   (ProjectileNugget 0x00509814), 6 doEffectPosition (DamageNugget 0x00507D84), 8
//   friend_postProcessLoad (ProjectileNugget 0x005097CD; the base 0x00507877 resolves the two
//   upgrade-name vectors). 5-7 are pure in the base. The others keep their slot number:
//   2 0x0050774A, 3/4 an empty two-argument body, 9 GetAssetList (WeaponTemplate 0x002CAC6E
//   walks the nuggets through it; empty in the base), 10-13 shared stubs.
// - The first argument of slots 1, 2, 5 and 6 is the firing context: its +0x08 is the
//   source object's ID (0x00507558 looks it up).
// Name: the donor (OpenSAGE) WeaponEffectNugget, whose fields are these three tokens.
#pragma once
#ifndef BFME2_WEAPONEFFECTNUGGET_H
#define BFME2_WEAPONEFFECTNUGGET_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/ObjectFilter.h"

class WeaponTemplate;
class AssetList;
class Object;
class Coord3D;
struct ProjectileNuggetFireInfo;

class WeaponEffectNugget
{
public:
	WeaponEffectNugget();								// 0x0050775B
	virtual ~WeaponEffectNugget();						// 0x00507823
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x005075D6
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);				// 2, 0x0050774A
	virtual void slot03(void *a, void *b);											// 3
	virtual void slot04(void *a, void *b);											// 4
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim) = 0;	// 5
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos) = 0;	// 6
	virtual Bool slot07() const = 0;													// 7
	virtual void friend_postProcessLoad();											// 8, 0x00507877
	virtual void GetAssetList(AssetList &assets, void *context);												// 9
	virtual Int slot10() const;														// 10
	virtual Int slot11() const;														// 11
	virtual Bool slot12() const;														// 12
	virtual Bool slot13() const;														// 13

	UnsignedByte m_unk0x04[0x80];						// 0x004 a 0x80-byte set (ctor 0x001EAE6F)
	UnsignedByte m_unk0x84[0x80];						// 0x084 a 0x80-byte set (ctor 0x001EAE6F)
	const WeaponTemplate *m_weaponTemplate;				// 0x104 the owner, set by the parser
	void *m_requiredUpgradeNames[3];					// 0x108 STLport vector<AsciiString>
	void *m_forbiddenUpgradeNames[3];					// 0x114 STLport vector<AsciiString>
	ObjectFilter m_specialObjectFilter;					// 0x120 SpecialObjectFilter
	Bool m_addedToOverride;								// 0x124 ctor false; WeaponTemplate::addNugget copies the template's m_isOverride here
};
// SIZE 0x128
typedef char WeaponEffectNugget_size[(sizeof(WeaponEffectNugget) == 0x128) ? 1 : -1];
typedef char WeaponEffectNugget_off_m_weaponTemplate[(offsetof(WeaponEffectNugget, m_weaponTemplate) == 0x104) ? 1 : -1];
typedef char WeaponEffectNugget_off_m_requiredUpgradeNames[(offsetof(WeaponEffectNugget, m_requiredUpgradeNames) == 0x108) ? 1 : -1];
typedef char WeaponEffectNugget_off_m_forbiddenUpgradeNames[(offsetof(WeaponEffectNugget, m_forbiddenUpgradeNames) == 0x114) ? 1 : -1];
typedef char WeaponEffectNugget_off_m_specialObjectFilter[(offsetof(WeaponEffectNugget, m_specialObjectFilter) == 0x120) ? 1 : -1];
typedef char WeaponEffectNugget_off_m_addedToOverride[(offsetof(WeaponEffectNugget, m_addedToOverride) == 0x124) ? 1 : -1];

#endif
