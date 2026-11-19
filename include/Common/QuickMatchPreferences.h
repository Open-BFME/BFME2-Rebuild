// QuickMatchPreferences.h: the quick-match lobby's settings file (a UserPreferences with typed
// accessors that store their values as decimal strings). SIZE 0x14, no members of its own.
//
// Target facts (retail game.dat): the constructor 0x005DF1A3 calls UserPreferences' constructor
// 0x003B1FF1, stores the table 0x00C776DC (the thirteen UserPreferences slots with its own deleting
// destructor 0x005DF187 in slot 0) and loads "%s\QMPref%d.ini" (VA 0x00C77710; arguments "Online Files"
// and the local profile id) through UserPreferences::load(AsciiString) 0x003B1A09. The destructor
// 0x005DF17C restores the table and jumps to ~UserPreferences 0x003B1F65. The accessors read the
// map base at +4 (find 0x001F8437) or go through the virtual getInt/setInt (slots 6 and 11).
#pragma once
#ifndef BFME2_QUICKMATCHPREFERENCES_H
#define BFME2_QUICKMATCHPREFERENCES_H

#include "Common/UserPreferences.h"

class QuickMatchPreferences : public UserPreferences
{
public:
	QuickMatchPreferences();													// 0x005DF1A3
	virtual ~QuickMatchPreferences();											// 0x005DF17C

	void setColor(Int val);														// 0x005DF214
	Int getColor();																// 0x005DF25C
	void setSide(Int val);														// 0x005DF2A5
	Int getSide();																// 0x005DF2ED
	void setHighest1vs1Rank(Int val);											// 0x005DF336
	Int getHighest1vs1Rank();													// 0x005DF37E
	void setHighest2vs2Rank(Int val);											// 0x005DF3C7
	Int getHighest2vs2Rank();													// 0x005DF40F
	Bool isMapSelected(const AsciiString &mapName);								// 0x005DF458
	void setMapSelected(const AsciiString &mapName, Bool selected);	// 0x005DF73F
	// getLastLadderAddr 0x005DF4B9 and getLastLadderPort 0x005DF50A fold with CustomMatchPreferences' identical bodies: the map names them there (CustomMatchPreferences.h).

	Int getMaxDisconnects();	// 0x005DF55B
	Int getMaxPoints();	// 0x005DF5AB
	Int getMinPoints();	// 0x005DF5FE
	Int getWaitTime();	// 0x005DF64E
	Int getNumPlayers();	// 0x005DF69E
	Int getMaxPing();	// 0x005DF6EE
	void setLastLadder(const AsciiString &addr, UnsignedShort port);	// 0x005DF7A7
	void setMaxDisconnects(Int val);	// 0x005DF856
	void setMaxPoints(Int val);	// 0x005DF8CF
	void setMinPoints(Int val);	// 0x005DF948
	void setWaitTime(Int val);	// 0x005DF9C1
	void setNumPlayers(Int val);	// 0x005DFA3A
	void setMaxPing(Int val);	// 0x005DFAB3
};
// SIZE 0x14
typedef char QuickMatchPreferencesSize[(sizeof(QuickMatchPreferences) == 0x14) ? 1 : -1];

#endif
