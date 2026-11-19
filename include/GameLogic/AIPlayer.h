// AIPlayer.h: the computer player's build and team logic (one per AI-controlled Player,
// Player::m_ai +0x2DC). Zero Hour's AIPlayer with BFME 2's changes.
// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
//
// Evidence (target facts first):
// - Size 0x78: Player::setPlayerType pushes 0x78 before new at 0x002A9A74; AISkirmishPlayer's
//   constructor 0x004EF3A3 starts its own members at +0x78.
// - Base Snapshot and the table at VA 0x00C62DC8 (27 slots): the constructor 0x004F04FC stores it,
//   the destructor 0x004F07E6 stores it, calls clearTeamsInQueue 0x004F05D6, then stores Snapshot's.
//   Slots 0..3 are Snapshot's: deleting dtor 0x004F111C, loadPostProcess (the shared empty body
//   0x000B3FD0), GetSnapshotName 0x004F05BA ("AIPlayer"), xfer 0x004F0DDE. Slots 4..26 below, each
//   with its retail target. BFME 2 dropped Zero Hour's base-building virtuals (buildAIBaseDefense,
//   buildAIBaseDefenseStructure, buildSpecificAIBuilding, doBaseBuilding, processBaseBuilding) and
//   crc; recruitSpecificAITeam gained a position, computeSuperweaponTarget returns void.
// - Slot 18 is Zero Hour's doTeamBuilding: 0x004F48D8 tests Player +0x338 (getCanBuildUnits), runs
//   the team timer (+0x14) and team delay (+0x24), calls queueUnits and slot 26, resets the delay to
//   5 * BfmeLogicFramesPerSecond. Slot 26 is processTeamBuilding: 0x004EF4A1 (22 bytes) calls slot
//   22 (selectTeamToBuild) and then queueUnits, as Zero Hour's; AISkirmishPlayer's slot 26 is the
//   same address (two identical bodies folded).
// - Slot 15 (0x00210C87) stores its argument at +0x30, m_skillsetSelector: Zero Hour's
//   selectSkillset (the map's names at that address, NetWrapperCommandMsg::setNumChunks and
//   AIPlayer::setAIDifficulty, are folds). Slots 11..13 are folded constant bodies (return false /
//   NULL / false, ret 8 for checkBridges), Zero Hour's inline defaults.
// - Members: constructor 0x004F04FC writes every offset from +0x04 to +0x74 (old decomp's matching
//   constructor, AIPlayerCtor.cpp, gives the order and values); isSupplySourceAttacked 0x004F1138
//   proves +0x6C/+0x70, updateBridgeRepair 0x004F418A proves +0x68, repairStructure 0x004F2F66 proves
//   +0x48/+0x60, Player::getAiBaseCenter 0x002A9C26 proves +0x34/+0x40. Names are Zero Hour's, in
//   Zero Hour's order, which retail's offsets follow exactly; +0x44 (m_baseRadius, constructor 10.0f)
//   and +0x74 (m_curWarehouseID, constructor 0) are at Zero Hour's positions. All members evidenced;
//   none offset-named.
// - The two queue heads are Zero Hour's DLINKHEAD structs (MAKE_DLINK_HEAD): a head with its own
//   constructor is a constructed subobject, which is what puts the head stores between the vptr
//   store and the +0x0C store in 0x004F04FC. Retail's head has no destructor (the old decomp's
//   matching constructor and destructor have no unwind state for it); Zero Hour's only asserts.
#pragma once
#ifndef BFME2_AIPLAYER_H
#define BFME2_AIPLAYER_H

#include "Common/GameType.h"
#include "Common/Snapshot.h"
#include "Common/Coord3D.h"
#include "Common/AsciiString.h"
#include "Common/DLinkIterator.h"
#include "GameLogic/TeamInQueue.h"

class BuildListInfo;
class Object;
class Player;
struct Region2D;
class SpecialPowerTemplate;
class Team;
class TeamInQueue;
class TeamPrototype;
class ThingTemplate;
class Waypoint;
class WorkOrder;
class Xfer;
enum GameDifficulty;

enum { INVALID_SKILLSET_SELECTION = -1 };	// the constructor stores -1 at +0x30

class AIPlayer : public Snapshot
{
	friend class Player;
public:
	AIPlayer(Player *p);							// 0x004F04FC
	virtual ~AIPlayer();							// slot 0, 0x004F07E6 (deleting 0x004F111C)

