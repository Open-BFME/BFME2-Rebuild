// AISiegeGatesTactic.h: an offensive siege tactic. Only the size is shown: the create row (0x005A9C71) allocates 0x60 bytes; its constructor
// (0x005A9C1E) is the class's own. The retail class derives AITacticSiege (its xfer calls that base); until AITacticSiege has a
// size this draft derives AITactic with 8 bytes of unshown tail. The placeholder tactic base (Rva005DCC24) caveat applies.
#pragma once
#ifndef BFME2_A_I_SIEGE_GATES_TACTIC_H
#define BFME2_A_I_SIEGE_GATES_TACTIC_H

#include "GameLogic/SkirmishAI/AITactic.h"

class AISiegeGatesTactic : public AITactic
{
public:
	AISiegeGatesTactic();
	virtual AITactic *create();
	virtual Bool canRun(void *request);	// the base pure slots the class overrides (rows not written)
	virtual void cleanUp();
	virtual void run();
	virtual void update();

	unsigned char m_unk0x58[0x8];	// +0x58..+0x60: not shown
};
// SIZE 0x60
typedef char AISiegeGatesTactic_size[(sizeof(AISiegeGatesTactic) == 0x60) ? 1 : -1];

#endif
