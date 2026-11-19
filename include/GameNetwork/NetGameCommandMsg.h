// NetGameCommandMsg.h: a game message carried over the network (Zero Hour's NetGameCommandMsg by role; offsets are
// retail's).
// Target facts (code RVAs, VA 0x00C601E4 table):
//  - SIZE 0x30 (ConnectionManager::sendLocalGameMessage 0x004D0E06 allocates 0x30). NetCommandMsg (0x1C) plus two words
//    +0x1C/+0x20 (zeroed by both constructors), the game message type +0x24, and a singly linked list of
//    GameMessageArgument (0x1C bytes each, payload at +8, type at +0x18) with head +0x28 and tail +0x2C.
//  - Constructors: 0x004D55D6 default (type word 4 = GAMECOMMAND at +0x14); 0x004D5CBE from a GameMessage copies the
//    message type and appends every argument through addArgument (0x004D5A7A), which allocates a node (0x1C bytes) and
//    links it at the tail. The destructor 0x004D5609 pops the list, deleting each node through its virtual destructor.
//  - Table (4 slots): 0 the scalar deleting destructor 0x004D5A5E, 1 getSortNumber (the base's 0x004CEEA1), 2 0x004D55FC
//    (answers whether the game message type is 0x448 = MSG_SELF_DESTRUCT; the map spells it vslot08), 3
//    getContentsAsAsciiString 0x004D5D55.
#pragma once
#ifndef BFME2_NETGAMECOMMANDMSG_H
#define BFME2_NETGAMECOMMANDMSG_H

#include "GameNetwork/NetCommandMsg.h"
#include "Common/MessageStream.h"

class NetGameCommandMsg : public NetCommandMsg
{
public:
	NetGameCommandMsg();								// 0x004D55D6
	NetGameCommandMsg(GameMessage *msg);				// 0x004D5CBE
	virtual ~NetGameCommandMsg();						// slot 0, 0x004D5609 (deleting 0x004D5A5E)

	virtual Bool vslot08();								// slot 2, 0x004D55FC
	virtual AsciiString getContentsAsAsciiString();		// slot 3, 0x004D5D55

	void addArgument(GameMessageArgumentDataType type, GameMessageArgumentType argument);	// 0x004D5A7A
	GameMessage *constructGameMessage();				// 0x004D68C6

	UnsignedInt m_unk0x1C;								// +0x1C
	UnsignedInt m_unk0x20;								// +0x20
	GameMessage::Type m_gameMessageType;				// +0x24
	GameMessageArgument *m_argList;						// +0x28
	GameMessageArgument *m_argTail;						// +0x2C
};
// SIZE 0x30
typedef char NetGameCommandMsgSize[(sizeof(NetGameCommandMsg) == 0x30) ? 1 : -1];

#endif
