// PeerThread.h: the request record the lobby code hands the GameSpy peer thread, and the queue it goes through (Zero Hour's
// PeerRequest and GameSpyPeerMessageQueueInterface by role and member order, GPL-3.0-or-later; offsets and sizes are retail's).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts (code RVAs):
//  - PeerRequest is 0x1EC bytes (every GameSpyInfo row that sends one reserves 0x1EC and builds it with 0x001EF661, destroys it
//    with 0x001EF723; the map holds the pair as BfmeOpaqueOwnedRecord492). The constructor builds, in order: strings at +4, the
//    wide string at +0x10, strings at +0x1C, +0x28, +0x34, +0x40, +0x4C, +0x58, +0x64, an array of eight strings at +0x70 (0xC
//    bytes each), a string at +0xF8 and a bit vector at +0x104 (0x14 bytes); the type word at +0 and the union from +0x118 are
//    not touched by it. The destructor frees the +0xF8 string and the bit vector first. The union starts at +0x118: the group
//    room id lives there (joinGroupRoom 0x003853DA stores it at +0x118).
//  - Type values read from the senders: joinGroupRoom 4, leaveGroupRoom 5, the slot-0x20 rejoin 6, leaveStagingRoom 12. They
//    are Zero Hour's order up to 5; from 6 on the retail list has one more entry than Zero Hour's (leaveStagingRoom is 12 where
//    Zero Hour's is 11), so only the evidenced values are named.
//  - The NAT negotiator's messages are type 13 (the player-to-player message; the old decomp's "13" with the id string "NAT", the
//    receiving nick at +4, the text at +0x40 and the staging-room flag byte at +0x118).
//  - The queue is GameSpyInfo's `TheGameSpyPeerMessageQueue` (VA 0x00E02340); its table: 0 destructor, 1 startThread, 2 endThread,
//    3 isThreadRunning, 4 isConnected, 5 isConnecting, 6 addRequest (+0x18), 7 getRequest, 8 addResponse, 9 getResponse, 10
//    getSerialAuthResult (the slots are in the Draft header GameSpyPeerMessageQueue.h).
#pragma once
#ifndef BFME2_PEERTHREAD_H
#define BFME2_PEERTHREAD_H

#include <stddef.h>
#include <string>
#include <vector>
#include <map>
#include <queue>
#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "WWLib/thread.h"
#include "WWLib/mutex.h"


class PeerRequest
{
public:
	PeerRequest();								// 0x001EF661
	~PeerRequest();								// 0x001EF723

	enum PeerRequestType
	{
		PEERREQUEST_JOINGROUPROOM = 4,
		PEERREQUEST_LEAVEGROUPROOM = 5,
		PEERREQUEST_UNKNOWN6 = 6,
		PEERREQUEST_LEAVESTAGINGROOM = 12,
		PEERREQUEST_UTMPLAYER = 13,
		PEERREQUEST_STOPQUICKMATCH = 18		// playerLeftCallback 0x0038C788 stores 0x12 when the matchbot leaves
	};

	Int peerRequestType;						// +0x00
	_STL::string nick;							// +0x04
	_STL::wstring text;							// +0x10
	_STL::string password;						// +0x1C
	_STL::string email;							// +0x28
	_STL::string id;							// +0x34
	_STL::string options;						// +0x40
	_STL::string ladderIP;						// +0x4C
	_STL::string hostPingStr;					// +0x58
	_STL::string gameOptsMapName;				// +0x64
	_STL::string gameOptsPlayerNames[8];		// +0x70
	unsigned char m_unk0xD0[0x28];				// +0xD0
	_STL::string m_unk0xF8;						// +0xF8
	_STL::vector<bool> qmMaps;					// +0x104
	union										// +0x118
	{
		struct
		{
			Int id;
		} groupRoom;
		struct
		{
			Bool isStagingRoom;
		} UTM;
		unsigned char m_unionBytes[0xD4];
	};
};
typedef char PeerRequestSize[(sizeof(PeerRequest) == 0x1EC) ? 1 : -1];

