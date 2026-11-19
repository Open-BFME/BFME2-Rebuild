// NetRequestPlayerLeaveCommandMsg.h: a player asks to leave (ConnectionManager::requestLeave sends one; the map names only
// the reader, and the constructor is a placeholder).
// Target facts (code RVAs): SIZE 0x24 (NetPacket::readRequestPlayerLeaveMessage 0x0058E3F4 allocates 0x24). The constructor
// 0x004D5A10 runs NetCommandMsg's, sets the dword +0x1C to -1 and +0x20 to 1, and stores its own table 0x00C60284 and the
// command type 7. The reader fills +0x1C through the shared dword setter 0x00318B09 and +0x20 through 0x00317B9B. The meaning
// of the two fields is not evidenced beyond their defaults.
#pragma once
#ifndef BFME2_NETREQUESTPLAYERLEAVECOMMANDMSG_H
#define BFME2_NETREQUESTPLAYERLEAVECOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetRequestPlayerLeaveCommandMsg : public NetCommandMsg
{
public:
	NetRequestPlayerLeaveCommandMsg();							// 0x004D5A10
	virtual AsciiString getContentsAsAsciiString();				// slot 3, 0x004D63DB (this class has its own table 0x00C60284)
	void setFirst(Int value);									// 0x00318B09 (folded dword setter at +0x1C)
	void setSecond(Int value);									// 0x00317B9B (folded dword setter at +0x20)

	Int m_first;												// +0x1C
	Int m_second;												// +0x20
};
// SIZE 0x24
typedef char NetRequestPlayerLeaveCommandMsgSize[(sizeof(NetRequestPlayerLeaveCommandMsg) == 0x24) ? 1 : -1];

#endif
