// NetPacketAddress.h: an IP address and port (Zero Hour's NetPacketAddress by role).
// Target facts: 8 bytes with padding; Transport's per-slot entries copy it as two dwords (setSlotSocket 0x004D51A7,
// setDestAddrToSocket 0x004D51ED), and RemoveSocketForSlot 0x004D5133 resets one through a value-initialised copy.
#pragma once
#ifndef BFME2_NETPACKETADDRESS_H
#define BFME2_NETPACKETADDRESS_H

#include "Common/GameType.h"

struct NetPacketAddress
{
	NetPacketAddress() : m_ip(0), m_port(0) {}
	NetPacketAddress(UnsignedInt ip, UnsignedShort port) : m_ip(ip), m_port(port) {}
	UnsignedInt m_ip;			// +0
	UnsignedShort m_port;		// +4
};
// SIZE 8

#endif
