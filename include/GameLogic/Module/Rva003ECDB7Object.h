// Rva003ECDB7Object.h: the 0x554-byte heap record ThreatFinderUpdate keeps at
// +0x20 and registers with the threat-finder manager by name.
// Target facts (old repo ThreatFinderUpdate.cpp, Rva003ECDB7Unregister.cpp):
// the record is 20 0x44-byte ThreatInfo entries plus an AsciiString name at
// +0x550; ??1Rva003ECDB7Object 0x003ECDB7 unregisters the name when it is not
// the shared empty string and then destroys it, and registerName 0x003ECE09
// registers it the same way. Both reach the manager through TheThreatFinderManager.
#pragma once
#ifndef BFME2_RVA003ECDB7OBJECT_H
#define BFME2_RVA003ECDB7OBJECT_H

#include "Common/AsciiString.h"
#include "Common/GameType.h"

class Xfer;

class ThreatFinderManager
{
public:
	void remove(const AsciiString &name, void *object);
	void add(const AsciiString &name, void *object);
};

extern ThreatFinderManager *TheThreatFinderManager;

class Rva003ECDB7Object
{
public:
	~Rva003ECDB7Object();
	void registerName();

private:
	char m_pad00[0x550];

protected:
	AsciiString m_name;					// 0x550
};
// SIZE 0x554

#endif
