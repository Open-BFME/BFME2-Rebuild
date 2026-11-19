// NetRequestGameSpyStatsAuthKeyCommandMsg.h: a request for the GameSpy stats authorisation key (BFME II only; named after the
// map's FillBufferWithRequestGameSpyStatsAuthKeyCommand 0x005915D6, the constructor is a placeholder).
// Target facts (code RVAs): SIZE 0x20 (NetPacket::rva00592496 reader allocates 0x20). The constructor 0x004D64F5 runs
// NetCommandMsg's, stores its own table 0x00C6050C and the command type 5 and zeroes the AsciiString +0x1C. The destructor
// 0x004D65A6 (deleting 0x004D6AF1) releases it; the string setter is 0x004D65F9 and the folded getter 0x002D9BA6.
#pragma once
#ifndef BFME2_NETREQUESTGAMESPYSTATSAUTHKEYCOMMANDMSG_H
#define BFME2_NETREQUESTGAMESPYSTATSAUTHKEYCOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"
#include "Common/AsciiString.h"

class NetRequestGameSpyStatsAuthKeyCommandMsg : public NetCommandMsg
{
public:
	NetRequestGameSpyStatsAuthKeyCommandMsg();					// 0x004D64F5
	virtual ~NetRequestGameSpyStatsAuthKeyCommandMsg();			// slot 0, 0x004D65A6 (deleting 0x004D6AF1)
	virtual AsciiString getContentsAsAsciiString();				// slot 3, 0x004D650E (table 0x00C6050C)
	AsciiString getName() const;								// 0x002D9BA6 (folded)
	void setName(AsciiString name);								// 0x004D65F9

	AsciiString m_name;											// +0x1C
};
// SIZE 0x20
typedef char NetRequestGameSpyStatsAuthKeyCommandMsgSize[(sizeof(NetRequestGameSpyStatsAuthKeyCommandMsg) == 0x20) ? 1 : -1];

#endif
