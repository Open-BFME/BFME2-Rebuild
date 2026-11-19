// DRAFT (h05 modgen): generated from constructor RVA 0x0049B560 and xfer RVA 0x0049B66B; no function
// matches against it yet, so it stays in Draft until one does.
// AttributeModifierAuraUpdate.h: the AttributeModifierAuraUpdate module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0049B560 calls UpdateModule first; it stores the
// tables 0x00C50D2C at +0x0, 0x00C49F78 at +0xC, 0x00C50D20 at +0x10. Members are what xfer (0x0049B66B) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x0049B5D6 ?Rva0049B5D6Get@@YAHXZ
//   +0xC slot 10 -> 0x004543A9 ?getUpdateExitInterface@QueueProductionExitUpdate@@UAEPAVExitInterface@@XZ
//   +0xC slot 48 -> 0x004B7BAA ?loadPostProcess@AudioLoopUpgrade@@MAEXXZ
//   +0xC slot 49 -> 0x00484181 ?name@Rva00484181Named@@QBEPBDXZ
//   +0x10 slot 0 -> 0x0049BCA3 ?update@AttributeModifierAuraUpdate@@UAE?AW4UpdateSleepTime@@XZ
// Not established: transfers xfer makes that this view cannot place: slot10 ?.
// Round 7 (h05, by hand): the constructor also builds an UpgradeMux at +0x20 (`lea edi,[esi+0x20]`,
// ??0UpgradeMux, then its table 0x00C50CD8 stored through edi), so the class is UpdateModule +
// UpgradeMux; xfer transfers the mux through UpgradeMux::upgradeMuxXfer behind !IsLightCRC.
#pragma once
#ifndef BFME2_ATTRIBUTE_MODIFIER_AURA_UPDATE_H
#define BFME2_ATTRIBUTE_MODIFIER_AURA_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/Module/UpgradeModule.h"
#include "Draft/AttributeModifierAuraUpdateModuleData.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char AttributeModifierAuraUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class AttributeModifierAuraUpdate : public UpdateModule, public UpgradeMux
{
public:
	AttributeModifierAuraUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x0049BCA3)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

	// UpgradeMux (table 0x00C50CD8 at +0x20)
	virtual void upgradeRemovalImplementation();						// slot 8
	virtual void upgradeImplementation();								// slot 10
	virtual void getUpgradeActivationMasks(UpgradeMaskType &activation, UpgradeMaskType &conflicting) const;	// slot 11, 0x0049B621
	virtual void performUpgradeFX();									// slot 12, 0x0049B648
	virtual Bool requiresAllActivationUpgrades() const;					// slot 13
	virtual Bool requiresAllConflictingUpgrades() const;				// slot 14
	virtual void processUpgradeRemoval() { ((const AttributeModifierAuraUpdateModuleData *)getModuleData())->m_upgradeMuxData.muxDataProcessUpgradeRemoval(getObject()); }	// slot 15: retail 0x0049B648, ICF-folded with performUpgradeFX (both call the mux data's `ret 4` 0x0047A69C)
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x28
typedef char AttributeModifierAuraUpdate_size[(sizeof(AttributeModifierAuraUpdate) == 0x28) ? 1 : -1];

#endif
