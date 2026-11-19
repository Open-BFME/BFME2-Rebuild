// LANGameInfo.h: the game record of a LAN lobby (Zero Hour's LANGameInfo by role, GPL-3.0-or-later: names and the
// member order are theirs where a retail body agrees; offsets and sizes are retail's).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts:
//  - SIZE 0xF6C: GameInfo (0xDC) then eight LANGameSlots of 0x1D0 bytes at +0xDC (the constructor 0x004481D4 runs the
//    vector constructor over 0x00447B0E with the destructor 0x00448150, then setSlotPointer for each), the dwords +0xF5C
//    (last heard), +0xF60 (next game in the list) and +0xF64, the byte +0xF68, and takes the local address from TheLAN's
//    slot 64 (+0x100).
//  - getLANSlot 0x00447773 (bounds-checked address of a slot), getLocalSlotNum 0x00447794, amIHost 0x004477C7 (slot 0 is
//    the local player), resetAccepted 0x004477E0 (TheLAN's slot 31, then unAccept on every slot).
#pragma once
#ifndef BFME2_LANGAMEINFO_H
#define BFME2_LANGAMEINFO_H

#include "GameNetwork/GameInfo.h"
#include "GameNetwork/LANPlayer.h"
#include "Common/AsciiString.h"

// A GameSlot with the LAN bookkeeping appended (0x1AC + 0x24 bytes; the extra bytes are not evidenced yet).
class LANGameSlot : public GameSlot
{
public:
	LANGameSlot();											// 0x00448150
	virtual LANGameSlot *slot5() { return this; }
	// The destructor is implicit and virtual (retail's 0x00447B0E releases m_serial, runs ~LANPlayer 0x00447ACA on m_user and ~GameSlot 0x002294FD
	// without storing a vtable, which is how MSVC 7.1 compiles an implicit destructor); slot 0 is its deleting copy 0x0044818B.
	Bool isLocalPlayer() const;								// 0x0044770F
	Bool isPlayer(UnicodeString userName) const;				// 0x004481A7 (the map holds it as isPlayer on a placeholder class: GameSlot::isPlayer(UnicodeString) with the compare's operands swapped)
	LANPlayer *getUser();									// 0x004479B1 (Zero Hour's LANGameSlot::getUser: fills m_user from the slot and returns it for a human slot; the map holds it as rva004479B1 on GameSlot)
	LANGameSlot &operator=(const LANGameSlot &that);		// 0x00447B58 (the map holds it as rva00447B58 on a placeholder class)
	void setSerial(AsciiString serial);						// 0x00581740 (the map holds it as setSerial on the placeholder class Rva00581777AsciiField)
	AsciiString getSerial() const;							// 0x00581777
	void setLogin(AsciiString login);						// 0x0024955E (translates the name into m_user's login)
	void setHost(AsciiString host);							// 0x00249595 (translates the name into m_user's host)
	void setUserLogin(UnicodeString login);					// 0x00447A5C (assigns m_user's login; the map holds it as rva00447A5C)
	void setUserHost(UnicodeString host);					// 0x00447A93 (assigns m_user's host; the map holds it as rva00447A93)

	LANPlayer m_user;										// +0x1AC (the destructor 0x00447B0E runs ~LANPlayer 0x00447ACA here; Zero Hour's LANGameSlot::m_user)
	AsciiString m_serial;									// +0x1C8 (the player's serial: setSerial 0x00581740 and getSerial 0x00581777 are the map's names; assigned by operator=, released first by the destructor)
	UnsignedInt m_lastHeard;								// +0x1CC (getUser copies it into m_user)
};
// SIZE 0x1D0
typedef char LANGameSlotSize[(sizeof(LANGameSlot) == 0x1D0) ? 1 : -1];

class LANGameInfo : public GameInfo
{
public:
	LANGameInfo();											// 0x004481D4
	virtual LANGameInfo *slot4() { return this; }
	virtual Int slot7() const { return 3; }
	void *getLANSlot(Int slotNum);							// 0x00447773 (the map types the result void *)
	void setSlotLastHeard(Int slotNum, UnsignedInt time);	// 0x00248D35 (stamps a slot's last-heard time; the map has no better name)
	virtual const char *GetSnapshotName() const;			// slot 2, 0x00448276 (the class name; the map holds it as name@Rva00448276Named)
	virtual const Image *rva00401015(Int kind);			// slot 6, 0x00447BE0 (the lobby status image of a column: the lock for a game in progress, the base class's otherwise)
	virtual Int getLocalSlotNum() const;					// slot 13, 0x00447794
	virtual void resetAccepted();							// slot 14, 0x004477E0
	virtual UnicodeString getGameName() const;				// slot 22, 0x0044829E (the map holds it as rva0044829E)
	UnicodeString getPlayerName(Int slot) const;			// 0x004482B7 (the name of a slot's player; the map holds it as rva004482B7 on a placeholder class)
	virtual UnicodeString getName();						// slot 23, 0x00448280 (the LAN game's name at +0xF64; the map holds it as rva00448280)

	LANGameInfo *getNext() const { return m_next; }
	void setNext(LANGameInfo *next) { m_next = next; }
	UnsignedInt getLastHeard() const { return m_lastHeard; }
	void setLastHeard(UnsignedInt lastHeard) { m_lastHeard = lastHeard; }
	const BfmeNetAddress *getHostAddress() const { return &m_LANSlot[0].m_address; }	// +0x114
	const BfmeNetAddress *getAddress(Int slot) const { return &m_LANSlot[slot].m_address; }
	void setName(UnicodeString name);						// 0x00449A81 (stores the game name at +4)

	LANGameSlot m_LANSlot[8];								// +0x0DC
	LANGameInfo *m_next;									// +0xF5C (RemoveGame 0x00449913 walks it)
	UnsignedInt m_lastHeard;								// +0xF60
	UnicodeString m_name;									// +0xF64 (the LAN game's name, set by setName 0x00449A81)
	Bool m_isDirectConnect;									// +0xF68
	unsigned char m_padF69[3];								// +0xF69
};
// SIZE 0xF6C
typedef char LANGameInfoSize[(sizeof(LANGameInfo) == 0xF6C) ? 1 : -1];

void GameInfoToByteBuffer(LANGameInfo *game, char *buffer, Int size);	// 0x00447CA9

#endif
