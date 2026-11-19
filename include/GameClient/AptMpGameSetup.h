// AptMpGameSetup.h: the multiplayer game-setup panel that AptLanLobby and AptSkirmish (and the online screens)
// embed: eight player rows of combo boxes (player, colour, team, faction, handicap, hero), the map list and preview,
// the rules, clan and chat sub-panels.
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - Constructor 0x00441ECE (owner, mode), destructor 0x004421E1 (deleting 0x004428AD), table VA 0x00C3DD7C with three
//    slots: 0 the deleting destructor, 1 0x0043FFD7 (resets the panel; AptLanLobby calls it as `[panel]+4`), 2 0x004421DA
//    (returns the map list window at +0x394). Base: AptCallbackHost (constructor 0x002D2C34, called first at 0x00441EE3).
//    SIZE 0x3E0 (AptLanLobby embeds it at +0x288 and its next member starts at +0x668).
//  - Members in constructor order: +0x58 the owner (MpGameSetupOwner*), +0x5C the owner's first target handle (retained:
//    the constructor increments its count at +4; the destructor releases it with 0x0007DEEF), +0x60 the map preview
//    sub-panel (AptMapPreview, constructor 0x0057CF43, 0x70 bytes), +0xD0 the rules sub-panel (0xC0 bytes, base table
//    VA 0x0083D954), +0x190 the clans sub-panel (0xB4 bytes, base table 0x0083D95C, destructor 0x0057F2DE), +0x244 the chat
//    sub-panel (AptMpChat, 0x6C bytes: constructor 0x0057FF3E, destructor 0x0057FE6B), then the panel's own members.
//  - +0x2D4..+0x393: six arrays of eight gadget handles, one per player row. The GameWindow-named gadgets AptMpGameSetup's
//    InitGadgets 0x0043EB1D registers: "Player" +0x2D4, "Color" +0x2F4 (a 4-byte colour-combo handle object bound by 0x0007B719),
//    "Team" +0x314, "PlayerTemplate" +0x334, "Handicap" +0x354, "Hero" +0x374; "MapList" is the map list at +0x394.
//  - +0x398 a vector of AsciiString (the map paths), +0x3A4 the mode flags (the constructor's second argument; bit 0x40 is the
//    clan-list flag), +0x3AC/+0x3B0 the current and previous sort key (the sort rows 0x0043DC75..), +0x3B8/+0x3BC/+0x3C0 the
//    three ping images ("AptPing03", "AptPing02", "AptPing01"), +0x3C4 a heap block the destructor frees, +0x3DC a flag.
#pragma once
#ifndef BFME2_APTMPGAMESETUP_H
#define BFME2_APTMPGAMESETUP_H

#include <vector>
#include "GameClient/AptCallbackHost.h"
#include "GameClient/MpGameSetupOwner.h"
#include "Common/AsciiString.h"
#include "Common/Vector.h"
#include "Apt/AptRef.h"

class GameWindow;
class WinInstanceData;
class Image;
class GameInfo;
class GameSlot;
#include "GameClient/AptMapPreview.h"
#include "GameClient/AptMpClans.h"

// The panel's link to the game being set up (the owner's target handle, 12 bytes: vptr, reference count at +4 (AptRefTarget),
// the game at +8). Its get() 0x0043DA65 validates the game through slot 1 (called with the stored pointer) and clears the
// pointer when it answers false, then returns it; the map folds it into Rva0043DA1BCreate and names no class for it.
class GameInfoTarget : public AptRefTarget
{
public:
	virtual bool slot1(GameInfo *);
	GameInfo *get();													// 0x0043DA65
	GameInfo *m_gameInfo;												// +0x08
};

