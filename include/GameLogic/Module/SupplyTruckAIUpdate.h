// SupplyTruckAIUpdate.h: the supply truck AI update (f03, promoted from Draft). SIZE 0x408 (ModuleFactory operator new).
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts: the constructor 0x004A71A1 calls AIUpdateInterface first and stores the tables 0x00C52FB0 at +0x0,
// 0x00BFA3C0 at +0xC, 0x00C52FA0 at +0x10, 0x00C47B88 at +0x20, 0x00C52F88 at +0x24 and 0x00C52F38 at +0x3E4 (the
// SupplyTruckAIInterface). xfer 0x004A6B3A transfers the members below after AIUpdateInterface::xfer.
#pragma once
#ifndef BFME2_SUPPLYTRUCKAIUPDATE_H
#define BFME2_SUPPLYTRUCKAIUPDATE_H

#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "GameLogic/Module/AIUpdate.h"
#include "GameLogic/Module/SupplyTruckAIInterface.h"
#include "GameLogic/Module/SupplyTruckStateMachine.h"
#include "GameLogic/Module/AIUpdate.h"

class Thing;
class ModuleData;
class Xfer;
class SupplyTruckStateMachine;

// SupplyTruckAIUpdateModuleData: Zero Hour's class (SupplyTruckAIUpdate.h) without the depleted-supplies voice, plus
// BFME 2's four harvest fields. SIZE 0x84 (ModuleFactory operator new). Its constructor 0x0024EE77 (emitted with the
// module factory's friend_newModuleData copies: the class-body inline) calls the AI module data's 0x0026E5D7, stores
// the members below and the table 0x00C4B6C8. Parse table entries (Draft SupplyAIUpdateModuleData.h): MaxBoxes +0x64,
// SupplyCenterActionDelay +0x68, SupplyWarehouseActionDelay +0x6C, SupplyWarehouseScanDistance +0x70, HarvestTrees
// +0x74, HarvestActivationRange +0x78, HarvestPreparationTime +0x7C, HarvestActionTime +0x80 (durations: UnsignedInt).
// The base is the AI module data, which the map still spells TransportAIUpdateModuleData (0x0026E5D7 is the base call
// of every AI update's module data; h07 asks the map to rename it AIUpdateModuleData).
class SupplyTruckAIUpdateModuleData : public AIUpdateModuleData
{
public:
	Int m_maxBoxesData;										// +0x64 MaxBoxes
	UnsignedInt m_centerDelay;								// +0x68 SupplyCenterActionDelay
	UnsignedInt m_warehouseDelay;							// +0x6C SupplyWarehouseActionDelay
	Real m_warehouseScanDistance;							// +0x70 SupplyWarehouseScanDistance
	Bool m_harvestTrees;									// +0x74 HarvestTrees
	Real m_harvestActivationRange;							// +0x78 HarvestActivationRange
	UnsignedInt m_harvestPreparationTime;					// +0x7C HarvestPreparationTime
	UnsignedInt m_harvestActionTime;						// +0x80 HarvestActionTime

	SupplyTruckAIUpdateModuleData()
	{
		m_maxBoxesData = 0;
		m_centerDelay = 0;
		m_warehouseDelay = 0;
		m_warehouseScanDistance = 100;
		m_harvestTrees = false;
		m_harvestActivationRange = 50;
		m_harvestPreparationTime = 1;
		m_harvestActionTime = 1;
	}
};
typedef char SupplyTruckAIUpdateModuleData_size[(sizeof(SupplyTruckAIUpdateModuleData) == 0x84) ? 1 : -1];
typedef char SupplyTruckAIUpdateModuleData_off_m_maxBoxesData[(offsetof(SupplyTruckAIUpdateModuleData, m_maxBoxesData) == 0x64) ? 1 : -1];
typedef char SupplyTruckAIUpdateModuleData_off_m_harvestTrees[(offsetof(SupplyTruckAIUpdateModuleData, m_harvestTrees) == 0x74) ? 1 : -1];
typedef char SupplyTruckAIUpdateModuleData_off_m_harvestActionTime[(offsetof(SupplyTruckAIUpdateModuleData, m_harvestActionTime) == 0x80) ? 1 : -1];

// The supply truck update: AIUpdateInterface (0x3E4 bytes) followed by the SupplyTruckAIInterface vptr at +0x3E4 (table
// 0x00C52F38, 20 slots), then the members its xfer 0x004A6B3A transfers. The constructor is 0x004A71A1, the destructor
// 0x004A69BF (it deletes the state machine through its virtual destructor with flag 0, then operator delete: `::delete`).
class SupplyTruckAIUpdate : public AIUpdateInterface, public SupplyTruckAIInterface
{
public:
	SupplyTruckAIUpdate(Thing *thing, const ModuleData *moduleData);	// 0x004A71A1
	virtual ~SupplyTruckAIUpdate();										// 0x004A69BF
	// Primary slots 94 and 95 return the supply interface at +0x3E4 (table 0x00852FB0).
	virtual const SupplyTruckAIInterface *getSupplyTruckAIInterface() const { return this; }
	virtual SupplyTruckAIInterface *getSupplyTruckAIInterface() { return this; }

	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();									// 0x004A6A58; update interface slot 0 at +0x10

