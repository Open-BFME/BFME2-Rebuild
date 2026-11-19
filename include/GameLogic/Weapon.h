// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Weapon.h: SAGE weapon instance. Partial layout. SIZE 0x60.
// Target facts (old repo Code/GameEngine/Include/GameLogic/Weapon.h): Weapon derives
// from Snapshot; constructor 0x002CC23D, destructor 0x002CC39E (slot 0 ??_G
// 0x002CCE37); loadAmmoNow 0x002CE1AC. The owner id is at +0x08.
// xfer 0x002CDD47 transfers, in order, ObjectID +0x08, WeaponSlotType +0x0C, uint
// +0x14..+0x2C, int +0x34..+0x3C, the +0x40 vector, bool +0x4C, uint +0x50, int +0x54,
// then outside a CRC WeaponStatus +0x10, uint +0x30, int +0x58 and ObjectID +0x5C.
#pragma once
#ifndef BFME2_WEAPON_H
#define BFME2_WEAPON_H

#include <stddef.h>
#include <vector>
#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "GameLogic/WeaponTemplate.h"

// The reload, prefire, affects-mask and collide-mask names (retail VA 0x00DBC63C, 0x00DBC64C,
// 0x00DBC660, 0x00DBC68C, in Weapon's object, in this order). Zero Hour's names and guards.
#ifdef DEFINE_WEAPONRELOAD_NAMES
static const char *TheWeaponReloadNames[] =
{
	"YES",
	"NO",
	"RETURN_TO_BASE",
	NULL
};
#endif

#ifdef DEFINE_WEAPONPREFIRE_NAMES
static const char *TheWeaponPrefireNames[] =
{
	"PER_SHOT",
	"PER_ATTACK",
	"PER_CLIP",
	"PER_POSITION",
	NULL
};
#endif

#ifdef DEFINE_WEAPONAFFECTSMASK_NAMES
static const char *TheWeaponAffectsMaskNames[] =
{
	"SELF",
	"ALLIES",
	"ENEMIES",
	"NEUTRALS",
	"SUICIDE",
	"NOT_SIMILAR",
	"NOT_AIRBORNE",
	"PROJECTILES",
	"SAME_HEIGHT_ONLY",
	"MINES",
	NULL
};
#endif

#ifdef DEFINE_WEAPONCOLLIDEMASK_NAMES
static const char *TheWeaponCollideMaskNames[] =
{
	"ALLIES",
	"ENEMIES",
	"NEUTRAL",
	"STRUCTURES",
	"SHRUBBERY",
	"PROJECTILES",
	"WALLS",
	"SMALL_MISSILES",
	"BALLISTIC_MISSILES",
	"CONTROLLED_STRUCTURES",
	"MONSTERS",
	NULL
};
#endif

// The WeaponBonus condition and field names (retail VA 0x00DBC6C0 and 0x00DBC71C, in Weapon's
// object). Zero Hour's names and guards.
#ifdef DEFINE_WEAPONBONUSCONDITION_NAMES
static const char *TheWeaponBonusNames[] =
{
	"GARRISONED",
	"HORDE",
	"CONTINUOUS_FIRE_MEAN",
	"CONTINUOUS_FIRE_FAST",
	"NATIONALISM",
	"PLAYER_UPGRADE",
	"DRONE_SPOTTING",
	"DEMORALIZED_OBSOLETE",
	"ENTHUSIASTIC",
	"VETERAN",
	"ELITE",
	"HERO",
	"BATTLEPLAN_BOMBARDMENT",
	"BATTLEPLAN_HOLDTHELINE",
	"BATTLEPLAN_SEARCHANDDESTROY",
	"SUBLIMINAL",
	"SOLO_HUMAN_EASY",
	"SOLO_HUMAN_NORMAL",
	"SOLO_HUMAN_HARD",
	"SOLO_AI_EASY",
	"SOLO_AI_NORMAL",
	"SOLO_AI_HARD",
	NULL
};
#endif

#ifdef DEFINE_WEAPONBONUSFIELD_NAMES
static const char *TheWeaponBonusFieldNames[] =
{
	"DAMAGE",
	"RADIUS",
	"RANGE",
	"RATE_OF_FIRE",
	"PRE_ATTACK",
	"FIRING",
	NULL
};
#endif

class Object;
class WeaponBonus;
class Matrix3D;
class Coord3D;

