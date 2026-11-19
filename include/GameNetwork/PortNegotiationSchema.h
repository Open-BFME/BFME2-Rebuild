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
//    0x005DBB7A reads +0x738 and 0x005DBB4D the words at +0x838 (names not evidenced).
#pragma once
#ifndef BFME2_PORTNEGOTIATIONSCHEMA_H
#define BFME2_PORTNEGOTIATIONSCHEMA_H

#include <stdlib.h>
#include "Common/GameType.h"

// One pair's ping statistics (0x14 bytes: table 0x00C71AE0, three floats, a counter); the name is a role name.
class PortNegotiationPing
{
public:
	PortNegotiationPing();									// 0x005DB84F
	virtual ~PortNegotiationPing() {}						// 0x005A66B8
	Real getFailures() const { return m_unk0x08; }			// +8 (the old views call it the failure count)
	Real getOutstanding() const { return m_unk0x0C; }		// +0xC (the old views call it the outstanding count)
	Real hasSamples();										// 0x005A66BF (1.0 when both +4 and +8 are positive: the map holds it on a placeholder class)
	Real getLatency();										// 0x005DB95B (the ping estimate; +8 and +0xC feed it)
	void reset();											// 0x005DB86E (zeroes the three floats and the stamp; the map holds it on a placeholder class)
	void setStats(Real a, Real b, Real c);					// 0x005DB8F7 (stores +4, +8, +0xC and stamps +0x10 with timeGetTime)
	Real getLossRate();										// 0x005DB928 ((+8 - +0xC - 1) clamped at zero over +8; 0 while +8 is 1 or less)
	Real m_unk0x04;										// +4
	Real m_unk0x08;										// +8
	Real m_unk0x0C;										// +0xC
	Int m_unk0x10;											// +0x10
};

// The 16-byte listener list at +4 (constructor 0x00330757; its destructor frees the first pointer).
struct PortNegotiationListenerList
{
	PortNegotiationListenerList();							// 0x00330757
	~PortNegotiationListenerList() { if (m_items != 0) free(m_items); }	// frees m_items
	void *m_items;											// +0
	UnsignedInt m_unk0x04;									// +4
	UnsignedInt m_unk0x08;									// +8
	UnsignedInt m_unk0x0C;									// +0xC
};

class PortNegotiationSchema
{
public:
	PortNegotiationSchema();								// 0x005DC43B
	virtual ~PortNegotiationSchema();						// slot 0, 0x005A71B1 (deleting 0x005A7200)

	void *peekPing(UnsignedShort first, UnsignedShort second);	// 0x005DB98E
	Int GetConnectionState(UnsignedShort first, UnsignedShort second);	// 0x005DB9BC
	void attachSlotList(void *slotList, UnsignedShort localSlot);	// 0x005DBA3D
	Bool IsConflictingWithProceedingNegotiation(UnsignedShort slot);	// 0x005DBA60
	Int getTimeToSendPing(UnsignedShort slot);				// 0x005DBBA5
	Bool isConnectionDone(UnsignedShort first, UnsignedShort second);	// 0x005DBBC1
	Bool isRetryConnectingOverLimit(UnsignedShort first, UnsignedShort second);	// 0x005DBC2E
	Int getActionID(UnsignedShort first, UnsignedShort second);	// 0x005DBCA6
	Bool setPingStats(UnsignedShort first, UnsignedShort second, Real a, Real b, Real c);	// 0x005DBE95
	Bool receivedAPong(UnsignedShort first, UnsignedShort second, Int value);	// 0x005DBEEB
	Bool sentAPingPacket(UnsignedShort first, UnsignedShort second);	// 0x005DBF2D
	void negotiationStarted(UnsignedShort first, UnsignedShort second, Int cookie, Bool flag);	// 0x005DBF8C
	void playerLeave(UnsignedShort slot);					// 0x005DC070
	void reconnectPlayers(UnsignedShort first, UnsignedShort second, Int resetRetries);	// 0x005DC187 (the map spells the flag bool, _N; retail's callers push the caller's dword unchanged)
	void playerJoin(UnsignedShort slot);					// 0x005DC291
	void handleNegotiationResult(UnsignedShort first, UnsignedShort second, Int cookie, Int result);	// 0x005DC3C1

private:
	void rva005DB9E4();									// 0x005DB9E4: clears every array, the slot list pointer and sets the local slot to 8 (attachSlotList and the constructor call it; the map has no better name)
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
