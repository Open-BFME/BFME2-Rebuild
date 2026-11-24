// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G00050C4D name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x00050C4D size=20 name=?testFlagBit@@YA_NHH@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
bool testFlagBit(int flags, int bit)
{
	return (flags & (1 << bit)) != 0;
}
