// PortNegotiationListener.h: the interface the NAT's PortNegotiationSchema notifies (a screen that draws the connection state).
// Target facts (code RVAs; table addresses are VAs):
//  - Base table VA 0x008711BC, three slots: 0 and 1 are the shared empty three-argument body 0x000D1407 (`ret 0xC`), 2 is the
//    deleting destructor 0x0059EB41. Four-byte object (the vptr). The listener list at NAT +0x2C (PortNegotiationListenerList)
//    holds pointers to these; AptOnlineCustomMatch (+0x6C, table VA 0x008713AC) and AptConnectionScreen (+0x58, table VA
//    0x00C7678C) are the two implementers, each overriding slots 0 and 1 and the destructor.
#pragma once
#ifndef BFME2_PORTNEGOTIATIONLISTENER_H
#define BFME2_PORTNEGOTIATIONLISTENER_H

class PortNegotiationListener
{
public:
	virtual void onNegotiationChanged(void *, int, int) {}	// slot 0
	virtual void onPairChanged(void *, int, int) {}			// slot 1
	virtual ~PortNegotiationListener() {}					// slot 2
};

#endif
