// ContainModule.h: ContainModuleInterface, the contain family's interface.
//
// Target facts: OpenContain's constructor 0x004649F8 stores the interface's
// own table 0x00C430E8 at +0x20 (94 __purecall entries, 0x0003B810) and then
// OpenContain's 0x00C433B0 (94 slots). There is no base class and no virtual
// destructor (the +0x24 interface bounds this one to its vptr). Retail has no
// ContainModule class: no constructor stores either table outside the
// OpenContain family, so the family's base is OpenContain (OpenContain.h).
//
// Slots named from rowed overrides in OpenContain's / CaveContain's tables:
// 3 0x004624E3, 22 0x00463097, 23 0x00464D02, 38 0x00462977, 39 0x00464830,
// 40 0x004638F1 / 0x0046646E, 41 0x00463509 / 0x004666C9, 42 0x004635C0 /
// 0x00466A50, 61 0x004632E0, 68 0x00466486, 28 0x0046651C, 20 0x00466977
// (CaveContain's onContaining 0x004664BB calls slot 20 after
// OpenContain::onContaining, as recalcApparentControllingPlayer).
// Slot 1 (0x004647CD) returns this-0x20, the OpenContain itself. Slot 45 is
// the no-argument passenger-fire query WeaponSet 0x002C7907 makes; slot 81's
// mask is compared with a player's mask in Player::garrisonAllUnits
// 0x002AEEA7; Object::releaseWeaponLock 0x0028D8B6 forwards to slot 91.
// Slots 4, 69, 70 and 73 keep the names their consumers on main already
// use (StealthDetectorUpdate, DetachableRiderUpdate, AutoPickUpUpdate).
// Donor facts: Zero Hour's ContainModule.h supplies method names
// (asOpenContain, getApparentControllingPlayer, getContainExitInterface,
// onSelling, getContainMax, isPassengerAllowedToFire, iterateContained,
// calcBestGarrisonPosition, getPlayerWhoEntered, enableLoadSounds); BFME 2's
// table and call sites, not the donor's shorter order, place them. Slots
// without evidence for a name keep containSlotNN.
#pragma once
#ifndef BFME2_CONTAINMODULE_H
#define BFME2_CONTAINMODULE_H

#include "Common/GameType.h"
#include "Common/BitFlags.h"
#include "GameLogic/Module/HordeContainInterface.h"
#include "GameLogic/ContainmentListView.h"

class Object;
class OpenContain;
class TunnelContain;
class Player;
class Team;
class ExitInterface;
class Coord3D;
enum ObjectEnterExitType;
enum WeaponLockType;
enum WeaponSlotType;
enum CommandSourceType;
class Weapon;

typedef void (*ContainIterateFunc)(Object *obj, void *userData);

