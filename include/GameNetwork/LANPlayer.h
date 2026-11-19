// LANPlayer.h: a player sitting in the LAN lobby (Zero Hour's LANPlayer by role and member order, GPL-3.0-or-later; offsets are
// retail's, BFME II replaces the player's IP dword with a BfmeNetAddress).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts (code RVAs): SIZE 0x1C, no vptr and no base. RequestSetName 0x0044B7A9 allocates 0x1C bytes and initialises the
// three strings (+0, +4, +8), the dwords +0xC and +0x10 and the address (+0x14 dword, +0x18 word); LANGameSlot's constructor
// repeats those stores at +0x1AC. The copy constructor 0x0044A901 copies the three strings, then the dwords +0xC/+0x10/+0x14 and
// the word +0x18; the destructor 0x00447ACA releases +8, +4 and +0. LookupPlayer 0x0044998E compares +0x14 through
// BfmeNetAddress::Rva00248CBF; removePlayer 0x004499C7 and addPlayer 0x0044B40C walk the +0x10 link and compare the string at +0 and
// the address at +0x14.
#pragma once
#ifndef BFME2_LANPLAYER_H
#define BFME2_LANPLAYER_H

#include "Common/UnicodeString.h"
#include "GameNetwork/BfmeNetAddress.h"

class LANPlayer
{
public:
	LANPlayer() : m_lastHeard(0), m_next(0) {}				// 0x0044B864 region (RequestSetName builds it inline)
	LANPlayer(const LANPlayer &that);						// 0x0044A901
	~LANPlayer();											// 0x00447ACA
	LANPlayer &operator=(const LANPlayer &that);			// 0x004479FD (the map holds it as rva004479FD on a placeholder class)

	const UnicodeString &getName() const { return m_name; }
	UnicodeString getLogin() const { return m_login; }
	UnicodeString getHost() const { return m_host; }
	void setName(UnicodeString name) { m_name = name; }
	void setLogin(UnicodeString name) { m_login = name; }
	void setHost(UnicodeString name) { m_host = name; }
	UnsignedInt getLastHeard() const { return m_lastHeard; }
	void setLastHeard(UnsignedInt lastHeard) { m_lastHeard = lastHeard; }
	LANPlayer *getNext() const { return m_next; }
	void setNext(LANPlayer *next) { m_next = next; }
	UnsignedInt getIP() const { return m_address.m_ip; }
	const BfmeNetAddress *getAddress() const { return &m_address; }

	UnicodeString m_name;									// +0x00
	UnicodeString m_login;									// +0x04
	UnicodeString m_host;									// +0x08
	UnsignedInt m_lastHeard;								// +0x0C
	LANPlayer *m_next;										// +0x10
	BfmeNetAddress m_address;								// +0x14 (ip dword, port word)
};
// SIZE 0x1C (the address is 8 bytes with its padding word)
typedef char LANPlayerSize[(sizeof(LANPlayer) == 0x1C) ? 1 : -1];

#endif
