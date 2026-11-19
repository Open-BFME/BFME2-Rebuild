// LadderPreferences.h: the recent-ladders list of the online lobby (a UserPreferences that keeps a map of
// LadderPref records keyed by last-play date). Text after Zero Hour's Common/LadderPreferences.h.
//
// Target facts (retail game.dat): the constructor 0x005DFFD3 calls UserPreferences' constructor 0x003B1FF1,
// stores the table 0x00C77788 (the UserPreferences slots with its own deleting destructor and write 0x005E0026)
// and builds the map at +0x14 (0x00242F01); the destructor 0x005DFF7B restores the table, destroys the map
// (0x005DFE18) and calls ~UserPreferences. LadderPref is 0x10 bytes: the display name (UnicodeString) at +0,
// the address (AsciiString) at +4, the port at +8 and the last-play date at +0xC; its operator== 0x005DFC0B
// compares the address and the port, its memberwise operator= is 0x005DFB48.
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
#pragma once
#ifndef BFME2_LADDERPREFERENCES_H
#define BFME2_LADDERPREFERENCES_H

#include <map>
#include <time.h>
#include "Common/UserPreferences.h"
#include "Common/UnicodeString.h"

class LadderPref
{
public:
	UnicodeString name;
	AsciiString address;
	UnsignedShort port;
	time_t lastPlayDate;

	LadderPref() {}
	LadderPref(const LadderPref &other);	// 0x005BA3F5
	~LadderPref();	// 0x005BA3C0

	bool operator==(const LadderPref &other)
	{
		return (address == other.address && port == other.port);
	}
};
// SIZE 0x10
typedef char LadderPrefSize[(sizeof(LadderPref) == 0x10) ? 1 : -1];

typedef _STL::map<time_t, LadderPref> LadderPrefMap;

class LadderPreferences : public UserPreferences
{
public:
	LadderPreferences();														// 0x005DFFD3
	virtual ~LadderPreferences();												// 0x005DFF7B

	Bool loadProfile(Int profileID);											// 0x005E0201
	virtual bool write();														// 0x005E0026

	const LadderPrefMap &getRecentLadders() { return m_ladders; }
	void addRecentLadder(LadderPref ladder);									// 0x005E03F6

private:
	LadderPrefMap m_ladders;													// +0x14
};

#endif
