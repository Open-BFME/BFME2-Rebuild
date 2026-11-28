// cl: /O1
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G002555AC name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "GameLogic/Module/CommandSetUpgradeModuleData.h"
#include "Common/INI.h"
#include "GameLogic/Module/StatusBitsUpgradeIfEldestKindofModuleData.h"
#include "GameLogic/Module/FadeAndDieOrnamentUpdateModuleData.h"
// Module creators. Retail registers each module class with a static friend_newModuleInstance that allocates the
// module for a thing and its module data, and each module data class with a static friend_newModuleData that
// allocates the data and, when an INI block is open, fills it from the class's buildFieldParse table. The bodies
// below are those two shapes. A class whose creator is not here yet has no real header, lacks the constructor or
// buildFieldParse declaration, or did not compile with its current header (the usual cause is a pure virtual that
// the header does not override yet); its row stays unwritten.
// FUNCTION: BFME2 0x002555AC size=85 name=??0FadeAndDieOrnamentUpdateModuleData@@QAE@XZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x00255601 size=81 name=?friend_newModuleData@FadeAndDieOrnamentUpdateModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *FadeAndDieOrnamentUpdateModuleData::friend_newModuleData(INI *ini)
{
	FadeAndDieOrnamentUpdateModuleData *data = new FadeAndDieOrnamentUpdateModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, FadeAndDieOrnamentUpdateModuleData::buildFieldParse);
	return data;
}
// FUNCTION: BFME2 0x00255652 size=70 name=??0CommandSetUpgradeModuleData@@QAE@XZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
CommandSetUpgradeModuleData::CommandSetUpgradeModuleData()
{
	m_commandSet = AsciiString::TheEmptyString;
}
// FUNCTION: BFME2 0x00255698 size=84 name=?friend_newModuleData@CommandSetUpgradeModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *CommandSetUpgradeModuleData::friend_newModuleData(INI *ini)
{
	CommandSetUpgradeModuleData *data = new CommandSetUpgradeModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, CommandSetUpgradeModuleData::buildFieldParse);
	return data;
}
// FUNCTION: BFME2 0x002556EC size=84 name=?friend_newModuleData@StatusBitsUpgradeIfEldestKindofModuleData@@SAPAVModuleData@@PAVINI@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
ModuleData *StatusBitsUpgradeIfEldestKindofModuleData::friend_newModuleData(INI *ini)
{
	StatusBitsUpgradeIfEldestKindofModuleData *data = new StatusBitsUpgradeIfEldestKindofModuleData;
	if (ini)
		ini->initFromINIMultiProc(data, StatusBitsUpgradeIfEldestKindofModuleData::buildFieldParse);
	return data;
}
// FUNCTION: BFME2 0x0025575C size=57 name=??1StatusBitsUpgradeIfEldestKindofModuleData@@UAE@XZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
