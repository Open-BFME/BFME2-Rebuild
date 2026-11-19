// NetDisconnectPlayerCommandMsg.h: "this player is leaving" (Zero Hour's NetDisconnectPlayerCommandMsg by role; the map
// names the class and its setters, offsets are retail's).
// Target facts (code RVAs, VA 0x00C60244 table):
//  - SIZE 0x28 (DisconnectManager::sendDisconnectCommand 0x004D3D5C allocates 0x28): NetCommandMsg plus the slot byte
//    +0x1C (zeroed), a dword +0x20 the constructor does not write, and the disconnect frame +0x24 (zeroed). The
//    constructor 0x004D57F1 stores the compiler's shared four-slot table and the command type 0x1A.
//  - setDisconnectSlot 0x0046EDE3 is a one-instruction byte store folded with the other messages' byte setters;
//    setDisconnectFrame 0x005739F6 a dword store folded with NetWrapperCommandMsg::setDataOffset.
#pragma once
#ifndef BFME2_NETDISCONNECTPLAYERCOMMANDMSG_H
#define BFME2_NETDISCONNECTPLAYERCOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetDisconnectPlayerCommandMsg : public NetCommandMsg
{
public:
	NetDisconnectPlayerCommandMsg();					// 0x004D57F1

	void setDisconnectSlot(UnsignedByte slot);			// 0x0046EDE3
	void setDisconnectFrame(UnsignedInt frame);			// 0x005739F6
	UnsignedByte getDisconnectSlot();							// 0x004C54EC (folded byte getter at +0x1C)
	UnsignedInt getDisconnectFrame();							// 0x00091A56 (folded dword getter at +0x24)

	UnsignedByte m_disconnectSlot;						// +0x1C
	unsigned char m_pad1D[3];							// +0x1D
	UnsignedInt m_unk0x20;								// +0x20
	UnsignedInt m_disconnectFrame;						// +0x24
};
// SIZE 0x28
typedef char NetDisconnectPlayerCommandMsgSize[(sizeof(NetDisconnectPlayerCommandMsg) == 0x28) ? 1 : -1];

#endif
