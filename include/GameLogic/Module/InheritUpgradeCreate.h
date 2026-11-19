// InheritUpgradeCreate.h: the InheritUpgradeCreate module. SIZE 0x18 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B9428 calls CreateModule first; it stores the
// tables 0x00C596F4 at +0x0, 0x00BEF9C0 at +0xC, 0x00C596E4 at +0x10. Members are what xfer (0x004B94E3) transfers after CreateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004B9452 ?Rva004B9452Get@@YAHXZ
//   +0x10 slot 0 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x10 slot 1 -> 0x004B96A9 ?onBuildComplete@InheritUpgradeCreate@@UAEXXZ
//   +0x10 slot 2 -> 0x004B96C4 ?slot02@InheritUpgradeCreate@@UAEXXZ
#pragma once
#ifndef BFME2_INHERIT_UPGRADE_CREATE_H
#define BFME2_INHERIT_UPGRADE_CREATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/CreateModule.h"

class Thing;
class ModuleData;
class Xfer;

// CreateModule ends at 0x18; this class's members follow it.
typedef char InheritUpgradeCreate_base_size[(sizeof(CreateModule) == 0x18) ? 1 : -1];

class InheritUpgradeCreate : public CreateModule
{
public:
	InheritUpgradeCreate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual ~InheritUpgradeCreate();
	virtual void onBuildComplete();	// override: onBuildComplete slot 1 at +0x10 (0x004B96A9)
	virtual void vslot02() { rva004B959C(); }	// CreateModuleInterface slot 2: 0x004B96C4 (`add ecx,-0x10; jmp 0x004B959C`; the map spells it ?slot02@InheritUpgradeCreate@@UAEXXZ)
	void rva004B959C();	// 0x004B959C: the inheriting work onBuildComplete 0x004B96A9 also calls

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
typedef char InheritUpgradeCreate_size[(sizeof(InheritUpgradeCreate) == 0x18) ? 1 : -1];

#endif
