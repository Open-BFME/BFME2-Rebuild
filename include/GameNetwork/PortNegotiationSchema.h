// PortNegotiationSchema.h: the NAT negotiator's per-pair bookkeeping for eight slots (BFME II only; the map names its
// methods PortNegotiationSchema::..., WorldBuilder's debug build keeps the class name).
// Target facts (code RVAs):
//  - SIZE 0x8BC. Constructor 0x005DC43B: constructs the listener list at +4 (0x00330757), runs the element vector
//    constructor iterator over the 0x40 ping records at +0x218 (element constructor 0x005DB84F, destructor 0x005A66B8,
//    0x14 bytes each), stores the table 0x00C71BF8 and calls the reset 0x005DB9E4, which zeroes every array below, sets
//    +0x14 to 8 and clears the slot list pointer +0x8B8.
//  - Destructor 0x005A71B1 (a header-inline virtual, emitted in NAT.cpp's unit; scalar deleting 0x005A7200): stores the
//    table, runs the vector destructor iterator over the ping records, then the listener list's destructor frees its buffer.
//  - Accessors prove the arrays: GetConnectionState 0x005DB9BC reads +0x18 [a * 8 + b]; getActionID 0x005DBCA6 reads +0x118;
//    peekPing 0x005DB98E returns the record at +0x218 + (a * 8 + b) * 0x14; getTimeToSendPing 0x005DBBA5 reads +0x718 [a];
//    getRetryTimeout 0x005DBB7A reads +0x738 and getRetryCount 0x005DBB4D the words at +0x838.
#pragma once
#ifndef BFME2_PORTNEGOTIATIONSCHEMA_H
#define BFME2_PORTNEGOTIATIONSCHEMA_H

#include <stdlib.h>
#include "Common/GameType.h"
#include "GameNetwork/PortNegotiationListener.h"

// One pair's ping statistics (0x14 bytes: table 0x00C71AE0, three floats, a counter); the name is a role name.
class PortNegotiationPing
{
public:
	PortNegotiationPing();									// 0x005DB84F
	virtual ~PortNegotiationPing() {}						// 0x005A66B8
	Real getFailures() const { return m_unk0x08; }			// +8 (the failure count)
	Real getOutstanding() const { return m_unk0x0C; }		// +0xC (the outstanding count)
	Real hasSamples();										// (1.0 when both +4 and +8 are positive)
	Real getLatency();										// 0x005DB95B (the ping estimate; +8 and +0xC feed it)
	void reset();											// 0x005DB86E (zeroes the three floats and the stamp)
	void recordPong(Int sendTime);							// 0x005DB885 (updates running latency average on pong)
	void incrementSent();									// 0x005DB8E4 (increments sent ping count)
	void setStats(Real a, Real b, Real c);					// 0x005DB8F7 (stores +4, +8, +0xC and stamps +0x10 with timeGetTime)
	Real getLossRate();										// 0x005DB928 ((+8 - +0xC - 1) clamped at zero over +8; 0 while +8 is 1 or less)
	Real m_unk0x04;											// +4
	Real m_unk0x08;											// +8
	Real m_unk0x0C;											// +0xC
	Int m_unk0x10;											// +0x10
};

struct PortNegotiationNotifyCall
{
	void (PortNegotiationListener::*notify)(void *, int, int);
	void *arg;
	int value;
	int extra;
};

// The 16-byte listener list at +4 (constructor 0x00330757; its destructor frees the first pointer).
struct PortNegotiationListenerList
{
	PortNegotiationListenerList();							// 0x00330757
	~PortNegotiationListenerList() { if (m_items != 0) free(m_items); }	// frees m_items
	void apply(const PortNegotiationNotifyCall &call);		// 0x005DBDF7
	void forEach(void (PortNegotiationListener::*notify)(void *, int, int), void *arg, int value, int extra);	// 0x005DBE6A
	PortNegotiationListener **m_items;						// +0
	PortNegotiationListener **m_end;						// +4
	PortNegotiationListener **m_cap;						// +8
	UnsignedInt m_index;									// +0xC
};

class PortNegotiationSchema
{
public:
	PortNegotiationSchema();								// 0x005DC43B
	virtual ~PortNegotiationSchema();						// slot 0, 0x005A71B1 (deleting 0x005A7200)

	void *peekPing(UnsignedShort first, UnsignedShort second);
	Int GetConnectionState(UnsignedShort first, UnsignedShort second);
	void attachSlotList(void *slotList, UnsignedShort localSlot);
	Bool IsConflictingWithProceedingNegotiation(UnsignedShort slot);
	Bool isAllConnected(Bool checkAll);						// 0x005DBA9C
	Short getRetryCount(UnsignedShort row, UnsignedShort column) const;		// 0x005DBB4D
	Int getRetryTimeout(UnsignedShort row, UnsignedShort column) const;		// 0x005DBB7A
	Int getTimeToSendPing(UnsignedShort slot);
	Bool isConnectionDone(UnsignedShort first, UnsignedShort second);
	Bool isRetryConnectingOverLimit(UnsignedShort first, UnsignedShort second);
	Int getActionID(UnsignedShort first, UnsignedShort second);
	Bool setPingStats(UnsignedShort first, UnsignedShort second, Real a, Real b, Real c);
	Bool receivedAPong(UnsignedShort first, UnsignedShort second, Int value);
	Bool sentAPingPacket(UnsignedShort first, UnsignedShort second);
	void negotiationStarted(UnsignedShort first, UnsignedShort second, Int cookie, Bool flag);
	void playerLeave(UnsignedShort slot);
	void reconnectPlayers(UnsignedShort first, UnsignedShort second, Bool resetRetries);
	void playerJoin(UnsignedShort slot);
	void handleNegotiationResult(UnsignedShort first, UnsignedShort second, Int cookie, Int result);

private:
	void rva005DB9E4();
public:

	PortNegotiationListenerList m_listeners;				// +0x04
	UnsignedShort m_slotCount;								// +0x14 (8 after the reset)
	unsigned char m_pad16[2];								// +0x16
	Int m_connectionState[8][8];							// +0x18
	Int m_actionID[8][8];									// +0x118
	PortNegotiationPing m_pings[8][8];						// +0x218
	Int m_timeToSendPing[8];								// +0x718
	Int m_unk0x738[8][8];									// +0x738
	UnsignedShort m_unk0x838[8][8];							// +0x838
	void *m_slotList;										// +0x8B8
};
// SIZE 0x8BC
typedef char PortNegotiationSchemaSize[(sizeof(PortNegotiationSchema) == 0x8BC) ? 1 : -1];

#endif
