// mutex.h: WWLib's three synchronization classes (WWVegas/WWLib/mutex.h).
// Target facts (retail game.dat; names from the map's mutex.cpp rows): MutexClass
// ctor 0x006139F0 and CriticalSectionClass ctor 0x00613AE0 each initialise two
// words, handle +0 and a lock count +4 (SIZE 0x8). The mutex's Lock 0x00613A30
// takes the wait time, whose -1 is WAIT_INFINITE. LockClass's constructor
// 0x00613A70 (mutex) calls Lock with the time and keeps a failure flag; its
// destructor 0x00613AC0 unlocks only when the lock was taken. The critical
// section's handle is an array allocation of the OS structure (ctor 0x00613AE0,
// destructor 0x00613B10 releases it with delete[]).
// FastCriticalSectionClass is one lock word (+0). Its LockClass constructor
// calls the out-of-line flag setter 0x0006577F (the map names it
// BFMEPoolCriticalSection::Lock; wwstring's temporary-string pool takes it) and
// its destructor 0x000657D0 is a store of 0 through the saved reference.
// ProfileFastCS (0x006C5EF0, 0x006C5F40) is a separate profile-library class
// with the same shape and is not this one.
// Donor facts: the member names follow Zero Hour's WWLib/mutex.h (GPL), used for
// naming only.
#pragma once
#ifndef BFME2_MUTEX_H
#define BFME2_MUTEX_H

class MutexClass
{
	void *handle;				// +0x00
	unsigned locked;			// +0x04
	bool Lock(int time);			// 0x00613A30
	void Unlock();				// 0x00613A60

public:
	MutexClass(const char *name = 0);	// 0x006139F0
	~MutexClass();				// 0x00613A20
	enum { WAIT_INFINITE = -1 };

	class LockClass
	{
		MutexClass &mutex;
		bool failed;
	public:
		LockClass(MutexClass &m, int time = MutexClass::WAIT_INFINITE);	// 0x00613A70
		~LockClass();							// 0x00613AC0
		bool Failed() { return failed; }
	private:
		LockClass &operator=(const LockClass &) { return *this; }
	};
	friend class LockClass;
};
// SIZE 0x08

class CriticalSectionClass
{
	void *handle;				// +0x00
	unsigned locked;			// +0x04
	void Lock();				// 0x00613B30
	void Unlock();				// 0x00613B50

public:
	CriticalSectionClass();			// 0x00613AE0
	~CriticalSectionClass();		// 0x00613B10

	class LockClass
	{
		CriticalSectionClass &CriticalSection;
	public:
		LockClass(CriticalSectionClass &c);
		~LockClass();
	private:
		LockClass &operator=(const LockClass &) { return *this; }
	};
	friend class LockClass;
};
// SIZE 0x08

class FastCriticalSectionClass
{
public:
	unsigned Flag;				// +0x00

	// Retail spins on `lock bts` and yields through ThreadClass::Switch_Thread
	// (0x006105C0); the body is hand-written assembly in the original, so it is
	// declared here and not recovered.
	void Thread_Safe_Set_Flag();		// 0x0006577F

	class LockClass
	{
		FastCriticalSectionClass &cs;
	public:
		LockClass(FastCriticalSectionClass &critical_section) : cs(critical_section) { cs.Thread_Safe_Set_Flag(); }	// 0x000657BC
		~LockClass() { cs.Flag = 0; }					// 0x000657D0
	};
};
// SIZE 0x04

#endif
