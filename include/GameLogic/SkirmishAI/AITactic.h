// AITactic.h: the base of the skirmish AI's tactics (AITacticalAI/AITacticsGenerator). SIZE 0x58.
// Target facts (retail 1.06, read from the disassembly; no donor declaration used):
//  - Layout: vptr +0x00; vector<TeamPrototype *> +0x04 (the NotifyTeamCreated walk compares its
//    elements with Team's prototype at +0x30); byte flag +0x10; vector of 0x14-byte team records
//    +0x14 (record +0x00 = team ID, +0x10 = the value isTeamIdle compares with the threshold);
//    target pointer +0x20 and owning Player pointer +0x24, both zeroed by the constructor; AsciiString name +0x2C;
//    unsigned id +0x30, taken from a process-wide counter when the tactic is built; flag +0x34 (the
//    constructor's second argument); two Coord3D at +0x38 and +0x44; flags +0x50/+0x51; dword +0x54.
//  - Vtable (VA 0x00C629E4, nine slots): 0 deleting destructor, 1 and 2 and 6 and 7 pure, 3
//    initializeTeamTemplate, 4 a function that returns 1 (its name is unproved), 5 xfer, 8 the
//    nonvirtual row rva004EDDAA.
//  - The constructor's vptr store is the first write; the two vectors and the string are built
//    with their own constructors, so the layout above is the order in which retail constructs them.
#pragma once
#ifndef BFME2_AITACTIC_H
#define BFME2_AITACTIC_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"
#include "Common/Vector.h"
#include "GameLogic/AITarget.h"

class Xfer;
class Player;
class Object;
class Team;
class TeamPrototype;

// One team record of the AI tactic: 0x14 bytes.
struct AITacticTeamRecord
{
	Int m_teamID;					// +0x00
	unsigned char m_unk0x04[0x0C];	// +0x04
	Int m_value;					// +0x10
};

// The map spells the record type Rva004ECECDNode (rows 0x004ECECD, 0x004ECF05).
struct Rva004ECECDNode : public AITacticTeamRecord {};

class AITactic
{
public:
	AITactic(const AsciiString &name, Bool flag);	// 0x004ED3D1
	virtual ~AITactic();							// slot 0
	virtual Bool canRun(void *request) = 0;			// slot 1 (pure)
	virtual void cleanUp() = 0;						// slot 2 (pure)
	virtual Bool initializeTeamTemplate(void *team, Int count);	// slot 3, 0x004ECE61
	virtual unsigned int vslot04() { return 1; }	// slot 4, native 0x000910FB
	virtual void xfer(Xfer *xfer);					// slot 5, 0x004EDA8C
	virtual void run() = 0;							// slot 6 (pure)
	virtual void update() = 0;						// slot 7 (pure)

	unsigned char rva004ED169();		// 0x008ED169 (the map name; the base test the update uses)
	void end(Bool first, Bool second);	// 0x008ED748
	Bool anyTeamMembersInCombat();		// 0x004ECF41: true when a member of any of the tactic's teams has a victim
	void cohereTeams();				// 0x004ED08A: runs each team through a temporary AI group
	Rva004ECECDNode *rva004ECF05(Int teamID);	// 0x004ECF05 (map: placeholder returning a Rva004ECECDNode *): the record of the team that owns that player id
	Bool isTeamIdle(Int index);		// 0x004ED134
	void teamAttackObject(Int id, Object *victim);	// 0x004ED1A1
	void teamGarrisonObject(Int id, Object *building);	// 0x004ED2ED
	void teamAttackMove(Int id, const Coord3D *point);	// 0x004ED1FD
	void teamForceMove(Int id, const Coord3D *point);	// 0x004ED257 (the map's name; rva004ED372 calls it per team)
	void rva004ED342(void *point);		// 0x004ED342 (the map name)
	void rva004ED372(const Coord3D *point);	// 0x004ED372 (the map name): rva004ED342's twin, teamForceMove for every team
	void NotifyTeamCancelled(Team *team);	// 0x004EDD4D
	virtual void vslot08();							// slot 8, row 0x004EDDAA

	_STL::vector<TeamPrototype *> m_prototypes;	// +0x04
	Bool m_active;				// +0x10; set by roaming defense run and read by return-the-ring update
	_STL::vector<AITacticTeamRecord> m_teams;	// +0x14
	AITarget *m_owner20;						// +0x20 (the target object; AITarget.h)
	Player *m_player;			// +0x24; roaming update 0x005AB085 passes it to SkirmishAIManager (0x002A8F24)
	Bool m_flag28;								// +0x28
	AsciiString m_name;							// +0x2C
	unsigned int m_id;							// +0x30
	Bool m_flag34;								// +0x34
	Coord3D m_pos38;							// +0x38
	Coord3D m_pos44;							// +0x44
	Bool m_flag50;								// +0x50
	Bool m_flag51;								// +0x51
	unsigned int m_field54;						// +0x54
};
// SIZE 0x58

typedef char AITacticSizeCheck[(sizeof(AITactic) == 0x58) ? 1 : -1];

#endif
