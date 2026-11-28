// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004B8CC9 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "GameLogic/Module/LockWeaponCreate.h"
#include "GameLogic/Module/LockWeaponCreateModuleData.h"
#include "GameLogic/Object.h"
#include "GameLogic/Weapon.h"
// FUNCTION: BFME2 0x004B8CC9 size=21 name=?onBuildComplete@LockWeaponCreate@@UAEXXZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void LockWeaponCreate::onBuildComplete()
{
	CreateModule::onBuildComplete();

	const ModuleData *moduleData = getModuleData();
	const LockWeaponCreateModuleData *data = static_cast<const LockWeaponCreateModuleData *>(moduleData);
	getObject()->setWeaponLock(static_cast<WeaponSlotType>(data->m_slotToLock), LOCKED_PERMANENTLY);
}
