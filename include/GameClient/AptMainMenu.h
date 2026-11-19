// AptMainMenu.h: the main menu screen ("MainMenu.apt").
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - Constructor 0x00516211 (one argument for _bfme_AptGameWindow's constructor 0x0051268C), destructor 0x00514EAB, deleting
//    destructor 0x0051588B; tables VA 0x00865F10 (primary) and 0x00865F0C (AptCallbackHost secondary). SIZE 0x2AC (screen
//    factory 0x002D1FB1).
//  - Members: +0x27C..+0x282 seven flag bytes (zeroed by the constructor), +0x288 the pending action (the constructor stores
//    8 or 0, from a flag in the global at VA 0x00DFE77C) and +0x28C its argument, a 0x14-byte object at +0x290 (constructor
//    0x005B6ED1), +0x2A4 an int, +0x2A8 a byte ('E' at construction; the campaign handlers copy their argument's first
//    character into it).
//  - The button handlers store the pending action: LoadGame, Options, CreateAHero, WarOfTheRing, LoadCampaign, Skirmish, LoadReplay,
//    GoodCampaign and EvilCampaign set +0x288 to 9 and +0x28C to the kind (1, 4/5, 9, 8, 2, 7, 3, 0xB, 0xC); 0x00514B8D, 0x00514BA1
//    and 0x00514BB5 set +0x288 to 5, 6 and 7 while the menu instance (the map's g_Va00E048DC) exists.
#pragma once
#ifndef BFME2_APTMAINMENU_H
#define BFME2_APTMAINMENU_H

#include "GameClient/BfmeAptGameWindow.h"
#include "Common/AsciiString.h"

class AptMainMenu : public _bfme_AptGameWindow
{
public:
	AptMainMenu(void *);							// 0x00516211
	virtual ~AptMainMenu();							// 0x00514EAB

	virtual int rva00514950(int, int, int);			// 0x00514950

	static int LinearCampaignStart(const AsciiString &campaign, float time, bool restart);	// 0x00514D05

	void LoadGame(const char *);					// 0x00514A9B
	void Options(const char *);						// 0x00514AB2
	void GoodCampaign(const char *);				// 0x00514AE0
	void EvilCampaign(const char *);				// 0x00514B03
	void CreateAHero(const char *);					// 0x00514B26
	void WarOfTheRing(const char *);				// 0x00514B38
	void LoadCampaign(const char *);				// 0x00514B4F
	void rva00514B8D();								// 0x00514B8D
	void rva00514BA1();								// 0x00514BA1
	void rva00514BB5();								// 0x00514BB5
	void Skirmish(const char *);					// 0x00514BC9
	void LoadReplay(const char *);					// 0x00514BE0
	void StopGameMovie(const char *);				// 0x00514BF7
	void ContinueCampaign(const char *);			// 0x00515041: stays on the menu when the campaign autosave exists
	void BattleSchool(const char *);				// 0x00515074
	void ResetResolution(const char *);				// 0x00514C15
	void RenderCredits(void *, void *, void *, void *);	// 0x00514CEB
	void rva00514DC0(int);							// 0x00514DC0
	void rva00514878();								// 0x00514878: cancels a pending action 2 and turns the shell map back on
	void rva00514FE9();								// 0x00514FE9: the tutorial prompt's close: restores the shell's music and reverses the MainMenuToSubMenu transition
	void rva00515633();								// 0x00515633
	void ShowCreditsMenu();							// 0x005158A7: picks the credits menu and runs rva00515633
	void OnInitialized(const char *);				// 0x00514E76

	bool m_flag27C;									// +0x27C (OnInitialized sets it)
	bool m_flag27D;									// +0x27D
	bool m_flag27E;									// +0x27E
	bool m_flag27F;									// +0x27F
	bool m_flag280;									// +0x280
	bool m_flag281;									// +0x281
	bool m_flag282;									// +0x282 (0x00514DC0: set when the action is 8 and the argument is 2)
	unsigned char m_pad283[5];
	int m_action;									// +0x288
	int m_actionArg;								// +0x28C
	unsigned char m_x290[0x14];						// +0x290: an object built by 0x005B6ED1
	AsciiString m_menuName;							// +0x2A4 (ShowCreditsMenu 0x005158A7 sets "CreditsMenu")
	char m_campaignInitial;							// +0x2A8
	unsigned char m_pad2A9[3];
};
typedef char AptMainMenuSizeCheck[sizeof(AptMainMenu)==0x2AC ? 1 : -1];

#endif
