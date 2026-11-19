// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// always.h: WWLib's by-value max template (WWVegas/WWLib/always.h; only max so far: min joins it when a row needs it).
// part_ldr.h's FLOAT_TO_BYTE clamps through max(fval, 0.0F): ParticleEmitterDefClass::Save_Props 0x001B1970
// keeps the 0.0f in xmm1 for its three clamps only with this by-value if/else form (_STL::max by reference does not).
#pragma once
#ifndef BFME2_ALWAYS_H
#define BFME2_ALWAYS_H

#ifdef max
#undef max
#endif

template <class T> T max(T a,T b)
{
	if (a>b) {
		return a;
	} else {
		return b;
	}
}

#endif