	virtual void computeSuperweaponTarget(const SpecialPowerTemplate *power, Coord3D *pos, Int playerNdx, Real weaponRadius);	// slot 4, 0x004F1D38
	virtual void update();							// slot 5, 0x004F4927
	virtual void newMap() {}						// slot 6, the shared empty body 0x000B3FD0
	virtual void onUnitProduced(Object *factory, Object *unit);			// slot 7, 0x004F4962
	virtual void onStructureProduced(Object *factory, Object *structure);	// slot 8, 0x004F33EC
	virtual void buildSpecificAITeam(TeamPrototype *teamProto, Bool priorityBuild);	// slot 9, 0x004F2464
	virtual void recruitSpecificAITeam(TeamPrototype *teamProto, Real recruitRadius, const Coord3D *pos);	// slot 10, 0x004F437E
	virtual Bool isSkirmishAI() { return false; }	// slot 11, folded 0x0047A699
	virtual Player *getAiEnemy() { return NULL; }	// slot 12, folded 0x000D43D0
	virtual Bool checkBridges(Object *unit, Waypoint *way) { return false; }	// slot 13, folded 0x0050B5F1
	virtual void repairStructure(ObjectID structure);	// slot 14, 0x004F2F66
	virtual void selectSkillset(Int skillset);		// slot 15, folded 0x00210C87 (stores +0x30)

	Bool getBaseCenter(Coord3D *pos) const { *pos = m_baseCenter; return m_baseCenterSet; }

	void buildUpgrade(const AsciiString &upgrade);	// 0x004F2C14
	void aiPreTeamDestroy(const Team *team);		// 0x004F0819
	GameDifficulty getAIDifficulty() const;		// out of line: Player::getPlayerDifficulty 0x002A9BF2 calls the shared 4-byte getter 0x005C4ACD (reads +0x2C)
	Bool isSupplySourceAttacked();					// 0x004F1138 (Player 0x002A9CB8 forwards to it)
	Bool isSupplySourceSafe(Int minSupplies);		// 0x004F2BEE
	Bool isLocationSafe(const Coord3D *pos, const ThingTemplate *thing);	// 0x004F08A8
	void guardSupplyCenter(Team *team, Int minSupplies);	// 0x004F2ABC
	static void getPlayerStructureBounds(Region2D *bounds, Int playerNdx);	// 0x004F1B8C

	// Zero Hour's MAKE_DLINK_HEAD(TeamInQueue, TeamBuildQueue) and (TeamInQueue, TeamReadyQueue).
	// BFME 2 keeps these out of line (rows 0x004F03DF..0x004F04C0; the ready-queue pair at
	// 0x004EF373/0x004EF383); flip is the map's name for reverse_TeamReadyQueue.
	DLINK_ITERATOR<TeamInQueue> iterate_TeamBuildQueue() const
	{
		return DLINK_ITERATOR<TeamInQueue>(m_dlinkhead_TeamBuildQueue.m_head, &TeamInQueue::dlink_next_TeamBuildQueue);
	}
	DLINK_ITERATOR<TeamInQueue> iterate_TeamReadyQueue() const
	{
		return DLINK_ITERATOR<TeamInQueue>(m_dlinkhead_TeamReadyQueue.m_head, &TeamInQueue::dlink_next_TeamReadyQueue);
	}
	Bool isInList_TeamBuildQueue(TeamInQueue *o) const;		// 0x004F03DF
	Bool isInList_TeamReadyQueue(TeamInQueue *o) const;		// 0x004EF373
	void prependTo_TeamBuildQueue(TeamInQueue *o);			// 0x004F03EF
	void prependTo_TeamReadyQueue(TeamInQueue *o);			// 0x004EF383
	void removeFrom_TeamBuildQueue(TeamInQueue *o);			// 0x004F040F
	void removeFrom_TeamReadyQueue(TeamInQueue *o);			// 0x004F0479
	void removeAll_TeamBuildQueue(void (*p)(TeamInQueue *o));	// 0x004F042F
	void removeAll_TeamReadyQueue(void (*p)(TeamInQueue *o));	// 0x004F0499
	void reverse_TeamBuildQueue();							// 0x004F0456
	void flip();											// 0x004F04C0
	void clearTeamsInQueue();								// 0x004F05D6

protected:
	virtual void loadPostProcess() {}				// slot 1, the shared empty body 0x000B3FD0
	virtual const char *GetSnapshotName() const;	// slot 2, 0x004F05BA
	virtual void xfer(Xfer *xfer);					// slot 3, 0x004F0DDE

