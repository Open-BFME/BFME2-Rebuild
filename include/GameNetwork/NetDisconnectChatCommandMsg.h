// NetDisconnectChatCommandMsg.h: the chat line sent while the connection is being torn down (Zero Hour's
// NetDisconnectChatCommandMsg by role; the map names the class only through ConnectionManager::processDisconnectChat
// 0x004D1023, its constructor is a placeholder).
// Target facts (code RVAs): SIZE 0x20 (NetPacket::readDisconnectChatMessage 0x0059205C allocates 0x20). The constructor
// 0x004D60CA runs NetCommandMsg's, stores its own table 0x00C60464 and the command type 0xD, and zeroes the UnicodeString
// text +0x1C. The destructor 0x004D60E3 (deleting 0x004D6A81, protected) releases the text. The text getter 0x004D6119
// and the by-value setter 0x004D6187 are shared with NetChatCommandMsg's (identical bodies, folded).
#pragma once
#ifndef BFME2_NETDISCONNECTCHATCOMMANDMSG_H
#define BFME2_NETDISCONNECTCHATCOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"
#include "Common/UnicodeString.h"

class NetDisconnectChatCommandMsg : public NetCommandMsg
{
public:
	NetDisconnectChatCommandMsg();								// 0x004D60CA
protected:
	virtual ~NetDisconnectChatCommandMsg();						// slot 0, 0x004D60E3 (deleting 0x004D6A81)
public:
	UnicodeString getText() const;								// 0x004D6119
	void setText(UnicodeString text);							// 0x004D6187

	UnicodeString m_text;										// +0x1C
};
// SIZE 0x20
typedef char NetDisconnectChatCommandMsgSize[(sizeof(NetDisconnectChatCommandMsg) == 0x20) ? 1 : -1];

#endif
