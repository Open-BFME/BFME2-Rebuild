// GameSpyMiscPreferences.h: the online lobby's miscellaneous settings, a UserPreferences read from
// GeneralsOnline\GSMiscPref<profile>.ini (Zero Hour's class of the same name). Target facts: the destructor
// 0x005596EA stores the table 0x00C6B3AC (the class's own virtual destructor over UserPreferences' slots) and
// jumps to UserPreferences' destructor; the accessors call UserPreferences' virtual getInt (slot 6), getBool
// (slot 4), getAsciiString (slot 8) and the setters with the keys "Locale", "ToolTipCachedStats",
// "AllOtherCachedStats", "QMResLock" and "MaxMessagesPerUpdate" (default 100). Zero Hour has one cached-stats string
// (CachedStats); this game keeps two.
#pragma once
#ifndef BFME2_GAMESPYMISCPREFERENCES_H
#define BFME2_GAMESPYMISCPREFERENCES_H

#include "Common/UserPreferences.h"

class GameSpyMiscPreferences : public UserPreferences
{
public:
	GameSpyMiscPreferences();									// 0x00559711 (loads GeneralsOnline\GSMiscPref<profile>.ini)
	virtual ~GameSpyMiscPreferences();							// 0x005596EA

	Int getLocale();											// 0x00559782
	void setLocale(Int val);									// 0x005597CB
	AsciiString getToolTipCachedStats();						// 0x00559813
	void setToolTipCachedStats(AsciiString val);				// 0x0055986F
	AsciiString getAllOtherCachedStats();						// 0x005598C8
	void setAllOtherCachedStats(AsciiString val);				// 0x00559924
	Bool getQuickMatchResLocked();								// 0x0055997D
	Int getMaxMessagesPerUpdate();								// 0x005599C8
};

#endif
