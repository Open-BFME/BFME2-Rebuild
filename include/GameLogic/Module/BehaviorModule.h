// BehaviorModule.h: SAGE BehaviorModuleInterface and BehaviorModule. SIZE 0x10.
// Target facts (old repo Code/GameEngine/Include/GameLogic/Module/BehaviorModule.h,
// reverse/class_contracts/BehaviorModule.json, re-read from the image):
// ??0BehaviorModule (0x00253330) calls ??0ObjectModule, stores the interface's
// own 47-purecall table 0x00BEE900 and its BehaviorModule table 0x00BEE9C0 at
// +0x0C and the primary 0x00BEEA7C at +0. The interface has exactly 47 slots and
// no virtual destructor; family getters override: body 0, collide 1, contain 2,
// create 3, damage 4, die 7, special power 8, update 9, upgrade 10, AI update 19.
// Slots 0-42 are 0x000D43D0 `xor eax,eax; ret`, 43/45/46 are 0x0047A69C `ret 4`
// and 44 is 0x000B3FD0 `ret`.
// Interface getters 12-28: each slot is overridden by exactly one module family
// (slot targets in the 47-slot BehaviorModuleInterface tables of vtables.csv), in
// SAGE's getter order: bridge 12, bridge tower 13, bridge scaffold 14, cave 16,
// projectile 18, AI 19, exit 20, dock 22, slow death 23, special-power update 25,
// slaved 26, production 28, spawn 30. The names are SAGE's for those interfaces.
// Slot 9 is the one that places the update interface: 0x00BEFF90's slot 9 (and
// BehaviorModule's own slot 9) is 0x002533D3 for an update module, a 13-byte
// `lea eax,[ecx-0xC]; add ecx,4; neg; sbb; and; ret` that converts this +0x0C
// interface pointer into the object's +0x10 UpdateModuleInterface. A module's
// update interface is at +0x10, never +0x0C.
// The primary table is ObjectModule's 10 slots plus two returning 0.
// ~DieModule (0x0045CE54) restores BehaviorModule's tables inline: its destructor
// is inline and empty. BehaviorModule's own loadPostProcess is 0x000B3FD0 and its
// xfer 0x004C9C7D (Version1, then ObjectModule's xfer).
#pragma once
#ifndef BFME2_BEHAVIORMODULE_H
#define BFME2_BEHAVIORMODULE_H

#include "GameLogic/Module/ObjectModule.h"

class AIUpdateInterface;
class BodyModuleInterface;
class CollideModuleInterface;
class ContainModuleInterface;
class CreateModuleInterface;
class DamageModuleInterface;
class Dict;
class DieModuleInterface;
class SpecialPowerModuleInterface;
class UpdateModuleInterface;
class UpgradeModuleInterface;
class Xfer;

class DockUpdateInterface;

class ExitInterface;
class FoundationAIUpdateInterface;
class SpawnBehaviorInterface;
class SpawnSlaveWalkInterface;
class BridgeBehaviorInterface;
class BridgeTowerBehaviorInterface;
class BridgeScaffoldBehaviorInterface;
class CaveInterface;
class ProjectileUpdateInterface;
class SlowDeathBehaviorInterface;
class SpecialPowerUpdateInterface;
class SlavedUpdateInterface;
class ProductionUpdateInterface;
class RainOfFireUpdateInterface;
class RunOffMapBehaviorInterface;
class BehaviorModuleInterface
{
public:
	virtual BodyModuleInterface *getBody() = 0;							// 0
	virtual CollideModuleInterface *getCollide() = 0;					// 1
	virtual ContainModuleInterface *getContain() = 0;					// 2
	virtual CreateModuleInterface *getCreate() = 0;						// 3
	virtual DamageModuleInterface *getDamage() = 0;						// 4
	virtual void *slot05() = 0;											// 5
	virtual void *slot06() = 0;											// 6
	virtual DieModuleInterface *getDie() = 0;							// 7
	virtual SpecialPowerModuleInterface *getSpecialPower() = 0;			// 8
	virtual UpdateModuleInterface *getUpdate() = 0;						// 9
	virtual UpgradeModuleInterface *getUpgrade() = 0;					// 10
	virtual void *slot11() = 0;
	virtual BridgeBehaviorInterface *getBridgeBehaviorInterface() = 0;		// 12: BridgeBehavior overrides it; BridgeBehavior::getBridgeBehaviorInterfaceFromObject 0x00456556 asks +0x30
	virtual BridgeTowerBehaviorInterface *getBridgeTowerBehaviorInterface() = 0;		// 13: BridgeTowerBehavior overrides it
	virtual BridgeScaffoldBehaviorInterface *getBridgeScaffoldBehaviorInterface() = 0;		// 14: BridgeScaffoldBehavior overrides it; its FromObject 0x004582F3 asks +0x38
	virtual void *slot15() = 0;
	virtual CaveInterface *getCaveInterface() = 0;		// 16: CaveContain overrides it (0x00466457, the ledger names it getCaveInterface)
	virtual void *slot17() = 0;
	virtual ProjectileUpdateInterface *getProjectileUpdateInterface() = 0;		// 18: MissileUpdate and BezierProjectileBehavior override it
	virtual AIUpdateInterface *getAIUpdateInterface() = 0;				// 19
	// 20: Object::getObjectExitInterface 0x0028B445 asks +0x50 of each behavior.
	virtual ExitInterface *getUpdateExitInterface() = 0;
	virtual void *slot21() = 0;
	virtual DockUpdateInterface *getDockUpdateInterface() = 0;			// 22: Object::getDockUpdateInterface 0x0028BCB4 asks +0x58 of each behavior
	virtual SlowDeathBehaviorInterface *getSlowDeathBehaviorInterface() = 0;		// 23: SlowDeathBehavior and its subclasses override it (0x003730FC)
	virtual void *slot24() = 0;
	virtual SpecialPowerUpdateInterface *getSpecialPowerUpdateInterface() = 0;		// 25: every special-power update (SpecialAbilityUpdate, OCLUpdate, ...) overrides it
	virtual SlavedUpdateInterface *getSlavedUpdateInterface() = 0;		// 26: SlavedUpdate overrides it
	virtual void *slot27() = 0;
	virtual ProductionUpdateInterface *getProductionUpdateInterface() = 0;		// 28: ProductionUpdate overrides it; getProductionUpdateInterfaceFromObject 0x0049CEFC asks +0x70
	virtual void *slot29() = 0;
	// 30: Object::getSpawnBehaviorInterface 0x0028BCD4 asks +0x78 of each behavior.
	virtual SpawnBehaviorInterface *getSpawnBehaviorInterface() = 0;
	virtual RainOfFireUpdateInterface *getRainOfFireUpdateInterface() = 0;
	virtual void *slot32() = 0;
	virtual void *slot33() = 0;
	virtual void *slot34() = 0;
	virtual RunOffMapBehaviorInterface *getRunOffMapBehaviorInterface() = 0;
	virtual void *slot36() = 0;
	virtual void *slot37() = 0;
	virtual FoundationAIUpdateInterface *slot38() = 0;
	virtual void *slot39() = 0;
	virtual void *slot40() = 0;
	virtual void *slot41() = 0;
	virtual SpawnSlaveWalkInterface *slot42() = 0;
	virtual void slot43(float readyFraction) = 0;										// 43
	virtual void slot44() = 0;											// 44
	virtual void mapLoadPostProcess(const Dict *properties) = 0;										// 45
	virtual void slot46(Int) = 0;										// 46
};

