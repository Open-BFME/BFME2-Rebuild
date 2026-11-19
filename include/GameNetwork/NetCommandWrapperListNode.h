// NetCommandWrapperListNode.h: the reassembly state of one wrapped command (Zero Hour's
// NetCommandWrapperListNode; names are theirs by role, offsets are retail's).
// Target facts (code RVAs, VA 0x00C70A20 table):
//  - SIZE 0x20 (processWrapper 0x0058C11F allocates 0x20 before the constructor 0x0058BFA8). Table of one
//    slot: the scalar deleting destructor 0x0058C1F3 (the destructor 0x0058C00C is protected, `M`).
//  - The constructor clears the link +4, takes the chunk count from the message (+0x18), allocates
//    the present-flags with new[] (+0x14, zeroed in a loop, +0x1C = 0), takes the total length (+0x10),
//    allocates the data with new[] (+0xC) and the wrapped command id (+8). The destructor
//    delete[]s +0x14 and +0xC and nulls them.
#pragma once
#ifndef BFME2_NETCOMMANDWRAPPERLISTNODE_H
#define BFME2_NETCOMMANDWRAPPERLISTNODE_H

#include "Common/GameType.h"

class NetWrapperCommandMsg;
class NetCommandWrapperList;

class NetCommandWrapperListNode
{
	friend class NetCommandWrapperList;	// destroys nodes through the protected destructor
public:
	NetCommandWrapperListNode(NetWrapperCommandMsg *msg);	// 0x0058BFA8

protected:
	virtual ~NetCommandWrapperListNode();					// slot 0, 0x0058C00C (deleting 0x0058C1F3)

public:
	Bool isComplete();										// 0x0058C03B
	void copyChunkData(NetWrapperCommandMsg *msg);			// 0x0058C047
	Int getPercentComplete();								// 0x0058C20F
	UnsignedShort getCommandID() const { return m_commandID; }
	UnsignedInt getRawDataLength() const { return m_dataLength; }
	UnsignedByte *getRawData() const { return m_data; }

	NetCommandWrapperListNode *m_next;						// +0x04
	UnsignedShort m_commandID;								// +0x08
	unsigned char m_pad0A[2];								// +0x0A
	UnsignedByte *m_data;									// +0x0C
	UnsignedInt m_dataLength;								// +0x10
	Bool *m_chunksPresent;									// +0x14
	UnsignedInt m_numChunks;								// +0x18
	UnsignedInt m_numChunksPresent;							// +0x1C
};
// SIZE 0x20

#endif
