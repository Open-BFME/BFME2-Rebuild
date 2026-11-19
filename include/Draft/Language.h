// DRAFT (h05 fpgen): Language from retail's field-parse table (VA 0x00BDEAA0+0x0).
// Offsets are compile-checked below; no function matches against it yet, so it stays
// in Draft until one does. Types are what each entry's parse function stores; names are
// the INI tokens.
// SIZE not established: members reach 0x138; bases and vptr, if any, are the leading gap.
// Unresolved members: m_localFontFile (+0x0, parseStringEntryRva001EAD69), m_copyrightFont (+0x2C, GlobalLanguage::parseFontDesc), m_messageFont (+0x38, GlobalLanguage::parseFontDesc), m_militaryCaptionTitleFont (+0x44, GlobalLanguage::parseFontDesc), m_militaryCaptionFont (+0x50, GlobalLanguage::parseFontDesc), m_audioSubtitleFont (+0x5C, GlobalLanguage::parseFontDesc), m_superweaponCountdownNormalFont (+0x68, GlobalLanguage::parseFontDesc), m_superweaponCountdownReadyFont (+0x74, GlobalLanguage::parseFontDesc), m_namedTimerCountdownNormalFont (+0x80, GlobalLanguage::parseFontDesc), m_namedTimerCountdownReadyFont (+0x8C, GlobalLanguage::parseFontDesc), m_drawableCaptionFont (+0x98, GlobalLanguage::parseFontDesc), m_defaultWindowFont (+0xA4, GlobalLanguage::parseFontDesc), m_defaultDisplayStringFont (+0xB0, GlobalLanguage::parseFontDesc), m_tooltipFontName (+0xBC, GlobalLanguage::parseFontDesc), m_nativeDebugDisplay (+0xC8, GlobalLanguage::parseFontDesc), m_drawGroupInfoFont (+0xD4, GlobalLanguage::parseFontDesc), m_creditsTitleFont (+0xE0, GlobalLanguage::parseFontDesc), m_creditsMinorTitleFont (+0xEC, GlobalLanguage::parseFontDesc), m_creditsNormalFont (+0xF8, GlobalLanguage::parseFontDesc), m_helpBoxNameFont (+0x104, GlobalLanguage::parseFontDesc), m_helpBoxCostFont (+0x110, GlobalLanguage::parseFontDesc), m_helpBoxShortcutFont (+0x11C, GlobalLanguage::parseFontDesc), m_helpBoxDescriptionFont (+0x128, GlobalLanguage::parseFontDesc).
#pragma once
#ifndef BFME2_LANGUAGE_H
#define BFME2_LANGUAGE_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"


class Language
{
public:

