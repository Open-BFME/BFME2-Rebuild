// Slots 13-15 of the primary table 0x00C49AC8 are this class's own (0x00483BA3, 0x00483BBB, 0x00483BCD: f08 read the table, which ends at the
// "SelfHealSupression" literal at slot 16; the h05 note below that the bounded table's slots 13-15 are ModuleData's was wrong). They are Zero Hour's
// protected virtuals resetSelfHealSupression, startCrippledEffects and stopCrippledEffects, in that order.
// bases UpdateModule and DamageModuleInterface (+0x20, identified by its three slots' shape; the constructor 0x00483A8B stores the pure table there first), size 0x2C; the bounded table's slots 13-15 are the next table's (ModuleData's), so it adds no slot of its own. GetSnapshotName, getModuleNameKey and xfer match against it.
// SupplyWarehouseCripplingBehavior.h: the SupplyWarehouseCripplingBehavior module. SIZE 0x2C (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00483A8B calls UpdateModule first; it stores the
// tables 0x00BE2B78 at +0x20, 0x00C49AC8 at +0x0, 0x00C49800 at +0xC, 0x00C49AB8 at +0x10, 0x00C49AAC at +0x20. Members are what xfer (0x00483BDF) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x20 slot 0 -> 0x0003B810 __purecall
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004839F3 ?name@Rva004839F3Named@@QBEPBDXZ
//   +0x0 slot 13 -> 0x00483BA3 ?rva00483BA3@Rva00483BA3@@QAEXXZ
//   +0x0 slot 14 -> 0x00483BBB ?bfmeGoBEC@BfmeThingBEC@@QAEXXZ
//   +0x0 slot 15 -> 0x00483BCD ?bfmeGo940D@BfmeThing940D@@QAEXXZ
//   +0xC slot 4 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0x10 slot 0 -> 0x00483B3D ?update@Rva00483B3D@@QAEHXZ
//   +0x20 slot 0 -> 0x00483B10 ?rva00483B10@Rva00483B10@@UAEXH@Z
//   +0x20 slot 1 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x20 slot 2 -> 0x004839F9 ?rva004839F9@Rva004839F9@@UAEXHHH@Z
//   +0x20 slot 4 -> 0x00253376 ?getDisabledTypesToProcess@UpdateModule@@UBE?AV?$BitFlags@$0L@@@XZ
//   +0x20 slot 5 -> 0x0044DF8D ?rva0044DF8D@UpdateModule@@UAEXW4UpdateSleepTime@@@Z
// Not established: interface tables at +0x20 (their vptrs sit in the gaps there).
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
#pragma once
#ifndef BFME2_SUPPLY_WAREHOUSE_CRIPPLING_BEHAVIOR_H
#define BFME2_SUPPLY_WAREHOUSE_CRIPPLING_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/DamageModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char SupplyWarehouseCripplingBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class SupplyWarehouseCripplingBehavior : public UpdateModule, public DamageModuleInterface
{
public:
	SupplyWarehouseCripplingBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual DamageModuleInterface *getDamage() { return this; }	// +0xC slot 4: the shared getter 0x004543A9
	virtual UpdateSleepTime update();	// +0x10 slot 0 (0x00483B3D; the map: Rva00483B3D::update)

	// DamageModuleInterface at +0x20: the constructor 0x00483A8B stores its pure table 0x00BE2B78 there
	// first, then the class's 0x00C49AAC (a one-argument body, the shared one-argument empty `ret 4`, and a
	// three-argument body that tests the new state against 2: the onDamage/onHealing/
	// onBodyDamageStateChange shape of DamageModuleInterface)
	virtual void onDamage(DamageInfo *damageInfo);	// slot 0 (0x00483B10)
	virtual void onHealing(DamageInfo *damageInfo) {}	// slot 1 (the shared empty 0x0047A69C)
	virtual void onBodyDamageStateChange(const DamageInfo *damageInfo, BodyDamageType oldState, BodyDamageType newState);	// slot 2 (0x004839F9)

protected:
	virtual void xfer(Xfer *xfer);
	virtual void resetSelfHealSupression();		// slot 13, 0x00483BA3: the healing waits SelfHealSupression frames after a hit
	virtual void startCrippledEffects();		// slot 14, 0x00483BBB: cripples the object's dock
	virtual void stopCrippledEffects();			// slot 15, 0x00483BCD: restores the dock

public:
	UnsignedInt m_healingSupressedUntilFrame;	// +0x24; xfer UnsignedInt; the constructor zeroes it
	UnsignedInt m_nextHealingFrame;				// +0x28; xfer UnsignedInt; the constructor zeroes it
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x2C
typedef char SupplyWarehouseCripplingBehavior_size[(sizeof(SupplyWarehouseCripplingBehavior) == 0x2C) ? 1 : -1];
typedef char SupplyWarehouseCripplingBehavior_off_m_healingSupressedUntilFrame[(offsetof(SupplyWarehouseCripplingBehavior, m_healingSupressedUntilFrame) == 0x24) ? 1 : -1];
typedef char SupplyWarehouseCripplingBehavior_off_m_nextHealingFrame[(offsetof(SupplyWarehouseCripplingBehavior, m_nextHealingFrame) == 0x28) ? 1 : -1];

#endif
