// NetCommandMsg.h: the base of every network command message (Zero Hour's NetCommandMsg; its names
// supply the vocabulary, every offset below is retail's).
// Target facts (code RVAs, VA 0x00C60130 table):
//  - SIZE 0x1C: the allocation at 0x0058E0BB pushes 0x1C before the constructor call at 0x0058E0D1.
//    Constructor 0x004D5593 stores the table at +0 and writes timestamp +4 = -1, execution frame
//    +8 = -1, player id +0xC = 0, id word +0x10 = 0, command type +0x14 = -1, reference count
//    +0x18 = 1. The destructor 0x004CEE78 only stores the table back (no base destructor), so this
//    is a root class: Zero Hour's MemoryPoolObject base does not exist in retail.
//  - Table (4 slots): 0 scalar deleting destructor 0x004CEEA6 (protected, `M`), 1 getSortNumber
//    0x004CEEA1 (returns the id word +0x10), 2 the shared `xor al,al; ret` 0x0047A699 (NetGameCommandMsg's
//    override 0x004D55FC compares its dword +0x24 with 0x448; the name is not evidenced), 3 the
//    formatter 0x004D5B4C (getContentsAsAsciiString, reads the first five fields).
//  - attach is the out-of-line `inc dword [ecx+0x18]; ret` 0x004D55B8 (NetCommandRef's constructor 0x0058B88A and the
//    ConnectionManager senders call it); detach 0x004D55BC
//    decrements and deletes the message through its virtual destructor when the count reaches zero (or
//    goes negative). setID 0x004CEE90 writes the word +0x10.
#pragma once
#ifndef BFME2_NETCOMMANDMSG_H
#define BFME2_NETCOMMANDMSG_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"

// Only the three acknowledgement types are evidenced here (isEqualCommandMsg 0x0058B174 tests them);
// the other values are not named.
enum NetCommandType
{
	NETCOMMANDTYPE_UNKNOWN = -1,
	NETCOMMANDTYPE_ACKBOTH = 0,
	NETCOMMANDTYPE_ACKSTAGE1 = 1,
	NETCOMMANDTYPE_ACKSTAGE2 = 2,
	NETCOMMANDTYPE_FRAMEINFO = 3,		// NetFrameCommandMsg's constructor 0x004CEEC3 stores 3
	NETCOMMANDTYPE_GAMECOMMAND = 4,		// NetGameCommandMsg's constructors store 4
	NETCOMMANDTYPE_REQUESTGAMESPYSTATSAUTHKEY = 5,	// NetRequestGameSpyStatsAuthKeyCommandMsg's constructor 0x004D64F5 stores 5
	NETCOMMANDTYPE_REQUESTPLAYERLEAVE = 7,	// NetRequestPlayerLeaveCommandMsg's constructor 0x004D5A10 stores 7
	NETCOMMANDTYPE_INFORMPLAYERLEAVEFRAME = 8,	// NetInformPlayerLeaveFrameCommandMsg's constructor 0x004D59D1 stores 8
	NETCOMMANDTYPE_REQUESTFRAMEDATA = 9,	// NetRequestFrameDataCommandMsg's constructor 0x004D5A30 stores 9
	NETCOMMANDTYPE_PLAYERLEAVE = 10,		// NetPlayerLeaveCommandMsg's constructor 0x004D5795 stores 0xA
	NETCOMMANDTYPE_DESTROYPLAYER = 11,		// NetDestroyPlayerCommandMsg's constructor 0x004D57AE stores 0xB; DisconnectManager::sendPlayerDestruct 0x004D3AE2 asks about 0xB
	NETCOMMANDTYPE_KEEPALIVE = 12,		// NetKeepAliveCommandMsg's constructor 0x004D57C7 stores 0xC
	NETCOMMANDTYPE_DISCONNECTCHAT = 13,	// NetDisconnectChatCommandMsg's constructor 0x004D60CA stores 0xD
	NETCOMMANDTYPE_CHAT = 14,			// NetChatCommandMsg's constructor 0x004D6134 stores 0xE
	NETCOMMANDTYPE_PROGRESS = 15,		// NetProgressCommandMsg's constructor 0x004D582B stores 0xF
	NETCOMMANDTYPE_LOADCOMPLETE = 16,	// ConnectionManager::loadProgressComplete 0x004D02F6 stores 0x10
	NETCOMMANDTYPE_DISCONNECTKEEPALIVE = 25,	// NetDisconnectKeepAliveCommandMsg's constructor 0x004D57DC stores 0x19
	NETCOMMANDTYPE_DISCONNECTPLAYER = 26,	// NetDisconnectPlayerCommandMsg's constructor 0x004D57F1 stores 0x1A
	NETCOMMANDTYPE_DISCONNECTVOTE = 27,		// DisconnectManager::processDisconnectCommand 0x004D46AF routes 0x1B to processDisconnectVote
	NETCOMMANDTYPE_DISCONNECTFRAME = 28,	// and 0x1C to processDisconnectFrame
	NETCOMMANDTYPE_FILE = 19,			// NetFileCommandMsg's constructor 0x004D61BB stores 0x13
	NETCOMMANDTYPE_FILEANNOUNCE = 21,	// NetFileAnnounceCommandMsg's constructor 0x004D62A9 stores 0x15
	NETCOMMANDTYPE_FILEPROGRESS = 22,	// NetFileProgressCommandMsg's constructor 0x004D598E stores 0x16
	NETCOMMANDTYPE_WRAPPER = 18,		// NetWrapperCommandMsg's constructor 0x004D5844 stores 0x12
	NETCOMMANDTYPE_HERO = 20			// NetHeroCommandMsg's constructor 0x004D58DE stores 0x14
};