// The rules sub-panel at +0xD0 is still reached through the placeholder declarations below until its class is recovered; the map
// preview at +0x60 is the real AptMapPreview.
// The map preview's player picker (the map spells its owner Rva0057CA78).
class Rva0057CA78
{
public:
	int GetNextSelectablePlayer(int);									// 0x0057CA78
};
// The window manager's image-store entry (the map spells the owner of 0x002239E2 with this placeholder class).
class Rva002239B2
{
public:
	void rva002239E2(const AsciiString &, const Image *);				// 0x002239E2
};
class Rva0057DFFB
{
public:
	bool rva0057DFFB(bool);												// 0x0057DFFB
};
// The chat sub-panel at +0x244: 0x0057FDB0 has no map name (a fragment of the chat rows), so it is named for its address here.
// The clans sub-panel at +0x190 (0xB4 bytes; the map names its methods AptMpClans::).
class ChatWindowsInGame;
class AptMpChat
{
public:
	void Send(const char *);											// 0x0057FDE0: sends the typed chat line
	void InitGadgets(const char *, void *, GameWindow *);				// 0x0057FDEF: binds the chat window, the player list and the entry to the list object
	void rva0057FD6E();													// 0x0057FD6E
	void rva0057FD94();													// 0x0057FD94: the chat tab was selected
	bool rva0057FD7B(unsigned int, unsigned int, unsigned int);		// 0x0057FD7B: the chat's window message handler
	void rva0057FDB0(bool);												// 0x0057FDB0
	void rva0057FDBF(int index, char *out, bool skip);					// 0x0057FDBF: Apt query

	unsigned char m_x00[0x64];											// +0x00
	ChatWindowsInGame *m_list;											// +0x64 the chat list object (the in-game chat's class)
	bool m_listReady;												// +0x68 cleared while the gadgets are being bound
	unsigned char m_x69[3];											// +0x69
};
typedef char AptMpChatSizeCheck[sizeof(AptMpChat) == 0x6C ? 1 : -1];
class Rva0057E3DB
{
public:
	void rva0057C7BA(int);												// 0x0057C7BA
	int rva0057CDA1(unsigned int, unsigned int, unsigned int);			// 0x0057CDA1: the map preview's window message handler (0x4026 from its combo box: reselects the campaign, 1)
};
class Rva0057EE5C
{
public:
	void rva0057F002(int);												// 0x0057F002
	void rva0057EA0F();													// 0x0057EA0F
	void rva0057E59B(int, bool);										// 0x0057E59B (the map spells its owner Rva0057E97A)
	void rva0057E6D8();													// 0x0057E6D8: the rules tab was selected (a jmp to AptMpGameRules 0x0057E6C1's tail)
	void rva0057EF87();													// 0x0057EF87
};

// The clans sub-panel at +0x190 (the map spells its owner Rva0057F2DE): the text of its +0xA0 combo box, trimmed (HandleClanChange 0x004409D2).
class Rva0057F2DE
{
public:
	UnicodeString rva0057F3A9();										// 0x0057F3A9
	bool rva0057FC75(unsigned int, unsigned int, unsigned int);			// 0x0057FC75: the clans panel's window message handler
};
// A slot's clan name getter (0x003821B9, the map spells its owner Rva003821B9AsciiField; HandleClanChange 0x004409D2 calls it with the slot as this).
class Rva003821B9AsciiField
{
public:
	AsciiString get() const;											// 0x003821B9
};
// The game's rule copy (0x00381CED copies the panel's ten rule values into GameInfo's rules at +0x60; the map spells its owner Rva00381CED).
class Rva00381CED
{
public:
	void rva00381CED(void *);											// 0x00381CED
};
// A slot's hero-index setter (0x003FF0E7 stores its argument at slot +0x5C; the map spells its owner Rva003FF0E7DwordSlot).
class Rva003FF0E7DwordSlot
{
public:
	void set(int);														// 0x003FF0E7
};

class AptMpGameSetup : public AptCallbackHost
{
public:
	AptMpGameSetup(MpGameSetupOwner *owner, int mode);					// 0x00441ECE
	virtual ~AptMpGameSetup();											// 0x004421E1
	virtual void slot1();												// 0x0043FFD7 (the map spells it rva0043FFD7, non-virtual)
	virtual GameWindow *getMapList();									// slot 2, 0x004421DA: the map list window

