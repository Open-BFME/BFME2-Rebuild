// SystemTime.h: the Win32 local-time record, declared here so game code need not include windows.h.
// Target facts: GetLocalTime is a stdcall import (retail UserPreferences 0x00535781 calls it through the
// import table with the address of a 16-byte record and passes the record by value to the date
// formatter 0x002DBFAD, whose map name spells the parameter `struct SYSTEMTIME`).
#pragma once
#ifndef BFME2_SYSTEMTIME_H
#define BFME2_SYSTEMTIME_H

struct SYSTEMTIME
{
	unsigned short wYear, wMonth, wDayOfWeek, wDay, wHour, wMinute, wSecond, wMilliseconds;
};

extern "C" __declspec(dllimport) void __stdcall GetLocalTime(SYSTEMTIME *time);

#endif
