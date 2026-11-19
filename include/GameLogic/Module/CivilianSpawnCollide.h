// CivilianSpawnCollide.h: the CivilianSpawnCollide module. SIZE 0x14 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004BD694 calls CollideModule first; it stores the
// tables 0x00C5AA88 at +0x0, 0x00C5A040 at +0xC, 0x00C5AA70 at +0x10. Members are what xfer (0x0045CE8C) transfers after CollideModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004BD606 ?name@Rva004BD606Named@@QBEPBDXZ
//   +0x0 slot 13 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 15 -> 0x0047A69C ?DoXfer@EmissionVelocityInfo@FXParticleSystem@@UAEXAAVXfer@@@Z
//   +0x0 slot 16 -> 0x0047A699 ?IsCRC@Xfer@@UBE_NXZ
//   +0x0 slot 26 -> 0x000B69A1 ?slot14@ModuleData@@UBE?AVAsciiString@@H@Z
//   +0x0 slot 28 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 42 -> 0x0050B5C6 ?vslot10@GameWindow@@UAE_NXZ
//   +0x10 slot 0 -> 0x004BD6DA ?bfmeGo1026F@BfmeF1026@@QAEXHHH@Z
//   +0x10 slot 15 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
#pragma once
#ifndef BFME2_CIVILIAN_SPAWN_COLLIDE_H
#define BFME2_CIVILIAN_SPAWN_COLLIDE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/CollideModule.h"

class Thing;
class ModuleData;
class Xfer;

// CollideModule ends at 0x14; this class's members follow it.
typedef char CivilianSpawnCollide_base_size[(sizeof(CollideModule) == 0x14) ? 1 : -1];

class CivilianSpawnCollide : public CollideModule
{
public:
	CivilianSpawnCollide(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;

protected:
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { CollideModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to CollideModule's (h05)
};
// SIZE 0x14
typedef char CivilianSpawnCollide_size[(sizeof(CivilianSpawnCollide) == 0x14) ? 1 : -1];

#endif
