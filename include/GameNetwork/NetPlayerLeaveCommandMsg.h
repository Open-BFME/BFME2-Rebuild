// NetPlayerLeaveCommandMsg.h: "this slot has left" (Zero Hour's NetPlayerLeaveCommandMsg by role; the map names only its
// constructor, as a placeholder).
// Target facts (code RVAs): SIZE 0x20 (the allocation in readPlayerLeaveMessage pushes 0x20). The constructor 0x004D5795
// runs NetCommandMsg's, stores its own table 0x00C60224 and the command type 0xA, and zeroes the byte +0x1C.
// NetPacket::readPlayerLeaveMessage 0x0058DDF2 reads one byte into it through the shared byte setter 0x0046EDE3.
#pragma once
#ifndef BFME2_NETPLAYERLEAVECOMMANDMSG_H
#define BFME2_NETPLAYERLEAVECOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetPlayerLeaveCommandMsg : public NetCommandMsg
{
public:
	NetPlayerLeaveCommandMsg();									// 0x004D5795
	virtual AsciiString getContentsAsAsciiString();				// slot 3, 0x004D5FB4 (this class has its own table 0x00C60224)
	void setLeavingPlayerID(UnsignedByte slot);					// 0x0046EDE3 (folded byte setter)
	UnsignedByte getLeavingPlayerID();							// 0x004C54EC (folded byte getter at +0x1C)

	UnsignedByte m_leavingPlayerID;								// +0x1C
	unsigned char m_pad1D[3];									// +0x1D
};
// SIZE 0x20
typedef char NetPlayerLeaveCommandMsgSize[(sizeof(NetPlayerLeaveCommandMsg) == 0x20) ? 1 : -1];

#endif
