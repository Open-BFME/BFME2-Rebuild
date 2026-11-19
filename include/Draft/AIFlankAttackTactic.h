// AIFlankAttackTactic.h: the flank-attack tactic. Derives AITactic. Its one own field is a pointer at +0x58 to the
// implementation object (xfer 0x005AA60F calls AIFlankAttackTacticImp::DoXfer through it after AITactic::xfer), so the
// object is AITactic's 0x58 bytes plus 4 (create row 0x005AA2A7 allocates 0x5C).
// Inheritance caveat: the retail tactic chain has a placeholder class between AITactic and these tactics (Rva005DCC24:
// ctor 0x005DCC0A calls the AITactic ctor, dtor 0x005DCC24 tail-jumps to ~AITactic, and each tactic dtor 0x...B tail-jumps
// to it). Until that class has a real name this header derives AITactic directly; sizes and the create/xfer rows agree.
#pragma once
#ifndef BFME2_AIFLANKATTACKTACTIC_H
#define BFME2_AIFLANKATTACKTACTIC_H

#include "GameLogic/SkirmishAI/AITactic.h"
#include "Draft/AIFlankAttackTacticImp.h"

class Xfer;

class AIFlankAttackTactic : public AITactic
{
public:
	AIFlankAttackTactic();								// 0x005AA23E
	virtual ~AIFlankAttackTactic();						// 0x005AA55D
	virtual Bool canRun(void *request);					// 0x005AA0D8
	virtual void cleanUp();
	virtual void xfer(Xfer *xfer);						// 0x005AA60F
	virtual void run();									// 0x005AA5AE
	virtual void update();								// 0x005AA1EA
	virtual AITactic *create();							// 0x005AA2A7, non-base slot 0x24

	AIFlankAttackTacticImp *m_impl;						// +0x58
};
// SIZE 0x5C
typedef char AIFlankAttackTactic_size[(sizeof(AIFlankAttackTactic) == 0x5C) ? 1 : -1];
typedef char AIFlankAttackTactic_off_m_impl[(offsetof(AIFlankAttackTactic, m_impl) == 0x58) ? 1 : -1];

#endif
