// AptPlayerTribute.h: the in-game tribute screen (the player-to-player resource gift menu).
//
// Target facts (retail game.dat; only what the rows written so far prove): derives from _bfme_AptGameWindow; the open screen's
// instance is the global at VA 0x00A046B4 (the map's only name for it is address-spelled: g_Va00A046B4); the constructors are 0x00510FDB
// (one argument) and 0x0051066A, the destructors 0x00510D0C and 0x0050ED58. SIZE and the members are not recovered yet.
#pragma once
#ifndef BFME2_APTPLAYERTRIBUTE_H
#define BFME2_APTPLAYERTRIBUTE_H

#include "GameClient/BfmeAptGameWindow.h"

class AptPlayerTribute : public _bfme_AptGameWindow
{
public:
	void TributeEnabled(int index, char *buffer, bool query);	// 0x0050E98A: the Apt extern answering whether the local player can still send tribute
	static void CloseScreen();					// 0x0050E9D3: marks the open screen as closing, clears the shell background and enables the window manager
};

#endif
