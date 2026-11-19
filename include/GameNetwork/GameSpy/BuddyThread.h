// BuddyThread.h: the queue the lobby code uses to talk to the GameSpy buddy thread (Zero Hour's
// GameSpyBuddyMessageQueueInterface / GameSpyBuddyMessageQueue / BuddyThreadClass by role and member order,
// GPL-3.0-or-later; offsets and slots are retail's).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts (code RVAs; table addresses are VAs):
//  - The queue is GameSpyInfo's twin of the peer queue (TheGameSpyBuddyMessageQueue, VA 0x00E05FBC): createNewMessageQueue
//    0x00551B28 allocates 0x74 bytes and runs its constructor 0x00551A94. Its table has thirteen slots: 0 deleting destructor
//    0x00551B02, 1 startThread 0x00550FDD, 2 endThread 0x00550B4C, 3 isThreadRunning 0x00550609 (the peer queue's folded copy:
//    forwards to the thread's 0x006105E0), 4 isConnected 0x00550618, 5 0x003889E8 (the peer queue's
//    isConnected body, folded), 6 addRequest 0x00551B5A, 7 getRequest 0x0055106D, 8 addResponse 0x00551BA7, 9 getResponse
//    0x005510D3, 10 getLocalProfileID 0x00550626, 11 and 12 the two string getters 0x005508F0 / 0x00550904. The layout follows
//    the peer queue's: the request mutex at +4, the response mutex at +0xC, two queues (a deque each, 0x28 bytes) at +0x14 and
//    +0x3C, the thread pointer at +0x64, a mutex at +0x68 and a cleared dword at +0x70 (the constructor 0x00551A94 builds the three
//    mutexes with 0x006139F0 and the two queues with 0x00551A06 / 0x00551A1B).
//  - The thread (BuddyThreadClass: its table's slots are the deleting destructor 0x00551323, ThreadClass::Execute 0x006104B0 and
//    Thread_Function 0x00552027) keeps a flag byte at +0x52 (isConnected reads it), the local profile id at +0x54 and the two
//    std::strings at +0x84 and +0x90 that the string getters return the character pointers of (a getter answers "" without a
//    thread). Its other members are not recovered.
#pragma once
#ifndef BFME2_BUDDYTHREAD_H
#define BFME2_BUDDYTHREAD_H

#include <string>
#include "Common/GameType.h"
#include "WWLib/thread.h"
#include "WWLib/mutex.h"

class BuddyRequest;
class BuddyResponse;
class BuddyThreadClass;

class GameSpyBuddyMessageQueueInterface
{
public:
	virtual ~GameSpyBuddyMessageQueueInterface() {}							// slot 0x00
	virtual void startThread() = 0;											// slot 0x04
	virtual void endThread() = 0;											// slot 0x08
	virtual Bool isThreadRunning() = 0;										// slot 0x0C
	virtual Bool isConnected() = 0;											// slot 0x10
	virtual Bool vslot14() = 0;												// slot 0x14 (0x003889E8, shared with the peer queue's isConnected; name not evidenced)
	virtual void addRequest(const BuddyRequest &request) = 0;				// slot 0x18
	virtual Bool getRequest(BuddyRequest &request) = 0;						// slot 0x1C
	virtual void addResponse(const BuddyResponse &response) = 0;			// slot 0x20
	virtual Bool getResponse(BuddyResponse &response) = 0;					// slot 0x24
	virtual Int getLocalProfileID() = 0;									// slot 0x28
	virtual const char *vslot2C() const = 0;										// slot 0x2C (0x005508F0: the thread's string at +0x84; name not evidenced)
	virtual const char *get() const = 0;										// slot 0x30 (0x00550904: the thread's string at +0x90; the map evidences the name get)
	static GameSpyBuddyMessageQueueInterface *createNewMessageQueue();		// 0x00551B28
};

class GameSpyBuddyMessageQueue : public GameSpyBuddyMessageQueueInterface
{
public:
	GameSpyBuddyMessageQueue();												// 0x00551A94
	virtual ~GameSpyBuddyMessageQueue();									// slot 0x00, deleting 0x00551B02
	virtual void startThread();												// slot 0x04, 0x00550FDD
	virtual void endThread();												// slot 0x08, 0x00550B4C
	virtual Bool isThreadRunning();											// slot 0x0C, 0x00550609
	virtual Bool isConnected();												// slot 0x10, 0x00550618
	virtual Bool vslot14();													// slot 0x14, 0x003889E8
	virtual void addRequest(const BuddyRequest &request);					// slot 0x18, 0x00551B5A
	virtual Bool getRequest(BuddyRequest &request);							// slot 0x1C, 0x0055106D
	virtual void addResponse(const BuddyResponse &response);				// slot 0x20, 0x00551BA7
	virtual Bool getResponse(BuddyResponse &response);						// slot 0x24, 0x005510D3
	virtual Int getLocalProfileID();										// slot 0x28, 0x00550626
	virtual const char *vslot2C() const;											// slot 0x2C, 0x005508F0
	virtual const char *get() const;											// slot 0x30, 0x00550904

	MutexClass m_requestMutex;												// +0x04
	MutexClass m_responseMutex;												// +0x0C
	unsigned char m_requests[0x28];											// +0x14 (a queue<BuddyRequest>, a deque; not typed until BuddyRequest is recovered)
	unsigned char m_responses[0x28];										// +0x3C (a queue<BuddyResponse>)
	BuddyThreadClass *m_thread;												// +0x64
	MutexClass m_unk0x68;													// +0x68 (the constructor 0x00551A94 builds it like the other two)
	UnsignedInt m_unk0x70;													// +0x70 (the constructor clears it)
};
// SIZE 0x74

// The GameSpy buddy thread, partly recovered: only the members the queue's rows read are declared.
class BuddyThreadClass : public ThreadClass
{
public:
	unsigned char m_unk0x50[2];												// +0x50 (the base ThreadClass is 0x50 bytes)
	Bool m_isConnected;														// +0x52
	unsigned char m_pad0x53;												// +0x53
	Int m_profileID;														// +0x54
	unsigned char m_unk0x58[0x2C];											// +0x58
	_STL::string m_unk0x84;													// +0x84
	_STL::string m_unk0x90;													// +0x90
};

extern GameSpyBuddyMessageQueueInterface *TheGameSpyBuddyMessageQueue;		// VA 0x00E05FBC

#endif
