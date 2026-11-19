// IgnorePreferences.h: the online lobby's saved ignore list, a UserPreferences read from
// "Online Files\IgnorePref<profile>.ini" (Zero Hour's class of the same name, which reads
// "GeneralsOnline\Ignore<profile>.ini"). GameSpyInfo::addToSavedIgnoreList / removeFromSavedIgnoreList keep one numbered
// entry per ignored profile in it.
// Target facts (code RVAs; table addresses are VAs): SIZE 0x14, no member beyond UserPreferences. The constructor
// 0x003B21C0 runs UserPreferences' constructor 0x003B1FF1, stores the table 0x00C1EF40 (its own deleting destructor
// 0x003B2231 over UserPreferences' other twelve slots), asks TheGameSpyInfo for the local profile id (slot 0x7C),
// formats "%s\IgnorePref%d.ini" (VA 0x00C1EF78) with the directory "Online Files" (VA 0x00C194D8) and loads the
// result through load(AsciiString) 0x003B1A09. The destructor 0x003B1FD4 restores the table and jumps to
// ~UserPreferences 0x003B1F65. The two callers (0x00385B1C, 0x00385E2A) call setNumberedEntry 0x003B2322 and write
// 0x003B1BF3 on a local one.
#pragma once
#ifndef BFME2_IGNOREPREFERENCES_H
#define BFME2_IGNOREPREFERENCES_H

#include "Common/UserPreferences.h"

class IgnorePreferences : public UserPreferences
{
public:
	IgnorePreferences();										// 0x003B21C0
	virtual ~IgnorePreferences();								// slot 0, 0x003B1FD4 (deleting 0x003B2231)
};
typedef char IgnorePreferences_size[(sizeof(IgnorePreferences) == 0x14) ? 1 : -1];

#endif
