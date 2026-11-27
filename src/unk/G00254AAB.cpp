// cl: /O1
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G00254AAB name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include <stddef.h>
#include "Common/INI.h"
#include "Common/INI/FieldParse.h"
#include "GameLogic/Module/SpecialPowerCompletionDieModuleData.h"
#include "GameLogic/Module/DieMuxData.h"
#include "GameLogic/Module/SquishCollide.h"
#include "GameLogic/Module/ModuleData.h"
// FUNCTION: BFME2 0x00254AAB size=81 name=?friend_newModuleData@SquishCollide@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *SquishCollide::friend_newModuleData(INI *ini)
{
	ModuleData *data = new ModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, ModuleData::buildFieldParse);
	return data;
}

// FUNCTION: BFME2 0x00254AFC size=34 name=?buildFieldParse@SpecialPowerCompletionDieModuleData@@SAXAAVMultiIniFieldParse@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
static const FieldParse s_specialPowerCompletionDieModuleDataFieldParse[] =
{
	{ "SpecialPowerTemplate", INI::parseSpecialPowerTemplate, 0, offsetof(SpecialPowerCompletionDieModuleData, m_specialPowerTemplate) },
	{ 0, 0, 0, 0 }
};

void SpecialPowerCompletionDieModuleData::buildFieldParse(MultiIniFieldParse &p)
{
	p.add(DieMuxData::getFieldParse(), 0x8);
	p.add(s_specialPowerCompletionDieModuleDataFieldParse, 0);
}

// FUNCTION: BFME2 0x00254B1E size=56 name=?friend_newModuleInstance@SpecialPowerTimerRefreshSpecialPower@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x00254B82 size=42 name=??0ModelConditionSoundSelectorClientBehavior@@QAE@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