// What the peer thread hands back to the lobby (Zero Hour's PeerResponse by role and member order, GPL-3.0-or-later; offsets are retail's).
// Target facts (code RVAs): the constructor 0x00389F77 builds, in order, the strings at +4/+0x10/+0x1C, the wide string at +0x28,
// the strings at +0x34/+0x40, the wide string at +0x4C, the strings at +0x58/+0x64/+0x70/+0x7C, the eight strings at +0x88, the
// strings at +0xE8/+0xF4 and the vector at +0x100 (0x00611E58); the type word at +0 and the 0x23C-byte payload from +0x10C are not
// touched by it. The destructor 0x0038A063 tears the same members down; the copy constructor is 0x00389C14, the assignment
// 0x0038B383, and the whole record is 0x348 bytes.
class PeerResponse
{
public:
	PeerResponse();								// 0x00389F77
	~PeerResponse();							// 0x0038A063

	enum PeerResponseType
	{
		PEERRESPONSE_LOGIN,
		PEERRESPONSE_DISCONNECT,
		PEERRESPONSE_MESSAGE,
		PEERRESPONSE_GROUPROOM,
		PEERRESPONSE_STAGINGROOM,
		PEERRESPONSE_STAGINGROOMLISTCOMPLETE,
		PEERRESPONSE_STAGINGROOMPLAYERINFO,
		PEERRESPONSE_JOINGROUPROOM,
		PEERRESPONSE_CREATESTAGINGROOM,
		PEERRESPONSE_JOINSTAGINGROOM,
		PEERRESPONSE_PLAYERJOIN,
		PEERRESPONSE_PLAYERLEFT,
		PEERRESPONSE_PLAYERCHANGEDNICK,
		PEERRESPONSE_PLAYERINFO,
		PEERRESPONSE_PLAYERCHANGEDFLAGS,
		PEERRESPONSE_ROOMUTM,
		PEERRESPONSE_PLAYERUTM,
		PEERRESPONSE_QUICKMATCHSTATUS,
		PEERRESPONSE_GAMESTART,
		PEERRESPONSE_FAILEDTOHOST,
		PEERRESPONSE_ROOMNOTICE,				// BFME 2: a room NoticeMessage's text alone (roomMessageCallback 0x0038BE17 stores 0x14)
		PEERRESPONSE_GROUPROOMTOTALS,			// BFME 2: the lobby's game and player totals (countGroupRoomsCallback 0x0038A288 stores 0x15)
		PEERRESPONSE_PLAYERKEYCHANGED			// BFME 2: a player's room key changed (roomKeyChangedCallback 0x0038E202 stores 0x16)
	};

	Int peerResponseType;						// +0x00
	_STL::string groupRoomName;					// +0x04
	_STL::string nick;							// +0x10
	_STL::string oldNick;						// +0x1C
	_STL::wstring text;							// +0x28
	_STL::string locale;						// +0x34
	_STL::string stagingServerGameOptions;		// +0x40
	_STL::wstring stagingServerName;			// +0x4C
	_STL::string stagingServerPingString;		// +0x58
	_STL::string stagingServerLadderIP;			// +0x64
	_STL::string stagingRoomMapName;			// +0x70
	_STL::string m_unk0x7C;						// +0x7C
	_STL::string stagingRoomPlayerNames[8];		// +0x88
	_STL::string command;						// +0xE8
	_STL::string commandOptions;				// +0xF4
	_STL::vector<AsciiString> m_unk0x100;		// +0x100
	// The per-type payload from +0x10C (0x23C bytes). Field order and widths are the old decomp's views of the callbacks that fill
	// them (the connect, group-room, join, player, listing-games and quickmatch callbacks 0x0038B9F8..0x0038CBBA), in Zero Hour's names.
	union
	{
		UnsignedInt m_payload[143];

		struct
		{
			Int reason;							// DisconnectReason
		} discon;

		struct
		{
			Int id;
			Int numWaiting;
			Int maxWaiting;
			Int numGames;
			Int numPlaying;
			Int roomType;						// the server's "roomType" key (listGroupRoomsCallback 0x0038B91F stores it at +0x120)
		} groupRoom;

		struct									// PEERRESPONSE_GROUPROOMTOTALS (countGroupRoomsCallback 0x0038A288)
		{
			Int unused[6];
			Int numGames;
			Int numPlayers;
		} groupRoomTotals;