// 0x005811B5: whether commands of this type carry a command id.
Bool DoesCommandRequireACommandID(NetCommandType type);

class NetCommandMsg;

// 0x00581229: whether the message must be acknowledged; 0x005812C4: whether it is sent directly instead of through the
// frame ring; 0x005812A0: whether its type is part of the synchronised frame data.
Bool CommandRequiresAck(NetCommandMsg *msg);
Bool CommandRequiresDirectSend(NetCommandMsg *msg);
Bool IsCommandSynchronized(NetCommandType type);

// 0x005813D4: the printable name of a command type (the map spells it Rva005813D4Get; its argument is an int).
AsciiString Rva005813D4Get(Int type);

// 0x005811A8: the next command id (a 13-byte global counter).
UnsignedShort GenerateNextCommandID();

class NetCommandMsg
{
public:
	NetCommandMsg();									// 0x004D5593

protected:
	virtual ~NetCommandMsg() {}							// slot 0, 0x004CEE78 (deleting 0x004CEEA6); inline: the derived destructors store the table inline

public:
	virtual Int getSortNumber();						// slot 1, 0x004CEEA1
	virtual Bool vslot08() { return false; }			// slot 2, 0x0047A699 (the map names the override vslot08; the role is not evidenced)
	virtual AsciiString getContentsAsAsciiString();		// slot 3, 0x004D5B4C

	UnsignedInt getTimestamp() const { return m_timestamp; }
	void setTimestamp(UnsignedInt timestamp) { m_timestamp = timestamp; }
	UnsignedInt getExecutionFrame() const { return m_executionFrame; }
	void setExecutionFrame(UnsignedInt frame) { m_executionFrame = frame; }
	void setTiming(UnsignedInt timestamp, UnsignedInt frame) { m_timestamp = timestamp; m_executionFrame = frame; }
	UnsignedInt getPlayerID() const { return m_playerID; }
	void setPlayerID(UnsignedInt playerID) { m_playerID = playerID; }
	UnsignedShort getID() const { return m_id; }
	void setID(UnsignedShort id) { m_id = id; }			// inline in callers; the out-of-line copy is 0x004CEE90
	NetCommandType getNetCommandType() const { return m_commandType; }
	void setNetCommandType(NetCommandType type) { m_commandType = type; }
	void attach();										// 0x004D55B8 (the map spells it Rva004D55B8DwordCounter::inc)
	void detach();										// 0x004D55BC

	UnsignedInt m_timestamp;							// +0x04
	UnsignedInt m_executionFrame;						// +0x08
	UnsignedInt m_playerID;								// +0x0C
	UnsignedShort m_id;									// +0x10
	NetCommandType m_commandType;						// +0x14
	Int m_referenceCount;								// +0x18
};
// SIZE 0x1C

#endif
