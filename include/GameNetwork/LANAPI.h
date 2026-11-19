// LANAPI.h: the LAN lobby's network interface (Zero Hour's LANAPIInterface and LANAPI by role and name, GPL-3.0-or-later; the
// virtual order, the parameter types and the layout are retail's: BFME II passes BfmeNetAddress where Zero Hour passes a bare IP
// and adds the packed-options, MP-setup and WOTR (War of the Ring) requests).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts (code RVAs; the table is VA 0x00C3E680, 65 slots):
//  - LANAPI derives from SubsystemInterface (slots 0..13, 0xC bytes), then: 14 setIsActive 0x00449A29, 15 RequestLocations
//    0x0044965C, 16 RequestGameJoin 0x00449E9A, 17 RequestGameJoinDirectConnect 0x00449FE8, 18 RequestGameLeave 0x0044A0CF,
//    19 RequestAccept 0x0044A32F, 20 RequestHasMap 0x0044A3BF, 21 RequestChat 0x0044A652, 22 RequestEnableMPSetupUI 0x0044A75D,
//    23 RequestGameStart 0x00449693, 24 RequestGameStartTimer 0x00449745, 25 RequestGameOptions 0x0044A808, 26
//    RequestGameOptionsPacked 0x004497EC, 27 RequestGameCreate 0x0044B55A, 28 RequestGameAnnounce 0x0044A214, 29 RequestSetName
//    0x0044B7A9, 30 RequestLobbyLeave 0x004498A4, 31 ResetGameStartTimer 0x0044973C, then the On callbacks in LANAPICallbacks.cpp
//    (32 OnGameList 0x00248E6F .. 48 OnNameChange 0x0024952E), 49 LookupGame 0x0044B915, 50 LookupGameByListOffset 0x004498F0,
//    51 the game list getter (folded +0x10 getter 0x001DB09D), 52/53 SetLocalIP (the overloads land in reverse declaration
//    order), 54 AmIHost 0x004499FF, 55 GetMyName 0x00449B8F, 56 the current game getter (folded +0x44 getter 0x005C4AF9),
//    57 fillInLANMessage 0x0044A966, 58 an empty stub (0x000B3FD0), 59 the lobby player getter (folded +0xC getter 0x001DB0A8),
//    60 0x00449B79, 61 0x00248E2F, 62 an empty one-argument stub (0x0047A69C), 63 LookupPlayer 0x0044998E, 64 the local
//    address getter (folded +0x48 getter 0x0053B8E5).
//  - SIZE 0x60 (the constructor 0x00449AB8 stores the table at +0 and builds the members below).
#pragma once
#ifndef BFME2_LANAPI_H
#define BFME2_LANAPI_H

#include "Common/GameType.h"
#include "Common/SubsystemInterface.h"
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"
#include "GameNetwork/BfmeNetAddress.h"

class LANGameInfo;
class LANPlayer;
class LivingWorldBattle;
class Transport;
struct LANMessage;

class LANAPIInterface : public SubsystemInterface
{
public:
	virtual ~LANAPIInterface() {}										// slot 0: stores the table 0x00C3E588, then SubsystemInterface's destructor 0x001B4E74
	enum ChatType
	{
		LANCHAT_NORMAL = 0,
		LANCHAT_TYPE1 = 1,
		LANCHAT_EMOTE = 2,
		LANCHAT_SYSTEM = 3
	};

	enum ReturnType
	{
		RET_OK = 0,
		RET_TIMEOUT,
		RET_GAME_FULL,
		RET_DUPLICATE_NAME,
		RET_CRC_MISMATCH,
		RET_SERIAL_DUPE,
		RET_GAME_STARTED,
		RET_GAME_EXISTS,
		RET_GAME_GONE,
		RET_BUSY,
		RET_UNKNOWN
	};

	UnicodeString getErrorStringFromReturnType(ReturnType ret);	// 0x00248EC3

