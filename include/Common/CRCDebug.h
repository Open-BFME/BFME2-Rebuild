// CRCDebug.h: the logic random log, the object retail keeps in theLogicRandomLogFile.
//
// GameEngine::init allocates it with operator new and stores it there only when the deep
// or lite CRC switch is set (TheDeepCRC, TheLiteCRC); otherwise the pointer stays null, so
// every writer tests it first. The object is a virtual table pointer and a vector of
// formatted lines; its xfer writes the lines in a block labelled "CRCParameterCheck".
// The class keeps its placeholder name until that label or another witness settles it.
#pragma once
#ifndef BFME2_CRCDEBUG_H
#define BFME2_CRCDEBUG_H

#include <vector>
#include "Common/AsciiString.h"

class Rva002CEC0A
{
public:
	Rva002CEC0A();
	virtual ~Rva002CEC0A();

	// Formats the arguments with _vsnprintf into a 1000-character buffer and appends the
	// text to m_lines. A null format adds nothing. The callers are the logic random draws,
	// ThingFactory::newObject and the AI desync traces.
	void addLine(const char *format, ...);

protected:
	_STL::vector<AsciiString> m_lines;	// +0x04
};

// Defined in CRCDebug.cpp; null unless CRC logging is on.
extern Rva002CEC0A *theLogicRandomLogFile;

#endif
