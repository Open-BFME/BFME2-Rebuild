// NetWrapperCommandMsg.h: a message that carries one chunk of a command too large for a packet
// (Zero Hour's NetWrapperCommandMsg; names are theirs by role, offsets are retail's).
// Target facts (code RVAs, VA 0x00C60254 table):
//  - SIZE 0x38: the allocations at 0x0058E0F2 (readWrapperMessage) and 0x0059474B push 0x38 before
//    the constructor 0x004D5844, which runs NetCommandMsg's constructor, zeroes +0x1C..+0x30 and the word
//    +0x34, stores the table and the command type 0x12 at +0x14. The destructor 0x004D5871 stores the table,
//    frees +0x1C and stores the base table back (no base destructor call).
//  - Table (4 slots): 0 the scalar deleting destructor 0x004D5AF8 (protected), slots 1..3 are
//    NetCommandMsg's (0x004CEEA1, 0x0047A699, 0x004D5B4C).
//  - The accessors are out-of-line, folded rows: getData 0x0030F2C7 (+0x1C), getDataLength 0x0030D377
//    (+0x20), getDataOffset 0x00091A56 (+0x24), getTotalDataLength 0x005C4AE1 / setTotalDataLength
//    0x0033F8CA (+0x28), getChunkNumber 0x005C4ACD / setChunkNumber 0x002B2210 (+0x2C), getNumChunks
//    0x003F3FE8 / setNumChunks 0x00210C87 (+0x30), getWrappedCommandID 0x004D58CD / setWrappedCommandID
//    0x004D58D2 (word +0x34); setDataOffset 0x005739F6 (+0x24). setData 0x004D5894 frees +0x1C,
//    allocates with new[], copies and writes +0x1C/+0x20.
#pragma once
#ifndef BFME2_NETWRAPPERCOMMANDMSG_H
#define BFME2_NETWRAPPERCOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetWrapperCommandMsg : public NetCommandMsg
{
public:
	NetWrapperCommandMsg();								// 0x004D5844

protected:
	virtual ~NetWrapperCommandMsg();					// slot 0, 0x004D5871 (deleting 0x004D5AF8)

public:
	UnsignedByte *getData();							// 0x0030F2C7
	void setData(UnsignedByte *data, UnsignedInt dataLength);	// 0x004D5894
	UnsignedInt getDataLength();						// 0x0030D377
	UnsignedInt getDataOffset();						// 0x00091A56
	void setDataOffset(UnsignedInt offset);				// 0x005739F6
	UnsignedInt getTotalDataLength();					// 0x005C4AE1
	void setTotalDataLength(UnsignedInt length);		// 0x0033F8CA
	UnsignedInt getChunkNumber();						// 0x005C4ACD
	void setChunkNumber(UnsignedInt chunkNumber);		// 0x002B2210
	UnsignedInt getNumChunks();							// 0x003F3FE8
	void setNumChunks(UnsignedInt numChunks);			// 0x00210C87
	UnsignedShort getWrappedCommandID();				// 0x004D58CD
	void setWrappedCommandID(UnsignedShort id);			// 0x004D58D2

	UnsignedByte *m_data;								// +0x1C
	UnsignedInt m_dataLength;							// +0x20
	UnsignedInt m_dataOffset;							// +0x24
	UnsignedInt m_totalDataLength;						// +0x28
	UnsignedInt m_chunkNumber;							// +0x2C
	UnsignedInt m_numChunks;							// +0x30
	UnsignedShort m_wrappedCommandID;					// +0x34
	unsigned char m_pad36[2];							// +0x36
};
// SIZE 0x38

#endif
