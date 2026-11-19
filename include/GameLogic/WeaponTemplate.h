// WeaponTemplate.h: one Weapon INI definition. SIZE 0x180.
// Target facts (retail game.dat):
// - WeaponStore::newWeaponTemplate 0x002CD5FC allocates 0x180 bytes, runs the inline
//   ctor (COMDAT 0x002CD169), sets the name at +0x08 and the name key at +0x0C.
// - The ctor stores the one-slot table VA 0x00C02170 (deleting dtor 0x002CE047) at +0,
//   zero at +0x04, "NoNameWeapon" at +0x08, builds STLport vectors at +0x40/+0x4C, the
//   two sound handles at +0xD0/+0xD8, the projectile filter at +0x120, an STLport list at
//   +0x17C, and the defaults kept in the comments below.
// - Members from +0x10 are the field table VA 0x00C00B48 (offset, parser): the token is
//   the member's name, the parser its type. Offset-0 entries (the nuggets, ClipReloadTime,
//   DelayBetweenShots, ScatterTarget, LinearTarget, WeaponBonus) are class parsers.
// - MaxAttackPassengers is parsed with parseInt at +0x141 while the ctor clears it as a
//   byte and +0x144 is FiringDuration, so the member is one byte wide.
// Members whose parser stores a compound value keep their token and extent as bytes.
#pragma once
#ifndef BFME2_WEAPONTEMPLATE_H
#define BFME2_WEAPONTEMPLATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include <vector>
#include <list>
#include "Common/AsciiString.h"
#include "Common/Coord2D.h"
#include "GameLogic/ObjectFilter.h"

class Object;
class WeaponBonus;
class Coord3D;
class FXList;
class ParticleSystemTemplate;
enum NameKeyType;
enum VeterancyLevel;
class INI;
class Weapon;
class WeaponEffectNugget;
struct ProjectileNuggetFireInfo;
class WeaponBonusSet;
class AssetList;
struct TargetCoord;

class WeaponTemplate
{
public:
	enum { LEVEL_COUNT = 4 };	// the per-veterancy-level FX and exhaust arrays

	WeaponTemplate();													// 0x002CD169 (called by newWeaponTemplate / newOverride)
	virtual ~WeaponTemplate();											// deleting dtor 0x002CE047
	WeaponTemplate &operator=(const WeaponTemplate &that);				// 0x002CD67C (newOverride copies the original through it)

