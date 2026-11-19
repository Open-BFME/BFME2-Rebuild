// GameMessageParser.h: the argument-type summary of a game message (Zero Hour's GameMessageParser and
// GameMessageParserArgumentType by role; offsets are retail's).
// Target facts (code RVAs): NetPacket::GetGameCommandSize 0x0058C356 allocates the parser with `push 0x10`, builds it from a
// GameMessage through the constructor 0x0054D54A, reads the first node at +4 and each node's type at +8 and run length at
// +0xC (the next node at +4 of a node), and frees the parser through its virtual destructor (table slot 0, deleting body
// 0x0054D5B7). The parser keeps the first node at +4, the last at +8 and the node count at +0xC; each node is 0x10 bytes with
// a vptr (constructor 0x0054D593).
#pragma once
#ifndef BFME2_GAMEMESSAGEPARSER_H
#define BFME2_GAMEMESSAGEPARSER_H

#include "Common/GameType.h"
#include "Common/GameMessageArgument.h"

class GameMessage;

class GameMessageParserArgumentType
{
public:
	GameMessageParserArgumentType(GameMessageParserArgumentType *next, GameMessageArgumentDataType type, Int argCount);	// 0x0054D593
	virtual ~GameMessageParserArgumentType();

	GameMessageParserArgumentType *getNext() const { return m_next; }
	GameMessageArgumentDataType getType() const { return m_type; }
	Int getArgCount() const { return m_argCount; }

	GameMessageParserArgumentType *m_next;					// +0x04
	GameMessageArgumentDataType m_type;						// +0x08
	Int m_argCount;											// +0x0C
};
// SIZE 0x10

class GameMessageParser
{
public:
	GameMessageParser(GameMessage *message);				// 0x0054D54A
	virtual ~GameMessageParser();							// slot 0, 0x0054D5B7 (deleting)

	GameMessageParserArgumentType *getFirstArgumentType() const { return m_first; }
	Int getNumTypes() const { return m_argTypeCount; }

	GameMessageParserArgumentType *m_first;					// +0x04
	GameMessageParserArgumentType *m_last;					// +0x08
	Int m_argTypeCount;										// +0x0C
};
// SIZE 0x10

#endif
