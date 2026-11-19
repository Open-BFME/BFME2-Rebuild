// ProjectileNugget.h: a weapon effect nugget. SIZE 0x154.
// Target facts (retail game.dat):
// - parseProjectileNugget 0x002CC841 (Weapon.cpp) allocates 0x154 bytes, runs the ctor 0x00509CC3, stores the
//   template at +0x104 and reads the nugget through 0x00509780 (base and class tables).
// - The ctor stores table VA 0x00C646B0 (14 slots). Slots whose target differs from the
//   WeaponEffectNugget table: 0 0x00509D30, 1 0x00509720, 2 0x00507943, 5 0x00509814, 6 0x00509A3C, 7 0x0047A699, 8 0x005097CD, 9 0x00509753, 11 0x00509D29, 12 0x00509D22.
// - Members from the field table VA 0x00C64640 (token = name, parser = type).
// Slot names follow WeaponEffectNugget.h; the name is the parser's (ledger).
#pragma once
#ifndef BFME2_PROJECTILENUGGET_H
#define BFME2_PROJECTILENUGGET_H

#include "GameLogic/WeaponEffects/WeaponEffectNugget.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"

class INI;
class ThingTemplate;

class ProjectileNugget : public WeaponEffectNugget
{
public:
	ProjectileNugget();										// 0x00509CC3
	virtual ~ProjectileNugget();								// 0, deleting dtor 0x00509D30
	virtual Bool canAffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 1, 0x00509720
	virtual Bool slot02(const ProjectileNuggetFireInfo *info, Int arg);	// 2, 0x00507943
	virtual void doEffectObject(const ProjectileNuggetFireInfo *info, Object *victim);	// 5, 0x00509814
	virtual void doEffectPosition(const ProjectileNuggetFireInfo *info, const Coord3D *pos);	// 6, 0x00509A3C
	virtual Bool slot07() const;	// 7, 0x0047A699
	virtual void friend_postProcessLoad();	// 8, 0x005097CD
	virtual void GetAssetList(AssetList &assets, void *context);	// 9, 0x00509753
	virtual Int slot11() const;	// 11, 0x00509D29
	virtual Bool slot12() const;	// 12, 0x00509D22

	// 0x00509780: the INI parse callback the parser calls with (ini, nugget, NULL, NULL); it
	// reads the base table and this class's table together.
	static void parseFromINI(INI *ini, void *instance, void *store, const void *userData);

	const WeaponTemplate *m_warheadTemplate;			// 0x128 friend_postProcessLoad 0x005097CD: TheWeaponStore->findWeaponTemplate(WarheadTemplateName)
	const ThingTemplate *m_projectileTemplate;			// 0x12C friend_postProcessLoad: TheThingFactory->findTemplate(ProjectileTemplateName) when named
	AsciiString m_warheadTemplateName;					// 0x130 WarheadTemplateName
	AsciiString m_projectileTemplateName;					// 0x134 ProjectileTemplateName
	AsciiString m_projectileStreamName;					// 0x138 ProjectileStreamName
	Int m_weaponLaunchBoneSlotOverride;					// 0x13C WeaponLaunchBoneSlotOverride (a WeaponSlotType)
	Coord3D m_alwaysAttackHereOffset;					// 0x140 AlwaysAttackHereOffset
	Bool m_useAlwaysAttackOffset;					// 0x14C UseAlwaysAttackOffset
	UnsignedByte m_unk0x14D[0x7];					// 0x14D
};
// SIZE 0x154
typedef char ProjectileNugget_size[(sizeof(ProjectileNugget) == 0x154) ? 1 : -1];
typedef char ProjectileNugget_off_m_warheadTemplate[(offsetof(ProjectileNugget, m_warheadTemplate) == 0x128) ? 1 : -1];
typedef char ProjectileNugget_off_m_projectileTemplate[(offsetof(ProjectileNugget, m_projectileTemplate) == 0x12C) ? 1 : -1];
typedef char ProjectileNugget_off_m_warheadTemplateName[(offsetof(ProjectileNugget, m_warheadTemplateName) == 0x130) ? 1 : -1];
typedef char ProjectileNugget_off_m_projectileTemplateName[(offsetof(ProjectileNugget, m_projectileTemplateName) == 0x134) ? 1 : -1];
typedef char ProjectileNugget_off_m_projectileStreamName[(offsetof(ProjectileNugget, m_projectileStreamName) == 0x138) ? 1 : -1];
typedef char ProjectileNugget_off_m_weaponLaunchBoneSlotOverride[(offsetof(ProjectileNugget, m_weaponLaunchBoneSlotOverride) == 0x13C) ? 1 : -1];
typedef char ProjectileNugget_off_m_alwaysAttackHereOffset[(offsetof(ProjectileNugget, m_alwaysAttackHereOffset) == 0x140) ? 1 : -1];
typedef char ProjectileNugget_off_m_useAlwaysAttackOffset[(offsetof(ProjectileNugget, m_useAlwaysAttackOffset) == 0x14C) ? 1 : -1];

#endif
