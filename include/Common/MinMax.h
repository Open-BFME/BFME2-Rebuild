// MinMax.h: the reference-returning min and max templates that retail's script action code is built with.
// Target facts: ScriptActions::doTeamSetHealth 0x003C301D clamps a Real with two statements,
// `health = min(health, 100.0f); health = max(health, 0.0f);`, and retail's code stores each operand in a stack
// slot and selects an address (`lea edx,[ebp+8]` / `lea edx,[ebp+0xC]`), which only the const-reference
// forms below produce: min picks a when `a < b` (otherwise b), max picks b when `a < b` (otherwise a).
// Both are inline: out of line the int instantiations are called instead of expanded (UnicodeString.cpp's
// IntRange::combine shows the same).
// The retail spelling is min/max; here they are minOf/maxOf because WWLib's always.h (by-value max, visible in
// every unit that includes a WW3D2 header) would make a call to `max` ambiguous. Other units keep private copies
// with other comparison forms (`a > b ? a : b`); they are not this header's.
#pragma once
#ifndef BFME2_MINMAX_H
#define BFME2_MINMAX_H

template <class T> inline const T &minOf(const T &a, const T &b)
{
	return a < b ? a : b;
}

template <class T> inline const T &maxOf(const T &a, const T &b)
{
	return a < b ? b : a;
}

#endif
