// NetChatCommandMsg.h: a chat line for some of the players (Zero Hour's NetChatCommandMsg by role; the map names the
// class only through ConnectionManager::processChat 0x004D10F1, its constructor is a placeholder).
// Target facts (code RVAs): SIZE 0x24 (NetPacket::readChatMessage 0x00592123 allocates 0x24). The constructor 0x004D6134
// runs NetCommandMsg's, stores its own table 0x00C60474 and the command type 0xE, and zeroes the UnicodeString text +0x1C
// and the dword +0x20 (the player mask, read after the text by readChatMessage and written after it by
// FillBufferWithChatCommand 0x00591170). The destructor 0x004D6151 (deleting 0x004D6A9D, protected) releases the text.
// The text getter 0x004D6119 copies the string out (a UnicodeString returned by value); the setter 0x004D6187 takes
// the string by value; the mask getter 0x0030D377 and setter 0x00317B9B are the shared dword accessors at +0x20.
#pragma once
#ifndef BFME2_NETCHATCOMMANDMSG_H
#define BFME2_NETCHATCOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"
#include "Common/UnicodeString.h"

class NetChatCommandMsg : public NetCommandMsg
{
public:
	NetChatCommandMsg();										// 0x004D6134
protected:
	virtual ~NetChatCommandMsg();								// slot 0, 0x004D6151 (deleting 0x004D6A9D)
public:
	UnicodeString getText() const;								// 0x004D6119
	void setText(UnicodeString text);							// 0x004D6187
	Int getPlayerMask();										// 0x0030D377 (folded dword getter at +0x20)
	void setPlayerMask(Int mask);								// 0x00317B9B (folded dword setter at +0x20)

	UnicodeString m_text;										// +0x1C
	Int m_playerMask;											// +0x20
};
// SIZE 0x24
typedef char NetChatCommandMsgSize[(sizeof(NetChatCommandMsg) == 0x24) ? 1 : -1];

#endif
