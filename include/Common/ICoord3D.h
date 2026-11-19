// ICoord3D.h: SAGE's three-int coordinate (Zero Hour BaseType.h ICoord3D).
// Target facts: XferSaveAsText 0x0060DA49 reads x +0, y +4, z +8 as ints.
// Retail exports ??0ICoord3D@@QAE@XZ (folded onto the shared `mov eax,ecx; ret`
// 0x0047A6A9) and ??0ICoord3D@@QAE@HHH@Z (0x005CC49A, a COMDAT with two direct
// callers): both constructors are user-declared inline in the header.
// zero (0x00004DCF) is exported with no direct callers: inline. length
// (0x00004D95, also uncalled) is left undeclared until its body is recovered.
#pragma once
#ifndef BFME2_ICOORD3D_H
#define BFME2_ICOORD3D_H

#include "Common/GameType.h"

struct ICoord3D
{
	Int x;		// 0x0
	Int y;		// 0x4
	Int z;		// 0x8

	ICoord3D() {}
	ICoord3D(Int ax, Int ay, Int az) : x(ax), y(ay), z(az) {}

	void zero() { x = 0; y = 0; z = 0; }
};
// SIZE 0xC
typedef char ICoord3DSize[(sizeof(ICoord3D) == 0xC) ? 1 : -1];

#endif
