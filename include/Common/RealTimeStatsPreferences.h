// RealTimeStatsPreferences.h: a profile's real-time (skirmish and multiplayer) statistics file,
// "RealTimeStats.ini" in the profile directory. SIZE 0x14 (no members of its own).
// Target facts: the constructor 0x00537DF9 calls ProfilePreferences(0), stores the table 0x00C69188 (the base table with
// slot 13 = 0x00537DAB), loads the file through that slot and sets "UseMapListTooltips" to true. Slot 0 is the base's
// deleting destructor (0x00537E60). deleteStatsFile 0x00537A4C removes the file.
#pragma once
#ifndef BFME2_REALTIMESTATSPREFERENCES_H
#define BFME2_REALTIMESTATSPREFERENCES_H

#include "Common/ProfilePreferences.h"

class RealTimeStatsPreferences : public ProfilePreferences
{
public:
	RealTimeStatsPreferences(const UnicodeString &profilePath);	// 0x00537DF9
	virtual void loadProfileStats(const UnicodeString &profilePath);	// 0x00537DAB
	static bool deleteStatsFile(const UnicodeString &profilePath);	// 0x00537A4C
};

#endif
