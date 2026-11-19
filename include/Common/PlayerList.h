// PlayerList.h: the players of a game (ThePlayerList, VA 0x00DFEEE8). SIZE 0x68.
// Target facts (code RVAs, tables VAs):
//  - Bases SubsystemInterface +0x00 and Snapshot +0x0C: ctor 0x002A7F80 calls
//    SubsystemInterface(), stores Snapshot's table at +0x0C, then its own
//    tables 0x00BFD618 (+0x00, 17 slots) and 0x00BFD604 (+0x0C, 4 slots).
//  - Size 0x68: GameEngine::init allocates 0x68 (0x0022F62F).
//  - +0x10 local player (setLocalPlayer 0x002A7B34), +0x14 player count
//    (init 0x002A7EF1 sets 1; Rva002A7A6F walks that many), +0x18 twenty
//    Player pointers (the ctor allocates twenty 0x758-byte Players into them;
//    getNthPlayer 0x002A7A29 bounds the index by 20). Slot 0 is the neutral player.
//  - Primary slots 1 init 0x002A7EF1, 9 reset 0x002A7AA0, 10 update 0x002A7AB6,
//    14 newGame 0x002A8008, 15 newMap 0x002A7AE6; slot 16 (0x002A7ACE, calls
//    a Player method on each of the 20) has no recovered name, so it is not
//    declared. Snapshot slots: GetSnapshotName 0x002A7A23 (protected), xfer
//    0x002A7D63 (the map spells it DoXfer).
#pragma once
#ifndef BFME2_PLAYERLIST_H
#define BFME2_PLAYERLIST_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/Snapshot.h"
#include "Common/SubsystemInterface.h"

class Player;
class Object;
class Team;
class MapObject;
class Xfer;

enum { MAX_PLAYER_COUNT = 20 };

class PlayerList : public SubsystemInterface, public Snapshot
{
public:
	PlayerList();				// 0x002A7F80
	virtual ~PlayerList();			// 0x002A79A9

	virtual void init();			// slot 1, 0x002A7EF1
	virtual void reset();			// slot 9, 0x002A7AA0
	virtual void update();			// slot 10, 0x002A7AB6
	virtual void newGame();			// slot 14, 0x002A8008
	virtual void newMap();			// slot 15, 0x002A7AE6
	virtual void rva002A7ACE();		// slot 16, 0x002A7ACE: retail's table 0x007FD618 has 17 slots; loops over the 20 players like update()

protected:
	virtual void loadPostProcess() {}	// Snapshot slot 1, the shared empty body 0x000B3FD0
	virtual const char *GetSnapshotName() const;	// Snapshot slot 2, 0x002A7A23
public:
	virtual void xfer(Xfer *xfer);		// Snapshot slot 3, 0x002A7D63

	Player *getNthPlayer(Int index);	// 0x002A7A29
	Player *findPlayerWithNameKey(NameKeyType key);	// 0x002A7A41
	Player *Rva002A7A6F(Int id);		// 0x002A7A6F, by Player +0x3AC
	void rva002A7AFE(void *arg);		// 0x002A7AFE
	void updateTeamStates();		// 0x002A7B1C
	void setLocalPlayer(Player *player);	// 0x002A7B34
	Player *getPlayerFromMask(Int mask);	// 0x002A7B91
	Player *getEachPlayerFromMask(Int &mask);	// 0x002A7BC9
	Int rva002A7C0B(Bool flag);		// 0x002A7C0B
	Int getPlayersWithRelationship(Int srcPlayerIndex, UnsignedInt allowedRelationships, Bool flag);	// 0x002A7C70
	Int rva002A7D30();			// 0x002A7D30
	Bool rva002A7DD0();			// 0x002A7DD0 the local player is not the one Player::isPlayerActive holds
	Bool rva002A7DDE(Object *object);		// 0x002A7DDE true when the local player has no observer flag, or when it is allied to the object team (the map spells the argument Rva002A7DDEArg)
	void refreshObservedPlayerView(Player *player);		// 0x002A7E42: refreshes the shroud, control bar scheme and drawables for a new observed player (the map spells the class Rva002A7E42)
	Player *bfmePickRV();			// 0x002A7E14 the local player, or the observer look-at player of TheControlBar when the local one is an observer (the map spells the class BfmeThingRV and the result BfmeMemberRV)
	Bool get() const;			// 0x002A7EA0 fewer than 19 players (the map spells the class Rva002A7EA0CmpBoolField; its +0x14 is the player count)
	Player *createClonedAllyFor(void *arg);	// 0x002A7F2A
	void rva002A8306(Bool flag);		// 0x002A8306
	Team *validateTeam(AsciiString owner, MapObject *obj);	// 0x002A851D

	Player *getLocalPlayer() { return m_local; }
	Player *getNeutralPlayer() { return m_players[0]; }
	Int getPlayerCount() { return m_playerCount; }

	Player *m_local;			// +0x10
	Int m_playerCount;			// +0x14
	Player *m_players[MAX_PLAYER_COUNT];	// +0x18
};
// SIZE 0x68
typedef char PlayerListSize[(sizeof(PlayerList) == 0x68) ? 1 : -1];
typedef char PlayerListPlayersOffset[(offsetof(PlayerList, m_players) == 0x18) ? 1 : -1];

extern PlayerList *ThePlayerList;

#endif
