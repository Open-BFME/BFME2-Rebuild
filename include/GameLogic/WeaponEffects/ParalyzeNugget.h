// ParalyzeNugget.h: a weapon nugget that paralyzes what it hits. SIZE 0x138.
// Target facts (retail game.dat): parseParalyzeNugget 0x002CC711 allocates 0x138 bytes;
// the ctor 0x00508F51 calls WeaponEffectNugget's, stores table VA 0x00C644A0 and zeroes
// +0x128/+0x12C/+0x134 with +0x130 = PI. Field table VA 0x00C64450 (token = name).
// canAffectObject 0x00508F1E adds an Object::isKindOf(0x220) exclusion to the base test.
#pragma once
#ifndef BFME2_PARALYZENUGGET_H
#define BFME2_PARALYZENUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"

class INI;
class FXList;

class ParalyzeNugget : public WeaponEffectNugget
{
public:
	ParalyzeNugget();																		// 0x00508F51
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x00508F1E
	virtual ~ParalyzeNugget();															// 0, deleting dtor 0x00508F8C
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);					// 2, 0x00507943
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x005091D6
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x0050921D
	virtual Bool slot07() const;															// 7, 0x0050B5C6
	virtual void GetAssetList(AssetList &assets, void *context);													// 9, 0x00508F0C

	// 0x00508EBF: the INI parse callback parseParalyzeNugget calls with (ini, nugget, NULL, NULL).
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	Real m_radius;							// 0x128 Radius
	UnsignedInt m_duration;					// 0x12C Duration (duration)
	Real m_damageArc;						// 0x130 DamageArc (angle), default PI
	const FXList *m_paralyzeFX;				// 0x134 ParalyzeFX
};
// SIZE 0x138
typedef char ParalyzeNugget_size[(sizeof(ParalyzeNugget) == 0x138) ? 1 : -1];
typedef char ParalyzeNugget_off_m_radius[(offsetof(ParalyzeNugget, m_radius) == 0x128) ? 1 : -1];
typedef char ParalyzeNugget_off_m_paralyzeFX[(offsetof(ParalyzeNugget, m_paralyzeFX) == 0x134) ? 1 : -1];

#endif
