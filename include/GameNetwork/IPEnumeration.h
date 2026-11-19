// IPEnumeration.h: the machine's IP addresses and name, found through Winsock (Zero Hour's IPEnumeration and
// EnumeratedIP by role; offsets are retail's).
// Target facts (code RVAs):
//  - EnumeratedIP SIZE 0xC (the allocation at 0x0050C0AF pushes 0xC): an AsciiString +0 (the dotted address), the
//    address in host order +4, the next node +8. The constructor 0x0050BFC4 clears the string, sets it to "" (the
//    AsciiString set from a C string), and zeroes +4 and +8. The destructor is the compiler's (releaseBuffer of the
//    string; folded at 0x0048BA39). A list is kept sorted by address (getAddresses 0x0050C02E walks
//    gethostbyname's h_addr_list, formats each entry "%d.%d.%d.%d" and inserts it in order).
//  - IPEnumeration: the list head +0 and a byte +4 (Winsock started). getMachineName 0x0050C1FD starts Winsock when it is
//    not yet (WSAStartup 2.2; a version other than 2.2 cleans up again), asks gethostname for up to 0x100 characters and
//    returns the name, or the empty string on any failure. The destructor 0x0050C283 cleans Winsock up when it started
//    it and deletes the list.
#pragma once
#ifndef BFME2_IPENUMERATION_H
#define BFME2_IPENUMERATION_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"

class EnumeratedIP
{
public:
	EnumeratedIP();												// 0x0050BFC4
	~EnumeratedIP() {}

	void setIPstring(AsciiString ip);							// 0x0050BFFD
	EnumeratedIP *getNext() const { return m_next; }

	AsciiString m_IPstring;										// +0x00
	UnsignedInt m_IP;											// +0x04
	EnumeratedIP *m_next;										// +0x08
};
// SIZE 0xC

class IPEnumeration
{
public:
	~IPEnumeration();											// 0x0050C283

	EnumeratedIP *getAddresses();								// 0x0050C02E
	AsciiString getMachineName();								// 0x0050C1FD

	EnumeratedIP *m_IPlist;										// +0x00
	Bool m_isWinsockInitialized;								// +0x04
};
// SIZE 8

#endif
