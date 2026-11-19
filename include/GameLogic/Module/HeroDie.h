// HeroDie.h: the HeroDie module. SIZE 0x14 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004C2324 calls DieModule first; it stores the
// tables 0x00C5C428 at +0x0, 0x00C4A650 at +0xC, 0x00C5C424 at +0x10. Members are what xfer (0x0045CE8C) transfers after DieModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004C2293 ?name@Rva004C2293Named@@QBEPBDXZ
//   +0x10 slot 0 -> 0x004C236A ?onDie@HeroDie@@UAEXPBVDamageInfo@@@Z
//   +0x10 slot 6 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 10 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 11 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
#pragma once
#ifndef BFME2_HERO_DIE_H
#define BFME2_HERO_DIE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DieModule.h"

class Thing;
class Object;
class ModuleData;
class Xfer;

// DieModule ends at 0x14; this class's members follow it.
typedef char HeroDie_base_size[(sizeof(DieModule) == 0x14) ? 1 : -1];

class HeroDie : public DieModule
{
public:
	HeroDie(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual ~HeroDie();
	virtual void onDie(const DamageInfo *);	// override: onDie slot 0 at +0x10 (0x004C236A)

private:
	static Int rva004C2299(Object *obj, void *userData);	// 0x004C2299: Player::iterateObjects callback (private static in the map)

public:

protected:
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { DieModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to DieModule's, ICF-folded across classes (h05)
};
// SIZE 0x14
typedef char HeroDie_size[(sizeof(HeroDie) == 0x14) ? 1 : -1];

#endif
