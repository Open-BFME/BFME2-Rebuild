// FirewallHelper.h: NAT/firewall behaviour detection against the manglers (Zero Hour's FirewallHelperClass by role;
// offsets are retail's). Only the members the written rows prove are named.
// Target facts (code RVAs, VA 0x00C70A2C table):
//  - SIZE 0x190. One virtual slot, the destructor (0x00595D0B deleting). The constructor 0x00594CDD stores the table,
//    zeroes the words +4..+0x10, the eight spare-socket entries at +0x14 (8 bytes each: a UDP pointer and a port word
//    at +0x18), four dwords at +0x54, the words at +0x68 and +0x78 (eight each), the mangler message slots at +0x8A
//    (eight of 0x1E bytes; a slot is free when its dword at +0x14 is zero), and the dwords +0x17C..+0x18C, then seeds
//    the static source port pool with 0x1000 + (timeGetTime() / 1000) % 1000.
//  - findSpareSocketByPort 0x00594D77 returns the entry whose port matches, findEmptyMessage 0x00594D9C the first free
//    message slot (null when all eight are used), openSpareSocket 0x00595089 binds a new UDP to the port in the first
//    free entry. The free functions formatIPPort 0x00595C04, formatAddress 0x00595C79 and formatIP 0x00595C9B print
//    "%d.%d.%d.%d:%d" and "%d.%d.%d.%d" from a host-order address.
#pragma once
#ifndef BFME2_FIREWALLHELPER_H
#define BFME2_FIREWALLHELPER_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"

class UDP;

// Packed to 2 bytes: retail lays the eight of them out at 0x1E bytes each, with the dword at +0x14 unaligned.
#pragma pack(push, 2)
struct ManglerMessage
{
	UnsignedInt m_crc;										// +0x00 (network order on the wire; the CRC of the next 0x10 bytes)
	UnsignedShort m_magic;									// +0x04 (0xF00E in a request)
	UnsignedShort m_packetID;								// +0x06 (matched against getManglerResponse's port)
	UnsignedShort m_response;								// +0x08 (what getManglerResponse returns: the mangled source port)
	UnsignedShort m_originalPort;							// +0x0A (the sender's own source port)
	UnsignedByte m_mangledAddress[4];						// +0x0C
	UnsignedByte m_commandType;								// +0x10 (0xC in a request)
	UnsignedByte m_blitzMe;									// +0x11
	UnsignedShort m_padding;								// +0x12
	UnsignedInt m_inUse;									// +0x14 (zero = free; the byte count of a received message while it waits)
	UnsignedByte m_unk0x18[6];								// +0x18
};
#pragma pack(pop)
// SIZE 0x1E

struct NetworkAddress
{
	UnsignedInt m_ip;										// +0
	UnsignedShort m_port;									// +4
};

struct SpareSocket
{
	UDP *m_udp;												// +0
	UnsignedShort m_port;									// +4
	unsigned char m_pad06[2];								// +6
};
// SIZE 8

AsciiString formatIPPort(UnsignedInt ip, UnsignedShort port);			// 0x00595C04
AsciiString formatAddress(const NetworkAddress *address);				// 0x00595C79
AsciiString formatIP(UnsignedInt ip);									// 0x00595C9B

class FirewallHelperClass;
FirewallHelperClass *createFirewallHelper();				// 0x00595143 (Zero Hour's name: allocates 0x190 bytes and constructs; the map holds it as Rva00595143Get)

class FirewallHelperClass
{
public:
	FirewallHelperClass();									// 0x00594CDD
	virtual ~FirewallHelperClass();							// slot 0, 0x00595D0B (deleting)

