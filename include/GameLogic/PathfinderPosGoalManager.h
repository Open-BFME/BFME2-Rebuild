// PathfinderPosGoalManager.h: the per-object pathfinding goal record Object keeps at +0xA4.
// Target facts: Object's forwarders 0x0028ACCA..0x0028ADB0 test Object +0xA4 for NULL and
// tail-jump to these members (Pathfinder/PathfinderPosGoalManager.cpp, object O004DD619);
// Object::GetGoalLayer 0x0028AD6C reads +0x28 directly (1, the ground layer, without one).
// Method names are the map's; only the members a body proves are typed. SIZE unknown
// (at least 0x2C).
#pragma once
#ifndef BFME2_PATHFINDERPOSGOALMANAGER_H
#define BFME2_PATHFINDERPOSGOALMANAGER_H

#include "Common/GameType.h"

class Coord3D;

class PathfinderPosGoalManager
{
public:
	Bool rva004DD637();									// 0x004DD637 (Object::IsAtGoalPosition)
	Bool rva004DD80A(Coord3D *pos);						// 0x004DD80A (Object::GetGoalPosition)
	Real rva004DD843();									// 0x004DD843 (Object::GetGoalAngle)
	Bool rva004DD84D(Coord3D *pos);						// 0x004DD84D (Object::GetPathfinderPos)
	void rva004DDD6E();									// 0x004DDD6E
	void rva004DDD80();									// 0x004DDD80
	void SetGoal(Int a, Real angle, Int b);				// 0x004DE109
	void rva004DE24B(Int a);							// 0x004DE24B
	Bool rva004DE2ED();									// 0x004DE2ED
	void rva004DE4E2(Int a);							// 0x004DE4E2
	void rva004DE511(Int a, Int b);						// 0x004DE511

	unsigned char m_unk0x00[0x28];
	Int m_goalLayer;									// +0x28 (Object::GetGoalLayer 0x0028AD6C)
};

#endif
