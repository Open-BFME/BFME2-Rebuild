// HordeMemberCollide.h: the HordeMemberCollide module. SIZE 0x14 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004BC389 calls CollideModule first; it stores the
// tables 0x00C5A5D0 at +0x0, 0x00C5A040 at +0xC, 0x00C5A5B8 at +0x10. Members are what xfer (0x0045CE8C) transfers after CollideModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004BC33E ?name@Rva004BC33ENamed@@QBEPBDXZ
//   +0x0 slot 12 -> 0x004BC56D ?onCollide@CrateCollide@@UAEXPAVObject@@PBUCoord3D@@1@Z
//   +0x0 slot 13 -> 0x004BC51B ?forward@Rva004BC51B@@UAEXXZ
//   +0x0 slot 14 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 20 -> 0x004BC515 ?name@Rva004BC515Named@@QBEPBDXZ
//   +0x0 slot 22 -> 0x0003B810 __purecall
//   +0x0 slot 23 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 27 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x0 slot 28 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 31 -> 0x004BC6C2 ?isValidToExecute@CrateCollide@@MBE_NPBVObject@@@Z
//   +0x10 slot 0 -> 0x004BC3CF ?onCollide@HordeMemberCollide@@UAEXPAVObject@@PBVCoord3D@@1@Z
#pragma once
#ifndef BFME2_HORDE_MEMBER_COLLIDE_H
#define BFME2_HORDE_MEMBER_COLLIDE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/CollideModule.h"

class Thing;
class ModuleData;
class Xfer;

// CollideModule ends at 0x14; this class's members follow it.
typedef char HordeMemberCollide_base_size[(sizeof(CollideModule) == 0x14) ? 1 : -1];

class HordeMemberCollide : public CollideModule
{
public:
	HordeMemberCollide(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual void onCollide(Object *other, const Coord3D *loc, const Coord3D *normal);	// override: onCollide slot 0 at +0x10 (0x004BC3CF)

protected:
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { CollideModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to CollideModule's, ICF-folded across classes (h05)
};
// SIZE 0x14
typedef char HordeMemberCollide_size[(sizeof(HordeMemberCollide) == 0x14) ? 1 : -1];

#endif
