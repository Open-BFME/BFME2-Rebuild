// Generated from the ledger; map snapshot c591f7146f88c8a8248ebb8ae22f0c978c22aed463db64e05ef57ff0dfae99cc
// object=G001B9680 name-evidence=invented
// Extents and edges are hypotheses; byte verification alone does not establish original TU boundaries.
#include <string.h>

#include "GameClient/VP6FrameBuffer.h"
#include "GameClient/VP6PlaybackInstance.h"

extern "C" void UpdateUMVBorder(PB_INSTANCE *pbi, int offset);

// FUNCTION: BFME2 0x001B9680 size=22 name=_VP6_InitialiseConfiguration evidence=N:ledger tier=B hypothesis:mm1-gap-unit
extern "C" void VP6_InitialiseConfiguration(PB_INSTANCE *pbi)
{
	pbi->m_unk0x1C0 = 8;
	pbi->m_unk0x1C4 = 8;
}

// FUNCTION: BFME2 0x001B96A0 size=1047 name=_UpdateUMVBorder evidence=N:ledger tier=B hypothesis:mm1-gap-unit
// FUNCTION: BFME2 0x001B9AC0 size=299 name=_CopyFrame evidence=N:ledger tier=B hypothesis:mm1-gap-unit
extern "C" void CopyFrame(PB_INSTANCE *pbi, const YUV_BUFFER_CONFIG *src, int offset)
{
	unsigned char *dst = pbi->m_yBuffer + offset;
	const unsigned char *from = src->m_yBuffer;
	for (int row = 0; row < src->m_yHeight; row++)
	{
		memcpy(dst, from, src->m_yWidth);
		dst += pbi->m_yStride;
		from += src->m_yStride;
	}

	dst = pbi->m_uBuffer + offset;
	from = src->m_uBuffer;
	for (int row = 0; row < src->m_uvHeight; row++)
	{
		memcpy(dst, from, src->m_uvWidth);
		dst += pbi->m_uvStride;
		from += src->m_uvStride;
	}

	dst = pbi->m_vBuffer + offset;
	from = src->m_vBuffer;
	for (int row = 0; row < src->m_uvHeight; row++)
	{
		memcpy(dst, from, src->m_uvWidth);
		dst += pbi->m_uvStride;
		from += src->m_uvStride;
	}

	UpdateUMVBorder(pbi, offset);
}
