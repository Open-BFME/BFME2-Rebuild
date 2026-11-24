#pragma once

#include "Common/Coord2D.h"
#include "Common/Snapshot.h"
#include "Common/Vector.h"
#include "GameLogic/ArmySummary.h"

class LivingWorldArmy;
class LivingWorldBattleListener;
class LivingWorldPlayer;
class LivingWorldRegion;

// LivingWorldArmy sends these five notifications from its listener list at
// +0x08. The base table 0x0083702C has no deleting slot. Native dispatch
// targets are ret 4 except slot 1, which takes both armies and returns ret 8.
class LivingWorldArmyListener
{
public:
	virtual void RemoveArmy(LivingWorldArmy *army) {}	// slot 0, e.g. the army destructor at 0x00319D34
	virtual void onArmyEvent1(LivingWorldArmy *army, LivingWorldArmy *other) {}	// 0x003198FA
	virtual void onArmyEvent2(LivingWorldArmy *army) {}	// 0x00319B2A
	virtual void onArmyEvent3(LivingWorldArmy *army) {}	// 0x00319B51
	virtual void onArmyPathChanged(LivingWorldArmy *army) {}	// 0x003198AE, after 0x00538DC1 updates the path
};

// 0x00210158 constructs this in a 0x40-byte allocation. The constructor
// 0x003F6A3A installs Snapshot table 0x0083709C at +0 and the army
// listener table 0x00837088 at +4; 0x003F6E95 registers that subobject.
class LivingWorldBattle : public Snapshot, public LivingWorldArmyListener
{
public:
	LivingWorldBattle();	// 0x003F6A3A
	virtual ~LivingWorldBattle();
	virtual void RemoveArmy(LivingWorldArmy *army);	// listener slot 0, -4 thunk at 0x003F59FE

	int GetTeamNumberForSide(int side);
	int GetRetreatedPlayerCount(int side);
	int GetRetreatedPlayerID(int side, int index);	// (-1 when index is out of range)
	int rva003F4831();	// (the map's name; SelectPendingBattle takes a battle whose value is positive)
	int getSideForPlayer(const LivingWorldPlayer *player);	// 0x003F4752: the side whose team number is the player's team, -1 when none
	int findPlayerOnSide(int side, int playerID);	// 0x003F4798: the index of the side's record for the player id, -1 when none
	int getNumHumanPlayersOnSide(int side);	// 0x003F47E6: records whose player is of kind 0
	int findPlayerOnSide(int side, const LivingWorldPlayer *player);	// 0x003F4FAA: by the player's id
	bool rva003F4538();	// (true once two different sides have player records)
	int rva003F459A();	// (the total number of player records over all sides)
	int rva003F45DF();	// (the index of the first side with player records, 0 when none)
	int *rva003F46F2(int side, int player);	// (the player's combat-stat record)
	int rva003F48FE(int side, int player, int index);	// (army pointer from the native indexed list, held by the current int map key)

	// The serialized type name is at 0x00837040. The constructor clears it.
	enum ResolutionType { RESOLUTION_NONE = 0 };

	// 0x003F5CA9 transfers these records at a 0x30 stride.
	struct BattlePlayer
	{
		void SwapArmies(int first, int second);

		LivingWorldPlayer *m_player;	// +0x00, findPlayerOnSide 0x003F4798
		_STL::vector<LivingWorldArmy *> m_armies;	// +0x04, resolved by army ID in 0x002B8D06
		_STL::vector<ArmySummary> m_armySummaries;	// +0x10, transferred by 0x003F5CA9 at 0x68 stride
		int m_combatStats[5];	// +0x1C, filled from Object and ScoreKeeper in 0x003F470E
	};

	struct BattleSide
	{
		int m_teamNumber;	// +0x00, GetTeamNumberForSide 0x003F458A
		_STL::vector<BattlePlayer> m_players;	// +0x04, record count in 0x003F459A
		_STL::vector<int> m_retreatedPlayerIDs;	// +0x10, GetRetreatedPlayerID 0x003F46C1
	};

protected:
	virtual void loadPostProcess();	// Snapshot slot 1, 0x003F4AD8
	virtual const char *GetSnapshotName() const;	// Snapshot slot 2, 0x003F6A8B
	virtual void xfer(Xfer *xfer);	// Snapshot slot 3, 0x003F7023 (map: DoXfer(Xfer &))

private:
	_STL::vector<LivingWorldBattleListener *> m_listeners;	// +0x08, iterated through 0x003F5206
	int m_listenerIndex;	// +0x14, 0x003F4BE9 saves and updates the active index
	_STL::vector<BattleSide> m_sides;	// +0x18, 0x003F7023 xfers a 0x1C stride

public:
	LivingWorldRegion *m_region;	// +0x24, findPendingBattleByRegion 0x0020E57F

private:
	Coord2D m_beaconPosition;	// +0x28, passed to CreateBeaconObject by 0x003F43D3

public:
	int m_battleID;	// +0x30, findPendingBattleByID 0x0020E501
	int m_battleKey;	// +0x34, findCompletedBattleByKey 0x0020E5FD

private:
	int m_unk0x38;	// int xfer at 0x003F706D, initialized to -1
	ResolutionType m_resolutionType;	// +0x3C, enum transferred by 0x003F3FB6
};

typedef char LivingWorldBattlePlayer_size[(sizeof(LivingWorldBattle::BattlePlayer) == 0x30) ? 1 : -1];
typedef char LivingWorldBattleSide_size[(sizeof(LivingWorldBattle::BattleSide) == 0x1C) ? 1 : -1];
typedef char LivingWorldBattle_size[(sizeof(LivingWorldBattle) == 0x40) ? 1 : -1];
