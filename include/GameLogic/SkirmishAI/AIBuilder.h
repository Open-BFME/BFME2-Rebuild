// AIBuilder.h: the per-player build manager every SkirmishAI starts with, SIZE 0x15C, a plain class with no virtual
// table. The object aggregates the AI builders whose rows sit in the same unit: AIBaseBuilder at +0x04, AIUnitBuilder at
// +0x38, AITeamBuilder at +0x90 (two of the placeholder-named members forward to it), a list head at +0x13C and
// AIDozerManager at +0x140 (the dozer-completion member and registerProducedObject call it). Those members are not typed
// yet, so the bytes are one gap.
#pragma once
#ifndef BFME2_AIBUILDER_H
#define BFME2_AIBUILDER_H

#include <stddef.h>
#include "Common/GameType.h"

class AIBuildable;
class Object;
class Player;
class TeamPrototype;
class Xfer;

class AIBuilder
{
public:
	AIBuilder(Player *player);
	~AIBuilder();

	void update();
	void DoXfer(Xfer *xfer);	// 0x004EC1D9
	void onUnitCreated(Object *factory, Object *unit, Bool isHorde);
	void registerProducedObject(Object *object);	// files a newly owned object under the builders that track its kind
	void registerBuildable(AIBuildable *buildable);
	void unRegisterProducedObject(Object *object);
	void rva004EC072(TeamPrototype *team);	// forwards to the team builder at +0x90
	void rva004EC07D(TeamPrototype *team);	// forwards to the team builder at +0x90
	void rva004EC2D4(ObjectID objectID);	// dozer completion

	unsigned char m_unk0x00[0x15C];
};
typedef char AIBuilderSize[(sizeof(AIBuilder) == 0x15C) ? 1 : -1];

#endif
