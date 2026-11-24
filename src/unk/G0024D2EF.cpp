// cl: /O1
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G0024D2EF name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "GameLogic/Module/SiegeAIUpdateModuleData.h"
#include "Common/INI.h"
#include "GameLogic/Module/SiegeAIUpdate.h"
#include "GameLogic/Module/LargeGroupBonusUpdateModuleData.h"
#include "GameLogic/Module/LargeGroupBonusUpdate.h"
#include "GameLogic/Module/GiantBirdAIUpdateModuleData.h"
#include "GameLogic/Module/GiantBirdAIUpdate.h"
#include "GameLogic/Module/LaserUpdateModuleData.h"
#include "GameLogic/Module/LaserUpdate.h"
#include "GameLogic/Module/HeightDieUpdateModuleData.h"
#include "GameLogic/Module/HeightDieUpdate.h"
// Module creators. Retail registers each module class with a static friend_newModuleInstance that allocates the
// module for a thing and its module data, and each module data class with a static friend_newModuleData that
// allocates the data and, when an INI block is open, fills it from the class's buildFieldParse table. The bodies
// below are those two shapes. A class whose creator is not here yet has no real header, lacks the constructor or
// buildFieldParse declaration, or did not compile with its current header (the usual cause is a pure virtual that
// the header does not override yet); its row stays unwritten.
// FUNCTION: BFME2 0x0024D2EF size=59 name=?friend_newModuleInstance@GiantBirdAIUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *GiantBirdAIUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new GiantBirdAIUpdate(thing, moduleData);
}
// FUNCTION: BFME2 0x0024D32A size=84 name=?friend_newModuleData@GiantBirdAIUpdateModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *GiantBirdAIUpdateModuleData::friend_newModuleData(INI *ini)
{
	GiantBirdAIUpdateModuleData *data = new GiantBirdAIUpdateModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, GiantBirdAIUpdateModuleData::buildFieldParse);
	return data;
}
// FUNCTION: BFME2 0x0024D37E size=56 name=?friend_newModuleInstance@HeightDieUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *HeightDieUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new HeightDieUpdate(thing, moduleData);
}
// FUNCTION: BFME2 0x0024D3B6 size=81 name=?friend_newModuleData@HeightDieUpdateModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *HeightDieUpdateModuleData::friend_newModuleData(INI *ini)
{
	HeightDieUpdateModuleData *data = new HeightDieUpdateModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, HeightDieUpdateModuleData::buildFieldParse);
	return data;
}
// FUNCTION: BFME2 0x0024D407 size=56 name=?friend_newModuleInstance@LargeGroupBonusUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *LargeGroupBonusUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new LargeGroupBonusUpdate(thing, moduleData);
}
// FUNCTION: BFME2 0x0024D43F size=81 name=?friend_newModuleData@LargeGroupBonusUpdateModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *LargeGroupBonusUpdateModuleData::friend_newModuleData(INI *ini)
{
	LargeGroupBonusUpdateModuleData *data = new LargeGroupBonusUpdateModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, LargeGroupBonusUpdateModuleData::buildFieldParse);
	return data;
}
// FUNCTION: BFME2 0x0024D490 size=59 name=?friend_newModuleInstance@SiegeAIUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *SiegeAIUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new SiegeAIUpdate(thing, moduleData);
}
// FUNCTION: BFME2 0x0024D4CB size=81 name=?friend_newModuleData@SiegeAIUpdateModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *SiegeAIUpdateModuleData::friend_newModuleData(INI *ini)
{
	SiegeAIUpdateModuleData *data = new SiegeAIUpdateModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, SiegeAIUpdateModuleData::buildFieldParse);
	return data;
}
// FUNCTION: BFME2 0x0024D51C size=6 name=?Rva0024D51CGet@@YAHXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// No retail code or data refers to this function. It follows the creators of SiegeAIUpdate and returns the bit set 0x800, which looks like a module interface mask;
// nothing ties it to SiegeAIUpdate, so it keeps its ledger name.
int Rva0024D51CGet()
{
	return 2048;
}
// FUNCTION: BFME2 0x0024D522 size=56 name=?friend_newModuleInstance@LaserUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *LaserUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new LaserUpdate(thing, moduleData);
}
// FUNCTION: BFME2 0x0024D55A size=81 name=?friend_newModuleData@LaserUpdateModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *LaserUpdateModuleData::friend_newModuleData(INI *ini)
{
	LaserUpdateModuleData *data = new LaserUpdateModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, LaserUpdateModuleData::buildFieldParse);
	return data;
}
