// SkirmishPreferences.h: the skirmish menu's settings file (Skirmish.ini): the per-profile
// keys and the list of known user names. SIZE 0x20.
//
// Target facts (retail game.dat): the constructor 0x0043C128 calls UserPreferences'
// constructor 0x003B1FF1, stores the table 0x00C3D658 at +0, its int argument (the profile
// index) at +0x14, builds the list at +0x18 through 0x0043C072 and clears the four-byte
// AsciiString at +0x1C; it loads "Skirmish.ini" (VA 0x00C3D648) and reads the "UserNames"
// and "CurrentUserName" keys into those two members. The destructor 0x0043C286 restores
// the table, releases +0x1C, destroys the list (0x00433BD7) and runs ~UserPreferences.
// formatProfileKey 0x0043BA95 builds ":%d:%s:%s" (VA 0x00C3D60C) from the profile index,
// the given user key and the setting name. The table has thirteen slots: the base's with
// write overridden (0x0043C6C7: rebuildUserNamesEntry, then UserPreferences::write).
// Names are the ledger's (rows) and Zero Hour's where they agree.
#pragma once
#ifndef BFME2_SKIRMISHPREFERENCES_H
#define BFME2_SKIRMISHPREFERENCES_H

#include <list>
#include "Common/UserPreferences.h"
#include "Common/UnicodeString.h"

class SkirmishPreferences : public UserPreferences
{
public:
	SkirmishPreferences(int profileIndex);										// 0x0043C128
	virtual ~SkirmishPreferences();
	virtual bool write();

	AsciiString formatProfileKey(const AsciiString *userKey, const char *name);
	AsciiString encodeUserKey(const UnicodeString &user, const char *name);
	AsciiString buildProfileKey(const char *name);
	void setCurrentUserName(const UnicodeString &newName);
	AsciiString getPreferredMap();
	bool usesSystemMapDir();
	bool Rva0043B9E8();
	_STL::list<UnicodeString> getUserNames_Rva0043C2D0();						// 0x0043C2D0: a copy of the user name list
	UnicodeString Rva0043B9F5();
	UnicodeString Rva0043BB88();
	int Rva0043BBB6(UnicodeString user);
	void Rva0043BE36(const AsciiString &mapName);
	void Rva0043BFDB(const UnicodeString &user, int profileIndex);
	void rva0043BE7C();
	void rva0043BD36();

	int m_profileIndex;															// +0x14
	_STL::list<UnicodeString> m_userNames;										// +0x18 (UserNames)
	AsciiString m_currentUserName;												// +0x1C (CurrentUserName)

private:
	void rebuildUserNamesEntry();
};
// SIZE 0x20

#endif
