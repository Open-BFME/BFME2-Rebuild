// NetHeroCommandMsg.h: a create-a-hero data blob sent to the other players (BFME II only; the name is the map's).
// Target facts (code RVAs, VA 0x00C60264 table):
//  - SIZE 0x2C: NetCommandMsg plus a dword +0x1C and a word +0x20 (zeroed by the constructor 0x004D58DE, which stores
//    the command type 0x14), the data pointer +0x24 and its length +0x28. setData 0x004D5925 stores the length and copies
//    the data into a fresh array allocation (new[], memcpy); the destructor 0x004D5902 frees the data with delete[] when
//    there is some. Table: 0 the deleting destructor 0x004D5B14, the rest are the base's.
#pragma once
#ifndef BFME2_NETHEROCOMMANDMSG_H
#define BFME2_NETHEROCOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetHeroCommandMsg : public NetCommandMsg
{
public:
	NetHeroCommandMsg();								// 0x004D58DE
	virtual ~NetHeroCommandMsg();						// slot 0, 0x004D5902 (deleting 0x004D5B14)

	void setData(UnsignedByte *data, UnsignedInt dataLength);	// 0x004D5925

	UnsignedInt m_unk0x1C;								// +0x1C
	UnsignedShort m_unk0x20;							// +0x20
	unsigned char m_pad22[2];							// +0x22
	UnsignedByte *m_data;								// +0x24
	UnsignedInt m_dataLength;							// +0x28
};
// SIZE 0x2C
typedef char NetHeroCommandMsgSize[(sizeof(NetHeroCommandMsg) == 0x2C) ? 1 : -1];

#endif