		struct
		{
			Int id;
			Bool ok;
		} joinGroupRoom;

		struct
		{
			Int result;
		} createStagingRoom;

		struct
		{
			Int id;
			Bool ok;
			Bool isHostPresent;
			Int result;
		} joinStagingRoom;

		struct
		{
			Bool isPrivate;
			Bool isAction;
			Int profileID;
		} message;

		struct
		{
			Int profileID;
			Int wins;
			Int losses;
			Int roomType;						// RoomType
			Int flags;
			UnsignedInt IP;
			Int rankPoints;
			Int side;
			Int preorder;
			UnsignedInt internalIP;
			UnsignedInt externalIP;
			char loginTextA[256];				// connectCallback 0x0038B9F8 copies two 256-byte global buffers to +0x138 and +0x238
			char loginTextB[256];
			Bool loginComplete;
			Int rank1v1;						// +0x33C (the b_rank1v1 stat key; getPlayerInfo 0x0038C48B fills the three through its last references)
			Int rank2v2;
			Int bSide;
		} player;

		struct									// listingGamesCallback 0x0038CBBA
		{
			Int id;
			Int action;
			Bool isStaging;
			Bool requiresPassword;
			Bool allowObservers;
			Bool useStats;
			UnsignedInt version;
			UnsignedInt exeCRC[4];
			UnsignedInt iniCRC;
			UnsignedInt cmdCRC;
			unsigned char ladderHash[16];
			UnsignedShort ladderPort;
			Int wins[8];
			Int losses[8];
			Int profileID[8];
			Int faction[8];
			Int color[8];
			Int handicap[8];
			Int numPlayers;
			Int numObservers;
			Int maxPlayers;
			Int percentComplete;
			Int gameType;
			Int ladPortValues[10];
			Int scenario;
		} stagingRoom;

		struct									// handleQMMatch 0x0038B6C1 / quickmatch status
		{
			Int status;							// QMStatus
			Int poolSize;
			Int mapIdx;
			Int seed;
			UnsignedInt IP[8];
			UnsignedShort port[8];
			Int side[8];
			Int color[8];
			Int nat[8];
			Int extra[8];
		} qmStatus;
	};
};
typedef char PeerResponseSize[(sizeof(PeerResponse) == 0x348) ? 1 : -1];

// What the peer thread's quickmatch state machine is doing (Zero Hour's QMStatus; the thread stores it at +0x294).
enum QMStatus
{
	QM_IDLE,
	QM_JOININGQMCHANNEL,
	QM_LOOKINGFORBOT,
	QM_SENTINFO,
	QM_WORKING,
	QM_POOLSIZE,
	QM_WIDENINGSEARCH,
	QM_MATCHED,
	QM_INCHANNEL,
	QM_NEGOTIATINGFIREWALLS,
	QM_STARTINGGAME,
	QM_COULDNOTFINDBOT,
	QM_COULDNOTFINDCHANNEL,
	QM_COULDNOTNEGOTIATEFIREWALLS,
	QM_STOPPED
};

// Why the lobby connection was dropped (Zero Hour's DisconnectReason order; the serial reasons are where BFME 2 put them).
enum DisconnectReason
{
	DISCONNECT_NICKTAKEN = 1,
	DISCONNECT_BADNICK,
	DISCONNECT_LOSTCON,
	DISCONNECT_COULDNOTCONNECT,
	DISCONNECT_GP_LOGIN_TIMEOUT,
	DISCONNECT_GP_LOGIN_BAD_NICK,
	DISCONNECT_GP_LOGIN_BAD_EMAIL,
	DISCONNECT_GP_LOGIN_BAD_PASSWORD,
	DISCONNECT_GP_LOGIN_BAD_PROFILE,
	DISCONNECT_GP_LOGIN_PROFILE_DELETED,
	DISCONNECT_GP_LOGIN_CONNECTION_FAILED,
	DISCONNECT_GP_LOGIN_SERVER_AUTH_FAILED,
	DISCONNECT_SERIAL_INVALID,
	DISCONNECT_SERIAL_NOT_PRESENT,
	DISCONNECT_SERIAL_BANNED,
	DISCONNECT_GP_NEWUSER_BAD_NICK,
	DISCONNECT_GP_NEWUSER_BAD_PASSWORD,
	DISCONNECT_GP_NEWPROFILE_BAD_NICK,
	DISCONNECT_GP_NEWPROFILE_BAD_OLD_NICK,
	DISCONNECT_MAX
};

