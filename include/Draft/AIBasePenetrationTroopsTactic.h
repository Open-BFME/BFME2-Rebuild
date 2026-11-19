// AIBasePenetrationTroopsTactic.h: the base-penetration tactic. Derives AITactic and adds no members: its create row
// 0x005A9A57 allocates 0x58 bytes, AITactic's size. Overrides are the rows' own slots (run 0x005A9916, update 0x005A9923,
// cleanUp 0x005A994B, canRun 0x005A99D7) plus the constructor 0x005A9988 and deleting destructor.
// Inheritance caveat: the retail tactic chain has a placeholder class between AITactic and these tactics (Rva005DCC24:
// ctor 0x005DCC0A calls the AITactic ctor, dtor 0x005DCC24 tail-jumps to ~AITactic, and each tactic dtor 0x...B tail-jumps
// to it). Until that class has a real name this header derives AITactic directly; sizes and the create/xfer rows agree.
#pragma once
#ifndef BFME2_AIBASEPENETRATIONTROOPSTACTIC_H
#define BFME2_AIBASEPENETRATIONTROOPSTACTIC_H

#include "GameLogic/SkirmishAI/AITactic.h"

class Xfer;

class AIBasePenetrationTroopsTactic : public AITactic
{
public:
	AIBasePenetrationTroopsTactic();					// 0x005A9988
	virtual ~AIBasePenetrationTroopsTactic();			// 0x005A990B (row 11B)
	virtual Bool canRun(void *request);					// 0x005A99D7
	virtual void cleanUp();								// 0x005A994B
	virtual void run();									// 0x005A9916
	virtual void update();								// 0x005A9923
	virtual AITactic *create();							// 0x005A9A57, non-base slot 0x24
};
// SIZE 0x58 (no members of its own)
typedef char AIBasePenetrationTroopsTactic_size[(sizeof(AIBasePenetrationTroopsTactic) == 0x58) ? 1 : -1];

#endif