	virtual void setIsActive(Bool isActive) = 0;										// slot 14
	virtual void RequestLocations() = 0;
	virtual void RequestGameJoin(LANGameInfo *game, const BfmeNetAddress &address) = 0;
	virtual void RequestGameJoinDirectConnect(void *data) = 0;
	virtual void RequestGameLeave() = 0;
	virtual void RequestAccept(Bool accept) = 0;
	virtual void RequestHasMap() = 0;													// slot 20
	virtual void RequestChat(UnicodeString message, ChatType format, Int unused) = 0;
	virtual void RequestEnableMPSetupUI(Bool enable) = 0;
	virtual void RequestGameStart(Bool force) = 0;
	virtual void RequestGameStartTimer(UnsignedInt seconds) = 0;
	virtual void RequestGameOptions(AsciiString gameOptions, Int isPublic, UnsignedInt ip) = 0;	// slot 25
	virtual void RequestGameOptionsPacked(Bool isPublic, BfmeNetAddress *address) = 0;
	virtual void RequestGameCreate(UnicodeString gameName, Bool isDirectConnect) = 0;
	virtual void RequestGameAnnounce() = 0;
	virtual void RequestSetName(UnicodeString newName) = 0;
	virtual void RequestLobbyLeave(Bool forced) = 0;									// slot 30
	virtual void ResetGameStartTimer() = 0;
	virtual void OnGameList(LANGameInfo *gameList) = 0;
	virtual void rva00248E87() = 0;													// slot 33 (the player list refresh; the map has no better name)
	virtual void OnGameJoin(ReturnType ret, LANGameInfo *theGame, LANMessage *message) = 0;
	virtual void OnPlayerJoin(Int slot, UnicodeString playerName) = 0;					// slot 35
	virtual void OnHostLeave() = 0;
	virtual void OnPlayerLeave(UnicodeString player) = 0;
	virtual void OnAccept(const BfmeNetAddress *address, Bool status) = 0;
	virtual void OnHasMap(const BfmeNetAddress *address, Bool status) = 0;
	virtual void OnChat(const UnicodeString &player, const BfmeNetAddress *address, const UnicodeString &message, ChatType format) = 0;	// slot 40
	virtual void rva00249B08(LivingWorldBattle *battle) = 0;							// slot 41 (the map has no better name)
	virtual void OnGameStart() = 0;
	virtual void OnWOTRGameStart() = 0;
	virtual void OnGameStartTimer(Int seconds) = 0;
	virtual void OnGameOptions(const BfmeNetAddress &sender, Int playerSlot, AsciiString options) = 0;	// slot 45
	virtual Bool OnGameOptionsPacked(const BfmeNetAddress &sender, Int playerSlot, const void *data, Int length) = 0;
	virtual void OnGameCreate(ReturnType ret) = 0;
	virtual void OnNameChange(UnsignedInt ip, UnicodeString newName) = 0;
	virtual LANGameInfo *LookupGame(UnicodeString gameName) = 0;
	virtual LANGameInfo *LookupGameByListOffset(Int offset) = 0;							// slot 50
	virtual LANGameInfo *GetGameList() = 0;											// slot 51
	virtual Bool SetLocalIP(UnsignedInt localIP) = 0;									// slot 53 (overloads land in reverse order)
	virtual void SetLocalIP(AsciiString localIP) = 0;									// slot 52
	virtual Int AmIHost() = 0;
	virtual UnicodeString GetMyName() = 0;												// slot 55
	virtual LANGameInfo *GetMyGame() = 0;												// slot 56
	virtual void fillInLANMessage(LANMessage *message) = 0;								// slot 57
private:
	virtual void slot58() = 0;															// slot 58 (an empty stub; no callable identity)
public:
	virtual LANPlayer *GetLobbyPlayers() = 0;											// slot 59
	virtual Bool rva00449B79() = 0;													// slot 60
};

enum
{
	LAN_GAME_NAME_LENGTH = 16,
	LAN_MAX_OPTIONS_LENGTH = 0x186,
	LAN_MAX_SERIAL_LENGTH = 0x17
};

// The datagram LANAPI sends and receives. Target facts: the type dword at +0, the player name at +4 (11 wide characters), the
// two-character user and host names at +0x1A and +0x1C, then the per-message payload at +0x1E. Offsets of the payload fields
// are in the comments (sendMessage 0x004495A2 sends the whole structure).
#pragma pack(push, 1)
struct LANMessage
{
	enum MessageType
	{
		MSG_REQUEST_LOCATIONS = 0,
		MSG_GAME_ANNOUNCE = 1,
		MSG_LOBBY_ANNOUNCE = 2,
		MSG_REQUEST_JOIN = 3,
		MSG_JOIN_ACCEPT = 4,
		MSG_JOIN_DENY = 5,
		MSG_REQUEST_GAME_LEAVE = 6,
		MSG_REQUEST_LOBBY_LEAVE = 7,
		MSG_REQUEST_HOST_LEAVE = 8,
		MSG_SET_ACCEPT = 9,
		MSG_MAP_AVAILABILITY = 10,
		MSG_CHAT = 11,
		MSG_ENABLE_MPSETUP_UI = 12,
		MSG_GAME_START = 13,
		MSG_WOTR_GAME_START = 14,
		MSG_GAME_START_TIMER = 15,
		MSG_GAME_OPTIONS = 16,
		MSG_INACTIVE = 17,
		MSG_REQUEST_GAME_INFO = 18,
		MSG_GAME_OPTIONS_PACKED = 19
	};

