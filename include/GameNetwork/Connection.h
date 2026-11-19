// Connection.h: one peer connection of the connection manager (Zero Hour's Connection; names are theirs by role,
// offsets are retail's).
// Target facts (code RVAs):
//  - SIZE 0x358 by the constructor 0x0058BDA0: +0 = -1, +4 = 0, +8/+0xC = 0, the word +0x10 = 0, +0x14 = 0,
//    the command list pointer +0x18 = 0, +0x1C = 0x7D0, the average latency +0x20 = 0.0f, a 200-float latency
//    history at +0x24 (0x320 bytes), +0x344 = 1, +0x348/+0x34C/+0x350 = 0 and the retry-metrics time +0x354 = 0.
//  - setQuitting 0x0058B9B4 stores its argument at +0 and the current time (timeGetTime, IAT 0x00BBA918) at +4.
//    isQueueEmpty 0x0058B9A9 tests the command list's head. clearCommandsExceptFrom 0x0058B960 removes and
//    frees every queued command whose message's player id (+0xC) is not the argument. processAck 0x0058B9CA finds
//    the queued command that matches (id, player, +4, +8), folds its round-trip time into a running average over
//    the latency history (index id % 200, weight 1/200 = the constant at VA 0x00C1B4F0), removes it and returns it.
//    processAck(msg) 0x0058BD7B hands an ack message (type 0 or 1) to the folded 0x0058BD4F, which unpacks its
//    id, original player and the two words +0x20/+0x24. doRetryMetrics 0x0058BA7B zeroes the retry count once
//    ten seconds have passed and counts the period in a global.
#pragma once
#ifndef BFME2_CONNECTION_H
#define BFME2_CONNECTION_H

#include "Common/GameType.h"
#include "Common/UnicodeString.h"
#include "GameNetwork/NetPacketAddress.h"
#include "GameNetwork/NetCommandList.h"

class NetCommandList;

// The address block 0x0058BAA7 reads (an ip dword and a port word, copied as two dwords; the map spells the struct InitAddrs).
struct InitAddrs : public NetPacketAddress
{
};
class NetCommandRef;
class NetCommandMsg;
class NetAckStage1CommandMsg;
class NetAckBothCommandMsg;

class Connection
{
public:
	Connection();												// 0x0058BDA0
	~Connection() { ::delete m_netCommandList; }				// 0x004D060B (emitted in ConnectionManager.cpp: frees the command list, then the name)

	void rva0058BAA7(InitAddrs *addrs, const UnicodeString &name, Int slot);	// 0x0058BAA7 (sets up the connection for a peer)
	void sendNetCommandMsg(NetCommandMsg *msg, UnsignedByte relay);		// 0x0058BDFA
	void clearCommandsExceptFrom(Int playerID);					// 0x0058B960
	UnsignedInt doSend(Bool isInGame);							// 0x0058BB5C (builds packets from the pending commands and queues them on the transport)
	Bool isQueueEmpty();										// 0x0058B9A9
	void setQuitting(UnsignedInt quitValue);					// 0x0058B9B4
	NetCommandRef *processAck(UnsignedShort commandID, UnsignedByte originalPlayerID, UnsignedInt timestamp, UnsignedInt executionFrame);	// 0x0058B9CA
	NetCommandRef *processAck(NetCommandMsg *msg);				// 0x0058BD7B
	NetCommandRef *rva0058BD4F(void *ackMsg);					// 0x0058BD4F (the folded body shared by the per-type overloads below)
	NetCommandRef *processAck(NetAckStage1CommandMsg *msg);		// folded into 0x0058BD4F
	NetCommandRef *processAck(NetAckBothCommandMsg *msg);		// folded into 0x0058BD4F

protected:
	void doRetryMetrics();										// 0x0058BA7B

public:
	UnsignedInt m_quitValue;									// +0x00 (-1 after the constructor)
	UnsignedInt m_quitTime;										// +0x04
	Int m_unk0x08;											// +0x08
	NetPacketAddress m_address;								// +0x0C the peer's address (ip dword, port word; the constructor value-initialises it)
	UnicodeString m_name;										// +0x14 (the player name; ConnectionManager::getPlayerName copies it, 0x004D060B releases it)
	NetCommandList *m_netCommandList;							// +0x18
	Int m_unk0x1C;												// +0x1C (0x7D0 after the constructor)
	Real m_averageLatency;										// +0x20
	Real m_latencies[200];										// +0x24
	Int m_frameGrouping;										// +0x344 (1 after the constructor; ConnectionManager::setFrameGrouping 0x004CF7F5 writes it)
	Int m_unk0x348;												// +0x348
	Int m_unk0x34C;												// +0x34C
	Int m_retries;												// +0x350
	UnsignedInt m_retryMetricsTime;								// +0x354
};
// SIZE 0x358
typedef char ConnectionSize[(sizeof(Connection) == 0x358) ? 1 : -1];

#endif
