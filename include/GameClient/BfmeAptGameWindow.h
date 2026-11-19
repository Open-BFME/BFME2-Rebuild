// BfmeAptGameWindow.h: _bfme_AptGameWindow, the window every Apt screen derives from.
//
// Target facts (retail game.dat; RVAs, table addresses are VAs):
//  - Constructor 0x0051268C (takes one argument, handed to GameWindow's constructor 0x00313847),
//    deleting destructor 0x005127D7 over the body 0x005126F5; both are unrowed in this tree.
//  - Layout: GameWindow (0x218), the AptCallbackHost secondary base at +0x218 (0x58 bytes, ctor 0x002D2C34
//    called at 0x005126B2), then +0x270 an AsciiString (zeroed inline; the destructor releases it
//    with 0x00036410), +0x274 an int initialised to -1 (the owner value the screen passes to the window
//    manager's invoke: AptLanLobby::CancelMenu 0x00444E8A reads it), +0x278 a byte (zeroed). SIZE 0x27C.
//  - Primary table VA 0x00C659E8, 14 slots: GameWindow's eleven (0x0080BBCC) with the Apt overrides at
//    1 (0x005748AD), 2 (0x0051274F, the message handler a screen overrides) and slot 7 (0x00510D87, the
//    screen's window factory), then three slots introduced here (0x2C, 0x30, 0x34) that screens override
//    (AptLanLobby 0x004442DB and 0x00446772 are two of them). Secondary table VA 0x00C659E4, one slot
//    (the deleting destructor's adjustor, 0x005126EA: `sub ecx, 0x218`).
#pragma once
#ifndef BFME2_BFMEAPTGAMEWINDOW_H
#define BFME2_BFMEAPTGAMEWINDOW_H

#include "GameClient/GameWindow.h"
#include "GameClient/AptCallbackHost.h"
#include "Common/AsciiString.h"

class _bfme_AptGameWindow : public GameWindow, public AptCallbackHost
{
public:
	_bfme_AptGameWindow(void *);		// 0x0051268C
	virtual ~_bfme_AptGameWindow();	// 0x005127D7

	int rva0051274F(int message, unsigned int data1, unsigned int data2);	// 0x0051274F: slot 2, the message handler a screen overrides (the map spells its signature this way)

	AsciiString m_screenName;			// +0x270: released by the destructor
	void *m_callbackOwner;				// +0x274: -1 until set; passed as the owner of window-manager invokes
	bool m_flag278;					// +0x278: zeroed by the constructor
};
typedef char BfmeAptGameWindowSizeCheck[sizeof(_bfme_AptGameWindow)==0x27C ? 1 : -1];

#endif
