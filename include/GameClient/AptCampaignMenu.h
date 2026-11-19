// AptCampaignMenu.h: the in-game campaign menu ("CampaignMenu.apt").
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - Constructor 0x005212FC (one argument for _bfme_AptGameWindow's constructor 0x0051268C), destructor 0x00521206, deleting destructor
//    0x005212E0; tables VA 0x00867720 (primary) and 0x0086771C (AptCallbackHost secondary). SIZE 0x288 (screen factory 0x002D2181).
//  - Members: +0x27C and +0x280 ints and +0x284 a byte (the constructor zeroes all three through one pointer to +0x27C). The byte
//    is the "victorious" flag Victorious 0x0052112F answers. The menu instance is the map's g_Va00E0492C.
//  - The buttons: OnBttnSaveGame and OnBttnLoadGame open AptSaveLoad (kind 3 and 2, mode 1).
#pragma once
#ifndef BFME2_APTCAMPAIGNMENU_H
#define BFME2_APTCAMPAIGNMENU_H

#include "GameClient/BfmeAptGameWindow.h"

class AptCampaignMenu : public _bfme_AptGameWindow
{
public:
	AptCampaignMenu(void *);						// 0x005212FC
	virtual ~AptCampaignMenu();						// 0x00521206

	void Victorious(int, char *, bool);				// 0x0052112F
	void rva0052115F(const char *);					// 0x0052115F (focuses the menu window; no map name)
	void OnBttnMainMenu(const char *);				// 0x00521172: closes the menu and keeps the shell background
	void OnBttnLastMission(const char *);			// 0x0052119F: restarts the current linear campaign mission
	void OnBttnNextMission(const char *);			// 0x005211B9: reports the current mission complete
	void OnBttnSaveGame(const char *);				// 0x0052117D
	void OnBttnLoadGame(const char *);				// 0x0052118E

	int m_x27C;										// +0x27C
	int m_x280;										// +0x280
	bool m_victorious;								// +0x284
	unsigned char m_pad285[3];
};
typedef char AptCampaignMenuSizeCheck[sizeof(AptCampaignMenu)==0x288 ? 1 : -1];

#endif
