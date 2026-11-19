// WeaponSet.h: an object's live weapon slots (Object+0x330). SIZE 0x40.
// Target facts (retail game.dat):
// - A Snapshot: table 0x00C0089C is the deleting dtor 0x002C75B5, the shared
//   empty loadPostProcess 0x000B3FD0, GetSnapshotName 0x002C74FB and xfer
//   0x002C8872.
// - Ctor 0x002C72D5 zeroes +0x04, the six slot pointers at +0x08..+0x1C (loop
//   of 6), +0x20..+0x30, the Bools at +0x34/+0x35, six per-slot bytes at
//   +0x36..+0x3B and +0x3C.
// - xfer reads the template set back by name: TheThingFactory->findTemplate
//   (0x002D06CA), then the template's weapon-set lookup 0x0033DCD1, stored at +4.
// Member names follow Zero Hour's WeaponSet where the slot shape matches; BFME 2
// has six slots (Zero Hour three) and the per-slot bytes and +0x3C are its own.
#pragma once
#ifndef BFME2_WEAPONSET_H
#define BFME2_WEAPONSET_H

#include "Common/GameType.h"
#include "Common/Snapshot.h"

class Object;
class Weapon;
class WeaponTemplateSet;
class Xfer;
enum WeaponSlotType;
enum CanAttackResult;
enum AbleToAttackType;
enum WeaponChoiceCriteria;
enum CommandSourceType;
class Coord3D;

// Zero Hour's lock levels; retail compares the argument and +0x24 with 1 and 2.
enum WeaponLockType
{
	NOT_LOCKED = 0,
	LOCKED_TEMPORARILY,
	LOCKED_PERMANENTLY
};

class WeaponSet : public Snapshot
{
public:
	enum { WEAPONSLOT_COUNT = 6 };

	WeaponSet();														// 0x002C72D5
	virtual ~WeaponSet();												// deleting dtor 0x002C75B5

	Weapon *getWeaponInSlot(WeaponSlotType slot) const;					// 0x002C7469, out of line in retail (an 11-byte COMDAT that SpecialAbilityUpdate::killSpecialObjects 0x0044F72E, Object::getLargestWeaponRange 0x0028AE13 and TurretAI 0x004D8310 call); the definition is the row in WeaponSet.cpp
	bool isOutOfAmmo() const;
	void reloadAllAmmo(const Object *obj, Bool now);					// 0x002C739E
	// Inline: Object::hasAnyWeapon 0x0028ADE0 tests +0x28 in place.
	Bool hasAnyWeapon() const { return m_filledWeaponSlotMask != 0; }
	// Inline: Object::hasAnyDamageWeapon 0x0028ADEC tests +0x35 in place (f08).
	Bool hasAnyDamageWeapon() const { return m_hasDamageWeapon; }
	// Zero Hour's inline; Object 0x0028ADF7 tests +0x30 in place.
	Bool hasWeaponToDealDamageType(Int typeToDeal) const { return (m_totalDamageTypeMask & (1 << typeToDeal)) ? true : false; }
	const Weapon *findAmmoPipShowingWeapon() const;					// 0x002C73F7
	Weapon *findWaypointFollowingCapableWeapon();						// 0x002C741C
	// Zero Hour's name; BFME 2 tests the template set for NULL first.
	Bool isSharedReloadTime() const;									// 0x002C75FA
	// The per-slot bytes at +0x36 override the template set's AutoChooseSources mask: a slot
	// is auto-choosable from a command source when its byte is set or the source's bit is in
	// the mask (0x002C75D1). The names are role names; the map has placeholders.
	void toggleAutoChooseOverride(Int slot);							// 0x002C7447
	void setAutoChooseOverride(Int slot, Bool on);						// 0x002C745A
	Bool isAutoChoosable(Int slot, Int commandSource) const;				// 0x002C75D1
	Bool setWeaponLock(WeaponSlotType weaponSlot, WeaponLockType lockType);	// 0x002C8AAE
	void releaseWeaponLock(WeaponLockType lockType);						// 0x002C8B9B
	void updateWeaponSet(const Object *obj);								// 0x002C8C97
	// Object::getAbleToAttackSpecificObject 0x0028D051 asks it with the attacker (the object, or a horde's member).
	CanAttackResult getAbleToAttackSpecificObject(AbleToAttackType attackType, const Object *source,
		const Object *target, CommandSourceType commandSource) const;	// 0x002C83FC
	// Object's forwarders (0x0028AF2B, 0x0028AF4A, 0x0028AF65) call these with the object as source.
	CanAttackResult getAbleToUseWeaponAgainstTarget(AbleToAttackType attackType, const Object *source,
		const Object *victim, const Coord3D *pos, CommandSourceType commandSource) const;	// 0x002C7907
	Bool chooseBestWeaponForTarget(const Object *obj, const Object *victim, WeaponChoiceCriteria criteria,
		CommandSourceType commandSource);									// 0x002C7D03
	// Out of line in retail (Object::isCurWeaponLocked calls the shared +0x24 getter 0x00091A56).
	WeaponLockType getCurWeaponLockedStatus() const;
	// Zero Hour's inline accessors (Object::fireCurrentWeapon 0x00291873 reads +0x20 and +0x08[+0x20] in place).
	Weapon *getCurWeapon() { return m_weapons[m_currentWeapon]; }
	WeaponSlotType getCurWeaponSlot() const { return (WeaponSlotType)m_currentWeapon; }
	// 0x002C7474: every weapon's +0x50 frame to now (Weapon 0x002C95DE with 0).
	void rva002C7474();

private:
	Bool isAnyWithinTargetPitch(const Object *obj, const Object *victim) const;	// 0x002C7362

public:

protected:
	virtual void loadPostProcess() {}									// 0x000B3FD0
	virtual const char *GetSnapshotName() const;						// 0x002C74FB
	virtual void xfer(Xfer *xfer);										// 0x002C8872

	const WeaponTemplateSet *m_curWeaponTemplateSet;					// 0x04
	Weapon *m_weapons[WEAPONSLOT_COUNT];								// 0x08
	Int m_currentWeapon;												// 0x20
	Int m_currentWeaponLockedStatus;									// 0x24
	UnsignedInt m_filledWeaponSlotMask;									// 0x28
	Int m_totalAntiMask;												// 0x2C (xfer: the int overload)
	UnsignedInt m_totalDamageTypeMask;									// 0x30
	Bool m_hasPitchLimit;												// 0x34
	Bool m_hasDamageWeapon;												// 0x35
	Bool m_slotFlags[WEAPONSLOT_COUNT];									// 0x36 (xfer: the bool overload)
	ObjectID m_ownerID;													// 0x3C the owner (setWeaponLock 0x002C8AAE looks it up)
};
// SIZE 0x40

#endif
