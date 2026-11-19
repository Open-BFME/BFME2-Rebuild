// BridgeTowerBehavior.h: the module a bridge's end towers carry. SIZE 0x24.
// Target facts (retail game.dat):
// - Layout (constructor 0x00458704): BehaviorModule at +0 (0x10 bytes), then three interface
//   subobjects: BridgeTowerBehaviorInterface at +0x10 (table 0x00C40C9C), DieModuleInterface at
//   +0x14 (0x00C40C98) and DamageModuleInterface at +0x18 (0x00C40C8C). The constructor stores the
//   bases' own tables first (0x00BE2B78 at +0x10 and +0x18, 0x00C1C780 at +0x14, so the bases are
//   declared in that order), clears +0x1C and +0x20, then stores its own tables.
// - The tower interface's slots 1 and 2 (bridge id getter, tower type setter) are bodies that
//   retail folded into 0x001DB0A8 and 0x005F69CE, so there are no rows of their own.
// - m_bridgeID +0x1C is what setBridge stores and xfer transfers as an ObjectID; m_towerType
//   +0x20 is the Int xfer transfers through XferBridgeTowerType and the constructor zeroes.
// - onDie 0x00458A88 (DieModuleInterface slot 0), onDamage 0x00458841 and onHealing 0x00458997
//   (DamageModuleInterface slots 0 and 1) are reached through their subobject pointers, so the
//   object is at this-0x10 for the damage pair.
#pragma once
#ifndef BFME2_BRIDGE_TOWER_BEHAVIOR_H
#define BFME2_BRIDGE_TOWER_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/BehaviorModule.h"
#include "GameLogic/Module/BridgeTowerBehaviorInterface.h"
#include "GameLogic/Module/DieModuleInterface.h"
#include "GameLogic/Module/DamageModule.h"
#include "GameLogic/Module/BridgeBehavior.h"

class Thing;
class ModuleData;
class Xfer;
class DamageInfo;

// BehaviorModule ends at 0x10; this class's bases and members follow it.
typedef char BridgeTowerBehavior_base_size[(sizeof(BehaviorModule) == 0x10) ? 1 : -1];

class BridgeTowerBehavior : public BehaviorModule, public BridgeTowerBehaviorInterface, public DieModuleInterface, public DamageModuleInterface
{
public:
	BridgeTowerBehavior(Thing *thing, const ModuleData *moduleData);					// 0x00458704
	virtual NameKeyType getModuleNameKey() const;										// 0x0045876C

	virtual BridgeTowerBehaviorInterface *getBridgeTowerBehaviorInterface() { return this; }	// slot 13
	virtual DamageModuleInterface *getDamage() { return this; }						// slot 4
	virtual DieModuleInterface *getDie() { return this; }								// slot 7

	// BridgeTowerBehaviorInterface
	virtual void setBridge(Object *bridge);											// 0x004586EE
	virtual ObjectID getBridgeID() { return m_bridgeID; }								// slot 1, folded into 0x001DB0A8
	virtual void setTowerType(BridgeTowerType type) { m_towerType = type; }			// slot 2, folded into 0x005F69CE

	static BridgeTowerBehaviorInterface *getBridgeTowerBehaviorInterfaceFromObject(Object *obj);	// 0x0045880B

	// DamageModuleInterface
	virtual void onDamage(DamageInfo *damageInfo);										// 0x00458841
	virtual void onHealing(DamageInfo *damageInfo);									// 0x00458997
	virtual void onBodyDamageStateChange(const DamageInfo *damageInfo, BodyDamageType oldState, BodyDamageType newState) {}	// slot 2, folded into 0x000D1407

	// DieModuleInterface
	virtual void onDie(const DamageInfo *damageInfo);									// 0x00458A88

protected:
	virtual const char *GetSnapshotName() const;										// 0x00458759
	virtual void xfer(Xfer *xfer);														// 0x004587CD
	virtual ~BridgeTowerBehavior();													// 0x004586C7

public:
	ObjectID m_bridgeID;																// +0x1C; xfer ObjectID
	Int m_towerType;																	// +0x20; constructor store, xfer 0x004587CD
};
// SIZE 0x24
typedef char BridgeTowerBehavior_size[(sizeof(BridgeTowerBehavior) == 0x24) ? 1 : -1];
typedef char BridgeTowerBehavior_off_m_bridgeID[(offsetof(BridgeTowerBehavior, m_bridgeID) == 0x1C) ? 1 : -1];
typedef char BridgeTowerBehavior_off_m_towerType[(offsetof(BridgeTowerBehavior, m_towerType) == 0x20) ? 1 : -1];

#endif
