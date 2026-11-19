// CollideModule.h: the collide-module interface and the concrete CollideModule.
// Target facts (retail game.dat):
// - CollideModuleInterface's own table 0x00C40818 is six __purecall slots.
//   Slot 0 is onCollide(Object *, const Coord3D *, const Coord3D *) (the map's
//   onCollide rows). ActionManager::canConvertObjectToCarBomb 0x0041B9B0 calls
//   slot 1 with the target and slot 3 with no argument, which are Zero Hour's
//   wouldLikeToCollideWith and isCarBombCrateCollide. Slots 2, 4 and 5 keep
//   Zero Hour's names for the remaining predicates (provisional: BFME 2 has
//   one fewer than Zero Hour's seven).
// - CollideModule is BehaviorModule (ctor 0x00253330) plus the interface at
//   +0x10 and no member (size 0x14). Ctor 0x004BB702 stores 0x00C5A130 at +0,
//   0x00C5A040 at +0xC and 0x00C5A00C at +0x10, whose slots 1-5 are the
//   defaults below (0x005CB9FF `xor al,al; ret 4`, 0x0047A699 `xor al,al; ret`).
#pragma once
#ifndef BFME2_COLLIDEMODULE_H
#define BFME2_COLLIDEMODULE_H

#include "Common/GameType.h"
#include "GameLogic/Module/BehaviorModule.h"
#include "Common/Xfer.h"

class Object;
class Thing;
class Coord3D;

class CollideModuleInterface
{
public:
	virtual void onCollide(Object *other, const Coord3D *loc, const Coord3D *normal) = 0;	// slot 0
	virtual Bool wouldLikeToCollideWith(const Object *other) const = 0;	// slot 1
	virtual Bool isHijackedVehicleCrateCollide() const = 0;			// slot 2
	virtual Bool isCarBombCrateCollide() const = 0;						// slot 3
	virtual Bool isRailroad() const = 0;								// slot 4
	virtual Bool isSalvageCrateCollide() const = 0;						// slot 5
};

class CollideModule : public BehaviorModule, public CollideModuleInterface
{
public:
	CollideModule(Thing *thing, const ModuleData *moduleData);			// 0x004BB702

	virtual CollideModuleInterface *getCollide() { return this; }

	virtual Bool wouldLikeToCollideWith(const Object *) const { return false; }
	virtual Bool isHijackedVehicleCrateCollide() const { return false; }
	virtual Bool isCarBombCrateCollide() const { return false; }
	virtual Bool isRailroad() const { return false; }
	virtual Bool isSalvageCrateCollide() const { return false; }
protected:
	// Out of line: retail's is the COMDAT 0x004BB68E (stores 0x00C5A00C at +0x10, BehaviorModule's
	// tables, then jumps to ~ObjectModule); SquishCollide 0x004BB949 and CrateCollide 0x004BC4FC
	// tail-jump to it. The map names it only by placeholder.
	virtual ~CollideModule();
	// The fold 0x004CE56D (Collide/Die/Damage modules): Version 1, then BehaviorModule's.
	virtual void xfer(Xfer *xfer)
	{
		xfer->Version1();
		BehaviorModule::xfer(xfer);
	}
protected:
	virtual const char *GetSnapshotName() const;	// slot 2 of CollideModule's own table 0x0085A130: 0x004BB733 returns "CollideModule"
	virtual void loadPostProcess();	// out of line (a jmp to BehaviorModule's), ICF-folded with UpdateModule's at 0x0058B03E; inline, MSVC folds it into each subclass's forwarder and loses retail's second level (0x00497805) (h05)
};
// SIZE 0x14

#endif
