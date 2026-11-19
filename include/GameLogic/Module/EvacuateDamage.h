// EvacuateDamage.h: the EvacuateDamage module. SIZE 0x18 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004BAE1D calls DamageModule first; it stores the
// tables 0x00C59E2C at +0x0, 0x00C597F8 at +0xC, 0x00C59E20 at +0x10. Members are what xfer (0x004BAC83) transfers after DamageModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004BAE7D ?Rva004BAE7DGet@@YAHXZ
//   +0x10 slot 0 -> 0x004BAFE5 ?rva004BAFE5@Rva004BAFE5@@QAEXPAUDamageInfo@@@Z
//   +0x10 slot 1 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x10 slot 2 -> 0x000D1407 ?Set_Animation@RenderObjClass@@UAEXPAVHAnimClass@@MH@Z
//   +0x10 slot 8 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 12 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 13 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
// Not established: transfers xfer makes that this view cannot place: ?xfer@DamageModule@@MAEXPAVXfer@@@Z ?.
#pragma once
#ifndef BFME2_EVACUATE_DAMAGE_H
#define BFME2_EVACUATE_DAMAGE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DamageModule.h"

class Thing;
class ModuleData;
class Xfer;

// DamageModule ends at 0x14; this class's members follow it.
typedef char EvacuateDamage_base_size[(sizeof(DamageModule) == 0x14) ? 1 : -1];

class EvacuateDamage : public DamageModule
{
public:
	EvacuateDamage(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
	UnsignedByte m_gap0x14[0x4];
protected:
	virtual void loadPostProcess() { DamageModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to DamageModule's (h05)
};
// SIZE 0x18
typedef char EvacuateDamage_size[(sizeof(EvacuateDamage) == 0x18) ? 1 : -1];

#endif
