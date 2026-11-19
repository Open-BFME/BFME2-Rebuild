// ScriptTracker.h: the sequential-script runner that every AIUpdateInterface carries at +0x3D0.
// Target facts: the constructor 0x00262436 (inline in retail, stores owner, clears the script, sets +8 to 1 and
// clears +9..+0xB) is run by AIUpdateInterface's own constructor with this = the member; setCurScript 0x00262453
// (called by AIGroup 0x0036E2A7 and ScriptAction 0x003B3D75) and update 0x0026249D read the owner at +0 and the
// flag bytes at +8..+0xB. Size 0xC (the distance to AIUpdateInterface +0x3DC). Role names only: nothing in the
// binary spells the flag bytes.
#pragma once
#ifndef BFME2_SCRIPTTRACKER_H
#define BFME2_SCRIPTTRACKER_H

#include "Common/GameType.h"

class AIUpdateInterface;
class SequentialScript;

class ScriptTracker
{
public:
	ScriptTracker(AIUpdateInterface *owner)
		: m_owner(owner), m_script(0), m_idle(true), m_flag9(false), m_flagA(false), m_flagB(false)
	{
	}

	void setCurScript(SequentialScript *script, Bool flag, Int argument);	// 0x00262453
	void update();															// 0x0026249D

private:
	AIUpdateInterface *m_owner;		// +0x00
	SequentialScript *m_script;		// +0x04
	Bool m_idle;					// +0x08 (set by the constructor, 1)
	Bool m_flag9;					// +0x09
	Bool m_flagA;					// +0x0A
	Bool m_flagB;					// +0x0B
};

#endif // BFME2_SCRIPTTRACKER_H
