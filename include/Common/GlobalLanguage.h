// GlobalLanguage.h: the language/locale settings singleton (TheGlobalLanguageData,
// VA 0x00DFDC84): separators, the audio language and every font the UI asks for.
// FontDesc is the (name, point size, bold) triple those entries parse into.
//
// Evidence (retail game.dat): GameEngine::init allocates 0x13C bytes and runs the
// constructor 0x001EA9B5, which builds a SubsystemInterface and stores table 0x00BDEFC8
// (14 slots; init 0x001EA55A, empty reset and update, the rest inherited). The INI
// field table 0x00BDEAA0 gives every named member: three separator strings at
// +0x0C/+0x10/+0x14, UnicodeFontName +0x18, AudioLanguage +0x20, UseHardWordWrap +0x24,
// MilitaryCaptionSpeed +0x28, twenty-two FontDesc entries from +0x2C (stride 0xC, each parsed
// by parseFontDesc 0x0037691C) and ResolutionFontAdjustment at +0x134. FontDesc's constructor
// 0x00376900 stores the string "Arial"-style default name, size 12 and bold false; its
// assignment 0x002216DD copies the string, the size and the bold byte. +0x1C is a second
// string-sized slot the constructor clears and +0x138 the LocalFontFile list head (parsed
// by 0x001EAD69); both stay untyped. Names are Zero Hour's (donor) where the INI token
// agrees with them, the INI token otherwise.
#pragma once
#ifndef BFME2_GLOBALLANGUAGE_H
#define BFME2_GLOBALLANGUAGE_H

#include <stddef.h>
#include "Common/SubsystemInterface.h"
#include "Common/AsciiString.h"

class INI;
struct FieldParse;

struct FontDesc
{
	FontDesc();									// 0x00376900
	FontDesc &operator=(const FontDesc &other);	// 0x002216DD

	AsciiString name;							// +0x00
	int size;									// +0x04
	bool bold;									// +0x08
};
// SIZE 0x0C

class GlobalLanguage : public SubsystemInterface
{
public:
	GlobalLanguage();							// 0x001EA9B5
	virtual ~GlobalLanguage();					// 0x001EABAB
	virtual void init();						// 0x001EA55A
	virtual void reset() {}
	virtual void update() {}

	int adjustFontSize(int theFontSize);		// 0x001EA40D
	static const FieldParse m_languageParseTable[];	// VA 0x00BDEAA0: the INI field table (every member below, token by token)
	void onGameEngineExit();					// 0x001EA77F
	static void parseFontDesc(INI *ini, void *instance, void *store, const void *userData);	// 0x0037691C

	AsciiString m_decimalSeparator;				// +0x0C
	AsciiString m_thousandSeparator;			// +0x10
	AsciiString m_timeMinuteToSecondSeparator;	// +0x14
	AsciiString m_unicodeFontName;				// +0x18
	unsigned char m_unk0x1C[0x4];
	AsciiString m_audioLanguage;				// +0x20
	bool m_useHardWordWrap;						// +0x24
	unsigned char m_pad25[3];
	int m_militaryCaptionSpeed;					// +0x28
	FontDesc m_copyrightFont;					// +0x2C
	FontDesc m_messageFont;						// +0x38
	FontDesc m_militaryCaptionTitleFont;		// +0x44
	FontDesc m_militaryCaptionFont;				// +0x50
	FontDesc m_audioSubtitleFont;				// +0x5C
	FontDesc m_superweaponCountdownNormalFont;	// +0x68
	FontDesc m_superweaponCountdownReadyFont;	// +0x74
	FontDesc m_namedTimerCountdownNormalFont;	// +0x80
	FontDesc m_namedTimerCountdownReadyFont;	// +0x8C
	FontDesc m_drawableCaptionFont;				// +0x98
	FontDesc m_defaultWindowFont;				// +0xA4
	FontDesc m_defaultDisplayStringFont;		// +0xB0
	FontDesc m_tooltipFont;						// +0xBC (INI token TooltipFontName)
	FontDesc m_nativeDebugDisplay;				// +0xC8
	FontDesc m_drawGroupInfoFont;				// +0xD4
	FontDesc m_creditsTitleFont;				// +0xE0
	FontDesc m_creditsMinorTitleFont;			// +0xEC
	FontDesc m_creditsNormalFont;				// +0xF8
	FontDesc m_helpBoxNameFont;					// +0x104
	FontDesc m_helpBoxCostFont;					// +0x110
	FontDesc m_helpBoxShortcutFont;				// +0x11C
	FontDesc m_helpBoxDescriptionFont;			// +0x128
	float m_resolutionFontAdjustment;			// +0x134
	unsigned char m_unk0x138[0x4];
};
// SIZE 0x13C

typedef char GlobalLanguageSizeCheck[(sizeof(GlobalLanguage) == 0x13C) ? 1 : -1];
typedef char GlobalLanguageFontOffset[(offsetof(GlobalLanguage, m_helpBoxDescriptionFont) == 0x128) ? 1 : -1];
typedef char GlobalLanguageAdjustOffset[(offsetof(GlobalLanguage, m_resolutionFontAdjustment) == 0x134) ? 1 : -1];

extern GlobalLanguage *TheGlobalLanguageData;

#endif
