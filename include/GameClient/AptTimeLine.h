// AptTimeLine.h: the end-of-game timeline screen (the statistics graphs of the score screens).
//
// Target facts (retail game.dat, RVAs; only what the rows written so far prove):
//  - Derives from _bfme_AptGameWindow (members from +0x27C). The constructors are 0x005202C8 (one argument), 0x0051FAFC and the
//    copy constructor 0x0051F87B; the destructor is 0x005204EF; the deleting destructor 0x005206E2. SIZE is not established.
//  - +0x288 and +0x28C are the begin and end of a vector of 0x50-byte player records (PlayerColor 0x0051E8C8 and PlayerFaction 0x0051EC4E
//    index it; CollectAllPlayerData 0x0051FA13 walks it). A record keeps the player's name as a UnicodeString at +4, a 24-bit colour
//    at +8 and the faction name as an AsciiString at +0xC; the rest is not recovered.
//  - +0x2B8 and +0x2BC are the begin and end of a vector of floats, the graph focus values GraphFocus 0x0051EA7A stores as hundredths.
#pragma once
#ifndef BFME2_APTTIMELINE_H
#define BFME2_APTTIMELINE_H

#include <vector>
#include "GameClient/BfmeAptGameWindow.h"
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"

struct AptTimeLinePlayer
{
	unsigned char m_x00[4];			// +0x00 not recovered
	UnicodeString m_name;			// +0x04: the player's name (CollectAllPlayerData 0x0051FA13 passes it to the window manager's text setter)
	unsigned int m_color;			// +0x08: the colour in the low 24 bits
	AsciiString m_faction;			// +0x0C: the faction name
	unsigned char m_x10[0x40];		// +0x10 not recovered
};
typedef char AptTimeLinePlayerSizeCheck[sizeof(AptTimeLinePlayer) == 0x50 ? 1 : -1];

class AptTimeLine : public _bfme_AptGameWindow
{
public:
	void PlayerColor(int index, char *buffer, bool query);			// 0x0051E8C8: the Apt extern answering a player's colour as "%#x"
	void PlayerFaction(int index, char *buffer, bool query);		// 0x0051EC4E: the Apt extern answering a player's faction name
	void GraphFocus(int index, const char *value, bool query);		// 0x0051EA7A: stores a graph focus value (the text as hundredths)
	void bfmeGo1020M(int);									// 0x0051E3A3: takes the window focus and starts the first step
	void openSaveScreen(const char *);						// 0x0051E3C3: opens the save screen for the finished game
	void OnButtonContinue(const char *);							// 0x0051F6D7: the continue button
	void rva0051ED7E();												// 0x0051ED7E

	int m_state;											// +0x27C (bfmeGo1020M 0x0051E3A3 sets it to 1)
	unsigned char m_gap280[0x288 - 0x280];						// +0x280 not recovered
	_STL::vector<AptTimeLinePlayer> m_players;					// +0x288 (begin, end and capacity of the player records)
	unsigned char m_gap294[0x2B8 - 0x294];							// +0x294 not recovered
	_STL::vector<float> m_graphFocus;							// +0x2B8 (begin, end and capacity of the graph focus values)
};

#endif
