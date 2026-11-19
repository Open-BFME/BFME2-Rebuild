// NetDisconnectFrameCommandMsg.h: the frame a player disconnected on (Zero Hour's NetDisconnectFrameCommandMsg by role).
// The class is identified by retail's own routing: DisconnectManager::processDisconnectCommand 0x004D46AF sends command type
// 0x1C to processDisconnectFrame; the map does not name it.
// Target facts (code RVAs): SIZE 0x20. The constructor 0x004D59B8 runs NetCommandMsg's, stores the shared table 0x00C60244
// and the command type 0x1C, and zeroes the dword +0x1C. NetPacket::readDisconnectFrameMessage 0x0058E511 reads one dword
// into it through the shared dword setter 0x00318B09.
#pragma once
#ifndef BFME2_NETDISCONNECTFRAMECOMMANDMSG_H
#define BFME2_NETDISCONNECTFRAMECOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetDisconnectFrameCommandMsg : public NetCommandMsg
{
public:
	NetDisconnectFrameCommandMsg();								// 0x004D59B8
	void setDisconnectFrame(UnsignedInt frame);					// 0x00318B09 (folded dword setter at +0x1C)
	UnsignedInt getDisconnectFrame();							// 0x0030F2C7 (folded dword getter at +0x1C)

	UnsignedInt m_disconnectFrame;								// +0x1C
};
// SIZE 0x20
typedef char NetDisconnectFrameCommandMsgSize[(sizeof(NetDisconnectFrameCommandMsg) == 0x20) ? 1 : -1];

#endif
