// AptSaveLoad.h: the Apt save/load screen ("SaveLoad").
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - Constructor 0x00436259 (one argument for _bfme_AptGameWindow's constructor 0x0051268C), destructor 0x00435DE7,
//    deleting destructor 0x00436197; tables VA 0x0083CDC8 (primary) and 0x0083CDC4 (the AptCallbackHost secondary).
//    SIZE 0x2B0 (the screen factory 0x002D20D3 allocates 0x2B0).
//  - Members from the constructor and the rows: +0x27C int state (the Apt dialogs' step machine: 0 idle, 1 and 3 begin
//    saving/loading, 6 a pending game, 10 closing, 0xE..0x17 confirmations: ConfirmationOk 0x004357CF switches on 0x12..0x15),
//    +0x280 the pending item (AptSaveLoadPending below, zero until a list row is picked: Load 0x004348CA stores
//    rva00433F7F's result), +0x284 byte (the game's pause state at construction, restored by the destructor through
//    GameLogic 0x0023CD9E), +0x288/+0x28C the two list boxes rva00433F7F reads, +0x290 a window the enable helper 0x00434432
//    enables, +0x294 int dialog kind (2 Load, 3 Save), +0x298 the game-kind filter bits, +0x29C and +0x29D bytes (the constructor stores 0 and 1),
//    +0x2A0 int (the dialog mode: 4 and 0x10 are tested), +0x2A4 byte, +0x2A8 int (written by rva00433D71), +0x2AC a
//    pooled list (4 bytes: ctor 0x00434916 allocates its 0xDFC-byte head node, destructor 0x00435378).
#pragma once
#ifndef BFME2_APTSAVELOAD_H
#define BFME2_APTSAVELOAD_H

#include "GameClient/BfmeAptGameWindow.h"

// One entry of the save list (0xDF4 bytes: the list node is 0xDFC = 8 + 0xDF4; copy constructor 0x0022D106). Only the two
// fields the screen reads are typed: the kind at +0x28 (6 = a game in progress, 7 = a replay, ...) and the byte at +0xDE4.
class GameSpyGameSlot;
class AptSaveLoadPending
{
public:
	unsigned char m_head[0x28];			// +0x00..+0x27 not recovered
	int m_kind;							// +0x28
	unsigned char m_body[0xDB8];			// +0x2C..+0xDE3 not recovered
	bool m_flagDE4;						// +0xDE4
	unsigned char m_tail[0xF];			// +0xDE5..+0xDF3 not recovered
};
typedef char AptSaveLoadPendingSizeCheck[sizeof(AptSaveLoadPending)==0xDF4 ? 1 : -1];

class AptSaveLoad : public _bfme_AptGameWindow
{
public:
	AptSaveLoad(void *);				// 0x00436259
	virtual ~AptSaveLoad();				// 0x00435DE7

	static AptSaveLoad *s_instance;					// VA 0x00E032E0: the open screen
	static void OpenScreen(int, int, bool);	// 0x00434160: pushes SaveLoad.apt (kind 2 Load, 3 Save)
	void rva00433D71(int);				// 0x00433D71
	static GameSpyGameSlot *GetOpponentSlotData();	// 0x00433FF3: the first human opponent's slot of the current game
	static bool PromptAddFriend();		// 0x004354F3
	int rva00434132(int, unsigned char, unsigned char);	// 0x00434132: the key handler (primary slot 1; the map spells its owner Rva00434132)
	void Externs(int, char *, bool);		// 0x00433DFE
	void SetDefaultFileName();			// 0x00434AAE
	void InitGadgets(const char *, void *, GameWindow *);	// 0x00435172
	void rva0043684C();					// 0x0043684C
	void rva00436FF6();					// 0x00436FF6
	void rva00433D96();					// 0x00433D96: closes the screen after the "1" refresh
	void OnClosed(const char *);			// 0x00433DB1
	void Cancel(const char *);			// 0x00433DE0
	void rva00433DF1(const char *);		// 0x00433DF1
	int rva00433F7F();					// 0x00433F7F: the selected list item (a pointer)
	void rva00433FDD();					// 0x00433FDD
	void rva004340AC();					// 0x004340AC
	void Delete(const char *);			// 0x00434318
	void rva00434432();					// 0x00434432: enables the lists, then 0x004340AC
	void rva0043448C();					// 0x0043448C
	void Load(const char *);			// 0x004348CA
	void ConfirmationOk(const char *);	// 0x004357CF
	void rva00434B8A();					// 0x00434B8A
	void rva00435224();					// 0x00435224

	int m_state;						// +0x27C
	AptSaveLoadPending *m_pending;		// +0x280
	bool m_wasPaused;					// +0x284
	unsigned char m_pad285[3];
	GameWindow *m_listA;				// +0x288
	GameWindow *m_listB;				// +0x28C
	GameWindow *m_listC;				// +0x290
	int m_dialogKind;					// +0x294: 2 = Load, 3 = Save (Externs 0x00433DFE answers "Load"/"Save")
	int m_typeFilter;					// +0x298: the game kinds listed: 1 Campaign, 2 Skirmish, 4 Replay, 8 WOTRSP, 0x10 WOTRMP (the bit names Externs returns)
	bool m_flag29C;						// +0x29C
	bool m_flag29D;						// +0x29D
	unsigned char m_pad29E[2];
	int m_mode;							// +0x2A0
	bool m_flag2A4;						// +0x2A4
	unsigned char m_pad2A5[3];
	int m_x2A8;							// +0x2A8
	void *m_saves;						// +0x2AC: the pooled list's head pointer
};
typedef char AptSaveLoadSizeCheck[sizeof(AptSaveLoad)==0x2B0 ? 1 : -1];

#endif
