// NetGameSpyStatsAuthKeyCommandMsg.h: the GameSpy stats authorisation key, as two strings (BFME II only; named after the
// map's FillBufferWithGameSpyStatsAuthKeyCommand 0x00591691, the constructor is a placeholder).
// Target facts (code RVAs): SIZE 0x24 (NetPacket::rva00592520 reader allocates 0x24). The constructor 0x004D65DC runs
// NetCommandMsg's, stores its own table 0x00C60530 and the command type 6 and zeroes the AsciiStrings +0x1C and +0x20. The
// destructor 0x004D6708 (deleting 0x004D6B0D, protected) releases both; the second string's setter is 0x004D662D, the
// getters are the folded 0x002D9BA6 (+0x1C) and 0x002D9BC1 (+0x20).
#pragma once
#ifndef BFME2_NETGAMESPYSTATSAUTHKEYCOMMANDMSG_H
#define BFME2_NETGAMESPYSTATSAUTHKEYCOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"
#include "Common/AsciiString.h"

class NetGameSpyStatsAuthKeyCommandMsg : public NetCommandMsg
{
public:
	NetGameSpyStatsAuthKeyCommandMsg();							// 0x004D65DC
protected:
	virtual ~NetGameSpyStatsAuthKeyCommandMsg();				// slot 0, 0x004D6708 (deleting 0x004D6B0D)
public:
	virtual AsciiString getContentsAsAsciiString();				// slot 3, 0x004D6661 (table 0x00C60530)
	AsciiString getFirst() const;								// 0x002D9BA6 (folded)
	AsciiString getSecond() const;								// 0x002D9BC1 (folded)
	void setSecond(AsciiString text);	// 0x004D662D (stores +0x20; the second string is printed as authToken, the first as authKey)

	AsciiString m_first;										// +0x1C
	AsciiString m_second;										// +0x20
};
// SIZE 0x24
typedef char NetGameSpyStatsAuthKeyCommandMsgSize[(sizeof(NetGameSpyStatsAuthKeyCommandMsg) == 0x24) ? 1 : -1];

#endif
