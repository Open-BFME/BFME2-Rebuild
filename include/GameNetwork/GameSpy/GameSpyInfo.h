// GameSpyInfo.h: what the main thread knows about GameSpy: the group rooms, the staging rooms, the players in the room, the buddy
// lists, the local login and the ignore lists (Zero Hour's GameSpyInfo by role and member order, GPL-3.0-or-later; offsets and
// slot numbers are retail's).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts (code RVAs, table VA 0x00C19500):
//  - Constructor 0x00385FE1 stores the table at +0, zeroes the strings at +0x0C/+0x10/+0x14, builds the maps at +0x18 (groups),
//    +0x24 (staging rooms), +0x34 (buddies), +0x40 (buddy requests), +0x4C (players) and the list at +0x58 (buddy messages,
//    whose header node is 0x20 bytes: two links and one 0x18-byte record), the strings +0x60/+0x70/+0x78/+0x7C/+0x80, the
//    player statistics at +0x84 (0x0055621F), the staging room at +0x5E4 (0x004FDE4D), the sets and map at +0x1608/+0x1614/
//    +0x1620/+0x162C, runs reset 0x00385D25 and stores the staging room's address into the globals TheGameSpyGame
//    (0x00E02324) and 0x00E02EEC (TheGameInfo).
//  - reset 0x00385D25: +4/+5 = 0, +0x5C = 0, the string +0x60 cleared, +0x64/+0x68 = 0, clearGroupRoomList (slot 2), clearStagingRoomList
//    (slot 0x28), the buddy maps and the list cleared, the staging room reset (its slot 10), +0x5DC/+0x5E0/+0x1604 = 0, +0x6C = 0,
//    +0x70 set to the empty string, +0x74 = 0, +0x5D8 = 100, +0x5CC/+0x5CD = 0, +0x8 = 0, the strings +0x80/+0x7C/+0x78/+0x14/
//    +0x10/+0x0C cleared, +0x5D0/+0x5D4 = 0, the saved-ignore map and the preorder set cleared, the statistics reset, +0x1638
//    = -1 and +0x163C = 0.
//  - The table has 99 slots (to +0x188). The record accessors the map names are named; the folded getters and setters (a
//    `lea` of a member, a dword load) take their names from Zero Hour's interface at the same place in the order.
#pragma once
#ifndef BFME2_GAMESPYINFO_H
#define BFME2_GAMESPYINFO_H

#include <stddef.h>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"
#include "GameNetwork/GameSpy/PeerDefs.h"
#include "GameNetwork/GameSpy/StagingRoomGameInfo.h"

