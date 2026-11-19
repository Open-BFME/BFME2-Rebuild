// GadgetPushButton.h: the push button (and check-like button) gadget's free-function interface, declared from the
// names retail's own map carries (decorated names demangled; every one is a plain
// cdecl function that takes the window first). The per-gadget data struct lives in
// the window's instance data, so none of it is exposed here. Each
// declaration's comment is its retail RVA. Evidence: map identities only; the data
// layouts behind them are recovered with the functions that use them.
#pragma once
#ifndef BFME2_GADGETPUSHBUTTON_H
#define BFME2_GADGETPUSHBUTTON_H

#include "GameClient/GameWindow.h"
#include "Common/OpaqueRefElement4.h"

class AsciiString;
class UnicodeString;
class GameFont;
class Image;

// The push button's own data (what the window's user data points at). SIZE 0x38; constructor 0x00327E22.
// Evidence (retail): the constructor zeroes every field; GadgetButtonDrawClock 0x00328485 and 0x003284B9 set the
// byte at +0 (1 and 2) with a value at +4 and a colour at +8; GadgetButtonSetBorder 0x00328454 sets the byte at +0xC
// and the colour at +0x10; the setter 0x00328518 and GadgetButtonGetData 0x00327D56 use the pointer at +0x14;
// 0x003284ED sets +0x18; 0x0032857F sets the byte at +0x20; 0x00328543 sets +0x24 and the window status bit
// 0x2000000; 0x00327D86 switches the display string at +0x30 on or off with the flag at +0x28 and clears +0x2C;
// GadgetButtonSetDisallowed 0x003285AA sets the byte at +0x34. The field at +0x1C is cleared by the constructor
// and released by the destructor 0x00327E50 through Release_Ref 0x00050ED3 (an OpaqueRefElement4).
class DisplayString;
struct _PushButtonData
{
	_PushButtonData();
	~_PushButtonData();

	unsigned char m_drawClock;			// +0x00 0 none, 1 and 2 the two clock forms
	int m_clockPercent;					// +0x04
	int m_clockColor;					// +0x08
	unsigned char m_drawBorder;			// +0x0C
	unsigned int m_borderColor;			// +0x10
	void *m_userData;					// +0x14
	int m_field18;						// +0x18
	OpaqueRefElement4 m_field1C;		// +0x1C an owning handle: the destructor 0x00327E50 releases it
	unsigned char m_field20;			// +0x20
	int m_field24;						// +0x24
	unsigned char m_showDisplayString;	// +0x28
	int m_field2C;						// +0x2C
	DisplayString *m_displayString;		// +0x30
	unsigned char m_disallowed;			// +0x34
};
// SIZE 0x38

void GadgetButtonSetEnabledImage_Rva002C0433(GameWindow *, const Image *);	// 0x002C0433 (placeholder spelling in the map)
void GadgetButtonSetHiliteImage_Rva002C04DB(GameWindow *, const Image *);	// 0x002C04DB
void GadgetButtonSetHiliteImage123_Rva002C0505(GameWindow *, const Image *);
void GadgetButtonSetDisabledImage_Rva002C0487(GameWindow *, const Image *);	// 0x002C0487 (placeholder spelling in the map)	// 0x002C0505
void GadgetButtonSetEnabledColor(GameWindow *, int);	// 0x002C074F
void GadgetCheckLikeButtonSetVisualCheck(GameWindow *, bool);	// 0x00327C61
bool GadgetCheckLikeButtonIsChecked(GameWindow *);	// 0x00327C9B
void GadgetButtonEnableCheckLike(GameWindow *, bool, bool);	// 0x00327CB9
void * GadgetButtonGetData(GameWindow *);	// 0x00327D56
void Rva003284B9(GameWindow *, int, int);	// 0x003284B9 (the clock overlay: percent and colour; placeholder spelling in the map; ControlBar::updateContextMultiSelect 0x0053DF0A)
void Rva003284ED(GameWindow *, int);	// 0x003284ED (sets the overlay image word at +0x18; ZH's GadgetButtonDrawOverlayImage)
void Rva00328518(GameWindow *, int);	// 0x00328518 (the data pointer's setter; placeholder spelling in the map)
WindowMsgHandledType GadgetPushButtonInput(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00327E5D
void GadgetButtonSetText(GameWindow *, UnicodeString);	// 0x003283F7
void GadgetButtonSetBorder(GameWindow *, unsigned int, bool);	// 0x00328454
void GadgetButtonDrawClock(GameWindow *, int, unsigned int);	// 0x00328485
void GadgetButtonSetDisallowed(GameWindow *, int);	// 0x003285AA
WindowMsgHandledType GadgetPushButtonSystem(GameWindow *, unsigned int, unsigned int, unsigned int);	// 0x00328639
void GadgetButtonSetAltSound(GameWindow *, AsciiString);	// 0x0032884A

#endif