	// SupplyTruckAIInterface (+0x3E4). Bodies take the interface as this (retail reads the module data at this-0x3E0 and
	// the members at this+0x1C/+0x20/+0x21/+0x8). Slots 0, 1, 4, 6-9, 11-15, 17 and 18 are emitted inline (one-to-three
	// instruction bodies, several folded with other classes' copies); 2, 3, 10 and 19 are in SupplyTruckAIUpdate.cpp.
	virtual Int getNumberBoxes() const { return m_numberBoxes; }							// 0, 0x0030F2C7 (fold)
	virtual Int getMaxBoxes() const { return getSupplyTruckAIUpdateModuleData()->m_maxBoxesData; }	// 1, 0x004A6E29
	virtual Bool loseOneBox();											// 2, 0x004A6E8E
	virtual Bool gainOneBox(Int remainingStock);						// 3, 0x004A6EC6
	virtual Bool isAvailableForSupplying() const { return true; }		// 4, 0x0050B5C6 (the shared `return true`)
	// 5, 0x004A6A81 (emitted inline): Zero Hour's state test; retail tests states 2, 4 and 5 true, 0, 1 and 3 false.
	virtual Bool isCurrentlyFerryingSupplies() const
	{
		if (m_supplyTruckStateMachine)
		{
			switch (m_supplyTruckStateMachine->getCurrentStateID())
			{
				case 2:
				case 4:
				case 5:
					return true;
				case 0:
				case 1:
				case 3:
					return false;
			}
		}
		return false;
	}
	virtual Bool getHarvestTrees() const { return getSupplyTruckAIUpdateModuleData()->m_harvestTrees; }					// 6, 0x004A6FE5
	virtual Real getHarvestActivationRange() const { return getSupplyTruckAIUpdateModuleData()->m_harvestActivationRange; }	// 7, 0x004A6FEF
	virtual UnsignedInt getHarvestPreparationTime() const { return getSupplyTruckAIUpdateModuleData()->m_harvestPreparationTime; }	// 8, 0x004A6FF9
	virtual UnsignedInt getHarvestActionTime() const { return getSupplyTruckAIUpdateModuleData()->m_harvestActionTime; }	// 9, 0x004A7003
	virtual Real getWarehouseScanDistance() const;						// 10, 0x004A6FBF
	virtual void setForceWantingState(Bool settingVal) { m_forcePending = settingVal; }		// 11, 0x00210CD3 (fold)
	virtual Bool isForcedIntoWantingState() const { return m_forcePending; }				// 12, 0x00232E8F (fold)
	virtual void setForceBusyState(Bool settingVal) { m_forcedBusyPending = settingVal; }	// 13, 0x005975D7 (fold)
	virtual Bool isForcedIntoBusyState() const { return m_forcedBusyPending; }				// 14, 0x004A6A40
	virtual ObjectID getPreferredDockID() const { return m_preferredDock; }				// 15, 0x0030F45F (fold)
	virtual Bool getUnknownPosition(Coord3D *pos) const;				// 16, 0x004A6A44 (copies +0x3F0 out, returns the Bool at +0x3FC)
	virtual void unknownNotify17(Int arg) {}							// 17, 0x0047A69C (the shared empty one-argument body)
	virtual Int unknownQuery18() { return 0; }							// 18, 0x000D43D0 (the shared `return 0`)
	virtual UnsignedInt getActionDelayForDock(Object *dock);			// 19, 0x004A6F0A

	const SupplyTruckAIUpdateModuleData *getSupplyTruckAIUpdateModuleData() const { return static_cast<const SupplyTruckAIUpdateModuleData *>(getModuleData()); }

protected:
	virtual void loadPostProcess() { AIUpdateInterface::loadPostProcess(); }	// slot 1: retail 0x0049A7EE, the fold of the AI updates' `jmp AIUpdateInterface::loadPostProcess` (h05 r9)
	virtual const char *GetSnapshotName() const;
	virtual AIStateMachine *makeStateMachine();							// 0x004A6E33
	virtual void xfer(Xfer *xfer);										// 0x004A6B3A
	virtual void privateIdle(CommandSourceType);						// 0x004A6AB5
	virtual void privateDock(Object *, CommandSourceType);				// 0x004A6ADB
	virtual void privateHarvest(const Coord3D *, CommandSourceType);	// 0x004A6B06

public:
	SupplyTruckStateMachine *m_supplyTruckStateMachine;					// +0x3E8; xfer'd as a snapshot
	ObjectID m_preferredDock;											// +0x3EC; xfer ObjectID (slot 15; Zero Hour's name)
	Coord3D m_unk0x3F0;													// +0x3F0; xfer Coord3D
	Bool m_unk0x3FC;													// +0x3FC; xfer Bool
	Int m_numberBoxes;													// +0x400; xfer Int (slots 0, 2, 3; Zero Hour's name)
	Bool m_forcePending;												// +0x404; xfer Bool (slots 11/12; Zero Hour's name)
	Bool m_forcedBusyPending;											// +0x405; xfer Bool (slots 13/14; Zero Hour's name)
};
// SIZE 0x408
typedef char SupplyTruckAIUpdate_size[(sizeof(SupplyTruckAIUpdate) == 0x408) ? 1 : -1];

#endif
