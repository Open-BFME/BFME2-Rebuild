// DestroyDie.h: the DestroyDie module. SIZE 0x14 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x0048650E calls DieModule first; it stores the
// tables 0x00C4ACC8 at +0x0, 0x00C4A650 at +0xC, 0x00C4ACC4 at +0x10. Members are what xfer (0x0045CE8C) transfers after DieModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004864C3 ?name@Rva004864C3Named@@QBEPBDXZ
//   +0x10 slot 0 -> 0x00486554 ?onDie@DestroyDie@@UAEXPBVDamageInfo@@@Z
//   +0x10 slot 6 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 10 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 11 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
#pragma once
#ifndef BFME2_DESTROY_DIE_H
#define BFME2_DESTROY_DIE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DieModule.h"

class Thing;
class ModuleData;
class Xfer;
class DamageInfo;

// DieModule ends at 0x14; this class's members follow it.
typedef char DestroyDie_base_size[(sizeof(DieModule) == 0x14) ? 1 : -1];

class DestroyDie : public DieModule
{
public:
	DestroyDie(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual void onDie(const DamageInfo *damageInfo);

protected:
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { DieModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to DieModule's, ICF-folded across classes (h05)
};
// SIZE 0x14
typedef char DestroyDie_size[(sizeof(DestroyDie) == 0x14) ? 1 : -1];

#endif
