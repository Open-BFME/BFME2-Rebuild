// GadgetTextEntry.h: the text entry gadget's free-function interface, declared from the
// names retail's own map carries (decorated names demangled; every one is a plain
// cdecl function that takes the window first). The per-gadget data struct lives in
// the window's instance data, so none of it is exposed here. Each
// declaration's comment is its retail RVA. Evidence: map identities only; the data
// layouts behind them are recovered with the functions that use them.
#pragma once
#ifndef BFME2_GADGETTEXTENTRY_H
#define BFME2_GADGETTEXTENTRY_H

#include "GameClient/GameWindow.h"

class AsciiString;
class UnicodeString;
class GameFont;
class Image;

// The text entry's user data (GameWindow::winGetUserData): only the cursor and the anchor of the selection are established.
struct GadgetTextEntryData
{
	unsigned char m_gap0x0[0x1C];
	short m_charPos;		// +0x1C (HasSelection 0x00444016 compares it with the next word)
	short m_conCharPos;		// +0x1E
};

void GadgetTextEntrySetText(GameWindow *, UnicodeString);	// 0x002C17EB
class BfmeKeyLC;
void GadgetTextEntrySetMaxChars(BfmeKeyLC *, unsigned short);	// 0x00433D07 (the map's spelling)
void GadgetTextEntrySetValidationFlags(GameWindow *, int);	// 0x0032060D
void GadgetTextEntrySetFont(GameWindow *, GameFont *);	// 0x00320641
void GadgetTextEntrySetCursorPosition(GameWindow *, unsigned int);	// 0x00320737
WindowMsgHandledType GadgetTextEntrySystem(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x003207F5
UnicodeString GadgetTextEntryGetText(GameWindow *);	// 0x00320AAB
bool GadgetTextEntryUpdateComposition(GameWindow *);	// 0x00320B3C
bool GadgetTextEntryInsertCharacter(GameWindow *, unsigned int);	// 0x00320C6F
WindowMsgHandledType GadgetTextEntryInput(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00320DA9
void GadgetTextEntrySetTextColor(GameWindow *, int);	// 0x00322430
bool GadgetTextEntryHasSelection(GameWindow *);	// 0x00444016

#endif
