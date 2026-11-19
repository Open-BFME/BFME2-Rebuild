// GameSorter.h: the sort state of a multiplayer lobby's games list (the custom-match and LAN lobby screens embed one, SIZE 0x1C).
//
// Target facts (retail game.dat, RVAs): constructor 0x00580842 (takes the owning screen's Apt callback host, binds the sort
// callbacks OnSortName 0x005801C0, OnSortMap 0x005801CA, OnSortPlayers 0x005801D4, OnSortPing 0x005801DE and OnSortStatus
// 0x005801E8 through it, stores 4 at +0x0C, 8 at +0x10, clears the byte at +0x14, sets the byte at +0x15 and clears +0x18).
// 0x00580172 tests and clears the byte at +0x14 (the sort changed since the list was last filled); 0x00580316 stores the sort
// columns in a GameModePreferences and 0x00580263 reads them back. Only what the screens read is declared; the rest of the
// class (its sort over the game records, 0x005803C0 and the std::sort instantiation) is rows of src/unk/O005801C0.cpp.
#pragma once
#ifndef BFME2_GAMESORTER_H
#define BFME2_GAMESORTER_H

class GameModePreferences;

class GameSorter
{
public:
	void rva00580316(GameModePreferences *);	// 0x00580316: stores the sort columns in the preferences
	bool rva00580172();							// 0x00580172: whether the sort changed since the last list refresh (reads and clears the byte at +0x14)

	unsigned char m_x00[0x14];					// +0x00
	bool m_changed;								// +0x14
	unsigned char m_x15[0x7];					// +0x15
};
typedef char GameSorterSize[sizeof(GameSorter) == 0x1C ? 1 : -1];

#endif
