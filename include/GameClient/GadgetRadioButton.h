// GadgetRadioButton.h: the radio button gadget's free-function interface, declared from the
// names retail's own map carries (decorated names demangled; every one is a plain
// cdecl function that takes the window first). The per-gadget data struct lives in
// the window's instance data, so none of it is exposed here. Each
// declaration's comment is its retail RVA. Evidence: map identities only; the data
// layouts behind them are recovered with the functions that use them.
#pragma once
#ifndef BFME2_GADGETRADIOBUTTON_H
#define BFME2_GADGETRADIOBUTTON_H

#include "GameClient/GameWindow.h"

class AsciiString;
class UnicodeString;
class GameFont;
class Image;

WindowMsgHandledType GadgetRadioButtonInput(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00327630
void GadgetRadioSetGroup(GameWindow *, int, int);	// 0x003277BC
void GadgetRadioSetSelection(GameWindow *, bool);	// 0x003277D3
WindowMsgHandledType GadgetRadioButtonSystem(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x003277F9
void GadgetRadioSetText(GameWindow *, UnicodeString);	// 0x003278F1

#endif
