// AptOnlineQuickMatch.h: the quick match lobby screen ("QuickMatch.apt" panel of the online shell): the AptOnline::OnlineQuickMatch
// sub-screen base plus the preference combo boxes.
//
// Target facts (retail game.dat; RVAs):
//  - Derives AptOnline::OnlineQuickMatch (Draft/AptOnline/OnlineQuickMatch.h: the shell pointer at +0x58, the state at +0x60, the
//    QMPref preferences object at +0x64). The screen's own members start at +0x7C: the three combo boxes the preference rows fill,
//    "players versus players" at +0x84 (0x005BA626: entries 1 versus 1 and 2 versus 2, selected by the preferences' player count),
//    the maximum ping at +0x8C (0x005BA6EE: one entry per hundred milliseconds up to the GameSpy configuration's ping timeout, then
//    "Any") and the ladder at +0x90 (0x005BA36D reads the selected ladder's list item).
#pragma once
#ifndef BFME2_APTONLINEQUICKMATCH_H
#define BFME2_APTONLINEQUICKMATCH_H

#include "Draft/AptOnline/OnlineQuickMatch.h"

class GameWindow;

class AptOnlineQuickMatch : public AptOnline::OnlineQuickMatch
{
public:
	bool rva005BA626();										// 0x005BA626: refills the players-versus-players combo box
	bool rva005BA6EE();										// 0x005BA6EE: refills the maximum ping combo box

	unsigned char m_gap7C[0x84 - 0x7C];						// +0x7C not recovered
	GameWindow *m_playersCombo;								// +0x84
	unsigned char m_gap88[4];								// +0x88 not recovered
	GameWindow *m_pingCombo;								// +0x8C
	GameWindow *m_ladderCombo;								// +0x90
};

#endif