	float getMinimumAttackRange() const;
	float getAttackRange(const Object *, const WeaponBonus &, const Coord3D *) const;
	// 0x002C995D: the attack range for a height difference (Weapon 0x002C9B80 passes the
	// target's z less the source's); the map has a placeholder.
	Real rva002C995D(const Object *source, const WeaponBonus &bonus, Real heightDelta) const;
	// 0x002C9217: AttackRange x the bonus's RANGE less 2.5, raised by RangeBonus/RangeBonusPerFoot
	// when the target is RangeBonusMinHeight or more below, zero past RestrictedHeightRange,
	// floored at zero. 0x002C92B8 and 0x002C92DC pass the height difference to two objects / a point.
	Real rva002C9217(const WeaponBonus &bonus, Real heightDelta) const;
	Real rva002C92B8(const WeaponBonus &bonus, const Object *source, const Object *target) const;
	Real rva002C92DC(const WeaponBonus &bonus, const Coord3D *pos) const;
	// 0x002C98E2: the source's range multiplier: 1 + its attribute 7 when it has one, times
	// GlobalData's GarrisonedRangeMultiplier while INSIDE_GARRISON.
	Real rva002C98E2(const Object *source) const;
	// 0x002C9322: the delay between shots (a random pick in the min/max range), divided by
	// the rate-of-fire bonus times scale, floored and rounded through Float_To_Long.
	Int rva002C9322(const WeaponBonus &bonus, Real scale) const;
	Int getClipReloadTime(const WeaponBonus &bonus) const;							// 0x002C937C
	// 0x002C93DF: PreAttackDelay x the bonus's PRE_ATTACK field; 0x002C93F7: FiringDuration
	// (the bonus is unused). Zero Hour's name for the first; the map has placeholders.
	Int getPreAttackDelay(const WeaponBonus &bonus) const;
	Int getFiringDuration(const WeaponBonus &bonus) const;
	// 0x002C9400..0x002C9415: out-of-line flag getters (MeleeWeapon +0x125, ChaseWeapon +0x126,
	// CanFireWhileMoving +0x81, CanSwoop +0x83); names from the INI tokens.
	Bool isMeleeWeapon() const;
	Bool isChaseWeapon() const;
	Bool canFireWhileMoving() const;
	Bool canSwoop() const;
	// 0x002CBA59: copies +0x160 into the nugget's +0x124 and appends it to the list at +0x17C;
	// every parse<X>Nugget in Weapon.cpp ends with it.
	void addNugget(WeaponEffectNugget *nugget);
	// 0x002CAAFA (map placeholder class Rva002CAAFA; adjustTargetPositionOnVictim 0x002CAB3A calls it with its own this).
	Bool getPreferredTargetBonePosition(Object *victim, Coord3D *pos) const;
	// 0x002CAA59: does any nugget accept the victim (slot 1)? 0x002CAD8B: does any nugget answer
	// slot 13? Placeholder names; the map has none.
	Bool rva002CAA59(Object *victim, const ProjectileNuggetFireInfo *info) const;
	// 0x002CA9CA (map: placeholder ?rva002CA9CA@Rva002CA9CA@@QAE_NHPBX@Z, 143 B, Weapon.cpp): a test of the template
	// against a kind (Object::getRelationship 0x0028D156 asks kind 6 of the current weapon).
	Bool rva002CA9CA(Int kind, const Weapon *weapon) const;
	Bool rva002CAD8B() const;
	void postProcessLoad();															// 0x002CA942 (WeaponStore::postProcessLoad calls it per template)
	const ParticleSystemTemplate *getProjectileExhaust(VeterancyLevel level) const;		// 0x000D1AB3
	Bool shouldProjectileCollideWith(const Object *source, const Object *projectile, const Object *thing,
		ObjectID intendedVictimID) const;												// 0x002CBA7C
	TargetCoord adjustTargetPositionOnVictim(Object *source, Object *victim, Bool arg);		// 0x002CAB3A
	void fireWeaponTemplate(Object *source, Int wslot, Int barrel, Int arg, Object *victim, Int arg2,
		const Coord3D *pos, const WeaponBonus &bonus, Bool flag, Weapon *weapon, Int arg3);	// 0x002CBBF5
	// VA 0x00C00B48: the 100-odd entry table WeaponStore::parseWeaponTemplateDefinition 0x002CE102
	// hands to initFromINI (Zero Hour's name).
	static const struct FieldParse TheWeaponTemplateFieldParseTable[];
	static void parseShotDelay(INI *ini, void *instance, void *store, const void *userData);		// 0x002C9055
	static void parseClipReloadTime(INI *ini, void *instance, void *store, const void *userData);	// 0x002C9136

private:
	static void parseScatterTarget(INI *ini, void *instance, void *store, const void *userData);	// 0x002CE813

public:

	// 0x002CAC6E: hands the asset list to the eight FX lists at +0xA4..+0xC4 (0x001E11F8) and
	// to every nugget's slot 9; FireWeaponNugget's slot 4 0x001F01B9 forwards to it.
	void rva002CAC6E(AssetList &assets, void *context) const;
	// 0x002CA970 (map placeholder Rva002CA9CA::rva002CA970): the pre-fire hook Weapon::preFireWeapon
	// 0x002CDC3D runs with (weapon, source, victim, position).
	void rva002CA970(Weapon *weapon, const Object *source, const Object *victim, const Coord3D *pos) const;
	// 0x002CAA9D: with a source and a victim, runs doEffectObject (slot 5) of every nugget whose
	// slot 7 and canAffectObject (slot 1) agree.
	void rva002CAA9D(const ProjectileNuggetFireInfo *info, const Object *source, Object *victim, const WeaponBonus &bonus);

	Bool isCapableOfFollowingWaypoints() const { return m_capableOfFollowingWaypoints; }
	Bool isShowsAmmoPips() const { return m_showsAmmoPips; }
	Real getContinueAttackRange() const { return m_continueAttackRange; }	// +0x148, read inline (decl_queue h07 14:42, ACCEPTED k10)
	Bool isContactWeapon() const;	// 0x0047A699 (out of line; the fold of the `xor al,al; ret` getters, alias published in m08 P12): AIUpdateInterface::privateAttackPosition 0x0026DB1A calls it on the current weapon's template