enum SerialAuthResult
{
	SERIAL_NONEXISTENT,
	SERIAL_AUTHFAILED,
	SERIAL_BANNED,
	SERIAL_OK
};

struct _SBServer;

// The rooms a GameSpy peer is in: the SDK's RoomType (peer.h declares the same anonymous enum under this typedef name; a file that
// includes peer.h first has it already).
#ifndef _PEER_H_
typedef enum
{
	TitleRoom,
	GroupRoom,
	StagingRoom,
	NumRooms
} RoomType;
#endif

// The GameSpy peer thread (Zero Hour's PeerThreadClass by role and member order, GPL-3.0-or-later; offsets are retail's).
// Target facts (code RVAs): the constructor 0x0038AAEB runs ThreadClass(0) and stores table 0x00C1989C (three slots: deleting
// destructor 0x0038ADC6, ThreadClass::Execute 0x006104B0, Thread_Function 0x0038EDD7); GameSpyPeerMessageQueue::startThread 0x0038B1C0
// allocates 0x4B0 bytes for it. The constructor builds the strings at +0x54/+0x60/+0x6C/+0x78/+0xB4/+0xC4/+0x160/+0x16C, the eight
// at +0xD0, the wide string at +0x284 and the one at +0x490, the two string-keyed trees at +0x98/+0xA4 and the server tree at
// +0x278, and the owned 0x1EC-byte request at +0x298; the destructor 0x0038ADE2 tears down the same spans. The six eight-dword
// player arrays at +0x17C..+0x23C are read by the six getters 0x003888A8..0x0038892F.
class PeerThreadClass : public ThreadClass
{
public:
	PeerThreadClass(MutexClass *queueMutex);							// 0x0038AAEB
	virtual ~PeerThreadClass();											// slot 0, deleting 0x0038ADC6, body 0x0038ADE2
	virtual void Thread_Function();										// slot 2, 0x0038EDD7

	Int getPlayerWins(Int index);										// 0x003888A8
	Int getPlayerLosses(Int index);										// 0x003888C3
	Int getPlayerProfileID(Int index);									// 0x003888DE
	Int getPlayerFaction(Int index);									// 0x003888F9
	Int getPlayerColor(Int index);										// 0x00388914
	Int getPlayerHandicap(Int index);									// 0x0038892F

	void nickErrorCallback(void *peer, Int type, const char *nick);	// 0x0038BBF0 (retries the login with a numbered nick, or reports the failure)
	void stopHostingAlready(void *peer);								// 0x0038D526
	_SBServer *findServerByID(Int id);									// 0x003896BD
	Int addServerToMap(_SBServer *server);								// 0x00389F4A
	Int removeServerFromMap(_SBServer *server);							// 0x0038A6DF
	Int lookupStatForPlayer(RoomType roomType, const char *nick, const char *key);	// 0x0038A63C
	void clearPlayerStats(RoomType roomType);							// 0x0038A6BF
	void trackStatsForPlayer(RoomType roomType, const char *nick, const char *key, const char *value);	// 0x0038E0C4
	_STL::string packStatKey(const char *nick, const char *key);		// 0x0038A5D6

	void roomJoined(Bool joined);										// 0x00388951 (Zero Hour's name for the setter of the byte at +0x484)
	void setQMGroupRoom(Int groupID);									// 0x0038895E (+0x488)
	void sawEndOfEnumPlayers();											// 0x0038896B (sets the byte at +0x48C)
	QMStatus getQMStatus();												// 0x00388973 (+0x294)
	_STL::string getQMBotName();										// inline in retail (returns the matchbot name at +0x490)
	void markAsDisconnected() { m_isConnecting = m_isConnected = false; }
	Int getQMGroupRoom();												// 0x0038897A (+0x488)

