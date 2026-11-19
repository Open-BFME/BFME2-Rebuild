// StrategicStatsPreferences.h: a profile's strategic (campaign) statistics file, "StrategicStats.ini" in the
// profile directory. SIZE 0x14 (no members of its own).
// Target facts: the constructor 0x00537DBC calls ProfilePreferences(1), stores the table 0x00C6913C (the base table with
// slot 13 = 0x00537D9A), and loads the file through that slot. Slot 0 is the base's deleting destructor
// (0x00537E60, shared by identical code folding). deleteStatsFile 0x0053789A removes the file.
#pragma once
#ifndef BFME2_STRATEGICSTATSPREFERENCES_H
#define BFME2_STRATEGICSTATSPREFERENCES_H

#include "Common/ProfilePreferences.h"

class StrategicStatsPreferences : public ProfilePreferences
{
public:
	StrategicStatsPreferences(const UnicodeString &profilePath);	// 0x00537DBC
	virtual void loadProfileStats(const UnicodeString &profilePath);	// 0x00537D9A
	static bool deleteStatsFile(const UnicodeString &profilePath);	// 0x0053789A
};

#endif
