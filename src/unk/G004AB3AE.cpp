// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004AB3AE name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "GameLogic/Module/WallUpgradeUpdate.h"
#include "GameLogic/Module/UpdateModule.h"
#include "GameLogic/GameLogic.h"
#include "GameLogic/Object.h"
#include "Common/GameType.h"
// FUNCTION: BFME2 0x004AB3AE size=61 name=?rva004AB3AE@WallUpgradeUpdate@@UAE_NXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
bool WallUpgradeUpdate::rva004AB3AE()
{
	Object *obj = getObject();
	return obj->testModelConditionState((ModelConditionFlagType)0x109)
		|| obj->testModelConditionState((ModelConditionFlagType)0x108)
		|| obj->testModelConditionState((ModelConditionFlagType)0x107);
}

// FUNCTION: BFME2 0x004AB3EB size=42 name=?onBodyDamageStateChange@WallUpgradeUpdate@@UAEXPBVDamageInfo@@W4BodyDamageType@@1@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void WallUpgradeUpdate::onBodyDamageStateChange(const DamageInfo *damageInfo, BodyDamageType oldState, BodyDamageType newState)
{
	if (newState > oldState && newState == BODY_RUBBLE)
	{
		m_2C = BfmeLogicFramesPerSecond * 8;
		setWakeFrame(getObject(), UPDATE_SLEEP_NONE);
	}
}
