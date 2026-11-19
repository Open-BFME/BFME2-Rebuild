// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// W3DShroud.h: the shroud (fog of war) texture owner (Zero Hour's W3DShroud.h, Mark Wilczynski).
// Target facts (retail game.dat): getShroudLevel 0x00072988 reads the cell counts at +0 and +4 and the 16-bit texel data
// pointer at +0x18; ReAcquireResources 0x000728F0 reads a texture handle at +0x1C (TextureBaseClass), its source and pitch
// words at +0x20/+0x24 and sets a flag byte at +0x35. The bytes between are not read by the rows written.
#pragma once
#ifndef BFME2_W3DSHROUD_H
#define BFME2_W3DSHROUD_H

#include "Common/GameType.h"

class W3DShroud
{
public:
	UnsignedByte getShroudLevel(Int x, Int y);		// 0x00072988

protected:
	Int m_numCellsX;							// +0x00
	Int m_numCellsY;							// +0x04
	Byte m_unk08[0x10];							// +0x08
	UnsignedShort *m_finalFogData;				// +0x18
};

#endif
