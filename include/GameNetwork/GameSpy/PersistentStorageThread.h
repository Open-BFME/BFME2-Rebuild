// PersistentStorageThread.h: the GameSpy persistent-storage worker thread (Zero Hour's PSThreadClass by role and member order,
// GPL-3.0-or-later; offsets are retail's).
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Target facts (code RVAs): the constructor 0x0055436A runs ThreadClass(0), stores table 0x00C6B0D0 (three slots, like the peer
// thread's), builds the container at +0x5C (0x0073C432), takes the queue mutex pointer into +0x68 and clears the bytes at
// +0x50/+0x51/+0x58 and the dword at +0x54; the destructor 0x0055405A frees the container's nodes and then runs ~ThreadClass.
// The class is 0x6C bytes (Thread_Function 0x005580FB, the persistence callbacks 0x00552C7A..0x00552C9E read +0x50/+0x51/+0x54).
#pragma once
#ifndef BFME2_PERSISTENTSTORAGETHREAD_H
#define BFME2_PERSISTENTSTORAGETHREAD_H

#include <stddef.h>
#include "Common/GameType.h"
#include "WWLib/thread.h"
#include "WWLib/mutex.h"

// What preAuthCDCallback 0x00552C9E fills for the caller waiting on the CD-key login (success, done, profile id).
struct CDAuthInfo
{
	Bool success;								// +0x00
	Bool done;									// +0x01
	unsigned char m_pad02[2];					// +0x02
	Int id;										// +0x04
};

class PSThreadClass : public ThreadClass
{
public:
	PSThreadClass(MutexClass *queueMutex);		// 0x0055436A
	virtual ~PSThreadClass();					// slot 0, body 0x0055405A
	virtual void Thread_Function();				// slot 2, 0x005580FB

	void persAuthCallback(Bool val) { m_loginOK = val; m_doneTryingToLogin = true; }
	void decrOpCount() { --m_opCount; }

private:
	Bool tryConnect();							// 0x00552C5F (true when the stats connection is open or InitStatsConnection(0) succeeds; the map spells it private)
public:
	Bool m_loginOK;								// +0x50
	Bool m_doneTryingToLogin;					// +0x51
	unsigned char m_pad52[2];					// +0x52
	Int m_opCount;								// +0x54
	Bool m_sawLocalData;						// +0x58
	unsigned char m_pad59[3];					// +0x59
	unsigned char m_unk0x5C[0xC];				// +0x5C (a node container: the destructor frees each node's pointer at +0x14)
	MutexClass *m_queueMutex;					// +0x68
};
typedef char PSThreadClassSize[(sizeof(PSThreadClass) == 0x6C) ? 1 : -1];

#endif
