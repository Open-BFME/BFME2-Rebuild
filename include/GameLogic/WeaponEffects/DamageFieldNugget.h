// DamageFieldNugget.h: a weapon nugget that leaves a damage field (another weapon fired over
// time). SIZE 0x134.
// Target facts (retail game.dat): parseDamageFieldNugget 0x002CC774 allocates 0x134 bytes;
// the ctor 0x00509522 calls WeaponEffectNugget's, stores table VA 0x00C64520 and zeroes
// +0x130 (the string), +0x128 and +0x12C. Field table VA 0x00C644F0: WeaponTemplateName
// +0x130, Duration +0x12C. friend_postProcessLoad 0x005092B2 runs the base's and then
// resolves +0x130 through TheWeaponStore->findWeaponTemplate into +0x128.
#pragma once
#ifndef BFME2_DAMAGEFIELDNUGGET_H
#define BFME2_DAMAGEFIELDNUGGET_H

#include "Common/AsciiString.h"
#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"

class INI;

class DamageFieldNugget : public WeaponEffectNugget
{
public:
	DamageFieldNugget();																	// 0x00509522
	virtual ~DamageFieldNugget();															// 0, deleting dtor 0x00509548
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x005092D4
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);					// 2, 0x00507943
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x00509307
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x005093C6
	virtual Bool slot07() const;															// 7, 0x0047A699
	virtual void friend_postProcessLoad();		// 8, 0x005092B2
	virtual void GetAssetList(AssetList &assets, void *context);													// 9, 0x00509367

	// 0x00509379: the INI parse callback parseDamageFieldNugget calls with (ini, nugget, NULL, NULL).
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	const WeaponTemplate *m_fieldWeaponTemplate;	// 0x128 resolved from the name (the base's +0x104 is the owner)
	UnsignedInt m_duration;						// 0x12C Duration (duration)
	AsciiString m_weaponTemplateName;			// 0x130 WeaponTemplateName
};
// SIZE 0x134
typedef char DamageFieldNugget_size[(sizeof(DamageFieldNugget) == 0x134) ? 1 : -1];
typedef char DamageFieldNugget_off_m_weaponTemplateName[(offsetof(DamageFieldNugget, m_weaponTemplateName) == 0x130) ? 1 : -1];

#endif