// Zero Hour Weapon.h status. BFME 2 has more: computeStatus 0x002CC422 also returns 4 (before
// the frame at +0x1C) and 5 (before +0x20), and SavedWeaponState's ctor 0x002C9424 starts at
// 6, one past the last value.
enum WeaponStatus
{
	READY_TO_FIRE = 0,
	OUT_OF_AMMO,
	BETWEEN_FIRING_SHOTS,
	RELOADING_CLIP,
	WEAPONSTATUS_4,
	WEAPONSTATUS_5,

	WEAPONSTATUS_COUNT
};

struct LookupListRec;
// The slot names as a lookup list (VA 0x00C00698), SlotToLock's userData.
extern const LookupListRec TheWeaponSlotTypeNamesLookupList[];

// Weapon slots: retail's names (TheWeaponSlotTypeNames, VA 0x00DBC284) are PRIMARY..
// QUINARY; the per-slot arrays of WeaponSet and WeaponTemplateSet hold six.
enum WeaponSlotType
{
	PRIMARY_WEAPON = 0,
	SECONDARY_WEAPON,
	TERTIARY_WEAPON,
	QUATERNARY_WEAPON,
	QUINARY_WEAPON
};

class Weapon;

// One slot's firing state, kept across a weapon-set change: WeaponSet::updateWeaponSet
// 0x002C8C97 builds six (ctor 0x002C9424), fills them from the old weapons (0x002C943B) and
// hands them to each new weapon (Weapon 0x002CE226), which takes the entry whose template
// key matches its own. A struct in retail (the map's mangled name). SIZE 0x14.
struct SavedWeaponState
{
	SavedWeaponState();								// 0x002C9424
	void copyFrom(const Weapon *weapon);			// 0x002C943B

	NameKeyType m_templateKey;			// 0x00 the template's name key
	UnsignedInt m_ammoInClip;			// 0x04 Weapon +0x14
	WeaponStatus m_status;				// 0x08 Weapon +0x10, WEAPONSTATUS_COUNT when empty
	UnsignedInt m_whenLastReloadStarted;	// 0x0C Weapon +0x28
	UnsignedInt m_whenWeCanFireAgain;	// 0x10 Weapon +0x18
};

// What Weapon::processRequestAssistance 0x002C9DF5 hands makeAssistanceRequest 0x002C9D47
// for each of the owner's objects: the asker, its victim and RequestAssistRange squared.
// Zero Hour's name; retail's inline ctor leaves its 0.0 store before the real value.
struct AssistanceRequestData
{
	AssistanceRequestData() : m_requestingObject(NULL), m_victimObject(NULL), m_requestDistanceSquared(0.0f) {}

	const Object *m_requestingObject;	// 0x0
	Object *m_victimObject;				// 0x4
	Real m_requestDistanceSquared;		// 0x8
};

class Weapon : public Snapshot
{
	friend class WeaponStore;
	// The destructor is protected in retail (??1Weapon@@MAE@XZ 0x002CC39E); these
	// owners free their weapons with ::delete (slot 0 with flag 0, then ::operator delete).
	friend class WeaponSet;
	friend class FireWeaponUpdate;
	friend class WeaponFireSpecialAbilityUpdate;
	friend class FireWeaponWhenDamagedBehavior;

protected:
	Weapon(const WeaponTemplate *tmpl, WeaponSlotType wslot);
	Weapon(const Weapon &that);

	virtual ~Weapon();									// 0

public:
	Weapon &operator=(const Weapon &that);				// 0x002C9462 (public in retail: ??4Weapon@@QAE)

