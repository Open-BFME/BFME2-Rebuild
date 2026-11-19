// AIStoredCommand.h: the 0xC4-byte record an AI update keeps of its most recent command. SIZE 0xC4.
// Role name (f03): the map spells the class Rva0026AFDAMember (initMember 0x0026AFDA, destructor 0x0026B03D,
// doXfer 0x003533BF) and Rva00351570 (store 0x00351570 from an AICommandParms, reconstitute 0x0035164E back into
// one). It is a member of AIUpdateInterface (+0x2EC), DeployStyleAIUpdate (+0x3E4) and GiantBirdAIUpdate (+0x3F0).
// Target facts (retail game.dat):
// - initMember 0x0026AFDA sets the command to -1, zeroes the source, the position, the two ObjectIDs, the two strings
//   (null buffers), the trigger, the waypoint, the int and the real, constructs the vector, builds the DamageInfo
//   at +0x40 through its constructor 0x00263895 and zeroes the command button and the path.
// - The destructor 0x0026B03D frees the vector's buffer and releases the two strings (reverse order); the DamageInfo
//   and the two pointers are not destroyed.
// - doXfer 0x003533BF: Version (1, 2), XferAICommandType(command), XferCommandSourceType(source), the position, the
//   two ObjectIDs, the two strings, the coordinate count and each coordinate, the area trigger as its ID (looked up
//   through the terrain logic when loading), the waypoint as its name (looked up by name), the int, the real (version
//   2), the DamageInfo through its slot 0, the command button as its name (ControlBar::findCommandButton), and the
//   path as a present flag followed by Path::DoXfer (a new 0x28-byte Path is built when loading).
// Member names are roles read from those two functions.
#pragma once
#ifndef BFME2_AI_STORED_COMMAND_H
#define BFME2_AI_STORED_COMMAND_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Coord3D.h"
#include "Common/Vector.h"
#include "GameLogic/Damage.h"

class Xfer;
class PolygonTrigger;
class Waypoint;
class CommandButton;
class Path;
struct AICommandParms;

class AIStoredCommand
{
public:
	AIStoredCommand() throw();											// 0x0026AFDA (the map's initMember)
	~AIStoredCommand();													// 0x0026B03D
	void doXfer(Xfer *xfer);											// 0x003533BF
	void store(const AICommandParms &parms);							// 0x00351570 (the map's rva00351570 on the address-named Rva00351570): copies the command out of the argument block
	void reconstitute(AICommandParms &parms) const;						// 0x0035164E: copies it back

	Int m_command;														// +0x00 an AICommandType; -1 for none
	Int m_commandSource;												// +0x04 a CommandSourceType
	Coord3D m_position;													// +0x08
	ObjectID m_object;													// +0x14
	ObjectID m_otherObject;												// +0x18
	AsciiString m_string1;												// +0x1C
	AsciiString m_string2;												// +0x20
	_STL::vector<Coord3D> m_coords;										// +0x24
	PolygonTrigger *m_area;												// +0x30 xfer'd as the trigger's ID (its +0x4)
	Waypoint *m_waypoint;												// +0x34 xfer'd as the waypoint's name (its +0x40)
	Int m_value;														// +0x38
	Real m_facing;														// +0x3C transferred from version 2
	DamageInfo m_damage;												// +0x40 built by DamageInfo's constructor 0x00263895, transferred through its slot 0
	const CommandButton *m_commandButton;								// +0xBC xfer'd as the button's name (its +0x10)
	Path *m_path;														// +0xC0
};
// SIZE 0xC4
typedef char AIStoredCommand_size[(sizeof(AIStoredCommand) == 0xC4) ? 1 : -1];
typedef char AIStoredCommand_off_m_coords[(offsetof(AIStoredCommand, m_coords) == 0x24) ? 1 : -1];
typedef char AIStoredCommand_off_m_area[(offsetof(AIStoredCommand, m_area) == 0x30) ? 1 : -1];
typedef char AIStoredCommand_off_m_commandButton[(offsetof(AIStoredCommand, m_commandButton) == 0xBC) ? 1 : -1];
typedef char AIStoredCommand_off_m_path[(offsetof(AIStoredCommand, m_path) == 0xC0) ? 1 : -1];

#endif
