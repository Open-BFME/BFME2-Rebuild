// Rva004A2D49.h: the wake-frame setter beside StealthDetectorUpdate. SIZE 0x20.
// Target facts: rva004A2D49 0x004A2D49 pushes UPDATE_SLEEP_NONE (1) or
// UPDATE_SLEEP_FOREVER (0x3FFFFFFF) from its bool argument and calls
// setWakeFrame with the Object at +0x08, so it is an UpdateModule.
#pragma once
#ifndef BFME2_RVA004A2D49_H
#define BFME2_RVA004A2D49_H

#include "Common/GameType.h"
#include "GameLogic/Module/UpdateModule.h"

class Rva004A2D49 : public UpdateModule
{
public:
	Rva004A2D49(Thing *thing, const ModuleData *moduleData);
	virtual ~Rva004A2D49();

	void rva004A2D49(Bool enabled);						// 0x004A2D49
	virtual UpdateSleepTime update() { return UPDATE_SLEEP_FOREVER; }
};
// SIZE 0x20

#endif
