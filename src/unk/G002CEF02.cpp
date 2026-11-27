// platformsdk
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G002CEF02 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include <windows.h>

extern unsigned char isLoadingMap;

// FUNCTION: BFME2 0x002CEF02 size=18 name=?yieldIfLoading@@YAXXZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
void yieldIfLoading()
{
	if (isLoadingMap)
		Sleep(0);
}
