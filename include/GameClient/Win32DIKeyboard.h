// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// DirectInputKeyboard: the keyboard subsystem that reads buffered keys from DirectInput 8 (Zero Hour's Win32DIKeyboard.h).
// Target facts: the constructor 0x00098B55 runs Keyboard's constructor 0x00232920, zeroes the two pointers at +0xE20 and +0xE24,
// stores VA 0x00BC8688 and asks GetKeyState(VK_CAPITAL); the factory 0x0004C6DF allocates 0xE28 bytes. The table overrides
// init (slot 1, 0x00098CAC), update (slot 10, the one-instruction jump 0x00098BC3 to Keyboard's 0x00232BC7), getCapsState
// (slot 14, 0x00098BC8) and getKey (slot 16, 0x00098AAC); the deleting destructor is 0x00098C90 over 0x00098B8B.
#pragma once
#ifndef BFME2_WIN32DIKEYBOARD_H
#define BFME2_WIN32DIKEYBOARD_H

#ifndef DIRECTINPUT_VERSION
#define DIRECTINPUT_VERSION 0x800
#endif

#include <windows.h>
#include <dinput.h>

#include "GameClient/Keyboard.h"

class DirectInputKeyboard : public Keyboard
{
public:
	DirectInputKeyboard();							// 0x00098B55
	virtual ~DirectInputKeyboard();					// 0x00098B8B
	virtual void init();							// 0x00098CAC
	virtual void update();							// 0x00098BC3
	virtual bool getCapsState();					// 0x00098BC8

protected:
	virtual void getKey(KeyboardIO *key);			// 0x00098AAC
	void openKeyboard();							// 0x00098BD4
	void closeKeyboard();							// 0x00098A6F

	LPDIRECTINPUT8 m_pDirectInput;					// +0xE20
	LPDIRECTINPUTDEVICE8 m_pKeyboardDevice;			// +0xE24
};

typedef char DirectInputKeyboardSize[(sizeof(DirectInputKeyboard) == 0xE28) ? 1 : -1];

#endif
