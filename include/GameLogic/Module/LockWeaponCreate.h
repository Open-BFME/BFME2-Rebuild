// LockWeaponCreate.h: the LockWeaponCreate module. SIZE 0x18 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B8C0E calls CreateModule first; it stores the
// tables 0x00C592FC at +0x0, 0x00BEF9C0 at +0xC, 0x00C592EC at +0x10. Members are what xfer (0x004851E3) transfers after CreateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 4 -> 0x004B8C57 ?rva004B8C57@LockWeaponCreate@@SA?AW4NameKeyType@@XZ
//   +0x10 slot 0 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 1 -> 0x004B8CC9 ?onBuildComplete@LockWeaponCreate@@UAEXXZ
#pragma once
#ifndef BFME2_LOCK_WEAPON_CREATE_H
#define BFME2_LOCK_WEAPON_CREATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/CreateModule.h"

class Thing;
class ModuleData;
class Xfer;

// CreateModule ends at 0x18; this class's members follow it.
typedef char LockWeaponCreate_base_size[(sizeof(CreateModule) == 0x18) ? 1 : -1];

class LockWeaponCreate : public CreateModule
{
public:
	LockWeaponCreate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~LockWeaponCreate();
	virtual void onBuildComplete();	// override: onBuildComplete slot 1 at +0x10 (0x004B8CC9)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
public:
	virtual void onCreate() {}	// CreateModuleInterface slot 0: retail's is the shared empty body 0x000B3FD0; retail's table has this class's own (h05, gen3 vtable-content list)
protected:
	virtual void loadPostProcess() { CreateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to CreateModule's, ICF-folded across classes (h05)
};
// SIZE 0x18
typedef char LockWeaponCreate_size[(sizeof(LockWeaponCreate) == 0x18) ? 1 : -1];

#endif
