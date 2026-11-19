// SupplyWarehouseDockUpdateModuleData.h: the INI data of SupplyWarehouseDockUpdate. SIZE 0x18.
// Target facts: the module is registered with operator new 0x18; its ctor reads
// StartingBoxes (Int) at +0x10 and writes the module's box count from there. DockUpdateModuleData
// (0x10) is the base; DeleteWhenEmpty (Bool) sits at +0x14 and bytes +0x15..+0x17 are padding.
#pragma once
#ifndef BFME2_SUPPLYWAREHOUSEDOCKUPDATEMODULEDATA_H
#define BFME2_SUPPLYWAREHOUSEDOCKUPDATEMODULEDATA_H

#include "Common/GameType.h"
#include "GameLogic/Module/DockUpdate.h"

class SupplyWarehouseDockUpdateModuleData : public DockUpdateModuleData
{
public:
	static void buildFieldParse(MultiIniFieldParse &parse);	// 0x004A7D3A

	Int StartingBoxes;						// 0x10
	Bool DeleteWhenEmpty;					// 0x14
};
// SIZE 0x18

// Retail VA 0x00C5370C: two fields and the zero sentinel.
extern const FieldParse BfmeSupplyWarehouseDockUpdateFields[];

#endif
