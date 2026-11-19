// AptMpClans.h: the clans sub-panel of the multiplayer setup screens (the panel embedded at +0x190 of AptMpGameSetup).
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - SIZE 0xB4 (the gap between the map preview's rules panel at +0x190 and the chat panel at +0x244 of AptMpGameSetup).
//  - Members: +0x58 an embedded GameSpyLoginPreferences-style object (WebSite-sibling rows 0x0057F920 and 0x0057FA1C delete and
//    add clans through it, then notify it through its slot 3), +0xA0 the "ClanName" combo box, +0xA4 the "ClanPlayers" list box
//    (columns 0x41 and 0x23 wide), +0xAC and +0xB0 two strings the clan rows pass to that object.
//  - Rows: WebSite 0x0057F41A, OnGameInfoChange 0x0057F5ED, PopulateMyClans 0x0057F7AC, Delete 0x0057F920, InitGadgets 0x0057F9A3.
#pragma once
#ifndef BFME2_APTMPCLANS_H
#define BFME2_APTMPCLANS_H

class GameWindow;
class UnicodeString;

class AptMpClans
{
public:
	void WebSite(const char *);									// 0x0057F41A: opens the clan wars home page (URL:ClanWarsHome) in the browser
	void OnGameInfoChange();											// 0x0057F5ED: refills the clan players list
	void PopulateMyClans(const UnicodeString &selected);				// 0x0057F7AC
	void InitGadgets(const char *name, void *argument, GameWindow *window);	// 0x0057F9A3
	void rva0057FAB0();													// 0x0057FAB0: its Apt callback registration

	unsigned char m_x00[0xA0];											// +0x00
	GameWindow *m_clanCombo;											// +0xA0 "ClanName"
	GameWindow *m_playersList;											// +0xA4 "ClanPlayers"
	unsigned char m_xA8[0xC];											// +0xA8
};
typedef char AptMpClansSizeCheck[sizeof(AptMpClans)==0xB4 ? 1 : -1];

#endif
