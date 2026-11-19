// GameModePreferences.h: a UserPreferences whose keys are prefixed with the game mode
// ("Rts" or "Strat"). The LAN and custom-match preferences derive from it.
//
// Target facts (retail game.dat): the constructor 0x0044D54B calls UserPreferences'
// constructor 0x003B1FF1, stores its int argument at +0x14 and the table 0x00C3EF60 at +0,
// and clears the four-byte AsciiString at +0x18; the destructor 0x0044D56A restores the
// table, releases +0x18 (releaseBuffer 0x00036410) and calls ~UserPreferences 0x003B1F65.
// Both are out of line. makeKey 0x0044D512 formats "%s:%s" (VA 0x00C3E4D0) from the mode's
// name ("Rts" at 0x00C3EF54, "Strat" at 0x00C3EF58) and the key into the string at +0x18
// and returns it. The table has thirteen slots: the base's, with write (0x0044D50D),
// getBool/getReal/getInt (0x0044D6C7/0x0044D6F6/0x0044D729) and setBool/setReal/setInt
// (0x0044D636/0x0044D665/0x0044D698) overridden (each prefixes its key through makeKey).
// SIZE 0x1C. Member names are neutral: retail does not name them.
#pragma once
#ifndef BFME2_GAMEMODEPREFERENCES_H
#define BFME2_GAMEMODEPREFERENCES_H

#include "Common/UserPreferences.h"

class GameModePreferences : public UserPreferences
{
public:
	GameModePreferences(int mode);												// 0x0044D54B
	virtual ~GameModePreferences();												// 0x0044D56A

	virtual bool write();														// 0x0044D50D
	virtual bool getBool(const AsciiString &key, bool defaultValue) const;		// 0x0044D6C7
	virtual float getReal(const AsciiString &key, float defaultValue) const;	// 0x0044D6F6
	virtual int getInt(const AsciiString &key, int defaultValue) const;			// 0x0044D729
	virtual void setBool(const AsciiString &key, bool val);						// 0x0044D636
	virtual void setReal(const AsciiString &key, float val);					// 0x0044D665
	virtual void setInt(const AsciiString &key, int val);						// 0x0044D698
	// The four setters the custom-match screen's slot-preference save calls (the map holds them as placeholders of this class).
	void rva0044DC54(int);														// 0x0044DC54
	void rva0044DCB9(int);														// 0x0044DCB9
	void rva0044DD1E(int);														// 0x0044DD1E
	void rva0044DD83(AsciiString);												// 0x0044DD83
	AsciiString rva0044DAA8(const AsciiString &);								// 0x0044DAA8 (the custom-match game name for the given player name)
	AsciiString rva0044DBA5();													// 0x0044DBA5 (the custom-match password)
	// Inline: AptLanLobby::OnLoadScreen 0x0044438A stores the mode through the LANPreferences member directly.
	void setMode(int mode) { m_mode = mode; }

protected:
	const AsciiString &makeKey(const char *key) const;							// 0x0044D512

	int m_mode;																	// +0x14
	mutable AsciiString m_keyBuffer;											// +0x18
};
// SIZE 0x1C

#endif