	void rva004422B4(int);												// 0x004422B4: the panel's mode setter
	void rva0043DB6E();													// 0x0043DB6E
	bool Init(GameInfo *, int);											// 0x00443C6E: binds the panel to a game and fills the eight rows
	bool PopulateColorCombo(int);										// 0x0044009D
	void rva0043EDB4();													// 0x0043EDB4
	void UpdateClans();													// 0x004428C9
	bool OnUpdate();													// 0x00443EA8: runs the pending refreshes
	bool rva0043E1CC(int);												// 0x0043E1CC
	void InitAvailableColors();											// 0x004419FA
	void rva0043FFD7();													// 0x0043FFD7: the panel reset (virtual slot 1)
	void rva0043DC0F();													// 0x0043DC0F
	void rva0043DC40(int);												// 0x0043DC40
	int GetDisplayedPlayerTemplateIndex(int);							// 0x0043DD02
	void rva0043E253(int);												// 0x0043E253: fills the slot's handicap combo
	void rva0043E3E2(int, int);											// 0x0043E3E2
	void ChangePlayerSelection(int, int);								// 0x0043E30F
	void rva0043E49C(const UnicodeString &);							// 0x0043E49C: shows a lobby message
	void OnKickPlayer(const char *);									// 0x0043E4B6
	void rva0043E5C1(int, int, int);									// 0x0043E5C1: sets a slot's connection icon
	const Image *rva0043E512(int);										// 0x0043E512: the ping image for a connection level
	void PopulatePlayerCombo(int);										// 0x0043F483
	void rva0043F244(int, bool);										// 0x0043F244
	void rva0043FA68(GameInfo *);										// 0x0043FA68
	void UpdateReadyIcon(int);											// 0x0043FB5C
	void rva00440017(const AsciiString &);								// 0x00440017: the map changed to the named one
	void rva0043FFCF();													// 0x0043FFCF: rva0043FA68(0)
	void rva004404AC(bool, int);										// 0x004404AC
	void rva004415D5(const AsciiString &);								// 0x004415D5
	void PopulateHeroCombo(int);										// 0x004406BA
	void UpdateHeroToFaction(GameSlot *, int, bool);					// 0x0044149C
	void rva00441685(bool);												// 0x00441685: rva004404AC(flag, -1)
	bool handlePlayerTemplateSelection(int);							// 0x00441AAA
	void UpdateSlot(int);												// 0x00441B15
	void HostValidateColors();											// 0x004422DD
	void PopulatePlayerTemplateCombo(int);								// 0x004424E7
	void rva004427C5(int);												// 0x004427C5: refreshes one slot's combos
	void rva004427FA();													// 0x004427FA: refreshes all eight slots once
	void rva00442A19();													// 0x00442A19: host validation and slot update
	bool rva00442BCC();													// 0x00442BCC
	bool rva00442C9C();													// 0x00442C9C
	void ExternFunc(int, char *, bool);									// 0x00442F65
	void rva00443538(int);												// 0x00443538
	void rva00443BF3();													// 0x00443BF3
	unsigned int rva0043DF22();											// 0x0043DF22: the number of human, non-observer players with data
	void beginStartBusy();												// 0x0043DBA3: the host's start countdown reached zero: marks the panel busy and tells the owner
	bool isStartBusy();													// 0x0043DBCA
	void enable();														// 0x0043DB47 (the map's provisional name): sets the two enable bytes at +0x2B9 and +0x2BA
	void updateRulesPanelEnable();										// 0x0043DBE0: enables the rules panel's entry 8 unless the game's mode (rules +0x1C) is 1 or 2
	bool HandleTeamSelection(int);										// 0x0043DFE0: a slot's team combo changed
	bool HandleHandicapSelection(int);									// 0x0043E0C5: a slot's handicap combo changed
	bool HandleStartPositionSelection(int, int);						// 0x0043E132: a slot's start position changed
	void OnTabSelect(const char *);										// 0x0043DC93: the tab callback ("ChatTab", "RulesTab", "ClansTab", "MapTab")
	bool SetSlotHeroData(GameSlot *, int);								// 0x0043DD34: stores a hero choice on a slot
	void InitGadgets();													// 0x0043DE19
	void InitGadgets(const char *, void *, GameWindow *);				// 0x0043EB1D
	static void PlayerTooltip(GameWindow *, WinInstanceData *, unsigned int);	// 0x0043DF71
	bool HandleHeroSelection(int);										// 0x0043E04D
	bool InitGameInfoFromSaveGame(GameInfo *);							// 0x0043E750
	bool rva0043ECC1(int);												// 0x0043ECC1
	void OnReadyPress(const char *);									// 0x0043EFD3
	void UpdateHeroDisplay(int);										// 0x0043F8B3
	bool rva0043FC1F(int, bool);										// 0x0043FC1F
	bool HandleClanChange();											// 0x004409D2
	bool OnHostStartGame(bool);											// 0x00440BDF
	bool WaitStartGame();												// 0x0044127C
	void UpdatePlayerTemplateDisplay(int);								// 0x00441693
	bool rva00442A68(int);												// 0x00442A68
	int rva00442CB3(unsigned int, unsigned int, unsigned int);			// 0x00442CB3
	void rva0044303D();													// 0x0044303D
	void OnSortName(const char *);										// 0x0043DC75
	void OnSortPlayers(const char *);									// 0x0043DC7F
	void OnSortIcons(const char *);										// 0x0043DC89

