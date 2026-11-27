// cl: /O1
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004C7E09 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include <vector>
#include "Common/AsciiString.h"
#include "Common/INI.h"
#include "Common/INI/FieldParse.h"
#include "GameLogic/Module/PlayerHealSpecialPowerModuleData.h"
#include "GameLogic/Module/PlayerUpgradeSpecialPowerModuleData.h"
// FUNCTION: BFME2 0x004C7E09 size=63 name=?Rva004C7E09_ParseUpgradeName@@YAXPAVINI@@PAX1PBX@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
void Rva004C7E09_ParseUpgradeName(INI *ini, void *instance, void *store, const void *userData)
{
	AsciiString name = ini->getNextAsciiString();
	static_cast<_STL::vector<AsciiString> *>(store)->push_back(name);
}
static const FieldParse s_playerUpgradeSpecialPowerModuleDataFieldParse[] =
{
	{ "UpgradeName", Rva004C7E09_ParseUpgradeName, 0, 0x7C },
	{ 0, 0, 0, 0 }
};

// FUNCTION: BFME2 0x004C7E48 size=27 name=?buildFieldParse@PlayerUpgradeSpecialPowerModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void PlayerUpgradeSpecialPowerModuleData::buildFieldParse(MultiIniFieldParse &parse)
{
	SpecialPowerModuleData::buildFieldParse(parse);
	parse.add(s_playerUpgradeSpecialPowerModuleDataFieldParse, 0);
}

static const FieldParse s_playerHealSpecialPowerModuleDataFieldParse[] =
{
	{ "HealAmount", INI::parseReal, 0, 0x7C },
	{ "HealAsPercent", INI::parseBool, 0, 0x80 },
	{ "HealAffects", BitFlags<218>::parseFromINI, 0, 0x88 },
	{ "HealRadius", INI::parseReal, 0, 0x84 },
	{ "HealFX", INI::parseFXList, 0, 0xA4 },
	{ "HealOCL", INI::parseObjectCreationList, 0, 0xA8 },
	{ 0, 0, 0, 0 }
};

// FUNCTION: BFME2 0x004C7E63 size=27 name=?buildFieldParse@PlayerHealSpecialPowerModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void PlayerHealSpecialPowerModuleData::buildFieldParse(MultiIniFieldParse &parse)
{
	SpecialPowerModuleData::buildFieldParse(parse);
	parse.add(s_playerHealSpecialPowerModuleDataFieldParse, 0);
}

