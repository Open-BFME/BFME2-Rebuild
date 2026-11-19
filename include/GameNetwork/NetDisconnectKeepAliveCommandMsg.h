// NetDisconnectKeepAliveCommandMsg.h: the keep-alive sent while the disconnect screen is up (Zero Hour's
// NetDisconnectKeepAliveCommandMsg; the map names it).
// Target facts (code RVAs): SIZE 0x1C (DisconnectManager::sendKeepAlive 0x004D3942 allocates 0x1C). The constructor
// 0x004D57DC runs NetCommandMsg's, stores the compiler's four-slot table (VA 0x00C60244) and the command type 0x19.
#pragma once
#ifndef BFME2_NETDISCONNECTKEEPALIVECOMMANDMSG_H
#define BFME2_NETDISCONNECTKEEPALIVECOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetDisconnectKeepAliveCommandMsg : public NetCommandMsg
{
public:
	NetDisconnectKeepAliveCommandMsg();					// 0x004D57DC
};
// SIZE 0x1C
typedef char NetDisconnectKeepAliveCommandMsgSize[(sizeof(NetDisconnectKeepAliveCommandMsg) == 0x1C) ? 1 : -1];

#endif
