// thread.h: WWLib's thread wrapper (WWVegas/WWLib/thread.h).
// SIZE 0x50, vftable 0x00C7B6AC with three slots: deleting destructor 0x00610620,
// Execute 0x006104B0 and the pure Thread_Function.
// Target facts (retail game.dat): the constructor 0x00610430 stores the table at +0,
// copies the name into the 64-byte buffer at +4 (default "No name"), and zeroes
// ThreadID +0x44, the handle +0x48 and the priority +0x4C. Execute creates the
// OS thread with the BFME start routine at 0x00610490 (which first calls the
// Debug singleton's InstallExceptionHandler, slot 22, then Thread_Function, slot
// 2), stores the handle, applies the priority and resumes it. Stop(ms) waits for
// the handle to clear and terminates the thread when the wait passes the limit.
// Donor facts: names follow Zero Hour's WWLib/thread.h (GPL), naming only.
#pragma once
#ifndef BFME2_THREAD_H
#define BFME2_THREAD_H

class ThreadClass
{
public:
	ThreadClass(const char *name = 0);		// 0x00610430
	virtual ~ThreadClass();				// 0x00610480
	virtual void Execute();				// 0x006104B0
	virtual void Thread_Function() = 0;

	void Set_Priority(int priority);		// 0x006104F0
	void Stop(unsigned ms);			// 0x00610510
	void Stop();					// 0x006105F0
	bool Is_Running();				// 0x006105E0

	static void Switch_Thread();			// 0x006105C0
	static unsigned _Get_Current_Thread_ID();	// 0x006105D0 (an import thunk to GetCurrentThreadId)

protected:
	char ThreadName[64];				// +0x04
	unsigned long ThreadID;			// +0x44
	void *handle;					// +0x48
	int ThreadPriority;				// +0x4C
};
// SIZE 0x50

#endif
