// MixerFieldEdit.h: the shared-memory record the INI "mixer" tool writes a field edit into.
// Target facts: Rva0002CEB5ApplyFields 0x0002CEB5 reads the token count at +0, the field name at +0x24 and the
// tokens (0x20 bytes each, last first) from +0x44; the name of the edited object sits between at +0x04.
// The mapping is named "_mappedMixerFile" (0x0002CF86).
#pragma once
#ifndef BFME2_MIXERFIELDEDIT_H
#define BFME2_MIXERFIELDEDIT_H

struct MixerFieldEdit
{
	int m_tokenCount;			// +0x00
	char m_objectName[0x20];	// +0x04
	char m_fieldName[0x20];		// +0x24
	char m_tokens[1][0x20];		// +0x44, m_tokenCount entries
};

#endif
