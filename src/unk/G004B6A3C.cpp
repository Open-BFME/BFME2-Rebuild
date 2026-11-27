// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004B6A3C name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "Common/INI.h"
#include "Common/INI/FieldParse.h"
#include "GameLogic/Module/GeometryUpgrade.h"
#include "GameLogic/Module/GeometryUpgradeModuleData.h"
#include "GameLogic/Module/UpgradeMuxData.h"
static const FieldParse s_geometryUpgradeModuleDataFieldParse[] =
{
	{ "ShowGeometry", INI::parseAsciiStringVectorAppend, 0, 0x118 },
	{ "HideGeometry", INI::parseAsciiStringVectorAppend, 0, 0x124 },
	{ "WallBoundsMesh", INI::parseAsciiString, 0, 0x130 },
	{ "RampMesh1", INI::parseAsciiString, 0, 0x134 },
	{ "RampMesh2", INI::parseAsciiString, 0, 0x138 },
	{ 0, 0, 0, 0 }
};

// FUNCTION: BFME2 0x004B6A3C size=34 name=?buildFieldParse@GeometryUpgradeModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void GeometryUpgradeModuleData::buildFieldParse(MultiIniFieldParse &parse)
{
	parse.add(UpgradeMuxData::getFieldParse(), 0x8);
	parse.add(s_geometryUpgradeModuleDataFieldParse, 0);
}

// FUNCTION: BFME2 0x004B6A5E size=91 name=??0GeometryUpgrade@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
GeometryUpgrade::GeometryUpgrade(Thing *thing, const ModuleData *moduleData)
	: UpgradeModule(thing, moduleData)
{
	m_unk0x1C.clear();
}
