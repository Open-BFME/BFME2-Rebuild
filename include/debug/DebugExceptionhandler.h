// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// DebugExceptionhandler.h: the debug library's unhandled-exception handler (Zero Hour's debug_except.cpp, internal_except.h).
// Target facts (retail game.dat): all members are static (the class is only a namespace for the handler); GetExceptionType
// 0x0003C7A0 is public (`SA`), LogFPURegisters 0x0003CD30 and LogRegisters 0x0003DA20 private (`CA`), ExceptionFilter 0x0003DCD0
// public and stdcall (`SGJ`).
#pragma once
#ifndef BFME2_DEBUGEXCEPTIONHANDLER_H
#define BFME2_DEBUGEXCEPTIONHANDLER_H

class Debug;
struct _EXCEPTION_POINTERS;

class DebugExceptionhandler
{
public:
	// The exception name; `explanation` receives the long text (it must hold 512 bytes).
	static const char *GetExceptionType(struct _EXCEPTION_POINTERS *exptr, char *explanation);	// 0x0003C7A0
	static long __stdcall ExceptionFilter(struct _EXCEPTION_POINTERS *exptr);			// 0x0003DCD0

private:
	static void LogFPURegisters(Debug &dbg, struct _EXCEPTION_POINTERS *exptr);			// 0x0003CD30
	static void LogRegisters(Debug &dbg, struct _EXCEPTION_POINTERS *exptr);			// 0x0003DA20
};

#endif
