// AICommandParms.h: the argument block AICommandInterface's command wrappers fill and
// pass to aiDoCommand. SIZE 0xC0.
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE. Target facts: the constructor 0x00351BD0 takes the
// command type and the command source, zeroes +0x14..+0x1C, +0x2C..+0x34, +0x38 and
// +0xB8/+0xBC, builds the vector at +0x20 and a 0x7C-byte sub-object at +0x3C. The
// wrappers aiTightenToPosition 0x0036EA74, aiMoveToAndEvacuate 0x0036EAE0 and the rest
// copy the Coord3D to +0x08 (three movs), store an extra Int at +0x34 and a Real at +0x38,
// and the implicit destructor frees the vector at +0x20. Command types: 2 tighten,
// 3 move-and-evacuate, 4 move-evacuate-and-exit, 0xC force-attack, 0xD attack-team,
// 0x14..0x16 resume/heal/repair, 0x19 harvest, 0x1A exit, 0x1E..0x1F guard, 0x21 guard-area,
// 0x23 attack-area, 0x35 follow-path-append, 0x4E move with a facing, 0x51 attack-move with
// a facing, 0x52 amphibious move.
#pragma once
#ifndef BFME2_AICOMMANDPARMS_H
#define BFME2_AICOMMANDPARMS_H

#include "Common/GameType.h"
#include "Common/Coord3D.h"
#include "Common/Vector.h"
#include "GameLogic/Damage.h"

class AICommandInterface;
class Object;
class Team;
class Waypoint;
class PolygonTrigger;

// The command numbers aiDoCommand 0x002673F6 switches on (BFME2's own numbering; callers cast the
// number). The map names the constructor ??0AICommandParms@@QAE@W4AICommandType@@W4CommandSourceType@@@Z.
enum AICommandType
{
	AICMD_NO_COMMAND = -1
};

struct AICommandParms
{
	// 0x00351BD0: a COMDAT (expected in AIStates.cpp; /O1 keeps it out of line at every caller): Zero Hour's
	// initializer list and body (m_pos.zero(), m_coords.clear() = the erase(begin, end) call 0x002A133B).
	AICommandParms(AICommandType type, CommandSourceType cmdSource)
		: m_type(type), m_cmdSource(cmdSource), m_object(NULL), m_unk0x18(0), m_team(NULL),
		  m_waypoint(NULL), m_area(NULL), m_extraInt(0), m_facing(0.0f), m_unk0xB8(0), m_unk0xBC(0)
	{
		m_pos.zero();
		m_unk0x20.clear();
	}

	Int m_type;										// +0x00, the command (AICommandType)
	CommandSourceType m_cmdSource;					// +0x04
	Coord3D m_pos;									// +0x08, the position argument
	Object *m_object;								// +0x14, the target object of the object commands
	Int m_unk0x18;									// +0x18
	const Team *m_team;								// +0x1C, the team of aiAttackTeam
	_STL::vector<ObjectID> m_unk0x20;				// +0x20, freed by the implicit destructor
	const Waypoint *m_waypoint;						// +0x2C, the path of the waypoint commands
	const PolygonTrigger *m_area;						// +0x30, the area of the area commands
	Int m_extraInt;									// +0x34, the Int argument of the attack-move wrapper
	Real m_facing;									// +0x38, the facing argument of the *AndFaceDirection wrappers
	DamageInfo m_damage;							// +0x3C, built by DamageInfo's constructor 0x00263895 inside the out-of-line AICommandParms constructor (f03: the 0x7C-byte sub-object is a DamageInfo; AIStoredCommand keeps the same one at +0x40)
	Int m_unk0xB8;									// +0xB8
	Int m_unk0xBC;									// +0xBC
};
// SIZE 0xC0

#endif
