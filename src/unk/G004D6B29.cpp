// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004D6B29 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "GameLogic/Squad.h"
// FUNCTION: BFME2 0x004D6B29 size=17 name=?Rva004D6B29@@YAMM@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004D6B3A size=52 name=?fovToFocalLength@@YAMM@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004D6B6E size=47 name=?focalLengthToFov@@YAMM@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004D6B9D size=27 name=?fovDegreesToFocalLength@@YAMM@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004D6BB8 size=21 name=?focalLengthToFovDegrees@@YAMM@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004D6BCD size=35 name=?rva004D6BCD@Rva004D6BCD@@QBE_NPBVObject@@@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004D6BF0 size=57 name=?rva004D6BF0@Rva004D6BF0@@QAEXPAX@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004D6C29 size=31 name=?clearSquad@Squad@@QAEXXZ evidence=X:map request tier=B hypothesis:mm1-gap-unit
void Squad::clearSquad()
{
	m_objectIDs.clear();
	m_objectsCached.clear();
}
