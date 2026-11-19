// OpacityEnvelope.h: the five-phase opacity envelope that INI::Rva00338E52_ParseEnvelope 0x00338E52 fills.
// SIZE 0x28. Target facts: the parser zeroes +0x0C and +0x24 first, then reads tokens until "End":
// InitialOpacity, PeakOpacity and SustainOpacity into the three floats at +0x00/+0x04/+0x08 (scanReal), and
// InitialDelay, AttackTime, DecayTime, SustainTime, ReleaseTime into +0x10/+0x14/+0x18/+0x1C/+0x20
// (milliseconds scaled to logic frames and rounded up, the shape of parseDurationUnsignedInt 0x00338B30).
// FadeAndDieOrnamentUpdateModuleData keeps one at +0x0C ("Envelope") and the FollowTarget field reuses
// the same parser at +0x34 (Draft/FadeAndDieOrnamentUpdateModuleData.h); W3DLaserDrawModuleData keeps one at +0x4C.
#pragma once
#ifndef BFME2_OPACITYENVELOPE_H
#define BFME2_OPACITYENVELOPE_H

struct OpacityEnvelope
{
	float m_initialOpacity;				// 0x00 InitialOpacity
	float m_peakOpacity;				// 0x04 PeakOpacity
	float m_sustainOpacity;				// 0x08 SustainOpacity
	int m_unk0x0C;						// 0x0C zeroed by the parser
	unsigned int m_initialDelay;		// 0x10 InitialDelay, in frames
	unsigned int m_attackTime;			// 0x14 AttackTime, in frames
	unsigned int m_decayTime;			// 0x18 DecayTime, in frames
	unsigned int m_sustainTime;			// 0x1C SustainTime, in frames
	unsigned int m_releaseTime;			// 0x20 ReleaseTime, in frames
	int m_state;						// 0x24 zeroed by the parser
};
typedef char OpacityEnvelope_size[(sizeof(OpacityEnvelope) == 0x28) ? 1 : -1];

#endif