	Bool m_isConnecting;												// +0x50 (GameSpyPeerMessageQueue::isConnecting 0x003889F6 reads it)
	Bool m_isConnected;													// +0x51
	unsigned char m_pad0x52[2];											// +0x52
	_STL::string m_loginName;											// +0x54
	_STL::string m_originalName;										// +0x60
	_STL::string m_password;											// +0x6C
	_STL::string m_email;												// +0x78
	Int m_profileID;													// +0x84
	Int m_groupRoomID;													// +0x88
	Bool m_unk0x8C;														// +0x8C (0x00388988 sets it)
	unsigned char m_pad0x8D[3];											// +0x8D
	Int m_unk0x90;														// +0x90
	Bool m_unk0x94;														// +0x94
	unsigned char m_pad0x95[3];											// +0x95
	_STL::map<_STL::string, Int> m_groupRoomStats;						// +0x98
	_STL::map<_STL::string, Int> m_stagingRoomStats;					// +0xA4
	Bool m_isHosting;													// +0xB0
	Bool m_hasPassword;													// +0xB1
	unsigned char m_pad0xB2[2];											// +0xB2
	_STL::string m_mapName;												// +0xB4
	Int m_unk0xC0;														// +0xC0
	_STL::string m_unk0xC4;												// +0xC4
	_STL::string m_playerNames[8];										// +0xD0
	UnsignedInt m_unk0x130[11];											// +0x130..+0x15B (the CRCs, version and counters; not typed yet)
	Bool m_unk0x15C;													// +0x15C
	unsigned char m_pad0x15D[3];										// +0x15D
	_STL::string m_pingStr;												// +0x160
	_STL::string m_ladderIP;											// +0x16C
	UnsignedShort m_ladderPort;											// +0x178
	unsigned char m_pad0x17A[2];										// +0x17A
	Int m_playerWins[8];												// +0x17C
	Int m_playerLosses[8];												// +0x19C
	Int m_playerProfileID[8];											// +0x1BC
	Int m_playerColors[8];												// +0x1DC (getPlayerColor 0x00388914)
	Int m_playerHandicaps[8];											// +0x1FC (getPlayerHandicap 0x0038892F, the ledger's name)
	Int m_playerFactions[8];											// +0x21C (getPlayerFaction 0x003888F9, the ledger's name)
	Int m_unk0x23C[14];													// +0x23C..+0x273 (numPlayers, maxPlayers, numObservers and counters; not typed yet)
	Int m_nextStagingServer;											// +0x274 (addServerToMap 0x00389F4A post-increments it)
	_STL::map<Int, _SBServer *> m_stagingServers;						// +0x278
	_STL::wstring m_localStagingServerName;								// +0x284
	Int m_unk0x290;														// +0x290
	QMStatus m_qmStatus;												// +0x294
	unsigned char m_qmInfo[0x1EC];										// +0x298 (a PeerRequest; the constructor calls 0x001EF661 on it)
	Bool m_roomJoined;													// +0x484
	unsigned char m_pad0x485[3];										// +0x485
	Int m_qmGroupRoom;													// +0x488
	Bool m_sawEndOfEnumPlayers;											// +0x48C
	Bool m_sawMatchbot;													// +0x48D
	unsigned char m_pad0x48E[2];										// +0x48E
	_STL::string m_matchbotName;										// +0x490
	Bool m_unk0x49C;													// +0x49C
	unsigned char m_pad0x49D[3];										// +0x49D
	Int m_unk0x4A0;														// +0x4A0
	Int m_unk0x4A4;														// +0x4A4
	Bool m_unk0x4A8;													// +0x4A8
	unsigned char m_pad0x4A9[3];										// +0x4A9
	MutexClass *m_queueMutex;											// +0x4AC
};
typedef char PeerThreadClassSize[(sizeof(PeerThreadClass) == 0x4B0) ? 1 : -1];
typedef char PeerThreadClassQmOffset[(offsetof(PeerThreadClass, m_qmStatus) == 0x294) ? 1 : -1];

