// AIStructureCreepTactic.h: the structure-creep tactic. Derives AITactic. Only its size is shown so far: the create row
// (0x005AB9AF) allocates 0x80 bytes, so the 0x28 bytes after AITactic's 0x58 are not typed yet. Overrides are the
// rows' own slots. Inheritance caveat as in the other tactic drafts (the placeholder Rva005DCC24 base is not named).
#pragma once
#ifndef BFME2_AISTRUCTURECREEPTACTIC_H
#define BFME2_AISTRUCTURECREEPTACTIC_H

#include "GameLogic/SkirmishAI/AITactic.h"

class AIStructureCreepTactic : public AITactic
{
public:
	AIStructureCreepTactic();							// 0x005AB91D
	virtual Bool canRun(void *request);			// the base pure slots the class overrides (rows not written)
	virtual void cleanUp();
	virtual void run();
	virtual void update();
	virtual AITactic *create();							// 0x005AB9AF, non-base slot 0x24

	unsigned char m_unk0x58[0x10];						// +0x58..+0x68: not shown
	UnsignedInt m_unk0x68;						// +0x68: create (0x005AB9AF) copies the creator's dword here after the constructor
	unsigned char m_unk0x6C[0x14];					// +0x6C..+0x80: not shown
};
// SIZE 0x80
typedef char AIStructureCreepTactic_size[(sizeof(AIStructureCreepTactic) == 0x80) ? 1 : -1];

#endif
