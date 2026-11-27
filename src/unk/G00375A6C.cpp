// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G00375A6C name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "GameLogic/Object.h"
// FUNCTION: BFME2 0x00375A6C size=7 name=?get@Rva00375A6CDwordField@@QBEHXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x00375A73 size=116 name=?rva00375A73@AerialPathfinder@@QAE_NPAVRva00375A73Context@@MPAURva00375A73Coord@@@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x00375AE7 size=16 name=?rva00375AE7@Object@@QBEMPBV1@@Z evidence=P:provisional (wb-assert) tier=B hypothesis:mm1-gap-unit
Real Object::rva00375AE7(const Object *other) const
{
	return Get3DCenterDistanceSquared(other->getPosition());
}
