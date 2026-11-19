// DRAFT (h05 fpgen): AudioLOD from retail's field-parse table (VA 0x00BE34B8+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x6; bases and vptr, if any, are the leading gap.
// Unresolved members: none.
#pragma once
#ifndef BFME2_AUDIO_L_O_D_H
#define BFME2_AUDIO_L_O_D_H

#include <stddef.h>
#include "Common/GameType.h"


class AudioLOD
{
public:

	Int m_maximumAmbientStreams;                           // +0x0 MaximumAmbientStreams
	Bool m_allowDolby;                                     // +0x4 AllowDolby
	Bool m_allowReverb;                                    // +0x5 AllowReverb
};
typedef char AudioLOD_off_m_maximumAmbientStreams[(offsetof(AudioLOD, m_maximumAmbientStreams) == 0x0) ? 1 : -1];
typedef char AudioLOD_off_m_allowDolby[(offsetof(AudioLOD, m_allowDolby) == 0x4) ? 1 : -1];
typedef char AudioLOD_off_m_allowReverb[(offsetof(AudioLOD, m_allowReverb) == 0x5) ? 1 : -1];

#endif