	Int LANMessageType;												// +0x00
	WideChar name[11];												// +0x04
	char userName[2];												// +0x1A
	char hostName[2];												// +0x1C
	union
	{
		struct
		{
			UnsignedInt gameIP;										// +0x1E
			UnsignedByte exeCRC[16];								// +0x22
			UnsignedInt iniCRC;										// +0x32
			UnsignedInt bfmeExtraCRC;								// +0x36
			char serial[LAN_MAX_SERIAL_LENGTH];						// +0x3A
		} GameToJoin;
		struct
		{
			UnsignedInt ip;										// +0x1E
			UnsignedShort port;									// +0x22
			WideChar playerName[11];							// +0x24
		} PlayerInfo;											// MSG_REQUEST_GAME_INFO
		struct
		{
			WideChar gameName[LAN_GAME_NAME_LENGTH + 1];			// +0x1E
			UnsignedInt gameIP;										// +0x40
			UnsignedShort gamePort;									// +0x44
			UnsignedInt playerIP;									// +0x46
			UnsignedShort playerPort;								// +0x4A
			LANAPIInterface::ReturnType reason;						// +0x4C
			Bool exeCRCMatch;										// +0x50
			Bool iniCRCMatch;										// +0x51
			Bool bfmeExtraCRCMatch;									// +0x52
		} JoinDeny;
		struct
		{
			WideChar gameName[LAN_GAME_NAME_LENGTH + 1];			// +0x1E
			UnsignedInt gameIP;										// +0x40
			UnsignedShort gamePort;									// +0x44
			UnsignedInt playerIP;									// +0x46
			UnsignedShort playerPort;								// +0x4A
			Int slotPosition;										// +0x4C
		} GameJoined;
		struct
		{
			WideChar gameName[LAN_GAME_NAME_LENGTH + 1];			// +0x1E
			UnsignedInt mapCRC;										// +0x40
			Bool hasMap;											// +0x44
		} MapStatus;
		struct
		{
			WideChar gameName[LAN_GAME_NAME_LENGTH + 1];			// +0x1E
			Bool inProgress;										// +0x40
			Bool isDirectConnect;									// +0x41
			char options[LAN_MAX_OPTIONS_LENGTH];					// +0x42
			UnsignedByte tail[16];									// +0x1C8
		} GameInfo;
		struct
		{
			char options[LAN_MAX_OPTIONS_LENGTH];					// +0x1E
		} GameOptions;
		struct
		{
			WideChar gameName[LAN_GAME_NAME_LENGTH + 1];			// +0x1E
			Bool enable;										// +0x40
		} EnableMPSetupUI;
		struct
		{
			WideChar gameName[LAN_GAME_NAME_LENGTH + 1];			// +0x1E
			Bool accepted;										// +0x40
		} GameAccept;
		struct
		{
			Int seconds;											// +0x1E
		} GameStartTimer;
		struct
		{
			WideChar gameName[LAN_GAME_NAME_LENGTH + 1];			// +0x1E
		} GameToLeave;
		struct
		{
			WideChar gameName[LAN_GAME_NAME_LENGTH + 1];			// +0x1E
			LANAPIInterface::ChatType chatType;						// +0x40
			WideChar message[101];									// +0x44
		} Chat;
	};
};
#pragma pack(pop)

void Rva0044802D(char *buffer, Int size);						// 0x0044802D (writes the game options string; the map has no better name)

class LANAPI : public LANAPIInterface
{
public:
	enum PendingAction
	{
		ACT_NONE = 0,
		ACT_JOIN = 1,
		ACT_JOINDIRECTCONNECT = 2,
		ACT_LEAVE = 3
	};

	LANAPI();																			// 0x00449AB8
	virtual ~LANAPI();																	// slot 0, 0x0044B992 (deleting 0x0044BA0E)

