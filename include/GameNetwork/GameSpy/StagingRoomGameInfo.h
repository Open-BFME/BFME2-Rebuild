// StagingRoomGameInfo.h: the game record of a GameSpy staging room (Zero Hour's GameSpyStagingRoom and GameSpyGameSlot by role
// and member order, GPL-3.0-or-later; offsets and sizes are retail's).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts (code RVAs, table VA 0x00C19440):
//  - SIZE 0x1020: GameInfo (0xDC) then eight GameSpyGameSlots of 0x1E0 bytes at +0xDC (the constructor 0x004FDE4D runs the vector
//    constructor over the slot constructor 0x004FDD8B and the destructor 0x00382398), then the members from +0xFDC (the
//    constructor's stores: +0xFDC = 0, +0xFE4 = 0, the string +0xFE8 set to the empty string, +0xFF4 byte, +0xFF8, the
//    strings +0xFFC and +0x1000 cleared, the word +0x1008, +0x1018 and +0x101C = 0).
//  - GameSpyInfo keeps one by value at +0x5E4 and passes it by value to addStagingRoom 0x003857EB, updateStagingRoom
//    0x00383784 and removeStagingRoom 0x003837D3.
//  - Table (24 slots): the GameInfo order; the slots this class replaces are 0 deleting destructor 0x00383768, 2 name getter
//    0x0038374D, 3 xfer 0x004FDAC7, 5 0x0047A6A9 (the shared `mov eax,ecx; ret`), 6 0x004FDC78, 7 0x0044827C, 8 0x00383753,
//    9 0x0038375A, 10 reset 0x004FDC73, 11 startGame 0x004FF09F, 12 amIHost 0x004FDBAA, 13 getLocalSlotNum 0x004FDBF5,
//    14 resetAccepted 0x004FDA62, 21 0x00383761, and a slot 23 of its own, 0x004FDA42.
#pragma once
#ifndef BFME2_STAGINGROOMGAMEINFO_H
#define BFME2_STAGINGROOMGAMEINFO_H

#include <stddef.h>
#include "GameNetwork/GameInfo.h"
#include "GameNetwork/GameSlot.h"

class Transport;

// A GameSlot with the GameSpy identity appended (0x1AC + 0x34 bytes; Zero Hour's profile id, login, locale, ping string and
// ping value, wins, losses, rank points and favourite side are nine of the thirteen dwords; the layout of the bytes is not
// evidenced yet).
class GameSpyGameSlot : public GameSlot
{
public:
	GameSpyGameSlot();										// 0x004FDD8B
	virtual GameSpyGameSlot *slot6() { return this; }
	GameSpyGameSlot &operator=(const GameSpyGameSlot &that);	// 0x003824A5 (the map holds it as rva003824A5 on a placeholder class: GameSlot's operator=, then the members in order)
	AsciiString getPingString() const;						// 0x00382216 (the string at +0x1B8; the map holds it as get@Rva00382216AsciiField, row_attributions: caller-this GameSlot)
	void setPingString(AsciiString pingString);				// 0x004FDB04 (the map holds it as rva004FDB04 on a placeholder class: stores the string and the ping value the info object computes from it)
	AsciiString getLoginName() const;						// 0x004FDCE1 (the map holds it as get@Rva004FDCE1AsciiField)
	void setLoginName(AsciiString name);					// 0x004FDCFF (the map names it setGameSpyName on that placeholder class)
	void setClan(AsciiString clan);							// 0x004FDD36 (the map names it setClan on a placeholder class)
	AsciiString getLastString() const;						// 0x004FDD6D (the second string of the stats auth key command, the one it prints as authToken; processGameSpyStatsAuthKeyCommand 0x004CFE24 stores it through setLastString 0x004CFBC2)
	AsciiString getAuthKey() const;							// 0x004CFB6D (the first string of that command, stored by processGameSpyStatsAuthKeyCommand 0x004CFE24)
	void setAuthKey(AsciiString key);						// 0x004CFB8B
	void setLastString(AsciiString token);					// 0x004CFBC2

	Int m_profileID;										// +0x1AC (Zero Hour's order; not read here)
	AsciiString m_loginName;								// +0x1B0
	AsciiString m_clan;										// +0x1B4
	AsciiString m_pingString;								// +0x1B8 (setPingString 0x004FDB04)
	Int m_pingValue;										// +0x1BC
	Int m_wins;												// +0x1C0 (Zero Hour's order after the ping value: wins, losses, rank points, favourite side;
	Int m_losses;											// +0x1C4  operator= 0x003824A5 copies the six dwords one at a time)
	Int m_rankPoints;										// +0x1C8
	Int m_favoriteSide;										// +0x1CC
	Int m_unk0x1D0;											// +0x1D0
	Int m_unk0x1D4;											// +0x1D4
	AsciiString m_authKey;									// +0x1D8 (getAuthKey 0x004CFB6D)
	AsciiString m_lastString;								// +0x1DC (getLastString 0x004FDD6D)
};
typedef char GameSpyGameSlotSize[(sizeof(GameSpyGameSlot) == 0x1E0) ? 1 : -1];

