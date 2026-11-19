// NetFrameCommandMsg.h: the per-frame summary message (Zero Hour's NetFrameCommandMsg by role; offsets are retail's).
// Target facts (code RVAs, VA 0x00C60140 table):
//  - SIZE 0x28 (NetPacket::readFrameMessage 0x0058DCEB allocates 0x28): NetCommandMsg plus three dwords +0x1C, +0x20,
//    +0x24. The constructor 0x004CEEC3 zeroes +0x1C/+0x20, sets +0x24 to -1, stores the table and the command type 3.
//    readFrameMessage reads the three dwords from the packet in that order. The table is the compiler's (deleting
//    destructor 0x004D5B30) with its own slot 3, getContentsAsAsciiString 0x004D5C2B.
#pragma once
#ifndef BFME2_NETFRAMECOMMANDMSG_H
#define BFME2_NETFRAMECOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetFrameCommandMsg : public NetCommandMsg
{
public:
	NetFrameCommandMsg();								// 0x004CEEC3

	virtual AsciiString getContentsAsAsciiString();		// slot 3, 0x004D5C2B

	UnsignedInt getLogicFrame() const { return m_unk0x1C; }
	UnsignedInt getClientFrame() const { return m_unk0x20; }
	UnsignedInt getTotalCommands() const { return m_unk0x24; }

	UnsignedInt m_unk0x1C;								// +0x1C
	UnsignedInt m_unk0x20;								// +0x20
	UnsignedInt m_unk0x24;								// +0x24 (-1 after the constructor)
};
// SIZE 0x28
typedef char NetFrameCommandMsgSize[(sizeof(NetFrameCommandMsg) == 0x28) ? 1 : -1];

#endif
