// SupplyWarehouseDockUpdate.h: the supply warehouse dock module. SIZE 0x8C.
// Target facts: DockUpdate (0x88) is the base. The constructor 0x004A7DEA runs the base
// constructor and then copies StartingBoxes from its module data into +0x88. xfer 0x004A7D7B
// is Version1, the base xfer, then one Int at +0x88. The destructor 0x004A7D55 is empty apart
// from the vtable stores and a tail jump to ~DockUpdate. onObjectCreated, loadPostProcess,
// action and setDockCrippled override DockUpdate's and Module's virtuals in place, so the
// table keeps DockUpdate's slots. The +0x88 member name is still an offset, not a proven name.
#pragma once
#ifndef BFME2_SUPPLYWAREHOUSEDOCKUPDATE_H
#define BFME2_SUPPLYWAREHOUSEDOCKUPDATE_H

#include "Common/GameType.h"
#include "GameLogic/Module/DockUpdate.h"

class Object;
class Thing;
class ModuleData;
class Xfer;

class SupplyWarehouseDockUpdate : public DockUpdate
{
public:
	virtual DockUpdateInterface *getDockUpdateInterface() { return this; }
	SupplyWarehouseDockUpdate(Thing *thing, const ModuleData *moduleData);	// 0x004A7DEA
	virtual ~SupplyWarehouseDockUpdate();									// 0x004A7D55

	virtual NameKeyType getModuleNameKey() const;							// 0x004A7DA5
	virtual void onObjectCreated();											// 0x004A7E43
	virtual void loadPostProcess();											// 0x004A7FC5
	virtual Bool action(Object *docker, Object *drone);					// 0x004A7E67
	virtual void setDockCrippled(Bool setting);								// 0x004A800D
	void setCashValue(Int cashValue);										// 0x004A7F78 (map: rva004A7F78)

protected:
	virtual const char *GetSnapshotName() const;							// 0x004A7D75
	virtual void xfer(Xfer *xfer);											// 0x004A7D7B

public:
	Int m_boxesStored;						// +0x88, StartingBoxes at construction; xfer'd; action takes one per delivery, setCashValue sets it
};
// SIZE 0x8C

#endif
