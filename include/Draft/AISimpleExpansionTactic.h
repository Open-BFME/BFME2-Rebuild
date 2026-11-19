// AISimpleExpansionTactic.h: the simple expansion tactic. Derives AITactic. Its own bytes are +0x58..+0x60 (create row
// 0x005AAAD6 allocates 0x60); their contents are not shown by the rows written so far.
// Inheritance caveat: the retail tactic chain has a placeholder class between AITactic and these tactics (Rva005DCC24:
// ctor 0x005DCC0A calls the AITactic ctor, dtor 0x005DCC24 tail-jumps to ~AITactic, and each tactic dtor 0x...B tail-jumps
// to it). Until that class has a real name this header derives AITactic directly; sizes and the create/xfer rows agree.
#pragma once
#ifndef BFME2_AISIMPLEEXPANSIONTACTIC_H
#define BFME2_AISIMPLEEXPANSIONTACTIC_H

#include "GameLogic/SkirmishAI/AITactic.h"

class Xfer;

class AISimpleExpansionTactic : public AITactic
{
public:
	AISimpleExpansionTactic();							// 0x005AA9DB
	virtual ~AISimpleExpansionTactic();					// 0x005AA860 (row 11B)
	virtual Bool canRun(void *request);					// not written yet
	virtual void cleanUp();								// 0x005AA88D
	virtual void xfer(Xfer *xfer);						// 0x005AA90D
	virtual void run();								// slot 6: the empty fold (0x004B3FD0)
	virtual void update();								// 0x005AAB08
	virtual AITactic *create();							// 0x005AAAD6, non-base slot 0x24

	unsigned char m_unk0x58[0x8];						// +0x58..+0x60: not shown
};
// SIZE 0x60
typedef char AISimpleExpansionTactic_size[(sizeof(AISimpleExpansionTactic) == 0x60) ? 1 : -1];

#endif