	virtual void init();																// slot 1, 0x00449BAA
	virtual void reset();																// slot 9, 0x0044B49B
	virtual void update();																// slot 10, 0x0044AAED
	virtual void setIsActive(Bool isActive);											// slot 14, 0x00449A29
	virtual void RequestLocations();													// 0x0044965C
	virtual void RequestGameJoin(LANGameInfo *game, const BfmeNetAddress &address);		// 0x00449E9A
	virtual void RequestGameJoinDirectConnect(void *data);								// 0x00449FE8
	virtual void RequestGameLeave();													// 0x0044A0CF
	virtual void RequestAccept(Bool accept);											// 0x0044A32F
	virtual void RequestHasMap();														// 0x0044A3BF
	virtual void RequestChat(UnicodeString message, ChatType format, Int unused);		// 0x0044A652
	virtual void RequestEnableMPSetupUI(Bool enable);									// 0x0044A75D
	virtual void RequestGameStart(Bool force);											// 0x00449693
	virtual void RequestGameStartTimer(UnsignedInt seconds);							// 0x00449745
	virtual void RequestGameOptions(AsciiString gameOptions, Int isPublic, UnsignedInt ip);	// 0x0044A808
	virtual void RequestGameOptionsPacked(Bool isPublic, BfmeNetAddress *address);		// 0x004497EC
	virtual void RequestGameCreate(UnicodeString gameName, Bool isDirectConnect);		// 0x0044B55A
	virtual void RequestGameAnnounce();													// 0x0044A214
	virtual void RequestSetName(UnicodeString newName);									// 0x0044B7A9
	virtual void RequestLobbyLeave(Bool forced);										// 0x004498A4
	virtual void ResetGameStartTimer();													// 0x0044973C
	virtual void OnGameList(LANGameInfo *gameList);										// 0x00248E6F
	virtual void rva00248E87();															// 0x00248E87
	virtual void OnGameJoin(ReturnType ret, LANGameInfo *theGame, LANMessage *message);	// 0x00249D46
	virtual void OnPlayerJoin(Int slot, UnicodeString playerName);						// 0x002493BC
	virtual void OnHostLeave();															// 0x00248E41
	virtual void OnPlayerLeave(UnicodeString player);									// 0x0024A0C7
	virtual void OnAccept(const BfmeNetAddress *address, Bool status);					// 0x00248D9D
	virtual void OnHasMap(const BfmeNetAddress *address, Bool status);					// 0x002498FA
	virtual void OnChat(const UnicodeString &player, const BfmeNetAddress *address, const UnicodeString &message, ChatType format);	// 0x0024A178
	virtual void rva00249B08(LivingWorldBattle *battle);								// 0x00249B08
	virtual void OnGameStart();															// 0x002495CC
	virtual void OnWOTRGameStart();														// 0x0024900D
	virtual void OnGameStartTimer(Int seconds);											// 0x00248F6E
	virtual void OnGameOptions(const BfmeNetAddress &sender, Int playerSlot, AsciiString options);	// 0x0024A348
	virtual Bool OnGameOptionsPacked(const BfmeNetAddress &sender, Int playerSlot, const void *data, Int length);	// 0x0024924D
	virtual void OnGameCreate(ReturnType ret);											// 0x00249430
	virtual void OnNameChange(UnsignedInt ip, UnicodeString newName);					// 0x0024952E
	virtual LANGameInfo *LookupGame(UnicodeString gameName);							// 0x0044B915
	virtual LANGameInfo *LookupGameByListOffset(Int offset);							// 0x004498F0
	virtual LANGameInfo *GetGameList() { return m_games; }								// folded +0x10 getter 0x001DB09D
	virtual Bool SetLocalIP(UnsignedInt localIP);										// 0x0044A9E6
	virtual void SetLocalIP(AsciiString localIP);										// 0x0044AA84
	virtual Int AmIHost();																// 0x004499FF
	virtual UnicodeString GetMyName() { return m_name; }								// 0x00449B8F
	virtual LANGameInfo *GetMyGame() { return m_currentGame; }							// folded +0x44 getter 0x005C4AF9
	virtual void fillInLANMessage(LANMessage *message);									// 0x0044A966
private:
	virtual void slot58();																// 0x000B3FD0 (empty)
public:
	virtual LANPlayer *GetLobbyPlayers() { return m_lobbyPlayers; }						// folded +0xC getter 0x001DB0A8
	virtual Bool rva00449B79();															// 0x00449B79
	virtual void rva00248E2F(Bool flag);												// 0x00248E2F
private:
	virtual void slot62(Int unused);													// 0x0047A69C (empty)
public:
	virtual LANPlayer *LookupPlayer(const BfmeNetAddress *address);						// 0x0044998E
	virtual BfmeNetAddress *GetLocalAddress() { return &m_localAddress; }				// folded +0x48 getter 0x0053B8E5

