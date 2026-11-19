// udp.h: a UDP socket wrapper (Zero Hour's UDP class; names are theirs by role, offsets are retail's).
// Target facts (code RVAs): SIZE 0x20 by the members the constructor 0x005948F5 writes: the socket handle +0 = -1,
// the local address +4 and port word +8 = 0, a sockaddr_in at +0xC (family word 2 at +0xC, port word +0xE,
// address dword +0x10; 16 bytes), and the last error +0x1C = 0x2710. The destructor 0x00594918 shuts the socket
// down (how = 2), drains it with recvfrom (flags 0) and closes it; it is not virtual. The socket functions are
// called through the import table (VA 0x00BBA940 recvfrom, 0x00BBA954 getsockopt, 0x00BBA958 setsockopt, 0x00BBA95C
// ioctlsocket, 0x00BBA960 shutdown, 0x00BBA980 WSAGetLastError, 0x00BBA98C htons, 0x00BBA994 closesocket,
// 0x00BBA998 and 0x00BBA9xx bind/other). GetStatus 0x00594A06 maps the last error to Zero Hour's sockStat codes.
#pragma once
#ifndef BFME2_UDP_H
#define BFME2_UDP_H

#include "Common/GameType.h"

struct sockaddr_in;

// The 16-byte Internet socket address the object keeps at +0xC (layout from retail's stores: family word 2,
// port word, address dword; the rest is zero).
struct UDPAddress
{
	UnsignedShort m_family;
	UnsignedShort m_port;
	UnsignedInt m_address;
	unsigned char m_zero[8];
};

class UDP
{
public:
	// GetStatus' result: the Winsock error translated to a small negative code; the names are the
	// WSAE constants the retail comparisons test (0 = no error, anything else comes back as the raw error).
	enum sockStat
	{
		OK = 0,
		ADDRNOTAVAIL = -7,			// the Bind-by-name failure code (Transport::update compares against -7)
		BADF = -8,					// WSAEBADF 0x2719
		CONNREFUSED = -9,			// WSAECONNREFUSED 0x274D
		INTR = -10,					// WSAEINTR 0x2714
		INPROGRESS = -3,			// WSAEINPROGRESS 0x2734
		INVAL = -14,				// WSAEINVAL 0x2726
		ISCONN = -2,				// WSAEISCONN 0x2748
		NOTSOCK = -11,				// WSAENOTSOCK 0x2736
		TIMEDOUT = -15,				// WSAETIMEDOUT 0x274C
		ALREADY = -4,				// WSAEALREADY 0x2735
		WOULDBLOCK = -13			// WSAEWOULDBLOCK 0x2733
	};

	UDP();									// 0x005948F5
	~UDP();									// 0x00594918

	Int getLocalAddr(UnsignedInt &ip, UnsignedShort &port);	// 0x0059495D
	Int Bind(UnsignedInt ip, UnsignedShort port);			// 0x00594B56
	Int Bind(const char *ip, UnsignedShort port);			// 0x00594C7F
	Int Read(UnsignedByte *msg, UnsignedInt len, sockaddr_in *from);	// 0x005949AB
	Int Write(const char *msg, Int len, unsigned long ip, UnsignedShort port);	// 0x00594C12 (the map holds it under the placeholder class Rva00594C12)
	sockStat GetStatus();									// 0x00594A06
	Int SetInputBuffer(UnsignedInt bytes);					// 0x00594A7C
	Int SetOutputBuffer(UnsignedInt bytes);					// 0x00594AA6
	Int GetInputBuffer();									// 0x00594AD0
	Int GetOutputBuffer();									// 0x00594AFF
	Int AllowBroadcasts(Bool allow);						// 0x00594B2E

private:
	Int SetBlocking(Int block);								// 0x00594976

	Int m_fd;												// +0x00
	UnsignedInt m_myIP;										// +0x04
	UnsignedShort m_myPort;									// +0x08
	unsigned char m_pad0A[2];								// +0x0A
	UDPAddress m_addr;										// +0x0C a sockaddr_in
	Int m_lastError;										// +0x1C
};
// SIZE 0x20

#endif
