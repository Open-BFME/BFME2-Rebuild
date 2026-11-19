// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// systimer.h: WWLib's millisecond clock over timeGetTime (WWVegas/WWLib/systimer.h).
// SIZE 0x08: StartTime +0, WrapAdd +4; the one instance is SystemTime (VA 0x00E095A4).
// Target facts (retail game.dat): the constructor 0x0061AD40 zeroes both words and
// calls timeBeginPeriod(1); the destructor 0x0061AD60 calls timeEndPeriod(1); Reset
// 0x0061AD70 stores timeGetTime() as StartTime and its negation as WrapAdd.
// Get is inline: it resets once (a function-local flag at VA 0x00E07BFD), reads
// timeGetTime and returns time-StartTime, or time+WrapAdd when the clock wrapped.
// Is_Getting_Late 0x0061AD90 is the same read compared with 0x6FFFFFFF.
// Donor facts: names follow Zero Hour's WWLib/systimer.h (GPL), naming only.
#pragma once
#ifndef BFME2_SYSTIMER_H
#define BFME2_SYSTIMER_H

extern "C" __declspec(dllimport) unsigned long __stdcall timeGetTime(void);	// IAT 0x00BBA918

class SysTimeClass
{
public:
	SysTimeClass();				// 0x0061AD40
	~SysTimeClass();			// 0x0061AD60

	__forceinline unsigned long Get()
	{
		static bool is_init = false;
		if (!is_init) {
			Reset();
			is_init = true;
		}
		unsigned long time = timeGetTime();
		if (time > StartTime) return time - StartTime;
		return time + WrapAdd;
	}
	unsigned long operator()() { return Get(); }
	void Reset();				// 0x0061AD70
	bool Is_Getting_Late();		// 0x0061AD90

private:
	unsigned long StartTime;		// +0x00
	unsigned long WrapAdd;		// +0x04
};
// SIZE 0x08

extern SysTimeClass SystemTime;		// VA 0x00E095A4

#endif
