// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// bound.h: WWLib's clamp template (WWVegas/WWLib/bound.h). WW3D::Normalize_Coordinates 0x001184D0 expands it twice.
#pragma once
#ifndef BFME2_BOUND_H
#define BFME2_BOUND_H

template <class T> inline T Bound(T original, T minval, T maxval)
{
	if (original < minval) return(minval);
	if (original > maxval) return(maxval);
	return(original);
}

#endif
