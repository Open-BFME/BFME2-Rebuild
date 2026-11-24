// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#include "Common/Veterancy.h"
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004CE519 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x004CE519 size=21 name=?getVeterancyLevelFlag@@YA_NIW4VeterancyLevel@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
bool getVeterancyLevelFlag(unsigned int flags, VeterancyLevel level)
{
	return (flags & (1UL << (level - 1))) != 0;
}
