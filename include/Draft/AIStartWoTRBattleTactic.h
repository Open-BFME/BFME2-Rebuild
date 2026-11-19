// AIStartWoTRBattleTactic.h: the War-of-the-Ring battle tactic. Derives AITactic and adds no members, so it is
// AITactic's 0x58 bytes (the create row 0x005AB203 allocates 0x58 for it; the constructor 0x005AB1B4 calls the base).
// Overrides listed here are the derived vtable slots the rows name; cleanUp and update are the base's pure slots.
// Inheritance caveat: the retail tactic chain has a placeholder class between AITactic and these tactics (Rva005DCC24:
// ctor 0x005DCC0A calls the AITactic ctor, dtor 0x005DCC24 tail-jumps to ~AITactic, and each tactic dtor 0x...B tail-jumps
// to it). Until that class has a real name this header derives AITactic directly; sizes and the create/xfer rows agree.
#pragma once
#ifndef BFME2_AISTARTWOTRBATTLETACTIC_H
#define BFME2_AISTARTWOTRBATTLETACTIC_H

#include "GameLogic/SkirmishAI/AITactic.h"

class Xfer;

class AIStartWoTRBattleTactic : public AITactic
{
public:
	AIStartWoTRBattleTactic();							// 0x005AB1B4
	virtual ~AIStartWoTRBattleTactic();					// 0x005AB125 (deleting form at slot 0)
	virtual Bool canRun(void *request);					// 0x005AB130
	virtual void cleanUp();
	virtual void xfer(Xfer *xfer);						// 0x005AB188
	virtual void run();									// 0x005AB235
	virtual void update();
	virtual AITactic *create();							// 0x005AB203, non-base slot 0x24
};
// SIZE 0x58 (no members of its own)
typedef char AIStartWoTRBattleTactic_size[(sizeof(AIStartWoTRBattleTactic) == 0x58) ? 1 : -1];

#endif
