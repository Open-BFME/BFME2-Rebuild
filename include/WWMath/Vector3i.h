// Portions derived from Command & Conquer Generals Zero Hour, (c) Electronic Arts Inc., GPL-3.0-or-later; see LICENSE and NOTICE.
// Vector3i.h: WWMath's small integer triples.
// Target facts: MeshGeometryClass::Compute_Plane 0x0016ACC0 indexes the polygon
// array at a 6-byte stride and reads three unsigned 16-bit vertex indices at
// +0, +2 and +4.
#pragma once
#ifndef BFME2_VECTOR3I_H
#define BFME2_VECTOR3I_H

class Vector3i16
{
public:
	unsigned short I;	// +0x0
	unsigned short J;	// +0x2
	unsigned short K;	// +0x4

	// Zero Hour's accessors return references.
	const unsigned short &operator[](int n) const { return (&I)[n]; }
	unsigned short &operator[](int n) { return (&I)[n]; }
};

typedef char Vector3i16Size[(sizeof(Vector3i16) == 0x6) ? 1 : -1];

typedef Vector3i16 TriIndex;

// Zero Hour's vector3i.h: three 32-bit indices. AABTreeBuilderClass::Build_AABTree 0x00199E30 takes an array of them
// (map ?Build_AABTree@AABTreeBuilderClass@@QAEXHPAVVector3i@@HPAVVector3@@@Z) and narrows each to a Vector3i16.
class Vector3i
{
public:
	int I;
	int J;
	int K;
};

#endif
