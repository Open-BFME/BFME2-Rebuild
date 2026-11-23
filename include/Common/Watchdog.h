// Watchdog.h: frame heartbeat watchdog worker thread.
#pragma once
#ifndef BFME2_WATCHDOG_H
#define BFME2_WATCHDOG_H

#include <stddef.h>
#include "Common/GameType.h"
#include "WWLib/thread.h"
#include "WWLib/mutex.h"

class Watchdog : public ThreadClass
{
public:
	Watchdog(Int intervalSec, Int warnSec, Int hangSec);
	virtual ~Watchdog();
	virtual void Execute();
	virtual void Thread_Function();
	virtual void reportWatchdog();

	void stop();
	void lock();
	void unlock();
	void pause();
	void heartbeat();
	void resume();

	UnsignedInt m_mainThreadID;
	Int m_lastHeartbeatTime;
	Int m_intervalSec;
	Int m_unk0x5C;
	Int m_warnSec;
	Int m_hangSec;
	Int m_unk0x68;
	Int m_pauseCount;
	UnsignedInt m_criticalSection[6];
	MutexClass m_mutex;
	Int m_unk0x90;
};

Watchdog *createWatchdog(Int intervalSec, Int warnSec, Int hangSec);

extern Watchdog *theBfmeDfe6e4;

typedef char WatchdogSize[(sizeof(Watchdog) == 0x94) ? 1 : -1];
typedef char WatchdogThreadIDOffset[(offsetof(Watchdog, m_mainThreadID) == 0x50) ? 1 : -1];
typedef char WatchdogPauseOffset[(offsetof(Watchdog, m_pauseCount) == 0x6C) ? 1 : -1];
typedef char WatchdogCritSecOffset[(offsetof(Watchdog, m_criticalSection) == 0x70) ? 1 : -1];
typedef char WatchdogMutexOffset[(offsetof(Watchdog, m_mutex) == 0x88) ? 1 : -1];

#endif
