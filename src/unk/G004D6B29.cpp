// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G004D6B29 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include <math.h>
#include "Common/GameType.h"

// FUNCTION: BFME2 0x004D6B29 size=17 name=?Rva004D6B29@@YAMM@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004D6B3A size=52 name=?fovToFocalLength@@YAMM@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Real fovToFocalLength(Real fov)
{
	Real t = tan(fov * 0.5f);
	if (t != 0.0f)
		return 12.0f / t;
	return 1.0e10f;
}
// FUNCTION: BFME2 0x004D6B6E size=47 name=?focalLengthToFov@@YAMM@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Real focalLengthToFov(Real focalLength)
{
	if (focalLength > 0.0f)
		return atan(12.0f / focalLength) * 2.0f;
	return 1.5707964f;
}
// FUNCTION: BFME2 0x004D6B9D size=27 name=?fovDegreesToFocalLength@@YAMM@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Real fovDegreesToFocalLength(Real degrees)
{
	return fovToFocalLength(degrees * 0.017453292f);
}
// FUNCTION: BFME2 0x004D6BB8 size=21 name=?focalLengthToFovDegrees@@YAMM@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
Real focalLengthToFovDegrees(Real focalLength)
{
	return focalLengthToFov(focalLength) * 57.295776f;
}
// FUNCTION: BFME2 0x004D6BCD size=35 name=?rva004D6BCD@Rva004D6BCD@@QBE_NPBVObject@@@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004D6BF0 size=57 name=?rva004D6BF0@Rva004D6BF0@@QAEXPAX@Z evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x004D6C29 size=31 name=?clearSquad@Squad@@QAEXXZ evidence=X:map request tier=B hypothesis:mm1-gap-unit
