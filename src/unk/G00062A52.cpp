// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G00062A52 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#include "GameClient/WorldHeightMap.h"

// FUNCTION: BFME2 0x00062A52 size=6 name=?Rva00062A52Get@@YAHXZ evidence=?:placeholder tier=B hypothesis:mm1-gap-unit
// No retail code or data refers to this function. What the constant 65535 means to its caller is not evidenced, so the row keeps its ledger name.
int Rva00062A52Get()
{
	return 65535;
}
// FUNCTION: BFME2 0x00062A58 size=38 name=?getHeight@WorldHeightMap@@QBEGHH@Z evidence=N:ledger tier=B hypothesis:mm1-gap-unit
UnsignedShort WorldHeightMap::getHeight(Int xIndex, Int yIndex) const
{
	Int index = yIndex * m_width + xIndex;
	if (index >= 0 && index < m_dataSize && m_data)
		return m_data[index];
	return 0;
}
