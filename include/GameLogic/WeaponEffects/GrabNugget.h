// GrabNugget.h: a weapon effect nugget. SIZE 0x140.
// Target facts (retail game.dat):
// - parseGrabNugget 0x002CC907 (Weapon.cpp) allocates 0x140 bytes, runs the ctor 0x0050AA97, stores the
//   template at +0x104 and reads the nugget through 0x0050A7E5 (base and class tables).
// - The ctor stores table VA 0x00C64AC0 (14 slots). Slots whose target differs from the
//   WeaponEffectNugget table: 0 0x00508F8C, 1 0x0050A98F, 2 0x0050B5F1, 5 0x0050A9F2, 6 0x0050B238, 7 0x0047A699.
// - Members from the field table VA 0x00C64A30 (token = name, parser = type).
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_GRABNUGGET_H
#define BFME2_GRABNUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"

class INI;

class GrabNugget : public WeaponEffectNugget
{
public:
	GrabNugget();										// 0x0050AA97
	virtual ~GrabNugget();								// 0, deleting dtor 0x00508F8C
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x0050A98F
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);	// 2, 0x0050B5F1
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x0050A9F2
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x0050B238
	virtual Bool slot07() const;	// 7, 0x0047A699

	// 0x0050A7E5: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	Bool m_containTargetOnEffect;					// 0x128 ContainTargetOnEffect
	Bool m_impactTargetOnEffect;					// 0x129 ImpactTargetOnEffect
	Bool m_removeTargetFromOtherContain;					// 0x12A RemoveTargetFromOtherContain
	Real m_shockWaveAmount;					// 0x12C ShockWaveAmount
	Real m_shockWaveRadius;					// 0x130 ShockWaveRadius
	Real m_shockWaveTaperOff;					// 0x134 ShockWaveTaperOff
	Real m_shockWaveSpeed;					// 0x138 ShockWaveSpeed
	Real m_shockWaveZMult;					// 0x13C ShockWaveZMult
};
// SIZE 0x140
typedef char GrabNugget_size[(sizeof(GrabNugget) == 0x140) ? 1 : -1];
typedef char GrabNugget_off_m_containTargetOnEffect[(offsetof(GrabNugget, m_containTargetOnEffect) == 0x128) ? 1 : -1];
typedef char GrabNugget_off_m_impactTargetOnEffect[(offsetof(GrabNugget, m_impactTargetOnEffect) == 0x129) ? 1 : -1];
typedef char GrabNugget_off_m_removeTargetFromOtherContain[(offsetof(GrabNugget, m_removeTargetFromOtherContain) == 0x12A) ? 1 : -1];
typedef char GrabNugget_off_m_shockWaveAmount[(offsetof(GrabNugget, m_shockWaveAmount) == 0x12C) ? 1 : -1];
typedef char GrabNugget_off_m_shockWaveRadius[(offsetof(GrabNugget, m_shockWaveRadius) == 0x130) ? 1 : -1];
typedef char GrabNugget_off_m_shockWaveTaperOff[(offsetof(GrabNugget, m_shockWaveTaperOff) == 0x134) ? 1 : -1];
typedef char GrabNugget_off_m_shockWaveSpeed[(offsetof(GrabNugget, m_shockWaveSpeed) == 0x138) ? 1 : -1];
typedef char GrabNugget_off_m_shockWaveZMult[(offsetof(GrabNugget, m_shockWaveZMult) == 0x13C) ? 1 : -1];

#endif
