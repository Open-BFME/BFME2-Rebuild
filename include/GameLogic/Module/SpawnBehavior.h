// SpawnBehavior.h: the module that spawns and tends slaves. SIZE 0x64 (ModuleFactory operator new).
// Promoted from Draft by f10 (2026-11-17).
// Target facts (retail game.dat): the constructor 0x0045F581 runs UpdateModule's constructor, then stores
// the six interface tables, in this order of subobjects:
//   +0x00 primary (UpdateModule: 0x00C426AC), +0x0C behavior interface (0x00C425F0), +0x10 update interface (0x00C425E0),
//   +0x20 SpawnBehaviorInterface (13 slots, 0x00C425AC), +0x24 DieModuleInterface (1 slot, 0x00C425A8),
//   +0x28 DamageModuleInterface (3 slots, 0x00C4259C), +0x2C a one-slot interface (0x00C42598, slot 0 is
//   0x0045FE4A: it walks the slave list with a callback), +0x30 UpgradeMux (0x00C42550, 8 bytes).
// The tables are consecutive in .rdata (0x00C42598 .. 0x00C425FC), which is why the Draft read each as holding the next.
// Members: the destructor 0x0045F6D3 destroys the two lists at +0x4C and +0x48 (STLport _List_base<int> 0x004EC395, after the
// +0x48 list's clear 0x0023DAA5). +0x48 holds frames (0x004601CC compares the game frame with each node's value),
// +0x4C holds slave object ids (every slave walk looks each up with GameLogic::findObjectByID).
// xfer 0x00460586 transfers, after BehaviorModule::xfer (0x004C9C7D) and a Version(1, 3), UpdateModule::xfer (0x0044DF9F, only
// for version 3 or later): bool +0x52, UpgradeMux::upgradeMuxXfer (0x004CE397) and uint +0x5C from version 2, the spawn template
// by name (+0x38, looked up again on load), int +0x3C, +0x40, +0x44, the frame list, the slave list, bool +0x50, +0x51, int +0x54, uint +0x58.
#pragma once
#ifndef BFME2_SPAWN_BEHAVIOR_H
#define BFME2_SPAWN_BEHAVIOR_H

#include <stddef.h>
#include <list>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "GameLogic/Module/SpawnBehaviorModuleData.h"
#include "GameLogic/Module/DamageModule.h"
#include "GameLogic/Module/DieModuleInterface.h"
#include "GameLogic/Module/SpawnBehaviorInterface.h"

class Thing;
class ModuleData;
class Xfer;
class ThingTemplate;
class AsciiString;
class Object;

// findClosestOrphan's record (0x0045F39B, 0x0045F4D7): the template to look for, the spawner, and the best orphan so far with its squared distance.
struct SpawnOrphanSearch
{
	const ThingTemplate *m_template;	// +0x00
	Object *m_spawner;					// +0x04
	Object *m_best;						// +0x08
	Real m_bestDistSq;					// +0x0C
};

// The +0x2C interface: one slot (0x0045FE4A), the slave walk. The base table 0x00C1C780 is shared with other one-slot interfaces.
class SpawnSlaveWalkInterface
{
public:
	virtual Int rva0045FE4A(Int (*callback)(Object *, void *), void *userData) = 0;	// 0
};

