// Synchronization.h: the sys_util library's synchronization objects.
//
// Names. WorldBuilder (same engine libraries, shipped with retail) keeps the
// assert text of this unit, C:\Projects\bfme2\Code\Libraries\Source\sys_util\
// Synchronization.cpp: "Mutex::Mutex", "SynchronizingEvent::SynchronizingEvent",
// "CriticalSection::Lock" ("CriticalSections do not accept timeouts when locked.
// You must use INFINITE timeout with CriticalSections"), "SingleSyncObjectLock::
// Lock" ("Cannot use the same SingleSyncObjectLock object to lock a
// synchronization object twice"), "MultiSyncObjectLock::setUpHandleArray"
// ("Cannot use MultiSyncObjectLock with SyncObjects with no handles (like
// CriticalSection)") and "MultiSyncObjectLock::IsLocked". So the base is
// SyncObject, it holds a handle, and CriticalSection is a SyncObject without one.
// ScopedCriticalSection is the map's name (old ledger) for the guard of
// 0x00035620/0x00035640; Zero Hour's Common/CriticalSection.h has a class of that
// name in the same role (it guards the string and name-key code there too), and
// CriticalSection's enter/exit take Zero Hour's member names.
//
// Target facts (retail game.dat):
// - SyncObject table 0x007C16B8 {Lock 0x00040EF8, __purecall, deleting dtor
//   0x00041184}; inline ctor 0x00040ECE (vptr, handle +4 = 0); dtor 0x00040EDB
//   closes the handle (CloseHandle) and clears it. Lock(timeout) is
//   WaitForSingleObject(handle, timeout) == WAIT_OBJECT_0 or WAIT_ABANDONED.
// - Mutex table 0x007C16C4 {SyncObject::Lock, Unlock 0x00040F4C (ReleaseMutex),
//   deleting dtor 0x000411A0}; ctor 0x00040F1E = CreateMutexA(attributes,
//   initiallyUnlocked == 0, name); dtor 0x000411BC only runs ~SyncObject.
// - SynchronizingEvent table 0x007C16D0; ctor 0x00040F64 = CreateEventA(
//   attributes, manualReset == 0, initiallyUnset == 0, name) (argument roles from
//   the Win32 parameter each one feeds); three handle calls 0x00040F9D,
//   0x00040FB5, 0x00040FCD.
// - CriticalSection table 0x007C16DC {Lock 0x00041004, Unlock 0x00041024,
//   deleting dtor 0x000411F2}; ctor 0x000411C1 zeroes the handle, stores the
//   vptr, clears +0x20, InitializeCriticalSection(+8) and, when its argument is
//   0, locks; dtor 0x00040FE5 DeleteCriticalSection(+8), sets +0x20, ~SyncObject.
//   Every enter and leave skips the OS call while +0x20 is set (so a lock taken
//   during static destruction, after the dtor ran, is harmless). SIZE 0x24:
//   NameKeyGenerator keeps one at +0x2BF60 (nameToKey 0x00148E1A guards it).
// - CriticalSection::enter 0x000355E0 / exit 0x00035600 are the inline
//   non-virtual pair the guard expands to.
// - ScopedCriticalSection { CriticalSection * +0; bool +4 }: ctor 0x00035880
//   (pointer, stays-unlocked flag), dtor 0x000358B0, private Lock 0x00035620 and
//   Unlock 0x00035640 (out-of-line copies; the /O2 string unit inlines them all).
// - SingleSyncObjectLock { SyncObject * +0; bool +4 }: ctor 0x0004120E, dtor
//   0x0004122F, Lock 0x00041037 (refuses a second lock), Unlock 0x00041055.
//
// The Win32 types stay out of this header (it is included by every user of
// NameKeyGenerator.h): handles are void *, the OS critical section is 0x18 bytes
// of storage handed to the kernel32 calls, which are declared exactly as
// windows.h declares them.
#pragma once
#ifndef BFME2_SYNCHRONIZATION_H
#define BFME2_SYNCHRONIZATION_H

struct _RTL_CRITICAL_SECTION;
extern "C" __declspec(dllimport) void __stdcall EnterCriticalSection(struct _RTL_CRITICAL_SECTION *lpCriticalSection) throw();
extern "C" __declspec(dllimport) void __stdcall LeaveCriticalSection(struct _RTL_CRITICAL_SECTION *lpCriticalSection) throw();

class SyncObject
{
public:
	enum { INFINITE_TIMEOUT = -1 };

	SyncObject() : m_handle(0) {}				// 0x00040ECE
	virtual bool Lock(int timeout);				// slot 0: 0x00040EF8
	virtual bool Unlock() = 0;					// slot 1
	virtual ~SyncObject();						// slot 2: 0x00040EDB (deleting 0x00041184)

protected:
	void *m_handle;								// +0x04
};
// SIZE 0x8

class Mutex : public SyncObject
{
public:
	Mutex(int initiallyUnlocked, const char *name, void *securityAttributes);	// 0x00040F1E
	virtual bool Unlock();						// slot 1: 0x00040F4C
	virtual ~Mutex();							// 0x000411BC (deleting 0x000411A0)
};
// SIZE 0x8

class CriticalSection : public SyncObject
{
public:
	CriticalSection(int initiallyUnlocked);	// 0x000411C1
	virtual bool Lock(int timeout);				// slot 0: 0x00041004
	virtual bool Unlock();						// slot 1: 0x00041024
	virtual ~CriticalSection();					// 0x00040FE5 (deleting 0x000411F2)

	void enter()								// 0x000355E0
	{
		if (!m_destroyed)
			EnterCriticalSection((struct _RTL_CRITICAL_SECTION *)m_section);
	}
	void exit()									// 0x00035600
	{
		if (!m_destroyed)
			LeaveCriticalSection((struct _RTL_CRITICAL_SECTION *)m_section);
	}

private:
	void *m_section[6];							// +0x08: the OS CRITICAL_SECTION (0x18 bytes)
	bool m_destroyed;							// +0x20: set by the destructor
};
// SIZE 0x24

class ScopedCriticalSection
{
public:
	ScopedCriticalSection(CriticalSection *cs, int stayUnlocked = 0)	// 0x00035880
		: m_cs(cs), m_locked(false)
	{
		if (!stayUnlocked)
			Lock();
	}
	~ScopedCriticalSection()					// 0x000358B0
	{
		if (m_locked)
			Unlock();
	}

private:
	void Lock()									// 0x00035620
	{
		m_cs->enter();
		m_locked = true;
	}
	void Unlock()								// 0x00035640
	{
		m_cs->exit();
		m_locked = false;
	}

	CriticalSection *m_cs;						// +0x00
	bool m_locked;								// +0x04
};
// SIZE 0x8

#endif
