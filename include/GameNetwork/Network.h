// Network.h: the top-level network subsystem behind TheNetwork (Zero Hour's Network / NetworkInterface by role; replaces
// Draft/Network.h).
// Target facts (code RVAs, VA 0x007F6040 table, 67 slots):
//  - SIZE 0x40 (CreateTheNetwork 0x0025E46D allocates 0x40 and runs the constructor 0x0025DB6D). The constructor runs
//    SubsystemInterface's 0x001B4E63 (the base is 0xC bytes), stores the table and sets +0x3C to -1, +0xC (the
//    ConnectionManager), +0x10 (state), +0x28, +0x2C, +0x30 (byte), +0x34 and +0x38 (byte) to zero, then fills +0x18 and +0x20
//    with QueryPerformanceFrequency / QueryPerformanceCounter (kernel32 imports 0x007BA2E4 / 0x007BA2E0). The destructor
//    0x0025E4CD (deleting 0x0025E51D) restores the table, deletes the ConnectionManager (its destructor 0x004D2344, then
//    operator delete) and runs SubsystemInterface's 0x001B4E74.
//  - Slots 0..13 are SubsystemInterface's: Network overrides the destructor, init (slot 1, 0x0025DF01: replaces the
//    ConnectionManager with a fresh 0x121B0-byte one, calls its slot 0, zeroes the state, reads the performance counter and
//    the clock, resets the game logic and sets +0x3C to -1), and leaves reset (slot 9, 0x005CB260: `mov eax,[ecx]; jmp
//    [eax+4]`, i.e. it re-enters init) and the others to the base. Slots 14..66 are the NetworkInterface slots below.
//  - The map names most of the NetworkInterface slots through placeholder classes (the destructor's own address names them),
//    and four through `NetworkInterface` itself (getFrameAdvanceCount, getFramePacingStatus, isPacketRouter,
//    ProcessDestroyPlayerCommand): their bodies are Network's, which is why only the Network-named rows are written. Slots
//    without a map name are declared `vslotNN` with what their body does; most forward to the ConnectionManager at +0xC.
//  - Retail lays out same-name overloads in reverse declaration order: InitTransport() is declared before
//    InitTransport(Int) so that the (Int) overload takes the lower slot 0x4C.
#pragma once
#ifndef BFME2_NETWORK_H
#define BFME2_NETWORK_H

#include "Common/GameType.h"
#include "Common/AsciiString.h"
#include "Common/UnicodeString.h"
#include "Common/SubsystemInterface.h"

class ConnectionManager;
class CreateAHeroData;
struct NetLocalAddress
{
	UnsignedInt m_ip;											// +0
	UnsignedInt m_port;											// +4
};

