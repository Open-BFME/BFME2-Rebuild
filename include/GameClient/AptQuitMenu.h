// AptQuitMenu.h: the in-game quit menu ("QuitMenu.apt").
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - Constructor 0x0051BADF (one argument for _bfme_AptGameWindow's constructor 0x0051268C), tables VA 0x00866C58 (primary) and
//    0x00866C54 (AptCallbackHost secondary). SIZE 0x284 (screen factory 0x002D2099). The menu instance is the global the map names
//    TheAptQuitMenu (VA 0x00E04910).
//  - Members: +0x27C, +0x27D and +0x27E bytes (zeroed by the constructor: 0x27C is set by ExitMission, 0x27E by RestartMission,
//    0x27D is set to 1 by AptSaveLoad's destructor while a quit-menu save is pending), +0x280 an int: the close mode the close helper
//    0x0051AF0B stores (0 return to the game, 2 exit the mission). +0x278 (the _bfme_AptGameWindow byte) is the closing flag.
#pragma once
#ifndef BFME2_APTQUITMENU_H
#define BFME2_APTQUITMENU_H

#include "GameClient/BfmeAptGameWindow.h"

class AptQuitMenu : public _bfme_AptGameWindow
{
public:
	AptQuitMenu(void *);							// 0x0051BADF
	virtual ~AptQuitMenu();							// 0x0051B8EF

	void RestartMission(const char *);				// 0x0051AFB9
	void ExitMission(const char *);					// 0x0051AFC3
	void ReturnToGame(const char *);				// 0x0051AFEE
	int rva0051B125(int message, unsigned char key, int state);	// 0x0051B125: the Escape key opens the quit menu when no save or options screen is open
	void OptionsScreen(const char *);				// 0x0051AFDD: opens the options screen with no flags
	void SaveMenu(const char *);					// 0x0051AFF9: opens the save screen in the current game kind
	void Externs(int index, char *buffer, bool query);	// 0x0051AF46: the quit menu's Apt externs (index 1 answers the restart button's label)
	void HandleOverRestartButton(const char *);	// 0x0051B50E: the tooltip of the restart, forfeit or surrender button
	void LoadMenu(const char *);					// 0x0051B04A: opens the load screen in the current game kind
	int rva0051BED7();								// 0x0051BED7 (slot of table 0x00866C6C): the polled restart request: restarts the mission or closes the menu; always answers 1
	void rva0051B09B();								// 0x0051B09B (the map spells it Rva0051B09BEnable; the caller sets ecx to the menu): closes the menu and resumes the game

	bool m_flag27C;									// +0x27C: set by ExitMission
	bool m_flag27D;									// +0x27D
	bool m_flag27E;									// +0x27E: set by RestartMission
	unsigned char m_pad27F;
	int m_closeMode;								// +0x280
};
typedef char AptQuitMenuSizeCheck[sizeof(AptQuitMenu)==0x284 ? 1 : -1];
extern AptQuitMenu *TheAptQuitMenu;				// VA 0x00E04910

#endif
