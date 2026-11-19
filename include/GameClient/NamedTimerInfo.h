#pragma once
// NamedTimerInfo: one named countdown the in-game UI tracks (InGameUI::addNamedTimer 0x002A5A50).
// Evidence (retail): the constructor 0x0029B8B0 stores the vtable pointer (VA 0x00BFD02C) and clears the two
// strings; the destructor 0x0029B8DD releases the UnicodeString at +8 (releaseBuffer<G> 0x00036E70), then the
// AsciiString at +4 (0x00036410); the table holds the destructor only. The rest of the object and its SIZE are
// not recovered here.
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"

class NamedTimerInfo {
public:
	NamedTimerInfo();
	virtual ~NamedTimerInfo();

	AsciiString m_timerName;		// +4
	UnicodeString m_timerText;		// +8
};
