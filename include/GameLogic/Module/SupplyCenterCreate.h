// SupplyCenterCreate.h: the SupplyCenterCreate module. SIZE 0x18 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B8D81 calls CreateModule first; it stores the
// tables 0x00C593B8 at +0x0, 0x00BEF9C0 at +0xC, 0x00C593A8 at +0x10. Members are what xfer (0x004851E3) transfers after CreateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004B8DAB ?Rva004B8DABGet@@YAHXZ
//   +0x0 slot 12 -> 0x004B8F22 ?onCreate@SupplyWarehouseCreate@@UAEXXZ
//   +0x0 slot 13 -> 0x00250A49 ?onBuildComplete@CreateModule@@UAEXXZ
//   +0x0 slot 14 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 15 -> 0x00098CBC ?Is_Valid@RegistryClass@@QAE_NXZ
//   +0x0 slot 18 -> 0x004B8EA2 ?Rva004B8EA2Get@@YAHXZ
//   +0x0 slot 25 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x0 slot 26 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x0 slot 28 -> 0x004B901D ?onCreate@GrantUpgradeCreate@@UAEXXZ
//   +0x0 slot 29 -> 0x004B9093 ?onBuildComplete@GrantUpgradeCreate@@UAEXXZ
//   +0x0 slot 34 -> 0x004B8F8C ?Rva004B8F8CGet@@YAHXZ
//   +0x10 slot 1 -> 0x004B8E2B ?onBuildComplete@SupplyCenterCreate@@UAEXXZ
#pragma once
#ifndef BFME2_SUPPLY_CENTER_CREATE_H
#define BFME2_SUPPLY_CENTER_CREATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/CreateModule.h"

class Thing;
class ModuleData;
class Xfer;

// CreateModule ends at 0x18; this class's members follow it.
typedef char SupplyCenterCreate_base_size[(sizeof(CreateModule) == 0x18) ? 1 : -1];

class SupplyCenterCreate : public CreateModule
{
public:
	SupplyCenterCreate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void onBuildComplete();	// override: onBuildComplete slot 1 at +0x10 (0x004B8E2B)

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
typedef char SupplyCenterCreate_size[(sizeof(SupplyCenterCreate) == 0x18) ? 1 : -1];

#endif
