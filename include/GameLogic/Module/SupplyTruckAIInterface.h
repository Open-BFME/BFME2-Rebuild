// SupplyTruckAIInterface.h: SupplyTruckAIInterface, the harvester/supply interface an AI
// update returns from AIUpdateInterface::getSupplyTruckAIInterface (slot 95).
// The SupplyTruckAIUpdate class itself is GameLogic/Module/SupplyTruckAIUpdate.h (split out so AIUpdate.h can include only this interface).
// Target facts: SupplyTruckAIUpdate's ctor 0x004A69BF stores table 0x00C52F38 at
// +0x3E4 (20 slots; WorkerAIUpdate keeps its own copy at +0x3E8). Bodies take the
// interface as this, so `this - 0x3E0` is the module data (SupplyAIUpdateModuleData:
// MaxBoxes +0x64, HarvestTrees +0x74, HarvestActivationRange +0x78,
// HarvestPreparationTime +0x7C, HarvestActionTime +0x80).
// Slot names: map rows for 2 (loseOneBox), 3 (gainOneBox), 5
// (isCurrentlyFerryingSupplies), 14 (isForcedIntoBusyState), 19
// (getActionDelayForDock); 0, 4, 13 and 15 take Zero Hour's names by shape and
// order (number of boxes, `return true`, the setter of 14's byte, an ID getter);
// 1 and 6-9 are named for the INI token they return; 10 returns a Real computed
// from the controlling player (Zero Hour's getWarehouseScanDistance slot; not
// proven). 11/12 are a setter/getter pair on interface +0x20, 16 copies the
// Coord3D at +0xC out, 17 is an empty one-argument body and 18 returns 0: role
// names. Users: AIHarvestPrepareSiteState::onEnter 0x00544E3B asks slot 8 and
// the work-site state 0x00544E76 slot 9; AIDockProcessDockState's
// setNextDockActionFrame 0x005445FF asks slot 19.
#pragma once
#ifndef BFME2_SUPPLYTRUCKAIINTERFACE_H
#define BFME2_SUPPLYTRUCKAIINTERFACE_H

#include "Common/GameType.h"

class Object;
class Coord3D;

class SupplyTruckAIInterface
{
public:
	virtual Int getNumberBoxes() const = 0;							// 0, member at interface +0x1C
	virtual Int getMaxBoxes() const = 0;							// 1, MaxBoxes (role name)
	virtual Bool loseOneBox() = 0;									// 2, 0x004A6E8E
	virtual Bool gainOneBox(Int remainingStock) = 0;				// 3, 0x004A6EC6
	virtual Bool isAvailableForSupplying() const = 0;				// 4
	virtual Bool isCurrentlyFerryingSupplies() const = 0;			// 5, 0x004A6A81
	virtual Bool getHarvestTrees() const = 0;						// 6, HarvestTrees (role name)
	virtual Real getHarvestActivationRange() const = 0;				// 7 (role name)
	virtual UnsignedInt getHarvestPreparationTime() const = 0;		// 8 (role name)
	virtual UnsignedInt getHarvestActionTime() const = 0;			// 9 (role name)
	virtual Real getWarehouseScanDistance() const = 0;				// 10 (provisional)
	virtual void setForceWantingState(Bool settingVal) = 0;			// 11 (WorkerAIUpdate's slot 12 body is the map's isForcedIntoWantingState 0x004A9BB0)
	virtual Bool isForcedIntoWantingState() const = 0;				// 12, 0x004A9BB0 in WorkerAIUpdate's table
	virtual void setForceBusyState(Bool settingVal) = 0;			// 13
	virtual Bool isForcedIntoBusyState() const = 0;					// 14, 0x004A6A40
	virtual ObjectID getPreferredDockID() const = 0;				// 15
	virtual Bool getUnknownPosition(Coord3D *pos) const = 0;		// 16 (role name; SupplyTruck's 0x004A6A44 copies a Coord3D out and returns a Bool)
	virtual void unknownNotify17(Int arg) = 0;						// 17 (role name)
	virtual Int unknownQuery18() = 0;								// 18 (role name)
	virtual UnsignedInt getActionDelayForDock(Object *dock) = 0;	// 19, 0x004A6F0A
};
// SIZE 0x4

#endif
