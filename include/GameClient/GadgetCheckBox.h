// GadgetCheckBox.h: the check box gadget's free-function interface, declared from the
// names retail's own map carries (decorated names demangled; every one is a plain
// cdecl function that takes the window first). The per-gadget data struct lives in
// the window's instance data, so none of it is exposed here. Each
// declaration's comment is its retail RVA. Evidence: map identities only; the data
// layouts behind them are recovered with the functions that use them.
#pragma once
#ifndef BFME2_GADGETCHECKBOX_H
#define BFME2_GADGETCHECKBOX_H

#include "GameClient/GameWindow.h"

class AsciiString;
class UnicodeString;
class GameFont;
class Image;

WindowMsgHandledType GadgetCheckBoxInput(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00327938
void GadgetCheckBoxSetChecked(GameWindow *, bool);	// 0x00327AEE
bool GadgetCheckBoxIsChecked(GameWindow *);	// 0x00327B33
WindowMsgHandledType GadgetCheckBoxSystem(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00327B46
void GadgetCheckBoxSetText(GameWindow *, UnicodeString);	// 0x00327BD4

#endif
