// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G00101E1E name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include "Common/GameType.h"

// FUNCTION: BFME2 0x00101E1E size=79 name=?W3DLogicalScreenToPixelScreen@@YAXMMPAH0HH@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void W3DLogicalScreenToPixelScreen(Real logX, Real logY, Int *screenX, Int *screenY, Int screenWidth, Int screenHeight)
{
	*screenX = (Int)((screenWidth * (logX + 1.0f)) * 0.5f);
	*screenY = (Int)((screenHeight * (1.0f - logY)) * 0.5f);
}
// FUNCTION: BFME2 0x00101E6D size=86 name=?PixelScreenToW3DLogicalScreen@@YAXHHPAM0HH@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void PixelScreenToW3DLogicalScreen(Int screenX, Int screenY, Real *logX, Real *logY, Int screenWidth, Int screenHeight)
{
	*logX = ((2.0f * screenX) / (Real)screenWidth) - 1.0f;
	*logY = -(((2.0f * screenY) / (Real)screenHeight) - 1.0f);
}
// FUNCTION: BFME2 0x00101EC3 size=12 name=?rva00101EC3@Rva00101EC3@@QAEXXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
