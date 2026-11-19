// RefundDie.h: the RefundDie module. SIZE 0x14 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00485262 calls DieModule first; it stores the
// tables 0x00C4A70C at +0x0, 0x00C4A650 at +0xC, 0x00C4A64C at +0x10. Members are what xfer (0x0045CE8C) transfers after DieModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x00485217 ?name@Rva00485217Named@@QBEPBDXZ
//   +0x10 slot 0 -> 0x004852A8 ?onDie@RefundDie@@UAEXPBVDamageInfo@@@Z
//   +0x10 slot 1 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x10 slot 8 -> 0x002533D3 ?getDie@DieModule@@UAEPAVDieModuleInterface@@XZ
//   +0x10 slot 44 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x10 slot 45 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 57 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
#pragma once
#ifndef BFME2_REFUND_DIE_H
#define BFME2_REFUND_DIE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DieModule.h"

class Thing;
class ModuleData;
class Xfer;

// DieModule ends at 0x14; this class's members follow it.
typedef char RefundDie_base_size[(sizeof(DieModule) == 0x14) ? 1 : -1];

class RefundDie : public DieModule
{
public:
	RefundDie(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual ~RefundDie();
	virtual void onDie(const DamageInfo *);	// override: onDie slot 0 at +0x10 (0x004852A8)

protected:
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { DieModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to DieModule's, ICF-folded across classes (h05)
};
// SIZE 0x14
typedef char RefundDie_size[(sizeof(RefundDie) == 0x14) ? 1 : -1];

#endif
