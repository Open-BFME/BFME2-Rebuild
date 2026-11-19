// AIReturnTheRingTactic.h: the return-the-ring tactic. Derives AITactic. Its own fields, from its xfer
// (0x005AB354): an unsigned int at +0x58, ObjectIDs at +0x5C and +0x60, bools at +0x64 and +0x65; two bytes after
// them are not shown. The create row (0x005AB41F) allocates 0x68.
// Inheritance caveat: the retail tactic chain has a placeholder class between AITactic and these tactics (Rva005DCC24:
// ctor 0x005DCC0A calls the AITactic ctor, dtor 0x005DCC24 tail-jumps to ~AITactic, and each tactic dtor 0x...B tail-jumps
// to it). Until that class has a real name this header derives AITactic directly; sizes and the create/xfer rows agree.
#pragma once
#ifndef BFME2_AIRETURNTHERINGTACTIC_H
#define BFME2_AIRETURNTHERINGTACTIC_H

#include "GameLogic/SkirmishAI/AITactic.h"

class Xfer;

class AIReturnTheRingTactic : public AITactic
{
public:
	AIReturnTheRingTactic();							// 0x005AB3BE
	virtual ~AIReturnTheRingTactic();					// 0x005AB309 (row 11B)
	virtual Bool canRun(void *request);					// 0x005AB6A5
	virtual void cleanUp();								// 0x005AB314
	virtual void xfer(Xfer *xfer);						// 0x005AB354
	virtual void run();									// 0x005AB751
	virtual void update();								// 0x005AB71D
	Bool garrisonFortress();						// 0x005AB66A
	virtual AITactic *create();							// 0x005AB41F, non-base slot 0x24

	UnsignedInt m_unk0x58;								// +0x58; xfer 0x005AB354 writes it through the unsigned int operator
	ObjectID m_unk0x5C;									// +0x5C; XferObjectID
	ObjectID m_unk0x60;									// +0x60; XferObjectID
	Bool m_flag64;										// +0x64; xfer through the bool operator
	Bool m_flag65;										// +0x65; likewise
	unsigned char m_unk0x66[0x2];						// +0x66..+0x68: not shown
};
// SIZE 0x68
typedef char AIReturnTheRingTactic_size[(sizeof(AIReturnTheRingTactic) == 0x68) ? 1 : -1];

#endif
