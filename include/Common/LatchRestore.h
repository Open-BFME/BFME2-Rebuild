// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// LatchRestore.h: sets a variable for the length of a scope and puts the old value back afterwards.
// Target facts: ScriptEngine::evaluateConditions 0x00209748 builds two of these on its frame (vptr, the saved
// value, the address of the variable: 0xC bytes) and restores both at the end through the inlined destructor.
// The class has a virtual destructor (each instantiation has its own scalar deleting destructor, e.g. the
// <bool> one at 0x00051A46 and the <int> one at 0x002E7F86).
#pragma once
#ifndef BFME2_LATCHRESTORE_H
#define BFME2_LATCHRESTORE_H

template <class T>
class LatchRestore
{
protected:
	T valueToRestore;
	T &whereToRestore;

public:
	LatchRestore(T &dest, const T &src) : whereToRestore(dest)
	{
		valueToRestore = dest;
		dest = src;
	}

	virtual ~LatchRestore()
	{
		whereToRestore = valueToRestore;
	}
};

#endif
