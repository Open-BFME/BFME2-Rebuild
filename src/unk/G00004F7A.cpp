// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G00004F7A name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "debug/Debug.h"
#include "Common/RealRange.h"
// FUNCTION: BFME2 0x00004F7A size=78 name=??6@YAAAVDebug@@AAV0@ABURealRange@@@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Debug &operator<<(Debug &debug, const RealRange &range)
{
	debug << "[" << range.lo << ", " << range.hi << "]";
	return debug;
}
