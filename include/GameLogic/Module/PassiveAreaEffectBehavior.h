// PassiveAreaEffectBehavior.h: the PassiveAreaEffectBehavior module. SIZE 0x28 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00484A67 calls UpdateModule first; it stores the
// tables 0x00C4A448 at +0x0, 0x00BEFF90 at +0xC, 0x00C4A438 at +0x10. Members are what xfer (0x00484C58) transfers after UpdateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x00484AD7 ?Rva00484AD7Get@@YAHXZ
//   +0x0 slot 13 -> 0x00484C8B ?rva00484C8B@PassiveAreaEffectBehavior@@UAEXXZ
//   +0x0 slot 14 -> 0x004848E6 ?rva004848E6@PassiveAreaEffectBehavior@@UAEXPAVObject@@@Z
//   +0x0 slot 15 -> 0x0048491F ?rva0048491F@PassiveAreaEffectBehavior@@UAEXPAVObject@@@Z
//   +0x0 slot 16 -> 0x00484A08 ?rva00484A08@PassiveAreaEffectBehavior@@UAEXPAVObject@@@Z
//   +0x10 slot 0 -> 0x00484B6B ?update@PassiveAreaEffectBehavior@@UAE?AW4UpdateSleepTime@@XZ
#pragma once
#ifndef BFME2_PASSIVE_AREA_EFFECT_BEHAVIOR_H
#define BFME2_PASSIVE_AREA_EFFECT_BEHAVIOR_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Thing;
class ModuleData;
class Xfer;

// UpdateModule ends at 0x20; this class's members follow it.
typedef char PassiveAreaEffectBehavior_base_size[(sizeof(UpdateModule) == 0x20) ? 1 : -1];

class PassiveAreaEffectBehavior : public UpdateModule
{
public:
	PassiveAreaEffectBehavior(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~PassiveAreaEffectBehavior();
	virtual UpdateSleepTime update();	// override: update slot 0 at +0x10 (0x00484B6B)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedInt m_unk0x20;                         // +0x20; xfer UnsignedInt
	UnsignedByte m_gap0x24[0x4];
protected:
	virtual void loadPostProcess() { UpdateModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to UpdateModule's (h05)
};
// SIZE 0x28
typedef char PassiveAreaEffectBehavior_size[(sizeof(PassiveAreaEffectBehavior) == 0x28) ? 1 : -1];
typedef char PassiveAreaEffectBehavior_off_m_unk0x20[(offsetof(PassiveAreaEffectBehavior, m_unk0x20) == 0x20) ? 1 : -1];

#endif
