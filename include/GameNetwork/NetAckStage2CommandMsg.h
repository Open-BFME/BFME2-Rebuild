// NetAckStage2CommandMsg.h: an acknowledgement message (stage 2). Zero Hour's NetAckStage2CommandMsg supplies the names; offsets are retail's.
// Target facts (code RVAs, VA 0x00C60214 table):
//  - SIZE 0x28: NetCommandMsg (0x1C) plus a command id word +0x1C, the original player byte +0x1E and two
//    UnsignedInts +0x20/+0x24. The constructor from a message (0x004D570C) copies the message's id (+0x10) to +0x1C,
//    player (+0xC) to +0x1E, +4 to +0x20 and +8 to +0x24 (the fields' meaning is the acked message's timestamp and
//    execution frame by offset) and stores the command type 2 (ACKSTAGE2); the default constructor (0x004D5741) stores -1 at
//    +0x20/+0x24, zeroes +0x1C/+0x1E and the type.
//  - Table (4 slots): 0 the shared scalar deleting destructor 0x004D5B30 (the compiler's own, over ~NetCommandMsg),
//    1 getSortNumber 0x004D5776 (returns the command id word +0x1C), 2 the base's `return false`, 3 NetAckStage2CommandMsg's
//    getContentsAsAsciiString 0x004D5F1D. isEqualCommandMsg 0x0058B174 reads the id and player through the shared,
//    non-inlined getters 0x004D5767 (word +0x1C) and 0x004543C6 (byte +0x1E).
#pragma once
#ifndef BFME2_NETACKSTAGE2COMMANDMSG_H
#define BFME2_NETACKSTAGE2COMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"

class NetAckStage2CommandMsg : public NetCommandMsg
{
public:
	NetAckStage2CommandMsg();									// 0x004D5741
	NetAckStage2CommandMsg(NetCommandMsg *cmd);					// 0x004D570C

	virtual Int getSortNumber();					// slot 1, 0x004D5776
	virtual AsciiString getContentsAsAsciiString();	// slot 3, 0x004D5F1D

	void setCommandID(UnsignedShort commandID);		// 0x004D59AC (folded setter of +0x1C)
	void setOriginalPlayerID(UnsignedByte playerID);	// 0x004D576C (folded setter of +0x1E)
	UnsignedShort getCommandID() const;				// 0x004D5767 (shared getter)
	UnsignedByte getOriginalPlayerID() const;			// 0x004543C6 (shared getter)

	UnsignedShort m_commandID;						// +0x1C
	UnsignedByte m_originalPlayerID;					// +0x1E
	unsigned char m_pad1F;							// +0x1F
	UnsignedInt m_unk0x20;							// +0x20 the acked message's +4
	UnsignedInt m_unk0x24;							// +0x24 the acked message's +8
};
// SIZE 0x28

#endif
