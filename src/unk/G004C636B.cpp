#include "Common/INI.h"
#include "GameLogic/Module/SiegeDeployHordeSpecialPowerModuleData.h"
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004C636B name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x004C636B size=27 name=?buildFieldParse@SiegeDeployHordeSpecialPowerModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void SiegeDeployHordeSpecialPowerModuleData::buildFieldParse(MultiIniFieldParse &p)
{
	SpecialPowerUpdateModuleData::buildFieldParse(p);
	static const FieldParse dataFieldParse[] = {
		{ "HordeDeploy", INI::parseBool, 0, 0x18 },
		{ 0, 0, 0, 0 }
	};
	p.add(dataFieldParse);
}
// FUNCTION: BFME2 0x004C6386 size=82 name=??0SiegeDeployHordeSpecialPower@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
SiegeDeployHordeSpecialPower::SiegeDeployHordeSpecialPower(Thing *thing, const ModuleData *moduleData)
	: SpecialPowerUpdateModule(thing, moduleData)
{
	m_unk0x38 = 0;
	m_unk0x3C.x = 0.0f;
	m_unk0x3C.y = 0.0f;
	m_unk0x3C.z = 0.0f;
	m_unk0x48 = 0;
}
