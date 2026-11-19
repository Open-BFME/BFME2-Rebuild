// Rva003ECD60Object.h: the 0x56C-byte threat record ThreatFinderUpdate allocates
// in xfer and mapLoadPostProcess and registers by name.
// Target facts (old repo ThreatFinderUpdate.cpp, Rva003ECD60Ctor.cpp): twenty
// 0x44-byte ThreatInfo entries, the AsciiString name at +0x550, four floats
// +0x554..+0x560, an int +0x564 and a bool +0x568; the constructor 0x003ECD60
// zeroes the position, the radius float and the int, and stores the flag.
// Rva003ECDB7Object is the prefix view the dtor and manager helpers use.
#pragma once
#ifndef BFME2_RVA003ECD60OBJECT_H
#define BFME2_RVA003ECD60OBJECT_H

#include "Common/AsciiString.h"
#include "Common/GameType.h"

class Xfer;

// One 0x44-byte threat record: seventeen floats, transferred by the helper at
// 0x003ECAE0 (old repo ThreatFinderUpdate.cpp).
struct ThreatInfo
{
	ThreatInfo();
	void rva003ECAE0(Xfer *xfer);
	ThreatInfo *rva003ECA81(const ThreatInfo *other);
	void rva003ECB52(void *arg);

	float m_values[17];
};
// SIZE 0x44

class Rva003ECD60Object
{
public:
	Rva003ECD60Object(const AsciiString &name, bool flag);
	void xfer(Xfer *xfer);

	ThreatInfo m_elems[20];				// 0x000
	AsciiString m_name;					// 0x550
	float m_f554;						// 0x554
	float m_f558;						// 0x558
	float m_f55C;						// 0x55C
	float m_radius;						// 0x560
	int m_564;							// 0x564
	bool m_568;							// 0x568
};
// SIZE 0x56C

#endif
