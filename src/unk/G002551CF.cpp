// cl: /O1
#include "GameLogic/Module/NotifyTargetsOfImminentProbableCrushingUpdate.h"
// Module creators. Retail registers each module class with a static friend_newModuleInstance that allocates the
// module for a thing and its module data, and each module data class with a static friend_newModuleData that
// allocates the data and, when an INI block is open, fills it from the class's buildFieldParse table. The bodies
// below are those two shapes. A class whose creator is not here yet has no real header, lacks the constructor or
// buildFieldParse declaration, or did not compile with its current header (the usual cause is a pure virtual that
// the header does not override yet); its row stays unwritten.
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G002551CF name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x002551CF size=56 name=?friend_newModuleInstance@HordeNotifyTargetsOfImminentProbableCrushingUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *HordeNotifyTargetsOfImminentProbableCrushingUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new HordeNotifyTargetsOfImminentProbableCrushingUpdate(thing, moduleData);
}
// FUNCTION: BFME2 0x00255207 size=56 name=?friend_newModuleInstance@NotifyTargetsOfImminentProbableCrushingUpdate@@SAPAVModule@@PAVThing@@PBVModuleData@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Module *NotifyTargetsOfImminentProbableCrushingUpdate::friend_newModuleInstance(Thing *thing, const ModuleData *moduleData)
{
	return new NotifyTargetsOfImminentProbableCrushingUpdate(thing, moduleData);
}
