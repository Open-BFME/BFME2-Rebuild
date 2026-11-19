// AptMpGameRules.h: the rules sub-panel of AptMpGameSetup (ten rule gadgets: combo boxes and check boxes).
//
// Target facts (retail game.dat, RVAs; table addresses are VAs):
//  - The panel embeds it at +0xD0 (0xC0 bytes) as the class the map spells Rva0043DABD, whose constructor 0x0043DAA5 runs this
//    class's constructor 0x0057EDB2 and stores the table VA 0x0083D954 (two slots: the deleting destructor and the stub 0x0043FE01
//    taking (int, bool)). AptMpGameSetup's AptCallbackHost base is repeated here: the first 0x58 bytes are an AptCallbackHost.
//  - Members (offsets from the sub-object): +0x58 the link to the game being set up (the same GameInfoTarget AptMpGameSetup holds),
//    +0x5C an int, +0x60 the rule set (the argument 0x00559F7E/0x00559F95/0x00559FAC take), +0x64..+0x6F and +0x70..+0x7B two
//    vectors, +0x7C the array of ten gadget pointers (0x00 when a gadget is absent), +0x80 its end, +0x84 an int, +0x88 a flag that
//    UpdateRuleGadget clears while it writes and sets afterwards, +0x89 a flag (set once the gadgets are populated), +0x8C the ten rule
//    values (ints), then 0x0C bytes up to 0xC0.
#pragma once
#ifndef BFME2_APTMPGAMERULES_H
#define BFME2_APTMPGAMERULES_H

#include <vector>
#include "GameClient/AptCallbackHost.h"

struct AptMpGameRulesWidgets;						// a vector of gadget pointers (the two vectors at +0x64 and +0x70)
class GameInfoTarget;
class GameWindow;
class GameInfo;

class AptMpGameRules : public AptCallbackHost
{
public:
	virtual ~AptMpGameRules();										// slot 0
	virtual void OnRuleChanged(int, bool);							// slot 1 (0x0043FE01: the stub of the embedded class hands the change to the panel instance's rva0043FC1F)

	bool rva0057E707(int, GameWindow *, int);						// 0x0057E707: the rules panel's window message handler
	void EnableRuleGadget(int, bool);							// 0x0057E59B: enables or disables one rule gadget (the map spells its owner Rva0057E97A)
	void ExternFunc(int, char *, bool);							// 0x0057E55A
	void UpdateRuleGadget(int);									// 0x0057E5B5
	void rva0057E6C1();											// 0x0057E6C1: UpdateRuleGadget for every rule
	int GetGadgetValue(int);									// 0x0057E66A
	bool rva0057ED2B();											// 0x0057ED2B
	void rva0057E97A();											// 0x0057E97A: enables each gadget whose state is 1 when the local player hosts
	void rva0057E9ED(unsigned int index, int state);			// 0x0057E9ED: stores one gadget state
	void PopulateComboBoxes();									// 0x0057EAAD
	void PopulateCheckBoxes();									// 0x0057E7A1
	void InitGadgets(const char *, void *, GameWindow *);		// 0x0057F036
	void rva0057EF18();											// 0x0057EF18
	void rva0057EF46(AptMpGameRulesWidgets *, const char *, GameWindow *);	// 0x0057EF46
	void Reset(const char *);									// 0x0057E6DD
	void rva0057E779(int, int);									// 0x0057E779: stores a rule value and refreshes its gadget

	GameInfoTarget *m_target;										// +0x58
	int m_x5C;														// +0x5C
	int m_ruleSet;													// +0x60
	_STL::vector<int> m_vector64;									// +0x64 (4-byte entries; rva0057ED2B compares its size with 0x00559F7E)
	_STL::vector<int> m_vector70;									// +0x70 (4-byte entries; compared with 0x00559F95)
	GameWindow **m_gadgets;											// +0x7C
	GameWindow **m_gadgetsEnd;										// +0x80
	int m_x84;														// +0x84
	bool m_updating;												// +0x88
	bool m_populated;												// +0x89
	unsigned char m_pad8A[2];
	int m_values[10];												// +0x8C
	_STL::vector<int> m_ruleStates;									// +0xB4: one state per gadget (1 enables the gadget for the host)
};
typedef char AptMpGameRulesSizeCheck[sizeof(AptMpGameRules)==0xC0 ? 1 : -1];

#endif
