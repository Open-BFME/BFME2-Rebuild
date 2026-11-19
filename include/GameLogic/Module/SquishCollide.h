// SquishCollide.h: the SquishCollide module. SIZE 0x14 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004BB9CF calls CollideModule first; it stores the
// tables 0x00C5A204 at +0x0, 0x00C5A040 at +0xC, 0x00C5A1EC at +0x10. Members are what xfer (0x0045CE8C) transfers after CollideModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x10 slot 0 -> 0x004BBA65 ?onCollide@SquishCollide@@UAEXPAVObject@@PBUCoord3D@@1@Z
//   +0x10 slot 11 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 15 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x10 slot 16 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
#pragma once
#ifndef BFME2_SQUISH_COLLIDE_H
#define BFME2_SQUISH_COLLIDE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/CollideModule.h"

class Thing;
class ModuleData;
class Xfer;

// CollideModule ends at 0x14; this class's members follow it.
typedef char SquishCollide_base_size[(sizeof(CollideModule) == 0x14) ? 1 : -1];

class SquishCollide : public CollideModule
{
public:
	SquishCollide(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~SquishCollide();
	virtual void onCollide(Object *other, const Coord3D *loc, const Coord3D *normal);	// override: onCollide slot 0 at +0x10 (0x004BBA65)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { CollideModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to CollideModule's, ICF-folded across classes (h05)
};
// SIZE 0x14
typedef char SquishCollide_size[(sizeof(SquishCollide) == 0x14) ? 1 : -1];

#endif
