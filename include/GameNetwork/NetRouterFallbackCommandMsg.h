// NetRouterFallbackCommandMsg.h: the packet router's fallback plan, the eight player slots in order (Zero Hour's
// NetRouterFallbackCommandMsg by role; retail's map spells its setter BFMENetRouterFallbackCommandMsg).
// Target facts (code RVAs): SIZE 0x3C (the allocation at 0x0058DD98 pushes 0x3C). The constructor 0x004CEEE8 runs
// NetCommandMsg's, stores the table and the command type 0x17 and clears the eight slots; setPlayerOrder 0x004D577B copies
// eight dwords into +0x1C..+0x3B. NetPacket::readRouterFallbackMessage 0x0058DD89 reads eight bytes (one per slot).
#pragma once
#ifndef BFME2_NETROUTERFALLBACKCOMMANDMSG_H
#define BFME2_NETROUTERFALLBACKCOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetRouterFallbackCommandMsg : public NetCommandMsg
{
public:
	NetRouterFallbackCommandMsg();								// 0x004CEEE8
	void setPlayerOrder(const Int *order);						// 0x004D577B

	Int m_playerOrder[8];										// +0x1C
};
// SIZE 0x3C
typedef char NetRouterFallbackCommandMsgSize[(sizeof(NetRouterFallbackCommandMsg) == 0x3C) ? 1 : -1];

#endif
