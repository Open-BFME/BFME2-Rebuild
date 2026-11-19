// CallHelpOnDamage.h: the CallHelpOnDamage module. SIZE 0x18 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004BB43C calls DamageModule first; it stores the
// tables 0x00C59FDC at +0x0, 0x00C597F8 at +0xC, 0x00C59FD0 at +0x10. Members are what xfer (0x004BB4D6) transfers after DamageModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004BB46A ?Rva004BB46AGet@@YAHXZ
//   +0x0 slot 12 -> 0x0003B810 __purecall
//   +0x0 slot 13 -> 0x005CB9FF ?vslot24_01@AIUpdateInterface@@UAE_NH@Z
//   +0x0 slot 14 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 18 -> 0x004BB822 ?onCollide@FireWeaponCollide@@UAEXPAVObject@@PBUCoord3D@@1@Z
//   +0x10 slot 0 -> 0x004BB547 ?onDamage@CallHelpOnDamage@@UAEXPAUDamageInfo@@@Z
//   +0x10 slot 1 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x10 slot 2 -> 0x000D1407 ?Set_Animation@RenderObjClass@@UAEXPAVHAnimClass@@MH@Z
//   +0x10 slot 8 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 12 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 13 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
#pragma once
#ifndef BFME2_CALL_HELP_ON_DAMAGE_H
#define BFME2_CALL_HELP_ON_DAMAGE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DamageModule.h"

class Thing;
class ModuleData;
class Xfer;

// DamageModule ends at 0x14; this class's members follow it.
typedef char CallHelpOnDamage_base_size[(sizeof(DamageModule) == 0x14) ? 1 : -1];

class CallHelpOnDamage : public DamageModule
{
public:
	CallHelpOnDamage(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void onDamage(DamageInfo *damageInfo);	// override: onDamage slot 0 at +0x10 (0x004BB547)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedInt m_unk0x14;                         // +0x14; xfer UnsignedInt
protected:
	virtual void loadPostProcess() { DamageModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to DamageModule's (h05)
};
// SIZE 0x18
typedef char CallHelpOnDamage_size[(sizeof(CallHelpOnDamage) == 0x18) ? 1 : -1];
typedef char CallHelpOnDamage_off_m_unk0x14[(offsetof(CallHelpOnDamage, m_unk0x14) == 0x14) ? 1 : -1];

#endif
