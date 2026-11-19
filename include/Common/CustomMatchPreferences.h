// CustomMatchPreferences.h: the custom-match lobby's settings, a GameModePreferences (so every key is prefixed with the game
// mode) read from "Online Files\CustomPref<profile>.ini" (Zero Hour's class of the same name reads "GeneralsOnline\CustomPref<profile>.ini").
// Target facts (code RVAs; table addresses are VAs): SIZE 0x1C, no member beyond GameModePreferences. The constructor 0x0054F52F
// takes the mode, runs GameModePreferences' constructor 0x0044D54B with it, stores the table 0x00C6AA60 (its own deleting
// destructor 0x0054F513 over GameModePreferences' twelve other slots), asks TheGameSpyInfo for the local profile id (slot 0x7C),
// formats "%s\CustomPref%d.ini" (VA 0x00C6AA94) with the directory "Online Files" (VA 0x00C194D8) and loads the result through
// load(AsciiString) 0x003B1A09. The destructor 0x0054F508 restores the table and jumps to ~GameModePreferences 0x0044D56A.
// 0x0054F5A4 reads the key "LobbyRoomID" (VA 0x00C6AAC8) through makeKey and returns atoi of its value, 0 when it is absent
// (GameSpyInfo::joinPreferredGroupRoom 0x0038553D calls it on a local one built with mode 0 or 1). Evidence for the class
// name: the file name format is Zero Hour's CustomMatchPreferences', and the ledger names the neighbouring rows
// 0x0054F5DE.. CustomMatchPreferences::getDisallowAsianText and so on. Retail also folds QuickMatchPreferences::getLastLadderAddr
// 0x005DF4B9 and getLastLadderPort 0x005DF50A with these identical bodies and the map names the folded rows here.
#pragma once
#ifndef BFME2_CUSTOMMATCHPREFERENCES_H
#define BFME2_CUSTOMMATCHPREFERENCES_H

#include "Common/GameModePreferences.h"

class CustomMatchPreferences : public GameModePreferences
{
public:
	CustomMatchPreferences(int mode);									// 0x0054F52F
	virtual ~CustomMatchPreferences();								// slot 0, 0x0054F508 (deleting 0x0054F513)
	void setLobbyRoomID(Int id);									// 0x0054F7C0 (the map holds it as rva0054F7C0 on GameModePreferences; it writes the key getLobbyRoomID reads)
	Int getLobbyRoomID();											// 0x0054F5A4 (the map holds it as rva0054F5A4 on GameModePreferences)
	AsciiString getLastLadderAddr();											// 0x005DF4B9
	UnsignedShort getLastLadderPort();											// 0x005DF50A
	Bool getDisallowAsianText();												// 0x0054F5DE
	Bool getDisallowNonAsianText();												// 0x0054F63A
	void setLastLadder(const AsciiString &addr, UnsignedShort port);			// 0x0054F696
	void setUsesLongGameList(Bool val);										// 0x0054F745
	void setDisallowAsianText(Bool val);										// 0x0054F825
	void setDisallowNonAsianText(Bool val);										// 0x0054F8A0
};
typedef char CustomMatchPreferences_size[(sizeof(CustomMatchPreferences) == 0x1C) ? 1 : -1];

#endif
