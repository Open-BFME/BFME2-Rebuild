// VP6FrameBuffer.h: the VP6 codec's planar frame description (YUV_BUFFER_CONFIG).
// SIZE 0x24. Target facts: CopyFrame 0x001B9AC0 reads the luma and chroma
// extents at +0x00..+0x14 and the three plane pointers at +0x18..+0x20.
#pragma once
#ifndef BFME2_VP6_FRAME_BUFFER_H
#define BFME2_VP6_FRAME_BUFFER_H

#include <stddef.h>

struct YUV_BUFFER_CONFIG
{
	int m_yWidth;				// +0x00
	int m_yHeight;				// +0x04
	int m_yStride;				// +0x08
	int m_uvWidth;				// +0x0C
	int m_uvHeight;				// +0x10
	int m_uvStride;				// +0x14
	unsigned char *m_yBuffer;	// +0x18
	unsigned char *m_uBuffer;	// +0x1C
	unsigned char *m_vBuffer;	// +0x20
};

typedef char YUV_BUFFER_CONFIG_size[(sizeof(YUV_BUFFER_CONFIG) == 0x24) ? 1 : -1];

#endif
