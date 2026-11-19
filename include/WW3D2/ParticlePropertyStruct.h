// Portions derived from Command & Conquer Generals and Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// ParticlePropertyStruct.h: a particle property defined by keyframes (Zero Hour's part_emt.h): the property is typically
// defined by property / age pairs with linear interpolation between them. Target facts: ParticleEmitterDefClass's constructor
// 0x001B1350 clears five dwords for each float property and nine for the Vector3 one, so the record is Start, Rand, the
// keyframe count and two pointers.
#pragma once
#ifndef BFME2_PARTICLEPROPERTYSTRUCT_H
#define BFME2_PARTICLEPROPERTYSTRUCT_H

#include <stddef.h>
#include <string.h>

template <class T> struct ParticlePropertyStruct
{
	T Start;
	T Rand;
	unsigned int NumKeyFrames;
	float *KeyTimes;
	T *Values;
};

// Utility function for copying the contents of one keyframe struct into another.
template <class T> inline void Copy_Emitter_Property_Struct(ParticlePropertyStruct<T> &dest, const ParticlePropertyStruct<T> &src)
{
	dest.Start = src.Start;
	dest.Rand = src.Rand;
	dest.NumKeyFrames = src.NumKeyFrames;
	dest.KeyTimes = NULL;
	dest.Values = NULL;

	if (dest.NumKeyFrames > 0) {
		dest.KeyTimes = new float[dest.NumKeyFrames];
		dest.Values = new T[dest.NumKeyFrames];
		::memcpy (dest.KeyTimes, src.KeyTimes, sizeof (float) * dest.NumKeyFrames);
		::memcpy (dest.Values, src.Values, sizeof (T) * dest.NumKeyFrames);
	}
}

#endif
