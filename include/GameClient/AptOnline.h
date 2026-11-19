// AptOnline.h: the online shell screen's class (the ledger's name for the Apt callbacks "AptOnline::Options", "AptOnline::ShellExit" and
// so on). Only what the rows written so far use is declared; the screen's SIZE and the other members are not recovered yet.
//
// Target facts (retail game.dat): derives from _bfme_AptGameWindow (members from +0x27C). The sub-screens OnlineQuickMatch and
// OnlineHome carry the scope AptOnline:: in the ledger, so they are nested classes (Draft/AptOnline/).
#pragma once
#ifndef BFME2_APTONLINE_H
#define BFME2_APTONLINE_H

#include <vector>
#include "GameClient/BfmeAptGameWindow.h"
#include "GameClient/AptOnlineScreen.h"
#include "Common/AsciiString.h"

// The online shell screen. The ledger names the Apt callbacks "AptOnline::..." (Options, ShellExit, ShellLoadScreen, ShellUnloadScreen,
// ExternFunc) and LoadChildScreen "AptOnlineShell::..."; the members below are the screen's, so AptOnline holds them and
// AptOnlineShell derives it.
class AptOnline : public _bfme_AptGameWindow
{
public:
	class OnlineQuickMatch;							// the sub-screens the shell hosts (Draft/AptOnline/)
	class OnlineHome;

	void Options(const char *);						// 0x00516EC0: "AptOnline::Options" opens the options screen
	void ExternFunc(int query, char *value, bool set);	// 0x00517207: the Apt variables "OnlineShellStartScreen" (0, read only) and "OnlineAdvMode" (1, the GameSpy misc preference InAdvMode)
	void ShellExit(const char *);					// 0x00516ED3: "AptOnline::ShellExit" forgets the two pending screen ids and re-enables the window manager

	bool m_active;									// +0x27C (set by 0x00516EA7, cleared by 0x005174E4)
	unsigned char m_pad27D[3];
	_STL::vector<AptOnlineScreen *> m_children;		// +0x280 the sub-screens the shell hosts (begin +0x280, end +0x284)
	AsciiString m_pendingScreen;					// +0x28C the name of the sub-screen to load next (LoadChildScreen 0x00517027)
	AptOnlineScreen *m_current;						// +0x290 the current sub-screen (0x005174E4 releases it through its slot 2)
	AsciiString m_currentName;						// +0x294 the name of the current sub-screen (ExternFunc 0x00517207 answers it for "OnlineShellStartScreen")
	unsigned char m_gap298[0x2B4 - 0x298];			// +0x298 not recovered
	int m_preferredGroupRoom;						// +0x2B4
};

#endif