	void *findSpareSocketByPort(UnsignedShort port);		// 0x00594D77 (returns the entry; the map types it void *)
	Bool openSpareSocket(UnsignedShort port);				// 0x00595089
	UnsignedShort getNextTemporarySourcePort(Int step);		// 0x0059539B
	UnsignedShort getManglerResponse(UnsignedShort port, Int wait);	// 0x00594E07
	Bool behaviorDetectionUpdate();							// 0x00595D2D
	Bool detectionBeginUpdate();							// 0x00595452
	Bool detectionTest1Update();							// 0x00595685
	Bool detectionTest2Update();							// 0x0059573C
	Bool detectionTest3Update();							// 0x005957D5
	Bool detectionTest3WaitForResponsesUpdate();			// 0x005958AC
	Int getNATPortAllocationScheme(Int numPorts, UnsignedShort *originalPorts, UnsignedShort *mangledPorts, Int &relativeDelta, Bool &looksGood);	// 0x00594F19 (Zero Hour's name; the map holds it under the placeholder class Rva00594F19)
	Bool detectionTest4Stage1Update();						// 0x00595A9F
	Bool detectionTest4Stage2Update();						// 0x00595B97
	static void getManglerName(Int manglerIndex, char *name);	// 0x005953F4
	void flagNeedToRefresh(Bool refresh);							// 0x00595F80
	Bool readFirewallBehavior();							// 0x00595D95 (Zero Hour's readFirewallBehavior with a result: a stored "needs refresh" flag resets the behavior; false restarts detection; the map holds it under the placeholder class Rva00595D95)
	Int getBehaviorAndRestart();							// 0x00594EFC (clears the detection state and returns the behavior: the map holds it on a placeholder class)
	UnsignedShort getSourcePortAllocationDelta() const;		// 0x00594F07 (the word at +0xC; Zero Hour's name)
	void reset();											// 0x0059515C (the destructor's tail call; Zero Hour's name)
	void closeAllSpareSockets();							// 0x0059510D
	void rva00594EEF();										// 0x00594EEF (back to a simple firewall, detection restarts: the map has no better name)
	UnsignedByte rva00594F0C();								// 0x00594F0C (detection finished; returns 1)
	Bool rva00595213(UDP *udp, unsigned long mangler, UnsignedShort firstPort, UnsignedShort secondPort, Bool flag);	// 0x00595213 (sendToManglerFromPort over an open socket)
	Bool rva005952C4(void *message, UnsignedShort length, UnsignedShort *result);	// 0x005952C4 (the receiver Transport::doRecv offers every datagram first; true means it was consumed; the map holds it on the placeholder class Rva00594DC0)
	void byteAdjust(ManglerMessage *message);				// 0x00594DC0 (the map holds it under the placeholder class Rva00594DC0: converts a received message to host order)
	Bool sendToManglerFromPort(unsigned long mangler, UnsignedShort sourcePort, UnsignedShort firstPort, UnsignedShort secondPort, Bool flag);	// 0x0059517F (the map holds it under the placeholder class Rva0059517F)
	void closeSpareSocket(UnsignedShort port);				// 0x0059534A (Zero Hour's name)

private:
	ManglerMessage *findEmptyMessage();						// 0x00594D9C

	static Int m_sourcePortPool;							// VA 0x00DD31F4

	UnsignedInt m_behavior;									// +0x04
	UnsignedInt m_lastBehavior;									// +0x08
	UnsignedInt m_sourcePortAllocationDelta;									// +0x0C
	UnsignedInt m_lastSourcePortAllocationDelta;									// +0x10
	SpareSocket m_spareSockets[8];							// +0x14
	UnsignedInt m_manglers[4];								// +0x54
	UnsignedInt m_numManglers;									// +0x64
	UnsignedShort m_sparePorts[8];								// +0x68
	UnsignedShort m_mangledPorts[8];								// +0x78
	UnsignedShort m_packetID;								// +0x88
	ManglerMessage m_messages[8];							// +0x8A
	unsigned char m_pad17A[2];								// +0x17A
	UnsignedInt m_currentState;									// +0x17C
	UnsignedInt m_timeoutStart;									// +0x180
	UnsignedInt m_timeoutLength;									// +0x184
	Int m_numResponses;									// +0x188
	Int m_currentTry;											// +0x18C (signed: detectionTest3Update 0x005957D5 compares it with jge)
};
// SIZE 0x190

#endif
