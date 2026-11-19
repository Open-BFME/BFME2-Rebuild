// PeerDefs.h: the records GameSpy's main-thread info object keeps (Zero Hour's PeerDefs.h by role, GPL-3.0-or-later: names
// and member order are theirs where a retail body agrees; offsets and sizes are retail's).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts (code RVAs):
//  - GameSpyGroupRoom is 0x20 bytes: addGroupRoom 0x00386B7B takes it by value (ret 0x20); its copy constructor 0x00382444 copies
//    the name at +0, the translated name at +4 and six dwords +8..+0x1C: Zero Hour's seven members in their order, plus one
//    dword (+0x1C) after the five counters.
//  - PlayerInfo is 0x34 bytes: updatePlayerInfo 0x00386EF8 takes it by value (ret 0x38 with the old nick) and keys the player
//    map with the string at +0 (PlayerInfo::isIgnored 0x00382841 copies the string at +0 too); the map node holds it at +0x14
//    (key at +0x10); the player finder 0x00382CCE compares the string at +4, isIgnored reads the profile id at +0x14,
//    updatePlayerInfo reads the preorder flag at +0x30. Zero Hour's nine members in their order, plus four dwords the bodies do
//    not name (one before the profile id, three between the side and the preorder flag).
//  - BuddyMessage is 0x18 bytes: the list header's node allocation (0x20 at 0x0038295A) is the two links plus one record.
#pragma once
#ifndef BFME2_PEERDEFS_H
#define BFME2_PEERDEFS_H

#include <map>
#include <set>
#include <list>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"
#include "GameNetwork/GameSpy/PersistentStorageDefs.h"

class GameSpyStagingRoom;

typedef _STL::set<AsciiString> IgnoreList;
typedef _STL::map<Int, AsciiString> SavedIgnoreMap;

class BuddyInfo
{
public:
	Int m_id;
	AsciiString m_name;
	AsciiString m_email;
	AsciiString m_countryCode;
	Int m_status;
	UnicodeString m_statusString;
	UnicodeString m_locationString;
};
typedef _STL::map<Int, BuddyInfo> BuddyInfoMap;

class BuddyMessage
{
public:
	UnsignedInt m_timestamp;
	Int m_senderID;
	AsciiString m_senderNick;
	Int m_recipientID;
	AsciiString m_recipientNick;
	UnicodeString m_message;
};
typedef _STL::list<BuddyMessage> BuddyMessageList;
typedef char BuddyMessageSize[(sizeof(BuddyMessage) == 0x18) ? 1 : -1];

class GameSpyGroupRoom
{
public:
	AsciiString m_name;						// +0x00 (copy constructor 0x00382444 copies the string at +0)
	UnicodeString m_translatedName;			// +0x04 (the same constructor: lea ecx,[esi+4]; call 0x00437050)
	Int m_groupID;							// +0x08
	Int m_numWaiting;						// +0x0C
	Int m_maxWaiting;						// +0x10
	Int m_numGames;							// +0x14
	Int m_numPlaying;						// +0x18
	Int m_roomType;							// +0x1C (not in Zero Hour's record; joinBestGroupRoom 0x00385678 compares it with its room-type argument, AptOnlineCustomMatch::PopulateLobbyComboBox 0x0059FD3A tests it == 1)
};
typedef char GameSpyGroupRoomSize[(sizeof(GameSpyGroupRoom) == 0x20) ? 1 : -1];
typedef _STL::map<Int, GameSpyGroupRoom> GroupRoomMap;

typedef _STL::map<Int, GameSpyStagingRoom *> StagingRoomMap;

