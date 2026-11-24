// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004DF7DB name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "GameLogic/Object.h"
#include "GameLogic/Module/BodyModule.h"
#include "GameLogic/Module/ObjectRecoveryHelper.h"
#include "GameLogic/Module/ObjectRepulsorHelper.h"
// FUNCTION: BFME2 0x004DF7DB size=64 name=?rva004DF7DB@Rva004DF7DB@@QAEXW4UpdateSleepTime@@@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004DF836 size=25 name=??1ObjectRecoveryHelper@@UAE@XZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004DF84F size=20 name=?update@ObjectRecoveryHelper@@UAE?AW4UpdateSleepTime@@XZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
UpdateSleepTime ObjectRecoveryHelper::update()
{
	getObject()->getBodyModule()->reapplyDamageState();
	return UPDATE_SLEEP_FOREVER;
}
// FUNCTION: BFME2 0x004DF863 size=25 name=??1ObjectRepulsorHelper@@UAE@XZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004DF897 size=18 name=?update@ObjectRepulsorHelper@@UAE?AW4UpdateSleepTime@@XZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
UpdateSleepTime ObjectRepulsorHelper::update()
{
	// if we ever get here, clear this.
	getObject()->setStatus(OBJECT_STATUS_REPULSOR, false);

	// then go back to sleep until we are forcibly awakened.
	return UPDATE_SLEEP_FOREVER;
}
// FUNCTION: BFME2 0x004DF8D8 size=133 name=?Rva004DF8D8Test@@YA_NPBUVec2@@00MMMM@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004DF95D size=35 name=?rva004DF95D@Rva004DF95DOwner@@QAEXXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004DF980 size=3 name=?get@Rva004DF980@@QBEEXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
