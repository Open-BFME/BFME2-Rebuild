// LANPreferences.h: the LAN lobby's settings (user name and friends), a GameModePreferences
// with no members of its own. SIZE 0x1C.
//
// Target facts (retail game.dat): the constructor 0x0044D2F5 calls GameModePreferences'
// constructor 0x0044D54B with its int argument, stores the table 0x00C3EF04 (the game-mode
// table with the same thirteen slots) and calls loadFromIniFile 0x0044D2AC, which loads the
// file "NetworkPref.ini" (VA 0x00C3EF38) through UserPreferences::load(AsciiString).
// The destructor 0x0044D285 restores the table and jumps to ~GameModePreferences.
#pragma once
#ifndef BFME2_LANPREFERENCES_H
#define BFME2_LANPREFERENCES_H

#include "Common/GameModePreferences.h"
#include "Common/UnicodeString.h"

class LANPreferences : public GameModePreferences
{
public:
	LANPreferences(int mode);													// 0x0044D2F5
	virtual ~LANPreferences();													// 0x0044D285

	bool loadFromIniFile();														// 0x0044D2AC
	void rva0044D47F(UnicodeString);											// 0x0044D47F (no map name): AptLanLobby stores the entered player name through it
};
// SIZE 0x1C

#endif
