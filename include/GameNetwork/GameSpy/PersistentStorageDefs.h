// PersistentStorageDefs.h: the player statistics record GameSpy's persistent storage hands the lobby (Zero Hour's PSPlayerStats
// by role, GPL-3.0-or-later; the layout is retail's and not evidenced past its size).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts (code RVAs): SIZE 0x548: GameSpyInfo's constructor 0x00385FE1 builds one at +0x84 through 0x0055621F (its argument
// is 0) and its reset 0x00385D25 clears it through 0x00552CB8.
#pragma once
#ifndef BFME2_PERSISTENTSTORAGEDEFS_H
#define BFME2_PERSISTENTSTORAGEDEFS_H

#include "Common/GameType.h"

class PSPlayerStats
{
public:
	PSPlayerStats(Int playerID = 0);			// 0x0055621F
	~PSPlayerStats();							// out of line (GameSpyInfo's constructor registers it for unwinding, so it releases strings)
	void reset();								// 0x00552CB8 (the map spells it on a placeholder class)

	unsigned char m_unk0x00[0x548];				// +0x00
};
typedef char PSPlayerStatsSize[(sizeof(PSPlayerStats) == 0x548) ? 1 : -1];

#endif
