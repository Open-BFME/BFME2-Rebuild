// NetDestroyPlayerCommandMsg.h: "destroy this player" (Zero Hour's NetDestroyPlayerCommandMsg by role; the map names only
// its constructor, as a placeholder).
// Target facts (code RVAs): SIZE 0x20. The constructor 0x004D57AE runs NetCommandMsg's, stores its own table 0x00C60234 and
// the command type 0xB, and zeroes the dword +0x1C. NetPacket::readDestroyPlayerMessage 0x0058DE5B reads one dword into it
// through the shared dword setter 0x00318B09.
#pragma once
#ifndef BFME2_NETDESTROYPLAYERCOMMANDMSG_H
#define BFME2_NETDESTROYPLAYERCOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetDestroyPlayerCommandMsg : public NetCommandMsg
{
public:
	NetDestroyPlayerCommandMsg();								// 0x004D57AE
	virtual AsciiString getContentsAsAsciiString();				// slot 3, 0x004D6040 (this class has its own table 0x00C60234)
	void setPlayerIndex(UnsignedInt index);						// 0x00318B09 (folded dword setter at +0x1C)
	UnsignedInt getPlayerIndex();								// 0x0030F2C7 (folded dword getter at +0x1C)

	UnsignedInt m_playerIndex;									// +0x1C
};
// SIZE 0x20
typedef char NetDestroyPlayerCommandMsgSize[(sizeof(NetDestroyPlayerCommandMsg) == 0x20) ? 1 : -1];

#endif
