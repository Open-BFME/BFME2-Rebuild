// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// KeyDefs: the key codes of KeyboardIO::key (DirectInput scan codes: Zero Hour's KeyDefType, whose values are the DIK_ constants;
// written as the scan codes themselves so this header needs no dinput.h). Retail evidence: Keyboard::translateKey 0x00232763
// switches on 0x1D (KEY_LCTRL), 0x2A (KEY_LSHIFT), 0x36 (KEY_RSHIFT), 0x38 (KEY_LALT), 0x3A (KEY_CAPS), 0x9D (KEY_RCTRL) and
// 0xB8 (KEY_RALT); DirectInputKeyboard::getKey 0x00098AAC stores 0 for no key and 0xFF when the device was lost and re-acquired.
#pragma once
#ifndef BFME2_KEYDEFS_H
#define BFME2_KEYDEFS_H

enum KeyDefType
{
	// keypad keys
	KEY_KP0 = 0x52,			// DIK_NUMPAD0
	KEY_KP1 = 0x4F,
	KEY_KP2 = 0x50,
	KEY_KP3 = 0x51,
	KEY_KP4 = 0x4B,
	KEY_KP5 = 0x4C,
	KEY_KP6 = 0x4D,
	KEY_KP7 = 0x47,
	KEY_KP8 = 0x48,
	KEY_KP9 = 0x49,
	KEY_KPDEL = 0x53,		// DIK_NUMPADPERIOD
	KEY_KPSTAR = 0x37,		// DIK_NUMPADSTAR
	KEY_KPMINUS = 0x4A,
	KEY_KPPLUS = 0x4E,

	// regular keys
	KEY_ESC = 0x01,
	KEY_BACKSPACE = 0x0E,
	KEY_ENTER = 0x1C,
	KEY_SPACE = 0x39,
	KEY_TAB = 0x0F,
	KEY_F1 = 0x3B,
	KEY_F2 = 0x3C,
	KEY_F3 = 0x3D,
	KEY_F4 = 0x3E,
	KEY_F5 = 0x3F,
	KEY_F6 = 0x40,
	KEY_F7 = 0x41,
	KEY_F8 = 0x42,
	KEY_F9 = 0x43,
	KEY_F10 = 0x44,
	KEY_F11 = 0x57,
	KEY_F12 = 0x58,
	KEY_A = 0x1E,
	KEY_B = 0x30,
	KEY_C = 0x2E,
	KEY_D = 0x20,
	KEY_E = 0x12,
	KEY_F = 0x21,
	KEY_G = 0x22,
	KEY_H = 0x23,
	KEY_I = 0x17,
	KEY_J = 0x24,
	KEY_K = 0x25,
	KEY_L = 0x26,
	KEY_M = 0x32,
	KEY_N = 0x31,
	KEY_O = 0x18,
	KEY_P = 0x19,
	KEY_Q = 0x10,
	KEY_R = 0x13,
	KEY_S = 0x1F,
	KEY_T = 0x14,
	KEY_U = 0x16,
	KEY_V = 0x2F,
	KEY_W = 0x11,
	KEY_X = 0x2D,
	KEY_Y = 0x15,
	KEY_Z = 0x2C,
	KEY_1 = 0x02,
	KEY_2 = 0x03,
	KEY_3 = 0x04,
	KEY_4 = 0x05,
	KEY_5 = 0x06,
	KEY_6 = 0x07,
	KEY_7 = 0x08,
	KEY_8 = 0x09,
	KEY_9 = 0x0A,
	KEY_0 = 0x0B,
	KEY_MINUS = 0x0C,
	KEY_EQUAL = 0x0D,		// DIK_EQUALS
	KEY_LBRACKET = 0x1A,
	KEY_RBRACKET = 0x1B,
	KEY_SEMICOLON = 0x27,
	KEY_APOSTROPHE = 0x28,
	KEY_TICK = 0x29,		// DIK_GRAVE
	KEY_BACKSLASH = 0x2B,
	KEY_COMMA = 0x33,
	KEY_PERIOD = 0x34,
	KEY_SLASH = 0x35,

	// special keys
	KEY_SYSREQ = 0xB7,		// DIK_SYSRQ

	KEY_CAPS = 0x3A,		// DIK_CAPSLOCK
	KEY_NUM = 0x45,			// DIK_NUMLOCK
	KEY_SCROLL = 0x46,
	KEY_LCTRL = 0x1D,		// DIK_LCONTROL
	KEY_LALT = 0x38,
	KEY_LSHIFT = 0x2A,
	KEY_RSHIFT = 0x36,

	KEY_UP = 0xC8,			// DIK_UPARROW
	KEY_DOWN = 0xD0,
	KEY_LEFT = 0xCB,
	KEY_RIGHT = 0xCD,
	KEY_RALT = 0xB8,
	KEY_RCTRL = 0x9D,		// DIK_RCONTROL
	KEY_HOME = 0xC7,
	KEY_END = 0xCF,
	KEY_PGUP = 0xC9,
	KEY_PGDN = 0xD1,
	KEY_INS = 0xD2,			// DIK_INSERT
	KEY_DEL = 0xD3,			// DIK_DELETE
	KEY_KPENTER = 0x9C,
	KEY_KPSLASH = 0xB5,

	KEY_102 = 0x56,			// DIK_OEM_102

	// Japanese keyboard keys
	KEY_KANA = 0x70,
	KEY_CONVERT = 0x79,
	KEY_NOCONVERT = 0x7B,
	KEY_YEN = 0x7D,
	KEY_CIRCUMFLEX = 0x90,
	KEY_KANJI = 0x94,

	// specials
	KEY_NONE = 0x00,		// to report the end of the key stream
	KEY_LOST = 0xFF			// to report lost keyboard focus
};

#endif
