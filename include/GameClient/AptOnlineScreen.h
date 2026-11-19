// AptOnlineScreen.h: the base every online sub-screen (custom match, quick match, home, login) derives.
// Target facts (retail game.dat; RVAs, table addresses are VAs): AptCallbackHost's 0x58 bytes, then the owner screen pointer at
// +0x58 and a zeroed word at +0x5C (constructor 0x0056DC4C); it introduces the primary table's slots 1..10 (slot 0 is the destructor
// AptCallbackHost already has): the online shell keeps its children in a vector and forwards the window message to slot 5
// (AptOnlineShell 0x00516FE5 calls `[eax+0x14]` on each).
#pragma once
#ifndef BFME2_APTONLINESCREEN_H
#define BFME2_APTONLINESCREEN_H

#include "GameClient/AptCallbackHost.h"
#include "GameClient/BfmeAptGameWindow.h"

class GameWindow;

class AptOnlineScreen : public AptCallbackHost
{
public:
	virtual void slot1() = 0;										// 0x000B3FD0 (shared stub)
	virtual void InitGadgets() = 0;									// slot 2
	virtual void Update() = 0;										// slot 3
	virtual int rva005A6582(int, int, unsigned char, int) = 0;		// slot 4
	virtual int GameWindowSystem(GameWindow *, unsigned int, unsigned int, unsigned int) = 0;	// slot 5
	virtual void slot6() = 0;
	virtual bool slot7() = 0;
	virtual bool rva0059EF49() = 0;									// slot 8
	virtual bool rva005A6697() = 0;									// slot 9
	virtual const char *slot10() = 0;								// __purecall: the Apt movie prefix

	_bfme_AptGameWindow *m_screen;									// +0x58 the owner screen (0x0056DC4C stores its argument)
	int m_x5C;														// +0x5C
};
typedef char AptOnlineScreenSizeCheck[sizeof(AptOnlineScreen) == 0x60 ? 1 : -1];

#endif
