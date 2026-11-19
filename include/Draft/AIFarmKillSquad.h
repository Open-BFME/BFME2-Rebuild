// AIFarmKillSquad.h: the farm-kill squad tactic. Derives AITactic. Its own fields are two ObjectIDs at +0x58 and +0x5C
// (xfer 0x005ACEC8 runs XferObjectID on each) and four bytes after them that no row shows, so the object is
// AITactic's 0x58 bytes plus 12 (create row 0x005ACFAB allocates 0x64).
// Inheritance caveat: the retail tactic chain has a placeholder class between AITactic and these tactics (Rva005DCC24:
// ctor 0x005DCC0A calls the AITactic ctor, dtor 0x005DCC24 tail-jumps to ~AITactic, and each tactic dtor 0x...B tail-jumps
// to it). Until that class has a real name this header derives AITactic directly; sizes and the create/xfer rows agree.
#pragma once
#ifndef BFME2_AIFARMKILLSQUAD_H
#define BFME2_AIFARMKILLSQUAD_H

#include "GameLogic/SkirmishAI/AITactic.h"

class Xfer;

class AIFarmKillSquad : public AITactic
{
public:
	AIFarmKillSquad();									// 0x005ACF38
	virtual ~AIFarmKillSquad();							// 0x005ACCE4 (row 11B)
	virtual Bool canRun(void *request);					// 0x005ACCEF
	virtual void cleanUp();								// 0x005ACD51
	virtual Bool initializeTeamTemplate(void *team, Int count);	// 0x005ACFE7
	virtual void xfer(Xfer *xfer);						// 0x005ACEC8
	virtual void run();
	virtual void update();								// 0x005AD5DD
	virtual AITactic *create();							// 0x005ACFAB, non-base slot 0x24

	ObjectID m_unk0x58;									// +0x58; xfer 0x005ACEC8 runs XferObjectID on it
	ObjectID m_unk0x5C;									// +0x5C; likewise
	unsigned char m_unk0x60[0x4];						// +0x60..+0x64: not shown
};
// SIZE 0x64
typedef char AIFarmKillSquad_size[(sizeof(AIFarmKillSquad) == 0x64) ? 1 : -1];

#endif
