// VP6PlaybackInstance.h: the VP6 codec's playback state (PB_INSTANCE); only the
// members the written codec functions touch are named, the rest are gaps.
// Target facts: CopyFrame 0x001B9AC0 reads the plane base pointers at +0x78..+0x80
// and the luma and chroma strides at +0x98 / +0x9C; VP6_InitialiseConfiguration
// 0x001B9680 stores 8 at +0x1C0 and +0x1C4.
#pragma once
#ifndef BFME2_VP6_PLAYBACK_INSTANCE_H
#define BFME2_VP6_PLAYBACK_INSTANCE_H

#include <stddef.h>

struct PB_INSTANCE
{
	unsigned char m_gap0x00[0x78];
	unsigned char *m_yBuffer;	// +0x78
	unsigned char *m_uBuffer;	// +0x7C
	unsigned char *m_vBuffer;	// +0x80
	unsigned char m_gap0x84[0x14];
	int m_yStride;				// +0x98
	int m_uvStride;				// +0x9C
	unsigned char m_gap0xA0[0x120];
	int m_unk0x1C0;				// +0x1C0
	int m_unk0x1C4;				// +0x1C4
};

typedef char PB_INSTANCE_off_m_yBuffer[(offsetof(PB_INSTANCE, m_yBuffer) == 0x78) ? 1 : -1];
typedef char PB_INSTANCE_off_m_unk0x1C4[(offsetof(PB_INSTANCE, m_unk0x1C4) == 0x1C4) ? 1 : -1];

#endif
