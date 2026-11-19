// WeaponOCLNugget.h: a weapon effect nugget. SIZE 0x130.
// Target facts (retail game.dat):
// - parseWeaponOCLNugget 0x002CC7DE (Weapon.cpp) allocates 0x130 bytes, runs the ctor 0x0050968B, stores the
//   template at +0x104 and reads the nugget through 0x005095AE (base and class tables).
// - The ctor stores table VA 0x00C64588 (14 slots). Slots whose target differs from the
//   WeaponEffectNugget table: 0 0x005096AB, 1 0x00508925, 2 0x00507943, 5 0x0050962B, 6 0x0050965A, 7 0x0047A699, 8 0x005095FB, 9 0x0050959C.
// - Members from the field table VA 0x00C64568 (token = name, parser = type).
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_WEAPONOCLNUGGET_H
#define BFME2_WEAPONOCLNUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"
#include "Common/AsciiString.h"

class INI;

class WeaponOCLNugget : public WeaponEffectNugget
{
public:
	WeaponOCLNugget();										// 0x0050968B
	virtual ~WeaponOCLNugget();								// 0, deleting dtor 0x005096AB
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x00508925
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);	// 2, 0x00507943
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x0050962B
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x0050965A
	virtual Bool slot07() const;	// 7, 0x0047A699
	virtual void friend_postProcessLoad();	// 8, 0x005095FB
	virtual void GetAssetList(AssetList &assets, void *context);	// 9, 0x0050959C

	// 0x005095AE: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	UnsignedByte m_unk0x128[0x4];					// 0x128
	AsciiString m_weaponOCLName;					// 0x12C WeaponOCLName
};
// SIZE 0x130
typedef char WeaponOCLNugget_size[(sizeof(WeaponOCLNugget) == 0x130) ? 1 : -1];
typedef char WeaponOCLNugget_off_m_weaponOCLName[(offsetof(WeaponOCLNugget, m_weaponOCLName) == 0x12C) ? 1 : -1];

#endif
