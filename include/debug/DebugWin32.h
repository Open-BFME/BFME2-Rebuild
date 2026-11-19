// The Win32 entry points the debug library's I/O classes call (retail imports them from kernel32 and user32; no
// Platform SDK header is involved). Declared once here for debug_io_net.cpp, debug_io_con.cpp and debug_io_flat.cpp.
#pragma once
#ifndef BFME2_DEBUGWIN32_H
#define BFME2_DEBUGWIN32_H

#include "Common/SystemTime.h"

#define INVALID_HANDLE_VALUE ((void *)-1)
#define GENERIC_READ 0x80000000UL
#define GENERIC_WRITE 0x40000000UL
#define OPEN_EXISTING 3
#define PIPE_READMODE_MESSAGE 2
#define PIPE_WAIT 0
#define PIPE_NOWAIT 1

extern "C" {
__declspec(dllimport) int __stdcall CloseHandle(void *handle);
__declspec(dllimport) int __stdcall ReadFile(void *file, void *buffer, unsigned long toRead, unsigned long *read, void *overlapped);
__declspec(dllimport) int __stdcall WriteFile(void *file, const void *buffer, unsigned long toWrite, unsigned long *written, void *overlapped);
__declspec(dllimport) void *__stdcall CreateFileA(const char *name, unsigned long access, unsigned long share, void *security, unsigned long disposition, unsigned long flags, void *templateFile);
__declspec(dllimport) int __stdcall SetNamedPipeHandleState(void *pipe, unsigned long *mode, unsigned long *maxCollect, unsigned long *collectTimeout);
__declspec(dllimport) int __stdcall GetComputerNameA(char *buffer, unsigned long *size);
__declspec(dllimport) int __cdecl wsprintfA(char *out, const char *format, ...);
}

// Console I/O (debug_io_con.cpp). Structures are the Win32 layouts.
struct COORD { short X; short Y; };
struct SMALL_RECT { short Left; short Top; short Right; short Bottom; };
struct CONSOLE_SCREEN_BUFFER_INFO { COORD dwSize; COORD dwCursorPosition; unsigned short wAttributes; SMALL_RECT srWindow; COORD dwMaximumWindowSize; };
struct CONSOLE_CURSOR_INFO { unsigned long dwSize; int bVisible; };
struct KEY_EVENT_RECORD
{
	int bKeyDown;
	unsigned short wRepeatCount;
	unsigned short wVirtualKeyCode;
	unsigned short wVirtualScanCode;
	union { unsigned short UnicodeChar; char AsciiChar; } uChar;
	unsigned long dwControlKeyState;
};
struct INPUT_RECORD
{
	unsigned short EventType;
	union { KEY_EVENT_RECORD KeyEvent; unsigned long pad[4]; } Event;
};
struct CHAR_INFO { union { unsigned short UnicodeChar; char AsciiChar; } Char; unsigned short Attributes; };
#define KEY_EVENT 1
#define STD_INPUT_HANDLE ((unsigned long)-10)
#define STD_OUTPUT_HANDLE ((unsigned long)-11)
#define BACKGROUND_BLUE 0x0010
#define BACKGROUND_GREEN 0x0020
#define BACKGROUND_RED 0x0040
#define BACKGROUND_INTENSITY 0x0080
#define FOREGROUND_BLUE 0x0001
#define FOREGROUND_GREEN 0x0002
#define FOREGROUND_RED 0x0004
#define FOREGROUND_INTENSITY 0x0008
#define SC_CLOSE 0xF060

extern "C" {
__declspec(dllimport) int __stdcall AllocConsole(void);
__declspec(dllimport) int __stdcall FreeConsole(void);
__declspec(dllimport) void *__stdcall GetStdHandle(unsigned long which);
__declspec(dllimport) int __stdcall SetConsoleMode(void *console, unsigned long mode);
__declspec(dllimport) int __stdcall GetConsoleScreenBufferInfo(void *console, CONSOLE_SCREEN_BUFFER_INFO *info);
__declspec(dllimport) int __stdcall SetConsoleCursorInfo(void *console, const CONSOLE_CURSOR_INFO *info);
__declspec(dllimport) int __stdcall SetConsoleCtrlHandler(int (__stdcall *handler)(int), int add);
__declspec(dllimport) unsigned long __stdcall GetConsoleTitleA(char *title, unsigned long size);
__declspec(dllimport) int __stdcall SetConsoleTitleA(const char *title);
__declspec(dllimport) void __stdcall Sleep(unsigned long ms);
__declspec(dllimport) void *__stdcall FindWindowA(const char *className, const char *windowName);
__declspec(dllimport) void *__stdcall GetSystemMenu(void *window, int revert);
__declspec(dllimport) int __stdcall DeleteMenu(void *menu, unsigned int position, unsigned int flags);
__declspec(dllimport) int __stdcall DrawMenuBar(void *window);
__declspec(dllimport) unsigned long __stdcall GetModuleFileNameA(void *module, char *name, unsigned long size);
__declspec(dllimport) int __stdcall GetNumberOfConsoleInputEvents(void *console, unsigned long *count);
__declspec(dllimport) int __stdcall ReadConsoleInputA(void *console, INPUT_RECORD *buffer, unsigned long length, unsigned long *read);
__declspec(dllimport) int __stdcall WriteConsoleOutputA(void *console, const CHAR_INFO *buffer, COORD size, COORD coord, SMALL_RECT *region);
__declspec(dllimport) unsigned long __stdcall GetTickCount(void);
__declspec(dllimport) int __stdcall SetConsoleWindowInfo(void *console, int absolute, const SMALL_RECT *window);
__declspec(dllimport) int __stdcall SetConsoleScreenBufferSize(void *console, COORD size);
}