struct SelPair;									// the selection record sendChat takes a pointer to (the map's spelling of slot 0x100's third parameter; not recovered)
class PlayerInfo
{
public:
	Bool isIgnored();						// 0x00382841
	AsciiString m_name;						// +0x00 (the player map's key; PlayerInfo::isIgnored 0x00382841)
	AsciiString m_locale;					// +0x04 (the player finder 0x00382CCE compares it)
	Int m_wins;								// +0x08
	Int m_losses;							// +0x0C
	Int m_unk0x10;							// +0x10
	Int m_profileID;						// +0x14 (isIgnored 0x00382841, updatePlayerInfo 0x00386EF8)
	Int m_flags;							// +0x18
	Int m_rankPoints;						// +0x1C
	Int m_side;								// +0x20
	Int m_unk0x24[3];						// +0x24
	Int m_preorder;							// +0x30 (updatePlayerInfo marks the profile as a preorder when set)
};
typedef char PlayerInfoSize[(sizeof(PlayerInfo) == 0x34) ? 1 : -1];

struct AsciiComparator
{
	bool operator()(AsciiString s1, AsciiString s2) const;
};
typedef _STL::map<AsciiString, PlayerInfo, AsciiComparator> PlayerInfoMap;

// The interface the lobby code reaches the info object through (Zero Hour's GameSpyInfoInterface): GameSpyInfo's constructor
// runs an unwindable base before its first member (every EH state of the constructor 0x00385FE1 is one higher than the members
// alone would give), so retail's class has a base with a destructor; the table is GameSpyInfo's (slot numbers as there).
class GameSpyInfoInterface
{
public:
	virtual ~GameSpyInfoInterface() {}										// slot 0x00
	static GameSpyInfoInterface *createNewGameSpyInfoInterface();			// 0x003864CC: the new GameSpyInfo
	virtual void reset() {}														// slot 0x04: the interface's table holds the shared empty body 0x000B3FD0 here
	virtual void clearGroupRoomList() = 0;											// slot 0x08, 0x003862CA
	virtual GroupRoomMap *getGroupRoomList() = 0;									// slot 0x0C, 0x0036CBE7
	virtual void addGroupRoom(GameSpyGroupRoom room) = 0;							// slot 0x10, 0x00386B7B
	virtual Bool gotGroupRoomList() = 0;											// slot 0x14, 0x003860EF
	virtual void joinGroupRoom(Int groupID) = 0;									// slot 0x18, 0x003853DA
	virtual void leaveGroupRoom() = 0;												// slot 0x1C, 0x0038544D
	virtual void vslot20() = 0;														// slot 0x20, 0x003854C5
	virtual void joinBestGroupRoom(Int groupID) = 0;								// slot 0x24, 0x00385678
	virtual void joinPreferredGroupRoom(Bool flag, Int groupID) = 0;				// slot 0x28, 0x0038553D
	virtual void setCurrentGroupRoom(Int groupID) = 0;								// slot 0x2C, 0x003862DB
	virtual Int getCurrentGroupRoom() = 0;											// slot 0x30, 0x0008BB81
	virtual void vslot34(AsciiString value) = 0;									// slot 0x34, 0x00386139
	virtual AsciiString vslot38() = 0;												// slot 0x38, 0x0038616D
	virtual void vslot3C(Int value) = 0;											// slot 0x3C, 0x00238D83
	virtual Int vslot40() = 0;														// slot 0x40, 0x002004A1
	virtual void vslot44(Int value) = 0;											// slot 0x44, 0x003674FE
	virtual Int vslot48() = 0;														// slot 0x48, 0x00367508
	virtual void updatePlayerInfo(PlayerInfo info, AsciiString oldNick) = 0;		// slot 0x4C, 0x00386EF8
	virtual void playerLeftGroupRoom(AsciiString nick) = 0;							// slot 0x50, 0x00384660
	virtual PlayerInfoMap *getPlayerInfoMap() = 0;									// slot 0x54, 0x003EFDD7
	virtual PlayerInfo *findPlayerInfoByLocale(const char *locale) = 0;				// slot 0x58, 0x00382CCE (compares the locale string)
	virtual PlayerInfo *findPlayerInfoByProfileID(Int profileID) = 0;				// slot 0x5C, 0x00382D0A
	virtual BuddyInfoMap *getBuddyMap() = 0;										// slot 0x60, 0x002B236A
	virtual BuddyInfoMap *getBuddyRequestMap() = 0;									// slot 0x64, 0x0049CB86
	virtual BuddyMessageList *getBuddyMessages() = 0;								// slot 0x68, 0x00042287
	virtual Bool isBuddy(Int profileID) = 0;										// slot 0x6C, 0x003835D7
	virtual void setLocalName(AsciiString name) = 0;								// slot 0x70, 0x00386188
	virtual AsciiString getLocalName() = 0;											// slot 0x74, 0x003861BC
	virtual void setLocalProfileID(Int profileID) = 0;								// slot 0x78, 0x002D94FE
	virtual Int getLocalProfileID() = 0;											// slot 0x7C, 0x000421C4
	virtual AsciiString getLocalEmail() = 0;										// slot 0x80, 0x003861D7
	virtual void setLocalEmail(AsciiString email) = 0;								// slot 0x84, 0x003861F2
	virtual AsciiString getLocalPassword() = 0;										// slot 0x88, 0x00386226
	virtual void setLocalPassword(AsciiString password) = 0;						// slot 0x8C, 0x00386241
	virtual void setLocalBaseName(AsciiString name) = 0;							// slot 0x90, 0x00386275
	virtual AsciiString getLocalBaseName() = 0;										// slot 0x94, 0x003862AC
	virtual void setCachedLocalPlayerStats(PSPlayerStats stats) = 0;				// slot 0x98, 0x00387479
	virtual PSPlayerStats getCachedLocalPlayerStats() = 0;							// slot 0x9C, 0x003862ED
	virtual void clearStagingRoomList() = 0;										// slot 0xA0, 0x00382E2F
	virtual StagingRoomMap *getStagingRoomList() = 0;								// slot 0xA4, 0x00091A25
	virtual GameSpyStagingRoom *findStagingRoomByID(Int id) = 0;					// slot 0xA8, 0x00383846
	virtual void addStagingRoom(GameSpyStagingRoom room) = 0;						// slot 0xAC, 0x003857EB
	virtual void updateStagingRoom(GameSpyStagingRoom room) = 0;					// slot 0xB0, 0x00383784
	virtual void removeStagingRoom(GameSpyStagingRoom room) = 0;					// slot 0xB4, 0x003837D3
	virtual Bool hasStagingRoomListChanged() = 0;									// slot 0xB8, 0x00381DDA
	virtual void leaveStagingRoom() = 0;											// slot 0xBC, 0x003858DF
	virtual void markAsStagingRoomHost() = 0;										// slot 0xC0, 0x00382623
	virtual void markAsStagingRoomJoiner(Int game) = 0;								// slot 0xC4, 0x00383865
	virtual void sawFullGameList() = 0;												// slot 0xC8, 0x003860FA
	virtual Bool amIHost() = 0;														// slot 0xCC, 0x00381DBD
	virtual Bool vslotD0() = 0;														// slot 0xD0, 0x00381DC4
	virtual GameSpyStagingRoom *getCurrentStagingRoom() = 0;						// slot 0xD4, 0x003835A0
	virtual Bool validateStagingRoom(GameSpyStagingRoom *room) = 0;					// slot 0xD8, 0x00383207
	virtual void setGameOptions() = 0;												// slot 0xDC, 0x00386501
	virtual Int getCurrentStagingRoomID() = 0;										// slot 0xE0, 0x003860F3
	virtual void setDisallowAsianText(Bool value) = 0;								// slot 0xE4, 0x00381E05
	virtual void setDisallowNonAsianText(Bool value) = 0;							// slot 0xE8, 0x00381E12
	virtual Bool getDisallowAsianText() = 0;										// slot 0xEC, 0x00381E1F
	virtual Bool getDisallowNonAsianText() = 0;										// slot 0xF0, 0x00381E26
	virtual void addText(UnicodeString text, Int color) = 0;														// slot 0xF4, 0x001EF552 (retail callers push a UnicodeString by value and the chat colour dword: handleSlashCommands 0x0059FED1; Zero Hour's addText)
	virtual void vslotF8() = 0;														// slot 0xF8, 0x001EF90E
	virtual void addChat(PlayerInfo info, UnicodeString message, Bool isPublic, Bool isAction) = 0;	// slot 0xFC, 0x001EF9A0
	virtual Bool sendChat(UnicodeString text, Bool isAction, const SelPair *selection) = 0;													// slot 0x100, 0x001EFD9C (callers push a UnicodeString by value, 1 and 0: handleSlashCommands 0x0059FF2B; Zero Hour's sendChat)
	virtual Bool vslot104(UnicodeString text) = 0;													// slot 0x104, 0x001EFCF7 (one UnicodeString by value; name not evidenced)
	virtual void setMOTD(const AsciiString &motd) = 0;								// slot 0x108, 0x0038200D
	virtual const AsciiString &getMOTD() = 0;										// slot 0x10C, 0x001DBA69
	virtual void setConfig(const AsciiString &config) = 0;							// slot 0x110, 0x00382015
	virtual const AsciiString &getConfig() = 0;										// slot 0x114, 0x003F712F
	virtual void setPingString(const AsciiString &ping) = 0;						// slot 0x118, 0x0030D420
	virtual const AsciiString &getPingString() = 0;									// slot 0x11C, 0x005C4AD9
	virtual Int getPingValue(const AsciiString &otherPing) = 0;						// slot 0x120, 0x0038277F
	virtual void addToBuddyList(Int profileID) = 0;									// slot 0x124, 0x00382767
	virtual void removeBuddy(Int profileID) = 0;									// slot 0x128, 0x003871F6
	virtual void addToSavedIgnoreList(Int profileID, AsciiString nick) = 0;			// slot 0x12C, 0x00385E2A
	virtual void removeFromSavedIgnoreList(Int profileID) = 0;						// slot 0x130, 0x00385B1C
	virtual Bool isSavedIgnored(Int profileID) = 0;									// slot 0x134, 0x003839B9
	virtual SavedIgnoreMap *getSavedIgnoreMap() = 0;								// slot 0x138, 0x00381DFE
	virtual void loadSavedIgnoreList() = 0;											// slot 0x13C, 0x00385B78
	virtual IgnoreList returnIgnoreList() = 0;										// slot 0x140, 0x00385AFE
	virtual void addToIgnoreList(AsciiString nick) = 0;								// slot 0x144, 0x003846A8
	virtual void removeFromIgnoreList(AsciiString nick) = 0;						// slot 0x148, 0x003871BF
	virtual Bool isIgnored(AsciiString nick) = 0;									// slot 0x14C, 0x003846E5
	virtual void setLocalIPs(UnsignedInt internalIP, UnsignedInt externalIP) = 0;	// slot 0x150, 0x00381D9E
	virtual UnsignedInt getInternalIP() = 0;										// slot 0x154, 0x0043813D
	virtual UnsignedShort vslot158() = 0;											// slot 0x158, 0x003860FF (returns the constant 0x1F98)
	virtual UnsignedInt getExternalIP() = 0;										// slot 0x15C, 0x0028A936
	virtual Bool isDisconnectedAfterGameStart(Int *reason) const = 0;				// slot 0x160, 0x00386104
	virtual void markAsDisconnectedAfterGameStart(Int reason) = 0;					// slot 0x164, 0x00386117
	virtual Bool didPlayerPreorder(Int profileID) const = 0;						// slot 0x168, 0x00383580
	virtual void markPlayerAsPreorder(Int profileID) = 0;							// slot 0x16C, 0x00383E80
	virtual void setMaxMessagesPerUpdate(Int num) = 0;								// slot 0x170, 0x00381E2D
	virtual Int getMaxMessagesPerUpdate() = 0;										// slot 0x174, 0x00381E3A
	virtual Int getAdditionalDisconnects() = 0;										// slot 0x178, 0x000D43D0 (the shared `xor eax,eax` body: retail returns 0, as Zero Hour's does)
	virtual void clearAdditionalDisconnects() = 0;									// slot 0x17C, 0x00381DB5
	virtual void readAdditionalDisconnects() = 0;									// slot 0x180, 0x003853C6
	virtual UnsignedByte get() const = 0;											// slot 0x184, 0x00386125 (the byte at +0x163C; the ledger's name)
	virtual void set(UnsignedByte value) = 0;										// slot 0x188, 0x0038612C (the ledger's name)
};

#endif
