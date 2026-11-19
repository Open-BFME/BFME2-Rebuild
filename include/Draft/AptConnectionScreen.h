// AptConnectionScreen: the online connection grid ("Connection" Apt screen: eight rows by eight columns of port negotiation
// states and the eight player names), the screen CustomMatch opens while the NAT negotiates.
// Target facts (retail game.dat; RVAs, table addresses are VAs):
//  - Constructor 0x005DB6E2 (one argument, the Apt level), destructor 0x005DB681 (deleting 0x005DB6DA). It runs AptCallbackHost's
//    constructor 0x002D2C34 (0x58 bytes), stores the primary table VA 0x00C76798 at +0 and the NAT listener table VA 0x00C7678C at
//    +0x58 (a 4-byte listener subobject: its slot 0 0x005DB5F2 and slot 1 0x005DB628 are `this - 0x58` forwarders to UpdateGrid and
//    UpdateSlotName), the level at +0x5C. SIZE 0x60. While TheNAT exists the constructor appends the listener to the NAT's listener
//    list (0x005A0B4C on TheNAT+0x2C) and binds the Apt commands "Connection::RedrawGrid" (0x005DB647) and
//    "Connection::OnRetryConnections" (0x005DB36A) through AptCallbackHost's command map; the destructor removes the listener again
//    (0x002B7250).
//  - The level (+0x5C) is the owner every window-manager invoke of the screen passes (UpdateGrid 0x005DB512).
#pragma once
#ifndef BFME2_APTCONNECTIONSCREEN_H
#define BFME2_APTCONNECTIONSCREEN_H

#include "GameClient/AptCallbackHost.h"

class PortNegotiationSchema;
class PortNegotiationPing;

class AptConnectionScreen : public AptCallbackHost
{
public:
	AptConnectionScreen(void *level);						// 0x005DB6E2
	virtual ~AptConnectionScreen();							// 0x005DB681

	void OnRetryConnections(const char *);					// 0x005DB36A: "Connection::OnRetryConnections", asks the NAT to retry its failed pairs
	static int GetPingImageEnum(PortNegotiationPing *ping);	// 0x005DB37C: the ping quality band (0 none .. 3 bad) of a measured pair
	void UpdateSlotName(int slot);							// 0x005DB3B3: shows the slot's player name and number
	void UpdateGrid(PortNegotiationSchema *schema, int row, int column);	// 0x005DB512: plays the cell's "UpdateConnection_<row>_<column>" state
	void rva005DB630();										// 0x005DB630: refreshes the eight slot names
	void RedrawGrid(const char *);							// 0x005DB647: "Connection::RedrawGrid"

	unsigned char m_listener[4];							// +0x58: the NAT listener subobject (table VA 0x00C7678C)
	void *m_level;											// +0x5C
};
typedef char AptConnectionScreenSizeCheck[sizeof(AptConnectionScreen)==0x60 ? 1 : -1];

#endif
