// AIRoamingDefenseTactic.h: the roaming-defense tactic. Derives AITactic. Its one own field is a bool at +0x58
// (xfer 0x005AABA5 passes &this+0x58 to Xfer's bool operator, table slot 36, after AITactic::xfer), so the object is
// AITactic's 0x58 bytes plus 4 (create row 0x005AADA7 allocates 0x5C). The three bytes after the bool are not shown.
// Inheritance caveat: the retail tactic chain has a placeholder class between AITactic and these tactics (Rva005DCC24:
// ctor 0x005DCC0A calls the AITactic ctor, dtor 0x005DCC24 tail-jumps to ~AITactic, and each tactic dtor 0x...B tail-jumps
// to it). Until that class has a real name this header derives AITactic directly; sizes and the create/xfer rows agree.
#pragma once
#ifndef BFME2_AIROAMINGDEFENSETACTIC_H
#define BFME2_AIROAMINGDEFENSETACTIC_H

#include "GameLogic/SkirmishAI/AITactic.h"

class Xfer;

class AIRoamingDefenseTactic : public AITactic
{
public:
	AIRoamingDefenseTactic();							// 0x005AABC6
	virtual ~AIRoamingDefenseTactic();					// 0x005AAB91 (row 11B)
	virtual Bool canRun(void *request);					// 0x005AAC35
	virtual void cleanUp();								// 0x005AAD26
	virtual Bool initializeTeamTemplate(void *team, Int count);	// 0x005AADD9
	virtual void xfer(Xfer *xfer);						// 0x005AABA5
	virtual void run();									// 0x005AAB9C
	virtual void update();								// 0x005AB085
	virtual AITactic *create();							// 0x005AADA7, non-base slot 0x24

	bool m_flag58;										// +0x58; xfer 0x005AABA5 stores it through Xfer's bool operator
	unsigned char m_unk0x59[3];							// +0x59..+0x5C: not shown
};
// SIZE 0x5C
typedef char AIRoamingDefenseTactic_size[(sizeof(AIRoamingDefenseTactic) == 0x5C) ? 1 : -1];
typedef char AIRoamingDefenseTactic_off_m_flag58[(offsetof(AIRoamingDefenseTactic, m_flag58) == 0x58) ? 1 : -1];

#endif