class ContainModuleInterface
{
public:
	virtual void containSlot00(Bool) = 0;										// 0
	virtual OpenContain *asOpenContain() = 0;							// 1
	virtual TunnelContain *asTunnelContain() = 0;											// 2
	virtual void containReactToTransformChange() = 0;					// 3
	virtual bool isGarrisonable() const = 0;							// 4
	virtual Bool containSlot05() = 0;					// 5: OpenContain's is the shared `xor al,al; ret` 0x0047A699
	virtual Bool containSlot06() = 0;					// 6: OpenContain's is the shared `xor al,al; ret` 0x0047A699
	virtual Bool isImmuneToClearBuildingAttacks() const = 0;			// 7: GarrisonContain 0x0047840C returns its module data's +0xA1 byte, OpenContain's is the shared `mov al,1` 0x0050B5C6 (Zero Hour's name)
	virtual Bool containSlot08() = 0;					// 8: OpenContain's is the shared `xor al,al; ret` 0x0047A699
	virtual Bool containSlot09() = 0;					// 9: OpenContain's is the shared `xor al,al; ret` 0x0047A699
	virtual void *containSlot10() = 0;					// 10: OpenContain's is the shared `xor eax,eax; ret` 0x000D43D0
	virtual Bool containSlot11() = 0;					// 11: OpenContain's is the shared `xor al,al; ret` 0x0047A699
	virtual Bool containSlot12() = 0;					// 12: OpenContain's is the shared `xor al,al; ret` 0x0047A699
	virtual Bool containSlot13() = 0;					// 13: OpenContain's is the shared `xor al,al; ret` 0x0047A699
	virtual void containSlot14() = 0;
	virtual void containSlot15() = 0;					// 15: OpenContain's is the shared `ret` 0x000B3FD0
	virtual void containSlot16() = 0;
	virtual Bool onObjectWantsToEnterOrExit(Object *obj, ObjectEnterExitType wants) = 0;	// 17
	virtual void containSlot18() = 0;
	virtual const Player *getApparentControllingPlayer(const Player *observingPlayer) const = 0;	// 19
	virtual void recalcApparentControllingPlayer() = 0;					// 20
	virtual void containSlot21(Team *team) = 0;					// 21: OpenContain's is the shared `ret 4` 0x0047A69C
	virtual void onContaining(Object *obj, Bool wasSelected) = 0;		// 22
	virtual void onRemoving(Object *obj) = 0;							// 23
	virtual void containSlot24() = 0;
	virtual void onSelling() = 0;										// 25
	virtual void containSlot26() = 0;
	virtual void containSlot27(Int, Int) = 0;					// 27: OpenContain's is the shared `ret 8` 0x0050B238
	virtual Int getContainMax() const = 0;								// 28
	virtual ExitInterface *getContainExitInterface() = 0;				// 29
	// 30/31: one body (0x0046F7B9 in HordeContain, 0x000D43D0 elsewhere); declared
	// non-const first because MSVC lays same-name overloads out in reverse order.
	// WorldBuilder: "contain->getHordeContainInterface()".
	virtual HordeContainInterface *getHordeContainInterface() = 0;			// 31
	virtual const HordeContainInterface *getHordeContainInterface() const = 0;	// 30
	virtual void orderAllPassengersToExit(CommandSourceType cmdSource) = 0;	// 32: OpenContain 0x00465011 calls aiExit(container, source) on every rider with an AI (Zero Hour's name; ret 4)
	virtual void containSlot33(CommandSourceType cmdSource) = 0;			// 33: OpenContain 0x00465078 calls aiExit(container, source) on the first rider only (ret 4)
	virtual void containSlot34(Int) = 0;					// 34: OpenContain's is the shared `ret 4` 0x0047A69C
	virtual void containSlot35(CommandSourceType cmdSource) = 0;			// 35: empty in OpenContain (0x0047A69C, ret 4); GarrisonContain::onBodyDamageStateChange 0x0047876A calls it with CMD_FROM_AI before slot 32
	virtual void containSlot36() = 0;
	virtual void containSlot37() = 0;					// 37: OpenContain's is the shared `ret` 0x000B3FD0
	virtual Bool isValidContainerFor(Object *obj, Bool checkCapacity, Bool flag) = 0;	// 38
	virtual void addToContain(Object *obj) = 0;							// 39
	virtual void addToContainList(Object *obj) = 0;						// 40
	virtual void removeFromContain(Object *obj, Bool exposeStealthUnits) = 0;	// 41
	virtual void removeAllContained(Bool exposeStealthUnits) = 0;		// 42
	virtual void containSlot43(Object *first, Int count, Object *second) = 0;	// 43: OpenContain 0x0046274C records both objects' ids and the count (at least 1)
	virtual ObjectStatusMaskType getObjectStatusOfContained(const Object *rider) const = 0;	// 44: OpenContain 0x00462CE1 copies its data's +0x58 mask (f06); Object::isAbleToAttack 0x00290B73 passes the asking object, the const Object::isAbleToAttack passes this (k02)
	virtual Bool isPassengerAllowedToFire() const = 0;					// 45
	virtual Bool containSlot46() = 0;					// 46: OpenContain's is the shared `xor al,al; ret` 0x0047A699
	virtual void containSlot47() = 0;
	virtual Bool containSlot48(const Object *rider, const Object *victim) const = 0;	// 48: 0x00462BFF in every contain table; Object::isAbleToAttack 0x00290B73 asks it with (this, current victim) and tests al
	virtual void containSlot49(Int) = 0;					// 49: OpenContain's is the shared `ret 4` 0x0047A69C
	virtual Bool containSlot50() = 0;					// 50: OpenContain's is the shared `xor al,al; ret` 0x0047A699
	virtual Int containSlot51() const = 0;
	virtual Bool containSlot52() = 0;					// 52: OpenContain's is the shared `mov al,1; ret` 0x0050B5C6
	virtual Real containSlot53() = 0;	// 53: 0x004647D1 returns 1.0f
	virtual Bool containSlot54() = 0;					// 54: OpenContain's is the shared `xor al,al; ret` 0x0047A699
	virtual Bool containSlot55(Int a, Int b) = 0;	// 55: 0x004628AB forwards to the containing object's contain
	virtual void *containSlot56() = 0;					// 56: OpenContain's is the shared `xor eax,eax; ret` 0x000D43D0
	virtual Bool containSlot57(Int) = 0;					// 57: OpenContain's is the shared `xor al,al; ret 4` 0x005CB9FF
	virtual Bool containSlot58(Int object) = 0;	// 58: 0x004631FA whether the object (as an int) is in the contain list
	virtual void containSlot59(Object *object) = 0;					// 59: OpenContain's is the shared `ret 4` 0x0047A69C; f11: SpecialAbilityUpdate::startPreparation 0x00450D9A passes its target
	virtual void containSlot60() = 0;					// 60: OpenContain's is the shared `ret` 0x000B3FD0
	virtual Object *getClosestRider(const Coord3D *pos) = 0;			// 61
	virtual void containSlot62() = 0;					// 62: OpenContain's is the shared `ret` 0x000B3FD0
	virtual void containSlot63(Bool) = 0;
	virtual void containSlot64() = 0;
	virtual void containSlot65() = 0;
	virtual void containSlot66() = 0;
	virtual void containSlot67() = 0;
	virtual void iterateContained(ContainIterateFunc func, void *userData, UnsignedInt flags) = 0;	// 68
	// 69: the contained count. Unsigned: ScriptConditions::evaluateIsBuildingEmpty
	// 0x003E4079 tests it with `xor ecx,ecx; cmp ecx,eax; sbb` (0 < count, unsigned).
	virtual UnsignedInt getCount(bool) = 0;
	virtual ContainmentListView getList() = 0;								// 70
	virtual ContainmentListView containSlot71() = 0;
	virtual const Object *friend_getRider() const = 0;	// 72: RiderChangeContain 0x0047EA26 (ledger name); Object::clearDisabled / setDisabledUntil use the result as the rider (k02); OpenContain's is the shared `xor eax,eax; ret` 0x000D43D0
	virtual int getStealthUnitsContained() = 0;								// 73
	virtual void *containSlot74() = 0;					// 74: OpenContain's is the shared `xor eax,eax; ret` 0x000D43D0
	virtual void *containSlot75() = 0;					// 75: OpenContain's is the shared `xor eax,eax; ret` 0x000D43D0
	virtual Bool calcBestGarrisonPosition(Coord3D *sourcePos, const Coord3D *targetPos) = 0;	// 76
	// 77/78: GarrisonContain 0x004788CE (position) and 0x00478850 (victim), HordeContain 0x00468D47 (victim);
	// OpenContain's are the shared `xor al,al; ret 0xC` 0x0041BA5C. Declared in Zero Hour's order (victim
	// first): MSVC 7.1 lays same-name overloads out in reverse, position at 77, victim at 78.
	virtual Bool attemptBestFirePointPosition(Object *source, Weapon *weapon, Object *victim) = 0;
	virtual Bool attemptBestFirePointPosition(Object *source, Weapon *weapon, const Coord3D *targetPos) = 0;
	virtual void containSlot79() = 0;
	virtual Bool containSlot80() = 0;					// 80: OpenContain's is the shared `xor al,al; ret` 0x0047A699
	virtual UnsignedInt getPlayerWhoEntered() const = 0;				// 81
	virtual void containSlot82() = 0;
	virtual void enableLoadSounds(Bool enable) = 0;						// 83
	virtual void containSlot84() = 0;
	virtual Object *containSlot85() const = 0;	// 85: 0x00464825 the module's object
	virtual const Coord3D *containSlot86() = 0;	// 86: 0x004626CA the object's position, or the closest point on land for a kind-of 191 object
	virtual const Coord3D *containSlot87() = 0;	// 87: 0x004626AF slot 86 for a kind-of 191 object, else the position
	virtual void containSlot88() = 0;
	virtual Int containSlot89(Int) = 0;	// 89: 0x0046480A returns -1
	// 90: OpenContain 0x004625BB hands (slot, lockType, failed) to iterateContained (slot 68, flags 1) with the
	// visitor 0x0046258C (Object::setWeaponLock per object, failed |= !result) and returns !failed (`ret 8`);
	// Object::setWeaponLock 0x00290B24 calls [vtbl+0x168] with (weaponSlot, lockType).
	virtual Bool setWeaponLock(WeaponSlotType weaponSlot, WeaponLockType lockType) = 0;	// 90
	virtual void releaseWeaponLock(WeaponLockType lockType) = 0;		// 91
	virtual Bool containSlot92(Object *obj, Int) = 0;	// 92: 0x00462955 the manual pick-up filter accepts the object
	virtual void containSlot93() = 0;
};

#endif