class NetworkInterface : public SubsystemInterface
{
public:
	virtual void startNewSession() = 0;                                        // slot 0x38, 0x0025DFA1: resets the session state and tells the connection manager (slot 2)
	virtual void vslot3C(Int frame) = 0;                                       // slot 0x3C, 0x0025DBAE: forwards update(state == 1, frame) to the connection manager's slot 3
	virtual void vslot40(Int doInit) = 0;                                      // slot 0x40, 0x0025E970: starts a session when idle, runs the update above, then the disconnect screen and the state 3 hand-off
	virtual void ParseUserList(Int gameInfo) = 0;                              // slot 0x44, 0x0025DBCD: forwards to ConnectionManager::parseUserList 0x004D1616 (the map spells the argument as an int)
	virtual void SetLocalAddr(const NetLocalAddress &address) = 0;             // slot 0x48, 0x0025DBDC: copies the two address words to ConnectionManager +0x12050
	virtual void InitTransport() = 0;                                          // slot 0x50, 0x0025DBFB: forwards to ConnectionManager::initTransport 0x004CF664
	virtual void InitTransport(Int transport) = 0;                             // slot 0x4C, 0x0025DC08: forwards to ConnectionManager::attachTransport 0x004CF113
	virtual Int getFrameAdvanceCount() = 0;                                    // slot 0x54, 0x0025E014: frame pacing (the map names it NetworkInterface::getFrameAdvanceCount)
	virtual Int getFramePacingStatus() = 0;                                    // slot 0x58, 0x0025E163: frame pacing (the map names it NetworkInterface::getFramePacingStatus)
	virtual UnsignedInt vslot5C() = 0;                                         // slot 0x5C, 0x0025DC17: forwards to ConnectionManager 0x004CFC81, 0 without one
	virtual Bool vslot60() = 0;                                                // slot 0x60, 0x0025DC26: state == 1; ConnectionManager::getFrameCommandList 0x004D0027 calls it through TheNetwork
	virtual void notifyOthersOfCurrentFrame() = 0;                                                // slot 0x64, 0x0025E28E: forwards to ConnectionManager::sendFrameInfo 0x004D0AA9
	virtual void sendChat(UnicodeString text, Int playerMask) = 0;             // slot 0x68, 0x0025E29D: forwards to ConnectionManager::sendChat 0x004D17C5
	virtual void sendDisconnectChat(UnicodeString text) = 0;                   // slot 0x6C, 0x0025E2E3: forwards to ConnectionManager::sendDisconnectChat 0x004D187B
	virtual void sendFile(AsciiString path, UnsignedByte playerMask, UnsignedShort commandID) = 0;// slot 0x70, 0x0025E327: forwards to ConnectionManager::sendFile 0x004D1927
	virtual UnsignedShort sendFileAnnounce(AsciiString path, UnsignedByte playerMask) = 0;// slot 0x74, 0x0025E370: forwards to ConnectionManager::sendFileAnnounce 0x004D2D6A
	virtual Int getFileTransferProgress(Int slot, AsciiString path) = 0;                       // slot 0x78, 0x0025E3BB: forwards to ConnectionManager::getFileTransferProgress 0x004D28C6
	virtual Bool areAllQueuesEmpty() = 0;                                                // slot 0x7C, 0x0025DC30: forwards to ConnectionManager::areAllQueuesEmpty 0x004CF3B9
	virtual void vslot80() = 0;                                                // slot 0x80, 0x0025DC38: sets the byte at ConnectionManager +0x12134
	virtual void vslot84() = 0;                                                // slot 0x84, 0x0025DC47: clears the byte at ConnectionManager +0x12134
	virtual void vslot88(CreateAHeroData *hero, Int slot, UnsignedShort commandID, UnsignedByte playerMask) = 0;// slot 0x88, 0x0025DC56: forwards to ConnectionManager::sendHero 0x004D01D5
	virtual void updateLoadProgress(Int progress) = 0;                                    // slot 0x8C, 0x0025DC5E: forwards to ConnectionManager::updateLoadProgress 0x004CF940
	virtual void loadProgressComplete() = 0;                                                // slot 0x90, 0x0025DC6D: forwards to ConnectionManager::loadProgressComplete 0x004D02F6
	virtual void quitGame() = 0;                                               // slot 0x94, 0x0025E233: asks the connection manager to wind down, shows the 0x1D message and sets the state to 3
	virtual void vslot98(Int slot) = 0;                                        // slot 0x98, 0x0025E20F: votes the slot out through the connection manager and reports to the game logic
	virtual void requestLeave(Int slot) = 0;                                        // slot 0x9C, 0x0025E27F: forwards to ConnectionManager::requestLeave 0x004D00BB
	virtual Int vslotA0() = 0;                                                 // slot 0xA0, 0x0025DC7A: forwards to the DisconnectManager's 0x0057429A
	virtual Int vslotA4() = 0;                                                 // slot 0xA4, 0x0025DC95: forwards to the DisconnectManager's 0x0028A65E
	virtual Int vslotA8() = 0;                                                 // slot 0xA8, 0x0025DCB0: forwards to the DisconnectManager's 0x004E05E9
	virtual Bool isPacketRouter() = 0;                                         // slot 0xAC, 0x0025DCCB: forwards to ConnectionManager::isPacketRouter 0x004CF9CD (the map names it NetworkInterface::isPacketRouter)
	virtual UnsignedInt getPacketRouterSlot() = 0;                                         // slot 0xB0, 0x0025DCE2: forwards to ConnectionManager::getPacketRouterSlot 0x004CF3B2
	virtual Int getNumPlayers() = 0;                                                 // slot 0xB4, 0x0025DCF1: forwards to ConnectionManager::getNumPlayers 0x004CF926
	virtual UnsignedInt getLocalPlayerID() = 0;                                         // slot 0xB8, 0x0025DD01: forwards to ConnectionManager::getLocalPlayerID 0x004CF906
	virtual UnicodeString getPlayerName(Int slot) = 0;                               // slot 0xBC, 0x0025E406: forwards to ConnectionManager::getPlayerName 0x004D0198
	virtual UnsignedByte vslotC0() = 0;                                        // slot 0xC0, 0x005FD047: returns the byte at +0x30 (shared folded getter)
	virtual void vslotC4() = 0;                                                // slot 0xC4, 0x0025DD11: sets the byte at +0x38
	virtual void vslotC8() = 0;                                                // slot 0xC8, 0x0025DD16: clears the byte at +0x38
	virtual Bool isPlayerConnected(Int slot) = 0;                                        // slot 0xCC, 0x0025DFE8: forwards to ConnectionManager::isPlayerConnected 0x004CF083
	virtual Int vslotD0() = 0;                                                 // slot 0xD0, 0x0025DD25: the DisconnectManager's byte at +0x270
	virtual Real getIncomingBytesPerSecond() = 0;                                               // slot 0xD4, 0x0025DD48: forwards to the transport's 0x004D4F7A, 0 without one
	virtual Real getIncomingPacketsPerSecond() = 0;                                               // slot 0xD8, 0x0025DD76: forwards to the transport's accessor (same shape)
	virtual Real getOutgoingBytesPerSecond() = 0;                                               // slot 0xDC, 0x0025DDA4: forwards to the transport's accessor (same shape)
	virtual Real getOutgoingPacketsPerSecond() = 0;                                               // slot 0xE0, 0x0025DDD2: forwards to the transport's accessor (same shape)
	virtual Real getUnknownBytesPerSecond() = 0;                                               // slot 0xE4, 0x0025DE00: forwards to the transport's accessor (same shape)
	virtual Real getUnknownPacketsPerSecond() = 0;                                               // slot 0xE8, 0x0025DE2E: forwards to the transport's accessor (same shape)
	virtual Int vslotEC() = 0;                                                 // slot 0xEC, 0x0025DE5C: ConnectionManager +0x1205C minus the game logic frame
	virtual Int vslotF0() = 0;                                                 // slot 0xF0, 0x005C4AE9: shared folded dword getter
	virtual Int vslotF4(Int slot) = 0;                                         // slot 0xF4, 0x0025DE76: a command history query: slot*0x2000 + 0x24 into the connection manager, -1 out of range
	virtual Int vslotF8(Int slot) = 0;                                         // slot 0xF8, 0x0025DEA2: ConnectionManager acked frame of a slot
	virtual Bool vslotFC() = 0;                                                // slot 0xFC, 0x0025DD1B: +0x3C != -1
	virtual Bool isPlayerSlotActive(Int slot) = 0;                                       // slot 0x100, 0x0025DEBB: forwards to ConnectionManager::isPlayerSlotActive 0x004CF0CD
	virtual UnsignedByte vslot104(Int slot) = 0;                               // slot 0x104, 0x0025DECC: forwards to ConnectionManager 0x004CEF58
	virtual void vslot108(Int slot, Int frame, const UnicodeString &text) = 0; // slot 0x108, 0x0025DEDD: forwards to ConnectionManager 0x004D0E93
};

