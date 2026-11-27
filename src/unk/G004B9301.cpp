// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004B9301 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "GameLogic/Module/PreorderCreate.h"
#include "GameLogic/Object.h"
#include "Common/Player.h"
// FUNCTION: BFME2 0x004B9301 size=68 name=?onBuildComplete@PreorderCreate@@UAEXXZ evidence=X:map request tier=B hypothesis:mm1-gap-unit
void PreorderCreate::onBuildComplete()
{
	// Preordered players get the preorder model condition on the building.
	if (getObject()->getControllingPlayer()->m_unk0x33B)
		getObject()->setModelConditionState((ModelConditionFlagType)126);
	else
		getObject()->clearModelConditionState((ModelConditionFlagType)126);
}