	virtual void checkReadyTeams();					// slot 16, 0x004F2FC4
	virtual void checkQueuedTeams();				// slot 17, 0x004F1653
	virtual void doTeamBuilding();					// slot 18, 0x004F48D8
	virtual void doUpgradesAndSkills();				// slot 19, 0x004F31B6
	virtual Object *findDozer(const Coord3D *pos);	// slot 20, 0x004F0C85
	virtual void queueDozer();						// slot 21, 0x004F288C
	virtual Bool selectTeamToBuild();				// slot 22, 0x004F35A5
	virtual Bool selectTeamToReinforce(Int minPriority);	// slot 23, 0x004F3DB1
	virtual Bool startTraining(WorkOrder *order, Bool busyOK, AsciiString teamName);	// slot 24, 0x004F2063
	virtual Bool isAGoodIdeaToBuildTeam(TeamPrototype *proto);	// slot 25, 0x004F1566
	virtual void processTeamBuilding();				// slot 26, 0x004EF4A1

	static Int getPlayerSuperweaponValue(Coord3D *center, Int playerNdx, Real radius);	// 0x004F1256
	Object *findFactory(const ThingTemplate *thing, Bool busyOK, Int *buildIndex);	// 0x004F09CF
	Bool isPossibleToBuildTeam(TeamPrototype *proto, Bool requireIdleFactory, Bool &needMoney);	// 0x004F0B42
	Bool rva004F13D8(TeamPrototype *proto);			// 0x004F13D8, BFME 2's (the map's placeholder; isAGoodIdeaToBuildTeam accepts the team when it is true)
	Bool dozerInQueue();							// 0x004F19D3
	void checkForSupplyCenter(BuildListInfo *info, Object *bldg);	// 0x004F29E9
	void computeCenterAndRadiusOfBase(Coord3D *center, Real *radius);	// 0x004F1801
	void queueSupplyTruck();						// 0x004F37B0
	void queueUnits();								// 0x004F46A9
	void updateBridgeRepair();						// 0x004F418A
	Object *findSupplyCenter(Int minSupplies);		// 0x004F21D5

	struct DLINKHEAD_TeamBuildQueue
	{
		TeamInQueue *m_head;
		DLINKHEAD_TeamBuildQueue() : m_head(0) {}
	};
	struct DLINKHEAD_TeamReadyQueue
	{
		TeamInQueue *m_head;
		DLINKHEAD_TeamReadyQueue() : m_head(0) {}
	};
	DLINKHEAD_TeamBuildQueue m_dlinkhead_TeamBuildQueue;	// +0x04
	DLINKHEAD_TeamReadyQueue m_dlinkhead_TeamReadyQueue;	// +0x08

	enum { MAX_STRUCTURES_TO_REPAIR = 2 };

	Player *m_player;							// +0x0C
	Bool m_readyToBuildTeam;					// +0x10
	Bool m_readyToBuildStructure;				// +0x11
	Int m_teamTimer;							// +0x14 (constructor 2)
	Int m_structureTimer;						// +0x18 (constructor 2)
	Int m_teamSeconds;							// +0x1C (constructor 10, then TheAI's TeamSeconds)
	Int m_buildDelay;							// +0x20
	Int m_teamDelay;							// +0x24
	Int m_frameLastBuildingBuilt;				// +0x28
	GameDifficulty m_difficulty;				// +0x2C (TheScriptEngine's global difficulty)
	Int m_skillsetSelector;						// +0x30 (constructor -1)
	Coord3D m_baseCenter;						// +0x34
	Bool m_baseCenterSet;						// +0x40
	Real m_baseRadius;							// +0x44 (constructor 10.0f)
	ObjectID m_structuresToRepair[MAX_STRUCTURES_TO_REPAIR];	// +0x48
	ObjectID m_repairDozer;						// +0x50
	Coord3D m_repairDozerOrigin;				// +0x54
	Int m_structuresInQueue;					// +0x60
	Bool m_dozerQueuedForRepair;				// +0x64
	Bool m_dozerIsRepairing;					// +0x65
	Int m_bridgeTimer;							// +0x68
	UnsignedInt m_supplySourceAttackCheckFrame;	// +0x6C
	ObjectID m_attackedSupplyCenter;			// +0x70
	ObjectID m_curWarehouseID;					// +0x74
};
// SIZE 0x78
typedef char AIPlayerSize[(sizeof(AIPlayer) == 0x78) ? 1 : -1];

#endif
