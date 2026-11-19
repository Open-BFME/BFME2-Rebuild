// AptOnlineShell.h: the online shell screen the GameSpy screens (the custom match, the quick match) sit in. Only what the rows
// written so far use is declared; the screen's SIZE and the other members are not recovered yet.
//
// Target facts (retail game.dat): derives from _bfme_AptGameWindow (members from +0x27C). The quick match screen keeps a pointer to it at +0x58
// and calls rva00516F08 0x00516F08 (the map holds it on the class AptOnline) when a search is cancelled.
#pragma once
#ifndef BFME2_APTONLINESHELL_H
#define BFME2_APTONLINESHELL_H

#include "GameClient/AptOnline.h"

class AptOnlineShell : public AptOnline
{
public:
	void rva00517048(const UnicodeString &text);	// 0x00517048 (the map spells it rva00517048): puts the text in the log-in message label and opens the chat message
	void joinPreferredRoom();						// 0x00516F08 (the map holds it on the class AptOnline): joins the preferred group room again
	void LoadChildScreen(const char *name);			// 0x00517027: remembers the sub-screen to load when none is pending

	void rva00516EA7();								// 0x00516EA7: marks the shell active and shows the Apt background
	void closeChatMessage();						// 0x00516EE9 (the map holds it on the class Rva00517048): tells the movie to close the chat message
	int rva00516FE5(int message, unsigned int data1, unsigned int data2);	// 0x00516FE5: the base's window message handler, then every child's
};

#endif
