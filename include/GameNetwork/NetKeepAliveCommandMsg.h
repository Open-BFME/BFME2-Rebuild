// NetKeepAliveCommandMsg.h: the periodic "still here" message (Zero Hour's NetKeepAliveCommandMsg; the map names it).
// Target facts (code RVAs): SIZE 0x1C (ConnectionManager::sendKeepAliveCommand 0x004CF828 allocates 0x1C). The constructor
// 0x004D57C7 runs NetCommandMsg's, stores the compiler's four-slot table (VA 0x00C60244) and the command type 0xC.
#pragma once
#ifndef BFME2_NETKEEPALIVECOMMANDMSG_H
#define BFME2_NETKEEPALIVECOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetKeepAliveCommandMsg : public NetCommandMsg
{
public:
	NetKeepAliveCommandMsg();							// 0x004D57C7
};
// SIZE 0x1C
typedef char NetKeepAliveCommandMsgSize[(sizeof(NetKeepAliveCommandMsg) == 0x1C) ? 1 : -1];

#endif
