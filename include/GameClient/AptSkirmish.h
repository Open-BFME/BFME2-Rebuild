// AptSkirmish.h: the skirmish setup screen ("Skirmish.apt").
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - Constructor 0x00522B0E (screen argument, game mode), destructor 0x00521977, deleting destructor 0x00521ABD; tables VA
//    0x00867910 (primary), 0x0086790C (AptCallbackHost secondary) and 0x008678B0 (the MpGameSetupOwner base, 23 slots).
//    SIZE 0x6DC (screen factory 0x002D21BB).
//  - Layout: _bfme_AptGameWindow (0x27C), MpGameSetupOwner (+0x27C), the embedded AptMpGameSetup at +0x288 (constructed with
//    mode 0x10000007, then rva004422B4(mode)), a 0x30-byte helper at +0x668 (constructor 0x005C19B6 from the callback host,
//    destructor 0x005C1A36), SkirmishPreferences at +0x698 (0x20 bytes), the state word at +0x6B8, +0x6BC an int, +0x6C0 a byte
//    (1 at construction), +0x6C1 and +0x6C2 bytes, +0x6C4 an int, a window link at +0x6C8 (constructor 0x00521623 with 10,
//    destructor 0x003148E6) and a UnicodeString at +0x6D8.
//  - The state word (+0x6B8): OnInitialized sets 1, OnAddProfileMenu-style callbacks 3 and 4, OnStatsMenu 8, OnExitStatsScreen
//    moves 8 and 9 back to 7, StartGame 10, and OnClosed starts the game (StartGameAfterCDCheck) when it is 11.
//  - The instance pointer is the map's g_00E04930 (an int-typed global) and the screen answers the MpGameSetupOwner slots with
//    AptSkirmish members the map spells QAE or UAE.
#pragma once
#ifndef BFME2_APTSKIRMISH_H
#define BFME2_APTSKIRMISH_H

#include "GameClient/BfmeAptGameWindow.h"
#include "GameClient/MpGameSetupOwner.h"
#include "GameClient/AptMpGameSetup.h"
#include "Common/SkirmishPreferences.h"
#include "Common/UnicodeString.h"

class GameInfo;
class GameWindow;

class AptSkirmish : public _bfme_AptGameWindow, public MpGameSetupOwner
{
public:
	AptSkirmish(void *, int);						// 0x00522B0E
	virtual ~AptSkirmish();							// 0x00521977

	virtual void MpOwnerGetLocalPlayerName(UnicodeString &);	// 0x00521720: slot 14
	virtual bool MPOwnerValidatGameInfo(GameInfo *);			// 0x0052180E
	virtual int OnUpdateData();						// 0x00522290
	// The MpGameSetupOwner overrides this screen's table at VA 0x008678B0 holds directly (the bodies never touch `this`).
	virtual bool MpOwnerSelectColor(GameSlot *, int);			// 0x005216CB: slot 4
	virtual bool MpOwnerSelectMap(const AsciiString &);			// 0x00521C5A: slot 7
	virtual bool MpOwnerSelectPlayer(GameSlot *, SlotState, const UnicodeString &);	// 0x00521B16: slot 9
	virtual bool MpOwnerSelectHandicap(GameSlot *, int);			// 0x005216FD: slot 5
	virtual bool MpOwnerSelectPlayerTemplate(GameSlot *, int);	// 0x005216DB: slot 10
	virtual bool MpOwnerSelectStartPosition(GameSlot *, int);	// 0x0052170D: slot 11
	virtual bool MpOwnerSelectTeam(GameSlot *, int);			// 0x005216ED: slot 12
	virtual void MpOwnerPrintMessage(const UnicodeString &, int);	// 0x00521AC8: slot 17 (the map spells it showError)
	virtual void slot18(bool, int);								// 0x00521B48: slot 18 (the setup panel starts the game: gives the heroes their create-a-hero data)
	virtual int rva005231B0(int, unsigned int, unsigned int);	// 0x005231B0
	virtual void rva00523303();						// 0x00523303

	bool InitSetupPanel();							// 0x005216A6: gives the embedded setup panel the skirmish game
	void OnInitialized(const char *);				// 0x00521741
	void rva0052174E(const char *);					// 0x0052174E
	void StartGame(const char *);					// 0x00521756
	void OnStatsMenu(const char *);					// 0x00521763
	void OnProfilePopupCancel(const char *);		// 0x00521770
	void OnExitStatsScreen(const char *);			// 0x00521826
	void rva00521841();								// 0x00521841
	void InitCreateAHeroOnStartGame();				// 0x005218AF
	void OnAddProfileAccept(const char *);			// 0x00521B56
	void StartGameAfterCDCheck();					// 0x00521CFF
	void OnClosed(const char *);					// 0x00521E65
	void OnNewProfileMenu(const char *);			// 0x00521E76
	void rva00522556();								// 0x00522556
	void OnDeleteProfileMenu(const char *);			// 0x0052266B
	void OnChangeProfileMenu(const char *);			// 0x00522681
	void OnDeleteProfile(const char *);				// 0x00522833
	void OnChangeProfile(const char *);				// 0x005229D3
	void rva00522A91();								// 0x00522A91
	UnicodeString GetListboxProfileSelectedName();	// 0x00522697
	int rva00523178(int, unsigned int, unsigned int);	// 0x00523178: the screen's key handler
	static void OpenScreen(int mode);				// 0x0052192D: pushes "Skirmish.apt" unless the screen is open

	AptMpGameSetup m_setup;							// +0x288
	unsigned char m_helper[0x30];					// +0x668
	SkirmishPreferences m_preferences;				// +0x698
	int m_state;									// +0x6B8
	int m_x6BC;										// +0x6BC
	bool m_flag6C0;									// +0x6C0 (1 at construction)
	bool m_flag6C1;									// +0x6C1
	bool m_flag6C2;									// +0x6C2
	unsigned char m_pad6C3;
	GameWindow *m_profileList;						// +0x6C4: the "Skirmish::SelectProfile" list box (rva00522556 refills it)
	unsigned char m_linkHead[8];					// +0x6C8: a window link (0x10 bytes: 0x6C8..0x6D7)
	GameWindow *m_profileNameEntry;					// +0x6D0: the profile name text entry (OnNewProfileMenu 0x00521E76 focuses it and clears its text)
	unsigned char m_linkTail[4];					// +0x6D4
	UnicodeString m_text;							// +0x6D8
};
typedef char AptSkirmishSizeCheck[sizeof(AptSkirmish)==0x6DC ? 1 : -1];

#endif
