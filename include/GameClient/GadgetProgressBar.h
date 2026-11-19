// GadgetProgressBar.h: the progress bar gadget's free-function interface, declared from the
// names retail's own map carries (decorated names demangled; every one is a plain
// cdecl function that takes the window first). The per-gadget data struct lives in
// the window's instance data, so none of it is exposed here. Each
// declaration's comment is its retail RVA. Evidence: map identities only; the data
// layouts behind them are recovered with the functions that use them.
#pragma once
#ifndef BFME2_GADGETPROGRESSBAR_H
#define BFME2_GADGETPROGRESSBAR_H

#include "GameClient/GameWindow.h"

class AsciiString;
class UnicodeString;
class GameFont;
class Image;

WindowMsgHandledType GadgetProgressBarSystem(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x003215CF
void GadgetProgressBarSetProgress(GameWindow *, int);	// 0x003215FB

#endif
