// Retail constructor 0x005A9C1E calls AITacticSiege and installs table 0x00C71E28.
#pragma once
#ifndef BFME2_A_I_SIEGE_GATES_TACTIC_H
#define BFME2_A_I_SIEGE_GATES_TACTIC_H

#include "GameLogic/SkirmishAI/AITacticKinds.h"

class AISiegeGatesTactic : public AITacticSiege
{
public:
	AISiegeGatesTactic();
	virtual ~AISiegeGatesTactic();
	virtual AITactic *create();
	virtual Bool canRun(void *request);	// the base pure slots the class overrides (rows not written)
	virtual void cleanUp() {} // native 0x000B3FD0
	virtual Bool initializeTeamTemplate(void *team, Int count);
	virtual void xfer(Xfer *xfer);
	virtual void run();
	virtual void update();

	Bool rva005A9B20();	// 0x005A9B20: with a wall target, every team attacks it

	Bool m_unk0x5C; // 0x005A9C1E clears the first byte after the Siege base
};
// SIZE 0x60
typedef char AISiegeGatesTactic_size[(sizeof(AISiegeGatesTactic) == 0x60) ? 1 : -1];

#endif