class GameSpyInfo : public GameSpyInfoInterface
{
public:
	GameSpyInfo();																// 0x00385FE1
	virtual ~GameSpyInfo();														// slot 0x00, deleting 0x0038745D
	virtual void reset();														// slot 0x04, 0x00385D25
	virtual void clearGroupRoomList();											// slot 0x08, 0x003862CA
	virtual GroupRoomMap *getGroupRoomList();									// slot 0x0C, 0x0036CBE7
	virtual void addGroupRoom(GameSpyGroupRoom room);							// slot 0x10, 0x00386B7B
	virtual Bool gotGroupRoomList();											// slot 0x14, 0x003860EF
	virtual void joinGroupRoom(Int groupID);									// slot 0x18, 0x003853DA
	virtual void leaveGroupRoom();												// slot 0x1C, 0x0038544D
	virtual void vslot20();														// slot 0x20, 0x003854C5
	virtual void joinBestGroupRoom(Int groupID);								// slot 0x24, 0x00385678
	virtual void joinPreferredGroupRoom(Bool flag, Int groupID);				// slot 0x28, 0x0038553D
	virtual void setCurrentGroupRoom(Int groupID);								// slot 0x2C, 0x003862DB
	virtual Int getCurrentGroupRoom();											// slot 0x30, 0x0008BB81
	virtual void vslot34(AsciiString value);									// slot 0x34, 0x00386139
	virtual AsciiString vslot38();												// slot 0x38, 0x0038616D
	virtual void vslot3C(Int value);											// slot 0x3C, 0x00238D83
	virtual Int vslot40();														// slot 0x40, 0x002004A1
	virtual void vslot44(Int value);											// slot 0x44, 0x003674FE
	virtual Int vslot48();														// slot 0x48, 0x00367508
	virtual void updatePlayerInfo(PlayerInfo info, AsciiString oldNick);		// slot 0x4C, 0x00386EF8
	virtual void playerLeftGroupRoom(AsciiString nick);							// slot 0x50, 0x00384660
	virtual PlayerInfoMap *getPlayerInfoMap();									// slot 0x54, 0x003EFDD7
	virtual PlayerInfo *findPlayerInfoByLocale(const char *locale);				// slot 0x58, 0x00382CCE (compares the locale string)
	virtual PlayerInfo *findPlayerInfoByProfileID(Int profileID);				// slot 0x5C, 0x00382D0A
	virtual BuddyInfoMap *getBuddyMap();										// slot 0x60, 0x002B236A
	virtual BuddyInfoMap *getBuddyRequestMap();									// slot 0x64, 0x0049CB86
	virtual BuddyMessageList *getBuddyMessages();								// slot 0x68, 0x00042287
	virtual Bool isBuddy(Int profileID);										// slot 0x6C, 0x003835D7
	virtual void setLocalName(AsciiString name);								// slot 0x70, 0x00386188
	virtual AsciiString getLocalName();											// slot 0x74, 0x003861BC
	virtual void setLocalProfileID(Int profileID);								// slot 0x78, 0x002D94FE
	virtual Int getLocalProfileID();											// slot 0x7C, 0x000421C4
	virtual AsciiString getLocalEmail();										// slot 0x80, 0x003861D7
	virtual void setLocalEmail(AsciiString email);								// slot 0x84, 0x003861F2
	virtual AsciiString getLocalPassword();										// slot 0x88, 0x00386226
	virtual void setLocalPassword(AsciiString password);						// slot 0x8C, 0x00386241
	virtual void setLocalBaseName(AsciiString name);							// slot 0x90, 0x00386275
	virtual AsciiString getLocalBaseName();										// slot 0x94, 0x003862AC
	virtual void setCachedLocalPlayerStats(PSPlayerStats stats);				// slot 0x98, 0x00387479
	virtual PSPlayerStats getCachedLocalPlayerStats();							// slot 0x9C, 0x003862ED
	virtual void clearStagingRoomList();										// slot 0xA0, 0x00382E2F
	virtual StagingRoomMap *getStagingRoomList();								// slot 0xA4, 0x00091A25
	virtual GameSpyStagingRoom *findStagingRoomByID(Int id);					// slot 0xA8, 0x00383846
	virtual void addStagingRoom(GameSpyStagingRoom room);						// slot 0xAC, 0x003857EB
	virtual void updateStagingRoom(GameSpyStagingRoom room);					// slot 0xB0, 0x00383784
	virtual void removeStagingRoom(GameSpyStagingRoom room);					// slot 0xB4, 0x003837D3
	virtual Bool hasStagingRoomListChanged();									// slot 0xB8, 0x00381DDA
	virtual void leaveStagingRoom();											// slot 0xBC, 0x003858DF
	virtual void markAsStagingRoomHost();										// slot 0xC0, 0x00382623
	virtual void markAsStagingRoomJoiner(Int game);								// slot 0xC4, 0x00383865
	virtual void sawFullGameList();												// slot 0xC8, 0x003860FA
	virtual Bool amIHost();														// slot 0xCC, 0x00381DBD
	virtual Bool vslotD0();														// slot 0xD0, 0x00381DC4
	virtual GameSpyStagingRoom *getCurrentStagingRoom();						// slot 0xD4, 0x003835A0
	virtual Bool validateStagingRoom(GameSpyStagingRoom *room);					// slot 0xD8, 0x00383207
	virtual void setGameOptions();												// slot 0xDC, 0x00386501
	virtual Int getCurrentStagingRoomID();										// slot 0xE0, 0x003860F3
	virtual void setDisallowAsianText(Bool value);								// slot 0xE4, 0x00381E05
	virtual void setDisallowNonAsianText(Bool value);							// slot 0xE8, 0x00381E12
	virtual Bool getDisallowAsianText();										// slot 0xEC, 0x00381E1F
	virtual Bool getDisallowNonAsianText();										// slot 0xF0, 0x00381E26
	virtual void addText(UnicodeString text, Int color);														// slot 0xF4, 0x001EF552 (see PeerDefs.h)
	virtual void vslotF8();														// slot 0xF8, 0x001EF90E
	virtual void addChat(PlayerInfo info, UnicodeString message, Bool isPublic, Bool isAction);	// slot 0xFC, 0x001EF9A0
	virtual Bool sendChat(UnicodeString text, Bool isAction, const SelPair *selection);													// slot 0x100, 0x001EFD9C (see PeerDefs.h)
	virtual Bool vslot104(UnicodeString text);													// slot 0x104, 0x001EFCF7 (see PeerDefs.h)
	virtual void setMOTD(const AsciiString &motd);								// slot 0x108, 0x0038200D
	virtual const AsciiString &getMOTD();										// slot 0x10C, 0x001DBA69
	virtual void setConfig(const AsciiString &config);							// slot 0x110, 0x00382015
	virtual const AsciiString &getConfig();										// slot 0x114, 0x003F712F
	virtual void setPingString(const AsciiString &ping);						// slot 0x118, 0x0030D420
	virtual const AsciiString &getPingString();									// slot 0x11C, 0x005C4AD9
	virtual Int getPingValue(const AsciiString &otherPing);						// slot 0x120, 0x0038277F
	virtual void addToBuddyList(Int profileID);									// slot 0x124, 0x00382767
	virtual void removeBuddy(Int profileID);									// slot 0x128, 0x003871F6
	virtual void addToSavedIgnoreList(Int profileID, AsciiString nick);			// slot 0x12C, 0x00385E2A
	virtual void removeFromSavedIgnoreList(Int profileID);						// slot 0x130, 0x00385B1C
	virtual Bool isSavedIgnored(Int profileID);									// slot 0x134, 0x003839B9
	virtual SavedIgnoreMap *getSavedIgnoreMap();								// slot 0x138, 0x00381DFE
	virtual void loadSavedIgnoreList();											// slot 0x13C, 0x00385B78
	virtual IgnoreList returnIgnoreList();										// slot 0x140, 0x00385AFE
	virtual void addToIgnoreList(AsciiString nick);								// slot 0x144, 0x003846A8
	virtual void removeFromIgnoreList(AsciiString nick);						// slot 0x148, 0x003871BF
	virtual Bool isIgnored(AsciiString nick);									// slot 0x14C, 0x003846E5
	virtual void setLocalIPs(UnsignedInt internalIP, UnsignedInt externalIP);	// slot 0x150, 0x00381D9E
	virtual UnsignedInt getInternalIP();										// slot 0x154, 0x0043813D
	virtual UnsignedShort vslot158();											// slot 0x158, 0x003860FF (returns the constant 0x1F98)
	virtual UnsignedInt getExternalIP();										// slot 0x15C, 0x0028A936
	virtual Bool isDisconnectedAfterGameStart(Int *reason) const;				// slot 0x160, 0x00386104
	virtual void markAsDisconnectedAfterGameStart(Int reason);					// slot 0x164, 0x00386117
	virtual Bool didPlayerPreorder(Int profileID) const;						// slot 0x168, 0x00383580
	virtual void markPlayerAsPreorder(Int profileID);							// slot 0x16C, 0x00383E80
	virtual void setMaxMessagesPerUpdate(Int num);								// slot 0x170, 0x00381E2D
	virtual Int getMaxMessagesPerUpdate();										// slot 0x174, 0x00381E3A
	virtual Int getAdditionalDisconnects();										// slot 0x178, 0x000D43D0 (the shared `xor eax,eax` body: retail returns 0, as Zero Hour's does)
	virtual void clearAdditionalDisconnects();									// slot 0x17C, 0x00381DB5
	virtual void readAdditionalDisconnects();									// slot 0x180, 0x003853C6
	virtual UnsignedByte get() const;											// slot 0x184, 0x00386125 (the byte at +0x163C; the ledger's name)
	virtual void set(UnsignedByte value);										// slot 0x188, 0x0038612C (the ledger's name)

