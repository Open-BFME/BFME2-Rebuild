// SupplyWarehouseCreate.h: the SupplyWarehouseCreate module. SIZE 0x18 (ModuleFactory operator new).
// Target facts (h05 modgen): the constructor 0x004B8E78 calls CreateModule first; it stores the
// tables 0x00C593F8 at +0x0, 0x00BEF9C0 at +0xC, 0x00C593E8 at +0x10. Members are what xfer (0x004851E3) transfers after CreateModule::xfer,
// typed by the Xfer slot each goes through, in that order; m_unk names are offsets
// (no map or donor name established). Bytes xfer does not touch are gaps.
// Overrides of the base tables besides the trio (slot -> target, map name):
//   +0x0 slot 1 -> 0x00497805 ?loadPostProcess@BoneFXDamage@@MAEXXZ
//   +0x0 slot 2 -> 0x004B8EA2 ?Rva004B8EA2Get@@YAHXZ
//   +0x0 slot 12 -> 0x004B901D ?onCreate@GrantUpgradeCreate@@UAEXXZ
//   +0x0 slot 13 -> 0x004B9093 ?onBuildComplete@GrantUpgradeCreate@@UAEXXZ
//   +0x0 slot 14 -> 0x000B3FD0 ??1Region2D@@QAE@XZ
//   +0x0 slot 15 -> 0x00098CBC ?Is_Valid@RegistryClass@@QAE_NXZ
//   +0x0 slot 18 -> 0x004B8F8C ?Rva004B8F8CGet@@YAHXZ
//   +0x0 slot 25 -> 0x0050B238 ?SkipBadBlock@Xfer@@UAEXAAVSnapshot@@I@Z
//   +0x0 slot 26 -> 0x000D43D0 ?ControlBarInput@@YA?AW4WindowMsgHandledType@@PAVGameWindow@@III@Z
//   +0x10 slot 0 -> 0x004B8F22 ?onCreate@SupplyWarehouseCreate@@UAEXXZ
#pragma once
#ifndef BFME2_SUPPLY_WAREHOUSE_CREATE_H
#define BFME2_SUPPLY_WAREHOUSE_CREATE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "GameLogic/Module/CreateModule.h"

class Thing;
class ModuleData;
class Xfer;

// CreateModule ends at 0x18; this class's members follow it.
typedef char SupplyWarehouseCreate_base_size[(sizeof(CreateModule) == 0x18) ? 1 : -1];

class SupplyWarehouseCreate : public CreateModule
{
public:
	SupplyWarehouseCreate(Thing *thing, const ModuleData *moduleData);
	virtual NameKeyType getModuleNameKey() const;
	virtual void onCreate();	// override: onCreate slot 0 at +0x10 (0x004B8F22)

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

public:
protected:
	virtual void loadPostProcess() { CreateModule::loadPostProcess(); }	// retail slot 0x00497805: a jmp to CreateModule's, ICF-folded across classes (h05)
};
// SIZE 0x18
typedef char SupplyWarehouseCreate_size[(sizeof(SupplyWarehouseCreate) == 0x18) ? 1 : -1];

#endif
