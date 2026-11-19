// SpawnAndFadeNugget.h: a weapon effect nugget. SIZE 0x13C.
// Target facts (retail game.dat):
// - parseSpawnAndFadeNugget 0x002CCC2D (Weapon.cpp) allocates 0x13C bytes, runs the ctor 0x0050BB0E, stores the
//   template at +0x104 and reads the nugget through 0x0050B8DB (base and class tables).
// - The ctor stores table VA 0x00C64EB0 (14 slots). Slots whose target differs from the
//   WeaponEffectNugget table: 0 0x0050BB66, 1 0x0050B92D, 2 0x00507943, 5 0x0050B97B, 6 0x0050B98F, 7 0x0047A699, 8 0x0050B928, 9 0x0050B95B.
// - Members from the field table VA 0x00C64E70 (token = name, parser = type).
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_SPAWNANDFADENUGGET_H
#define BFME2_SPAWNANDFADENUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"

class INI;

class SpawnAndFadeNugget : public WeaponEffectNugget
{
public:
	SpawnAndFadeNugget();										// 0x0050BB0E
	virtual ~SpawnAndFadeNugget();								// 0, deleting dtor 0x0050BB66
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x0050B92D
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);	// 2, 0x00507943
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x0050B97B
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x0050B98F
	virtual Bool slot07() const;	// 7, 0x0047A699
	virtual void friend_postProcessLoad();	// 8, 0x0050B928
	virtual void GetAssetList(AssetList &assets, void *context);	// 9, 0x0050B95B

	// 0x0050B8DB: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	ObjectFilter m_objectTargetFilter;					// 0x128 ObjectTargetFilter
	AsciiString m_spawnedObjectName;					// 0x12C SpawnedObjectName
	Coord3D m_spawnOffset;					// 0x130 SpawnOffset
};
// SIZE 0x13C
typedef char SpawnAndFadeNugget_size[(sizeof(SpawnAndFadeNugget) == 0x13C) ? 1 : -1];
typedef char SpawnAndFadeNugget_off_m_objectTargetFilter[(offsetof(SpawnAndFadeNugget, m_objectTargetFilter) == 0x128) ? 1 : -1];
typedef char SpawnAndFadeNugget_off_m_spawnedObjectName[(offsetof(SpawnAndFadeNugget, m_spawnedObjectName) == 0x12C) ? 1 : -1];
typedef char SpawnAndFadeNugget_off_m_spawnOffset[(offsetof(SpawnAndFadeNugget, m_spawnOffset) == 0x130) ? 1 : -1];

#endif
