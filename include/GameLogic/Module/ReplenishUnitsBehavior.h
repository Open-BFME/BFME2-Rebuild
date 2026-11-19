// ReplenishUnitsBehavior.h: the ReplenishUnitsBehavior module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00484227 calls UpdateModule first; it stores the
// tables 0x00C4A034 at +0x0, 0x00C49F78 at +0xC, 0x00C49F68 at +0x10. Members are what xfer (0x00484365) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x004B7BAA ?loadPostProcess@AudioLoopUpgrade@@MAEXXZ
//   +0x0 slot 2 -> 0x00484181 ?name@Rva00484181Named@@QBEPBDXZ
//   +0xC slot 10 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0x10 slot 0 -> 0x0048438A ?update@ReplenishUnitsBehavior@@UAE?AW4UpdateSleepTime@@XZ
//   +0x10 slot 1 -> 0x004A2D67 ?getDisabledTypesToProcess@AIUpdateInterface@@UBE?AV?$BitFlags@$0L@@@XZ
#pragma once
#ifndef BFME2_REPLENISH_UNITS_BEHAVIOR_H
#define BFME2_REPLENISH_UNITS_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "Common/BitFlags11.h"
#include "GameLogic/Module/UpgradeModule.h"

class Thing;
class ModuleData;
class Xfer;
class Object;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char ReplenishUnitsBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class ReplenishUnitsBehavior : public UpdateModule, public UpgradeMux
{
public:
	virtual UpgradeModuleInterface *getUpgrade() { return this; }
	ReplenishUnitsBehavior(Thing *thing, const ModuleData *moduleData);
	virtual void upgradeRemovalImplementation();
	virtual void upgradeImplementation();
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();	// 0x0048438A (UpdateModuleInterface slot 0)
	virtual DisabledMaskType getDisabledTypesToProcess() const { return DisabledMaskType(DisabledMaskType::kInit, 3); }	// UpdateModuleInterface slot 1 (table 0x00849F68): the shared DISABLED_HELD body 0x004A2D67 (h05 r13)
	virtual const char *GetSnapshotName() const;
	virtual ~ReplenishUnitsBehavior();
	void rva004842DD(Object *replenished);	// 0x004842DD: replenish one object (FX, rider reset)

protected:
	virtual void getUpgradeActivationMasks(UpgradeMaskType &activation, UpgradeMaskType &conflicting) const;
	virtual void performUpgradeFX();
	virtual void processUpgradeRemoval();
	virtual Bool requiresAllActivationUpgrades() const;
	virtual Bool requiresAllConflictingUpgrades() const;
	virtual void xfer(Xfer *xfer);

public:
	// +0x20..+0x27 is the UpgradeMux base (xfer calls its upgradeMuxXfer 0x004CE397 on this+0x20).
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); upgradeMuxLoadPostProcess(); }	// retail slot 0x004B7BAA, the body AudioLoopUpgrade's shares (h05, gen3 vtable-content list)
};
// SIZE 0x28
typedef char ReplenishUnitsBehavior_size[(sizeof(ReplenishUnitsBehavior) == 0x28) ? 1 : -1];

#endif
