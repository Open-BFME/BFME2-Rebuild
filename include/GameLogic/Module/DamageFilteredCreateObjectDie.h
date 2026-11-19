// bases DieModule and DamageModuleInterface (+0x14, its pure table stored first by the constructor 0x00485F87), size 0x20, no new primary slots; the constructor, name, getModuleNameKey and the inline getDamage match against it.
// DamageFilteredCreateObjectDie.h: the DamageFilteredCreateObjectDie module. SIZE 0x20 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x00485F87 calls DieModule first; it stores the
// tables 0x00BE2B78 at +0x14, 0x00C4AB54 at +0x0, 0x00C4AA98 at +0xC, 0x00C4AA90 at +0x10, 0x00C4AA84 at +0x14. Members are what xfer (0x00485FE3) transfers after DieModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x14 slot 0 -> 0x0003B810 __purecall
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x00485EE3 ?name@DamageFilteredCreateObjectDie@@QBEPBDXZ
//   +0xC slot 4 -> 0x00485EE9 ?getDamage@DamageFilteredCreateObjectDie@@UAEPAVDamageModuleInterface@@XZ
//   +0x10 slot 0 -> 0x00486015 ?onDie@DamageFilteredCreateObjectDie@@UAEXPBVDamageInfo@@@Z
//   +0x14 slot 0 -> 0x004860E9 ?onDamage@DamageFilteredCreateObjectDie@@UAEXPAVDamageInfo@@@Z
//   +0x14 slot 1 -> 0x00485EF6 ?onHealing@DamageFilteredCreateObjectDie@@UAEXPAX@Z
//   +0x14 slot 2 -> 0x000D1407 ?Set_Animation@RenderObjClass@@UAEXPAVHAnimClass@@MH@Z
// Not established: interface tables at +0x14 (their vptrs sit in the gaps there).
#pragma once
#ifndef BFME2_DAMAGE_FILTERED_CREATE_OBJECT_DIE_H
#define BFME2_DAMAGE_FILTERED_CREATE_OBJECT_DIE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/DieModule.h"
#include "GameLogic/Module/DamageModule.h"

class Thing;
class ModuleData;
class Xfer;

// DieModule ends at 0x14; this class's members follow it.
typedef char DamageFilteredCreateObjectDie_base_size[(sizeof(DieModule) == 0x14) ? 1 : -1];

class DamageFilteredCreateObjectDie : public DieModule, public DamageModuleInterface
{
public:
	char const * name() const;	// 0x00485EE3
	DamageFilteredCreateObjectDie(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual const char *GetSnapshotName() const;
	virtual DamageModuleInterface *getDamage() { return this; }	// override: getDamage slot 4 at +0xC (0x00485EE9, emitted inline)
	virtual void onDie(const DamageInfo *damageInfo);	// override: onDie slot 0 at +0x10 (0x00486015)

	// DamageModuleInterface at +0x14 (the constructor 0x00485F87 stores its pure table 0x00BE2B78 there
	// first; the class's own table 0x00C4AA84 holds onDamage, onHealing and the shared three-argument
	// empty body 0x000D1407)
	virtual void onDamage(DamageInfo *damageInfo);	// slot 0 (0x004860E9)
	virtual void onHealing(DamageInfo *damageInfo);	// slot 1 (0x00485EF6: clears +0x18; the map spells its argument void *)
	virtual void onBodyDamageStateChange(const DamageInfo *damageInfo, BodyDamageType oldState, BodyDamageType newState) {}	// slot 2

protected:
	virtual void xfer(Xfer *xfer);

public:
	UnsignedInt m_unk0x18;                         // +0x18; xfer UnsignedInt; the constructor zeroes it
	Int m_unk0x1C;                                 // +0x1C; xfer Int; the constructor sets -1
protected:
	virtual void loadPostProcess() { DieModule::loadPostProcess(); }	// retail slot 1 0x00497805: a jmp to DieModule's (h05)
};
// SIZE 0x20
typedef char DamageFilteredCreateObjectDie_size[(sizeof(DamageFilteredCreateObjectDie) == 0x20) ? 1 : -1];
typedef char DamageFilteredCreateObjectDie_off_m_unk0x18[(offsetof(DamageFilteredCreateObjectDie, m_unk0x18) == 0x18) ? 1 : -1];
typedef char DamageFilteredCreateObjectDie_off_m_unk0x1C[(offsetof(DamageFilteredCreateObjectDie, m_unk0x1C) == 0x1C) ? 1 : -1];

#endif
