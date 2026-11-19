// INIBlockParse.h: the list of INI block parsers (the registrations behind retail's list head at 0x00DDF578).
// Target facts: INI::parseLine 0x0002C0F5 and findBlockParse 0x0002BBFC walk a singly linked list whose nodes hold
// the next node at +0, the block name at +4 and the parser at +8; getBlockParseList 0x0002BACF returns the head.
// Written from Zero Hour's INI.cpp (findBlockParse, theTypeTable), GPL-3.0-or-later.
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once
#ifndef BFME2_INIBLOCKPARSE_H
#define BFME2_INIBLOCKPARSE_H

#include <string.h>

class INI;

typedef void (*INIBlockParse)(INI *ini);

struct BlockParseNode
{
	BlockParseNode *m_next;		// +0x00
	const char *m_token;		// +0x04
	INIBlockParse m_parse;		// +0x08
};

extern BlockParseNode *TheBlockParseList;	// 0x00DDF578 (the map has no name for it)

int getBlockParseList();							// 0x0002BACF

#endif
