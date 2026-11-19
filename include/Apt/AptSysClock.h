#pragma once
// SIZE 0x20, plain struct (the map's mangled names use U for AptSysClock). Eight
// ints. Evidence: AptDate (+0x20 local clock, +0x40 UTC clock) exposes them
// through its getX/setX natives, each of which reads or writes one slot
// (getSeconds +0, getMinutes +4, getHours +8, getDate +0x10, getMonth +0x14,
// getFullYear/getYear +0x18, getMilliseconds +0x1C). The slot at +0x0C is neither
// read nor written by any recovered body (the weekday is computed by
// AptDate::getDayOfWeek); its name is the donor's spelling.
// The platform callback cell at VA 0x00E177B8 fills one: (clock, isUTC).
struct AptSysClock
{
    int Second;       // 0x00
    int Minute;       // 0x04
    int Hour;         // 0x08
    int Day;          // 0x0C
    int Date;         // 0x10
    int Month;        // 0x14
    int Year;         // 0x18
    int Milliseconds; // 0x1C
};

typedef char AptSysClockSize[(sizeof(AptSysClock) == 0x20) ? 1 : -1];
