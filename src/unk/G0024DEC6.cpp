// cl: /O1
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G0024DEC6 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "GameLogic/Module/BattlePlanUpdateModuleData.h"
#include "Common/INI.h"
#include "GameLogic/Module/BattlePlanUpdate.h"
#include "GameLogic/Module/BannerCarrierUpdate.h"
#include "GameLogic/Module/BoredUpdateModuleData.h"
#include "GameLogic/Module/BoredUpdate.h"
#include "GameLogic/Module/DemoTrapUpdateModuleData.h"
#include "GameLogic/Module/DemoTrapUpdate.h"
#include "GameLogic/Module/CommandButtonHuntUpdateModuleData.h"
#include "GameLogic/Module/CommandButtonHuntUpdate.h"
#include "GameLogic/Module/AutoPickUpUpdate.h"
#include "GameLogic/Module/AutoFindHealingUpdate.h"
#include "GameLogic/Module/GateOpenAndCloseBehavior.h"
#include "GameLogic/Module/BannerCarrierUpdateModuleData.h"
// Module creators. Retail registers each module class with a static friend_newModuleInstance that allocates the
// module for a thing and its module data, and each module data class with a static friend_newModuleData that
// allocates the data and, when an INI block is open, fills it from the class's buildFieldParse table. The bodies
// below are those two shapes. A class whose creator is not here yet has no real header, lacks the constructor or
// buildFieldParse declaration, or did not compile with its current header (the usual cause is a pure virtual that
// the header does not override yet); its row stays unwritten.
// FUNCTION: BFME2 0x0024DEC6 size=56 name=?friend_newModuleInstance@AutoFindHealingUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *AutoFindHealingUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new AutoFindHealingUpdate(thing, moduleData);
}
// FUNCTION: BFME2 0x0024DEFE size=81 name=?friend_newModuleData@AutoFindHealingUpdateModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *AutoFindHealingUpdateModuleData::friend_newModuleData(INI *ini)
{
	AutoFindHealingUpdateModuleData *data = new AutoFindHealingUpdateModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, AutoFindHealingUpdateModuleData::buildFieldParse);
	return data;
}

// FUNCTION: BFME2 0x0024DF4F size=56 name=?friend_newModuleInstance@CommandButtonHuntUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *CommandButtonHuntUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new CommandButtonHuntUpdate(thing, moduleData);
}
// FUNCTION: BFME2 0x0024DF87 size=81 name=?friend_newModuleData@CommandButtonHuntUpdateModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *CommandButtonHuntUpdateModuleData::friend_newModuleData(INI *ini)
{
	CommandButtonHuntUpdateModuleData *data = new CommandButtonHuntUpdateModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, CommandButtonHuntUpdateModuleData::buildFieldParse);
	return data;
}
// FUNCTION: BFME2 0x0024DFD8 size=56 name=?friend_newModuleInstance@DemoTrapUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *DemoTrapUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new DemoTrapUpdate(thing, moduleData);
}
// FUNCTION: BFME2 0x0024E010 size=81 name=?friend_newModuleData@DemoTrapUpdateModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *DemoTrapUpdateModuleData::friend_newModuleData(INI *ini)
{
	DemoTrapUpdateModuleData *data = new DemoTrapUpdateModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, DemoTrapUpdateModuleData::buildFieldParse);
	return data;
}
// FUNCTION: BFME2 0x0024E061 size=56 name=?friend_newModuleInstance@AutoPickUpUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *AutoPickUpUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new AutoPickUpUpdate(thing, moduleData);
}
// FUNCTION: BFME2 0x0024E099 size=81 name=?friend_newModuleData@AutoPickUpUpdateModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *AutoPickUpUpdateModuleData::friend_newModuleData(INI *ini)
{
	AutoPickUpUpdateModuleData *data = new AutoPickUpUpdateModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, AutoPickUpUpdateModuleData::buildFieldParse);
	return data;
}

// FUNCTION: BFME2 0x0024E0EA size=56 name=?friend_newModuleInstance@BoredUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *BoredUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new BoredUpdate(thing, moduleData);
}
// FUNCTION: BFME2 0x0024E122 size=81 name=?friend_newModuleData@BoredUpdateModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *BoredUpdateModuleData::friend_newModuleData(INI *ini)
{
	BoredUpdateModuleData *data = new BoredUpdateModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, BoredUpdateModuleData::buildFieldParse);
	return data;
}
// FUNCTION: BFME2 0x0024E173 size=56 name=?friend_newModuleInstance@BannerCarrierUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *BannerCarrierUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new BannerCarrierUpdate(thing, moduleData);
}
// FUNCTION: BFME2 0x0024E1AB size=81 name=?friend_newModuleData@BannerCarrierUpdateModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *BannerCarrierUpdateModuleData::friend_newModuleData(INI *ini)
{
	BannerCarrierUpdateModuleData *data = new BannerCarrierUpdateModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, BannerCarrierUpdateModuleData::buildFieldParse);
	return data;
}
// FUNCTION: BFME2 0x0024E1FC size=59 name=?friend_newModuleInstance@BattlePlanUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *BattlePlanUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new BattlePlanUpdate(thing, moduleData);
}
// FUNCTION: BFME2 0x0024E237 size=84 name=?friend_newModuleData@BattlePlanUpdateModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *BattlePlanUpdateModuleData::friend_newModuleData(INI *ini)
{
	BattlePlanUpdateModuleData *data = new BattlePlanUpdateModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, BattlePlanUpdateModuleData::buildFieldParse);
	return data;
}
// FUNCTION: BFME2 0x0024E28B size=65 name=?friend_newModuleInstance@GateOpenAndCloseBehavior@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *GateOpenAndCloseBehavior::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new GateOpenAndCloseBehavior(thing, moduleData);
}
// FUNCTION: BFME2 0x0024E2CC size=53 name=??0GateProxyBehavior@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x0024E301 size=13 name=?Rva0024E301Adjust@@YIPAXPAX@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
