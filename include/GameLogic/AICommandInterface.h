// AICommandInterface.h: SAGE AI command entry points (Zero Hour AI.h).
// Target facts (old repo Code/GameEngine/Include/GameLogic/Module/AIUpdate.h):
// AIUpdateInterface derives from UpdateModule (0x20 bytes), then this interface,
// so it sits at AIUpdateInterface+0x20; BloodthirstyUpdate (0x0044E4A9) adds 0x20
// to Object::getAI's result before calling aiIdle. Slot 0 is aiDoCommand; the
// ai* helpers are non-virtual.
#pragma once
#ifndef BFME2_AICOMMANDINTERFACE_H
#define BFME2_AICOMMANDINTERFACE_H

#include "Common/GameType.h"
#include "Common/Coord3D.h"

class Object;
class PolygonTrigger;
class Team;
class Waypoint;
enum GuardMode;
class Rva003427DD;	// the map's placeholder spelling of aiGoProne's record type (a DamageInfo at its call sites)
struct AICommandParms;

class AICommandInterface
{
public:
	virtual void aiDoCommand(const AICommandParms *parms) = 0;	// 0

	// Every mapped AICommandInterface helper with the signature its decorated
	// name states (rows that need placeholder types are left out).
	void aiIdle(CommandSourceType);	// 0x001E8A38
	void aiMoveToPosition(Coord3D const *, CommandSourceType);	// 0x0026C26D
	void rva0026C2D9(Object *, int, CommandSourceType);	// 0x0026C2D9
	void rva0026C347(Object *, CommandSourceType);	// 0x0026C347
	void aiDock(Object *, CommandSourceType);	// 0x0026C3AC
	void aiMoveAwayFromUnit(Object *, Coord3D const *, CommandSourceType);	// 0x0026C411
	void rva0026C486(Object *, CommandSourceType);	// 0x0026C486
	void aiAttackPosition(Coord3D const *, int, CommandSourceType);	// 0x0029599A
	void aiHunt(CommandSourceType);	// 0x002AE657
	void aiEvacuate(bool, CommandSourceType);	// 0x002AE6B3
	void aiMoveToObject(Object *, CommandSourceType);	// 0x00352ECA
	void aiMoveToPositionAndFaceDirection(Coord3D const *, CommandSourceType, float);	// 0x0036E906
	void aiAttackMoveToPosition(const Coord3D *pos, Int maxShotsToFire, CommandSourceType cmdSource);	// 0x00295A0F (the map's placeholder Rva00295A0FCommands::Rva00295A0FCommand takes (pointer, int, int): Zero Hour's wrapper; AssaultTransportAIUpdate::giveFinalOrders 0x0048F6EE calls it, f08)
	void aiAttackMoveToPositionAndFaceDirection(Coord3D const *, int, CommandSourceType, float);	// 0x0036E97F
	void aiMoveToPositionAmphibious(Coord3D const *, CommandSourceType);	// 0x0036EA01
	void aiTightenToPosition(Coord3D const *, CommandSourceType);	// 0x0036EA74
	void aiMoveToAndEvacuate(Coord3D const *, CommandSourceType);	// 0x0036EAE0
	void aiMoveToAndEvacuateAndExit(Coord3D const *, CommandSourceType);	// 0x0036EB4C
	void rva0036EBB8(Object *, CommandSourceType);	// 0x0036EBB8
	void rva0036EC1D(Object *, CommandSourceType);	// 0x0036EC1D
	void aiFollowWaypointPath(Waypoint const *, CommandSourceType);	// 0x0036EC82
	void aiFollowWaypointPathExact(Waypoint const *, CommandSourceType);	// 0x0036ECE7
	void aiFollowWaypointPathAsTeam(Waypoint const *, CommandSourceType);	// 0x0036ED4C
	void aiAttackFollowWaypointPath(const Waypoint *way, Int maxShotsToFire, CommandSourceType cmdSource);	// 0x00352F2F (Zero Hour name: command 0x10, the waypoint +0x2C and the shot count +0x34)
	void aiAttackFollowWaypointPathAsTeam(const Waypoint *way, Int maxShotsToFire, CommandSourceType cmdSource);	// 0x00352F9D (command 0x11)
	void aiBfmeCommand33(Waypoint const *, CommandSourceType);	// 0x0036EDB1
	void aiFollowPathAppend(Coord3D const *, CommandSourceType);	// 0x0036EF89
	void rva0036EFF5(Object *, CommandSourceType);	// 0x0036EFF5
	void aiForceAttackObject(Object *, int, CommandSourceType);	// 0x0036F05A
	void aiAttackTeam(Team const *, int, CommandSourceType);	// 0x0036F0C8
	void aiAttackArea(PolygonTrigger const *, CommandSourceType);	// 0x0036F136
	void rva0036F19B(Object *, CommandSourceType);	// 0x0036F19B
	void aiResumeConstruction(Object *, CommandSourceType);	// 0x0036F200
	void aiGetHealed(Object *, CommandSourceType);	// 0x0036F265
	void aiGetRepaired(Object *, CommandSourceType);	// 0x0036F2CA
	void aiHarvest(Coord3D const *, CommandSourceType);	// 0x0036F32F
	void aiExit(Object *, CommandSourceType);	// 0x0036F39B
	void aiGoProne(Rva003427DD const *, CommandSourceType);	// 0x0036F400 (SlavedUpdate::onSlaverDamage passes its DamageInfo)
	void aiGuardPosition(Coord3D const *, GuardMode, CommandSourceType);	// 0x0036F46A
	void aiGuardObject(Object *, int, CommandSourceType);	// 0x0036F4DF
	void rva0036F54D(Team const *, int, CommandSourceType);	// 0x0036F54D
	void aiGuardArea(PolygonTrigger const *, int, CommandSourceType);	// 0x0036F5BB
	void aiGuardAreaFromPosition(PolygonTrigger const *, int, CommandSourceType, Coord3D const *);	// 0x0036F629
	void rva0036F6A7(float, CommandSourceType);	// 0x0036F6A7
	void rva0037379B(Object *, CommandSourceType);	// 0x0037379B
	void aiMoveToPositionEvenIfSleeping(Coord3D const *, CommandSourceType);	// 0x00394191
	void aiAttackMoveToPositionAmphibious(Coord3D const *, CommandSourceType);	// 0x003C75DD
	void rva003C7653(Object *, CommandSourceType);	// 0x003C7653
	void rva003C76B8(int, CommandSourceType);	// 0x003C76B8
	void aiFaceObject(Object *, CommandSourceType);	// 0x003C771D
	void aiFacePosition(Coord3D const *, int);	// 0x003C7782
	void rva003C77EE(Waypoint const *, CommandSourceType);	// 0x003C77EE
	void aiWanderInPlace(CommandSourceType);	// 0x003C7853
	void rva003C78AF(Waypoint const *, CommandSourceType);	// 0x003C78AF
	void aiMoveToPositionSA(Coord3D const *, int);	// 0x0044FF6D
	void rva0044FFD9(Object *, CommandSourceType);	// 0x0044FFD9
	void rva0045003E(int, CommandSourceType);	// 0x0045003E
	void aiMoveToAndDelete(Coord3D const *, CommandSourceType);	// 0x00462224
	void aiMoveToAndDie(Coord3D const *, CommandSourceType);	// 0x00462290
	void aiBfmeObjectCommand3D(Object *, CommandSourceType);	// 0x00470447
	void rva004C6D35(Object *, CommandSourceType);	// 0x004C6D35
	void aiAttackLevelPosition(Coord3D const *, CommandSourceType);	// 0x004C6D9A
	void aiCommand83(CommandSourceType);	// 0x004BFD27 (placeholder row in ActiveBody.cpp): the argument-free command 0x53, sent by ActiveBody::attemptDamage after a clipped hit and by the Highlander/Immortal bodies
};

#endif
