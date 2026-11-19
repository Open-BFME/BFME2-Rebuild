// DRAFT (h05 fpgen): EyeTower from retail's field-parse table (VA 0x00C37668+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x14; bases and vptr, if any, are the leading gap.
// Unresolved members: none.
#pragma once
#ifndef BFME2_EYE_TOWER_H
#define BFME2_EYE_TOWER_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"


class EyeTower
{
public:

	UnsignedByte m_unk0x0[0x4];
	AsciiString m_pupilAnimObject;                         // +0x4 PupilAnimObject
	AsciiString m_pupilBeamAnimObject;                     // +0x8 PupilBeamAnimObject
	AsciiString m_eyeDecalAnimObject;                      // +0xC EyeDecalAnimObject
	AsciiString m_eyeDecalBeamAnimObject;                  // +0x10 EyeDecalBeamAnimObject
};
typedef char EyeTower_off_m_pupilAnimObject[(offsetof(EyeTower, m_pupilAnimObject) == 0x4) ? 1 : -1];
typedef char EyeTower_off_m_pupilBeamAnimObject[(offsetof(EyeTower, m_pupilBeamAnimObject) == 0x8) ? 1 : -1];
typedef char EyeTower_off_m_eyeDecalAnimObject[(offsetof(EyeTower, m_eyeDecalAnimObject) == 0xC) ? 1 : -1];
typedef char EyeTower_off_m_eyeDecalBeamAnimObject[(offsetof(EyeTower, m_eyeDecalBeamAnimObject) == 0x10) ? 1 : -1];

#endif
