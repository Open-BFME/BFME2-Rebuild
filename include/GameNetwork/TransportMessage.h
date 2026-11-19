// TransportMessage.h: one datagram in a Transport's ring (Zero Hour's TransportMessage).
// Target facts (code RVAs): 0x40E bytes, packed: 0x004D4A59 (its clearer, mapped under Transport) zeroes the crc
// dword +0, 0x100 dwords of data from +4, the length +0x404, the address dword +0x408 and the port word +0x40C.
// Transport's constructor clears the 128-entry out ring at +0 and the in ring at +0x20700 with it.
#pragma once
#ifndef BFME2_TRANSPORTMESSAGE_H
#define BFME2_TRANSPORTMESSAGE_H

#include "Common/GameType.h"

#pragma pack(push, 1)
struct TransportMessage
{
	void clear();					// 0x004D4A59 (zeroes the whole message; the map holds it as Transport::clearBuffer_Rva004D4A59)

	UnsignedInt m_crc;				// +0x000
	UnsignedByte m_data[0x400];		// +0x004
	Int m_length;					// +0x404
	UnsignedInt m_addr;				// +0x408
	UnsignedShort m_port;			// +0x40C
};
#pragma pack(pop)
// SIZE 0x40E
typedef char TransportMessageSize[(sizeof(TransportMessage) == 0x40E) ? 1 : -1];

#endif
