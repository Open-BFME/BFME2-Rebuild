// LazyNameKey.h: the two-word lazy name-key cache SAGE keeps for INI-driven
// properties.
// Target facts (old repo ThreatFinderUpdateMapLoadPostProcess.cpp): the object is
// a key at +0 and the name pointer at +4, and get() interns the name once and
// caches the result. Retail's ThreatFinder radius cache lives at VA 0x00DBDE1C
// with key zero and the literal "objectThreatFinderRadius".
#pragma once
#ifndef BFME2_LAZYNAMEKEY_H
#define BFME2_LAZYNAMEKEY_H

#include "Common/GameType.h"

class LazyNameKey
{
public:
	NameKeyType get();

	NameKeyType m_key;					// 0x0
	const char *m_name;					// 0x4
};

// Retail VA 0x00DBDE1C: the "objectThreatFinderRadius" property key.
extern LazyNameKey g_objectThreatFinderRadiusKey;

#endif