	MpGameSetupOwner *m_owner;											// +0x58
	GameInfoTarget *m_ownerTarget;										// +0x5C
	unsigned char m_mapPreviewHead[0x1C];								// +0x60: AptMapPreview (0x70 bytes)
	int m_previewMode;													// +0x7C (inside the preview: 1 is tested by many rows, 0 by 0x00442C9C)
	unsigned char m_mapPreviewMid[0x41];								// +0x80..+0xC0
	bool m_previewFlagC1;												// +0xC1 (set by Init, cleared by slot 1)
	unsigned char m_mapPreviewTail[0xE];								// +0xC2..+0xCF
	unsigned char m_rulesHead[0x8C];									// +0xD0: the rules sub-panel (0xC0 bytes)
	int m_ruleValues[10];												// +0x15C: the sub-panel's ten rule values (AptMpGameRules::m_values; [1] is the mode, 1 is tested by ExternFunc 0x00442F65; [3] the command point factor, 0x0043FC1F)
	unsigned char m_rulesTail[0xC];										// +0x184..+0x18F
	unsigned char m_clans[0xB4];										// +0x190: the clans sub-panel
	unsigned char m_chat[0x6C];											// +0x244: AptMpChat
	int m_x2B0;															// +0x2B0 (0 at construction; reset by slot 1)
	int m_pendingHero;													// +0x2B4 (-3 at construction: no hero choice waits for the template combo; UpdatePlayerTemplateDisplay 0x00441693)
	bool m_flag2B8;														// +0x2B8
	bool m_flag2B9;														// +0x2B9
	bool m_flag2BA;														// +0x2BA
	bool m_flag2BB;														// +0x2BB (not initialised by the constructor; OnUpdate 0x00443EA8 and 0x004409D2 use it)
	bool m_flag2BC;														// +0x2BC: set by rva0043DC40 (the sort changed)
	bool m_flag2BD;														// +0x2BD
	bool m_flag2BE;														// +0x2BE
	bool m_flag2BF;														// +0x2BF
	bool m_flag2C0;														// +0x2C0
	bool m_flag2C1;														// +0x2C1
	bool m_flag2C2;														// +0x2C2
	bool m_flag2C3;														// +0x2C3
	bool m_flag2C4;														// +0x2C4
	unsigned char m_pad2C5[3];
	int m_x2C8;															// +0x2C8
	int m_x2CC;															// +0x2CC
	int m_x2D0;															// +0x2D0
	GameWindow *m_playerCombo[8];										// +0x2D4 "Player"
	GameWindow *m_colorCombo[8];										// +0x2F4 "Color" (stored through the binder 0x0007B719; Init 0x00443C6E enables and shows it like the others)
	GameWindow *m_teamCombo[8];											// +0x314 "Team"
	GameWindow *m_templateCombo[8];										// +0x334 "PlayerTemplate"
	GameWindow *m_handicapCombo[8];										// +0x354 "Handicap"
	GameWindow *m_heroCombo[8];											// +0x374 "Hero"
	GameWindow *m_mapList;												// +0x394 "MapList"
	_STL::vector<AsciiString> m_mapPaths;								// +0x398
	int m_modeFlags;													// +0x3A4
	int m_x3A8;															// +0x3A8
	int m_sortKey;														// +0x3AC (2 at construction)
	int m_prevSortKey;													// +0x3B0
	unsigned char m_pad3B4[4];											// +0x3B4 (zeroed by the constructor's memset)
	const Image *m_ping3;												// +0x3B8 "AptPing03"
	const Image *m_ping2;												// +0x3BC "AptPing02"
	const Image *m_ping1;												// +0x3C0 "AptPing01"
	_STL::vector<bool> m_colorsAvailable;								// +0x3C4: which colours are offered (a 20-byte STLport bit vector; InitAvailableColors 0x004419FA clears, resizes and clears bits; the destructor frees its block with 0x00030830)
	int m_numColors;													// +0x3D8 (InitAvailableColors: the colour count, less the withdrawn ones)
	bool m_flag3DC;														// +0x3DC (1 at construction)
	unsigned char m_pad3DD[3];
};
typedef char AptMpGameSetupSizeCheck[sizeof(AptMpGameSetup)==0x3E0 ? 1 : -1];

#endif
