// AptPlayerStatus.h: the in-game objectives / player-status screen ("ObjectivesMenu.apt"; the strings register it as
// "AptObjectivesMenu::OnInitialized").
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - Constructor 0x004E4A45 (one argument for _bfme_AptGameWindow's constructor 0x0051268C), destructor 0x004E4655, tables VA
//    0x00862260 (primary) and 0x0086225C (AptCallbackHost secondary). SIZE 0x2B4 (screen factory 0x002D1FEB). The screen instance is
//    the map's g_Va00A04450.
//  - Members: +0x27C a vector<int> (the player colours PlayerColor 0x004E45FA indexes), +0x288 the mode (2 at construction; 1
//    when the list is shown), +0x28C a 0x20-byte block and +0x2AC an 8-byte block (both zeroed by memset in the constructor).
#pragma once
#ifndef BFME2_APTPLAYERSTATUS_H
#define BFME2_APTPLAYERSTATUS_H

#include "GameClient/BfmeAptGameWindow.h"
#include "Common/Vector.h"

// The screen's members. The ledger names its window message handler AptObjectivesMenu::OnSystemMsg and the Apt callbacks
// AptPlayerStatus::..., so AptObjectivesMenu holds the members and AptPlayerStatus derives it.
class AptObjectivesMenu : public _bfme_AptGameWindow
{
public:
	int OnSystemMsg(int message, unsigned int data1, unsigned int data2);	// 0x004E4038: a mute check box (message 0x4008) mutes its row's slot

	_STL::vector<int> m_playerColors;				// +0x27C
	int m_mode;										// +0x288
	GameWindow *m_muteBoxes[8];						// +0x28C (InitGadgets 0x004E40C7 stores each "PlayerStatus::Mute<n>" window by its number)
	signed char m_rowSlot[8];						// +0x2AC (the game slot shown by each row, -1 for none)
};

class AptPlayerStatus : public AptObjectivesMenu
{
public:
	AptPlayerStatus(void *);						// 0x004E4A45
	virtual ~AptPlayerStatus();						// 0x004E4655

	void Externs(int, char *, bool);			// 0x004E434C: the Apt extern values (0 the player count in the list mode, 1 and 2 the surrender and forfeit flags)
	void PlayerColor(int, char *, bool);			// 0x004E45FA
	void InitGadgets(const char *, void *, GameWindow *);	// 0x004E40C7: binds the eight "PlayerStatus::Mute<n>" check boxes
	void rva004E476C();								// 0x004E476C
	void rva004E4A34(const char *);					// 0x004E4A34
};
typedef char AptPlayerStatusSizeCheck[sizeof(AptPlayerStatus)==0x2B4 ? 1 : -1];

#endif
