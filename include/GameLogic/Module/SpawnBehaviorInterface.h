// SpawnBehaviorInterface.h: the interface SpawnBehavior exposes at +0x20 (13 slots, all pure in the base table
// 0x00C42518; SpawnBehavior's own table is 0x00C425AC).
// Target facts (f10, 2026-11-17): the slot targets in SpawnBehavior's table 0x00C425AC are, in order,
//   0 0x0045F2F8 (byte f(float), ledger name Rva0045F2F8)
//   1 0x0045FCFB onSpawnDeath(ObjectID, DamageInfo *)
//   2 0x0045F87C getClosestSlave(const Coord3D *)
//   3 0x0046070E orderSlavesToAttackTarget(Object *, Int, CommandSourceType)
//   4 0x00460757 orderSlavesToAttackPosition(const Coord3D *, Int, CommandSourceType)
//   5 0x0045F8E0 getCanAnySlavesAttackSpecificTarget(AbleToAttackType, const Object *, CommandSourceType)
//   6 0x0045F93D getCanAnySlavesUseWeaponAgainstTarget(AbleToAttackType, const Object *, const Coord3D *, CommandSourceType)
//   7 0x0045F99E canAnySlavesAttack()
//   8 0x004607A0 orderSlavesToGoIdle(CommandSourceType)
//   9 0x0045F37D (ledger name Rva0045F37DZeroSetter::disable: stores 0 in the byte at this+0x30)
//  10 0x0045FD9F rva0045FD9F()
//  11 0x00498802 (the shared byte getter: returns the byte at this+0x30)
//  12 0x004601CC rva004601CC()
// The table is followed in .rdata by the UpdateModuleInterface table 0x00C425E0, which is not part of it.
// The class has no data of its own: SpawnBehavior's members are reached from the interface's `this` (module + 0x20).
// The signatures come from the ledger's mangled names of the implementations in SpawnBehavior.
#pragma once
#ifndef BFME2_SPAWNBEHAVIORINTERFACE_H
#define BFME2_SPAWNBEHAVIORINTERFACE_H

#include "Common/GameType.h"

class Object;
class DamageInfo;
class Coord3D;
enum CanAttackResult;
enum AbleToAttackType;

class SpawnBehaviorInterface
{
public:
	virtual Bool maySpawnSelfTaskAI(Real maxSelfTaskersRatio) = 0;									// 0
	virtual void onSpawnDeath(ObjectID deadSlave, DamageInfo *damageInfo) = 0;			// 1
	virtual Object *getClosestSlave(const Coord3D *position) = 0;							// 2
	virtual void orderSlavesToAttackTarget(Object *target, Int maxShotsToFire, CommandSourceType cmdSource) = 0;	// 3
	virtual void orderSlavesToAttackPosition(const Coord3D *position, Int maxShotsToFire, CommandSourceType cmdSource) = 0;	// 4
	virtual CanAttackResult getCanAnySlavesAttackSpecificTarget(AbleToAttackType attackType, const Object *target, CommandSourceType cmdSource) = 0;	// 5
	virtual CanAttackResult getCanAnySlavesUseWeaponAgainstTarget(AbleToAttackType attackType, const Object *victim, const Coord3D *pos, CommandSourceType cmdSource) = 0;	// 6
	virtual Bool canAnySlavesAttack() = 0;													// 7
	virtual void orderSlavesToGoIdle(CommandSourceType cmdSource) = 0;						// 8
	virtual void disable() = 0;																// 9
	virtual void rva0045FD9F() = 0;															// 10
	virtual Bool rva00498802() = 0;														// 11
	virtual void rva004601CC() = 0;															// 12
};

#endif
