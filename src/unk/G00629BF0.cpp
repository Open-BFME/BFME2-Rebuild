#include <float.h>
// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G00629BF0 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x00629BF0 size=68 name=__RTC_Initialize evidence=N:ledger tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x00629C7E size=18 name=__setdefaultprecision evidence=N:ledger tier=B hypothesis:mm1-gap-unit
extern "C" void __cdecl _setdefaultprecision(void)
{
	_controlfp(_PC_53, _MCW_PC);
}
