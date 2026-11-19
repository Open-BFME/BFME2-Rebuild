// DisconnectMenu.h: the in-game "player disconnected" menu (Zero Hour's DisconnectMenu by role). Only what the
// DisconnectManager rows prove is declared; its members and size are not recovered yet.
// Target facts: TheDisconnectMenu (VA 0x00E048D0) is tested against null before DisconnectManager::applyDisconnectVote
// 0x004D3CD8 and resetPlayersVotes 0x004D3C32 call updateVotes 0x0051318E with (translated slot, vote count).
#pragma once
#ifndef BFME2_DISCONNECTMENU_H
#define BFME2_DISCONNECTMENU_H

#include "Common/GameType.h"
#include "Common/UnicodeString.h"
#include "GameClient/BfmeAptGameWindow.h"

// The disconnect screen. The ledger and the map spell its members DisconnectMenu:: (updateVotes, showChat, the player-name setter) and the
// Apt callbacks AptDisconnectScreen::, so DisconnectMenu holds the members and AptDisconnectScreen derives it.
class DisconnectMenu : public _bfme_AptGameWindow
{
public:
	void updateVotes(Int slot, Int votes);					// 0x0051318E
	void rva00512EDD(Int position, UnicodeString name);	// 0x00512EDD (shows a disconnected player at a screen position; the map holds it as AptDisconnectScreen::rva00512EDD)
	void rva00513497(Int slot, UnicodeString name);			// 0x00513497 (sets a player's name on the screen; the map has no better name)
	void showChat(UnicodeString text);						// 0x00513138

	GameWindow *m_chatBox;									// +0x27C ("ChatBox")
	GameWindow *m_chatEntry;								// +0x280 ("ChatEntry")
	bool m_refreshPending;									// +0x284
	bool m_flag285;											// +0x285
	bool m_kickButton[8];									// +0x286
	unsigned char m_pad28E[2];
};

extern DisconnectMenu *TheDisconnectMenu;					// VA 0x00E048D0

#endif
