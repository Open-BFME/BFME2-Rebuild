// GadgetSlider.h: the slider gadget's free-function interface, declared from the
// names retail's own map carries (decorated names demangled; every one is a plain
// cdecl function that takes the window first). The per-gadget data struct lives in
// the window's instance data, so none of it is exposed here. Each
// declaration's comment is its retail RVA. Evidence: map identities only; the data
// layouts behind them are recovered with the functions that use them.
#pragma once
#ifndef BFME2_GADGETSLIDER_H
#define BFME2_GADGETSLIDER_H

#include "GameClient/GameWindow.h"

class AsciiString;
class UnicodeString;
class GameFont;
class Image;

int GadgetSliderGetEnabledColor(GameWindow *);	// 0x0009FCCC
int GadgetSliderGetEnabledBorderColor(GameWindow *);	// 0x0009FCD4
int GadgetSliderGetDisabledColor(GameWindow *);	// 0x0009FD08
int GadgetSliderGetDisabledBorderColor(GameWindow *);	// 0x0009FD13
int GadgetSliderGetHiliteColor(GameWindow *);	// 0x0009FD4A
int GadgetSliderGetHiliteBorderColor(GameWindow *);	// 0x0009FD55
WindowMsgHandledType GadgetVerticalSliderInput(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00321620
WindowMsgHandledType GadgetVerticalSliderSystem(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x003218B0
WindowMsgHandledType GadgetHorizontalSliderInput(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00321B50
WindowMsgHandledType GadgetHorizontalSliderSystem(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00321E5C
int GadgetSliderGetEnabledSelectedThumbBorderColor(GameWindow *);	// 0x00323D86
int GadgetSliderGetDisabledSelectedThumbBorderColor(GameWindow *);	// 0x00323DB4
int GadgetSliderGetHiliteSelectedThumbBorderColor(GameWindow *);	// 0x00323DE8
void GadgetSliderGetMinMax(GameWindow *, int *, int *);	// 0x0051822B
int GadgetSliderGetPosition(GameWindow *);	// 0x00518246

#endif