// UpdateModule ends at 0x20; this class's interface bases and members follow it.
typedef char SpawnBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class SpawnBehavior : public UpdateModule, public SpawnBehaviorInterface, public DieModuleInterface, public DamageModuleInterface, public SpawnSlaveWalkInterface, public UpgradeMux
{
public:
	SpawnBehavior(Thing *thing, const ModuleData *moduleData);	// 0x0045F581

	// BehaviorModuleInterface slot 42 returns the slave-walk interface at +0x2C.
	virtual SpawnSlaveWalkInterface *slot42() { return this; }
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	const char *name() const; // 0x0045F6CD
	virtual ~SpawnBehavior();

	// BehaviorModuleInterface (+0x0C) getters: retail 0x00C425F0 slots 4, 7, 10 and 30 return the interface subobjects (shared one-line stubs)
	virtual DamageModuleInterface *getDamage() { return this; }
	virtual DieModuleInterface *getDie() { return this; }
	virtual UpgradeModuleInterface *getUpgrade() { return this; }
	virtual SpawnBehaviorInterface *getSpawnBehaviorInterface() { return this; }

	// SpawnBehaviorInterface (+0x20)
	virtual Bool maySpawnSelfTaskAI(Real maxSelfTaskersRatio);									// 0x0045F2F8
	virtual void onSpawnDeath(ObjectID deadSlave, DamageInfo *damageInfo);			// 0x0045FCFB
	virtual Object *getClosestSlave(const Coord3D *position);							// 0x0045F87C
	virtual void orderSlavesToAttackTarget(Object *target, Int maxShotsToFire, CommandSourceType cmdSource);	// 0x0046070E
	virtual void orderSlavesToAttackPosition(const Coord3D *position, Int maxShotsToFire, CommandSourceType cmdSource);	// 0x00460757
	virtual CanAttackResult getCanAnySlavesAttackSpecificTarget(AbleToAttackType attackType, const Object *target, CommandSourceType cmdSource);	// 0x0045F8E0
	virtual CanAttackResult getCanAnySlavesUseWeaponAgainstTarget(AbleToAttackType attackType, const Object *victim, const Coord3D *pos, CommandSourceType cmdSource);	// 0x0045F93D
	virtual Bool canAnySlavesAttack();													// 0x0045F99E
	virtual void orderSlavesToGoIdle(CommandSourceType cmdSource);						// 0x004607A0
	virtual void onDelete();												// 0x0045F75D (public: UAE)
	virtual void disable();																// 0x0045F37D
	virtual void rva0045FD9F();															// 0x0045FD9F
	virtual Bool rva00498802();														// 0x00498802 (shared byte getter)
	virtual void rva004601CC();															// 0x004601CC

	// DieModuleInterface (+0x24), DamageModuleInterface (+0x28), the slave walk (+0x2C)
	virtual void onDie(const DamageInfo *damageInfo);									// 0x0045F7B5
	virtual void onDamage(DamageInfo *damageInfo);										// 0x0045FDF3
	virtual void onHealing(DamageInfo *damageInfo);										// 0x0047A69C (shared `ret 4`)
	virtual void onBodyDamageStateChange(const DamageInfo *damageInfo, BodyDamageType oldState, BodyDamageType newState);	// 0x004604D5
	virtual Int rva0045FE4A(Int (*callback)(Object *, void *), void *userData);		// 0x0045FE4A

	// UpdateModule
	virtual UpdateSleepTime update();													// 0x004602F4

	Bool shouldTryToSpawn();															// 0x0045F3F8
	Bool reclaimOrphanSpawn();															// 0x0045F9D8
	void computeAggregateStates();														// 0x0045FE9A
	Object *rva0045F4D7();																// 0x0045F4D7 (public, placeholder name)
	void rva00460175(Int arg);															// 0x00460175 (public, placeholder name)

protected:
	virtual void xfer(Xfer *xfer);														// 0x00460586
	virtual void loadPostProcess();														// 0x0045F382
	virtual void getUpgradeActivationMasks(UpgradeMaskType &activation, UpgradeMaskType &conflicting) const;	// 0x0045F45F
	virtual void performUpgradeFX();													// 0x0045F46A
	// UpgradeMux slot 8 (0x004B471F) calls slot 9 with false to clear the executed flag.
	virtual void upgradeRemovalImplementation() { setUpgradeExecuted(false); }
	virtual void upgradeImplementation();												// 0x0045F393
	virtual Bool requiresAllActivationUpgrades() const;								// 0x0045F479
	virtual Bool requiresAllConflictingUpgrades() const;								// 0x0045F483
	virtual void processUpgradeRemoval() { ((const SpawnBehaviorModuleData *)getModuleData())->m_upgradeMuxData.muxDataProcessUpgradeRemoval(getObject()); }	// slot 15: retail 0x0045F46A, ICF-folded with performUpgradeFX (both call the mux data's `ret 4` 0x0047A69C)
	Bool createSpawn();																// 0x0045FA57 (AAE: private in retail)

private:
public:
	const ThingTemplate *m_spawnTemplate;											// +0x38; found from the module data's first SpawnTemplateName
	Int m_unk0x3C;																	// +0x3C; xfer Int; data->m_spawnNumber for a one-shot spawner, else -1
	Int m_unk0x40;																	// +0x40; xfer Int
	Int m_unk0x44;																	// +0x44; xfer Int
	_STL::list<Int> m_unk0x48;												// +0x48; frames (retail instantiates STLport list<int> for both lists)
	_STL::list<Int> m_slaves;													// +0x4C; slave object ids
	Bool m_unk0x50;																	// +0x50; xfer Bool; 1 in the constructor (disable() clears it through the interface)
	Bool m_unk0x51;																	// +0x51; xfer Bool; data->m_aggregateHealth
	Bool m_unk0x52;																	// +0x52; xfer Bool
	Int m_unk0x54;																	// +0x54; xfer Int; -1
	UnsignedInt m_unk0x58;															// +0x58; xfer UnsignedInt
	UnsignedInt m_unk0x5C;															// +0x5C; xfer UnsignedInt; data->m_initialBurst
	const AsciiString *m_unk0x60;													// +0x60; the module data's first SpawnTemplateName entry
};
// SIZE 0x64
typedef char SpawnBehavior_size[(sizeof(SpawnBehavior) == 0x64) ? 1 : -1];
typedef char SpawnBehavior_off_m_spawnTemplate[(offsetof(SpawnBehavior, m_spawnTemplate) == 0x38) ? 1 : -1];
typedef char SpawnBehavior_off_m_unk0x48[(offsetof(SpawnBehavior, m_unk0x48) == 0x48) ? 1 : -1];
typedef char SpawnBehavior_off_m_slaves[(offsetof(SpawnBehavior, m_slaves) == 0x4C) ? 1 : -1];
typedef char SpawnBehavior_off_m_unk0x50[(offsetof(SpawnBehavior, m_unk0x50) == 0x50) ? 1 : -1];
typedef char SpawnBehavior_off_m_unk0x5C[(offsetof(SpawnBehavior, m_unk0x5C) == 0x5C) ? 1 : -1];

#endif