// The peer thread's message queue (the interface the lobby code calls; the concrete queue is GameSpyPeerMessageQueue).
class GameSpyPeerMessageQueueInterface
{
public:
	virtual ~GameSpyPeerMessageQueueInterface() {}							// slot 0x00
	virtual void startThread() = 0;											// slot 0x04
	virtual void endThread() = 0;											// slot 0x08
	virtual Bool isThreadRunning() = 0;										// slot 0x0C
	virtual Bool isConnected() = 0;											// slot 0x10
	virtual Bool isConnecting() = 0;										// slot 0x14
	virtual void addRequest(const PeerRequest &request) = 0;				// slot 0x18
	virtual Bool getRequest(PeerRequest &request) = 0;						// slot 0x1C
	virtual void addResponse(const PeerResponse &response) = 0;				// slot 0x20
	virtual Bool getResponse(PeerResponse &response) = 0;					// slot 0x24
	virtual UnsignedInt getSerialAuthResult() = 0;							// slot 0x28 (returns SerialAuthResult)
	static GameSpyPeerMessageQueueInterface *createNewMessageQueue();		// 0x0038E460 (allocates the 0x78-byte GameSpyPeerMessageQueue and runs its constructor 0x0038E171)
};

// The concrete peer queue (Zero Hour's GameSpyPeerMessageQueue by role; retail's table 0x00C19868 has the interface's eleven slots).
// Target facts: the thread pointer is at +0x64 (startThread 0x0038B1C0 stores the new thread there; isConnecting 0x003889F6 reads
// the byte at thread+0x50); two eight-byte mutexes follow the table, then two 0x28-byte queues, the thread, the serial result, a third mutex and a four-byte object (the allocation in createNewMessageQueue 0x0038E460 is 0x78).
class GameSpyPeerMessageQueue : public GameSpyPeerMessageQueueInterface
{
public:
	GameSpyPeerMessageQueue();											// 0x0038E171
	virtual ~GameSpyPeerMessageQueue();									// slot 0x00, deleting 0x0038E1E6, body 0x0038DF4C
	virtual void startThread();											// slot 0x04, 0x0038B1C0
	virtual void endThread();											// slot 0x08, 0x0038909E
	virtual Bool isThreadRunning();										// slot 0x0C, 0x00550609
	virtual Bool isConnected();											// slot 0x10, 0x003889E8
	virtual Bool isConnecting();										// slot 0x14, 0x003889F6
	virtual void addRequest(const PeerRequest &request);				// slot 0x18, 0x0038D770
	virtual Bool getRequest(PeerRequest &request);						// slot 0x1C, 0x0038D4C4
	virtual void addResponse(const PeerResponse &response);				// slot 0x20, 0x0038D7BD
	virtual Bool getResponse(PeerResponse &response);					// slot 0x24, 0x0038DFCA
	virtual UnsignedInt getSerialAuthResult();							// slot 0x28, 0x00367508

	MutexClass m_requestMutex;											// +0x04
	MutexClass m_responseMutex;											// +0x0C
	_STL::queue<PeerRequest> m_requests;								// +0x14 (the request queue; a deque, 0x28 bytes: addRequest 0x0038D770 pushes at +0x14, getRequest 0x0038D4C4 tests its start and finish)
	_STL::queue<PeerResponse> m_responses;								// +0x3C (the response queue; addResponse 0x0038D7BD, getResponse 0x0038DFCA)
	PeerThreadClass *m_thread;											// +0x64
	UnsignedInt m_serialAuth;											// +0x68
	MutexClass m_unk0x6C;												// +0x6C (the destructor 0x0038DF4C runs ~MutexClass on it)
	UnsignedInt m_unk0x74;												// +0x74 (an object with the destructor 0x0049990D; endThread 0x0038909E calls that on it; type not recovered)
};
typedef char GameSpyPeerMessageQueueSize[(sizeof(GameSpyPeerMessageQueue) == 0x78) ? 1 : -1];
typedef char GameSpyPeerMessageQueueThreadOffset[(offsetof(GameSpyPeerMessageQueue, m_thread) == 0x64) ? 1 : -1];

extern GameSpyPeerMessageQueueInterface *TheGameSpyPeerMessageQueue;		// VA 0x00E02340

#endif
