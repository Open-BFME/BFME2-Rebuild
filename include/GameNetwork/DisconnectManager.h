// DisconnectManager.h: the vote-to-drop machinery of the connection manager (Zero Hour's DisconnectManager by role; names
// are theirs where a retail body agrees, offsets are retail's).
// Target facts (code RVAs, VA 0x00C601DC table):
//  - SIZE 0x28C (ConnectionManager::init 0x004D24A2 allocates 0x28C and runs the constructor 0x004D376A, then init
//    0x004D3822); ConnectionManager +0x12100 holds it and its destructor deletes it through slot 0 (the table has one
//    slot, the destructor).
//  - +4 and +0xC (1 after init) and +8/+0x10 (-1 after init) are state words; +0x14 holds seven per-player time stamps
//    (resetPlayerTimeout 0x004D3A06 stores timeGetTime, hasPlayerTimedOut 0x004D3AB2 compares the age against
//    TheWritableGlobalData's network player timeout); +0x30 is an 8 x 8 vote table of 8-byte entries (a byte flag and a
//    dword, countVotesForPlayer 0x004D3BD7 counts the flagged entries of a player's row); +0x230 eight frame numbers
//    (getMaxDisconnectFrame 0x004D3B63 is their maximum; playerHasAdvancedAFrame 0x004D3BB5 raises one and clears its
//    flag byte at +0x250), then six dwords +0x258..+0x26C, a byte +0x270, eight words +0x272 and eight bytes +0x282.
#pragma once
#ifndef BFME2_DISCONNECTMANAGER_H
#define BFME2_DISCONNECTMANAGER_H

#include "Common/GameType.h"

class ConnectionManager;
class NetCommandMsg;

// 0x00512DE1: opens the disconnect screen (the map spells it bfmeShowDisconnectScreen).
void bfmeShowDisconnectScreen();

struct DisconnectVote
{
	UnsignedByte m_voted;									// +0
	unsigned char m_pad01[3];								// +1
	UnsignedInt m_frame;									// +4
};
// SIZE 8

class DisconnectManager
{
public:
	DisconnectManager();									// 0x004D376A (the map holds it under the placeholder Rva004D376A)
	virtual ~DisconnectManager();							// slot 0, 0x004D381B
	Int rva0028A65E() const;								// 0x0028A65E (returns +0x260; shared dword getter, the map holds it on a placeholder class)
	Int rva004E05E9() const;								// 0x004E05E9 (returns +0x264)
	Int rva0057429A() const;								// 0x0057429A (returns +0x26C)

	void init();											// 0x004D3822
	void playerHasAdvancedAFrame(Int slot, UnsignedInt frame);	// 0x004D3BB5
	void voteForPlayerDisconnect(Int slot, ConnectionManager *connections);		// 0x004D3DF4
	void processDisconnectKeepAlive(void *msg, ConnectionManager *connections);	// 0x004D3CA8
	void processDisconnectPlayer(NetCommandMsg *msg, ConnectionManager *connections);	// 0x004D4637
	void processDisconnectCommand(void *ref, ConnectionManager *connections);	// 0x004D46AF
	Bool hasPlayerConnectionTimedOut(Int slot, void *connections);	// 0x004D3B7E (true unless the slot is a live connection; takes the ConnectionManager as a void pointer, as the map's name does)
	Int rva004D3E5D(ConnectionManager *connections);		// 0x004D3E5D (counts the slots that are connected, not timed out and not in the game; the map has no better name)
	Int rva004D3E93(Int excludedSlot, ConnectionManager *connections);	// 0x004D3E93 (the same count without one slot)

protected:
	void resetPlayerTimeout(Int slot);						// 0x004D3A06
	void resetPlayerTimeouts(ConnectionManager *connections);	// 0x004D3D2B
	void resetPlayersVotes(Int slot, UnsignedInt frame, ConnectionManager *connections);	// 0x004D3C32
	void applyDisconnectVote(Int slot, UnsignedInt frame, Int voterSlot, ConnectionManager *connections);	// 0x004D3CD8
	Int countVotesForPlayer(Int slot, ConnectionManager *connections);	// 0x004D3BD7
	Bool isPlayerInGame(Int slot, ConnectionManager *connections);	// 0x004D3F1B
	void sendKeepAlive(ConnectionManager *connections);		// 0x004D3942 (at most every 500 ms; Zero Hour's name, the map holds it on a placeholder class)
	void sendVoteCommand(Int slot, ConnectionManager *connections);	// 0x004D3A1B
	void sendPlayerDestruct(Int slot, ConnectionManager *connections);	// 0x004D3AE2
	void processDisconnectVote(NetCommandMsg *msg, ConnectionManager *connections);	// 0x004D3F74
	void processDisconnectFrame(NetCommandMsg *msg, ConnectionManager *connections);	// 0x004D3FCB
	void sendDisconnectCommand(Int slot, ConnectionManager *connections);	// 0x004D3D5C
	void populateDisconnectScreen(ConnectionManager *connections);	// 0x004D40DB
	void turnOnScreen(ConnectionManager *connections);		// 0x004D4176
	void disconnectPlayer(Int slot, ConnectionManager *connections);	// 0x004D41B3
	Bool rva004D38C5(Int slot) const;						// 0x004D38C5 (true when the slot's word at +0x272 has reached 5; the map holds it under the placeholder class Rva0066B100)
	void clearVotesAgainst(Int slot);						// 0x004D3906 (once: shuts the disconnect screen down, switches the manager off (+0x0C = 1) and clears every vote cast against the slot; the map holds it under the placeholder class Rva004D3906)
	Bool rva004D393A() const;								// 0x004D393A (+0x0C == 1; the map holds it under the placeholder class Rva004D393ACmpBoolField)
	Bool rva004D38D8(Real threshold);						// 0x004D38D8 (the map holds it under the placeholder class Rva004D38D8: true unless the progress ratio +0x264 / +0x260 is below the threshold)
	Bool hasPlayerTimedOut(Int slot);						// 0x004D3AB2
	Bool allOnSameFrame(ConnectionManager *connections);	// 0x004D427A
	Bool isLocalPlayerNextPacketRouter(ConnectionManager *connections);	// 0x004D4330
	Bool isPlayerVotedOut(Int slot, ConnectionManager *connections);	// 0x004D3ECF
	UnsignedInt getMaxDisconnectFrame();					// 0x004D3B63

public:
	Int m_unk0x04;											// +0x04
	Int m_unk0x08;											// +0x08 (-1 after init)
	Int m_unk0x0C;											// +0x0C (1 after init)
	Int m_unk0x10;											// +0x10 (-1 after init)
	UnsignedInt m_playerTimeouts[7];						// +0x14
	DisconnectVote m_votes[8][8];							// +0x30
	UnsignedInt m_playerFrame[8];							// +0x230
	UnsignedByte m_playerFrameAdvanced[8];					// +0x250
	UnsignedInt m_unk0x258;									// +0x258
	UnsignedInt m_unk0x25C;									// +0x25C
	Int m_unk0x260;									// +0x260
	Int m_unk0x264;									// +0x264
	UnsignedInt m_unk0x268;									// +0x268
	UnsignedInt m_unk0x26C;									// +0x26C
	UnsignedByte m_unk0x270;								// +0x270
	unsigned char m_pad271;									// +0x271
	UnsignedShort m_unk0x272[8];							// +0x272
	UnsignedByte m_unk0x282[8];								// +0x282
	unsigned char m_pad28A[2];								// +0x28A
};
// SIZE 0x28C
typedef char DisconnectManagerSize[(sizeof(DisconnectManager) == 0x28C) ? 1 : -1];

#endif