	void setOwnerID(ObjectID id) { m_ownerID = id; }
	void loadAmmoNow(const Object *source);
	void reloadAmmo(const Object *source);						// 0x002CE1E9
	Bool isWithinTargetPitch(const Object *source, const Object *victim) const;	// 0x002C9C82
	Real rva002C957E() const;							// 0x002C957E, the minimum attack range
	Real getAttackRange(const Object *source) const;
	UnsignedInt getPossibleNextShotFrame() const { return m_whenWeCanFireAgain; }
	const WeaponTemplate *getTemplate() const { return m_template; }
	// WeaponSet::findAmmoPipShowingWeapon 0x002C73F7 and findWaypointFollowingCapableWeapon
	// 0x002C741C read the template's flags in place.
	Bool isShowsAmmoPips() const { return m_template->isShowsAmmoPips(); }
	Real getContinueAttackRange() const { return m_template->getContinueAttackRange(); }	// inline (decl_queue h07 14:42, ACCEPTED k10)
	Bool isCapableOfFollowingWaypoints() const { return m_template->isCapableOfFollowingWaypoints(); }
	WeaponStatus getStatus() const;							// 0x002CCFC7
	Bool rva002CE74B(const Object *source, const Object *victim);	// 0x002CE74B (map placeholder; Object::fireCurrentWeapon 0x00291873 fires through it under object status 37)
	// Placeholder names (the map has no names; roles in Weapon.cpp).
	Bool rva002C9586();									// 0x002C9586, frame in [+0x1C, +0x24)
	void rva002C959E();									// 0x002C959E, +0x58 = random pre-attack extra
	void rva002C95DE(Int frames);						// 0x002C95DE, +0x50 = now + frames
	Bool rva002C95F0() const;							// 0x002C95F0, now < +0x50
	Int rva002C9504() const;								// 0x002C9504, +0x18 less +0x28 (the current reload/shot span)
	// 0x002C969D (map placeholder name and argument types): with a GrabNugget weapon, whether the
	// container object can take the grabbed object (ContainModuleInterface slot 38 with true, false).
	Bool rva002C969D(const void *container, Int grabbed);
	Bool rva002C96D8();									// 0x002C96D8, CanFireWhileCharging or ForceDisplayPercentReady
	// 0x002CE226: take the saved state whose template key matches, else load (InstantLoadClip
	// OnActivate) or start reloading.
	void rva002CE226(const Object *source, const SavedWeaponState *states);
	Object *forceFireWeapon(const Object *source, const Coord3D *pos);	// 0x002CE76F
	// The rest of Weapon.cpp's ledger-named members (declared for its fill; types from the
	// mangled names).
	Object *forceFireWeapon(const Object *source, const Object *victim);				// 0x002CE7A4
	Bool fireWeapon(const Object *source, const Coord3D *pos, Int *projectileID);		// 0x002CE6E8
	Bool fireProjectileDetonationWeapon(const Object *source, const Coord3D *pos, const Coord3D *dir, Int arg);	// 0x002CE72B
	Bool privateFireWeapon(const Object *source, const Coord3D *pos, const Object *victim, Int arg, const Coord3D *pos2,
		Bool a, Bool b, UnsignedInt c, Int *projectileID);								// 0x002CE30A
	void preFireWeapon(const Object *source, const Object *victim, const Coord3D *pos);	// 0x002CDC3D
	void prepareProjectileStreamForFiring();											// 0x002CC4DD
	WeaponStatus computeStatus(Bool *cacheIt) const;									// 0x002CC422
	void cacheStatus(WeaponStatus status) const;										// 0x002C94F5
	Bool isAmmoReady() const;														// 0x002CC3FF
	UnsignedInt getRemainingAmmo(Bool arg) const;										// 0x002CC578
	Real getPercentReadyToFire() const;												// 0x002CCE53
	Int getClipReloadTime(const Object *source) const;								// 0x002C9AC5
	Int getPreAttackDelay(const Object *source, const Object *victim, const Coord3D *pos) const;	// 0x002CCFF0
	Real getAttackRange(const Object *source, const Object *victim) const;				// 0x002C9BE4
	Real rva002C9B80(const Object *source, Real heightDelta) const;					// 0x002C9B80, range for a height difference
	Real rva002C9BC3(const Object *source, const Coord3D *pos) const;					// 0x002C9BC3, range to a point
	Int rva002C9D3B(const Object *source, UnsignedInt flags) const;					// 0x002C9D3B, the template's FiringDuration
	Real getAttackDistance(const Object *source, const Object *victim, const Coord3D *pos) const;	// 0x002C9C37
	Bool isTooClose(const Object *source, const Object *victim) const;					// 0x002C9AFE
	Bool isTooClose(const Object *source, const Coord3D *pos) const;					// 0x002C9B3D
	Bool isWithinAttackRange(const Object *source, const Object *victim, Real range, Int arg) const;	// 0x002CB933
	char isWithinAttackRange(Object *source, void *targetPos, Real range, Int arg) const;	// 0x002CB902 (map: ?isWithinAttackRange@Weapon@@QBEDPAVObject@@PAXMH@Z; the position twin of 0x002CB933)
	// 0x002CB35C: its last argument is never read; 0x002CB933 and 0x002CB902 push their own Int
	// argument straight into it (no bool conversion), so it is declared Int here.
	Bool isWithinAttackRange(Object *source, const Coord3D *pos, Object *victim, const Coord3D *pos2, Real range, Int arg);	// 0x002CB35C
	Bool isSourceObjectWithGoalPositionWithinAttackRange(const Object *source, const Coord3D *goal, const Object *victim,
		const Coord3D *victimPos) const;												// 0x002CB2D1
	Bool computeApproachTarget(const Object *source, const Object *victim, const Coord3D *pos, Real angle, Coord3D &result) const;	// 0x002CAEA3
	Bool isClearFiringLineOfSightTerrain(const Object *source, const Coord3D &pos) const;	// 0x002C961F
	// 0x002CB967 (map placeholder rva002CB967): the line of sight from the firing origin to the
	// victim's aim point, through TheTerrainLogic slot 15.
	Bool isClearFiringLineOfSightTerrain(const Object *source, const Object *victim) const;
	Coord3D bfmeGetLOSVictimPos(const Object *source, const Object *victim, Int arg) const;	// 0x002CB65B
	Bool isClearGoalFiringLineOfSightTerrain(const Object *source, const Coord3D &goal, const Coord3D &pos) const;	// 0x002C9655
	void setClipPercentFull(Real percent, Bool arg);									// 0x002CE280
	void setLeechRangeActive(Bool active);											// 0x002D95FE
	void setMaxShotCount(Int count);													// 0x00661D50
	static void calcProjectileLaunchPosition(const Object *launcher, WeaponSlotType wslot, Int barrel, Matrix3D &mtx, Coord3D &pos);	// 0x002C9E48
	static void positionProjectileForLaunch(Object *projectile, const Object *launcher, WeaponSlotType wslot, Int barrel);	// 0x002CA753

protected:
	void computeBonus(const Object *source, UnsignedInt flags, WeaponBonus &bonus) const;		// 0x002C9A6F
	void reloadWithBonus(const Object *source, const WeaponBonus &bonus, Bool loadInstantly);	// 0x002CDB99
	void rebuildScatterTargets();													// 0x002CDB53
	void getFiringLineOfSightOrigin(const Object *source, Coord3D &origin) const;			// 0x002C9601
	void processRequestAssistance(const Object *source, Object *victim);					// 0x002C9DF5

public:
	// 0x002CE6C5: fires at an object through privateFireWeapon with the victim's position;
	// FireWeaponCollide::onCollide passes (me, other's ID, other, NULL). Zero Hour's name,
	// BFME 2's argument list.
	Bool fireWeapon(const Object *source, ObjectID victimID, const Object *victim, Int *projectileID);

protected:
	virtual void loadPostProcess();						// 1
	virtual const char *GetSnapshotName() const;		// 2
	virtual void xfer(Xfer *xfer);						// 3

public:
	const WeaponTemplate *m_template;			// 0x04
	ObjectID m_ownerID;							// 0x08
	WeaponSlotType m_wslot;						// 0x0C
	mutable Int m_status;						// 0x10 WeaponStatus (cacheStatus 0x002C94F5 is const)
	UnsignedInt m_ammoInClip;					// 0x14
	UnsignedInt m_whenWeCanFireAgain;			// 0x18
	UnsignedInt m_whenPreAttackFinished;		// 0x1C
	UnsignedInt m_unk0x20;						// 0x20 a frame: computeStatus 0x002CC422 returns 5 before it
	UnsignedInt m_lastFireFrame;				// 0x24
	UnsignedInt m_whenLastReloadStarted;		// 0x28 getPercentReadyToFire 0x002CCE53 measures from it
	UnsignedInt m_unk0x2C;						// 0x2C
	UnsignedInt m_suspendFXFrame;				// 0x30
	Int m_maxShotCount;							// 0x34
	Int m_curBarrel;							// 0x38
	Int m_numShotsForCurBarrel;					// 0x3C
	// 0x40: indices into the template's ScatterTarget list not yet used (rebuildScatterTargets
	// 0x002CDB53). Includers need // stlport46.
	std::vector<Int> m_scatterTargetsUnused;
	Bool m_pitchLimited;						// 0x4C
	UnsignedInt m_unk0x50;						// 0x50 a frame (0x002C95DE sets now + n, 0x002C95F0 tests now < it)
	Int m_unk0x54;								// 0x54 xfer int
	Int m_unk0x58;								// 0x58 xfer int (not in a CRC); 0x002C959E: random 0..PreAttackRandomAmount
	ObjectID m_projectileStreamID;				// 0x5C xfer ObjectID (not in a CRC); ~Weapon 0x002CC39E destroys that object; prepareProjectileStreamForFiring 0x002CC4DD creates it
};
// SIZE 0x60

#endif
