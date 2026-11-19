// UserPreferences.h: a settings file as a string-to-string map with typed
// accessors. SIZE 0x14.
//
// Target facts (retail RVAs; table addresses are VAs): the constructor 0x003B1FF1
// stores the preferences vtable (VA 0x00C1EF0C) at +0, builds the map base at
// +0x04 and clears the four-byte filename at +0x10. The destructor 0x003B1F65
// restores the table, releases the filename and destroys the map. The vtable
// has thirteen slots: 0 deleting destructor, 1 load(UnicodeString), 2 load(AsciiString),
// 3 write, 4 getBool, 5 getReal, 6 getInt, 7 getEnumIndex, 8 getAsciiString,
// 9 setBool, 10 setReal, 11 setInt, 12 setAsciiString.
//
// The filename is a UnicodeString at +0x10: load(UnicodeString) 0x003B2026 sets it from the
// user-data directory and the leaf, write 0x003B1BF3 opens it with _wfopen.
#pragma once
#ifndef BFME2_USERPREFERENCES_H
#define BFME2_USERPREFERENCES_H

#include <map>
#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"

typedef _STL::map<AsciiString, AsciiString> PreferenceMap;

class UserPreferences : public PreferenceMap
{
public:
	UserPreferences();
	virtual ~UserPreferences();

	// MSVC lays same-name overloads out in reverse declaration order: retail slot 1 is the
	// UnicodeString version (0x003B2026) and slot 2 the AsciiString one (0x003B1A09), so the
	// AsciiString overload is declared first.
	virtual bool load(const AsciiString &fname);
	virtual bool load(const UnicodeString &fname);
	virtual bool write(void);

	virtual bool getBool(const AsciiString &key, bool defaultValue) const;
	virtual float getReal(const AsciiString &key, float defaultValue) const;
	virtual int getInt(const AsciiString &key, int defaultValue) const;
	virtual int getEnumIndex(const char *key, const char **names, int count, int defaultValue) const;
	virtual AsciiString getAsciiString(const AsciiString &key, const AsciiString &defaultValue) const;

	virtual void setBool(const AsciiString &key, bool val);
	virtual void setReal(const AsciiString &key, float val);
	virtual void setInt(const AsciiString &key, int val);
	virtual void setAsciiString(const AsciiString &key, const AsciiString &val);

	// Stores val under the key "%d" of number when enable is set, else erases that key. 0x003B2322 (the map names it
	// ?rva003B2322@UserPreferences@@QAEXABVAsciiString@@H_N@Z; the callers 0x00385B1C and 0x00385E2A pass an indexed entry).
	void setNumberedEntry(const AsciiString &val, int number, bool enable);

public:
	UnicodeString m_filename;	// +0x10, the path load() opened and write() reopens
};
typedef char UserPreferences_size[(sizeof(UserPreferences) == 0x14) ? 1 : -1];

#endif
