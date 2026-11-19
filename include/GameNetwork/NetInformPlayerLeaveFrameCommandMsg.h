// NetInformPlayerLeaveFrameCommandMsg.h: the frame a leaving player's departure takes effect on (the map names the
// setters under a BFME-prefixed class; the constructor is a placeholder).
// Target facts (code RVAs): SIZE 0x24 (NetPacket::readInformPlayerLeaveFrameMessage 0x0058E367 allocates 0x24). The
// constructor 0x004D59D1 runs NetCommandMsg's, zeroes the dword +0x1C, sets +0x20 to -1, and stores its own table
// 0x00C60274 and the command type 8. setLeavingPlayerID 0x00317B9B writes +0x20; setLeaveFrame 0x004D59FF writes +0x1C
// (and, as retail's body does, stores the command type 8); setLeaveInfo 0x004D59EE writes both.
#pragma once
#ifndef BFME2_NETINFORMPLAYERLEAVEFRAMECOMMANDMSG_H
#define BFME2_NETINFORMPLAYERLEAVEFRAMECOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetInformPlayerLeaveFrameCommandMsg : public NetCommandMsg
{
public:
	NetInformPlayerLeaveFrameCommandMsg();						// 0x004D59D1
	virtual AsciiString getContentsAsAsciiString();				// slot 3, 0x004D634E (this class has its own table 0x00C60274)
	void setLeaveInfo(Int leavingPlayerID, UnsignedInt leaveFrame);	// 0x004D59EE
	void setLeaveFrame(UnsignedInt leaveFrame);					// 0x004D59FF
	void setLeavingPlayerID(Int leavingPlayerID);				// 0x00317B9B
	Int getLeavingPlayerID();									// 0x0030D377 (folded dword getter at +0x20)
	UnsignedInt getLeaveFrame();								// 0x0030F2C7 (folded dword getter at +0x1C)

	UnsignedInt m_leaveFrame;									// +0x1C
	Int m_leavingPlayerID;										// +0x20
};
// SIZE 0x24
typedef char NetInformPlayerLeaveFrameCommandMsgSize[(sizeof(NetInformPlayerLeaveFrameCommandMsg) == 0x24) ? 1 : -1];

#endif
