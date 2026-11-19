// Transport.h: the UDP transport under the connection manager (Zero Hour's Transport; names are theirs by
// role, offsets are retail's; WorldBuilder's Transport.cpp supplies the class identity).
// Target facts (code RVAs):
//  - SIZE 0x41148. Constructor 0x004D4AF2: byte +0x40E00 = 1, dword +0x40E04 = 0, byte +0x40E08 = 0, the eight
//    per-slot socket entries at +0x40E0C (12 bytes each; the vector constructor iterator over the entry
//    constructor 0x004D4A80), ints +0x40E6C and +0x40E70 = 0, the word +0x41144 = 0, both 128-message rings
//    cleared with 0x004D4A59 (out ring at +0, in ring at +0x20700), and six 30-int statistics arrays at
//    +0x40E74 + n * 0x78 zeroed.
//  - allowBroadcasts 0x004D5112 forwards to the first entry's socket. RemoveSocketForSlot 0x004D5133 (slot < 8)
//    destroys the slot's socket unless another slot shares it and clears the entry. setSlotSocket 0x004D51A7 and
//    setDestAddrToSocket 0x004D51ED store the slot's address (two dwords) at +4/+8 of its entry.
//  - The six getters 0x004D4F7A .. 0x004D50CE average one statistics array over the 29 slots other than the
//    current one (+0x40E6C) and scale by the constant at VA 0x00C601E0 (their names are not evidenced, the map
//    keeps the placeholders): arrays 0, 3, 2, 5, 1, 4 in row order.
#pragma once
#ifndef BFME2_TRANSPORT_H
#define BFME2_TRANSPORT_H

#include "Common/GameType.h"
#include "GameNetwork/TransportMessage.h"
#include "GameNetwork/NetPacketAddress.h"

class UDP;
class AsciiString;
class Rva00594DC0;
struct TransportAddress
{
	TransportAddress() {}
	TransportAddress(UnsignedInt ip, UnsignedShort port) : m_ip(ip), m_port(port) {}
	UnsignedInt m_ip;
	UnsignedShort m_port;
};

class Transport
{
public:
	Transport();											// 0x004D4AF2
	~Transport();
	Bool init(const TransportAddress *address);				// 0x004D5219
	Bool rva004D53B5(void *address);						// 0x004D53B5 (init without a socket: starts winsock, records the address and clears both rings and the statistics; the map has no better name)
	void Rva004D5496();										// 0x004D5496 (called once after the constructor; the map has no better name)
	Bool init(AsciiString ip, UnsignedShort port);			// 0x004D5534
	Bool doRecv(Rva00594DC0 *receiver);						// 0x004D4D08
	Bool update(Rva00594DC0 *receiver);						// 0x004D54C1
	Bool doSend();											// 0x004D4BA7
	Bool queueSend(NetPacketAddress *address, const UnsignedByte *data, Int length);	// 0x004D4EC6
	Bool allowBroadcasts(Bool allow);						// 0x004D5112
	void RemoveSocketForSlot(UnsignedShort slot);			// 0x004D5133
	void setSlotSocket(void *socket, UnsignedShort slot, Int *address);	// 0x004D51A7
	void setDestAddrToSocket(Int slot, void *address);		// 0x004D51ED
	Real getIncomingBytesPerSecond() const;
	Real getIncomingPacketsPerSecond() const;
	Real getOutgoingBytesPerSecond() const;
	Real getOutgoingPacketsPerSecond() const;
	Real getUnknownBytesPerSecond() const;
	Real getUnknownPacketsPerSecond() const;						// 0x004D50CE

	// One socket and destination per player slot (12 bytes; the constructor 0x004D4A80 zeroes it).
	struct SocketEntry
	{
		SocketEntry();
		~SocketEntry() {}
		UDP *m_udp;							// +0
		NetPacketAddress m_address;			// +4
	};

	TransportMessage m_outBuffer[128];						// +0x00000
	TransportMessage m_inBuffer[128];						// +0x20700
	Bool m_unk0x40E00;										// +0x40E00 (1 after the constructor)
	unsigned char m_pad40E01[3];
	UnsignedInt m_localIP;									// +0x40E04 (init 0x004D5219 fills it through UDP::getLocalAddr; the constructor clears it)
	Bool m_unk0x40E08;										// +0x40E08
	unsigned char m_pad40E09[3];
	SocketEntry m_sockets[8];								// +0x40E0C
	Int m_statisticsSlot;									// +0x40E6C
	Int m_unk0x40E70;										// +0x40E70
	UnsignedInt m_stats[6][30];										// +0x40E74 six 0x78-byte arrays
	Int m_unk0x41144;										// +0x41144
};
// SIZE 0x41148
typedef char TransportSize[(sizeof(Transport) == 0x41148) ? 1 : -1];

#endif
