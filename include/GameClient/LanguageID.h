// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// LanguageID: the game's language selector (Zero Hour's GameClient language list). Retail evidence: the global OurLanguage
// (VA 0x00E01E54, mangled ?OurLanguage@@3W4LanguageID@@A) is read and written by Keyboard::initKeyNames 0x002306B8, which switches
// on it for the key names and forces LANGUAGE_ID_GERMAN for the German keyboard layouts; the case order of that switch is the enum order.
#pragma once
#ifndef BFME2_LANGUAGEID_H
#define BFME2_LANGUAGEID_H

enum LanguageID
{
	LANGUAGE_ID_US = 0,
	LANGUAGE_ID_UK,
	LANGUAGE_ID_GERMAN,
	LANGUAGE_ID_FRENCH,
	LANGUAGE_ID_SPANISH,
	LANGUAGE_ID_ITALIAN,
	LANGUAGE_ID_JAPANESE,
	LANGUAGE_ID_JABBER,
	LANGUAGE_ID_KOREAN,
	LANGUAGE_ID_UNKNOWN
};

extern LanguageID OurLanguage;	// VA 0x00E01E54

#endif
