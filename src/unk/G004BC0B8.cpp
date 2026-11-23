#include "GameLogic/Module/AODCrushCollide.h"
#include "GameLogic/GameLogic.h"
#include "GameLogic/Object.h"
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004BC0B8 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x004BC0B8 size=558 name=?onCollide@AODCrushCollide@@UAEXPAVObject@@PBVCoord3D@@1@Z evidence=X:retail PE exports class Coord3D: ??0Matrix4D@@QAE@ABVCoord3D@@@Z tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004BC2E6 size=63 name=?update@AODCrushCollide@@UAE?AW4UpdateSleepTime@@XZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
UpdateSleepTime AODCrushCollide::update()
{
	if (m_unk0x2C)
	{
		if (m_unk0x24 < TheGameLogic->m_frame)
		{
			getObject()->clearModelConditionState(1 * 32 + 9);
			m_unk0x2C = false;
		}
		return UPDATE_SLEEP_NONE;
	}
	return UPDATE_SLEEP_FOREVER;
}
