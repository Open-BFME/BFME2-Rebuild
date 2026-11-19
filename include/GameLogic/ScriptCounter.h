// ScriptCounter.h: one named script counter (the value TheScriptEngine's counter
// map holds). SIZE 8.
// Target facts (code RVAs):
//  - No virtual functions and no base: 0x00208792/95/98 initialize the value and
//    the two flags and no vptr is stored.
//  - 0x00206C13 allocates a 0x20-byte tree node and 0x002087E5 returns node+0x18,
//    so sizeof(ScriptCounter) is 8 aligned to 4; the pair helpers 0x00204A93-9B
//    and 0x00204B20-29 copy the whole eight-byte value.
//  - 0x00208FC9-0x00208FF8 compare the signed integer at +0. setTimer stores it
//    at 0x002095BD and the countdown flag at +4 (0x002095BF); pauseTimer clears
//    +4 at 0x002095F6 and restartTimer sets it at 0x00209630. setTimer's
//    floating-duration path sets the millisecond flag at +5 (0x0020957D) and its
//    integer path clears it at 0x002095BA. update tests +4 and decrements +0 at
//    0x0020D154-62, and tests node+0x1D at 0x0020D32C to pick the seconds
//    formatting that 0x00205560-80 uses.
//  - No standalone constructor, destructor, copy or assignment row exists.
#pragma once
#ifndef BFME2_SCRIPTCOUNTER_H
#define BFME2_SCRIPTCOUNTER_H

struct ScriptCounter
{
	int m_value;			// +0x00
	bool m_isCountdownTimer;	// +0x04
	bool m_isMillisecondTimer;	// +0x05

	ScriptCounter() : m_value(0), m_isCountdownTimer(false), m_isMillisecondTimer(false) {}
};
// SIZE 8

// One named script flag (the value of TheScriptEngine's flag map; keyed like the counters).  Target facts: the
// temporary 0x002088E5 (bfmeFlagForWrite) is one byte set to 0 before it goes into the map value pair.
struct ScriptFlag
{
	bool m_value;			// +0x00

	ScriptFlag() : m_value(false) {}
};
// SIZE 1

#endif