	WeaponTemplate *m_nextTemplate;										// 0x004 ctor 0
	AsciiString m_name;													// 0x008 "NoNameWeapon"
	NameKeyType m_nameKey;												// 0x00C
	Int m_fxTrigger;													// 0x010 FXTrigger (names VA 0x00DBC49C)
	Real m_attackRange;													// 0x014 AttackRange
	Real m_minimumAttackRange;											// 0x018 MinimumAttackRange
	Real m_rangeBonusMinHeight;											// 0x01C RangeBonusMinHeight
	Real m_rangeBonus;													// 0x020 RangeBonus
	Real m_rangeBonusPerFoot;											// 0x024 RangeBonusPerFoot
	Real m_requestAssistRange;											// 0x028 RequestAssistRange
	Real m_aimDelta;													// 0x02C AcceptableAimDelta
	Real m_aimDirection;												// 0x030 AimDirection
	Real m_scatterRadius;												// 0x034 ScatterRadius
	Real m_scatterTargetScalar;											// 0x038 ScatterTargetScalar
	Bool m_scatterIndependently;										// 0x03C ScatterIndependently
	Bool m_disableScatterForTargetsOnWall;								// 0x03D DisableScatterForTargetsOnWall
	std::vector<Coord2D> m_scatterTargets;								// 0x040 ScatterTarget (8-byte entries: Weapon::rebuildScatterTargets sizes it with >> 3)
	void *m_unk0x4C[3];													// 0x04C STLport vector
	Int m_damageType;													// 0x058 DamageType, default 22 (UNDEFINED)
	Int m_deathType;													// 0x05C DeathType
	Int m_damageFXType;													// 0x060 DamageFXType, default 29 (UNDEFINED)
	Int m_damageSubType;												// 0x064 DamageSubType
	Real m_weaponSpeed;													// 0x068 WeaponSpeed, default 999999
	Real m_minWeaponSpeed;												// 0x06C MinWeaponSpeed, default 999999
	Real m_maxWeaponSpeed;												// 0x070 MaxWeaponSpeed, default 999999
	Bool m_isScaleWeaponSpeed;											// 0x074 ScaleWeaponSpeed
	Bool m_canBeDodged;													// 0x075 CanBeDodged
	UnsignedInt m_idleAfterFiringDelay;									// 0x078 IdleAfterFiringDelay, default -1
	UnsignedInt m_holdAfterFiringDelay;									// 0x07C HoldAfterFiringDelay
	Bool m_holdDuringReload;											// 0x080 HoldDuringReload
	Bool m_canFireWhileMoving;											// 0x081 CanFireWhileMoving
	Bool m_canFireWhileCharging;										// 0x082 CanFireWhileCharging
	Bool m_canSwoop;													// 0x083 CanSwoop
	Real m_weaponRecoil;												// 0x084 WeaponRecoil
	Real m_minTargetPitch;												// 0x088 MinTargetPitch, default -PI
	Real m_maxTargetPitch;												// 0x08C MaxTargetPitch, default PI
	AsciiString m_preferredTargetBone;									// 0x090 PreferredTargetBone
	const ParticleSystemTemplate *m_projectileExhausts[LEVEL_COUNT];	// 0x094 ProjectileExhaust
	const FXList *m_fireFXs[LEVEL_COUNT];								// 0x0A4 FireFX
	const FXList *m_fireFlankFX;										// 0x0B4 FireFlankFX
	const FXList *m_preAttackFXs[LEVEL_COUNT];							// 0x0B8 PreAttackFX
	UnsignedByte m_fireSound[4];										// 0x0C8 FireSound (parseAudioEventRTS)
	UnsignedInt m_fireSoundLoopTime;									// 0x0CC FireSoundLoopTime
	UnsignedByte m_overrideVoiceAttackSound[8];							// 0x0D0 OverrideVoiceAttackSound
	UnsignedByte m_overrideVoiceEnterStateAttackSound[8];				// 0x0D8 OverrideVoiceEnterStateAttackSound
	WeaponBonusSet *m_extraBonus;										// 0x0E0 ctor 0; Weapon::computeBonus 0x002C9A6F appends it
	Int m_clipSize;														// 0x0E4 ClipSize
	Int m_minClipReloadTime;												// 0x0E8 ClipReloadTime (parseClipReloadTime 0x002C9136)
	Int m_maxClipReloadTime;												// 0x0EC
	Int m_minDelayBetweenShots;											// 0x0F0 DelayBetweenShots (parseShotDelay 0x002C9055)
	Int m_maxDelayBetweenShots;											// 0x0F4
	Int m_continuousFireOneShotsNeeded;									// 0x0F8 ContinuousFireOne, default 0x7FFFFFFF
	Int m_continuousFireTwoShotsNeeded;									// 0x0FC ContinuousFireTwo, default 0x7FFFFFFF
	UnsignedInt m_continuousFireCoastFrames;							// 0x100 ContinuousFireCoast
	UnsignedInt m_autoReloadWhenIdleFrames;								// 0x104 AutoReloadWhenIdle
	Int m_shotsPerBarrel;												// 0x108 ShotsPerBarrel, default 1
	UnsignedInt m_antiMask;												// 0x10C Anti* bits, default 2 (AntiGround)
	UnsignedInt m_affectsMask;											// 0x110 RadiusDamageAffects, default 0xE
	Bool m_hasDamageNugget;												// 0x114 ctor 0; set by parseDamageNugget, parseDamageFieldNugget, parseDOTNugget
	UnsignedInt m_collideMask;											// 0x118 ProjectileCollidesWith, default 8
	Bool m_damageDealtAtSelfPosition;									// 0x11C DamageDealtAtSelfPosition
	ObjectFilter m_projectileFilterInContainer;						// 0x120 ProjectileFilterInContainer (ctor 0x003623E5)
	Bool m_projectileSelf;												// 0x124 ProjectileSelf
	Bool m_meleeWeapon;													// 0x125 MeleeWeapon
	Bool m_chaseWeapon;													// 0x126 ChaseWeapon
	Int m_reloadType;													// 0x128 AutoReloadsClip (names VA 0x00DBC63C)
	Int m_prefireType;													// 0x12C PreAttackType (names VA 0x00DBC64C)
	Bool m_leechRangeWeapon;											// 0x130 LeechRangeWeapon
	Bool m_hitStoredTarget;												// 0x131 HitStoredTarget
	Bool m_capableOfFollowingWaypoints;									// 0x132 CapableOfFollowingWaypoints
	Bool m_showsAmmoPips;												// 0x133 ShowsAmmoPips
	Bool m_allowAttackGarrisonedBldgs;									// 0x134 AllowAttackGarrisonedBldgs
	Bool m_playFXWhenStealthed;											// 0x135 PlayFXWhenStealthed
	UnsignedInt m_preAttackDelay;										// 0x138 PreAttackDelay
	UnsignedInt m_preAttackRandomAmount;								// 0x13C PreAttackRandomAmount
	Bool m_passengerProportionalAttack;									// 0x140 PassengerProportionalAttack
	UnsignedByte m_maxAttackPassengers;									// 0x141 MaxAttackPassengers (see above)
	UnsignedInt m_firingDuration;										// 0x144 FiringDuration
	Real m_continueAttackRange;											// 0x148 ContinueAttackRange
	Real m_scatterRadiusVsInfantry;										// 0x14C ScatterRadiusVsInfantry
	UnsignedInt m_suspendFXDelay;										// 0x150 SuspendFXDelay
	Bool m_ignoreLinearFirstTarget;										// 0x154 IgnoreLinearFirstTarget
	Bool m_forceDisplayPercentReady;									// 0x155 ForceDisplayPercentReady
	Bool m_isAimingWeapon;												// 0x156 IsAimingWeapon
	Bool m_hasGrabNugget;												// 0x157 ctor 0; set by parseGrabNugget
	Real m_hitPercentage;												// 0x158 HitPercentage, default 1.0
	Real m_hitPassengerPercentage;										// 0x15C HitPassengerPercentage
	Bool m_isOverride;													// 0x160 ctor 0; WeaponStore::newOverride 0x002CE063 sets it and refuses to override an override
	Bool m_finishAttackOnceStarted;										// 0x161 FinishAttackOnceStarted, default true
	Real m_restrictedHeightRange;										// 0x164 RestrictedHeightRange
	Bool m_cannotTargetCastleVictims;									// 0x168 CannotTargetCastleVictims
	Bool m_requireFollowThru;											// 0x169 RequireFollowThru
	Bool m_shareTimers;													// 0x16A ShareTimers
	Bool m_noVictimNeeded;												// 0x16B NoVictimNeeded
	Bool m_rotatingTurret;												// 0x16C RotatingTurret
	Bool m_shouldPlayUnderAttackEvaEvent;								// 0x16D ShouldPlayUnderAttackEvaEvent, default true
	Bool m_instantLoadClipOnActivate;									// 0x16E InstantLoadClipOnActivate
	Bool m_lockWhenUsing;												// 0x16F LockWhenUsing
	Bool m_bombardType;													// 0x170 BombardType
	Bool m_useInnateAttributes;											// 0x171 UseInnateAttributes
	Int m_unk0x174;														// 0x174 ctor -1
	AsciiString m_projectileStreamName;									// 0x178 ProjectileStreamName
	std::list<WeaponEffectNugget *> m_nuggets;							// 0x17C addNugget 0x002CBA59 appends, postProcessLoad 0x002CA942 walks, parseClearNuggets 0x002CA8E8 empties
};
// SIZE 0x180
typedef char WeaponTemplate_size[(sizeof(WeaponTemplate) == 0x180) ? 1 : -1];
typedef char WeaponTemplate_off_m_nextTemplate[(offsetof(WeaponTemplate, m_nextTemplate) == 0x004) ? 1 : -1];
typedef char WeaponTemplate_off_m_name[(offsetof(WeaponTemplate, m_name) == 0x008) ? 1 : -1];
typedef char WeaponTemplate_off_m_nameKey[(offsetof(WeaponTemplate, m_nameKey) == 0x00C) ? 1 : -1];
typedef char WeaponTemplate_off_m_fxTrigger[(offsetof(WeaponTemplate, m_fxTrigger) == 0x010) ? 1 : -1];
typedef char WeaponTemplate_off_m_attackRange[(offsetof(WeaponTemplate, m_attackRange) == 0x014) ? 1 : -1];
typedef char WeaponTemplate_off_m_minimumAttackRange[(offsetof(WeaponTemplate, m_minimumAttackRange) == 0x018) ? 1 : -1];
typedef char WeaponTemplate_off_m_rangeBonusMinHeight[(offsetof(WeaponTemplate, m_rangeBonusMinHeight) == 0x01C) ? 1 : -1];
typedef char WeaponTemplate_off_m_rangeBonus[(offsetof(WeaponTemplate, m_rangeBonus) == 0x020) ? 1 : -1];
typedef char WeaponTemplate_off_m_rangeBonusPerFoot[(offsetof(WeaponTemplate, m_rangeBonusPerFoot) == 0x024) ? 1 : -1];
typedef char WeaponTemplate_off_m_requestAssistRange[(offsetof(WeaponTemplate, m_requestAssistRange) == 0x028) ? 1 : -1];
typedef char WeaponTemplate_off_m_aimDelta[(offsetof(WeaponTemplate, m_aimDelta) == 0x02C) ? 1 : -1];
typedef char WeaponTemplate_off_m_aimDirection[(offsetof(WeaponTemplate, m_aimDirection) == 0x030) ? 1 : -1];
typedef char WeaponTemplate_off_m_scatterRadius[(offsetof(WeaponTemplate, m_scatterRadius) == 0x034) ? 1 : -1];
typedef char WeaponTemplate_off_m_scatterTargetScalar[(offsetof(WeaponTemplate, m_scatterTargetScalar) == 0x038) ? 1 : -1];
typedef char WeaponTemplate_off_m_scatterIndependently[(offsetof(WeaponTemplate, m_scatterIndependently) == 0x03C) ? 1 : -1];
typedef char WeaponTemplate_off_m_disableScatterForTargetsOnWall[(offsetof(WeaponTemplate, m_disableScatterForTargetsOnWall) == 0x03D) ? 1 : -1];
typedef char WeaponTemplate_off_m_scatterTargets[(offsetof(WeaponTemplate, m_scatterTargets) == 0x040) ? 1 : -1];
typedef char WeaponTemplate_off_m_unk0x4C[(offsetof(WeaponTemplate, m_unk0x4C) == 0x04C) ? 1 : -1];
typedef char WeaponTemplate_off_m_damageType[(offsetof(WeaponTemplate, m_damageType) == 0x058) ? 1 : -1];
typedef char WeaponTemplate_off_m_deathType[(offsetof(WeaponTemplate, m_deathType) == 0x05C) ? 1 : -1];
typedef char WeaponTemplate_off_m_damageFXType[(offsetof(WeaponTemplate, m_damageFXType) == 0x060) ? 1 : -1];
typedef char WeaponTemplate_off_m_damageSubType[(offsetof(WeaponTemplate, m_damageSubType) == 0x064) ? 1 : -1];
typedef char WeaponTemplate_off_m_weaponSpeed[(offsetof(WeaponTemplate, m_weaponSpeed) == 0x068) ? 1 : -1];
typedef char WeaponTemplate_off_m_minWeaponSpeed[(offsetof(WeaponTemplate, m_minWeaponSpeed) == 0x06C) ? 1 : -1];
typedef char WeaponTemplate_off_m_maxWeaponSpeed[(offsetof(WeaponTemplate, m_maxWeaponSpeed) == 0x070) ? 1 : -1];
typedef char WeaponTemplate_off_m_isScaleWeaponSpeed[(offsetof(WeaponTemplate, m_isScaleWeaponSpeed) == 0x074) ? 1 : -1];
typedef char WeaponTemplate_off_m_canBeDodged[(offsetof(WeaponTemplate, m_canBeDodged) == 0x075) ? 1 : -1];
typedef char WeaponTemplate_off_m_idleAfterFiringDelay[(offsetof(WeaponTemplate, m_idleAfterFiringDelay) == 0x078) ? 1 : -1];
typedef char WeaponTemplate_off_m_holdAfterFiringDelay[(offsetof(WeaponTemplate, m_holdAfterFiringDelay) == 0x07C) ? 1 : -1];
typedef char WeaponTemplate_off_m_holdDuringReload[(offsetof(WeaponTemplate, m_holdDuringReload) == 0x080) ? 1 : -1];
typedef char WeaponTemplate_off_m_canFireWhileMoving[(offsetof(WeaponTemplate, m_canFireWhileMoving) == 0x081) ? 1 : -1];
typedef char WeaponTemplate_off_m_canFireWhileCharging[(offsetof(WeaponTemplate, m_canFireWhileCharging) == 0x082) ? 1 : -1];
typedef char WeaponTemplate_off_m_canSwoop[(offsetof(WeaponTemplate, m_canSwoop) == 0x083) ? 1 : -1];
typedef char WeaponTemplate_off_m_weaponRecoil[(offsetof(WeaponTemplate, m_weaponRecoil) == 0x084) ? 1 : -1];
typedef char WeaponTemplate_off_m_minTargetPitch[(offsetof(WeaponTemplate, m_minTargetPitch) == 0x088) ? 1 : -1];
typedef char WeaponTemplate_off_m_maxTargetPitch[(offsetof(WeaponTemplate, m_maxTargetPitch) == 0x08C) ? 1 : -1];
typedef char WeaponTemplate_off_m_preferredTargetBone[(offsetof(WeaponTemplate, m_preferredTargetBone) == 0x090) ? 1 : -1];
typedef char WeaponTemplate_off_m_projectileExhausts[(offsetof(WeaponTemplate, m_projectileExhausts) == 0x094) ? 1 : -1];
typedef char WeaponTemplate_off_m_fireFXs[(offsetof(WeaponTemplate, m_fireFXs) == 0x0A4) ? 1 : -1];
typedef char WeaponTemplate_off_m_fireFlankFX[(offsetof(WeaponTemplate, m_fireFlankFX) == 0x0B4) ? 1 : -1];
typedef char WeaponTemplate_off_m_preAttackFXs[(offsetof(WeaponTemplate, m_preAttackFXs) == 0x0B8) ? 1 : -1];
typedef char WeaponTemplate_off_m_fireSound[(offsetof(WeaponTemplate, m_fireSound) == 0x0C8) ? 1 : -1];
typedef char WeaponTemplate_off_m_fireSoundLoopTime[(offsetof(WeaponTemplate, m_fireSoundLoopTime) == 0x0CC) ? 1 : -1];
typedef char WeaponTemplate_off_m_overrideVoiceAttackSound[(offsetof(WeaponTemplate, m_overrideVoiceAttackSound) == 0x0D0) ? 1 : -1];
typedef char WeaponTemplate_off_m_overrideVoiceEnterStateAttackSound[(offsetof(WeaponTemplate, m_overrideVoiceEnterStateAttackSound) == 0x0D8) ? 1 : -1];
typedef char WeaponTemplate_off_m_extraBonus[(offsetof(WeaponTemplate, m_extraBonus) == 0x0E0) ? 1 : -1];
typedef char WeaponTemplate_off_m_clipSize[(offsetof(WeaponTemplate, m_clipSize) == 0x0E4) ? 1 : -1];
typedef char WeaponTemplate_off_m_minClipReloadTime[(offsetof(WeaponTemplate, m_minClipReloadTime) == 0x0E8) ? 1 : -1];
typedef char WeaponTemplate_off_m_maxClipReloadTime[(offsetof(WeaponTemplate, m_maxClipReloadTime) == 0x0EC) ? 1 : -1];
typedef char WeaponTemplate_off_m_minDelayBetweenShots[(offsetof(WeaponTemplate, m_minDelayBetweenShots) == 0x0F0) ? 1 : -1];
typedef char WeaponTemplate_off_m_maxDelayBetweenShots[(offsetof(WeaponTemplate, m_maxDelayBetweenShots) == 0x0F4) ? 1 : -1];
typedef char WeaponTemplate_off_m_continuousFireOneShotsNeeded[(offsetof(WeaponTemplate, m_continuousFireOneShotsNeeded) == 0x0F8) ? 1 : -1];
typedef char WeaponTemplate_off_m_continuousFireTwoShotsNeeded[(offsetof(WeaponTemplate, m_continuousFireTwoShotsNeeded) == 0x0FC) ? 1 : -1];
typedef char WeaponTemplate_off_m_continuousFireCoastFrames[(offsetof(WeaponTemplate, m_continuousFireCoastFrames) == 0x100) ? 1 : -1];
typedef char WeaponTemplate_off_m_autoReloadWhenIdleFrames[(offsetof(WeaponTemplate, m_autoReloadWhenIdleFrames) == 0x104) ? 1 : -1];
typedef char WeaponTemplate_off_m_shotsPerBarrel[(offsetof(WeaponTemplate, m_shotsPerBarrel) == 0x108) ? 1 : -1];
typedef char WeaponTemplate_off_m_antiMask[(offsetof(WeaponTemplate, m_antiMask) == 0x10C) ? 1 : -1];
typedef char WeaponTemplate_off_m_affectsMask[(offsetof(WeaponTemplate, m_affectsMask) == 0x110) ? 1 : -1];
typedef char WeaponTemplate_off_m_hasDamageNugget[(offsetof(WeaponTemplate, m_hasDamageNugget) == 0x114) ? 1 : -1];
typedef char WeaponTemplate_off_m_collideMask[(offsetof(WeaponTemplate, m_collideMask) == 0x118) ? 1 : -1];
typedef char WeaponTemplate_off_m_damageDealtAtSelfPosition[(offsetof(WeaponTemplate, m_damageDealtAtSelfPosition) == 0x11C) ? 1 : -1];
typedef char WeaponTemplate_off_m_projectileFilterInContainer[(offsetof(WeaponTemplate, m_projectileFilterInContainer) == 0x120) ? 1 : -1];
typedef char WeaponTemplate_off_m_projectileSelf[(offsetof(WeaponTemplate, m_projectileSelf) == 0x124) ? 1 : -1];
typedef char WeaponTemplate_off_m_meleeWeapon[(offsetof(WeaponTemplate, m_meleeWeapon) == 0x125) ? 1 : -1];
typedef char WeaponTemplate_off_m_chaseWeapon[(offsetof(WeaponTemplate, m_chaseWeapon) == 0x126) ? 1 : -1];
typedef char WeaponTemplate_off_m_reloadType[(offsetof(WeaponTemplate, m_reloadType) == 0x128) ? 1 : -1];
typedef char WeaponTemplate_off_m_prefireType[(offsetof(WeaponTemplate, m_prefireType) == 0x12C) ? 1 : -1];
typedef char WeaponTemplate_off_m_leechRangeWeapon[(offsetof(WeaponTemplate, m_leechRangeWeapon) == 0x130) ? 1 : -1];
typedef char WeaponTemplate_off_m_hitStoredTarget[(offsetof(WeaponTemplate, m_hitStoredTarget) == 0x131) ? 1 : -1];
typedef char WeaponTemplate_off_m_capableOfFollowingWaypoints[(offsetof(WeaponTemplate, m_capableOfFollowingWaypoints) == 0x132) ? 1 : -1];
typedef char WeaponTemplate_off_m_showsAmmoPips[(offsetof(WeaponTemplate, m_showsAmmoPips) == 0x133) ? 1 : -1];
typedef char WeaponTemplate_off_m_allowAttackGarrisonedBldgs[(offsetof(WeaponTemplate, m_allowAttackGarrisonedBldgs) == 0x134) ? 1 : -1];
typedef char WeaponTemplate_off_m_playFXWhenStealthed[(offsetof(WeaponTemplate, m_playFXWhenStealthed) == 0x135) ? 1 : -1];
typedef char WeaponTemplate_off_m_preAttackDelay[(offsetof(WeaponTemplate, m_preAttackDelay) == 0x138) ? 1 : -1];
typedef char WeaponTemplate_off_m_preAttackRandomAmount[(offsetof(WeaponTemplate, m_preAttackRandomAmount) == 0x13C) ? 1 : -1];
typedef char WeaponTemplate_off_m_passengerProportionalAttack[(offsetof(WeaponTemplate, m_passengerProportionalAttack) == 0x140) ? 1 : -1];
typedef char WeaponTemplate_off_m_maxAttackPassengers[(offsetof(WeaponTemplate, m_maxAttackPassengers) == 0x141) ? 1 : -1];
typedef char WeaponTemplate_off_m_firingDuration[(offsetof(WeaponTemplate, m_firingDuration) == 0x144) ? 1 : -1];
typedef char WeaponTemplate_off_m_continueAttackRange[(offsetof(WeaponTemplate, m_continueAttackRange) == 0x148) ? 1 : -1];
typedef char WeaponTemplate_off_m_scatterRadiusVsInfantry[(offsetof(WeaponTemplate, m_scatterRadiusVsInfantry) == 0x14C) ? 1 : -1];
typedef char WeaponTemplate_off_m_suspendFXDelay[(offsetof(WeaponTemplate, m_suspendFXDelay) == 0x150) ? 1 : -1];
typedef char WeaponTemplate_off_m_ignoreLinearFirstTarget[(offsetof(WeaponTemplate, m_ignoreLinearFirstTarget) == 0x154) ? 1 : -1];
typedef char WeaponTemplate_off_m_forceDisplayPercentReady[(offsetof(WeaponTemplate, m_forceDisplayPercentReady) == 0x155) ? 1 : -1];
typedef char WeaponTemplate_off_m_isAimingWeapon[(offsetof(WeaponTemplate, m_isAimingWeapon) == 0x156) ? 1 : -1];
typedef char WeaponTemplate_off_m_hasGrabNugget[(offsetof(WeaponTemplate, m_hasGrabNugget) == 0x157) ? 1 : -1];
typedef char WeaponTemplate_off_m_hitPercentage[(offsetof(WeaponTemplate, m_hitPercentage) == 0x158) ? 1 : -1];
typedef char WeaponTemplate_off_m_hitPassengerPercentage[(offsetof(WeaponTemplate, m_hitPassengerPercentage) == 0x15C) ? 1 : -1];
typedef char WeaponTemplate_off_m_isOverride[(offsetof(WeaponTemplate, m_isOverride) == 0x160) ? 1 : -1];
typedef char WeaponTemplate_off_m_finishAttackOnceStarted[(offsetof(WeaponTemplate, m_finishAttackOnceStarted) == 0x161) ? 1 : -1];
typedef char WeaponTemplate_off_m_restrictedHeightRange[(offsetof(WeaponTemplate, m_restrictedHeightRange) == 0x164) ? 1 : -1];
typedef char WeaponTemplate_off_m_cannotTargetCastleVictims[(offsetof(WeaponTemplate, m_cannotTargetCastleVictims) == 0x168) ? 1 : -1];
typedef char WeaponTemplate_off_m_requireFollowThru[(offsetof(WeaponTemplate, m_requireFollowThru) == 0x169) ? 1 : -1];
typedef char WeaponTemplate_off_m_shareTimers[(offsetof(WeaponTemplate, m_shareTimers) == 0x16A) ? 1 : -1];
typedef char WeaponTemplate_off_m_noVictimNeeded[(offsetof(WeaponTemplate, m_noVictimNeeded) == 0x16B) ? 1 : -1];
typedef char WeaponTemplate_off_m_rotatingTurret[(offsetof(WeaponTemplate, m_rotatingTurret) == 0x16C) ? 1 : -1];
typedef char WeaponTemplate_off_m_shouldPlayUnderAttackEvaEvent[(offsetof(WeaponTemplate, m_shouldPlayUnderAttackEvaEvent) == 0x16D) ? 1 : -1];
typedef char WeaponTemplate_off_m_instantLoadClipOnActivate[(offsetof(WeaponTemplate, m_instantLoadClipOnActivate) == 0x16E) ? 1 : -1];
typedef char WeaponTemplate_off_m_lockWhenUsing[(offsetof(WeaponTemplate, m_lockWhenUsing) == 0x16F) ? 1 : -1];
typedef char WeaponTemplate_off_m_bombardType[(offsetof(WeaponTemplate, m_bombardType) == 0x170) ? 1 : -1];
typedef char WeaponTemplate_off_m_useInnateAttributes[(offsetof(WeaponTemplate, m_useInnateAttributes) == 0x171) ? 1 : -1];
typedef char WeaponTemplate_off_m_unk0x174[(offsetof(WeaponTemplate, m_unk0x174) == 0x174) ? 1 : -1];
typedef char WeaponTemplate_off_m_projectileStreamName[(offsetof(WeaponTemplate, m_projectileStreamName) == 0x178) ? 1 : -1];
typedef char WeaponTemplate_off_m_nuggets[(offsetof(WeaponTemplate, m_nuggets) == 0x17C) ? 1 : -1];

#endif