class Network : public NetworkInterface
{
public:
	Network();													// 0x0025DB6D
	virtual ~Network();											// slot 0, 0x0025E4CD (deleting 0x0025E51D)
	virtual void init();										// slot 1, 0x0025DF01
	virtual void reset() { init(); }							// slot 9, 0x005CB260 (a tail call through slot 1)
	virtual void update() {}									// slot 10 (the shared empty body 0x000B3FD0)

	virtual void startNewSession();                                            // slot 0x38, 0x0025DFA1: resets the session state and tells the connection manager (slot 2)
	virtual void vslot3C(Int frame);                                           // slot 0x3C, 0x0025DBAE: forwards update(state == 1, frame) to the connection manager's slot 3
	virtual void vslot40(Int doInit);                                          // slot 0x40, 0x0025E970: starts a session when idle, runs the update above, then the disconnect screen and the state 3 hand-off
	virtual void ParseUserList(Int gameInfo);                                  // slot 0x44, 0x0025DBCD: forwards to ConnectionManager::parseUserList 0x004D1616 (the map spells the argument as an int)
	virtual void SetLocalAddr(const NetLocalAddress &address);                 // slot 0x48, 0x0025DBDC: copies the two address words to ConnectionManager +0x12050
	virtual void InitTransport();                                              // slot 0x50, 0x0025DBFB: forwards to ConnectionManager::initTransport 0x004CF664
	virtual void InitTransport(Int transport);                                 // slot 0x4C, 0x0025DC08: forwards to ConnectionManager::attachTransport 0x004CF113
	virtual Int getFrameAdvanceCount();                                        // slot 0x54, 0x0025E014: frame pacing (the map names it NetworkInterface::getFrameAdvanceCount)
	virtual Int getFramePacingStatus();                                        // slot 0x58, 0x0025E163: frame pacing (the map names it NetworkInterface::getFramePacingStatus)
	virtual UnsignedInt vslot5C();                                             // slot 0x5C, 0x0025DC17: forwards to ConnectionManager 0x004CFC81, 0 without one
	virtual Bool vslot60();                                                    // slot 0x60, 0x0025DC26: state == 1; ConnectionManager::getFrameCommandList 0x004D0027 calls it through TheNetwork
	virtual void notifyOthersOfCurrentFrame();                                                    // slot 0x64, 0x0025E28E: forwards to ConnectionManager::sendFrameInfo 0x004D0AA9
	virtual void sendChat(UnicodeString text, Int playerMask);                 // slot 0x68, 0x0025E29D: forwards to ConnectionManager::sendChat 0x004D17C5
	virtual void sendDisconnectChat(UnicodeString text);                       // slot 0x6C, 0x0025E2E3: forwards to ConnectionManager::sendDisconnectChat 0x004D187B
	virtual void sendFile(AsciiString path, UnsignedByte playerMask, UnsignedShort commandID);// slot 0x70, 0x0025E327: forwards to ConnectionManager::sendFile 0x004D1927
	virtual UnsignedShort sendFileAnnounce(AsciiString path, UnsignedByte playerMask);// slot 0x74, 0x0025E370: forwards to ConnectionManager::sendFileAnnounce 0x004D2D6A
	virtual Int getFileTransferProgress(Int slot, AsciiString path);                           // slot 0x78, 0x0025E3BB: forwards to ConnectionManager::getFileTransferProgress 0x004D28C6
	virtual Bool areAllQueuesEmpty();                                                    // slot 0x7C, 0x0025DC30: forwards to ConnectionManager::areAllQueuesEmpty 0x004CF3B9
	virtual void vslot80();                                                    // slot 0x80, 0x0025DC38: sets the byte at ConnectionManager +0x12134
	virtual void vslot84();                                                    // slot 0x84, 0x0025DC47: clears the byte at ConnectionManager +0x12134
	virtual void vslot88(CreateAHeroData *hero, Int slot, UnsignedShort commandID, UnsignedByte playerMask);// slot 0x88, 0x0025DC56: forwards to ConnectionManager::sendHero 0x004D01D5
	virtual void updateLoadProgress(Int progress);                                        // slot 0x8C, 0x0025DC5E: forwards to ConnectionManager::updateLoadProgress 0x004CF940
	virtual void loadProgressComplete();                                                    // slot 0x90, 0x0025DC6D: forwards to ConnectionManager::loadProgressComplete 0x004D02F6
	virtual void quitGame();                                                   // slot 0x94, 0x0025E233: asks the connection manager to wind down, shows the 0x1D message and sets the state to 3
	virtual void vslot98(Int slot);                                            // slot 0x98, 0x0025E20F: votes the slot out through the connection manager and reports to the game logic
	virtual void requestLeave(Int slot);                                            // slot 0x9C, 0x0025E27F: forwards to ConnectionManager::requestLeave 0x004D00BB
	virtual Int vslotA0();                                                     // slot 0xA0, 0x0025DC7A: forwards to the DisconnectManager's 0x0057429A
	virtual Int vslotA4();                                                     // slot 0xA4, 0x0025DC95: forwards to the DisconnectManager's 0x0028A65E
	virtual Int vslotA8();                                                     // slot 0xA8, 0x0025DCB0: forwards to the DisconnectManager's 0x004E05E9
	virtual Bool isPacketRouter();                                             // slot 0xAC, 0x0025DCCB: forwards to ConnectionManager::isPacketRouter 0x004CF9CD (the map names it NetworkInterface::isPacketRouter)
	virtual UnsignedInt getPacketRouterSlot();                                             // slot 0xB0, 0x0025DCE2: forwards to ConnectionManager::getPacketRouterSlot 0x004CF3B2
	virtual Int getNumPlayers();                                                     // slot 0xB4, 0x0025DCF1: forwards to ConnectionManager::getNumPlayers 0x004CF926
	virtual UnsignedInt getLocalPlayerID();                                             // slot 0xB8, 0x0025DD01: forwards to ConnectionManager::getLocalPlayerID 0x004CF906
	virtual UnicodeString getPlayerName(Int slot);                                   // slot 0xBC, 0x0025E406: forwards to ConnectionManager::getPlayerName 0x004D0198
	virtual UnsignedByte vslotC0();                                            // slot 0xC0, 0x005FD047: returns the byte at +0x30 (shared folded getter)
	virtual void vslotC4();                                                    // slot 0xC4, 0x0025DD11: sets the byte at +0x38
	virtual void vslotC8();                                                    // slot 0xC8, 0x0025DD16: clears the byte at +0x38
	virtual Bool isPlayerConnected(Int slot);                                            // slot 0xCC, 0x0025DFE8: forwards to ConnectionManager::isPlayerConnected 0x004CF083
	virtual Int vslotD0();                                                     // slot 0xD0, 0x0025DD25: the DisconnectManager's byte at +0x270
	virtual Real getIncomingBytesPerSecond();                                                   // slot 0xD4, 0x0025DD48: forwards to the transport's 0x004D4F7A, 0 without one
	virtual Real getIncomingPacketsPerSecond();                                                   // slot 0xD8, 0x0025DD76: forwards to the transport's accessor (same shape)
	virtual Real getOutgoingBytesPerSecond();                                                   // slot 0xDC, 0x0025DDA4: forwards to the transport's accessor (same shape)
	virtual Real getOutgoingPacketsPerSecond();                                                   // slot 0xE0, 0x0025DDD2: forwards to the transport's accessor (same shape)
	virtual Real getUnknownBytesPerSecond();                                                   // slot 0xE4, 0x0025DE00: forwards to the transport's accessor (same shape)
	virtual Real getUnknownPacketsPerSecond();                                                   // slot 0xE8, 0x0025DE2E: forwards to the transport's accessor (same shape)
	virtual Int vslotEC();                                                     // slot 0xEC, 0x0025DE5C: ConnectionManager +0x1205C minus the game logic frame
	virtual Int vslotF0();                                                     // slot 0xF0, 0x005C4AE9: shared folded dword getter
	virtual Int vslotF4(Int slot);                                             // slot 0xF4, 0x0025DE76: a command history query: slot*0x2000 + 0x24 into the connection manager, -1 out of range
	virtual Int vslotF8(Int slot);                                             // slot 0xF8, 0x0025DEA2: ConnectionManager acked frame of a slot
	virtual Bool vslotFC();                                                    // slot 0xFC, 0x0025DD1B: +0x3C != -1
	virtual Bool isPlayerSlotActive(Int slot);                                           // slot 0x100, 0x0025DEBB: forwards to ConnectionManager::isPlayerSlotActive 0x004CF0CD
	virtual UnsignedByte vslot104(Int slot);                                   // slot 0x104, 0x0025DECC: forwards to ConnectionManager 0x004CEF58
	virtual void vslot108(Int slot, Int frame, const UnicodeString &text);     // slot 0x108, 0x0025DEDD: forwards to ConnectionManager 0x004D0E93

	ConnectionManager *m_conMgr;								// +0x0C
	Int m_state;												// +0x10 (0 idle, 1 running, 2 and 3 set by the quit path)
	unsigned char m_pad14[4];									// +0x14
	__int64 m_perfFrequency;									// +0x18 (QueryPerformanceFrequency)
	__int64 m_perfCounter;										// +0x20 (QueryPerformanceCounter)
	__int64 m_accumulator;											// +0x28 (counter ticks gathered by getFramePacingStatus 0x0025E163; the session start clears it)
	UnsignedByte m_unk0x30;										// +0x30 (the byte slot 0xC0 returns)
	unsigned char m_pad31[3];									// +0x31
	Int m_unk0x34;												// +0x34
	UnsignedByte m_unk0x38;										// +0x38 (set and cleared by slots 0xC4 / 0xC8)
	unsigned char m_pad39[3];									// +0x39
	Int m_unk0x3C;												// +0x3C (-1 when unset; slot 0xFC tests it)
};
// SIZE 0x40
typedef char NetworkSize[(sizeof(Network) == 0x40) ? 1 : -1];

// 0x0025E46D: replaces TheNetwork with a fresh Network and initialises it.
extern NetworkInterface *TheNetwork;
void CreateTheNetwork();

#endif