	void sendMessage(LANMessage *message, const BfmeNetAddress *to = 0);								// 0x004495A2 (the map has no better name)
	void handleSetAccept(LANMessage *message, const BfmeNetAddress *address);			// 0x005815EA
	void handleGameStartTimer(LANMessage *message, const BfmeNetAddress *address);		// 0x005816A3
	void handleGameOptions(LANMessage *message, const BfmeNetAddress *address);			// 0x00582138
	void handleGameOptionsPacked(LANMessage *message, const BfmeNetAddress *address);	// 0x005816E3 (the map holds it as rva005816E3)

protected:
	friend class AptLanLobby;	// AptLanLobby::MPOwnerValidatGameInfo 0x00444208 and GetValidSelectedGameInfo 0x00444165 call ValidateGameInfo
	void handleRequestLobbyLeave(LANMessage *message, const BfmeNetAddress *address);	// 0x005815AB
	void handleRequestLocations(LANMessage *message, const BfmeNetAddress *address);	// 0x00581795
	void handleGameAnnounce(LANMessage *message, const BfmeNetAddress *address);		// 0x00581984
	void handleLobbyAnnounce(LANMessage *message, const BfmeNetAddress *address);		// 0x00581C4E
	void handleRequestGameInfo(LANMessage *message, const BfmeNetAddress *address);		// 0x00581D2D
	void handleJoinDeny(LANMessage *message, const BfmeNetAddress *address);			// 0x00581E3E
	void handleHasMap(LANMessage *message, const BfmeNetAddress *address);				// 0x00581EA7
	void handleChat(LANMessage *message, const BfmeNetAddress *address);				// 0x00581F80
	void handleInActive(LANMessage *message, const BfmeNetAddress *address);			// 0x0058219D
	void handleJoinAccept(LANMessage *message, const BfmeNetAddress *address);			// 0x00582287
	void handleRequestGameLeave(LANMessage *message, const BfmeNetAddress *address, Bool flag);	// 0x00582498
	void handleRequestJoin(LANMessage *message, const BfmeNetAddress *address);			// 0x0058277C
	void removeGame(LANGameInfo *game);													// 0x00449913
	Bool ValidateGameInfo(LANGameInfo *game);											// 0x00449969
	void removePlayer(LANPlayer *player);												// 0x004499C7
	void addGame(LANGameInfo *game);													// 0x0044B2EA
	void addPlayer(LANPlayer *player);													// 0x0044B40C

	LANPlayer *m_lobbyPlayers;															// +0x0C
	LANGameInfo *m_games;																// +0x10
	UnicodeString m_name;																// +0x14
	AsciiString m_userName;																// +0x18
	AsciiString m_hostName;																// +0x1C
	UnsignedInt m_gameStartTime;														// +0x20
	UnsignedInt m_gameStartSeconds;														// +0x24
	UnsignedInt m_pendingAction;														// +0x28
	UnsignedInt m_expiration;															// +0x2C
	UnsignedInt m_actionTimeout;														// +0x30
	BfmeNetAddress m_directConnectAddress;												// +0x34
	UnsignedInt m_lastResendTime;														// +0x3C
	Bool m_isInLANMenu;																	// +0x40
	Bool m_inLobby;																		// +0x41
	unsigned char m_pad42[2];															// +0x42
	LANGameInfo *m_currentGame;															// +0x44
	BfmeNetAddress m_localAddress;														// +0x48
	Transport *m_transport;																// +0x50
	UnsignedInt m_broadcastAddress;														// +0x54
	UnsignedInt m_unk0x58;																// +0x58
	Bool m_isActive;																	// +0x5C
	Bool m_unk0x5D;													// +0x5D (0 after the constructor)
	unsigned char m_pad5E[2];											// +0x5E
};
// SIZE 0x60
typedef char LANAPISize[(sizeof(LANAPI) == 0x60) ? 1 : -1];

UnsignedInt ResolveIP(AsciiString host);							// 0x00581339 (the map has no better name)

extern LANAPI *TheLAN;																	// VA 0x00DFE958

#endif
