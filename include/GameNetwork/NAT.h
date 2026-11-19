// NAT.h: the NAT negotiator (Zero Hour's NAT by role and by member order; BFME II replaced its per-round connection
// tables with a PortNegotiationSchema and eight address records, so the layout below is retail's, not Zero Hour's).
// Target facts (code RVAs; the map spells it `struct NAT`: TheNAT is `?TheNAT@@3PAUNAT@@A`):
//  - SIZE 0x974 (the constructor 0x005A721C writes through +0x970). It stores the table 0x00C71BFC, clears +4/+8, sets
//    +0xC to 8, clears +0x10..+0x25, runs the schema's constructor at +0x28 (0x005DC43B), clears the bytes at +0x8E4 and the
//    dwords at +0x8EC, allocates the eight 8-byte address records of +0x90C, sets +0x930 to 10, +0x934 to 0x7F00 and +0x970 to
//    1, and creates the global firewall helper (0x00595143 into VA 0x00E063B0) when it is missing.
//  - Destructor 0x005A734B (deleting 0x005A73C2): frees the eight address records, deletes the firewall helper through its
//    virtual destructor and clears the global, then runs the schema's destructor.
//  - +0x10 is the NATStateType update() returns (0x005A8F57: 1 = connecting, 2 = waiting for statistics); +0x14/+0x18/+0x20
//    are the local slot, target slot and negotiation cookie setConnectionState (0x005A688C) is called with.
#pragma once
#ifndef BFME2_NAT_H
#define BFME2_NAT_H

#include "Common/GameType.h"
#include "GameNetwork/BfmeNetAddress.h"
#include "GameNetwork/PortNegotiationSchema.h"

class Transport;
class GameSlot;
class GameInfo;
class AsciiString;
class FirewallHelperClass;

enum NATStateType
{
	NATSTATE_IDLE,
	NATSTATE_DOCONNECTIONPATHS,
	NATSTATE_WAITFORSTATS,
	NATSTATE_DONE,
	NATSTATE_FAILED
};

struct NAT
{
public:
	NAT();															// 0x005A721C
	virtual ~NAT();													// slot 0, 0x005A734B (deleting 0x005A73C2)

	Bool isReady() const;									// 0x005A6709 (the map keeps it as Rva005A6709::rva005A6709; callers test al)
	Bool isConnectionFinished() const;								// 0x005A6732
	void setHostSlot(UnsignedShort slot);							// 0x005A683C
	UnsignedInt get(UnsignedInt slot) const;						// 0x005A684F (the port of the slot's address record)
	void setLocalPort(Short port);									// 0x005A6869
	Bool isWaitingToBegin() const;									// 0x005A687F
	void setConnectionState(Int first, Int second, Int cookie, Int state);	// 0x005A688C
	void processPlayerJoin(Int slot);								// 0x005A6903
	void processPlayerLeave(Int slot);								// 0x005A695F
	void reconnectPlayers(Int first, Int second, Int resetRetries);	// 0x005A69C0
	void reconnectStalePairs();									// 0x005A6CA5 (reconnects the pairs whose last ping is older than 12 seconds)
	void retryFailedPairs();										// 0x005A6D47 (reconnects the pairs whose retries ran over the limit)
	void copyAddressesToSlots();									// 0x005A6A4C
	void probeMangler();											// 0x005A6A83
	void attachSlotList(GameInfo *game, Int localSlot, UnsignedInt localIP, UnsignedInt unused, Bool isHost);	// 0x005A6B14
	void rva005A7974(UnsignedShort port, void *targetSlot);		// 0x005A7974 (sends the PORT request for the target slot; the map has no better name)
	void sendPings();												// 0x005A6EF2
	void notifyTargetOfProbe(void *slot);							// 0x005A73DE (the map types it Rva005A7A96Slot *)
	void notifyConnectionFromTargetDone();							// 0x005A74D8
	void notifyConnectionToTargetFailed();							// 0x005A7683
	void notifyHostMyNATCreated(Int hostSlot);						// 0x005A7829
	void notifyNATHostSlot();										// 0x005A7C9C
	void processUDPPacket();										// 0x005A7EDC
	void sendAProbeToTarget();										// 0x005A831E
	void findMyMangledPort();										// 0x005A8405
	void processManglerResponse(UnsignedShort mangledPort);			// 0x005A854C
	void targetNotifyMeIWasProbed(Int slot);						// 0x005A8666
	void gotTargetMangledPort(Int slot, UnsignedShort mangledPort, Int mangledAddress);	// 0x005A86C0
	Int connectionUpdate();											// 0x005A879B
	void StartNegotiationAction(Int target, Int cookie);			// 0x005A89C2
	void processGlobalMessage(Int slotNum, const char *options);	// 0x005A8ABF
	Bool SetUDPSocketForSlot(UnsignedShort port, UnsignedShort slot, void *address);	// 0x005A6DAC
	AsciiString getConnectionStateName(Int state);					// 0x005A6E7A
	void setCurrentConnectionState(Int state);						// 0x005A6C90

protected:
	void generatePortNumbers(GameSlot **slotList, Int localSlot);	// 0x005A678E

public:
	Transport *m_transport;											// +0x04
	GameSlot **m_slotList;											// +0x08
	Int m_hostSlot;													// +0x0C (8 = none)
	Int m_NATState;													// +0x10
	Int m_localSlot;												// +0x14
	Int m_targetSlot;												// +0x18
	UnsignedInt m_localIP;											// +0x1C
	Int m_cookie;													// +0x20
	Bool m_unk0x24;													// +0x24
	Bool m_unk0x25;													// +0x25
	unsigned char m_pad26[2];										// +0x26
	PortNegotiationSchema m_schema;									// +0x28
	Bool m_hostKnown[8];											// +0x8E4
	Int m_slotCookie[8];											// +0x8EC
	BfmeNetAddress *m_addresses[8];									// +0x90C
	Int m_numRetries;												// +0x92C
	Int m_maxNumRetriesAllowed;										// +0x930 (10)
	UnsignedShort m_packetID;										// +0x934 (0x7F00)
	UnsignedShort m_spareSocketPort;								// +0x936
	UnsignedInt m_manglerRetryTime;									// +0x938
	Int m_manglerRetries;											// +0x93C
	UnsignedShort m_previousSourcePort;								// +0x940
	Bool m_beenProbed;												// +0x942
	Bool m_probeAnnounced;												// +0x943
	UnsignedInt m_manglerAddress;									// +0x944
	UnsignedInt m_timeTillNextSend;									// +0x948
	Int m_connectionState;											// +0x94C
	Int m_previousConnectionState;									// +0x950
	UnsignedInt m_unk0x954;											// +0x954
	UnsignedShort m_startingPortNumber;								// +0x958
	unsigned char m_pad95A[2];										// +0x95A
	UnsignedInt m_nextPortSendTime;									// +0x95C
	UnsignedInt m_timeoutTime;										// +0x960
	UnsignedInt m_roundTimeout;										// +0x964
	UnsignedInt m_negotiationTimeout;							// +0x968 (attachSlotList stores timeGetTime() plus its timeout argument)
	UnsignedInt m_nextHostTick;										// +0x96C
	Bool m_active;													// +0x970 (1 after the constructor)
	unsigned char m_pad971[3];										// +0x971
};
// SIZE 0x974
typedef char NATSize[(sizeof(NAT) == 0x974) ? 1 : -1];

extern NAT *TheNAT;													// VA 0x00E063F8

#endif
