// CrushDie.h: the CrushDie module. SIZE 0x14 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004862E1 calls DieModule first; it stores the
// tables 0x00C4AC34 at +0x0, 0x00C4A650 at +0xC, 0x00C4AC30 at +0x10. Members are what xfer (0x0045CE8C) transfers after DieModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x0048614F ?name@Rva0048614FNamed@@QBEPBDXZ
//   +0x10 slot 0 -> 0x00486327 ?onDie@CrushDie@@UAEXPBVDamageInfo@@@Z
//   +0x10 slot 6 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 10 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 11 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
#pragma once
#ifndef BFME2_CRUSH_DIE_H
#define BFME2_CRUSH_DIE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DieModule.h"

// Which end of the victim the crusher rolled over (the order is the crush sounds' order in the module data: total at +0x38, back at +0x3C, front at +0x40).
enum CrushEnum
{
	TOTAL_CRUSH,
	BACK_END_CRUSH,
	FRONT_END_CRUSH,
	NO_CRUSH,
	CRUSH_COUNT
};

class Thing;
class ModuleData;
class CrushDieModuleData;
class Xfer;

// DieModule ends at 0x14; this class's members follow it.
typedef char CrushDie_base_size[(sizeof(DieModule) == 0x14) ? 1 : -1];

class CrushDie : public DieModule
{
public:
	CrushDie(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual ~CrushDie();
	virtual void onDie(const DamageInfo *);	// override: onDie slot 0 at +0x10 (0x00486327)

protected:
	virtual void xfer(Xfer *xfer);

public:
	const CrushDieModuleData *getCrushDieModuleData() const { return (const CrushDieModuleData *)getModuleData(); }
protected:
	virtual void loadPostProcess() { DieModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to DieModule's, ICF-folded across classes (h05)
};
// SIZE 0x14
typedef char CrushDie_size[(sizeof(CrushDie) == 0x14) ? 1 : -1];

#endif
