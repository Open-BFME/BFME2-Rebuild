// NetCommandList.h: a doubly linked list of NetCommandRefs sorted by timestamp and command type
// (Zero Hour's NetCommandList and NetCommandRef; names are theirs by role, offsets are retail's).
// Target facts (code RVAs, VA 0x00C70A08 table):
//  - NetCommandRef, SIZE 0x14: constructor 0x0058B88A (COMDAT) stores the message at +0, clears the
//    links +4/+8, attaches the message (+0x18 of the message), sets +0x10 = -1 and the byte +0xC = 0.
//    Its destructor releases the message through the null-safe detach 0x0058B8AE (folded with the
//    node's detach) and the scalar deleting destructor is 0x004CF341.
//  - NetCommandList: constructor 0x0058B06A stores the table (one slot: the scalar deleting
//    destructor 0x0058B50F, which restores the table, calls reset 0x0058B283 and frees; protected,
//    `M`) and clears first +4, last +8 and the last-inserted pointer +0xC. SIZE 0x10 (FrameData
//    allocates 0x10 and calls this constructor at 0x005DA522).
#pragma once
#ifndef BFME2_NETCOMMANDLIST_H
#define BFME2_NETCOMMANDLIST_H

#include "Common/GameType.h"
#include "GameNetwork/NetCommandMsg.h"

class NetCommandRef
{
public:
	NetCommandRef(NetCommandMsg *msg);		// 0x0058B88A
	~NetCommandRef();						// scalar deleting 0x004CF341

	NetCommandMsg *getCommand() const { return m_command; }
	NetCommandRef *getNext() const { return m_next; }
	NetCommandRef *getPrev() const { return m_prev; }
	void setNext(NetCommandRef *next) { m_next = next; }
	void setPrev(NetCommandRef *prev) { m_prev = prev; }
	UnsignedByte getRelay() const { return m_relay; }
	void setRelay(UnsignedByte relay) { m_relay = relay; }
	UnsignedInt getTimeLastSent() const { return m_timeLastSent; }
	void setTimeLastSent(UnsignedInt time) { m_timeLastSent = time; }
	void detach();							// 0x0058B8AE (null-safe release of the message)
	AsciiString rva0058B8BA();				// 0x0058B8BA (the message's text plus ", relay=0x%X"; the map has no better name)

	NetCommandMsg *m_command;				// +0x00
	NetCommandRef *m_next;					// +0x04
	NetCommandRef *m_prev;					// +0x08
	UnsignedByte m_relay;					// +0x0C
	unsigned char m_pad0D[3];				// +0x0D
	UnsignedInt m_timeLastSent;				// +0x10
};
// SIZE 0x14

class NetCommandList
{
public:
	NetCommandList();						// 0x0058B06A

protected:
	friend class FrameData;
	friend class ConnectionManager;		// ~ConnectionManager 0x004D2344 and init/reset destroy and rebuild the lists
	friend class Connection;				// ~Connection 0x004D060B deletes its command list
	virtual ~NetCommandList() { reset(); }	// slot 0, deleting 0x0058B50F

public:
	void reset();							// 0x0058B283
	NetCommandRef *addMessage(NetCommandMsg *cmdMsg);			// 0x0058B2C6
	Bool isEqualCommandMsg(NetCommandMsg *msg1, NetCommandMsg *msg2);	// 0x0058B174
	NetCommandRef *getFirstMessage() const { return m_first; }
	NetCommandRef *findMessage(NetCommandMsg *msg);			// 0x0058B4E2
	NetCommandRef *findMessage(UnsignedShort id, UnsignedByte player, UnsignedInt timestamp);	// 0x0058B0D7
	NetCommandRef *findMessage(UnsignedShort id, UnsignedByte player, NetCommandType type, UnsignedInt frame);	// 0x0058B121
	void removeMessage(NetCommandRef *msg);	// 0x0058B07E
	void appendList(NetCommandList *list);	// 0x0058B531
	Int length();							// 0x0058B0CA

	NetCommandRef *m_first;					// +0x04
	NetCommandRef *m_last;					// +0x08
	NetCommandRef *m_lastMessageInserted;	// +0x0C
};
// SIZE 0x10

#endif
