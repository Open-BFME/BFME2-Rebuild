#include "WW3D2/dxwrapper.h"
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G0018BD7B name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x0018BD7B size=7 name=?getFogRangeStart@@YAMXZ evidence=N:ledger tier=B hypothesis:mm1-gap-unit
float getFogRangeStart()
{
	return DXWrapper::FogStart;
}

// FUNCTION: BFME2 0x0018BD82 size=7 name=?Rva0018BD82Get@@YAMXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
float Rva0018BD82Get()
{
	return DXWrapper::FogEnd;
}
