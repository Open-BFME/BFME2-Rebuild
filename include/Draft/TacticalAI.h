// TacticalAI: the tactical side of the skirmish AI. Partial: the Register and UnRegister forwarders (0x002C5FAA,
// 0x002C5FB2) load the generator pointer at +0x10 and tail-jump to AITacticsGenerator; nothing else is shown yet.
#pragma once
#ifndef BFME2_TACTICALAI_H
#define BFME2_TACTICALAI_H

#include "Draft/AITacticsGenerator.h"

class Team;

class TacticalAI
{
public:
	void Register(Team *team);		// 0x002C5FAA
	void UnRegister(Team *team);	// 0x002C5FB2

	unsigned char m_unk0x00[0x10];	// +0x00..+0x10: not shown
	AITacticsGenerator *m_generator10;	// +0x10: forwarded to AITacticsGenerator::Register / UnRegister
};

#endif
