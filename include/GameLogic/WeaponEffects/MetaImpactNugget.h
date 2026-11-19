// MetaImpactNugget.h: a weapon's shock-wave nugget. SIZE 0x164.
// Target facts (retail game.dat):
// - parseMetaImpactNugget 0x002CC8A4 allocates 0x164 bytes and runs the ctor 0x0050A39B,
//   which calls WeaponEffectNugget's ctor, stores table VA 0x00C649A0 (16 slots: the base
//   14 plus 0x00509F3A and 0x0050A1E9) and fills the defaults below.
// - Members from the field table VA 0x00C64810 (token = name, parser = type).
#pragma once
#ifndef BFME2_METAIMPACTNUGGET_H
#define BFME2_METAIMPACTNUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"

class INI;
class DamageInfo;

class MetaImpactNugget : public WeaponEffectNugget
{
public:
	MetaImpactNugget();									// 0x0050A39B
	virtual ~MetaImpactNugget();							// 0, deleting dtor 0x0050A791
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x00509E43
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);					// 2, 0x00507943
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);		// 5, 0x00509DF8
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x0050A4A2
	virtual Bool slot07() const;														// 7 (shared return-false)
	virtual Bool slot14(const ProjectileNuggetFireInfo *info, Object *victim, DamageInfo *damageInfo);	// 14, 0x00509F3A
	virtual void slot15(const ProjectileNuggetFireInfo *info, Object *victim);				// 15, 0x0050A1E9

	// 0x00509DAB: the INI parse callback parseMetaImpactNugget calls with (ini, nugget, NULL,
	// NULL); it adds WeaponEffectNugget's table and VA 0x00C64810, then initFromINIMulti.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	Real m_shockWaveAmount;								// 0x128 ShockWaveAmount (velocity)
	Real m_shockWaveRadius;								// 0x12C ShockWaveRadius
	Real m_shockWaveArc;								// 0x130 ShockWaveArc (angle), default PI
	Bool m_shockWaveArcInverted;						// 0x134 ShockWaveArcInverted
	Real m_shockWaveTaperOff;							// 0x138 ShockWaveTaperOff
	Real m_shockWaveSpeed;								// 0x13C ShockWaveSpeed (velocity)
	Real m_shockWaveZMult;								// 0x140 ShockWaveZMult, default 1.0
	Real m_cyclonicFactor;								// 0x144 CyclonicFactor
	UnsignedInt m_delayTime;							// 0x148 DelayTime (duration)
	Bool m_invertShockWave;								// 0x14C InvertShockWave
	Bool m_flipDirection;								// 0x14D FlipDirection
	Real m_heroResist;									// 0x150 HeroResist
	Bool m_onlyWhenJustDied;							// 0x154 OnlyWhenJustDied
	Bool m_shockWaveClearRadius;						// 0x155 ShockWaveClearRadius
	Real m_shockWaveClearMult;							// 0x158 ShockWaveClearMult, default 2.0
	Real m_shockWaveClearFlingHeight;					// 0x15C ShockWaveClearFlingHeight, default 100.0
	ObjectFilter m_killObjectFilter;					// 0x160 KillObjectFilter
};
// SIZE 0x164
typedef char MetaImpactNugget_size[(sizeof(MetaImpactNugget) == 0x164) ? 1 : -1];
typedef char MetaImpactNugget_off_m_shockWaveAmount[(offsetof(MetaImpactNugget, m_shockWaveAmount) == 0x128) ? 1 : -1];
typedef char MetaImpactNugget_off_m_shockWaveRadius[(offsetof(MetaImpactNugget, m_shockWaveRadius) == 0x12C) ? 1 : -1];
typedef char MetaImpactNugget_off_m_shockWaveArc[(offsetof(MetaImpactNugget, m_shockWaveArc) == 0x130) ? 1 : -1];
typedef char MetaImpactNugget_off_m_shockWaveArcInverted[(offsetof(MetaImpactNugget, m_shockWaveArcInverted) == 0x134) ? 1 : -1];
typedef char MetaImpactNugget_off_m_shockWaveTaperOff[(offsetof(MetaImpactNugget, m_shockWaveTaperOff) == 0x138) ? 1 : -1];
typedef char MetaImpactNugget_off_m_shockWaveSpeed[(offsetof(MetaImpactNugget, m_shockWaveSpeed) == 0x13C) ? 1 : -1];
typedef char MetaImpactNugget_off_m_shockWaveZMult[(offsetof(MetaImpactNugget, m_shockWaveZMult) == 0x140) ? 1 : -1];
typedef char MetaImpactNugget_off_m_cyclonicFactor[(offsetof(MetaImpactNugget, m_cyclonicFactor) == 0x144) ? 1 : -1];
typedef char MetaImpactNugget_off_m_delayTime[(offsetof(MetaImpactNugget, m_delayTime) == 0x148) ? 1 : -1];
typedef char MetaImpactNugget_off_m_invertShockWave[(offsetof(MetaImpactNugget, m_invertShockWave) == 0x14C) ? 1 : -1];
typedef char MetaImpactNugget_off_m_flipDirection[(offsetof(MetaImpactNugget, m_flipDirection) == 0x14D) ? 1 : -1];
typedef char MetaImpactNugget_off_m_heroResist[(offsetof(MetaImpactNugget, m_heroResist) == 0x150) ? 1 : -1];
typedef char MetaImpactNugget_off_m_onlyWhenJustDied[(offsetof(MetaImpactNugget, m_onlyWhenJustDied) == 0x154) ? 1 : -1];
typedef char MetaImpactNugget_off_m_shockWaveClearRadius[(offsetof(MetaImpactNugget, m_shockWaveClearRadius) == 0x155) ? 1 : -1];
typedef char MetaImpactNugget_off_m_shockWaveClearMult[(offsetof(MetaImpactNugget, m_shockWaveClearMult) == 0x158) ? 1 : -1];
typedef char MetaImpactNugget_off_m_shockWaveClearFlingHeight[(offsetof(MetaImpactNugget, m_shockWaveClearFlingHeight) == 0x15C) ? 1 : -1];
typedef char MetaImpactNugget_off_m_killObjectFilter[(offsetof(MetaImpactNugget, m_killObjectFilter) == 0x160) ? 1 : -1];

#endif
