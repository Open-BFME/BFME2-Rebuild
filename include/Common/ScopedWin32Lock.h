// ScopedWin32Lock.h: scope guard over a raw Win32 critical section (not sys_util's ScopedCriticalSection, which guards a CriticalSection object). Retail's
// GameWindowTransitionsHandler::update 0x001DBEB6, setGroup 0x001DC252, reverse 0x001DC345,
// remove 0x001DC42C and unlock 0x001DBBC8 keep the section's address in an unwind-registered
// local (entered in the constructor, left in the destructor) where lock 0x001DBBAD and
// isFinished 0x001DBFEE call Enter/LeaveCriticalSection directly.
// Needs the Platform SDK (`// psdk` in the including source file).
#pragma once
#ifndef BFME2_SCOPEDWIN32LOCK_H
#define BFME2_SCOPEDWIN32LOCK_H

#include <windows.h>

class ScopedWin32Lock
{
public:
	ScopedWin32Lock(CRITICAL_SECTION *section) : m_section(section) { EnterCriticalSection(m_section); }
	~ScopedWin32Lock() { LeaveCriticalSection(m_section); }
private:
	CRITICAL_SECTION *m_section;
};

#endif