	Bool m_sawFullGameList;						// +0x04 (sawFullGameList 0x003860FA sets it)
	Bool m_disconnectedAfterGameStart;			// +0x05 (markAsDisconnectedAfterGameStart 0x00386117 sets it, isDisconnectedAfterGameStart 0x00386104 reads it)
	unsigned char m_pad0x06[2];					// +0x06
	Int m_disconnectedAfterGameStartValue;		// +0x08 (the argument of markAsDisconnectedAfterGameStart)
	AsciiString m_motd;							// +0x0C (setMOTD 0x0038200D)
	AsciiString m_config;						// +0x10 (setConfig 0x00382015)
	AsciiString m_pingString;					// +0x14 (setPingString 0x0030D420)
	GroupRoomMap m_groupRooms;					// +0x18
	StagingRoomMap m_stagingRooms;				// +0x24 (findStagingRoomByID 0x00383846 finds in it)
	Bool m_stagingRoomListChanged;				// +0x30 (hasStagingRoomListChanged 0x00381DDA reads and clears it)
	unsigned char m_pad0x31[3];					// +0x31
	BuddyInfoMap m_buddyMap;					// +0x34 (isBuddy 0x003835D7 finds in it)
	BuddyInfoMap m_buddyRequestMap;				// +0x40
	PlayerInfoMap m_playerInfoMap;				// +0x4C
	BuddyMessageList m_buddyMessages;			// +0x58
	Int m_currentGroupRoomID;					// +0x5C (setCurrentGroupRoom 0x003862DB stores it and empties the player map)
	AsciiString m_unk0x60;						// +0x60 (slots 0x34 and 0x38)
	Int m_unk0x64;								// +0x64 (slots 0x3C and 0x40)
	Int m_unk0x68;								// +0x68 (slots 0x44 and 0x48)
	Bool m_gotGroupRoomList;					// +0x6C (gotGroupRoomList 0x003860EF)
	unsigned char m_pad0x6D[3];					// +0x6D
	AsciiString m_localName;					// +0x70 (getLocalName 0x003861BC, setLocalName 0x00386188)
	Int m_localProfileID;						// +0x74 (readAdditionalDisconnects 0x003853C6 passes it to GetAdditionalDisconnectsFromUserFile)
	AsciiString m_localPassword;				// +0x78 (getLocalPassword 0x00386226, setLocalPassword 0x00386241)
	AsciiString m_localEmail;					// +0x7C (getLocalEmail 0x003861D7, setLocalEmail 0x003861F2)
	AsciiString m_localBaseName;				// +0x80 (getLocalBaseName 0x003862AC, setLocalBaseName 0x00386275)
	PSPlayerStats m_cachedLocalPlayerStats;		// +0x84
	Bool m_disallowAsianText;					// +0x5CC (0x00381E05 sets it, 0x00381E1F reads it)
	Bool m_disallowNonAsianText;				// +0x5CD (0x00381E12 sets it, 0x00381E26 reads it)
	unsigned char m_pad0x5CE[2];				// +0x5CE
	UnsignedInt m_localIP;						// +0x5D0 (setLocalIPs 0x00381D9E)
	UnsignedInt m_localIP2;						// +0x5D4
	Int m_maxMessagesPerUpdate;					// +0x5D8 (0x00381E2D sets it, 0x00381E3A reads it)
	Int m_unk0x5DC;								// +0x5DC (slot 0xD0 and getCurrentStagingRoom read it)
	Bool m_unk0x5E0;							// +0x5E0 (amIHost 0x00381DBD)
	unsigned char m_pad0x5E1[3];				// +0x5E1
	GameSpyStagingRoom m_localStagingRoom;		// +0x5E4
	Int m_currentStagingRoomID;					// +0x1604 (getCurrentStagingRoomID 0x003860F3)
	IgnoreList m_ignoreList;					// +0x1608 (addToIgnoreList 0x003846A8, isIgnored 0x003846E5)
	SavedIgnoreMap m_savedIgnoreMap;			// +0x1614 (isSavedIgnored 0x003839B9)
	_STL::set<AsciiString> m_unk0x1620;			// +0x1620 (a set built by the constructor with 0x000D3A71; the element type is not evidenced)
	_STL::set<Int> m_preorderPlayers;			// +0x162C (didPlayerPreorder 0x00383580, markPlayerAsPreorder 0x00383E80)
	Int m_additionalDisconnects;				// +0x1638 (readAdditionalDisconnects 0x003853C6)
	UnsignedByte m_unk0x163C;					// +0x163C (slots 0x184 and 0x188)
	unsigned char m_pad0x163D[3];				// +0x163D
};
// SIZE 0x1640
typedef char GameSpyInfoSize[(sizeof(GameSpyInfo) == 0x1640) ? 1 : -1];
typedef char GameSpyInfoStatsOffset[(offsetof(GameSpyInfo, m_cachedLocalPlayerStats) == 0x84) ? 1 : -1];
typedef char GameSpyInfoRoomOffset[(offsetof(GameSpyInfo, m_localStagingRoom) == 0x5E4) ? 1 : -1];
typedef char GameSpyInfoPreorderOffset[(offsetof(GameSpyInfo, m_preorderPlayers) == 0x162C) ? 1 : -1];

extern GameSpyInfo *TheGameSpyInfo;

#endif