	UnsignedByte m_localFontFile[0xC];                     // +0x0 LocalFontFile; unresolved: parseStringEntryRva001EAD69
	AsciiString m_decimalSeparator;                        // +0xC DecimalSeparator
	AsciiString m_thousandSeparator;                       // +0x10 ThousandSeparator
	AsciiString m_timeMinuteToSecondSeparator;             // +0x14 TimeMinuteToSecondSeparator
	AsciiString m_unicodeFontName;                         // +0x18 UnicodeFontName
	UnsignedByte m_unk0x1C[0x4];
	AsciiString m_audioLanguage;                           // +0x20 AudioLanguage
	Bool m_useHardWordWrap;                                // +0x24 UseHardWordWrap
	Int m_militaryCaptionSpeed;                            // +0x28 MilitaryCaptionSpeed
	UnsignedByte m_copyrightFont[0xC];                     // +0x2C CopyrightFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_messageFont[0xC];                       // +0x38 MessageFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_militaryCaptionTitleFont[0xC];          // +0x44 MilitaryCaptionTitleFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_militaryCaptionFont[0xC];               // +0x50 MilitaryCaptionFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_audioSubtitleFont[0xC];                 // +0x5C AudioSubtitleFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_superweaponCountdownNormalFont[0xC];    // +0x68 SuperweaponCountdownNormalFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_superweaponCountdownReadyFont[0xC];     // +0x74 SuperweaponCountdownReadyFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_namedTimerCountdownNormalFont[0xC];     // +0x80 NamedTimerCountdownNormalFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_namedTimerCountdownReadyFont[0xC];      // +0x8C NamedTimerCountdownReadyFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_drawableCaptionFont[0xC];               // +0x98 DrawableCaptionFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_defaultWindowFont[0xC];                 // +0xA4 DefaultWindowFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_defaultDisplayStringFont[0xC];          // +0xB0 DefaultDisplayStringFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_tooltipFontName[0xC];                   // +0xBC TooltipFontName; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_nativeDebugDisplay[0xC];                // +0xC8 NativeDebugDisplay; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_drawGroupInfoFont[0xC];                 // +0xD4 DrawGroupInfoFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_creditsTitleFont[0xC];                  // +0xE0 CreditsTitleFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_creditsMinorTitleFont[0xC];             // +0xEC CreditsMinorTitleFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_creditsNormalFont[0xC];                 // +0xF8 CreditsNormalFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_helpBoxNameFont[0xC];                   // +0x104 HelpBoxNameFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_helpBoxCostFont[0xC];                   // +0x110 HelpBoxCostFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_helpBoxShortcutFont[0xC];               // +0x11C HelpBoxShortcutFont; unresolved: GlobalLanguage::parseFontDesc
	UnsignedByte m_helpBoxDescriptionFont[0xC];            // +0x128 HelpBoxDescriptionFont; unresolved: GlobalLanguage::parseFontDesc
	Real m_resolutionFontAdjustment;                       // +0x134 ResolutionFontAdjustment
};
typedef char Language_off_m_localFontFile[(offsetof(Language, m_localFontFile) == 0x0) ? 1 : -1];
typedef char Language_off_m_decimalSeparator[(offsetof(Language, m_decimalSeparator) == 0xC) ? 1 : -1];
typedef char Language_off_m_thousandSeparator[(offsetof(Language, m_thousandSeparator) == 0x10) ? 1 : -1];
typedef char Language_off_m_timeMinuteToSecondSeparator[(offsetof(Language, m_timeMinuteToSecondSeparator) == 0x14) ? 1 : -1];
typedef char Language_off_m_unicodeFontName[(offsetof(Language, m_unicodeFontName) == 0x18) ? 1 : -1];
typedef char Language_off_m_audioLanguage[(offsetof(Language, m_audioLanguage) == 0x20) ? 1 : -1];
typedef char Language_off_m_useHardWordWrap[(offsetof(Language, m_useHardWordWrap) == 0x24) ? 1 : -1];
typedef char Language_off_m_militaryCaptionSpeed[(offsetof(Language, m_militaryCaptionSpeed) == 0x28) ? 1 : -1];
typedef char Language_off_m_copyrightFont[(offsetof(Language, m_copyrightFont) == 0x2C) ? 1 : -1];
typedef char Language_off_m_messageFont[(offsetof(Language, m_messageFont) == 0x38) ? 1 : -1];
typedef char Language_off_m_militaryCaptionTitleFont[(offsetof(Language, m_militaryCaptionTitleFont) == 0x44) ? 1 : -1];
typedef char Language_off_m_militaryCaptionFont[(offsetof(Language, m_militaryCaptionFont) == 0x50) ? 1 : -1];
typedef char Language_off_m_audioSubtitleFont[(offsetof(Language, m_audioSubtitleFont) == 0x5C) ? 1 : -1];
typedef char Language_off_m_superweaponCountdownNormalFont[(offsetof(Language, m_superweaponCountdownNormalFont) == 0x68) ? 1 : -1];
typedef char Language_off_m_superweaponCountdownReadyFont[(offsetof(Language, m_superweaponCountdownReadyFont) == 0x74) ? 1 : -1];
typedef char Language_off_m_namedTimerCountdownNormalFont[(offsetof(Language, m_namedTimerCountdownNormalFont) == 0x80) ? 1 : -1];
typedef char Language_off_m_namedTimerCountdownReadyFont[(offsetof(Language, m_namedTimerCountdownReadyFont) == 0x8C) ? 1 : -1];
typedef char Language_off_m_drawableCaptionFont[(offsetof(Language, m_drawableCaptionFont) == 0x98) ? 1 : -1];
typedef char Language_off_m_defaultWindowFont[(offsetof(Language, m_defaultWindowFont) == 0xA4) ? 1 : -1];
typedef char Language_off_m_defaultDisplayStringFont[(offsetof(Language, m_defaultDisplayStringFont) == 0xB0) ? 1 : -1];
typedef char Language_off_m_tooltipFontName[(offsetof(Language, m_tooltipFontName) == 0xBC) ? 1 : -1];
typedef char Language_off_m_nativeDebugDisplay[(offsetof(Language, m_nativeDebugDisplay) == 0xC8) ? 1 : -1];
typedef char Language_off_m_drawGroupInfoFont[(offsetof(Language, m_drawGroupInfoFont) == 0xD4) ? 1 : -1];
typedef char Language_off_m_creditsTitleFont[(offsetof(Language, m_creditsTitleFont) == 0xE0) ? 1 : -1];
typedef char Language_off_m_creditsMinorTitleFont[(offsetof(Language, m_creditsMinorTitleFont) == 0xEC) ? 1 : -1];
typedef char Language_off_m_creditsNormalFont[(offsetof(Language, m_creditsNormalFont) == 0xF8) ? 1 : -1];
typedef char Language_off_m_helpBoxNameFont[(offsetof(Language, m_helpBoxNameFont) == 0x104) ? 1 : -1];
typedef char Language_off_m_helpBoxCostFont[(offsetof(Language, m_helpBoxCostFont) == 0x110) ? 1 : -1];
typedef char Language_off_m_helpBoxShortcutFont[(offsetof(Language, m_helpBoxShortcutFont) == 0x11C) ? 1 : -1];
typedef char Language_off_m_helpBoxDescriptionFont[(offsetof(Language, m_helpBoxDescriptionFont) == 0x128) ? 1 : -1];
typedef char Language_off_m_resolutionFontAdjustment[(offsetof(Language, m_resolutionFontAdjustment) == 0x134) ? 1 : -1];

#endif