class GameSpyStagingRoom : public GameInfo
{
public:
	GameSpyStagingRoom();									// 0x004FDE4D
	virtual GameSpyStagingRoom *slot5() { return this; }
	virtual Int slot7() const { return 3; }
	virtual ~GameSpyStagingRoom();							// slot 0, deleting 0x00383768
	virtual const char *GetSnapshotName() const;			// slot 2, 0x0038374D (returns "GameSpyStagingRoom")
	virtual Int slot8() const;								// slot 8, 0x00383753 (the dword at +0x100C: Zero Hour's reported player count)
	virtual Int slot9() const;								// slot 9, 0x0038375A (the dword at +0x1010: its reported player limit)
	virtual Int slot21();									// slot 21, 0x00383761 (the ping value at +0x1004)
	void cleanUpSlotPointers();								// 0x004FDA17
	virtual Bool amIHost() const;							// slot 12, 0x004FDBAA
	virtual void resetAccepted();							// slot 14, 0x004FDA62
	virtual void xfer(Xfer *xfer);							// slot 3, 0x004FDAC7 (the version, then GameInfo's record; nothing at all on a light CRC)
	virtual Int getLocalSlotNum() const;					// slot 13, 0x004FDBF5
	virtual const Image *rva00401015(Int column);			// slot 6, 0x004FDC78 (the lobby image of a column: the lock when the room asks for a password)
	void setPingString(AsciiString pingString);				// 0x004FDB57 (stores the string and the value the info object computes from it; the map holds it as rva004FDB57 on a placeholder class)
	Int getID() const { return m_id; }						// Zero Hour's inline getter: the staging room map's key, passed to find through a copy
	void setLocalName(AsciiString name);					// 0x00382234 (stores the string at +0xFE8, the one the constructor sets to the empty string; the map holds it as rva00382234 on a placeholder class)
	void assign(UnicodeString name);						// 0x003821E2 (the map's name; stores the game name at +4 of the GameInfo)
	void rva0059F2EB(AsciiString value);					// 0x0059F2EB: stores the string at +0xFFC (the map holds it on a placeholder class)
	AsciiString rva0059F322() const;						// 0x0059F322: the string at +0xFFC (the map holds it as get@Rva0059F322AsciiField)
	GameSpyGameSlot *getGameSpySlot(Int slotNum);			// 0x004FDA3D: a five-byte `jmp` to GameInfo::getSlot
	virtual void reset();									// slot 10, 0x004FDC73: a five-byte `jmp` to GameInfo::reset
	virtual void rva004FDA42();

	GameSpyGameSlot m_gameSpySlot[MAX_SLOTS];				// +0x0DC
	UnicodeString m_roomName;								// +0xFDC (cleared by the constructor)
	Int m_id;												// +0xFE0 (the staging room map's key: removeStagingRoom 0x003837D3 and addStagingRoom 0x003857EB read it from the room they are given)
	Transport *m_transport;									// +0xFE4 (the constructor clears it)
	AsciiString m_localName;								// +0xFE8 (set to the empty string by the constructor)
	Bool m_requiresPassword;								// +0xFEC (rva00401015 0x004FDC78 tests it)
	unsigned char m_unk0xFED[7];							// +0xFED
	Bool m_unk0xFF4;										// +0xFF4
	unsigned char m_pad0xFF5[3];							// +0xFF5
	Int m_unk0xFF8;											// +0xFF8
	AsciiString m_unk0xFFC;									// +0xFFC (cleared by the constructor)
	AsciiString m_pingString;								// +0x1000 (setPingString 0x004FDB57)
	Int m_pingValue;										// +0x1004
	UnsignedShort m_unk0x1008;								// +0x1008
	unsigned char m_pad0x100A[2];							// +0x100A
	Int m_reportedNumPlayers;								// +0x100C (slot 8 0x00383753)
	Int m_reportedMaxPlayers;								// +0x1010 (slot 9 0x0038375A)
	Int m_reportedNumObservers;								// +0x1014 (Zero Hour's third counter; not read here)
	Int m_unk0x1018;										// +0x1018
	Int m_unk0x101C;										// +0x101C
};
typedef char GameSpyStagingRoomSize[(sizeof(GameSpyStagingRoom) == 0x1020) ? 1 : -1];
typedef char GameSpyStagingRoomSlotOffset[(offsetof(GameSpyStagingRoom, m_gameSpySlot) == 0xDC) ? 1 : -1];

extern GameSpyStagingRoom *TheGameSpyGame;

#endif
