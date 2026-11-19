// NetDisconnectVoteCommandMsg.h: one player's vote to drop another (Zero Hour's NetDisconnectVoteCommandMsg by role; the
// class is identified by retail's own routing and is not named in the map).
// Target facts (code RVAs): DisconnectManager::processDisconnectCommand 0x004D46AF routes command type 0x1B to
// processDisconnectVote 0x004D3F74, which reads from the message the voted-for slot (the byte at +0x1C, through the shared
// byte getter 0x004C54EC) and the vote frame (the dword at +0x20, through the shared dword getter 0x0030D377). The
// constructor is 0x004D580E (type 0x1B, byte +0x1C and dword +0x20 zero), table 0x00C60244; DisconnectManager::sendVoteCommand
// 0x004D3A1B builds one. SIZE 0x24: NetPacket::readDisconnectVoteMessage 0x0058DFB8 allocates 0x24 and fills the byte through
// the shared byte setter 0x0046EDE3 and the dword through the shared dword setter 0x00317B9B.
#pragma once
#ifndef BFME2_NETDISCONNECTVOTECOMMANDMSG_H
#define BFME2_NETDISCONNECTVOTECOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetDisconnectVoteCommandMsg : public NetCommandMsg
{
public:
	NetDisconnectVoteCommandMsg();								// 0x004D580E
	void setSlot(UnsignedByte slot);							// 0x0046EDE3 (folded byte setter)
	void setVoteFrame(UnsignedInt frame);						// 0x00317B9B (folded dword setter at +0x20)
	UnsignedByte getSlot();										// folded byte getter 0x004C54EC
	UnsignedInt getVoteFrame();									// folded dword getter 0x0030D377

	UnsignedByte m_slot;										// +0x1C
	unsigned char m_pad1D[3];									// +0x1D
	UnsignedInt m_voteFrame;									// +0x20
};
// SIZE 0x24

#endif
