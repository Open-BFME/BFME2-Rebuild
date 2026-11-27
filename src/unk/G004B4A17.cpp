// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004B4A17 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "Common/INI.h"
#include "Common/INI/FieldParse.h"
#include "GameLogic/Module/StatusBitsUpgradeIfEldestKindofModuleData.h"
#include "GameLogic/Module/StatusBitsUpgradeIfEldestKindof.h"
static const FieldParse s_statusBitsUpgradeIfEldestKindofModuleDataFieldParse[] =
{
	{ "ObjectFilter", iniParseObjectFilter, 0, 0x138 },
	{ 0, 0, 0, 0 }
};

// FUNCTION: BFME2 0x004B4A17 size=27 name=?buildFieldParse@StatusBitsUpgradeIfEldestKindofModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void StatusBitsUpgradeIfEldestKindofModuleData::buildFieldParse(MultiIniFieldParse &parse)
{
	StatusBitsUpgradeModuleData::buildFieldParse(parse);
	parse.add(s_statusBitsUpgradeIfEldestKindofModuleDataFieldParse, 0);
}

// FUNCTION: BFME2 0x004B4A32 size=84 name=??0StatusBitsUpgradeIfEldestKindof@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
StatusBitsUpgradeIfEldestKindof::StatusBitsUpgradeIfEldestKindof(Thing *thing, const ModuleData *moduleData) :
	StatusBitsUpgrade(thing, moduleData)
{
	m_eldest.cacheGameLogicFrame();
}
