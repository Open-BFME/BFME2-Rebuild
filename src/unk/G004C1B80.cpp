#include <stddef.h>
#include "Common/INI.h"
#include "GameLogic/Module/DetachableRiderBodyModuleData.h"
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004C1B80 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// Field parse table for DetachableRiderBodyModuleData (decoded from retail image).
static const FieldParse s_detachableRiderBodyModuleDataFieldParse[] =
{
	{ "HealthPercentageWhenRiderDies", INI::parsePercentToReal, 0, offsetof(DetachableRiderBodyModuleData, m_healthPercentageWhenRiderDies) },
	{ "StartsActive", INI::parseBool, 0, offsetof(DetachableRiderBodyModuleData, m_startsActive) },
	{ "RiderlessDeathChance", INI::parsePercentToReal, 0, offsetof(DetachableRiderBodyModuleData, m_riderlessDeathChance) },
	{ 0, 0, 0, 0 }
};

// FUNCTION: BFME2 0x004C1B80 size=43 name=?buildFieldParse@DetachableRiderBodyModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void DetachableRiderBodyModuleData::buildFieldParse(MultiIniFieldParse &parse)
{
	ActiveBodyModuleData::buildFieldParse(parse);
	parse.add(UpgradeMuxData::getFieldParse(), offsetof(DetachableRiderBodyModuleData, m_upgradeMuxData));
	parse.add(s_detachableRiderBodyModuleDataFieldParse, 0);
}
