// NetProgressCommandMsg.h: a level-load progress message (Zero Hour's NetProgressCommandMsg by role; offsets are
// retail's).
// Target facts (code RVAs, VA 0x00C60244 table):
//  - SIZE 0x20: NetCommandMsg (0x1C) plus the percentage byte +0x1C. The constructor 0x004D582B runs
//    NetCommandMsg's, stores the table, the command type 0xF and a zero percentage. The table is the compiler's
//    four-slot copy of the base's (deleting destructor 0x004D5B30), shared by the other message types that add
//    nothing virtual.
//  - getPercentage 0x004C54EC is an out-of-line byte getter: processProgress 0x004CF2F5 calls it.
#pragma once
#ifndef BFME2_NETPROGRESSCOMMANDMSG_H
#define BFME2_NETPROGRESSCOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetProgressCommandMsg : public NetCommandMsg
{
public:
	NetProgressCommandMsg();							// 0x004D582B

	UnsignedByte getPercentage();					// 0x004C54EC
	void setPercentage(UnsignedByte percentage);		// 0x0046EDE3 (folded with NetDisconnectPlayerCommandMsg::setDisconnectSlot)

	UnsignedByte m_percentage;							// +0x1C
	unsigned char m_pad1D[3];							// +0x1D
};
// SIZE 0x20

#endif
