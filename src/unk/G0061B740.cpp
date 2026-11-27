// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G0061B740 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// FUNCTION: BFME2 0x0061B740 size=18 name=?XMP_Is_Negative@@YA_NPBIH@Z evidence=N:ledger tier=B hypothesis:mm1-held-object-same-path
bool XMP_Is_Negative(const unsigned int *r, int precision)
{
	return (int)r[precision - 1] < 0;
}
