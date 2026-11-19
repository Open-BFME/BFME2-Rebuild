// AISkirmishPlayer.h: the skirmish computer player, an AIPlayer that picks its own enemy.
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
//
// Evidence: the constructor 0x004EF3A3 calls AIPlayer's 0x004F04FC, stores the table at VA
// 0x00C62AF8, clears +0x78..+0x9C and turns on the player's unit building (Player +0x338), as
// Zero Hour's. Slot targets (27, AIPlayer's order): 0 deleting dtor 0x004EF635, 2 GetSnapshotName
// 0x004EF417, 3 xfer 0x004EF52F, 4 computeSuperweaponTarget 0x004EFDDC, 5 update 0x004EF520,
// 7 onUnitProduced 0x004EF455, 9 buildSpecificAITeam 0x004EF49C, 10 recruitSpecificAITeam
// 0x004EFFC9, 11 isSkirmishAI (folded `return true` 0x0050B5C6), 12 getAiEnemy 0x004EFDAB,
// 13 checkBridges 0x004EF6A0, 16 checkReadyTeams 0x004EF4B7, 17 checkQueuedTeams 0x004EF4BC,
// 18 doTeamBuilding 0x004EF4C1, 20 findDozer 0x004EF52A, 21 queueDozer 0x004EF525,
// 22 selectTeamToBuild 0x004EF45F, 23 selectTeamToReinforce 0x004EF45A, 24 startTraining
// 0x004EF651, 25 isAGoodIdeaToBuildTeam 0x004EF73A, 26 processTeamBuilding 0x004EF4A1. Slots 1, 6,
// 8, 14, 15 and 19 keep AIPlayer's targets (loadPostProcess, newMap, onStructureProduced,
// repairStructure, selectSkillset, doUpgradesAndSkills). xfer 0x004EF52F writes the eight
// defense members in Zero Hour's order after AIPlayer::xfer. Members are Zero Hour's, at the
// offsets the constructor clears; all evidenced, none offset-named.
#pragma once
#ifndef BFME2_AISKIRMISHPLAYER_H
#define BFME2_AISKIRMISHPLAYER_H

#include "GameLogic/AIPlayer.h"

class AISkirmishPlayer : public AIPlayer
{
public:
	AISkirmishPlayer(Player *p);					// 0x004EF3A3
	virtual ~AISkirmishPlayer();					// 0x004EF41D

	virtual void computeSuperweaponTarget(const SpecialPowerTemplate *power, Coord3D *pos, Int playerNdx, Real weaponRadius);
	virtual void update();
	virtual void onUnitProduced(Object *factory, Object *unit);
	virtual void buildSpecificAITeam(TeamPrototype *teamProto, Bool priorityBuild);
	virtual void recruitSpecificAITeam(TeamPrototype *teamProto, Real recruitRadius, const Coord3D *pos);
	virtual Bool isSkirmishAI() { return true; }
	virtual Player *getAiEnemy();
	virtual Bool checkBridges(Object *unit, Waypoint *way);

protected:
	virtual const char *GetSnapshotName() const;
	virtual void xfer(Xfer *xfer);

	virtual void checkReadyTeams();
	virtual void checkQueuedTeams();
	virtual void doTeamBuilding();
	virtual Object *findDozer(const Coord3D *pos);
	virtual void queueDozer();
	virtual Bool selectTeamToBuild();
	virtual Bool selectTeamToReinforce(Int minPriority);
	virtual Bool startTraining(WorkOrder *order, Bool busyOK, AsciiString teamName);
	virtual Bool isAGoodIdeaToBuildTeam(TeamPrototype *proto);
	virtual void processTeamBuilding();

	Int getMyEnemyPlayerIndex();					// 0x004EF464
	void acquireEnemy();							// 0x004EFB3D
	void adjustBuildList(BuildListInfo *list);		// 0x004EF87D

	Int m_curFrontBaseDefense;						// +0x78
	Int m_curFlankBaseDefense;						// +0x7C
	Real m_curFrontLeftDefenseAngle;				// +0x80
	Real m_curFrontRightDefenseAngle;				// +0x84
	Real m_curLeftFlankLeftDefenseAngle;			// +0x88
	Real m_curLeftFlankRightDefenseAngle;			// +0x8C
	Real m_curRightFlankLeftDefenseAngle;			// +0x90
	Real m_curRightFlankRightDefenseAngle;			// +0x94
	UnsignedInt m_frameToCheckEnemy;				// +0x98
	Player *m_currentEnemy;							// +0x9C
};
// SIZE 0xA0 (the last store of the constructor is +0x9C)

#endif
