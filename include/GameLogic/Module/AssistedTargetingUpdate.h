// AssistedTargetingUpdate.h: the AssistedTargetingUpdate module. SIZE 0x20 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00486E8D calls UpdateModule first; it stores the
// tables 0x00C4AF64 at +0x0, 0x00BEFF90 at +0xC, 0x00C4B1E0 at +0x10. Members are what xfer (0x004DF81B) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x10 slot 0 -> 0x004A6563 ?update@AssistedTargetingUpdate@@UAE?AW4UpdateSleepTime@@XZ
#pragma once
#ifndef BFME2_ASSISTED_TARGETING_UPDATE_H
#define BFME2_ASSISTED_TARGETING_UPDATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;
class Object;
class ThingTemplate;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char AssistedTargetingUpdate_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class AssistedTargetingUpdate : public UpdateModule
{
public:
	AssistedTargetingUpdate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual UpdateSleepTime update();								// +0x10 slot 0, 0x004A6563
	Bool isFreeToAssist() const;								// 0x00486EFD
	void assistAttack(const Object *victim, Object *target);			// 0x00487010

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

private:
	void makeFeedbackLaser(const ThingTemplate *laser, const Object *from, const Object *to);	// 0x00486F35 (AAE)
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to UpdateModule's, ICF-folded across classes (h05)
};
// SIZE 0x20
typedef char AssistedTargetingUpdate_size[(sizeof(AssistedTargetingUpdate) == 0x20) ? 1 : -1];

#endif
