// cl: /O1
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G0024DC54 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "Common/INI.h"
#include "GameLogic/Module/SpecialPowerModule.h"
#include "GameLogic/Module/WeaponModeSpecialPowerUpdate.h"
#include "GameLogic/Module/WeaponModeSpecialPowerUpdateModuleData.h"
// Module creators. Retail registers each module class with a static friend_newModuleInstance that allocates the
// module for a thing and its module data, and each module data class with a static friend_newModuleData that
// allocates the data and, when an INI block is open, fills it from the class's buildFieldParse table. The bodies
// below are those two shapes. A class whose creator is not here yet has no real header, lacks the constructor or
// buildFieldParse declaration, or did not compile with its current header (the usual cause is a pure virtual that
// the header does not override yet); its row stays unwritten.
// FUNCTION: BFME2 0x0024DC54 size=56 name=?friend_newModuleInstance@SpecialPowerModule@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *SpecialPowerModule::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new SpecialPowerModule(thing, moduleData);
}
// FUNCTION: BFME2 0x0024DC8C size=81 name=?friend_newModuleData@SpecialPowerModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *SpecialPowerModuleData::friend_newModuleData(INI *ini)
{
	SpecialPowerModuleData *data = new SpecialPowerModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, SpecialPowerModuleData::buildFieldParse);
	return data;
}
// FUNCTION: BFME2 0x0024DCDD size=6 name=?Rva0024DCDDGet@@YAHXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// No retail code or data refers to this function. It follows the creators of SpecialPower and returns the bit set 0x100, which looks like a module interface mask;
// nothing ties it to SpecialPower, so it keeps its ledger name.
int Rva0024DCDDGet()
{
	return 256;
}
// FUNCTION: BFME2 0x0024DCE3 size=6 name=?Rva0024DCE3Get@@YAHXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// No retail code or data refers to this function. It follows the creators of SpecialPower and returns the bit set 0x101, which looks like a module interface mask;
// nothing ties it to SpecialPower, so it keeps its ledger name.
int Rva0024DCE3Get()
{
	return 257;
}
// FUNCTION: BFME2 0x0024DCE9 size=56 name=?friend_newModuleInstance@WeaponModeSpecialPowerUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *WeaponModeSpecialPowerUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new WeaponModeSpecialPowerUpdate(thing, moduleData);
}
// FUNCTION: BFME2 0x0024DD21 size=81 name=?friend_newModuleData@WeaponModeSpecialPowerUpdateModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *WeaponModeSpecialPowerUpdateModuleData::friend_newModuleData(INI *ini)
{
	WeaponModeSpecialPowerUpdateModuleData *data = new WeaponModeSpecialPowerUpdateModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, WeaponModeSpecialPowerUpdateModuleData::buildFieldParse);
	return data;
}