// File copy, user and time queries (debug_io_flat.cpp).
#define FILE_ATTRIBUTE_NORMAL 0x80
#define FILE_FLAG_WRITE_THROUGH 0x80000000UL
#define CREATE_ALWAYS 2
#define ERROR_FILE_EXISTS 80

extern "C" {
__declspec(dllimport) int __stdcall CopyFileA(const char *existing, const char *newName, int failIfExists);
__declspec(dllimport) unsigned long __stdcall GetLastError(void);
__declspec(dllimport) unsigned long __stdcall GetEnvironmentVariableA(const char *name, char *buffer, unsigned long size);
__declspec(dllimport) int __stdcall GetUserNameA(char *buffer, unsigned long *size);
}

// Exception records (debug_except.cpp).
struct _FLOATING_SAVE_AREA
{
	unsigned long ControlWord;
	unsigned long StatusWord;
	unsigned long TagWord;
	unsigned long ErrorOffset;
	unsigned long ErrorSelector;
	unsigned long DataOffset;
	unsigned long DataSelector;
	unsigned char RegisterArea[80];
	unsigned long Cr0NpxState;
};
// The x86 thread context (winnt.h's CONTEXT, 0x2CC bytes): the debug library reads the integer, segment and FPU state.
struct _CONTEXT
{
	unsigned long ContextFlags;
	unsigned long Dr0, Dr1, Dr2, Dr3, Dr6, Dr7;
	_FLOATING_SAVE_AREA FloatSave;
	unsigned long SegGs, SegFs, SegEs, SegDs;
	unsigned long Edi, Esi, Ebx, Edx, Ecx, Eax;
	unsigned long Ebp, Eip, SegCs, EFlags, Esp, SegSs;
	unsigned char ExtendedRegisters[512];
};
struct _EXCEPTION_RECORD
{
	unsigned long ExceptionCode;
	unsigned long ExceptionFlags;
	struct _EXCEPTION_RECORD *ExceptionRecord;
	void *ExceptionAddress;
	unsigned long NumberParameters;
	unsigned long ExceptionInformation[15];
};
struct _EXCEPTION_POINTERS
{
	_EXCEPTION_RECORD *ExceptionRecord;
	_CONTEXT *ContextRecord;
};
#define EXCEPTION_ACCESS_VIOLATION 0xC0000005UL
#define EXCEPTION_ARRAY_BOUNDS_EXCEEDED 0xC000008CUL
#define EXCEPTION_BREAKPOINT 0x80000003UL
#define EXCEPTION_DATATYPE_MISALIGNMENT 0x80000002UL
#define EXCEPTION_FLT_DENORMAL_OPERAND 0xC000008DUL
#define EXCEPTION_FLT_DIVIDE_BY_ZERO 0xC000008EUL
#define EXCEPTION_FLT_INEXACT_RESULT 0xC000008FUL
#define EXCEPTION_FLT_INVALID_OPERATION 0xC0000090UL
#define EXCEPTION_FLT_OVERFLOW 0xC0000091UL
#define EXCEPTION_FLT_STACK_CHECK 0xC0000092UL
#define EXCEPTION_FLT_UNDERFLOW 0xC0000093UL
#define EXCEPTION_GUARD_PAGE 0x80000001UL
#define EXCEPTION_ILLEGAL_INSTRUCTION 0xC000001DUL
#define EXCEPTION_IN_PAGE_ERROR 0xC0000006UL
#define EXCEPTION_INT_DIVIDE_BY_ZERO 0xC0000094UL
#define EXCEPTION_INT_OVERFLOW 0xC0000095UL
#define EXCEPTION_INVALID_DISPOSITION 0xC0000026UL
#define EXCEPTION_INVALID_HANDLE 0xC0000008UL
#define EXCEPTION_NONCONTINUABLE_EXCEPTION 0xC0000025UL
#define EXCEPTION_PRIV_INSTRUCTION 0xC0000096UL
#define EXCEPTION_SINGLE_STEP 0x80000004UL
#define EXCEPTION_STACK_OVERFLOW 0xC00000FDUL

#endif