class BehaviorModule : public ObjectModule, public BehaviorModuleInterface
{
public:
	BehaviorModule(Thing *thing, const ModuleData *moduleData) : ObjectModule(thing, moduleData) {}	// COMDAT 0x00253330

protected:
	virtual ~BehaviorModule() {}
	virtual void loadPostProcess();										// Snapshot slot 1
	virtual void xfer(Xfer *xfer);											// Snapshot slot 3

public:
	virtual void *vslot10() { return 0; }									// primary slot 10
	virtual void *vslot11() { return 0; }									// primary slot 11

	virtual BodyModuleInterface *getBody() { return 0; }
	virtual CollideModuleInterface *getCollide() { return 0; }
	virtual ContainModuleInterface *getContain() { return 0; }
	virtual CreateModuleInterface *getCreate() { return 0; }
	virtual DamageModuleInterface *getDamage() { return 0; }
	virtual void *slot05() { return 0; }
	virtual void *slot06() { return 0; }
	virtual DieModuleInterface *getDie() { return 0; }
	virtual SpecialPowerModuleInterface *getSpecialPower() { return 0; }
	virtual UpdateModuleInterface *getUpdate() { return 0; }
	virtual UpgradeModuleInterface *getUpgrade() { return 0; }
	virtual void *slot11() { return 0; }
	virtual BridgeBehaviorInterface *getBridgeBehaviorInterface() { return 0; }
	virtual BridgeTowerBehaviorInterface *getBridgeTowerBehaviorInterface() { return 0; }
	virtual BridgeScaffoldBehaviorInterface *getBridgeScaffoldBehaviorInterface() { return 0; }
	virtual void *slot15() { return 0; }
	virtual CaveInterface *getCaveInterface() { return 0; }
	virtual void *slot17() { return 0; }
	virtual ProjectileUpdateInterface *getProjectileUpdateInterface() { return 0; }
	virtual AIUpdateInterface *getAIUpdateInterface() { return 0; }
	virtual ExitInterface *getUpdateExitInterface() { return 0; }
	virtual void *slot21() { return 0; }
	virtual DockUpdateInterface *getDockUpdateInterface() { return 0; }
	virtual SlowDeathBehaviorInterface *getSlowDeathBehaviorInterface() { return 0; }
	virtual void *slot24() { return 0; }
	virtual SpecialPowerUpdateInterface *getSpecialPowerUpdateInterface() { return 0; }
	virtual SlavedUpdateInterface *getSlavedUpdateInterface() { return 0; }
	virtual void *slot27() { return 0; }
	virtual ProductionUpdateInterface *getProductionUpdateInterface() { return 0; }
	virtual void *slot29() { return 0; }
	virtual SpawnBehaviorInterface *getSpawnBehaviorInterface() { return 0; }
	virtual RainOfFireUpdateInterface *getRainOfFireUpdateInterface() { return 0; }
	virtual void *slot32() { return 0; }
	virtual void *slot33() { return 0; }
	virtual void *slot34() { return 0; }
	virtual RunOffMapBehaviorInterface *getRunOffMapBehaviorInterface() { return 0; }
	virtual void *slot36() { return 0; }
	virtual void *slot37() { return 0; }
	virtual FoundationAIUpdateInterface *slot38() { return 0; }
	virtual void *slot39() { return 0; }
	virtual void *slot40() { return 0; }
	virtual void *slot41() { return 0; }
	virtual SpawnSlaveWalkInterface *slot42() { return 0; }
	virtual void slot43(float readyFraction) {}
	virtual void slot44() {}
	virtual void mapLoadPostProcess(const Dict *) {}
	virtual void slot46(Int) {}
};
// SIZE 0x10 (BehaviorModuleInterface vptr at 0x0C)

#endif
