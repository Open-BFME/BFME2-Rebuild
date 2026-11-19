// AptScoreScreen.h: the end-of-game score screen ("ScoreScreen.apt").
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - Constructor 0x0051D1E6 (one argument for _bfme_AptGameWindow's constructor 0x0051268C), tables VA 0x00866FAC (primary) and
//    0x00866FA8 (AptCallbackHost secondary). SIZE 0x2C4 (screen factory 0x002D210D). The screen instance is the map's g_00E04914.
//  - Members: +0x27C the step (0 at construction; OnInitialized sets 1 unless it is already 3, RestartGame and Continue set 3),
//    +0x280 an int (2), +0x284 a byte, +0x288 the data block objectiveChecked 0x0051BFF4 reads, +0x28C a 0x14-byte container
//    (constructor 0x0006BE8F), +0x2A0 a window (RenameCancel enables it), +0x2A4 and +0x2A8 ints (RenameCancel clears +0x2A8),
//    +0x2AC an int (-1; RenameCancel resets it), +0x2B0..+0x2BC four ints, +0x2C0 a UnicodeString ("APT:NULL").
#pragma once
#ifndef BFME2_APTSCORESCREEN_H
#define BFME2_APTSCORESCREEN_H

#include <vector>
#include "GameClient/BfmeAptGameWindow.h"
#include "Common/UnicodeString.h"

// The objectives summary objectiveChecked 0x0051BFF4 reads: a count at +0x04 and one checked byte per objective from +0x28.
struct AptScoreObjectives
{
	unsigned char m_pad00[4];
	int m_count;										// +0x04
	unsigned char m_pad08[0x20];
	unsigned char m_checked[8];							// +0x28
};

// The persistent unit a rename applies to: its name is the UnicodeString at +0xA0 (OnButtonRenameAccept 0x0051C17E stores the new text there).
struct AptScoreUnit
{
	unsigned char m_pad00[0xA0];
	UnicodeString m_name;								// +0xA0
};

class AptScoreScreen : public _bfme_AptGameWindow
{
public:
	AptScoreScreen(void *);							// 0x0051D1E6
	virtual ~AptScoreScreen();						// 0x0051D4B2 (deleting destructor's body)

	void OnInitialized(const char *);				// 0x0051BF75: takes the focus unless the screen is finished
	void Continue(const char *);					// 0x0051BFB6: tells the living world and finishes the screen (step 3)
	void Timeline(const char *);					// 0x0051BF9B
	void RestartGame(const char *);					// 0x0051BFA9
	void OnButtonRenameAccept(int);					// 0x0051C17E: stores the typed name in the renamed unit and its list row
	void RenameCancel(const char *);				// 0x0051BFD2
	void objectiveChecked(int index, char *result, bool skip);	// 0x0051BFF4: Apt query "objectiveChecked%d"
	void InitGadgets(const char *name, void *argument, GameWindow *window);	// 0x0051C7CC
	void heroVetUpgrade(int index, char *result, bool skip);	// 0x0051C992: Apt query "heroVetUpgrade%d"
	void rva0051BF2D();								// 0x0051BF2D: finishes the screen once the +0x284 flag was already set
	static bool OpenScreen(int mode, int objectives);	// 0x0051E262
	void CollectGameData();							// 0x0051DE3D

	int m_step;										// +0x27C
	int m_x280;										// +0x280
	bool m_flag284;									// +0x284
	unsigned char m_pad285[3];
	AptScoreObjectives *m_data;						// +0x288
	_STL::vector<bool> m_heroUpgrades;				// +0x28C
	GameWindow *m_unitsList;						// +0x2A0 "PersistentUnitsListBox"
	GameWindow *m_renameEntry;						// +0x2A4 "RenameUnitsTextEntry"
	AptScoreUnit *m_renamedUnit;					// +0x2A8 (the unit being renamed; RenameCancel 0x0051BFD2 clears it)
	int m_x2AC;										// +0x2AC
	int m_x2B0;										// +0x2B0
	int m_x2B4;										// +0x2B4
	int m_x2B8;										// +0x2B8
	int m_x2BC;										// +0x2BC
	UnicodeString m_text;							// +0x2C0
};
typedef char AptScoreScreenSizeCheck[sizeof(AptScoreScreen)==0x2C4 ? 1 : -1];

#endif
