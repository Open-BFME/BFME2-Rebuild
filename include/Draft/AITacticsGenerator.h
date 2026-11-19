// AITacticsGenerator: the tactics generator the tactical AI forwards team registration to. Only the two Register forwards
// are shown (0x009059A1 Register and 0x00905A56 UnRegister are reached from TacticalAI's forwarders); the layout is not.
#pragma once
#ifndef BFME2_AITACTICSGENERATOR_H
#define BFME2_AITACTICSGENERATOR_H

class Team;

class AITacticsGenerator
{
public:
	void Register(Team *team);		// 0x009059A1 (reached by a tail jump from TacticalAI::Register)
	void UnRegister(Team *team);	// 0x00905A56 (reached by a tail jump from TacticalAI::UnRegister)
};

#endif
